#include "World.h"
#include "libanvil/region_file_reader.hpp"

World::World()
{

}


void World::ReadFromFile(std::string path,
                         std::set<int> validIDs)
{

    region_file_reader reader = region_file_reader(path);
    reader.read();
    for (int rx = 0; rx < 32; rx++)
        for (int rz = 0; rz < 32; rz++)
        {
            // this keeps an exception from being thrown
            // when a non-existant chunk is requested
            if(!reader.is_filled(rx, rz))
                continue;

            // if everything goes well, retrieve the block/height data
            std::vector<int> blocks = reader.get_blocks_at(rx, rz);
            std::vector<int> heights = reader.get_heightmap_at(rx, rz);

            Chunk c;
            c.x = rx;
            c.z = rz;

            for (unsigned int x = 0; x < region_dim::BLOCK_WIDTH; x++)
                for (unsigned int z = 0; z < region_dim::BLOCK_WIDTH; z++)
                    for (unsigned int y = 0; y < region_dim::BLOCK_HEIGHT; y++)
                    {
                        int worldIndex = (y * 16 + z) * 16 + x;
                        int minecraftIndex = (y * 16 + z) * 16 + x;

                        if (minecraftIndex < blocks.size())
                        {
                            int id = blocks[minecraftIndex];
                            if (id > 0 && validIDs.count(id))
                                c.blockData[worldIndex] = id;
                            else
                                c.blockData[worldIndex] = 0;
                        }
                    }
            c.ComputeDrawData();
            chunks.push_back(c);
        }
}

void World::Draw(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, float offset, VoxelRenderer vr)
{

    for (int i = 0; i < chunks.size(); i++)
    {
        Chunk c = chunks[i];
        if (c.x > 4 || c.z > 4)
            continue;
        for (unsigned int x = 0; x < region_dim::BLOCK_WIDTH; x++)
            for (unsigned int z = 0; z < region_dim::BLOCK_WIDTH; z++)
                for (unsigned int y = 0; y < region_dim::BLOCK_HEIGHT; y++)
                {
                    int worldIndex = (y * 16 + z) * 16 + x;
                    if (c.blockData[worldIndex]  > 0 && c.drawData[worldIndex])
                    {
                        int id = c.blockData[worldIndex];
                        glm::vec3 translation(16*c.x + x,offset-y,16*c.z + z);
                        vr.Draw(ProjectionMatrix, ViewMatrix,translation,id);
                    }
                }
    }
}
