// Name : DEVENDRA CHAND     
// Roll Number : 2301CS91   
// Declaration of authorship : I hereby certify that the source code I am submitting is entirely my own original work except, where otherwise indicated.

#include <bits/stdc++.h>
using namespace std;
#define pb push_back
#define all(a) (a).begin(), (a).end()

// instruction set    //   format : ["instruction"]={opcode,0/1/2}
// 0: no operand , 1:one operand , 2:offset
map<string, pair<int, int>> ins;
vector<pair<int, string>> errors;
vector<pair<int, string>> warnings;
map<string, bool> dupcheck;
map<int, pair<int, int>> mp;                       // FIRST: LABEL OR NOT // SECOND: INSTRUCTION OR NOT
vector<pair<string, string>> instruction(6000000); // will store instruction if any
vector<string> label(6000000);                     // will store label if any
map<string, int> label_address;
map<int, string> machine_code;
map<int, int> num_to_pc;
map<int, string> source;
vector<string> data_instruct;
string filename;
int pc = 0;
void opcodes()
{
    ins["add"] = {6, 0};
    ins["sub"] = {7, 0};
    ins["shl"] = {8, 0};
    ins["shr"] = {9, 0};
    ins["a2sp"] = {11, 0};
    ins["sp2a"] = {12, 0};
    ins["return"] = {14, 0};
    ins["HALT"] = {18, 0};
    ins["data"] = {-1, 1};
    ins["SET"] = {-2, 1};
    ins["ldc"] = {0, 1};
    ins["adj"] = {10, 1};
    ins["adc"] = {1, 1};
    ins["ldl"] = {2, 2};
    ins["stl"] = {3, 2};
    ins["ldnl"] = {4, 2};
    ins["stnl"] = {5, 2};
    ins["call"] = {13, 2};
    ins["brz"] = {15, 2};
    ins["brlz"] = {16, 2};
    ins["br"] = {17, 2};
}

bool is_valid(int arg_cnt, vector<string> arg)
{
    string dev = arg[1];

    if (dev.find("asm") == -1)
    {
        cout << "ERROR: INCORRECT FILE FORMAT" << endl;
    }
}

bool valid_label(string lab)
{
    if (isdigit(lab[0]))
    {
        return false;
    }
    for (int i = 0; i < lab.size(); i++)
    {
        if ((!isalnum(lab[i])) && (lab[i] != '_'))
        {
            return false;
        }
    }
    return true;
}

// first pass to identify labels and detect common assembly errors and warnings
void firstpass(string line, int num)
{
    string st = "";
    for (int i = 0; i < line.size(); i++)
    {
        if (line[i] == ';') // Removing Comments
            break;
        st = st + line[i];
    }
    line = st;

    if (!line.empty())
    {
        // STORING VALID LABEL NAMES IF ANY
        string lab = "";
        int flag = 0;
        for (int i = 0; i < line.size(); i++)
        {
            if (line[i] == ' ' || line[i] == '\t')
                continue;
            if (line[i] == ':')
            {
                flag = 1;
                break;
            }
            lab += line[i];
        }

        mp[num].second = 0;
        mp[num].first = 0;
        map<string, int> infinite;
        if (flag == 1) // label detected
        {
            if (valid_label(lab))
            {
                infinite[lab]++;
                if (!dupcheck[lab])
                {
                    mp[num].first = 1;
                    label[num] = lab;
                    dupcheck[lab] = 1;
                    label_address[lab] = pc;
                }
                else
                {
                    errors.pb({num, "REDECLARATION OF LABEL"});
                }
            }
            else
            {
                errors.push_back({num, "INVALID LABEL NAME"});
            }
        }

        int ind = 0;
        if (flag == 1) // if label is there we need to search after the label declaration ends
        {
            for (int i = 0; i < line.size(); i++)
            {
                if (line[i] == ':')
                {
                    ind = i + 1;
                    break;
                }
            }
        }

        // NOW CHECKING FOR INSTRUCTIONS
        vector<string> instruct;
        string temp = "";
        for (int i = ind; i < line.size(); i++)
        {
            if (line[i] == ' ' || line[i] == '\t') // SKIP THE WHITESPACES OR TAB
            {
                if (!temp.empty())
                {
                    instruct.push_back(temp);
                    temp = "";
                }
                continue;
            }
            temp += line[i];
        }
        if (!temp.empty())
        {
            instruct.push_back(temp);
        }

        if (!instruct.empty())
        {
            if (ins[instruct[0]].second == 0) // INSTRUCTION WITH NO OPERAND
            {
                if (instruct.size() != 1) //
                {
                    errors.pb({num, "WRONG OPERAND TYPE"});
                }
                else
                {
                    mp[num].second = 1;
                    instruction[num].first = instruct[0];
                    instruction[num].second = "";
                }
            }
            else if (ins[instruct[0]].second == 1) // INSTRUCTION WITH SINGLE OPERAND
            {
                if (instruct.size() != 2) //
                {
                    errors.pb({num, "WRONG OPERAND TYPE"});
                }
                else
                {
                    if (infinite[instruct[1]] && instruct[0] == "br") // WARNING FOR INFINITE LOOP
                    {
                        /// cout << "dc " << endl;
                        warnings.pb({num, "INFINITE LOOP WILL EXIST"});
                    }
                    mp[num].second = 1;
                    instruction[num].first = instruct[0];
                    instruction[num].second = instruct[1];
                }
            }
            else // INSTRUCTION WITH OFFSET
            {
                if (instruct.size() != 2) //
                {
                    errors.pb({num, "WRONG OPERAND TYPE"});
                }
                else
                {

                    if (infinite[instruct[1]] && instruct[0] == "br") // WARNING FOR INFINITE LOOP
                    {
                        warnings.pb({num, "INFINITE LOOP WILL EXIST"});
                    }
                    mp[num].second = 1;
                    instruction[num].first = instruct[0];
                    instruction[num].second = instruct[1];
                }
            }
        }
        num_to_pc[num] = pc;
        if (instruction[num].first != "data")
        {
            if (mp[num].second == 1)
            {
                pc++; // INCREMENTING PROGRAM COUNTER IF AN INSTRUCTION WAS FOUND IN THE LINE
            }
        }
    }
    else
    {
        num_to_pc[num] = pc;
        mp[num] = {0, 0};
        // empty lines would be ignored
        // No warnings will be displayed
    }
}

