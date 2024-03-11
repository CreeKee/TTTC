#include "MoveList.hpp"
#include "boardInstance.hpp"
#include "includes.h"
#include <fstream>
#include <iostream>
#include <random>

#define BOARDTILE(shiftx, shifty) boardInst.board[x+tilex+shiftx][y+tiley+shifty].type

#define SELFCLAIM 10
#define OTHERCLAIM 11
#define PRECEPTCOUNT 18


#ifndef BRAIN_NN
#define BRAIN_NN

struct __attribute__ ((packed)) precepts{
    uint32_t openness;
    uint32_t surface;

    uint32_t selfclaimdiagempty;
    uint32_t selfclaimadjempty;
    uint32_t selfclaimpairdiagempty;
    uint32_t selfclaimpairadjempty;

    uint32_t otherclaimdiagadj;
    uint32_t otherclaimadjadj;
    uint32_t otherclaimpairdiagadj;
    uint32_t otherclaimpairadjadj;

    uint32_t selfclaimpairdiagadj;
    uint32_t selfclaimdiagadj;
    uint32_t selfclaimpairadjadj;
    uint32_t selfclaimadjadj;

    uint32_t otherclaimdiagempty;
    uint32_t otherclaimadjempty;
    uint32_t otherclaimpairdiagempty;
    uint32_t otherclaimpairadjempty;

    uint32_t win;
    uint32_t lose;
};

struct __attribute__ ((packed)) preceptWeights{
    float openness;
    float surface;

    float selfclaimdiagempty;
    float selfclaimadjempty;
    float selfclaimpairdiagempty;
    float selfclaimpairadjempty;

    float otherclaimdiagadj;
    float otherclaimadjadj;
    float otherclaimpairdiagadj;
    float otherclaimpairadjadj;

    float selfclaimpairdiagadj;
    float selfclaimdiagadj;
    float selfclaimpairadjadj;
    float selfclaimadjadj;

    float otherclaimdiagempty;
    float otherclaimadjempty;
    float otherclaimpairdiagempty;
    float otherclaimpairadjempty;
};

class Brain{
    public:
    preceptWeights weights;
    uint32_t playerID;

    Brain(char* filename);
    Brain();

    int32_t evaluate(boardInstance* boardInst, int32_t* hardwin);
    int32_t evaluateLight(boardInstance* boardInst, int32_t* hardwin);
    precepts gatherPrecepts(boardInstance* boardInst);
    precepts gatherPreceptsLight(boardInstance* boardInst);
    void gatherFromEmpty(precepts* traits, bool** checkfield, boardInstance* boardInst, int x, int y, uint32_t len, int dx, int dy, uint32_t prevType);
    void gatherFromAdj(precepts* traits, bool** checkfield, boardInstance* boardInst, int x, int y, uint32_t len, int dx, int dy, uint32_t prevType);

    void storeWeights(char* filename);
    void readWeights(char* filename);
    void scrambleWeights();

    void setPlayerID(uint32_t newid){playerID = newid;}

    void displayWeights(){for(int i = 0; i<PRECEPTCOUNT; i++) fprintf(stderr, "%f\n", ((float*)(&weights))[i]);}
};


#endif