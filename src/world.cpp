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
    return (worldOverrides[primitivePostoPos(x,y,z)]);
  }
  else{
    return y <= ((int) (noise->GetNoise((float)x, float(z)) * NOISE_CONSTANT));
  }
}

void World::destroyBlock(int x, int y, int z){
  createBlock(x,y,z,0);
}
std::tuple<POSITION_INT,POSITION_INT,POSITION_INT> World::primitivePostoPos(int x, int y, int z){
  std::tuple<POSITION_INT,POSITION_INT,POSITION_INT> tempPos(x,y,z);
  return tempPos;
}
void World::createBlock(int x, int y, int z, int type){

  if (!worldOverrides.count(primitivePostoPos(x,y,z))){
    worldOverrides.insert(std::pair<std::tuple<POSITION_INT,POSITION_INT,POSITION_INT>, BLOCK_INT>(primitivePostoPos(x,y,z), type));
  }
  else{
    worldOverrides[primitivePostoPos(x,y,z)] = type;
  }
}


int World::getBlockHeight(int x, int z){
  return ((int) (noise->GetNoise((float)x, float(z)) * NOISE_CONSTANT)) ;
}

// 1 = is world
// 0 = is user
int World::isWorldOrUser(int x, int y, int z){
  return ((int) (noise->GetNoise((float)x, float(z)) * NOISE_CONSTANT)) == y;
}

// Returns size of target
float* World::generateAVao(const float *baseVerticies, int *outputLen, int startVertexPos, int endVertexPos, int baseSize, int stride, int xStart, int yStart, int zStart, int xEnd, int yEnd, int zEnd){
  // First, lets malloc the needed targetVerticies
  // The current Position in the output array
  int currentPos = 0;
  // The pos that should be used in the base verticies
  int setPos_c = 0;
  // Stores the target verticies
  float *targetVerticies;

  // Calculate the target size
  int targetSize = (baseSize);
  targetSize *= (zEnd-zStart);
  targetSize *= (xEnd - xStart);
  targetSize *= (yEnd-yStart);
  // Allocate the target size
  targetVerticies = (float*) malloc(targetSize);

  for (int x = xStart; x<xEnd; x++){
    for (int y = yStart; y<yEnd; y++){
    for (int z = zStart; z<zEnd; z++){
      for (int c = 0; c<(baseSize/sizeof(float))/stride; c++){
        // If the block is valid
        if (getBlockState(x,y, z) != 0){
        for (int ver = startVertexPos; ver<endVertexPos; ver++){
          setPos_c = ((c*stride)+ver);
          switch (ver){
            case 0:
              targetVerticies[currentPos] = baseVerticies[setPos_c] + x;
            break;
            case 1:
              targetVerticies[currentPos] = baseVerticies[setPos_c] + y;
            break;

            case 2:
              targetVerticies[currentPos] = baseVerticies[setPos_c] + z;
            break;
          }
          currentPos++;
        }
        for (int ver2 = stride-endVertexPos; ver2<stride; ver2++){
          setPos_c = ((c*stride)+ver2);
          targetVerticies[currentPos] = baseVerticies[setPos_c];
          currentPos++;
        }
      }
    }
    }
    }
  }
  *outputLen = currentPos;
  return targetVerticies;
}