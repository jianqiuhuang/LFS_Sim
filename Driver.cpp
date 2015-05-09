#include "LFS_Sim.h"

int main(int argc, char **argv){
	if(argc != 4){
		std::cerr << "Usage: ./LFS-Sim <disk-size> <segment-size> <input-file-name>" << std::endl;
		return 1;
	}
	int diskSize, segmentSize;
	diskSize = atoi(argv[1]);
	segmentSize = atoi(argv[2]);
	
	if(diskSize / segmentSize >= 10){
		LFS_Sim test(diskSize, segmentSize);
		test.simulate(argv[3]);
	}else{
		std::cerr << "disk size must be at least 10 times as big as segment size" << std::endl;
		return 1;
	}
	return 0;
};
