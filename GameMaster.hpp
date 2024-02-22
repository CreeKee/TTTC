#include "includes.h"
#include "WindowManager.hpp"
#include "MoveList.hpp"
#include "boardInstance.hpp"

#ifndef FMINC
#define FMINC
#include "FieldManager.hpp"
#endif

#define UPDATEDIMS x = x+zset*RATIO; y = y+zset
#define CHECKEXIST (boardInst.board != nullptr) && (field[x] != nullptr) && (x < fieldSize) && (x >= 0) && (y < fieldSize) && (y >= 0) 
#define INFIELD(a,b)  boardInst.board[a+(boardInst.x_lim>>1)][b+(boardInst.y_lim>>1)].owner == curPlayer
#define UPDATELIMS if(x > lim.xmax) lim.xmax = x; else if(x < lim.xmin) lim.xmin = x;\
                   if(y > lim.ymax) lim.ymax = y; else if(y < lim.ymin) lim.ymin = y;

#ifndef GAME_MASTER
#define GAME_MASTER



class GameMaster{

    private:
    bounds lim;
    MoveList curmoves;
    MoveList prevmoves;
    uint8_t movedex;

    boardInstance boardInst;

    float ratio;
    float tileOffset;
    float lineWidth;
    
    WindowManager* WM;
    FieldManager* FM;

    uint32_t curPlayer;
    uint32_t playerCount;
    
    uint32_t freeTiles;

    

    bool claimTile(coord crds);
    bool placeEmptyTile(coord crds);
    bool blockTile(coord crds);

    bool checkWinCon(coord crds);

    public:
    GameMaster(int numPlayers);

    void startGame(){
        
        if(FM != nullptr) FM->placeTile(INITDIM*2, INITDIM);
        if(FM != nullptr) FM->blockTile(INITDIM*2, INITDIM);

        std::cout << std::endl << 
            "it is player " << curPlayer+1 <<
            "'s turn, choose action:" << std::endl << 
            "1: place 3 empty tiles" << std::endl << 
            "2: block 2 empty tiles" << std::endl <<
            "3: claim 1 empty tile" << std::endl <<
            "4: place 1 empty tile and block 1 empty tile" << std::endl;
    }

    bool checkChoice(int choice);
    bool gameAction(int action, coord crds, int* winner);
    int getPlayerChoice();
    coord getDesiredTile(); 

    int takeTurn();
    int takeTurn(MoveList moves);
    void setWM(WindowManager* newWM){WM = newWM;}
    void setFM(FieldManager* newFM){FM = newFM;}

    tile getTile(coord crds);
    tile** getField(){return boardInst.board;}

    MoveList getCurMoves(){return curmoves;}
    MoveList getPrevMoves(){return prevmoves;}

    void displayBoard(){boardInst.displayBoard();}
    //void readkeys(GLFWwindow* window, int key, int scancode, int action, int mods);
};

#endif