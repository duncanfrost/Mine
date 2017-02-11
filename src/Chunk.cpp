#include "Chunk.h"
#include "libanvil/region_dim.hpp"
#include <iostream>


inline int ComputeIndex(int x, int y, int z)
{
    return (y * 16 + z) * 16 + x;
}

Chunk::Chunk()
{
    x = 0;
    z = 0;

    blockData = new int[region_dim::BLOCK_COUNT * region_dim::BLOCK_HEIGHT];
    drawData = new bool[region_dim::BLOCK_COUNT * region_dim::BLOCK_HEIGHT];
    for (unsigned int x = 0; x < region_dim::BLOCK_WIDTH; x++)
        for (unsigned int z = 0; z < region_dim::BLOCK_WIDTH; z++)
            for (unsigned int y = 0; y < region_dim::BLOCK_HEIGHT; y++)
            {
                int index = (y * 16 + z) * 16 + x;
                blockData[index] = 0;
            }

}


void Chunk::ComputeDrawData()
{
//    std::cout << "Computing draw data" << std::endl;
    for (unsigned int x = 0; x < region_dim::BLOCK_WIDTH; x++)
        for (unsigned int z = 0; z < region_dim::BLOCK_WIDTH; z++)
            for (unsigned int y = 0; y < region_dim::BLOCK_HEIGHT; y++)
            {
                int index = ComputeIndex(x,y,z);
                drawData[index] = true;
            }

    for (unsigned int x = 1; x < region_dim::BLOCK_WIDTH-1; x++)
        for (unsigned int z = 1; z < region_dim::BLOCK_WIDTH-1; z++)
            for (unsigned int y = 1; y < region_dim::BLOCK_HEIGHT-1; y++)
            {
                int index = ComputeIndex(x,y,z);
                if (blockData[index] < 0)
                    continue;

                int up_index = ComputeIndex(x,y+1,z);
                int down_index = ComputeIndex(x,y-1,z);
                int left_index = ComputeIndex(x-1,y,z);
                int right_index = ComputeIndex(x+1,y,z);
                int forward_index = ComputeIndex(x,y,z-1);
                int backward_index = ComputeIndex(x,y,z+1);

                //If we have reached this point, the block exists

                drawData[index] = true;

                if (!(blockData[up_index] > 0))
                    continue;
                if (!(blockData[down_index] > 0))
                    continue;
                if (!(blockData[left_index]) > 0)
                    continue;
                if (!(blockData[right_index]) > 0)
                    continue;
                if (!(blockData[forward_index]) > 0)
                    continue;
                if (!(blockData[backward_index]) > 0)
                    continue;

                //If we've reached here, there is data in all of the blocks
                //neighbours
                drawData[index] = false;
            }

}
