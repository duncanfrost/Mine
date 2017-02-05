#define REGION_CHUNKS 32*32
class World
{

public:
    World();
    int *blockData[REGION_CHUNKS];



    void SetBlock(int x, int y, int z, int chunk);
    int width;
    int breadth;
    int depth;



};