string integer_to_hexa(string numberStr, int n)
{
    int number = stoi(numberStr);
    const char hexDigits[] = "0123456789ABCDEF";
    string hexResult;
    for (int i = 0; i < n; ++i)
    {
        char hexDigit = hexDigits[number & 0xF];
        hexResult += hexDigit;
        number >>= 4;
    }
    reverse(hexResult.begin(), hexResult.end());
    return hexResult;
}

string convert_to_decimal(string inputStr)
{
    int base = 10;
    int startIdx = 0;
    if (inputStr.substr(0, 2) == "0x")
    {
        base = 16;
        startIdx = 2;
    }
    else if (inputStr.substr(0, 2) == "0o")
    {
        base = 8;
        startIdx = 2;
    }
    int number = stoi(inputStr.substr(startIdx), nullptr, base);
    return to_string(number);
}

string operand_to_hexa(string operand, int length, int line_num)
{
    if (valid_label(operand) >= 1) // OPERAND IS A LABEL SO TAKING ITS ADDRESS AS THE VALUE
    {
        int dev;
        if (instruction[line_num].first == "br" | instruction[line_num].first == "brz" | instruction[line_num].first == "brlz" | instruction[line_num].first == "call")
        {                                                           // FOR BRANCHING
            dev = label_address[operand] - 1 - num_to_pc[line_num]; // DESTINATION ADDRESS -1 - CURRENT ADDRESS
        }
        else
        { // FOR OTHER INSTRUCTIONS
            dev = label_address[operand];
        }
        return integer_to_hexa(to_string(dev), 6);
    }
    else if ((operand.substr(0, 2)) == "0o" || (operand.substr(0, 2)) == "0x") // OPERAND IS OCTAL OR HEXA
    {
        operand = convert_to_decimal(operand);
        return integer_to_hexa(operand, length);
    }
    else
    { // cout << operand << endl;
        return integer_to_hexa(operand, length);
    }
}
void secondpass() // CONVERT ASSEMBLY INTO MACHINE CODE
{
    for (auto it : mp)
    {
        if ((it.second).second == 1)
        {
            int temp = it.first;
            //   cout << instruction[temp].second << " ";
            string command = instruction[temp].first;
            string mach_code = "";
            if (ins[command].second == 0) // NO OPERAND
            {
                mach_code = "000000" + integer_to_hexa(to_string(ins[command].first), 2);
            }
            else if (ins[command].second == 1) // SINGLE VALUE OPERAND
            {
                if (ins[command].first >= 0)
                {
                    mach_code = operand_to_hexa(instruction[temp].second, 6, temp) + integer_to_hexa(to_string(ins[command].first), 2);
                }
                else // FOR SET AND DATA INSTRUCTIONS NO OPCODES ARE THERE
                {
                    if (command == "data")
                    {
                        //  mach_code = "        ";
                        data_instruct.push_back(operand_to_hexa(instruction[temp].second, 8, temp)); // complete 8 hexa digits are there for operand
                    }
                    else
                    {
                        mach_code = operand_to_hexa(instruction[temp].second, 8, temp); // complete 8 hexa digits are there for operand
                    }
                }
            }
            else // OFFSET
            {
                mach_code = operand_to_hexa(instruction[temp].second, 6, temp) + integer_to_hexa(to_string(ins[command].first), 2);
            }

            if (!mach_code.empty())
            {
                machine_code[it.first] = mach_code;
            }
            else
            {
                mp[temp].second = 0;
            }
        }
    }
}
void print_warnings()
{
    if (!warnings.empty())
    {
        for (int i = 0; i < warnings.size(); i++)
        {
            cout << "LINE " << warnings[i].first << ":" << warnings[i].second << endl;
        }
    }
}

