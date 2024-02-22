#include "boardInstance.hpp"

boardInstance::boardInstance(){

    x_lim = BOARDINSTINIT;
    y_lim = BOARDINSTINIT;
    board = new tile*[x_lim]();

    x_scale = 0;
    y_scale = 0;


    for(int col = 0; col < x_lim; col++){
        board[col] = new tile[y_lim]();
    }

    board[(x_lim>>1)][(y_lim>>1)].type = BLOCKEDTILE;

    board[(x_lim>>1)-1][(y_lim>>1)].type = ADJTILE;
    board[(x_lim>>1)+1][(y_lim>>1)].type = ADJTILE;
    board[(x_lim>>1)][(y_lim>>1)-1].type = ADJTILE;
    board[(x_lim>>1)][(y_lim>>1)+1].type = ADJTILE;
}

boardInstance::boardInstance(tile** oldboard, uint32_t newx, uint32_t newy, uint32_t xshift, uint32_t yshift, MoveList oldDiff, uint32_t oldDDex){

    x_lim = newx+(xshift<<1);
    y_lim = newy+(yshift<<1);
    board = new tile*[x_lim]();

    x_scale = 0;
    y_scale = 0;

    diff = oldDiff;

    diffDex = oldDDex;

    for(int col = 0; col < x_lim; col++){
        board[col] = new tile[y_lim]();
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
            if(DBG) fprintf(stderr,"getting all empty tiles %d %d\n", x_lim, y_lim);
            LISTERATION(
                
                if(board[x][y].type != NULLTILE && board[x][y].type != ADJTILE){

                    if(x+1 < x_lim && board[x+1][y].type == NULLTILE){
                        board[x+1][y].type = ADJTILE;
                    }

                    if(y+1 < y_lim && board[x][y+1].type == NULLTILE){
                        board[x][y+1].type = ADJTILE;
                    }

                    if(x > 0 && board[x-1][y].type == NULLTILE){
                        board[x-1][y].type = ADJTILE;
                    }

                    if(y > 0 && board[x][y-1].type == NULLTILE){
                        board[x][y-1].type = ADJTILE;
                    }
                }

            )

            LISTERATION(
                if(board[x][y].type == ADJTILE){
                    newlist.insert(x-(x_lim>>1),y-(y_lim>>1));
                }

            )
            break;

        case CLAIMEDTILE:
        case BLOCKEDTILE:
            LISTERATION(

                if(board[x][y].type == EMPTYTILE){
                    newlist.insert(x-(x_lim>>1),y-(y_lim>>1));
                }

            )
            break;
    }

    return newlist;
}

/*
will automaticall shift dimensions 
*/
bool boardInstance::makeMove(uint32_t actType, coord crds, uint32_t curPlayer){

    //boardInstance newboard = boardInstance(binst.board, binst.x_lim, binst.y_lim, binst.x_scale, binst.y_scale, binst.diff, binst.diffDex);
    int shiftx = crds.x + (x_lim>>1);
    int shifty = crds.y + (y_lim>>1);
    uint32_t curAct = (actType == EBLOCK ? EMPTYTILE : actType);
    tile** oldboard = board;
    bool validMove = false;

    fprintf(stderr, "***%d %d*** >", crds.x, crds.y);

    if(shiftx >= x_lim-4 || shiftx <= 4){
        x_scale += 2;
    }
    if(shifty >= y_lim-4 || shifty <= 4){
        y_scale += 2;
    }

    if(x_scale != 0 || y_scale != 0){

        x_lim = x_lim+(x_scale<<1);
        y_lim = y_lim+(y_scale<<1);

        board = new tile*[x_lim];

        for(int col = 0; col < x_lim; col++){
            board[col] = new tile[y_lim]();//(tile*)calloc(y_lim, sizeof(tile));
        }

        for(int col = x_scale; col < x_lim-x_scale; col++){
            for(int row = y_scale; row < y_lim-y_scale; row++){
                board[col][row] = oldboard[col-x_scale][row-y_scale];
            }
            delete oldboard[col-x_scale];
        }

        x_scale = 0;
        y_scale = 0;

        delete oldboard;
    }


    shiftx = crds.x + (x_lim>>1);
    shifty = crds.y + (y_lim>>1);


    fprintf(stderr, "> ***%d %d***\n", shiftx, shifty);

    switch(curAct){
        case EMPTYTILE:
            if(board[shiftx][shifty].type == ADJTILE){

                //update tile type based on action
                board[shiftx][shifty].type = curAct;

                //update adjacencies an availabilities
                if(board[shiftx+1][shifty].type == NULLTILE){
                    board[shiftx+1][shifty].type = ADJTILE;
                }
                if(board[shiftx-1][shifty].type == NULLTILE){
                    board[shiftx-1][shifty].type = ADJTILE;
                }
                if(board[shiftx][shifty+1].type == NULLTILE){
                    board[shiftx][shifty+1].type = ADJTILE;
                }
                if(board[shiftx][shifty-1].type == NULLTILE){
                    board[shiftx][shifty-1].type = ADJTILE;
                }

                validMove = true;
            }
        break;

        case CLAIMEDTILE:
            if(board[shiftx][shifty].type == EMPTYTILE){
                //update tile type based on action
                board[shiftx][shifty].type = curAct;
                board[shiftx][shifty].owner = curPlayer;

                validMove = true;
            }
            
        break;

        case BLOCKEDTILE:
            if(board[shiftx][shifty].type == EMPTYTILE){
                //update tile type based on action
                board[shiftx][shifty].type = curAct;
                validMove = true;
            }
        break;

        default:
        break;
    }

    return validMove;

}

void boardInstance::displayBoard(){
    for(int x = 0; x<x_lim; x++){
        for(int y = 0; y<y_lim; y++){
            fprintf(stderr, "%d",board[x][y].type);
        }
        fprintf(stderr, "\n");
    }
}
