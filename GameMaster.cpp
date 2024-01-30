#include "GameMaster.hpp"



GameMaster::GameMaster(int numPlayers){
    playerCount = numPlayers;
    freeTiles = 0;
    

    fieldSize = INITDIM*8+1;
    zset = INITDIM;
    field = (tile**)calloc(sizeof(tile*), (fieldSize));

    for(int rows = 0; rows < fieldSize; rows++){
        field[rows] = (tile*)calloc(sizeof(tile), (fieldSize));
        for(int cols = 0; cols <fieldSize; cols++){
            field[rows][cols].owner = -1;
        }
    }

    lim.xmax = 0;
    lim.xmin = fieldSize;
    lim.xmax = 0;
    lim.xmin = fieldSize;

    curPlayer = 0;

    movedex = 0;

}

bool GameMaster::claimTile(int x, int y){

    bool ret = false;

    if(CHECKEXIST && field[x][y].type == EMPTYTILE){
        field[x][y].type = CLAIMEDTILE;
        field[x][y].owner = curPlayer;

        FM->claimTile(x, y, curPlayer);

        freeTiles--;
        ret = true;
    }

    return ret;

}

bool GameMaster::blockTile(int x, int y){

    bool ret = false;
    fprintf(stderr, "tile %d %d is of type %d\n",x,y,field[x][y].type);
    if(CHECKEXIST && field[x][y].type == EMPTYTILE){
        field[x][y].type = BLOCKEDTILE;
        FM->blockTile(x, y);
        freeTiles--;
        ret = true;
    }

    return ret;

}

bool GameMaster::placeEmptyTile(int x, int y){

    bool ret = false;
    fprintf(stderr, "tile %d %d is of type %d\n",x,y,field[x][y].type);
    if(CHECKEXIST && field[x][y].type == NULLTILE){
        if((x<fieldSize && field[x+1][y].type != NULLTILE) || (x>0 && field[x-1][y].type != NULLTILE) || (y<fieldSize && field[x][y+1].type != NULLTILE) || (y>0 && field[x][y-1].type != NULLTILE)){
            field[x][y].type = EMPTYTILE;
            FM->placeTile(x, y);
            freeTiles++;
            ret = true;
        }
        
    }
    
    return ret;
}

bool GameMaster::checkWinCon(int x, int y){

    int lined = 0;
    bool ret = false;

    if(INFIELD(x-1,y)){
        lined++;
        if(INFIELD(x-2,y)) lined++;
    }
    if(INFIELD(x+1,y)){
        lined++;
        if(INFIELD(x+2,y)) lined++;
    }
    ret = (lined >= 2);
    lined = 0;  

    if(!ret){
        if( INFIELD(x,y-1)){
            lined++;
            if(INFIELD(x,y-2)) lined++;
        }
        if(INFIELD(x,y+1)){
            lined++;
            if(INFIELD(x,y+2)) lined++;
        }
        ret = (lined >= 2);
        lined = 0;  
    }

    if(!ret){
        if( INFIELD(x-1,y-1)){
            lined++;
            if(INFIELD(x-2,y-2)) lined++;
        }
        if(INFIELD(x+1,y+1)){
            lined++;
            if(INFIELD(x+2,y+2)) lined++;
        }
        ret = (lined >= 2);
        lined = 0;  
    }

    if(!ret){
        if( INFIELD(x+1,y-1)){
            lined++;
            if(INFIELD(x+2,y-2)) lined++;
        }
        if(INFIELD(x-1,y+1)){
            lined++;
            if(INFIELD(x-2,y+2)) lined++;
        }
        ret = (lined >= 2);
        lined = 0;  
    }

    return ret;
}

bool GameMaster::gameAction(int action, int x, int y, int* winner){

    bool ret = false;

    //UPDATEDIMS;  

    switch(action){
        case CLAIM:
            if(ret = claimTile(x, y)){
                if(checkWinCon(x, y)){
                    *winner = curPlayer; 
                }
            }
            break;

        case PLACE:
            ret = placeEmptyTile(x,y);
            break;

        case BLOCK:
            ret = blockTile(x,y);
            break;

        default:
            break;
    }

    if(ret){
        UPDATELIMS;
        curmoves.addMove(action, x, y);
    }

    return ret;
}

