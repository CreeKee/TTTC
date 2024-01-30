#include "boardInstance.hpp"

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
