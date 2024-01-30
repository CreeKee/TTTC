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
        bool ret = true;
        bool find = false;

        for(int i = 0; i < 3 && ret; i++){
            
            for(int j = 0; j < 3 && find == false; j++){

                find = M.moves[j].action != NULLACTION && moves[i].action != NULLACTION &&
                (M.moves[j].action == moves[i].action) &&
                (M.moves[j].x == moves[i].x) &&
                (M.moves[j].y == moves[i].y);
            }
            ret = find;
            find = false;
        }
        return ret;
    }
};
#endif