int GameMaster::takeTurn(){
    static int32_t choice = -1;
    static int32_t select = -1;
    int winner = -1;
    static uint32_t phase = READY;
    static uint32_t steps = 0;
    coord pos;

    if(phase == READY) choice = getPlayerChoice();
    if(choice != -1 && phase == READY){select = choice;}
    if(select == -1){WM->click.pressed = false;}

    if(select != -1 || steps > 0){
       
        switch(select){
            case PLACETHREE:
                if(phase == READY) steps = 3;

                pos = getDesiredTile();
                
                if(pos.x >= 0 && pos.y >= 0){
                    if(gameAction(PLACE, pos.x, pos.y, &winner)){
                        printf("wee %d %d %d\n", select, steps, phase);
                        steps--;
                        phase = ACTIVE;
                        printf("wee2 %d %d %d\n", select, steps, phase);
                    } 
                    if(steps == 0) phase = END;
                }
                
                break;

            case BLOCKTWO:

                if(phase == READY) steps = 2;
                
                pos = getDesiredTile();
                if(pos.x >= 0 && pos.y >= 0){
                    if(gameAction(BLOCK, pos.x, pos.y, &winner)){
                        steps--;
                        phase = ACTIVE;
                    }
                    if(steps == 0) phase = END;
                }
                
                /*
                for(uint32_t blocked = 0; blocked <2;){
                    pos = getDesiredTile();
                    if(gameAction(BLOCK, pos.x, pos.y, &winner)) blocked++;
                    else; //display error message
                }*/
                break;

            case CLAIMONE:
                
                if(phase == READY) steps = 1;

                pos = getDesiredTile();
                if(pos.x >= 0 && pos.y >= 0){
                    if(gameAction(CLAIM, pos.x, pos.y, &winner)){
                        steps--;
                        phase = ACTIVE;
                    } 
                    if(steps == 0) phase = END;
                }

                break;

            case PLACEBLOCK:

                if(phase == READY) steps = 2;
                if(steps == 2){
                    pos = getDesiredTile();
                    if(pos.x >= 0 && pos.y >= 0){
                        if(gameAction(PLACE, pos.x, pos.y, &winner)){
                            steps--;
                            phase = ACTIVE;
                        }
                    }
                }
                else if(steps == 1){
                    pos = getDesiredTile();
                    if(pos.x >= 0 && pos.y >= 0){
                        if(gameAction(BLOCK, pos.x, pos.y, &winner)) steps--;
                    }
                }
                if(steps == 0) phase = END;

                /*
                for(uint32_t placed = 0; placed <1;){
                    pos = getDesiredTile();
                    if(gameAction(PLACE, pos.x, pos.y, &winner)) placed++;
                    else; //display error message
                }

                for(uint32_t blocked = 0; blocked <1;){
                    pos = getDesiredTile();
                    if(gameAction(BLOCK, pos.x, pos.y, &winner)) blocked++;
                    else; //display error message
                }
                */
                break;

            defualt:
                break;
        }

        if(phase == END){
            curPlayer = (curPlayer+1)%playerCount;
            WM->click.pressed = false;

            std::cout << std::endl << 
            "it is player " << curPlayer+1 <<
            "'s turn, choose action:" << std::endl << 
            "1: place 3 empty tiles" << std::endl << 
            "2: block 2 empty tiles" << std::endl <<
            "3: claim 1 empty tile" << std::endl <<
            "4: place 1 empty tile and block 1 empty tile" << std::endl;
            phase = READY;
            select = -1;

            prevmoves = curmoves;
            curmoves = MoveList();
        }

    }

    return winner;
}

int GameMaster::getPlayerChoice(){
    int choice;

    WM->readIns(true);
    choice = WM->getAction();

    if(checkChoice(choice)){
        WM->readIns(false);
    }
    else{
        choice = -1;
    }
    
    
    return choice;

}

bool GameMaster::checkChoice(int choice){

    bool ret = false;

    //printf("checking choice %d\n",choice);

    switch(choice){
        case PLACETHREE:
            ret = true;
            break;

        case BLOCKTWO:
            if(freeTiles >= 2)ret = true;
            else printf("not enough empty tiles (%d)\n", freeTiles);
            break;
                
        case CLAIMONE:
            if(freeTiles >= 1)ret = true;
            else printf("not enough empty tiles (%d)\n", freeTiles);
            break;

        case PLACEBLOCK:
            ret = true;
            break;
            
        default:
            break;
            
    }
    return ret;
}

coord GameMaster::getDesiredTile(){
    coord crds;
    position pos;
    //fprintf(stderr, "ready to get tile location\n");
    

    if(WM->click.pressed){

        pos.x = WM->click.x;//-10*RATIO;
        pos.y = WM->click.y;//+11;

        pos.x = 2*pos.x/WIDTH;
        pos.y = 2*pos.y/HEIGHT;

        pos.x -= 1.00;
        pos.y -= 1.00; 

        pos.x *= 1;
        pos.y *= -1;

        pos.x *= INITDIM*2*RATIO;
        pos.y *= INITDIM*2;

        crds.x = pos.x+INITDIM*2;
        crds.y = pos.y+INITDIM;

        /*
        crds.x = pos.x*BIAS*(INITDIM*2)*RATIO;
        crds.y = pos.y*INITDIM*2;

        crds.x += INITDIM*2;
        crds.y += INITDIM;
        */

        fprintf(stderr,"the selected tile is at (ints) x: %d, y: %d\n", crds.x, crds.y);
        WM->click.pressed = false;
    }
    else{
        crds.x = -1;
        crds.y = -1;
    }
    
    return crds;
}



