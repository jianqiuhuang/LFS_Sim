#include "LFS_Sim.h"

int main(int argc, char **argv){
	if(argc != 4){
		std::cerr << "Usage: ./LFS-Sim <disk-size> <segment-size> <input-file-name>" << std::endl;
		return 1;
	}

	LFS_Sim test(atoi(argv[1]),atoi(argv[2]));
	test.simulate(argv[3]);

	return 0;
};
