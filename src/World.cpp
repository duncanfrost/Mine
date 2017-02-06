#include "World.h"

World::World()
{
    width = 16;
    breadth = 16;
    depth = 256;

    for (int chunk = 0; chunk < REGION_CHUNKS; chunk++)
    {
        blockData[chunk] = new int[width*breadth*depth];
        for (unsigned int x = 0; x < width; x++)
            for (unsigned int z = 0; z < breadth; z++)
                for (unsigned int y = 0; y < depth; y++)
                {
                    int index = (y * 16 + z) * 16 + x;
                    blockData[chunk][index] = 0;
                }
    }
}


void World::SetBlock(int x, int y, int z, int chunk)
{
    int index = x + z*width + y*width*depth;
    blockData[chunk][index] = 1;
}
