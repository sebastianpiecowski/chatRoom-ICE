TARGETDIR = Debug/
TARGETCLIENT = Debug/client
TARGETSERVER = Debug/server
TARGETROOM = Debug/room
TARGETM = Debug/main
OBJS = $(SRC:.cpp=.o)
.PHONY: all clean
all: $(TARGETM) $(TARGETSERVER)

Chat.cpp Chat.h:
	slice2cpp Chat.ice

$(TARGETCLIENT): UserImpl.cpp Chat.cpp
	mkdir -p $(TARGETDIR)
	c++ -I. -c Chat.cpp UserImpl.cpp -Wall
	c++ -o $(TARGETCLIENT) Chat.o UserImpl.o -lIce -lIceUtil -pthread -Wall

$(TARGETSERVER): ServerImpl.cpp Chat.cpp
	mkdir -p $(TARGETDIR)
	c++ -I. -c Chat.cpp ServerImpl.cpp -Wall
	c++ -o $(TARGETSERVER) Chat.o ServerImpl.o -lIce -lIceUtil -pthread -Wall

$(TARGETROOM): RoomImpl.cpp Chat.cpp
	mkdir -p $(TARGETDIR)
	c++ -I. -c Chat.cpp RoomImpl.cpp -Wall
	c++ -o $(TARGETROOM) Chat.o RoomImpl.o -lIce -lIceUtil -pthread

$(TARGETM): main.cpp Chat.cpp
	mkdir -p $(TARGETDIR)
	c++ -I. -c Chat.cpp main.cpp -Wall
	c++ -o $(TARGETM) Chat.o main.o -lIce -lIceUtil -pthread
clean:
	-rm Chat.cpp Chat.h $(TARGETCLIENT) $(TARGETSERVER) $(TARGETROOM) $(TARGETM)
