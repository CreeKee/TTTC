#include "includes.h"
#include "gtNode.hpp"
#include "MoveList.hpp"
#include "Brain.hpp"
#include <iostream>
#include <fstream>

#ifndef GAME_TREE_OBSERVER
#define GAME_TREE_OBSERVER

class MCNode{
    MCNode* nextLevel;
    MCNode* parent;
    uint32_t kidCount;

};





class gtObserver{
    public:
    gtNode* head;
    std::fstream file;
    Brain brian;

    gtObserver();

    uint32_t computeLayer(bool doMax);
    uint32_t computeLayer(gtNode* curNode, bool doMax, uint32_t depth);

    uint64_t hashPosition();
    uint64_t hashMove(move mv);
    void expandKids();

    void updateBoard(MoveList mvl);
    MoveList getBestAction();

    void traverse(){
        fprintf(stderr, "doing traverse across %d\n", head->childCount);

        DISPLAYBOARD(head->boardInst.x_lim, head->boardInst.y_lim,fprintf(stderr, "%d", head->boardInst.board[x][y].type))

        fprintf(stderr, "\n");
            fprintf(stderr, "\n");

        for(int i = 0; i < head->childCount; i++){
            
            DISPLAYBOARD(head->children[i]->boardInst.x_lim, head->children[i]->boardInst.y_lim,fprintf(stderr, "%d", head->children[i]->boardInst.board[x][y].type))

            fprintf(stderr, "This node has dims of %dx%d and: %d children\n", head->children[i]->boardInst.x_lim, head->children[i]->boardInst.y_lim,head->children[i]->childCount);
            fprintf(stderr, "\n");

            //head->children[i]->delve(1);

        }

    }
    void dive(){
        int depth = 0;
        gtNode* cur = head;
        while(!cur->isleaf){
            fprintf(stderr, "playerID: %d depth: %d, wasmax %d, is leaf: %d, was atrophy: %d, golden child at %d\n", brian.playerID, depth++, cur->wasmax, cur->isleaf, cur->atrophy, cur->goldenIndex);
            cur = cur->children[cur->goldenIndex];
        }
        fprintf(stderr, "playerID: %d depth: %d, wasmax %d, is leaf: %d, was atrophy: %d, golden child at %d\n", brian.playerID, depth++, cur->wasmax, cur->isleaf, cur->atrophy, cur->goldenIndex);
        fprintf(stderr, "\n");
    }

    void reset();
};


#endif