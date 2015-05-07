#ifndef LFS_SIM_H
#define LFS_SIM_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>

struct fileInfo{
	//A collection of indexes that specify the location of all data blocks and iNode belong to this file
	//(block number, segment number)
	std::vector<int> dataBlockLocation;		
};

class LFT_Sim{
	public:
		LFT_Sim(int diskSize, int segmentSize, int fileSize);
		//return the the total seek distance
		int simulate();
	private:
		void createFile(int fileID);
		void readFile(int fileID, int blockNumber);
		void writeFile(int fileID);
		void removeFile(int fileID);

		//Perform garbage collection on segment with least live blocks
		void clean();
		//Called after every writeFile and createFile operation
		//Return true if the percentage of non-empty segments reaches 80%
		//If true, how many segments shoul be cleaned?
		bool requireCleaning();

		std::vector<int> log;
		std::map<int, fileInfo> files;
		std::queue<int> availableSegments;
		int diskSize;
		int fileSize;
		int segmentSize;	
		int head;
	
		int seekCount;
};

#endif
