#include "bot_gen1.hpp"

MoveList BotG1::getNextAction(MoveList mvl){

    MoveList moves;
    gameTree.updateBoard(mvl);

    moves = gameTree.getBestAction();
    gameTree.updateBoard(moves);
    gameTree.computeLayer(false);
    gameTree.computeLayer(false);
    //gameTree.computeLayer((gameTree.head), false);
    return moves;
}

MoveList BotG1::getNextAction(){

    MoveList moves;

    moves = gameTree.getBestAction();
    gameTree.updateBoard(moves);
    gameTree.computeLayer(false);
    return moves;
}