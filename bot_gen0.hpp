#include "includes.h"

#ifndef BOT_G0V0
#define BOT_G0V0



class MoveList{
    public:
    move* moves;
    uint32_t movecount;
    uint64_t seqnum;    

    MoveList(uint32_t count, move* list){
        seqnum = 0;
        movecount = count;
        moves = (move*)calloc(count, sizeof(move));

        for(int mv = 0; mv < movecount; mv++){
            moves[mv] = list[mv];
            seqnum += ((list[mv].x + list[mv].y<<8)<<(16*mv))+list[mv].action<<(48+2*mv);
        }

    }
};

class gtNode{
    public:
    gtNode** kids;
    boardInstance board;
    uint32_t childCount;

    gtNode();

    void computeLayer();
    void expandPlaceThreeEmpty();
    void expandBlockTwo();
    void expandClaimOne();
    void expandPlaceAndBlock();

    int32_t evaluatePosition();
    uint64_t hashPosition();
    void expandKids();
};



class BotG0{

    private:
    gtNode* gameTree;

    public:

    BotG0();

    //TODO: should not be void
    void getNextAction(/*action* moveList*/);
    



};


#endif


/*
take the list of actions taken between the previous game state and current one, hash it, and use it to index into the
desired game state in the tree
*/