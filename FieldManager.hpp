#include "includes.h"
#include <cmath>

#ifndef FIELD_MANAGER
#define FIELD_MANAGER

struct shape{
    int type = 0;
};

static const color COLORWHITE = {1.0f, 1.0f, 1.0f};

class FieldManager{

    friend class GameMaster;

    private:

    float ratio;
    float tileOffset;
    float lineWidth;
    int32_t xshift = 0;
    int32_t yshift = 0;


    tile** field;
    //GameMaster* GM;

    vertex* vertField;
    uint64_t vfieldSize;
    uint64_t vcount;
    uint64_t fieldSize;

    void generateRectangle(float x, float y, double rotation);

    public:
    FieldManager(float ratio);

    void setField(tile** newfield){field = newfield;}

    void placeX(int x, int y);
    bool placeTile(int x, int y);
    void placeTri(int x, int y);
    bool blockTile(int x, int y);
    void claimTile(int x, int y, int id);

    vertex* getVField(){return vertField;}
    uint64_t getVCount(){return vcount;}
    //shape getTile(float x, float y);
    position pix2field(float x, float y);
    position field2pix(int x, int y);

};


class Line{
    private:
    public:
        double length;
        double angle;

        vertex verts[6];

    
    Line(double angle, double linewidth, double ratio, double lgth);
    void add(float x, float y);
    vertex get(int index){return verts[index];}
};

#endif
