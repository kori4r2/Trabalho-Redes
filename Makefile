CFLAGS = -g -Wall -std=c++11
LINKFLAGS = -pthread -std=c++11
PROJECT = Redes
CC = g++
BINDIR = bin
SRCDIR = src
LIBDIR = lib
SERVER = server
CLIENT = airplane
LIBS := $(wildcard $(LIBDIR)/*hpp)
SRCS := $(wildcard $(SRCDIR)/*cpp)
OBJS := $(patsubst $(SRCDIR)/%.cpp, $(BINDIR)/%.o, $(SRCS))
SERVEROBJS := $(filter-out $(BINDIR)/$(CLIENT).o, $(OBJS))
CLIENTOBJS := $(filter-out $(BINDIR)/$(SERVER).o, $(OBJS))

all : build

build : $(BINDIR) buildClient buildServer

buildServer : $(SERVEROBJS)
	$(CC) $(LINKFLAGS) -o $(SERVER) $(SERVEROBJS)

buildClient : $(CLIENTOBJS)
	$(CC) $(LINKFLAGS) -o $(CLIENT) $(CLIENTOBJS)

$(BINDIR) :
	mkdir -p $(BINDIR)

$(BINDIR)/%.o : $(SRCDIR)/%.cpp $(LIBS)
	$(CC) -c $< -I $(LIBDIR) $(CFLAGS) -o $@

clean :
	rm -f $(BINDIR)/*.o
	rm -f $(PROJECT).zip
	rm -f $(PROJECT)
	rm -f $(SERVER)
	rm -f $(CLIENT)
	rm -f debug*.txt
	clear

client : buildClient
	./$(CLIENT)

server : buildServer
	./$(SERVER)

run : server

.zip : clean
	zip $(PROJECT).zip $(SRCS) $(LIBS) Makefile *.pdf Authors.txt

debug : all
#	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(PROJECT) < tests/dataset_6_10.txt > tests/output_6_10.txt 2> debug_10.txt
#	diff tests/output_6_10.txt tests/result_6_10_2.txt
#	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(PROJECT) < tests/dataset_6_20.txt > tests/output_6_20.txt 2> debug_20.txt
#	diff tests/output_6_20.txt tests/result_6_20_2.txt
#	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(PROJECT) < tests/dataset_6_50.txt > tests/output_6_50.txt 2> debug_50.txt
#	diff tests/output_6_50.txt tests/result_6_50_2.txt
#	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(PROJECT) < tests/dataset_6_100.txt > tests/output_6_100.txt 2> debug_100.txt
#	diff tests/output_6_100.txt tests/result_6_100_2.txt

