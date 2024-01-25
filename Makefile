CC = g++
CFLAGS = -g -Wall -fPIC
GLFLAGS = -lglfw -framework CoreVideo -framework OpenGL -framework IOKit -framework Cocoa -framework Carbon 

EXECUTABLE = TTTC
.PHONY: WindowManager.o FieldManager.o glad.o GameMaster.o

all: glad.o WindowManager.o FieldManager.o GameMaster.o
	$(CC) $(GLFLAGS) -o tttc TTTC.cpp glad.o WindowManager.o FieldManager.o GameMaster.o

glad.o:
	$(CC) $(CFLAGS) -c glad glad/src/glad.c

WindowManager.o:
	$(CC) $(CFLAGS) -c WindowManager.cpp

FieldManager.o:
	$(CC) $(CFLAGS) -c FieldManager.cpp

GameMaster.o:
	$(CC) $(CFLAGS) -c GameMaster.cpp

clean:
	rm *.out