#include "../include/world.hpp"

World::World(const int width, const int height, const int tall, const long max_width, const long max_height, const long max_tall) : w_width(width), w_height(height), w_tall(tall), mx_width(max_width), mx_height(max_height), mx_tall(max_tall){
  // Generate Random Seed
  srand(time(0));
  if (max_width > MAX_SIZE || max_height > MAX_SIZE || max_tall > MAX_SIZE){
    printf("Error: World too big\n");
  }
  

  printf("Generating World\n");


  noise = new FastNoiseLite(rand() % 10000);
  noise->SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);

  // Gather noise data
  for (int x = 0; x < width; x++)
  {
    for (int y = 0; y < tall; y++)
    {
      createBlock(x, ((int) (noise->GetNoise((float)x, (float)y) * NOISE_CONSTANT)) + mx_height/2, y, 1);
    }
  }

}


World::~World(){

}

int World::getWidth(){
  return w_width;
}
int World::getHeight(){
  return w_height;
}
int World::getTall(){
  return w_tall;
}

long World::getMaxWidth(){
  return mx_width;
}
long World::getMaxHeight(){
  return mx_height;
}
long World::getMaxTall(){
  return mx_tall;
}

int World::getRenderState(int x, int y, int z){
  return renderDat[x][y][z];
}

void World::destroyBlock(int x, int y, int z){
  renderDat[x][y][z] = 0;
  blockDat[x][y][z] = 0;
}
void World::createBlock(int x, int y, int z, int type){
  renderDat[x][y][z] = 1;
  blockDat[x][y][z] = type;
}

int World::getBlockHeight(int x, int z){
  return ((int) (noise->GetNoise((float)x, float(z)) * NOISE_CONSTANT)) + mx_height/2;
}