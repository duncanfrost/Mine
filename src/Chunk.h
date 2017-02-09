
class Chunk
{

public:
    Chunk();
    void ComputeDrawData();

    int *blockData;
    bool *drawData;
    int x, z;

};
