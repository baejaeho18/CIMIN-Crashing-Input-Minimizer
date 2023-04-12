# 2023 Spring Operating Systems (ITP30002-02)
## Homework 2 - CIMIN: Crashing Input Minimizer
### 21700331 Jaeho Bae, 22100768 Narin Han

**How to build and run the program**

In order to build and run 'cimin' to check the four given cases, give a command in each of the buggy program’s directory name after 'make'. So for example, 'jsmn' cases will run with the command 'make jsmn'. This will first clean up already built file if it exists and build the buggy programs as well as cimin.

Hidden commands are followed:

clean : 
  rm -f cimin
  
jsmn : clean cimin
  cd jsmn && ./build.sh
  cd jsmn && ../cimin -i testcases/crash.json -m "AddressSanitizer" -o reduced jsondump   

libxml2 : clean cimin
  cd libxml2 && ./build.sh
  cd libxml2 && ../cimin -i testcases/crash.xml -m "SEGV" -o reduced xmlint    
  
balance : clean cimin
  cd balance && ./build.sh
  cd balance && ../cimin -i -i testcases/fail -m "SampleError" -o reduced balance   
  
libpng : clean cimin
  cd libpng && ./build.sh
  cd libpng && ../cimin -i -i ./crash.png -m "MemorySanitizer" -o reduced libpng/test_pngfix   

Macros were used in the real file.
