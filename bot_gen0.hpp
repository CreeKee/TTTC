#include "includes.h"

//used in getAllMoves() in boardInstance
#define LISTERATION(code)for(int x = 0; x < x_lim; x++){\
        for(int y = 0; y < y_lim; y++){\
            code\
        }\
    }

#ifndef BOT_G0V0
#define BOT_G0V0

class tlist{
    public:
    tnode* head;

    void insert(tnode* newnode){newnode->next = head; newnode->prev = head->prev; head->prev = newnode;}
    void insert(int x, int y){
        tnode* newnode = new tnode;

        newnode->crds.x = x;
        newnode->crds.y = y;

        newnode->next = head; 
        newnode->prev = head->prev; 
        head->prev = newnode;
        }
    coord pop(){
        coord ret;
        tnode* oldhead = head;

        if(head){
            ret.x = head->crds.x;
            ret.y = head->crds.y;
            
            if(head == head->next){
                head = nullptr;
            }
            else{
                head->prev->next = head->next;
                head->next->prev = head->prev;
                head = head->next;
            }

            free(oldhead);
        }
        else{
            ret.x = -1;
            ret.y = -1;
        }

        return ret;
    }
    tlist copy(){
        tlist newlist;
        tnode* cur = head;

        if(cur){
            do{
                newlist.insert(cur->crds.x, cur->crds.y);
                cur = cur->next;
            }while(cur != head);
        }

        return newlist;

    }

    bool isEmpty(){return head == nullptr;}
};

class boardInstance{

    public:

    tile** board;
    uint32_t x_lim;
    uint32_t y_lim;

    uint32_t x_scale;
    uint32_t y_scale;

    MoveList diff;
    uint32_t diffDex;

    boardInstance(){board = nullptr;}
    boardInstance(tile** oldboard, uint32_t newx, uint32_t newy, uint32_t xshift, uint32_t yshift, MoveList oldDiff, uint32_t oldDDex);
    ~boardInstance(){
        for(int col = 0; col < x_lim; col++){
            free(board[col]);
        }
        free(board);
    }

    tlist getAllMoves(int32_t actType);

    void operator=(const boardInstance& B){ 
        board = B.board;
        x_lim = B.x_lim;
        y_lim = B.y_lim;

        x_scale = B.x_scale;
        y_scale = B.y_scale;

        diff = B.diff;
    }


    

};

class MoveList{
    public:
    move moves[3];
    uint32_t actionSelect;
    uint32_t moveDex;

    MoveList(){moveDex = 0; actionSelect = NULLACTION;}

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

class gtNode{
    public:
    gtNode** children;
    uint32_t childCount;
    uint32_t maxKids;
    bool isleaf;
    boardInstance boardInst;
    
    gtNode(){children = nullptr;}
    gtNode(boardInstance incBoard);
    ~gtNode(){
        for(uint32_t kid = 0; kid < childCount; kid++){
            delete children[kid];
            free(children);
        }
    }

    void goldenChild(gtNode gold){
        childCount = 1;
        children = (gtNode**)realloc(children, sizeof(gtNode*));
        *(children[0]) = gold;
    }

    void expandPlaceThreeEmpty();
    void expandBlockTwo();
    void expandClaimOne();
    void expandPlaceAndBlock();

    void computeAction(uint32_t actType, coord crds, tlist remList, uint32_t reps, boardInstance binst);

    void storeBoard(boardInstance boardInst);
};

class gtObserver{
    public:
    gtNode* head;
    gtNode** kids;
    boardInstance board;
    uint32_t childCount;
    

    gtObserver();

    uint32_t computeLayer(gtNode curNode, bool doMax);


    int32_t evaluate(boardInstance boardInst) {return 0;};
    uint64_t hashPosition();
    uint64_t hashMove(move mv);
    void expandKids();

    void updateBoard(MoveList mvl);
    MoveList getBestAction();
};

class BotG0{

    private:
    gtObserver gameTree;

    public:

    BotG0();

    //all move*'s are assumed to be well initiated versions of move moves[3]
    MoveList getNextAction(MoveList moves);

};


#endif


/*
take the list of actions taken between the previous game state and current one, hash it, and use it to index into the
desired game state in the tree
*/