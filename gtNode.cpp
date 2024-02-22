#include "gtNode.hpp"
#include "MoveList.hpp"
#include "tlist.hpp"

gtNode::gtNode(boardInstance incBoard, gtNode* prev){
    maxKids = INITKIDS;
    children = (gtNode**)calloc(INITKIDS, sizeof(gtNode*));
    childCount = 0;
    isleaf = true;
    atrophy = false;
    boardInst = incBoard;

    parent = prev;
}

void gtNode::expandPlaceThreeEmpty(){
    tlist tileList = boardInst.getAllMoves(EMPTYTILE);
    MoveList newDiff;
    newDiff.actionSelect = PLACETHREE;

    boardInstance newboard = boardInstance(boardInst.board, boardInst.x_lim, boardInst.y_lim, 0, 0, newDiff, 0);

    while(!tileList.isEmpty()){
        //fprintf(stderr, "list size %d\n", tileList.count);
        computeAction(EMPTYTILE, tileList.pop(), tileList.copy(), 2, newboard);
        if(DBG) fprintf(stderr, "list size after compute %d but empty == %d\n", tileList.count, tileList.isEmpty());
    }
}

void gtNode::expandBlockTwo(){

    //get all possible moves
    tlist tileList = boardInst.getAllMoves(BLOCKEDTILE);
    MoveList newDiff;
    newDiff.actionSelect = BLOCKTWO;

    //create new board instance
    boardInstance newboard = boardInstance(boardInst.board, boardInst.x_lim, boardInst.y_lim, 0, 0, newDiff, 0);

    //compute all possible board permutations

    while(tileList.count > 1){
        computeAction(BLOCKEDTILE, tileList.pop(), tileList.copy(), 1, newboard);
    }
}

void gtNode::expandClaimOne(){
    //get all possible moves
    tlist tileList = boardInst.getAllMoves(CLAIMEDTILE);
    MoveList newDiff;
    newDiff.actionSelect = CLAIMONE;

    //create new board instance
    boardInstance newboard = boardInstance(boardInst.board, boardInst.x_lim, boardInst.y_lim, 0, 0, newDiff, 0);

    //compute all possible board permutations

    while(!tileList.isEmpty()){
        computeAction(CLAIMEDTILE, tileList.pop(), tileList.copy(), 0, newboard);
    }
}

void gtNode::expandPlaceAndBlock(){
        //get all possible moves
    tlist tileList = boardInst.getAllMoves(EMPTYTILE);
    MoveList newDiff;
    newDiff.actionSelect = PLACEBLOCK;

    //create new board instance
    boardInstance newboard = boardInstance(boardInst.board, boardInst.x_lim, boardInst.y_lim, 0, 0, newDiff, 0);

    //compute all possible board permutations

    while(!tileList.isEmpty()){
        computeAction(EBLOCK, tileList.pop(), tileList.copy(), 1, newboard);
    }


}

void gtNode::computeAction(uint32_t actType, coord crds, tlist remList, uint32_t reps, boardInstance binst){
    
    boardInstance newboard = boardInstance(binst.board, binst.x_lim, binst.y_lim, binst.x_scale, binst.y_scale, binst.diff, binst.diffDex);
    int shiftx = crds.x + (newboard.x_lim>>1);
    int shifty = crds.y + (newboard.y_lim>>1);
    uint32_t curAct = (actType == EBLOCK ? EMPTYTILE : actType);

    //update tile type based on action
    newboard.board[shiftx][shifty].type = curAct;

    newboard.diff.addMove(curAct, crds.x, crds.y);

    if(curAct == EMPTYTILE){
        if(shiftx >= newboard.x_lim-4 || shiftx <= 4){
            newboard.x_scale += 2;
        }
        if(shifty >= newboard.y_lim-4 || shifty <= 4){
            newboard.y_scale += 2;
        }
        
        //fprintf(stderr, "pickles %d %d %d %d %d %d\n",crds.x, crds.y, shiftx, shifty, newboard.x_lim, newboard.y_lim);

        //update adjacencies an availabilities
        if(newboard.board[shiftx+1][shifty].type == NULLTILE){
            newboard.board[shiftx+1][shifty].type = ADJTILE;
            remList.insert(crds.x+1, crds.y);
        }
        if(newboard.board[shiftx-1][shifty].type == NULLTILE){
            newboard.board[shiftx-1][shifty].type = ADJTILE;
            remList.insert(crds.x-1, crds.y);
        }
        if(newboard.board[shiftx][shifty+1].type == NULLTILE){
            newboard.board[shiftx][shifty+1].type = ADJTILE;
            remList.insert(crds.x, crds.y+1);
        }
        if(newboard.board[shiftx][shifty-1].type == NULLTILE){
            newboard.board[shiftx][shifty-1].type = ADJTILE;
            remList.insert(crds.x, crds.y-1);
        }
        //fprintf(stderr, "flag\n");
    }
    else if(curAct == CLAIMEDTILE){
        newboard.board[shiftx][shifty].owner = reps;
    }
    //check whether further action is needed
    if(reps > 0 && curAct != CLAIMEDTILE){
        
        if(actType == EBLOCK){
            
            remList = newboard.getAllMoves(BLOCKEDTILE);

            while(!remList.isEmpty()){
                computeAction(BLOCKEDTILE, remList.pop(), remList.copy(), 0, newboard);
            }

        }
        
        //compute permutations
        else while(!remList.isEmpty()){
            computeAction(EMPTYTILE, remList.pop(), remList.copy(), reps-1, newboard);
        }
    }
    else{
        storeBoard(newboard);
    }


}

void gtNode::storeBoard(boardInstance boardInst){

    static int boardCount = 0;

    if(childCount >= maxKids){
        maxKids = maxKids<<1;
        children = (gtNode**)realloc(children, sizeof(gtNode*)*maxKids);
    }

    children[childCount++] = new gtNode(boardInst, this);

    //fprintf(stderr, "finished storing board %d\n", boardCount++);
}
