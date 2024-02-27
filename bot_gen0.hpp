#include "includes.h"
#include "MoveList.hpp"
#include "gtObserver.hpp"

#ifndef BOT_G0V0
#define BOT_G0V0

class BotG0{

    private:
    gtObserver gameTree;
    bool doMax;
    
    public:
    BotG0(uint32_t playerID);
    BotG0(uint32_t playerID, char* filename);

    //all move*'s are assumed to be well initiated versions of move moves[3]
    MoveList getNextAction(MoveList moves);
    MoveList getNextAction();

    void updateWeights(){gameTree.brian.scrambleWeights();}
    void saveWeights(char* filename){gameTree.brian.storeWeights(filename);}
    void readWeights(char* filename){gameTree.brian.readWeights(filename);}
    void displayWeights(){gameTree.brian.displayWeights();fprintf(stderr,"\n");}
    
    void dive(){gameTree.dive();}
};


#endif


/*
take the list of actions taken between the previous game state and current one, hash it, and use it to index into the
desired game state in the tree
*/