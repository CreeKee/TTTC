#include "gtObserver.hpp"
#include "limits.h"

gtObserver::gtObserver(){
    fprintf(stderr,"observer constructor\n");

    head = new gtNode;
    
    if(DBG) fprintf(stderr,"END observer constructor %p\n",head);

}

void gtObserver::updateBoard(MoveList mvl){

    bool found = false;
    uint32_t kidDex = 0;
    gtNode* newhead;

    fprintf(stderr,"updating player %d's board with moves: ", brian.playerID);
    for(int i = 0; i < mvl.moveDex; i++){
        fprintf(stderr,"%d@(%d,%d) ",mvl.moves[i].action,mvl.moves[i].x,mvl.moves[i].y);
    }
    fprintf(stderr,"\n");

    while(kidDex < head->childCount-1 && head->children[kidDex] && !(mvl == head->children[kidDex]->boardInst.diff)){
        kidDex++;
    }

    fprintf(stderr,"kidDex == %d out of %d which has diff of: ",kidDex, head->childCount);
    for(int i = 0; i < head->children[kidDex]->boardInst.diff.moveDex; i++){
        fprintf(stderr,"%d@(%d,%d) ",head->children[kidDex]->boardInst.diff.moves[i].action, head->children[kidDex]->boardInst.diff.moves[i].x,head->children[kidDex]->boardInst.diff.moves[i].y);
    }
    fprintf(stderr,"\n");

    for(uint32_t freekid = 0; freekid < head->childCount; freekid++){
        if(freekid != kidDex){
            //need to be recursive free
            delete head->children[freekid];
        }
    }

    newhead = head->children[kidDex];
    free(head);
    head = newhead;
    fprintf(stderr, "finished updating board\n");
    head->parent = nullptr;
    return;
}

MoveList gtObserver::getBestAction(){
    
    gtNode* newhead;
    bool doMax = true;
    
    //if we are at the bottom of the game tree, expand it to ensure we have moves to make
    if(head->isleaf) for(int depth = 0; depth < DEPTH; depth++){
        computeLayer(doMax);
        doMax = !doMax;
    }

    //select golden child
    return head->children[head->goldenIndex]->boardInst.diff;
}

uint32_t gtObserver::computeLayer(bool doMax){
    return computeLayer(head, doMax, 0);
}

uint32_t gtObserver::computeLayer(gtNode* curNode, bool doMax, uint32_t depth){

    static int compute_count = 0;

    int32_t val;
    int32_t bestVal = doMax == true ? INT_MIN : INT_MAX;
    int32_t hardwin = 0;
    gtNode* bestChild;

    uint32_t choice = 2;
    
    //record if it was max's or min's turn
    curNode->wasmax = doMax;

    //limit recursion depth
    /*
    if(depth >= DEPTH){
        return 0;
    }*/


    if(curNode->isleaf){

        //clear flags
        curNode->isleaf = false;
        curNode->atrophy = false;

        //expand all possible positions from the current board
        curNode->expandPlaceThreeEmpty();
        curNode->expandBlockTwo();
        curNode->expandClaimOne(brian.playerID);
        curNode->expandPlaceAndBlock();

        //evaluate all newely generated children
        for(uint32_t child = 0; child < curNode->childCount && !(hardwin); child++){
            val = brian.evaluateLight(curNode->children[child]->boardInst, &hardwin);
            curNode->children[child]->value = val;  
            curNode->children[child]->wasmax = !doMax;
        }

        //propogate values to parents
        curNode->children[0]->backprop(doMax, true);
        for(uint32_t child = 1; child < curNode->childCount; child++){
            curNode->children[child]->backprop(doMax);
        }
    }
    else{

        //expand all child nodes
        for(uint32_t child = 0; child < curNode->childCount; child++){

            //skip atrophied nodes
            if(!curNode->children[child]->atrophy) computeLayer((curNode->children[child]), !doMax, depth+1);
        }
    }

    for(uint32_t child = 0; child < curNode->childCount; child++){

        //get value from current node
        val = curNode->children[child]->value;

        //check if its max's turn
        if(doMax){

            //check if current child is of high value
            if((val > bestVal) || (hardwin == 1)){
                bestVal = val;
                choice = child;
            }

            //atrophy current child if its value is too low
            else if(val <= ATROPHYRATIO * bestVal ){
                curNode->children[child]->atrophy = true;
            }
            
        }

        //if it is min's turn
        else{

            //check if current child is of low value
            if((val < bestVal) || (hardwin == -1)){
                bestVal = val;
                choice = child;
            }

            //atrophy current child if its value is too high
            else if(val >= ATROPHYRATIO * bestVal+1 ){
                curNode->children[child]->atrophy = true;
            }
        }
    }

    //store index of best child
    curNode->goldenChild(choice);
    

    return bestVal;
}

void gtObserver::reset(){

    delete head;
    head = new gtNode;

}
