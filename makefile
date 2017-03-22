#testing makefile
CFLAGS = -fopenmp -lpthread
INCLUDE = ./BMPlib

ctomp: ct_omp.cpp MyBMP.cpp
	g++ -g -I$(INCLUDE) ct_omp.cpp MyBMP.cpp -o bmp $(CFLAGS)
	
clean:
	rm -f ctomp