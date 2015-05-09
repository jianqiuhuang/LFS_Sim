#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <chrono>
#include <algorithm>
#include <vector>

#define DEBUG 1 // 1 to turn debug on

typedef struct file_struct{
  int id;
  int size;
} File;

void usage(char *progName){
  std::cerr << "Usage: " << progName << " <disk-size> <write-ratio [0-100]> <output-file>" << std::endl;
}

std::string start(int fileID){
  std::stringstream ss;
  ss << "START " << fileID << std::endl;
  std::string instruction;
  std::getline(ss, instruction);
  return instruction;
}

std::string write(int fileID, int blockID){
  std::stringstream ss;
  ss << "WRITE " << fileID << " " << blockID << std::endl;
  std::string instruction;
  std::getline(ss, instruction);
  return instruction;  
}

std::string read(int fileID, int blockID){
  std::stringstream ss;
  ss << "READ " << fileID << " " << blockID << std::endl;
  std::string instruction;
  std::getline(ss, instruction);
  return instruction;  
}

std::string end(int fileID){
  std::stringstream ss;
  ss << "END " << fileID << std::endl;
  std::string instruction;
  std::getline(ss, instruction);
  return instruction;  
}

int main(int argc, char **argv){
  if(argc < 4){
    usage(argv[0]);
    exit(EXIT_FAILURE);
  }
  
  // extract command line arguments
  std::stringstream ss;
  int diskCapacity; ss << argv[1]; ss >> diskCapacity;
  int writeRatio=10; ss << argv[2]; ss >> writeRatio;
  char *ofileName = argv[3];
  
  // validate writeRatio
  if( (writeRatio < 0) || (writeRatio > 100) ){
    std::cerr << argv[0] << ": writeRatio out of range [0,100]: " << writeRatio << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ofstream outfile;
  outfile.open(ofileName);

  // seed rng
  std::chrono::high_resolution_clock::time_point time = std::chrono::high_resolution_clock::now();
  std::minstd_rand rng(time.time_since_epoch().count());

  /***** generate simluation sequence *****/
  if(DEBUG) std::cerr << argv[0] << ": generating...diskCap=" << diskCapacity << std::endl;
  int diskSize = 0;
  std::vector<File> aliveFiles;
  int new_fid = 1;
  int nFilesCreated = 0;

  // create at least 1 file
  File f;
  f.id = new_fid++;
  f.size = 1;
  aliveFiles.push_back(f);
  diskSize++;
  nFilesCreated++;
  outfile << start(f.id) << std::endl;
  
  do{
    // start
    if( ((rng()%10) < 2) && (nFilesCreated < diskCapacity/4) ){
      // new file
      f.id = new_fid++;
      f.size = 1;
      
      // add to disk
      aliveFiles.push_back(f);
      diskSize++;
      nFilesCreated++;
      outfile << start(f.id) << std::endl;
      continue;
    }
    
    // choose a file
    int new_fsize = -1;
    int new_fid = -1;
    if(!aliveFiles.empty()){
      std::random_shuffle(aliveFiles.begin(), aliveFiles.end());
      new_fid = aliveFiles.back().id;
      new_fsize = aliveFiles.back().size;
    }
    else{
      break;
    }
    
    // write
    if( (int)(rng() % 100) < writeRatio ){
      // writeRatio% chance to write
      int block = 1 + (rng() % new_fsize);
      File newFile;
      if(block == new_fsize){
	// writing new block
	diskSize++;
	new_fsize++;
	newFile.id = new_fid;
	newFile.size = new_fsize;
	aliveFiles.pop_back();
	aliveFiles.push_back(newFile);
      }
      outfile << write(newFile.id, block) << std::endl;
      continue;
    }
    
    // end/read
    if(rng() % 100 == 0){
      // end
      aliveFiles.pop_back();
      outfile << end(new_fid) << std::endl;
    }
    else{
      // (100-writeRatio)% chance to read
      if(new_fsize > 1){
	int block = 1 + (rng() % (new_fsize-1)); 
	outfile << read(new_fid, block) << std::endl;
      }
    }
    
  } while(diskSize < diskCapacity);
  
  // check that all files have ended, if not then clean up
  while(!aliveFiles.empty()){
    f = aliveFiles.back();
    aliveFiles.pop_back();
    outfile << end(new_fid) << std::endl;
  }
  
  return 0;
}
