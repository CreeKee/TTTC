CC = g++
CFLAGS = -g -Wall -fPIC
GLFLAGS = -lglfw -framework CoreVideo -framework OpenGL -framework IOKit -framework Cocoa -framework Carbon 

EXECUTABLE = TTTC

SOURCES = glad.o WindowManager.o FieldManager.o GameMaster.o MoveList.o bot_gen0.o tlist.o gtObserver.o gtNode.o boardInstance.o Brain.o
.PHONY: $(SOURCES)

all: $(SOURCES)
	$(CC) $(GLFLAGS) -o tttc TTTC.cpp $(SOURCES)

glad.o:
	$(CC) $(CFLAGS) -c glad glad/src/glad.c

WindowManager.o:
	$(CC) $(CFLAGS) -c WindowManager.cpp

FieldManager.o:
	$(CC) $(CFLAGS) -c FieldManager.cpp

GameMaster.o:
	$(CC) $(CFLAGS) -c GameMaster.cpp

bot_gen0.o:
	$(CC) $(CFLAGS) -c bot_gen0.cpp

MoveList.o:
	$(CC) $(CFLAGS) -c MoveList.cpp

tlist.o:
	$(CC) $(CFLAGS) -c tlist.cpp

gtObserver.o:
	$(CC) $(CFLAGS) -c gtObserver.cpp

gtNode.o:
	$(CC) $(CFLAGS) -c gtNode.cpp

boardInstance.o:
	$(CC) $(CFLAGS) -c boardInstance.cpp

Brain.o:
	$(CC) $(CFLAGS) -c Brain.cpp

clean:
	rm *.out