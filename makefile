all:
	g++ main.cpp -o chip8 -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
	./chip8



