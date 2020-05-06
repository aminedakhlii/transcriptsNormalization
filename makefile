CC = g++
LIBPATH = ./lib64
CFLAGS = -I ./include -L $(LIBPATH) -lxl -Wl,-rpath,$(LIBPATH)
transcripts : transcripts.cpp
	$(CC) -std=c++14 -o trans transcripts.cpp transFuncs.cpp transcripts.hpp -g $(CFLAGS)
