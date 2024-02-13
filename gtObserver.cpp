#include "gtObserver.hpp"

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

    head->boardInst.board[3][3].type = BLOCKEDTILE;

    head->childCount = 0;
    head->maxKids = INITKIDS;
    head->children = (gtNode**)calloc(INITKIDS, sizeof(gtNode*));
    head->isleaf = true;
    
    if(DBG) fprintf(stderr,"END observer constructor %p\n",head);

}

void gtObserver::updateBoard(MoveList mvl){

    bool found = false;
    uint32_t kidDex = 0;
    gtNode* newhead;

    while(kidDex < head->childCount && head->children[kidDex] && !(mvl == head->children[kidDex]->boardInst.diff)){
        kidDex++;
    }

    for(uint32_t freekid = 0; freekid < head->childCount; freekid++){
        if(freekid != kidDex){
            //need to be recursive free
            delete head->children[freekid];
        }
    }

    monteCarloStore(kidDex);

    newhead = head->children[kidDex];
    free(head);
    head = newhead;


    return;
}

MoveList gtObserver::getBestAction(){
    
    gtNode* newhead;

    newhead = head->children[head->goldenIndex];
    free(head);
    head = newhead;



    return head->boardInst.diff;
}

uint32_t gtObserver::computeLayer(bool doMax){
    if(DBG) fprintf(stderr, "head at %p\n", head);
    return computeLayer(head, doMax, 0);
}

uint32_t gtObserver::computeLayer(gtNode* curNode, bool doMax, uint32_t depth){

    static int compute_count = 0;

    uint32_t val;
    uint32_t bestVal = 0;
    int32_t hardwin = 0;
    gtNode* bestChild;

    uint32_t choice;
    
    //fprintf(stderr,"beginning computation at depth %d, count =  %d\n", depth, compute_count++);
    
    if(curNode->isleaf){
        
        curNode->isleaf = false;

        //expand all possible positions from the current board
        curNode->expandPlaceThreeEmpty();
        curNode->expandBlockTwo();
        curNode->expandClaimOne();
        curNode->expandPlaceAndBlock();

        if(DBG) fprintf(stderr, "finished placing 3\n");

        for(uint32_t child = 0; child < curNode->childCount; child++){

            //evaluate all newely generated children
            val = brian.evaluate(curNode->children[child]->boardInst, &hardwin);

            if(bestVal)


            //if it will be our turn when the current level is accessed then we predetermine the best move
            if(doMax){

                //check for best child
                if(val >= bestVal){
                    bestVal = val;
                    bestChild = (curNode->children[child]);
                    choice = child;
                    curNode->goldenChild(choice);
                }
            }

            //compute all children because we dont know if our opponent is optimal
            else{
                //val = computeLayer((curNode->children[child]), !doMax, depth+1);
                /*if(val <= bestVal){
                    bestVal = val;
                    bestChild = (curNode->children[child]);
                    choice = child;
                    curNode->goldenChild(choice);
                }
                bestChild = (curNode->children[child]);*/
            }
        }

        //alpha pruning (no beta pruning)
        if(doMax && PRUNING){
            curNode->goldenChild(choice);
        }
    }
    else{
        for(uint32_t child = 0; child < curNode->childCount; child++){
            bestVal = computeLayer((curNode->children[child]), !doMax, depth+1);
        }
    }

    if(DBG) fprintf(stderr, "finished expanding layer\n");

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