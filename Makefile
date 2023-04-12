CC = gcc
SOURCE = cimin.c
TARGET = cimin
OUTPUT = reduced
BUILD = ./build.sh

$(TARGET): $(SOURCE)
        $(CC) -o $@ $<

.PHONY: clean
clean: 
        rm -f $(TARGET)

jsmn: clean cimin $(TARGET)
        cd jsmn && $(BUILD)
        cd jsmn && ../$(TARGET) -i testcases/crash.json -m "AddressSanitizer" -o $(OUTPUT) jsondump

libxml2: clean cimin $(TARGET)
        cd libxml2 && $(BUILD)
        cd libxml2 && ../$(TARGET) -i testcases/crash.xml -m "SEGV" -o $(OUTPUT) xmlint

balance: clean cimin $(TARGET)
        cd balance && $(BUILD)
        cd balance && ../$(TARGET) -i testcases/fail -m "SampleError" -o $(OUTPUT) balance

libpng: clean cimin $(TARGET)
        cd libpng && $(BUILD)
        cd libpng && ../$(TARGET) -i ./crash.png -m "MemorySanitizer" -o $(OUTPUT) libpng/test_pngfix
        
