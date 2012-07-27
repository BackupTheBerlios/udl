
SRC = VC820.cpp ../../UDLmain/UdlSettingsFile.cpp
OBJ = $(SRC:.cpp=.o) # replaces the .c from SRC with .o
TARGET = ..\..\output\VC820.dll

LIBS = -L -lws2_32

CC = g++
#CFLAGS = -c -BUILD_DLL -I d:\CppLib\boost_1_39_0\

CFLAGS = -DBUILD_DLL
LDFLAGS = -shared -o 
RM = rm

.PHONY : all        # .PHONY ignores files named all
all: $(TARGET)      # all is dependent on $(EXE) to be complete

$(TARGET): $(OBJ)   # $(EXE) is dependent on all of the files in $(OBJ) to exist
	$(CC) $(LDFLAGS) $(TARGET) $(OBJ) $(LIBS) -Wl,-out-implib,VC820.a

%.o: %.cpp         # combined w/ next line will compile recently changed .c files
	$(CC) $(CFLAGS)-DBUILD_DLL -I d:/CppLib/boost_1_39_0 -o $@ -c $<



.PHONY : clean   # .PHONY ignores files named clean
clean:
	-$(RM) $(TARGET)  $(OBJ)    # '-' causes errors not to exit the process