IDIR =../include
CC=gcc
CXX=g++
CFLAGS=-I$(IDIR)
CXXFLAGS=-I$(IDIR) -Wall

ODIR=obj
LDIR=../lib

LIBS=-lwiringPi -lstdc++

_DEPS = Adafruit_CAP1188.h
DEPS = $(patsubst %, $(IDIR)/%, $(_DEPS))

_OBJ = main.o Adafruit_CAP1188.o
OBJ = $(patsubst %, $(ODIR)/%, $(_OBJ))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


touchSPI: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
