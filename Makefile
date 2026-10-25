CXXFLAGS = -std=c++20 -O3 -ffast-math `pkg-config --cflags raylib`
LDFLAGS =
LIBS = `pkg-config --libs raylib`
SRCS = main.cpp

main: main.cpp Makefile
	g++ $(CXXFLAGS) -o main $(SRCS) $(LDFLAGS) $(LIBS)
