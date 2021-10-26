#include "../include/world.hpp"

World::World(){
  // Generate Random Seed
  srand(time(0));
  

  printf("Generating World\n");


  noise = new FastNoiseLite(rand() % 10000);
  noise->SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);

}


World::~World(){
  delete noise;
}

BLOCK_INT World::getBlockState(int x, int y, int z){
  //return renderDat[x][y][z];
  
  if (worldOverrides.count(primitivePostoPos(x,y,z))){
    //auto it = worldOverrides.find(primitivePostoPos(x,y,z));
    return (1);
    

  }
  else{
    return y <= ((int) (noise->GetNoise((float)x, float(z)) * NOISE_CONSTANT));
  }
}

void World::destroyBlock(int x, int y, int z){
  worldOverrides.insert(std::pair<std::tuple<POSITION_INT,POSITION_INT,POSITION_INT>, BLOCK_INT>(primitivePostoPos(x,y,z), 0));
}
std::tuple<POSITION_INT,POSITION_INT,POSITION_INT> World::primitivePostoPos(int x, int y, int z){
  std::tuple<POSITION_INT,POSITION_INT,POSITION_INT> tempPos(x,y,z);
  return tempPos;
}
void World::createBlock(int x, int y, int z, int type){
  worldOverrides.insert(std::pair<std::tuple<POSITION_INT,POSITION_INT,POSITION_INT>, BLOCK_INT>(primitivePostoPos(x,y,z), type));
}


int World::getBlockHeight(int x, int z){
   return ((int) (noise->GetNoise((float)x, float(z)) * NOISE_CONSTANT)) ;

}