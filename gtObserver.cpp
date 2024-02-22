#include "gtObserver.hpp"
#include "limits.h"

gtObserver::gtObserver(){
    fprintf(stderr,"observer constructor\n");

    head = new gtNode;
    
    head->boardInst.x_lim = BOARDINSTINIT;
    head->boardInst.y_lim = BOARDINSTINIT;
    head->boardInst.board = (tile**)calloc(head->boardInst.x_lim, sizeof(tile*));

    head->boardInst.x_scale = 0;
    head->boardInst.y_scale = 0;


    for(int col = 0; col < head->boardInst.x_lim; col++){
        head->boardInst.board[col] = (tile*)calloc(head->boardInst.y_lim, sizeof(tile));
    }

    head->boardInst.board[BOARDINSTINIT>>1][BOARDINSTINIT>>1].type = BLOCKEDTILE;

    head->childCount = 0;
    head->maxKids = INITKIDS;
    head->children = (gtNode**)calloc(INITKIDS, sizeof(gtNode*));
    head->isleaf = true;
    head->atrophy = false;
    
    if(DBG) fprintf(stderr,"END observer constructor %p\n",head);

}

void gtObserver::updateBoard(MoveList mvl){

    bool found = false;
    uint32_t kidDex = 0;
    gtNode* newhead;

    while(kidDex < head->childCount-1 && head->children[kidDex] && !(mvl == head->children[kidDex]->boardInst.diff)){
        kidDex++;
    }

    fprintf(stderr,"kidDex == %d out of %d\n",kidDex, head->childCount);

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
    fprintf(stderr, "getting best child at index %d out of %d kids. was max? %d\n", head->goldenIndex, head->childCount, head->wasmax);
    if(head->isleaf) for(int depth = 0; depth < DEPTH; depth++){
        computeLayer(doMax);
        doMax = !doMax;
    }
    return head->children[head->goldenIndex]->boardInst.diff;
}

uint32_t gtObserver::computeLayer(bool doMax){
    //fprintf(stderr, "head at %p\n", head);
    return computeLayer(head, doMax, 0);
}

uint32_t gtObserver::computeLayer(gtNode* curNode, bool doMax, uint32_t depth){

    static int compute_count = 0;

    int32_t val;
    int32_t bestVal = doMax == true ? INT_MIN : INT_MAX;
    int32_t hardwin = 0;
    gtNode* bestChild;

    uint32_t choice = 2;
    
    //fprintf(stderr,"beginning computation at depth %d, count =  %d\n", depth, compute_count++);
    curNode->wasmax = doMax;
    if(depth >= DEPTH){
        return 0;
    }

    if(curNode->isleaf ){
        curNode->isleaf = false;
        curNode->atrophy = false;

        //expand all possible positions from the current board
        curNode->expandPlaceThreeEmpty();
        curNode->expandBlockTwo();
        curNode->expandClaimOne();
        curNode->expandPlaceAndBlock();

        //fprintf(stderr, "finished placing 3\n");

        for(uint32_t child = 0; child < curNode->childCount && !(hardwin); child++){
            //evaluate all newely generated children
            val = brian.evaluate(curNode->children[child]->boardInst, &hardwin);
            curNode->children[child]->value = val;            
        }

        //propogate values to parents
        curNode->children[0]->backprop(doMax, true);
        for(uint32_t child = 1; child < curNode->childCount; child++){
            curNode->children[child]->backprop(doMax);
        }
    }
    else{
        for(uint32_t child = 0; child < curNode->childCount; child++){
            if(!curNode->children[child]->atrophy) bestVal = computeLayer((curNode->children[child]), !doMax, depth+1);


        }
    }

    for(uint32_t child = 0; child < curNode->childCount; child++){

        //get value from current node
        val = curNode->children[child]->value;

        
        if(doMax && (val > bestVal || hardwin == 1)){
            bestVal = val;
            choice = child;
        }
        else if(!doMax && (val < bestVal || hardwin == -1)){
            bestVal = val;
        }

        if(doMax && val <= ATROPHYRATIO * bestVal){
            curNode->children[child]->atrophy = true;
        }
        else if(!doMax && val >= ATROPHYRATIO * bestVal){
            curNode->children[child]->atrophy = true;
        }
    }

    //store index of best child
    if(doMax && PRUNING || (doMax && hardwin) || true){
        curNode->goldenChild(choice);
    }
    

    if(choice == 2) fprintf(stderr, "choice is 2, bestVal is %d doMax is %d from player %d\n",bestVal, doMax, brian.playerID);


    return bestVal;
}

int32_t gtObserver::evaluate(boardInstance boardInst) {

    for(int y = 0; y < boardInst.y_lim-1; y++){
        for(int x = 0; x < boardInst.x_lim-1; x++){
            if(boardInst.board[x][y].type == CLAIMEDTILE){

            }
        }
    }
    
    return 1;
}

void gtObserver::monteCarloStore(uint32_t choice){
    
    uint32_t* num = new uint32_t;
    
    file.read((char*)num, 4);

    if(*num == -1){
        //something has gone wrong
    }
    if(choice != *num){

    }
}