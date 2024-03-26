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
    int w = 0;

    if(DBG){
        fprintf(stderr,"updating player %d's board with moves: ", brian.playerID);
        mvl.display();
    }
    while(kidDex < head->childCount-1 && head->children[kidDex] && !(mvl == head->children[kidDex]->boardInst.diff)){
        //fprintf(stderr,"%d | %d = ", kidDex, head->children[kidDex]->value);
        //head->children[kidDex]->boardInst.diff.display();
        kidDex++;
    }

    //fprintf(stderr,"%d | %d = ", kidDex, brian.evaluate(&head->children[kidDex]->boardInst, &w));
    //head->children[kidDex]->boardInst.diff.display();

    if(DBG){
        fprintf(stderr,"kidDex == %d out of %d which has diff of: ",kidDex, head->childCount);
        head->children[kidDex]->boardInst.diff.display();
        
    }
    
    for(uint32_t freekid = 0; freekid < head->childCount; freekid++){
        if(freekid != kidDex){

            //need to be recursive free
            delete head->children[freekid];
            head->children[freekid] = nullptr;
        }
    }

    head = head->children[kidDex];
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

    uint32_t choice = 0;
    
    //record if it was max's or min's turn
    curNode->wasmax = doMax;

    //limit recursion depth
    /*
    if(depth >= DEPTH){
        return 0;
    }*/


    if(curNode->isleaf){
        //fprintf(stderr, "compute count %d\n", compute_count++);

        //clear flags
        curNode->isleaf = false;
        curNode->atrophy = false;

        //expand all possible positions from the current board
        
        if(!curNode->expandClaimOne(brian.playerID)){
            
            curNode->expandPlaceThreeEmpty();
            curNode->expandBlockTwo();
            curNode->expandPlaceAndBlock();
        }
        


        //evaluate all newely generated children
        for(uint32_t child = 0; child < curNode->childCount && !(hardwin == 1); child++){
            val = brian.evaluate(&curNode->children[child]->boardInst, &hardwin);
            curNode->children[child]->value = val;  
            curNode->children[child]->wasmax = !doMax;
            curNode->children[child]->winnable = hardwin;
            if(hardwin == -1){
                curNode->atrophy = true;
                
            }
            else if(hardwin == 1){
                curNode->goldenChild(child);
                
            }
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


    if(hardwin != 1){

        bestVal = curNode->children[0]->value;
        for(uint32_t child = 1; child < curNode->childCount; child++){

            //get value from current node
            val = curNode->children[child]->value;

            //check if its max's turn
            if(doMax){

                //check if current child is of high value
                if((val > bestVal)){
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
                if((val < bestVal)){
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
    }

    return bestVal;
}

void gtObserver::reset(){

    delete head;
    head = new gtNode;

}
