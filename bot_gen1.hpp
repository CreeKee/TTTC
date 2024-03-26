#include "includes.h"
#include "MoveList.hpp"
#include "Agent.hpp"
#include "MCTree.hpp"

#ifndef BOT_G0V1
#define BOT_G0V1

class BotG1 : private Agent{

    private:
    MCTree gameTree;

    public:
    BotG1(uint32_t playerID);
    BotG1(uint32_t playerID, char* filename);

    MoveList getNextAction(MoveList moves);
    MoveList getNextAction();

    /*
    selection
    expansion
    simulation
    backpropogation
    */
    
};


#endif


/*
take the list of actions taken between the previous game state and current one, hash it, and use it to index into the
desired game state in the tree
*/