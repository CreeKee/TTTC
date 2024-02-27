#include "includes.h"

#ifndef MOVE_LIST
#define MOVE_LIST
class MoveList{
    public:
    move moves[3];
    uint32_t actionSelect;
    uint32_t moveDex;

    MoveList(){moveDex = 0; actionSelect = NULLACTION; moves[0].action = NULLACTION; moves[1].action = NULLACTION; moves[2].action = NULLACTION;}

    void addMove(uint32_t actType, uint32_t x, uint32_t y){
        if(moveDex < 3){
            moves[moveDex].action = actType;
            moves[moveDex].x = x;
            moves[moveDex++].y = y;
        }
        else{
            fprintf(stderr, "MoveList move overflow\n");
        }
    }

    void operator=(const MoveList& M){ 
        actionSelect = M.actionSelect;

        moveDex = M.moveDex;

        moves[0] = M.moves[0];
        moves[1] = M.moves[1];
        moves[2] = M.moves[2];
    }

    bool operator==(const MoveList& M){ 
        bool ret = M.moveDex == moveDex;
        bool find = false;

        for(int i = 0; i < moveDex && ret; i++){
            //fprintf(stderr,"%d %d %d / ",moves[i].action, moves[i].x, moves[i].y);
            for(int j = 0; j < moveDex && !find; j++){
                
                find = 
                (M.moves[j].action == moves[i].action) &&
                (M.moves[j].x == moves[i].x) &&
                (M.moves[j].y == moves[i].y);
                //fprintf(stderr,"%d %d %d | ",M.moves[j].action, M.moves[j].x, M.moves[j].y);
            }
            //fprintf(stderr, "find == %d\n",find);
            ret = find;
            find = false;
        }
        return ret;
    }
};
#endif