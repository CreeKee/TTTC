#include "includes.h"
#include "gtNode.hpp"
#include "MoveList.hpp"

#ifndef GAME_TREE_OBSERVER
#define GAME_TREE_OBSERVER

class gtObserver{
    public:
    gtNode* head;
    

    gtObserver();

    uint32_t computeLayer(bool doMax);
    uint32_t computeLayer(gtNode* curNode, bool doMax, uint32_t depth);


    int32_t evaluate(boardInstance boardInst);
    uint64_t hashPosition();
    uint64_t hashMove(move mv);
    void expandKids();

    void updateBoard(MoveList mvl);
    MoveList getBestAction();

    void traverse(){
        fprintf(stderr, "doing traverse across %d\n", head->childCount);

        //DISPLAYBOARD(fprintf(stderr, "%d", head->boardInst.board[x][y].type))

        //fprintf(stderr, "\n");
            //fprintf(stderr, "\n");

        for(int i = 0; i < head->childCount; i++){
            
            //DISPLAYBOARD(fprintf(stderr, "%d", head->children[i]->boardInst.board[x][y].type))

            //fprintf(stderr, "\n");
            //fprintf(stderr, "\n");

            //head->children[i]->delve(1);

        }

    }
};


#endif