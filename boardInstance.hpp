#include "includes.h"
#include "tlist.hpp"
#include "MoveList.hpp"

#ifndef BOARD_INSTANCE
#define BOARD_INSTANCE
class boardInstance{

    public:

    tile** board;
    uint32_t x_lim;
    uint32_t y_lim;

    uint32_t x_scale;
    uint32_t y_scale;

    bounds bnds;

    MoveList diff;
    uint32_t diffDex;

    boardInstance();
    boardInstance(tile** oldboard, uint32_t newx, uint32_t newy, uint32_t xshift, uint32_t yshift, MoveList oldDiff, uint32_t oldDDex);
    ~boardInstance(){
        //fprintf(stderr,"boardInst destruct\n");
        for(int col = 0; col < x_lim; col++){
            //delete board[col];
        }
        //delete board;
    }

    tlist getAllMoves(int32_t actType);

    void operator=(const boardInstance& B){ 

        x_lim = B.x_lim;
        y_lim = B.y_lim;

        x_scale = B.x_scale;
        y_scale = B.y_scale;

        diff = B.diff;

        board = (tile**)calloc(x_lim, sizeof(tile*));
        for(int col = 0; col < x_lim; col++){
            board[col] = (tile*)calloc(y_lim, sizeof(tile));
        }


        LISTERATION(
            board[x][y].type = B.board[x][y].type;
            board[x][y].owner = B.board[x][y].owner;
        )
    }

    bool makeMove(uint32_t actType, coord crds, uint32_t curPlayer);
    
    void displayBoard();
};

#endif
