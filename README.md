# 2023 Spring Operating Systems (ITP30002-02)
## Homework 2 - CIMIN: Crashing Input Minimizer
### 21700331 Jaeho Bae, 22100768 Narin Han

**How to build and run the program**

In order to build and run 'cimin' to check the four given cases, give a command in each of the buggy program’s directory name after 'make'. So for example, 'jsmn' cases will run with the command 'make jsmn'. This will build the buggy programs as well as cimin.

Hidden commands are followed:

./cimin -o output -m heap-buffer-overflow -i jsmn/testcases/crash.json jsmn/jsondump

./cimin -o output -m SEGV -i libxml2/testcases/crash.json -- libxml2/xmllint --recover --postvalid

./cimin -o output -m time -i balance/testcases/fail balance/balance

./cimin -o output -m MemorySanitizer -i libpng/crash.png libpng/libpng/test_pngfix 
