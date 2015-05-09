EXE = LFS-Sim

SRC = \
	Driver.cpp \
	LFS_Sim.cpp \

OBJ_FILES := $(SRC:.cpp=.o)

GCC = g++
FLAGS = -g -Wall
COMPILER = -std=c++11

all:	$(EXE)

$(EXE):	$(OBJ_FILES)
	$(GCC) $(FLAGS) $(COMPILER) $(OBJ_FILES) -o $@

# common build recipe for .o
%.o:	%.cpp
	$(GCC) $(FLAGS) $(COMPILER) -c $^

clean:
	rm -f *.o $(EXE) $(GEN)
