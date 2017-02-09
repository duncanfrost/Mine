#include "Chunk.h"
#include "libanvil/region_dim.hpp"

Chunk::Chunk()
{
    x = 0;
    z = 0;

    blockData = new int[region_dim::BLOCK_COUNT * region_dim::BLOCK_HEIGHT];
    for (unsigned int x = 0; x < region_dim::BLOCK_WIDTH; x++)
        for (unsigned int z = 0; z < region_dim::BLOCK_WIDTH; z++)
            for (unsigned int y = 0; y < region_dim::BLOCK_HEIGHT; y++)
            {
                int index = (y * 16 + z) * 16 + x;
                blockData[index] = 0;
            }

}