void print_errors()
{
    if (!errors.empty())
    {
        for (int i = 0; i < errors.size(); i++)
        {
            cout << "LINE " << errors[i].first << ":" << errors[i].second << endl;
        }
    }
}

void write_log()
{
    string s = filename;
    s += ".log";
    fstream logfile;
    logfile.open(s, ios::out);

    if (logfile.is_open())
    {
        if (!warnings.empty())
        {
            logfile << "WARNINGS";
            logfile << "\n";
            for (auto it : warnings)
            {
                logfile << "LINE ";
                logfile << it.first << " ";
                logfile << it.second;
                logfile << "\n";
            }
        }

        logfile << "ERRORS";
        logfile << "\n";
        for (auto it : errors)
        {

            logfile << "LINE ";
            logfile << it.first << " ";
            logfile << it.second;
            logfile << "\n";
        }
    }
    logfile.close();
    return;
}

void write_list()
{
    fstream listing_file;
    string s = filename;
    s += ".lst";
    listing_file.open(s, ios::out);
    if (listing_file.is_open())
    {
        for (auto it : mp)
        {
            if (it.second.second == 0 && it.second.first == 0) // NOTHING IS THERE
            {
                listing_file << integer_to_hexa(to_string(num_to_pc[it.first]), 8) << " ";
                listing_file << "        " << " ";
                listing_file << source[it.first];
                listing_file << "\n";
            }
            else if ((it.second.second == 0) && (it.second.first == 1)) // ONLY LABEL IS PRESENT
            {
                listing_file << integer_to_hexa(to_string(num_to_pc[it.first]), 8) << " ";
                listing_file << "        " << " ";
                listing_file << source[it.first];
                listing_file << "\n";
            }
            else if (it.second.second == 1 && it.second.first == 0) // INSTRUCTION
            {
                listing_file << integer_to_hexa(to_string(num_to_pc[it.first]), 8) << " ";
                listing_file << machine_code[it.first] << " ";
                listing_file << source[it.first];
                listing_file << "\n";
            }
            else //  LABEL AND INSTRUCTION
            {
                listing_file << integer_to_hexa(to_string(num_to_pc[it.first]), 8) << " ";
                listing_file << machine_code[it.first] << " ";
                listing_file << source[it.first];
                listing_file << "\n";
            }
        }
    }
    listing_file.close();
    return;
}

void write_obj()
{
    fstream objfile;
    string s = filename;
    s += ".o";
    objfile.open(s, ios::out);
    if (!data_instruct.empty())
    {
        for (auto values : data_instruct)
        {
            objfile << values;
        }
    }
    objfile << "\n"; // KEEPING THE FIRST TWO LINES ALWAYS FOR DATA VALUES THAT NEEDED TO BE STORED
    objfile << "\n";
    for (auto it : mp)
    {
        if (it.second.second == 1)
        {
            objfile << machine_code[it.first];
            objfile << "\n";
        }
    }
    objfile.close();
    return;
}

int main(int argu_cnt, char *argv[])
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    opcodes();

    // converting input into a vector of strings
    vector<string> arg(argv, argv + argu_cnt);

    // checking whether input file format is correct or not
    is_valid(argu_cnt, arg);

    // opening the input file
    fstream file_pointer;
    file_pointer.open(argv[1], ios::in);

    // extracting the filename
    filename = "";
    int ind;
    for (int i = 0; i < arg[1].size(); i++)
    {
        if (arg[1][i] == '.')
        {
            ind = i;
            break;
        }
    }
    for (int i = 0; i < ind; i++)
    {
        filename += arg[1][i];
    }

    if (file_pointer.is_open())
    {
        string curr_line;
        int lno = 1;
        // reading line by line
        while (getline(file_pointer, curr_line))
        {
            // first pass to identify labels and detect common assembly errors and warnings
            firstpass(curr_line, lno);
            source[lno] = curr_line;
            lno++;
        }
    }
    else
    {
        errors.pb({0, ".asm FILE NOT ACCESSIBLE"});
    }
    secondpass();
    write_log(); // WRITITNG ERRRORS AND WARNINGS IN LOG FILE

    if (errors.empty()) // CREATING OBJECT AND LISTING FILE IF NO ERRORS FOUND
    {
        write_list();
        write_obj();
    }
    return 0;
}