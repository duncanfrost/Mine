#define REGION_CHUNKS 32*32
#include "Chunk.h"
#include <vector>
#include <string>
#include <set>
#include "VoxelRenderer.h"

class World
{

public:
    World();

    std::vector<Chunk> chunks;

    void ReadFromFile(std::string path, std::set<int> validIDs);

    void Draw(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, float offset,
              VoxelRenderer vr);

};
