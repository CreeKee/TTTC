#include "GameMaster.hpp"



GameMaster::GameMaster(int numPlayers){
    playerCount = numPlayers;
    freeTiles = 0;
    
    FM = nullptr;

    curPlayer = 0;

    movedex = 0;

}

bool GameMaster::claimTile(coord crds){

    bool ret = false;

    if((ret = boardInst.makeMove(CLAIMEDTILE, crds, curPlayer) == true)){
        if(FM != nullptr) FM->claimTile(crds.x, crds.y, curPlayer);
        freeTiles--;
    }

    return ret;

}

bool GameMaster::blockTile(coord crds){

    bool ret = false;

    if((ret = boardInst.makeMove(BLOCKEDTILE, crds, curPlayer) == true)){
        if(FM != nullptr) FM->blockTile(crds.x, crds.y);
        freeTiles--;
    }

    return ret;

}

bool GameMaster::placeEmptyTile(coord crds){

    bool ret = false;
    //fprintf(stderr, "tile %d %d is of type %d\n",x,y,boardInst.board[x][y].type);

    if((ret = boardInst.makeMove(EMPTYTILE, crds, curPlayer) == true)){
        if(FM != nullptr) FM->placeTile(crds.x, crds.y);
        freeTiles++;
    }
    
    return ret;
}



bool GameMaster::gameAction(int action, coord crds, int* winner){

    bool ret = false;

    //UPDATEDIMS;  

    switch(action){
        case CLAIM:
            //fprintf(stderr, "handling claim action\n");
            if(ret = claimTile(crds)){
                
                if(boardInst.checkWinCon(crds, curPlayer)){
                    *winner = curPlayer; 
                }
            }
            break;

        case PLACE:
            ret = placeEmptyTile(crds);
            break;

        case BLOCK:
            ret = blockTile(crds);
            break;

        default:
            break;
    }

    if(ret){
        curmoves.addMove(action, crds.x, crds.y);
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

                
                if(pos.x != 0 || pos.y != 0){
                    if(gameAction(PLACE, pos, &winner)){
                        steps--;
                        phase = ACTIVE;
                    } 
                    else fprintf(stderr, "Illegal place tile action taken\n");
                }
                if(steps == 0) phase = END;
                
                break;

            case BLOCKTWO:

                if(phase == READY) steps = 2;
                
                pos = getDesiredTile();

                if(pos.x != 0 || pos.y != 0){
                    if(gameAction(BLOCK, pos, &winner)){
                        steps--;
                        phase = ACTIVE;
                    }
                    else fprintf(stderr, "Illegal block tile action taken\n");
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
                if(pos.x != 0 || pos.y != 0){
                    if(gameAction(CLAIM, pos, &winner)){
                        steps--;
                        phase = ACTIVE;
                    } 
                    else fprintf(stderr, "Illegal claim tile action taken\n");
                    if(steps == 0) phase = END;
                }

                break;

            case PLACEBLOCK:

                if(phase == READY) steps = 2;
                if(steps == 2){
                    pos = getDesiredTile();
                    if(pos.x != 0 || pos.y != 0){
                        if(gameAction(PLACE, pos, &winner)){
                            steps--;
                            phase = ACTIVE;
                        }
                    }
                }
                else if(steps == 1){
                    pos = getDesiredTile();
                    if(pos.x != 0 || pos.y != 0){
                        if(gameAction(BLOCK, pos, &winner)) steps--;
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

int GameMaster::takeTurn(MoveList moves){

    int winner = -1;

    coord crds;
    

    for(int move = 0; move < moves.moveDex; move++){

        crds.x = moves.moves[move].x;
        crds.y = moves.moves[move].y;

        switch(moves.moves[move].action){
            case EMPTYTILE:
                fprintf(stderr, "placing at %d %d\n",crds.x, crds.y);
                if(!gameAction(PLACE, crds, &winner)) fprintf(stderr, "Illegal place tile action taken\n");
            break;
            
            case BLOCKEDTILE:
                if(!gameAction(BLOCK, crds, &winner)) fprintf(stderr, "Illegal block tile action taken\n");
            break;

            case CLAIMEDTILE:
                if(!gameAction(CLAIM, crds, &winner)) fprintf(stderr, "Illegal claim tile action taken\n");
            break;
        }
    }
    curPlayer = (curPlayer+1)%playerCount;
    prevmoves = curmoves;
    curmoves = MoveList();

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

        crds.x = pos.x;
        crds.y = pos.y;

        /*
        crds.x = pos.x*BIAS*(INITDIM*2)*RATIO;
        crds.y = pos.y*INITDIM*2;

        crds.x += INITDIM*2;
        crds.y += INITDIM;
        */

        //fprintf(stderr,"the selected tile is at (ints) x: %d, y: %d\n", crds.x, crds.y);
        WM->click.pressed = false;
    }
    else{
        crds.x = 0;
        crds.y = 0;
    }
    
    return crds;
}



