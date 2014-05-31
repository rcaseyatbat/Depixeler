# Makefile for cs171 HW 0
# Just type "make" to compile; if it works right, you should have an
# executable called hw0.

CXX		= g++
# add -g for debugging info
CXXFLAGS	= -Wall -g -DMACOSX

ifeq "$(PLATFORM)" ""
PLATFORM:= $(shell uname)
endif

ifeq "$(PLATFORM)" "Darwin"
# use this for mac:
LDFLAGS	= -L/usr/X11R6/lib -framework GLUT -framework OpenGl -lpng
endif

ifeq "$(PLATFORM)" "Linux"
LDFLAGS	= -L/usr/X11R6/lib -lglut -lGL -lGLU -lXi -lXmu -lpng
endif

# use this for linux:
#LDFLAGS	= -L/usr/X11R6/lib -lglut -lGL -lGLU -lXi -lXmu -lpng

SRCS	= *.cpp *.h
OBJS	= main.o neighbors.o epx.o scale2x.o scale3x.o scale4x.o eagle.o bilinear2x.o bicubic2x.o eagle3x.o data.o matrix.o vec3.o readpng.o
PROG	= depixeler

all: $PROG

$PROG: $(OBJS)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $(PROG) $^

clean:
	rm *.o $(PROG)

main.o: main.cpp data.h
	$(CXX) $(CXXFLAGS) -c main.cpp

neighbors.o: neighbors.cpp neighbors.h
	$(CXX) $(CXXFLAGS) -c neighbors.cpp

epx.o: epx.cpp epx.h
	$(CXX) $(CXXFLAGS) -c epx.cpp

scale2x.o: scale2x.cpp scale2x.h
	$(CXX) $(CXXFLAGS) -c scale2x.cpp

scale3x.o: scale3x.cpp scale3x.h
	$(CXX) $(CXXFLAGS) -c scale3x.cpp

scale4x.o: scale4x.cpp scale4x.h
	$(CXX) $(CXXFLAGS) -c scale4x.cpp

eagle.o: eagle.cpp eagle.h
	$(CXX) $(CXXFLAGS) -c eagle.cpp

bilinear2x.o: bilinear2x.cpp bilinear2x.h
	$(CXX) $(CXXFLAGS) -c bilinear2x.cpp

bicubic2x.o: bicubic2x.cpp bicubic2x.h
	$(CXX) $(CXXFLAGS) -c bicubic2x.cpp

eagle3x.o: eagle3x.cpp eagle3x.h
	$(CXX) $(CXXFLAGS) -c eagle3x.cpp

matrix.o: matrix.cpp matrix.h
	$(CXX) $(CXXFLAGS) -c matrix.cpp

readpng.o: readpng.cpp readpng.h
	$(CXX) $(CXXFLAGS) -c readpng.cpp

vec3.o: vec3.cpp vec3.h
	$(CXX) $(CXXFLAGS) -c vec3.cpp
    
.cpp.o:
	g++ -c $(CXXFLAGS) -I/usr/local/include/freetype2 -o $@ $<

.PHONY: all clean
