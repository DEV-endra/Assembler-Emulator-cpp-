// Name : DEVENDRA CHAND
// Roll Number : 2301CS91
// Declaration of authorship : I hereby certify that the source code I am submitting is entirely my own original work except, where otherwise indicated.

#include <bits/stdc++.h>
#include "VariadicTable.h" // CUSTOM LIBRARY BEING USED FOR TABULAR DISPLAY
using namespace std;
#define pb push_back
#define all(a) (a).begin(), (a).end()
vector<string> commands = {"EXECUTE", "MEMORY", "REGISTERS", "CLEAR", "STOP"};
int REG_A, REG_B, PROG_CNT, STACK_POINT; // REGISTERS
vector<int> MEMORY(20000000, 0);         // MEMORY
map<string, int> get_opcode;
int ORIGINAL;
void load_op_set()
{
    get_opcode["00"] = 0;  // ldc  load constant
    get_opcode["01"] = 1;  // adc  add constant
    get_opcode["02"] = 2;  // ldl  load local
    get_opcode["03"] = 3;  // stl  store local
    get_opcode["04"] = 4;  // ldnl  load non local
    get_opcode["05"] = 5;  // stnl  store non local
    get_opcode["06"] = 6;  // add  addition
    get_opcode["07"] = 7;  // sub  subtraction
    get_opcode["08"] = 8;  // shl  shift left
    get_opcode["09"] = 9;  // shr  shift right
    get_opcode["0A"] = 10; // adj  adjust STACK pointer
    get_opcode["0B"] = 11; // a2sp  transfer A to SP
    get_opcode["0C"] = 12; // sp2a  transfer SP to A
    get_opcode["0D"] = 13; // call  call procedure
    get_opcode["0E"] = 14; // return  return procedure
    get_opcode["0F"] = 15; // brz  branch if zero
    get_opcode["10"] = 16; // brlz  branch if less than zero
    get_opcode["11"] = 17; // br  branch
    get_opcode["12"] = 18; // HALT  stop the EMULATOR

    STACK_POINT = 0;
    PROG_CNT = 0;
}

vector<string> instruct_set;

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
int value(string s1)
{
    unsigned int result = stoul(s1.substr(0, 6), nullptr, 16);
    if (result & 0x800000)
    {
        result = result - 0x1000000;
    }
    return result;
}

void memory_dump(string s)
{
    string ss = "";
    for (auto it : s)
    {
        if (it == '.')
        {
            break;
        }
        ss += it;
    }
    ss += ".txt";
    fstream memdump;
    memdump.open(ss, ios::out);
    if (memdump.is_open())
    {
        for (int i = 0; i < 100; i++)
        {
            memdump << integer_to_hexa(to_string(MEMORY[i]), 8) << "    ";
        }
        memdump << "\n";
    }
    memdump << "\n";
    memdump << "REG_A=" << REG_A << "   " << "REG_B=" << REG_B << "    " << "PROG_CNT=" << PROG_CNT << "    " << "STACK_POINT=" << STACK_POINT;
    memdump << "\n";
}

