#include "LFS_Sim.h"
#define DEBUG 1
LFS_Sim::LFS_Sim(int diskSize, int segmentSize){
	seekCount = 0;
	readCount = 0;
	writeCount = 0;
	diskSize = diskSize;
	segmentSize = segmentSize;
	//Initialize log vector
	//Assume diskSize is divisible by segmentSize
	log.resize( (int)(diskSize/segmentSize), segmentSize);
	//Initially, all segments are empty
	for(int i = 0; i < (int) log.size(); i++){
		availableSegments.push(i);
	}
	head = availableSegments.front();
	availableSegments.pop();
}

void LFS_Sim::simulate(std::string fileName){
	std::ifstream inFile(fileName.c_str());
        if(!(inFile.good())){
                std::cerr << "Cannot open file: " << fileName << std::endl;
                exit(1);
        }
        std::string line, word;
        int fileID, blockID;
        while(std::getline(inFile, line)){
                std::stringstream ss;
                ss << line;
                ss >> word;
                if(word.compare("START") == 0){
                        ss >> fileID;
                        createFile(fileID);
                }else if(word.compare("READ") == 0){
                        ss >> fileID >> blockID;
                        readFile(fileID, blockID);
                }else if(word.compare("WRITE") == 0){
                        ss >> fileID >> blockID;
                        writeFile(fileID, blockID);
                }else if(word.compare("END") == 0){
			ss >> fileID;
			removeFile(fileID);
		}else{
                        std::cerr << "Unknown input line from file: " << fileName << std::endl;
                        exit(1);
                }
        }
	inFile.close();
	//Temporary print statement
	if(DEBUG){
		std::cout << "seek count: " << seekCount << "\n number of read: " << readCount << "\n number of write: " << writeCount << std::endl;
	}
}

//True -- perform garbage collecting
bool LFS_Sim::requireClean(){
	//less than 20% of total segments are empty
	if( (int)availableSegments.size() < (int) ( log.size() * 0.2)){
		if(DEBUG) std::cerr << "threshold reached" << std::endl;
		return true;
	}
	return false;
}	

void LFS_Sim::clean(){
	//Clean 10% of the total segments
	int numberOfClean = (int) log.size() * 0.1 ;
	//Store the segment number of segments being cleaned

	std::queue<int> rewrite;
	if(DEBUG) std::cerr << "Start cleaning...number of segments to be clean: " <<  numberOfClean << std::endl;
	for(int clean = 0; clean < numberOfClean; clean++){
		int max = -1, victim = -1;
		seekCount++;
		for(int i = 0; i < (int) log.size(); i++){
			//Pick the segement has fewest live block, and also makesure not to pick the empty segment
			if(log[i] > max && log[i] != segmentSize){
				max = log[i];
				victim = i;
			}
		}
		//disk is full with live blocks
		if(victim == -1){
			std::cerr << " Disk is full -- no empty/garbage blocks\n" << "this should not occur" << std::endl;
			exit(1);
		}

		//set the victim segment to empty
		log[victim] = segmentSize;
		//Add the emptied segment to available list
		availableSegments.push(victim);
		rewrite.push(victim);
		if(DEBUG) std::cerr << "after cleaning, segement: " << victim << " is available now" << std::endl;
	}

		//Perform rewrite -- place live block from the deleted segment to the end of the log
		int rewriteBlockFrom = rewrite.front();
		rewrite.pop();
		if(DEBUG) std::cerr << "perform rewrite for alive data block from segment: " <<  rewriteBlockFrom << std::endl;
		for(std::map<int, struct fileInfo>::iterator it = files.begin(); it != files.end(); it++){
			for(int i = 0; i < (int) (it->second).dataBlockLocation.size(); i++){
				//If the file has a data block in the victim segment
				if( (it->second).dataBlockLocation[i] == rewriteBlockFrom ){
					if(DEBUG) std::cerr << "place block: " << i << " from file: " << it->first << " to the end of log" << std::endl;
					writeFile(it->first, i);
				}			
			}
		}
		
		
}
void LFS_Sim::debug(){
			for(auto it = files.begin(); it != files.end(); it++){
				std::cerr << "file: " << it->first << " has data blocks in segments: ";
				for(int i = 0; i < (int) (it->second).dataBlockLocation.size(); i++){
					std::cerr << (it->second).dataBlockLocation[i] << " ";
				}
				std::cerr << std::endl;
			}
}
//After file creation, an iNode is assign to the file
void LFS_Sim::createFile(int fileID){
	if(requireClean())
		clean();
	
	//Segment is full
	if(log[head] <= 0){
		if(!availableSegments.empty()){
			//Move read/write head to the empty segment
			head = availableSegments.front();
			availableSegments.pop();
		}else{
			std::cerr << "availableSegments has no additional empty segments, this error should not occur" << std::endl;
			exit(1);
		}
	}
	
	fileInfo tmp;
	tmp.dataBlockLocation.push_back(head);
	
	files.insert(std::pair<int, struct fileInfo>(fileID, tmp));
	
	log[head]--;
	
	if(DEBUG) std::cerr << "after creatFile, head: " << head << ", log[head]: " << log[head] << std::endl;
	if(DEBUG) debug();
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
	if( (int) (it->second).dataBlockLocation.size() < blockNumber ){
		std::cerr << "Cannot read from file: " << fileID << ". File does not have block: " << blockNumber << std::endl;
		exit(1);
	}
	if(DEBUG){
		std::cerr << "reading file: " << fileID << ", block number: " << blockNumber <<  std::endl;	
		debug();
	}
}

void LFS_Sim::writeFile(int fileID, int blockNumber){
	if(requireClean())
                clean();
        
        //Segment is full
        if(log[head] <= 0){
		if(!availableSegments.empty()){
                	//Move read/write head to the empty segment
        	        head = availableSegments.front();	
	                availableSegments.pop();
		}else{
			std::cerr << "availableSegments has no additional empty segments, this error should not occur" << std::endl;
			exit(1);
		}
        }
	
	if(DEBUG) std::cerr << "*******writing file: " << fileID << ", block: " << blockNumber << " to segment: " << head << std::endl;
	std::map<int, struct fileInfo>::iterator it;
	it = files.find(fileID);
	
	//Block already exists
	if( (int)(it->second).dataBlockLocation.size() > blockNumber)
       		(it->second).dataBlockLocation[blockNumber] = head;
	//Block not in disk
	else
       		(it->second).dataBlockLocation.push_back(head);
		

        log[head]--;
	
	if(DEBUG) std::cerr << "after writeFile, head: " << head << ", log[head]: " << log[head] << std::endl;	
	if(DEBUG) debug();
}

void LFS_Sim::removeFile(int fileID){
	std::map<int, struct fileInfo>::iterator it;
	it = files.find(fileID);
	
	for(int i = 0; i < (int) (it->second).dataBlockLocation.size(); i++){
		log[(it->second).dataBlockLocation[i]]++;
	}
	files.erase(it);
	if(DEBUG){
		std::cerr << "after removeFile fileID: " << fileID << std::endl;
		debug();
	}
}

