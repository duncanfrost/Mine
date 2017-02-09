#define REGION_CHUNKS 32*32
#include "Chunk.h"
#include <vector>

class World
{

public:
    World();

    std::vector<Chunk> chunks;

};