int main()
{
    load_op_set();
    string s1, s2;
    cout << "ENTER THE OBJECT FILE :";
    fstream objfile;
    cin >> s1;
    objfile.open(s1, ios::in);
    if (objfile.is_open())
    {
        string temp_instruct;
        // reading line by line
        int ct = 0;
        while (getline(objfile, temp_instruct))
        {
            ct++;
            if (ct > 2)
            {                                          // INSTRUCTIONS START FROM LINE 3
                instruct_set.push_back(temp_instruct); // STORING INSTRUCTIONS  IN INSTRUCT SET
            }
            else if (ct == 1) // LINE 1 WILL CONTAIN DATA VALUES
            {
                if (temp_instruct.size() != 0)
                {
                    for (int i = 0; i < temp_instruct.size() - 7; i += 8)
                    {
                        MEMORY[STACK_POINT] = stoi(temp_instruct.substr(i, 8), nullptr, 16);
                        STACK_POINT++;
                    }
                }
            }
            // LINE 2 WILL REMAIN EMPTY
        }
        ORIGINAL = STACK_POINT;
    }
    else
    {
        cout << "ERROR : FILE CAN'T BE OPENED";
        return 0;
    }
    while (true)
    {
        VariadicTable<string, string> dev({"EXECUTE", "TO START EXECUTION"}, 3);
        dev.addRow("MEMORY", "SHOW MEMORY");
        dev.addRow("REGISTERS", "SHOW REGISTER VALUES");
        dev.addRow("CLEAR", "CLEAR MEMORY BEFORE EXECUTION");
        dev.addRow("STOP", "STOPS THE SIMULATION");
        dev.print(std::cout);
        cin >> s2;
        string opera = s2;
        int flag = 0;
        for (auto it : commands)
        {
            if (it == opera)
            {
                flag = 1;
                break;
            }
        }
        if (flag == 0)
        { // UNKNOWN COMMAND ENTERED
            cout << "ERROR : COMMAND NOT FOUND " << endl;
        }
        else
        {
            if (opera == "EXECUTE") // FOR STARTING PROGRAM EXECUTION
            {
                PROG_CNT = 0;
                int STOP = instruct_set.size();
                int opcode;
                string curr_instruct;
                int flag = 0;
                while (PROG_CNT != STOP) // PROGRAM ENDS WHEN PC REACHES THE LAST VALUE
                {
                    cout << "PC=" << integer_to_hexa(to_string(PROG_CNT), 8) << " " << "A=" << REG_A << " " << "B=" << REG_B << " " << "SP=" << STACK_POINT << endl;
                    curr_instruct = instruct_set[PROG_CNT];
                    opcode = get_opcode[curr_instruct.substr(6)];
                    switch (opcode)
                    {
                    case 0: // ldc  load constant
                        REG_B = REG_A;
                        REG_A = value(curr_instruct);
                        break;
                    case 1: // adc  add constant
                        REG_A += value(curr_instruct);
                        break;
                    case 2: // ldl  load local
                        REG_B = REG_A;
                        REG_A = MEMORY[STACK_POINT + value(curr_instruct)];
                        break;
                    case 3: // stl  store local
                            // cout << STACK_POINT << " " << curr_instruct << endl;
                        MEMORY[STACK_POINT + value(curr_instruct)] = REG_A;
                        REG_A = REG_B;
                        break;
                    case 4: // ldnl  load non local
                        REG_A = MEMORY[REG_A + value(curr_instruct)];
                        break;
                    case 5: // stnl  store non local
                        MEMORY[REG_A + value(curr_instruct)] = REG_B;
                        break;
                    case 6: // add  addition
                        REG_A = REG_A + REG_B;
                        break;
                    case 7: // sub  subtraction
                        REG_A = REG_B - REG_A;
                        break;
                    case 8: // shl  shift left
                        REG_A = REG_B << REG_A;
                        break;
                    case 9: // shr  shift right
                        REG_A = REG_B >> REG_A;
                        break;
                    case 10: // adj  adjust STACK pointer
                        STACK_POINT += value(curr_instruct);
                        break;
                    case 11: // a2sp  transfer A to SP
                        STACK_POINT = REG_A;
                        REG_A = REG_B;
                        break;
                    case 12: // sp2a  transfer SP to A
                        REG_B = REG_A;
                        REG_A = STACK_POINT;
                        break;
                    case 13: //   call procedure
                        REG_B = REG_A;
                        REG_A = PROG_CNT;
                        PROG_CNT += value(curr_instruct);
                        break;
                    case 14: // return  return procedure
                        PROG_CNT = REG_A;
                        REG_A = REG_B;
                        break;
                    case 15:
                        if (REG_A == 0)
                        {
                            PROG_CNT += value(curr_instruct);
                        }
                        break;
                    case 16:
                        if (REG_A < 0)
                        {
                            PROG_CNT += value(curr_instruct);
                        }
                        break;
                    case 17:
                        PROG_CNT += value(curr_instruct);
                        break;
                    case 18:
                        PROG_CNT = STOP - 1;
                        break;
                    }

                    PROG_CNT++;
                }
            }
            else if (opera == "MEMORY") // SHOWS MEMORY SPACE
            {
                memory_dump(s1);
                VariadicTable<string, int, int, int, int, int, int, int, int, int, int> dev({"ADDRESS", "0000000", "0000001", "0000002", "0000003", "0000004", "0000005", "0000006", "0000007", "0000008", "0000009"}, 11);
                for (int i = 0; i < 100; i += 10)
                {
                    dev.addRow(integer_to_hexa(to_string(i), 8), MEMORY[i], MEMORY[i + 1], MEMORY[i + 2], MEMORY[i + 3], MEMORY[i + 4], MEMORY[i + 5], MEMORY[i + 6], MEMORY[i + 7], MEMORY[i + 8], MEMORY[i + 9]);
                }
                dev.print(std::cout);
            }
            else if (opera == "REGISTERS") // SHOWS REGISTER VALUES
            {
                VariadicTable<string, int> dev({"REGISTER", "VALUE"}, 4);
                dev.addRow("REGISTER A", REG_A);
                dev.addRow("REGISTER B", REG_B);
                dev.addRow("PROGRAM COUNTER", PROG_CNT);
                dev.addRow("STACK POINTER", STACK_POINT);
                dev.print(std::cout);
            }
            else if (opera == "CLEAR")
            {
                MEMORY.clear();
                REG_A = 0;
                REG_B = 0;
                PROG_CNT = 0;
                STACK_POINT = ORIGINAL;
            }
            else
            {
                break;
            }
        }
    }
    objfile.close();
}
