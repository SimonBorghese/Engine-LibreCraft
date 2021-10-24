#ifndef WORLD_H
#define WORLD_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include "../include/fastnoise/FastNoiseLite.hpp"
#include "../include/glm/glm.hpp"
#include <math.h>

#define MAX_SIZE 2000
#define NOISE_CONSTANT 10
/*
World idea:

3D array containing block data
X Y Z
Texutre to use, block data

3D array containing render data
X Y Z
Wheather the block is rendered or not

flat world generation:
Every value gets dirt
Every value that has a 0 in one of it's components gets rendered
Other values become verified unrendered

When a block value is changed
All blocks around get tested

When a block is deleted
First becomes marked as air
Already rendered blocks are ignored
If a verified empty is found, it becomes rendered
*/

class World{
  public:
    World(const int width, const int height, const int tall, const long max_width, const long max_height, const long max_tall);
    virtual ~World();

    int getWidth();
    int getHeight();
    int getTall(); // You grow to 6ft if you call this method

    long getMaxWidth();
    long getMaxHeight();
    long getMaxTall();

    int getRenderState(int x, int y, int z);

    int getBlockHeight(int x, int z);
  private:
    int w_width, w_height, w_tall;
    long mx_width, mx_height, mx_tall;

    unsigned char blockDat[MAX_SIZE+1][MAX_SIZE+1][MAX_SIZE+1];
    unsigned char renderDat[MAX_SIZE+1][MAX_SIZE+1][MAX_SIZE+1];

    FastNoiseLite *noise;

    void destroyBlock(int x, int y, int z);
    void createBlock(int x, int y, int z, int type);
};

#endif