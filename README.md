# Assembler-Emulator-cpp

///////////////////////////////    Name : DEVENDRA CHAND     ///////////////////////////////
//////////////////////////////     Roll Number : 2301CS91    //////////////////////////////


THE ASSEMBLER:

1-TWO PASS ASSEMBLER WHERE SINGLE PASS DETECTS LABELS USED, ERRORS, AND WARNINGS IF ANY IN THE ASSEMBLY

2-HANDLES EXTRA SPACES BETWEEN OPERANDS AND EXTRA WHITESPACES EXCEPT FOR LABELS FOR WHICH SPACE IS NECESSARY AFTER SEMICOLON.

3-HANDLES COMMENTS IN THE SAME LINE AND SEPARATE LINES

4 CAN GET DATA IN DECIMAL, OCTAL, AND HEXADECIMAL 2'S COMPLEMENT

5-IT CAN DETECT THE FOLLOWING ERRORS:
   DUPLICATE LABEL
   INVALID LABEL NAME (ALLOWS FOR UNDERSCORE IN BEGINNING BUT DOESN'T ALLOW NUMERICS IN START OR OTHER SPECIAL CHARACTERS)
   MISSING OPERAND 
   MORE OPERANDS THAN EXPECTED (INCORRECT OPERAND FORMAT)

6-IT CAN DETECT THE FOLLOWING WARNINGS:
   INFINITE LOOP

7-KEEPS TRACK OF MNEMONIC, OPCODES, AND POSSIBLE OPERANDS, LABELS

8-GENERATES A LOG FILE

9-GENERATES A LISTING FILE  (ONLY IF NO ERRORS)

10-GENERATES AN OBJECT FILE  (ONLY IF NO ERRORS)

11- IMPLEMENTS DATA INSTRUCTION :
    THE FIRST LINE OF EACH OBJECT FILE CONTAINS 8BIT HEXA VALUES THAT ARE NEEDED TO BE ASSIGNED IN THE MEMORY,               
    FOLLOWING LINE IS LEFT EMPTY AND THEN THE MACHINE CODE FOLLOWS


THE EMULATOR::

1-LOADS THE OBJECT FILE

2-THE FIRST STEP INVOLVES ASSIGNING MEMORY TO THE DATA VALUES (IF ANY DATA INSTRUCTIONS ARE USED)

3-USES CUSTOM "VariadicTable.h" LIBRARY FOR PRINTING THE VALUES IN TABULAR FORMAT

4-GENERATES A TEXT FILE CONTAINING MEMORY DUMP 

5-EXECUTE - EXECUTES THE MACHINE CODE

6-MEMORY  - SHOWS THE MEMORY IN TABULAR FORM 
             (MEMORY IS ASSUMED TO BE A VECTOR OF 20000000 ELEMENTS WHERE THE INDEX OF EACH ELEMENT REPRESENTS THE ADDRESS)
             (MEMORY ADDRESS STARTS FROM 00000000, ALL MEMORY VALUES ARE DISPLAYED IN DECIMAL NUMBERS) 
             (BUT THE MEMORY DUMP IS SHOWN IN HEXADECIMAL FORMAT)

7- REGISTERS - SHOWS THE REGISTER VALUES

8- CLEAR - RESETS THE MEMORY AND REGISTERS

9- STOP -  STOPS THE SIMULATION 
