#include "includes.h"
#include "WindowManager.hpp"

#ifndef FMINC
#define FMINC
#include "FieldManager.hpp"
#endif

#define UPDATEDIMS x = x+zset*RATIO; y = y+zset
#define CHECKEXIST (field != nullptr) && (field[x] != nullptr) && (x < fieldSize) && (x >= 0) && (y < fieldSize) && (y >= 0) 
#define INFIELD(a,b) ((a) < fieldSize) && ((a) >= 0) && ((b) < fieldSize) && ((b) >= 0) && field[a][b].owner == curPlayer
#define UPDATELIMS if(x > lim.xmax) lim.xmax = x; else if(x < lim.xmin) lim.xmin = x;\
                   if(y > lim.ymax) lim.ymax = y; else if(y < lim.ymin) lim.ymin = y;

#ifndef GAME_MASTER
#define GAME_MASTER

struct bounds{
    uint32_t xmax;
    uint32_t xmin;
    uint32_t ymax;
    uint32_t ymin;
};

class GameMaster{

    private:
    bounds lim;
    move curmoves[3];
    uint8_t movedex;

    tile** field;

    float ratio;
    float tileOffset;
    float lineWidth;
    
    uint64_t fieldSize;
    uint32_t zset;
    WindowManager* WM;
    FieldManager* FM;

    uint32_t curPlayer;
    uint32_t playerCount;
    
    uint32_t freeTiles;

    

    bool claimTile(int x, int y);
    bool placeEmptyTile(int x, int y);
    bool blockTile(int x, int y);

    bool checkWinCon(int x, int y);

    public:
    GameMaster(int numPlayers);

    void startGame(){
        
        FM->placeTile(INITDIM*2, INITDIM);
        FM->blockTile(INITDIM*2, INITDIM);
        field[INITDIM*2][INITDIM].type = BLOCKEDTILE;

        std::cout << std::endl << 
            "it is player " << curPlayer+1 <<
            "'s turn, choose action:" << std::endl << 
            "1: place 3 empty tiles" << std::endl << 
            "2: block 2 empty tiles" << std::endl <<
            "3: claim 1 empty tile" << std::endl <<
            "4: place 1 empty tile and block 1 empty tile" << std::endl;
    }

    bool checkChoice(int choice);
    bool gameAction(int action, int x, int y, int* winner);
    int getPlayerChoice();
    coord getDesiredTile(); 

    int takeTurn();
    void setWM(WindowManager* newWM){WM = newWM;}
    void setFM(FieldManager* newFM){FM = newFM;}

    tile getTile(int x, int y);
    tile** getField(){return field;}

    boardInstance getBoard();

    move* getMoves(uint8_t* count){*count = movedex; return curmoves;}

    //void readkeys(GLFWwindow* window, int key, int scancode, int action, int mods);
};

#endif