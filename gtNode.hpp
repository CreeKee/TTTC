#include "includes.h"
#include "boardInstance.hpp"

#ifndef GT_NODE
#define GT_NODE

class gtNode{
    public:
    
    gtNode** children;
    gtNode* parent;
    int32_t value;
    int32_t winnable;
    uint32_t childCount;
    uint32_t maxKids;
    bool isleaf;
    bool atrophy;
    boardInstance boardInst;
    uint32_t goldenIndex;
    bool wasmax;
    
    gtNode(){
        childCount = 0;
        maxKids = INITKIDS;
        children = (gtNode**)calloc(INITKIDS, sizeof(gtNode*));
        isleaf = true;
        atrophy = false;
        parent = nullptr;
        winnable = 0;
    }
    gtNode(boardInstance* incBoard, gtNode* prev);
    ~gtNode(){
        
        if(children){
            delete [] children;
        }
    }

    void backprop(bool doMax){
        if(parent != nullptr){
            if(doMax){
                if(value > parent->value){
                    parent->value = value;
                    parent->winnable = winnable;
                }
            }
            else if(value < parent->value){
                parent->value = value;
                parent->winnable = winnable;
            }
            parent->backprop(!doMax);
        }
    }
    void backprop(bool doMax, bool force){
        
        if(parent != nullptr){
            //parent->value = value;
            parent->winnable = winnable;
            parent->backprop(!doMax);
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
    bool expandClaimOne(uint32_t playerID);
    void expandPlaceAndBlock();

    bool computeAction(uint32_t actType, coord crds, tlist remList, uint32_t reps, boardInstance* binst);

    void storeBoard(boardInstance* boardInst);

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