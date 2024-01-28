#include "bot_gen0.hpp"


gtObserver::gtObserver(){

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

    newhead = head->children[kidDex];
    free(head);
    head = newhead;

}

MoveList gtObserver::getBestAction(){
    
    gtNode* newhead;

    newhead = head->children[0];
    free(head);
    head = newhead;

    return head->boardInst.diff;
}

uint32_t gtObserver::computeLayer(gtNode curNode, bool doMax){

    uint32_t val;
    uint32_t bestVal = 0;
    gtNode bestChild;

    if(curNode.isleaf){
        curNode.isleaf = false;

        //expand all possible positions from the current board
        curNode.expandPlaceThreeEmpty();
        //curNode.expandBlockTwo();
        //curNode.expandClaimOne();
        //curNode.expandPlaceAndBlock();

        for(uint32_t child = 0; child < curNode.childCount; child++){

            //evaluate all newely generated children
            val = evaluate(curNode.children[child]->boardInst);

            //if it will be our turn when the current level is accessed then we predetermine the best move
            if(doMax){

                //check for best child
                if(val >= bestVal){
                    bestVal = val;
                    bestChild = *curNode.children[child];
                }
            }

            //compute all children because we dont know if our opponent is optimal
            else{
                val = computeLayer(*curNode.children[child], !doMax);
                if(val >= bestVal){
                    bestVal = val;
                }
            }
        }

        //alpha pruning (no beta pruning)
        if(doMax && PRUNING){
            curNode.goldenChild(bestChild);
        }
    }
    else{
        for(uint32_t child = 0; child < curNode.childCount; child++){
            bestVal = computeLayer(*curNode.children[child], !doMax);
        }
    }

    return bestVal;
}

/*************************************/

gtNode::gtNode(boardInstance incBoard){
    maxKids = INITKIDS;
    children = (gtNode**)calloc(INITKIDS, sizeof(gtNode*));
    childCount = 0;
    isleaf = true;
    boardInst = incBoard;
}

void gtNode::expandPlaceThreeEmpty(){

    tlist tileList = boardInst.getAllMoves(EMPTYTILE);
    MoveList newDiff;
    newDiff.actionSelect = PLACETHREE;

    boardInstance newboard = boardInstance(boardInst.board, boardInst.x_lim, boardInst.y_lim, 0, 0, newDiff, 0);

    while(!tileList.isEmpty()){
        computeAction(EMPTYTILE, tileList.pop(), tileList.copy(), 3, newboard);
    }
}

void gtNode::computeAction(uint32_t actType, coord crds, tlist remList, uint32_t reps, boardInstance binst){

    boardInstance newboard = boardInstance(binst.board, binst.x_lim, binst.y_lim, binst.x_scale, binst.y_scale, binst.diff, binst.diffDex);

    //update tile type based on action
    newboard.board[crds.x][crds.y].type = actType;
    newboard.diff.addMove(actType, crds.x, crds.y);
    
    if(crds.x > newboard.x_lim-3 || crds.x < 3){
        newboard.x_scale++;
    }
    if(crds.y > newboard.y_lim-3 || crds.y < 3){
        newboard.y_scale++;
    }

    if(actType == CLAIMEDTILE){
        newboard.board[crds.x][crds.y].owner = reps;
    }

    //check whether further action is needed
    if(reps>0 && actType != CLAIMEDTILE){

        //update adjacencies an availabilities
        if(actType == EMPTYTILE){
            if(newboard.board[crds.x+1][crds.y].type == NULLTILE){
                newboard.board[crds.x+1][crds.y].type = ADJTILE;
                remList.insert(crds.x+1, crds.y);
            }
            if(newboard.board[crds.x-1][crds.y].type == NULLTILE){
                newboard.board[crds.x-1][crds.y].type = ADJTILE;
                remList.insert(crds.x-1, crds.y);
            }
            if(newboard.board[crds.x][crds.y+1].type == NULLTILE){
                newboard.board[crds.x][crds.y+1].type = ADJTILE;
                remList.insert(crds.x, crds.y+1);
            }
            if(newboard.board[crds.x][crds.y-1].type == NULLTILE){
                newboard.board[crds.x][crds.y-1].type = ADJTILE;
                remList.insert(crds.x, crds.y-1);
            }
        }
        
        //compute permutations
        while(!remList.isEmpty()){
            computeAction(EMPTYTILE, remList.pop(), remList.copy(), reps-1, newboard);
        }
    }
    else{
        storeBoard(newboard);
    }

}

void gtNode::storeBoard(boardInstance boardInst){

    if(childCount >= maxKids){
        maxKids = maxKids<<1;
        children = (gtNode**)realloc(children, sizeof(gtNode*)*maxKids);
    }

    children[childCount++] = new gtNode(boardInst);

}

/*************************************/

boardInstance::boardInstance(tile** oldboard, uint32_t newx, uint32_t newy, uint32_t xshift, uint32_t yshift, MoveList oldDiff, uint32_t oldDDex){

    x_lim = newx+(xshift<<1);
    y_lim = newy+(yshift<<1);
    board = (tile**)calloc(x_lim, sizeof(tile*));

    x_scale = 0;
    y_scale = 0;

    diff = oldDiff;

    diffDex = oldDDex;

    for(int col = 0; col < x_lim; col++){
        board[col] = (tile*)calloc(y_lim, sizeof(tile));
    }

    for(int col = xshift; col < x_lim-xshift; col++){
        for(int row = yshift; row < y_lim-yshift; row++){
            board[col][row] = oldboard[col-xshift][row-yshift];
        }
    }

    
}

tlist boardInstance::getAllMoves(int32_t actType){
    tlist newlist;


    switch(actType){
        case EMPTYTILE:
            LISTERATION(
                
                if(board[x][y].type == NULLTILE && ((x+1<x_lim && board[x+1][y].type != NULLTILE) || (y+1<y_lim && board[x][y+1].type != NULLTILE))){
                    board[x][y].type = ADJTILE;
                }
                if(board[x][y].type == ADJTILE){
                    newlist.insert(x,y);
                }

            )
            break;

        case CLAIMEDTILE:
        case BLOCKEDTILE:
            LISTERATION(

                if(board[x][y].type == EMPTYTILE){
                    newlist.insert(x,y);
                }

            )
            break;
    }

    return newlist;
}

/*************************************/

BotG0::BotG0(){
}

MoveList BotG0::getNextAction(MoveList mvl){

    MoveList moves;

    gameTree.updateBoard(mvl);
    moves = gameTree.getBestAction();
    gameTree.computeLayer(*(gameTree.head), false);
    return moves;
}

