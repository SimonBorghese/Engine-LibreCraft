#ifndef WORLD_H
#define WORLD_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include "../include/fastnoise/FastNoiseLite.hpp"
#include "../include/glm/glm.hpp"
#include <math.h>
#include <map>
#include <exception>

#define MAX_SIZE 2000
#define NOISE_CONSTANT 10
/*
New World Idea:

Follow Minecraft Spec chunks 16 * 255 * 16 (x*y*z)

The world should be regenerated as it needs to be loaded in

When a change is made, it is stored in a seperate array and file where changes are kept

When the world is loaded, the base version should be loaded first then each block can be overridden as needed

Example:
What's generated:
(0,1,1) = 0
World file has:
(0,1,1) = 5
Thus final cordinate is:
(0,1,1) = 5

*/

typedef uint32_t POSITION_INT;
typedef uint32_t BLOCK_INT;


class World{
  public:
    World();
    virtual ~World();

    BLOCK_INT getBlockState(int x, int y, int z);

    int getBlockHeight(int x, int z);
  private:
  

    std::map<std::tuple<POSITION_INT, POSITION_INT, POSITION_INT>, BLOCK_INT> worldOverrides;
    FastNoiseLite *noise;

    void destroyBlock(int x, int y, int z);
    void createBlock(int x, int y, int z, int type);

    std::tuple<POSITION_INT, POSITION_INT, POSITION_INT> primitivePostoPos(int x, int y, int z);
};

#endif