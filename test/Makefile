CC = g++


LIBS = -lsegment -luldict -lullib -lullib_ex 
LIB_PATH = ./lib
INC_PATH = -I../worddict -I../lib2-64/dict/include -I../lib2-64/wordseg -I../lib2-64/ullib/include -I../lib2-64/others-ex/include
CFLAGS = -g -Wall $(INC_PATH) -L $(LIB_PATH)
CPP_SRC = $(wildcard *.cpp)  
CPP_OBJ = $(patsubst %.cpp, %.o,$(CPP_SRC))  

TARGET = test_seg

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(CPP_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) -lcrypto -lpthread

%o: %cpp  
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -f *.o $(TARGET)
