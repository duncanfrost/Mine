#include "World.h"

World::World()
{
    width = 16;
    breadth = 16;
    depth = 256;
    blockData = new int[width*breadth*depth];
    for (unsigned int x = 0; x < width; x++)
        for (unsigned int z = 0; z < breadth; z++)
            for (unsigned int y = 0; y < depth; y++)
            {
                int index = x + z*width + y*width*breadth;
                blockData[index] = 0;
            }
}


void World::SetBlock(int x, int y, int z)
{
    int index = x + z*width + y*width*depth;
    blockData[index] = 1;
}
