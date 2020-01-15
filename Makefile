CXX = g++
CXXFLAGS = -std=c++14 -Wall -MMD -g 
EXEC = RAIInet
OBJECTS = main.o grid.o link.o player.o textdisplay.o window.o graphicsdisplay.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} -lX11

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}

