#include "LFS_Sim.h"

LFS_Sim::LFS_Sim(int diskSize, int segmentSize, int fileSize){
	diskSize = diskSize;
	segmentSize = segmentSize;
	fileSize = fileSize;
	bufferCount = segmentSize;
	//Initialize log vector
	//Assume diskSize is divisible by segmentSize
	log.resize( (diskSize/segmentSize), segmentSize);
	//Initially, all segments are empty
	for(int i = 0; i < (int) log.size(); i++){
		availableSegments.push_back(i);
	}
	head = availableSements.front();
}

//True -- perform garbage collecting
bool LFS_Sim::requireClean(){
	//less than 20% of total segments are empty
	if(availableSegments.size() < (int) (segmentSize * 0.2))
		return true;
	return false;
}	

void LFS_Sim::clean(){
	int max = -1;
	int victim = -1;
	for(int i = 0; i < (int) log.size(); i++){
		//Pick the segement with fewest live block, and also makesure not to pick the empty segment
		if(log[i] > max && log[i] != segmentSize){
			max = log[i];
			victim = i;
		}
	}
	if(victim == -1){
		std::cerr << "Error: victim segement: -1" << std::endl;
		exit(1);
	}
	//set the victim segment to empty
	log[victim] = segmentSize;
	//Add the emptied segment to available list
	availableSegment.push_back(victim);
}

//After file creation, an iNode is assign to the file
void LFS_Sim::createFile(int fileID){
	if(requireClean())
		clean();
	
	struct fileInfo tmp;
	//Current segment is not full
	if(log[head] > 0){
		log[head]--;
		tmp.iNodeLocation = head;
	}
	//Find new empty segment
	else{
		head = availableSegment.front();
		availableSegment.pop();
		log[head]--;
		tmp.iNodeLocation = head;
	}
	files.insert(std::pair<int, struct fileInfo>(fileID, tmp));
}

void LFS_Sim::writeFile(int blockNumber){


}
