CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

SRC = UDLMain.cpp UDLTask.cpp UDLAction.cpp UDLDataBase.cpp UDLMeasDevice.cpp
OBJ = $(SRC:.cpp=.o) # replaces the .c from SRC with .o

LIBS = -L d:/CppLib/boost_1_39_0/stage/lib \
       d:\CppLib\boost_1_39_0\stage\lib\libboost_program_options-mgw34-mt-s.lib \
       d:\CppLib\boost_1_39_0\stage\lib\libboost_system-mgw34-mt-s.lib \
       d:\CppLib\boost_1_39_0\stage\lib\libboost_thread-mgw34-mt-s.lib \
       d:\CppLib\boost_1_39_0\stage\lib\libboost_serialization-mgw34-mt-s.lib \
       d:\CppLib\boost_1_39_0\stage\lib\libboost_date_time-mgw34-mt-s.lib \
       -lws2_32

TARGET = udl.exe
LDFLAGS = -o 
RM = rm
CC = g++

#$(TARGET):	$(OBJS)
#	$(CXX) -o $(TARGET) -I d:/CppLib/boost_1_39_0  $(OBJS) $(LIBS)

#all:	$(TARGET)

.PHONY : all        # .PHONY ignores files named all
all: $(TARGET)      # all is dependent on $(EXE) to be complete

$(TARGET): $(OBJ)   # $(EXE) is dependent on all of the files in $(OBJ) to exist
	$(CC) $(LDFLAGS) $(TARGET) $(OBJ) $(LIBS)

%.o: %.cpp         # combined w/ next line will compile recently changed .c files
	$(CC) $(CFLAGS) -I d:/CppLib/boost_1_39_0 -o $@ -c $<


clean:
	RM -f $(OBJS) $(TARGET)
