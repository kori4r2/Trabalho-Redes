CFLAGS = -g -Wall -std=c++11
LINKFLAGS = -pthread -std=c++11
PROJECT = Redes
CC = g++
DEBUGDIR = tests
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

$(DEBUGDIR) :
	mkdir -p $(DEBUGDIR)

$(BINDIR) :
	mkdir -p $(BINDIR)

$(BINDIR)/%.o : $(SRCDIR)/%.cpp $(LIBS)
	$(CC) -c $< -I $(LIBDIR) $(CFLAGS) -o $@

clean :
	rm -rf $(BINDIR)
	rm -rf $(DEBUGDIR)
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

debugServer: $(DEBUGDIR) all
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(SERVER) > $(DEBUGDIR)/serverOutput.txt 2> $(DEBUGDIR)/serverError.txt

debugClient: $(DEBUGDIR) all
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(CLIENT) > $(DEBUGDIR)/clientOutput.txt 2> $(DEBUGDIR)/clientError.txt

