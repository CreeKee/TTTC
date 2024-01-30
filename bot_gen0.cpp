#include "bot_gen0.hpp"


BotG0::BotG0(){
    if(DBG) fprintf(stderr,"bot constructor\n");
    gameTree.computeLayer(true);
    gameTree.computeLayer(true);
    gameTree.computeLayer(true);
    gameTree.computeLayer(true);


    gameTree.traverse();

}

MoveList BotG0::getNextAction(MoveList mvl){

    MoveList moves;

    gameTree.updateBoard(mvl);
    moves = gameTree.getBestAction();
    //gameTree.computeLayer((gameTree.head), false);
    return moves;
}

