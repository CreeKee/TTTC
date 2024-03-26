#include "boardInstance.hpp"
#include "MoveList.hpp"

#ifndef MONTECARLO
#define MONTECARLO

class MCNode{
    public:
    MCNode** children;
    MCNode* parent;

    boardInstance boardInst;
    
    uint32_t wins;
    uint32_t totalSims;

    void runSim();
    void expand();
    void backprop();


};



class MCTree{
    
    private:
    MCNode* head;
    MCNode* cur;

    MCNode* select();
    
    public:
    MoveList getBestAction();
    void updateBoard(MoveList moves);


};

#endif