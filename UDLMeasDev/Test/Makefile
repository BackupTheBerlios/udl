
SRC = Test.cpp
OBJ = $(SRC:.cpp=.o) # replaces the .c from SRC with .o
TARGET = ..\..\output\Test.so

LIBS = -L -lws2_32

CC = gcc

CFLAGS = -Wall -fPIC -c
LDFLAGS = -shared -W1,-soname,libudlmd_test.so.1 -o libudlmd_test.so.1.0
RM = rm

.PHONY : all        # .PHONY ignores files named all
all: $(TARGET)      # all is dependent on $(EXE) to be complete

$(TARGET): $(OBJ)   # $(EXE) is dependent on all of the files in $(OBJ) to exist
	$(CC) $(LDFLAGS) $(OBJ)

%.o: %.cpp         # combined w/ next line will compile recently changed .c files
	$(CC) $(CFLAGS) $<


.PHONY : clean   # .PHONY ignores files named clean
clean:
	-$(RM) $(TARGET) $(OBJ)  core *~  # '-' causes errors not to exit the process