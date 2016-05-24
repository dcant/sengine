CC = g++
INCLUDE = -I./worddict -I./lib2-64/dict/include -I./lib2-64/wordseg -I./include -I./lib2-64/ullib/include -I./lib2-64/others-ex/include
LIBS = -lsegment -luldict -lullib -lullib_ex
LIBPATH = ./test/lib
CFLAGS = -g -Wall $(INCLUDE) -L $(LIBPATH)
BIN_PATH = bin
OBJ_PATH = obj
MKDIR = mkdir

all: $(BIN_PATH)/GenFwd $(BIN_PATH)/GenInv $(BIN_PATH)/Searcher


$(BIN_PATH)/GenIndex: $(OBJ_PATH)/index.o $(OBJ_PATH)/genindex.o $(OBJ_PATH)/segment.o $(OBJ_PATH)/scw.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) -lpthread -lcrypto

$(BIN_PATH)/GenFwd: $(OBJ_PATH)/fwdindex.o $(OBJ_PATH)/segment.o $(OBJ_PATH)/scw.o $(OBJ_PATH)/genfwd.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) -lpthread -lcrypto

$(BIN_PATH)/Searcher: $(OBJ_PATH)/searcher.o $(OBJ_PATH)/segment.o $(OBJ_PATH)/scw.o $(OBJ_PATH)/search.o $(OBJ_PATH)/keymap.o $(OBJ_PATH)/doc.o $(OBJ_PATH)/docmap.o $(OBJ_PATH)/hitdoc.o $(OBJ_PATH)/priqueue.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) -lpthread -lcrypto

$(BIN_PATH)/GenInv: $(OBJ_PATH)/invindex.o $(OBJ_PATH)/geninv.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) -lpthread -lcrypto

$(OBJ_PATH)/fwdindex.o: process/fwdindex.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/invindex.o: process/invindex.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/genfwd.o: process/genfwd.cpp
	$(CC) $(CFLAGS) -c $<  -o $@

$(OBJ_PATH)/geninv.o: process/geninv.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/index.o: process/index.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/genindex.o: process/genindex.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/segment.o: process/segment.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/scw.o: process/scw.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/searcher.o: process/searcher.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/search.o: process/search.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/keymap.o: process/keymap.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/doc.o: process/doc.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/docmap.o: process/docmap.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/hitdoc.o: process/hitdoc.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/priqueue.o: process/priqueue.cpp
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -fr $(BIN_PATH)/GenFwd
	rm -fr $(BIN_PATH)/GenInv
	rm -fr $(BIN_PATH)/Searcher
	rm -fr $(OBJ_PATH)/*.o
