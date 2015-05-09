#ifndef LFS_SIM_H

#define LFS_SIM_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <stdlib.h>
#include <fstream>
struct fileInfo{
	//A collection of indexes that specify the location of all data blocks and iNode belong to this file
	//(block number, segment number)
	std::vector<int> dataBlockLocation;		
};

class LFS_Sim{
	public:
		LFS_Sim(int diskSize, int segmentSize);
		//return the the total seek distance
		void simulate(std::string fileName);
	private:
		void createFile(int fileID);
		void readFile(int fileID, int blockNumber);
		void writeFile(int fileID, int blockNumber);
		void removeFile(int fileID);
		void debug();
		//Perform garbage collection on segment with least live blocks
		void clean();
		//Called after every writeFile and createFile operation
		//Return true if the percentage of non-empty segments reaches 80%
		//If true, how many segments shoul be cleaned?
		bool requireClean();

		std::vector<int> log;
		std::map<int, fileInfo> files;
		std::queue<int> availableSegments;
		int diskSize;
		int segmentSize;	
		int head;
	
		int seekCount, readCount, writeCount;
};

#endif
