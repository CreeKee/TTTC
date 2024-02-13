#include "includes.h"
#include "boardInstance.hpp"

#ifndef GT_NODE
#define GT_NODE

class gtNode{
    public:
    gtNode** children;
    uint32_t childCount;
    uint32_t maxKids;
    bool isleaf;
    boardInstance boardInst;
    uint32_t goldenIndex;
    
    gtNode(){children = nullptr;}
    gtNode(boardInstance incBoard);
    ~gtNode(){
        //fprintf(stderr, "gtNode destructor\n");

        if(children){
            for(uint32_t kid = 0; kid < childCount; kid++){
                delete children[kid];
            }
            free(children);
        }
    }

    void goldenChild(uint32_t index){
        /*childCount = 1;
        children = (gtNode**)realloc(children, sizeof(gtNode*));
        children[0] = gold;*/
        goldenIndex = index;
    }

    void expandPlaceThreeEmpty();
    void expandBlockTwo();
    void expandClaimOne();
    void expandPlaceAndBlock();

    void computeAction(uint32_t actType, coord crds, tlist remList, uint32_t reps, boardInstance binst);

    void storeBoard(boardInstance boardInst);

    void delve(int depth){
        for(int knum = 0; knum < childCount; knum++){
             fprintf(stderr, "(%d,%d)", depth, knum);
        }
        for(int knum = 0; knum < childCount; knum++){
            children[knum]->delve(depth+1);
        }
       
    }
};


#endif