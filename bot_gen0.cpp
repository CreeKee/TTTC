#include "bot_gen0.hpp"


#define QUICHECK if(gameTree.head->childCount == 0)for(int d = 0; d < DEPTH; d++){\
        gameTree.computeLayer(doMax);\
        doMax = !doMax;\
    }

BotG0::BotG0(uint32_t playerID){

    doMax = (playerID == 0);

    gameTree.brian.setPlayerID(playerID);
    top = gameTree.head;

    if(DBG) fprintf(stderr,"bot constructor\n");

    for(int d = 0; d < DEPTH+2; d++){
        gameTree.computeLayer(doMax);
    }

    //gameTree.traverse();

}

BotG0::BotG0(uint32_t playerID, char* filename){

    doMax = (playerID == 0);
    top = gameTree.head;
    gameTree.brian.setPlayerID(playerID);
    gameTree.brian.readWeights(filename);

    for(int d = 0; d < DEPTH; d++){
        gameTree.computeLayer(doMax);
    }
    //gameTree.traverse();

}

MoveList BotG0::getNextAction(MoveList mvl){

    MoveList moves;
    gameTree.updateBoard(mvl);
    QUICHECK

    moves = gameTree.getBestAction();
    gameTree.updateBoard(moves);
    gameTree.computeLayer(false);
    gameTree.computeLayer(false);
    //gameTree.computeLayer((gameTree.head), false);
    return moves;
}

MoveList BotG0::getNextAction(){

    MoveList moves;

    moves = gameTree.getBestAction();
    gameTree.updateBoard(moves);
    gameTree.computeLayer(false);
    return moves;
}

void BotG0::reset(){

    doMax = (gameTree.brian.playerID == 0);
    fprintf(stderr,"\n\n!!!resetting!!!\n\n");
    delete top;
    gameTree.head = new gtNode;

    for(int d = 0; d < DEPTH; d++){
        gameTree.computeLayer(doMax);
    }
}
