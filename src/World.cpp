#include "World.h"

World::World()
{
    width = 10;
    height = 10;
    depth = 10;
    blockData = new int[width*height*depth];
    for (unsigned int x = 0; x < width; x++)
        for (unsigned int y = 0; y < height; y++)
            for (unsigned int z = 0; z < depth; z++)
            {
                int index = x + y*width + z*width*depth;
                blockData[index] = 0;
            }
}


void World::SetBlock(int x, int y, int z)
{
    int index = x + y*width + z*width*depth;
    blockData[index] = 1;
}
