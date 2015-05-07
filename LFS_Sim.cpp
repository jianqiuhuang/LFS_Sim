#include "LFS_Sim.h"

LFS_Sim::LFS_Sim(int diskSize, int segmentSize, int fileSize){
	seekCount = 0;
	diskSize = diskSize;
	segmentSize = segmentSize;
	fileSize = fileSize;
	//Initialize log vector
	//Assume diskSize is divisible by segmentSize
	log.resize( (diskSize/segmentSize), segmentSize);
	//Initially, all segments are empty
	for(int i = 0; i < (int) log.size(); i++){
		availableSegments.push_back(i);
	}
}

//True -- perform garbage collecting
bool LFS_Sim::requireClean(){
	//less than 20% of total segments are empty
	if(availableSegments.size() < (int) (segmentSize * 0.2))
		return true;
	return false;
}	

void LFS_Sim::clean(){
	
	seekCount++;

	int max = -1;
	int victim = -1;

	for(int i = 0; i < (int) log.size(); i++){
		//Pick the segement with fewest live block, and also makesure not to pick the empty segment
		if(log[i] > max && log[i] != segmentSize){
			max = log[i];
			victim = i;
		}
	}
	//!!!!!!!!!!!!!!!!!!!!
	//Move live blocks to the end of the log
	
	//set the victim segment to empty
	log[victim] = segmentSize;
	//Add the emptied segment to available list
	availableSegment.push_back(victim);
}

//After file creation, an iNode is assign to the file
void LFS_Sim::createFile(int fileID){
	if(requireClean())
		clean();
	
	//Segment is full
	if(log[head] <= 0){
		//Move read/write head to the empty segment
		head = availableSegment.front();
		availableSegment.pop();
	}
	
	fileInfo tmp;
	tmp.dataBlockLocation.push_back(head);
	
	files.insert(std::pair<int, struct fileInfo>(fileID, tmp));
	
	log[head]--;
}

//Assume reading is permitted only if the block already exists in the disk 
void LFS_Sim::readFile(int fileID, int blockNumber){
	
	seekCount++;
	
	std::map<int, struct fileInfo>::iterator it;
		
	//Error checking
	it = files.find(fileID);
	if(it == files.end()){
		std::cerr << "Cannot read from file: " << fileID << ". File does not exist" << std::endl;
		exit(1);
	}
	if( (it->second).size() < blockNumber ){
		std::cerr << "Cannot read from file: " << fileID << ". File does not have block: " << blockNumber << std::endl;
		exit(1);
	}
}

void LFS_Sim::writeFile(int fileID, int blockNumber){
	if(requireClean())
                clean();
        
        //Segment is full
        if(log[head] <= 0){
                //Move read/write head to the empty segment
                head = availableSegment.front();
                availableSegment.pop();
        }

	std::map<int, struct fileInfo>::iterator it;
	it = files.find(fileID);

       	(it-second).dataBlockLocation.push_back(blockNumber);

        log[head]--;
	
}

void LFS_Sim::removeFile(int fileID){
	std::map<int, struct fileInfo>::iterator it;
	it = files.find(fileID);

	log[(it->second).iNodeLocation]--;
	for(int i = 0; i < (it->second).size(); i++){
		log[(it->second).dataBlockLocation[i]]++;
	}
}

