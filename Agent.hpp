#include "includes.h"
#include "MoveList.hpp"
#include "gtObserver.hpp"

#ifndef BASE_AGENT
#define BASE_AGENT

class Agent{

    private:
    Brain brian;
    
    public:

    MoveList getNextAction(MoveList moves);
    MoveList getNextAction();

    void updateWeights(){brian.scrambleWeights();}
    void saveWeights(char* filename){brian.storeWeights(filename);}
    void readWeights(char* filename){brian.readWeights(filename);}
    void displayWeights(){brian.displayWeights();fprintf(stderr,"\n");}
    
    void reset();

    void showCurrentBoard();
};


#endif


/*
take the list of actions taken between the previous game state and current one, hash it, and use it to index into the
desired game state in the tree
*/