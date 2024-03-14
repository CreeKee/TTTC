#include "gtNode.hpp"
#include "MoveList.hpp"
#include "tlist.hpp"

gtNode::gtNode(boardInstance* incBoard, gtNode* prev){
    maxKids = INITKIDS;
    children = (gtNode**)calloc(INITKIDS, sizeof(gtNode*));
    childCount = 0;
    isleaf = true;
    atrophy = false;
    boardInst = *incBoard;
    winnable = 0;
    parent = prev;
}

void gtNode::expandPlaceThreeEmpty(){

    tlist tileList = boardInst.getAllMoves(EMPTYTILE);
    MoveList newDiff;
    newDiff.actionSelect = PLACETHREE;

    boardInstance* newboard = new boardInstance(boardInst.board, boardInst.x_lim, boardInst.y_lim, 0, 0, newDiff, 0);

    //place all desired empty tiles
    while(!tileList.isEmpty()){
        computeAction(EMPTYTILE, tileList.pop(), tileList.copy(), 2, newboard);
    }

}

void gtNode::expandBlockTwo(){

    //get all possible moves
    tlist tileList = boardInst.getAllMoves(BLOCKEDTILE);
    MoveList newDiff;
    newDiff.actionSelect = BLOCKTWO;

    //create new board instance
    boardInstance* newboard = new boardInstance(boardInst.board, boardInst.x_lim, boardInst.y_lim, 0, 0, newDiff, 0);

    //compute all possible board permutations
    while(tileList.count > 1){
        computeAction(BLOCKEDTILE, tileList.pop(), tileList.copy(), 1, newboard);
    }
}

bool gtNode::expandClaimOne(uint32_t playerID){

    //get all possible moves
    tlist tileList = boardInst.getAllMoves(CLAIMEDTILE);
    MoveList newDiff;
    newDiff.actionSelect = CLAIMONE;

    //create new board instance
    boardInstance* newboard = new boardInstance(boardInst.board, boardInst.x_lim, boardInst.y_lim, 0, 0, newDiff, 0);

    //compute all possible board permutations
    while(!tileList.isEmpty()){
        if(computeAction(CLAIMEDTILE, tileList.pop(), tileList.copy(), playerID, newboard)) return true;
    }

    return false;
}

void gtNode::expandPlaceAndBlock(){

    //get all possible moves
    tlist tileList = boardInst.getAllMoves(EMPTYTILE);
    MoveList newDiff;
    newDiff.actionSelect = PLACEBLOCK;

    //create new board instance
    boardInstance* newboard = new boardInstance(boardInst.board, boardInst.x_lim, boardInst.y_lim, 0, 0, newDiff, 0);

    //compute all possible board permutations
    while(!tileList.isEmpty()){
        computeAction(EBLOCK, tileList.pop(), tileList.copy(), 1, newboard);
    }

}

bool gtNode::computeAction(uint32_t actType, coord crds, tlist remList, uint32_t reps, boardInstance* binst){
    
    boardInstance* newboard = new boardInstance(binst->board, binst->x_lim, binst->y_lim, binst->x_scale, binst->y_scale, binst->diff, binst->diffDex);
    int shiftx = crds.x + (newboard->x_lim>>1);
    int shifty = crds.y + (newboard->y_lim>>1);
    uint32_t curAct = (actType == EBLOCK ? EMPTYTILE : actType);
    coord crds2;
    //update tile type based on action
    newboard->board[shiftx][shifty].type = curAct;

    //add move to differentiation of the new board instance
    newboard->diff.addMove(curAct, crds.x, crds.y);
    if(curAct == EMPTYTILE){

        //check if board needs to be expanded
        if(shiftx >= newboard->x_lim-4 || shiftx <= 4){
            newboard->x_scale += 2;
        }
        if(shifty >= newboard->y_lim-4 || shifty <= 4){
            newboard->y_scale += 2;
        }
        
        //update adjacencies an availabilities
        if(newboard->board[shiftx+1][shifty].type == NULLTILE){
            newboard->board[shiftx+1][shifty].type = ADJTILE;
            remList.insert(crds.x+1, crds.y);
        }
        if(newboard->board[shiftx-1][shifty].type == NULLTILE){
            newboard->board[shiftx-1][shifty].type = ADJTILE;
            remList.insert(crds.x-1, crds.y);
        }
        if(newboard->board[shiftx][shifty+1].type == NULLTILE){
            newboard->board[shiftx][shifty+1].type = ADJTILE;
            remList.insert(crds.x, crds.y+1);
        }
        if(newboard->board[shiftx][shifty-1].type == NULLTILE){
            newboard->board[shiftx][shifty-1].type = ADJTILE;
            remList.insert(crds.x, crds.y-1);
        }

    }
    
    else if(curAct == CLAIMEDTILE){
        //claim tile
        newboard->board[shiftx][shifty].owner = reps; 
    }
    //check whether further action is needed
    if(reps > 0 && curAct != CLAIMEDTILE){
        
        if(actType == EBLOCK){
            
            //find all possible blocking tiles
            remList = newboard->getAllMoves(BLOCKEDTILE);

            //compute all possible blocking options
            while(!remList.isEmpty()){
                computeAction(BLOCKEDTILE, remList.pop(), remList.copy(), 0, newboard);
            }

        }
        
        //compute permutations
        else if(actType == EMPTYTILE) while(!remList.isEmpty()){
            computeAction(EMPTYTILE, remList.pop(), remList.copy(), reps-1, newboard);
        }
        else if(actType == BLOCKEDTILE) while(!remList.isEmpty()){
            computeAction(BLOCKEDTILE, remList.pop(), remList.copy(), reps-1, newboard);
        }
        
    }
    else{
        storeBoard(newboard);
    }
    return curAct == CLAIMEDTILE ? newboard->checkWinCon(crds, reps) : false;
}

void gtNode::storeBoard(boardInstance* boardInst){

    static int boardCount = 0;

    //check for resizing
    if(childCount >= maxKids){
        //expand child table
        maxKids = maxKids<<1;
        children = (gtNode**)realloc(children, sizeof(gtNode*)*maxKids);
    }
    boardCount++;

    if(boardCount%100000 == 0){
        fprintf(stderr, "board count = %d\n",boardCount);
    }

    //add child to child table
    children[childCount++] = new gtNode(boardInst, this);
}
