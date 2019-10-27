CC=g++
CFLAGS = `root-config --cflags --libs`

trackmake: main.cpp hit.cpp event.cpp
	$(CC) -std=c++0x -o track_finder.exe main.cpp hit.cpp event.cpp $(CFLAGS)