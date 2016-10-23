main:main.o universal.o surface.o magiccube.o
	g++ main.o universal.o surface.o magiccube.o -o main -g -Wall
main.o:main.cpp universal.h surface.h magiccube.h
	g++ -c main.cpp -g -Wall
universal.o:universal.cpp universal.h
	g++ -c universal.cpp -g -Wall
surface.o:surface.cpp surface.h universal.h
	g++ -c surface.cpp -g -Wall
magiccube.o:magiccube.cpp magiccube.h universal.h surface.h
	g++ -c magiccube.cpp -g -Wall
clean:
	rm main main.o universal.o surface.o magiccube.o
