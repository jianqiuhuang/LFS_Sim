EXE = LFS-Sim
GEN = GenLFS-Sim

SRC = \
	Driver.cpp \
	LFS_Sim.cpp 

GEN_SRC = \
	Generator.cpp

OBJ_FILES := $(SRC:.cpp=.o)
GEN_OBJ_FILES := $(GEN_SRC:.cpp=.o)

GCC = g++
FLAGS = -g -Wall
COMPILER = -std=c++11

all:	$(EXE)

$(EXE):	$(OBJ_FILES)
	$(GCC) $(FLAGS) $(COMPILER) $(OBJ_FILES) -o $@

gen:	$(GEN_OBJ_FILES)
	$(GCC) $(FLAGS) $(COMPILER) $(GEN_OBJ_FILES) -o $(GEN)


# common build recipe for .o
%.o:	%.cpp
	$(GCC) $(FLAGS) $(COMPILER) -c $^

clean:
	rm -f *~ *# *.o *.exe $(EXE) $(GEN)
