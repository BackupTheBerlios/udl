CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

SRC = ../UDLlib/system.cpp ../UDLlib/DynamicLib.cpp \
      ../UDLlib/StringTools.cpp \
      ../UDLlib/getopt_pp/getopt_pp.cpp \
      UDLMain.cpp UDLTask.cpp UDLDataBase.cpp UdlDbCsv.cpp \
      UDLDevices.cpp UDLDevice.cpp UDLMeasDevice.cpp UdlSettings.cpp UdlStdOut.cpp UdlSettingsFile.cpp
      
OBJ = $(SRC:.cpp=.o) # replaces the .c from SRC with .o

LIBS = -lstdc++ -lrt -ldl

#TARGET = ..\output\udl.exe
TARGET = ../output/udl
LDFLAGS = -o 
RM = rm
CC = gcc

.PHONY : all        # .PHONY ignores files named all
all: $(TARGET)      # all is dependent on $(EXE) to be complete

$(TARGET): $(OBJ)   # $(EXE) is dependent on all of the files in $(OBJ) to exist
	$(CC) $(LDFLAGS) $(TARGET) $(OBJ) $(LIBS)

%.o: %.cpp         # combined w/ next line will compile recently changed .c files
	$(CC) $(CFLAGS) -o $@ -c $<


clean:
	$(RM) -f $(OBJ) $(TARGET)
