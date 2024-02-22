#include "Brain.hpp"

Brain::Brain(char* filename){
    readWeights(filename);
}

Brain::Brain(){
    float* weight = (float*)(&weights);
    for(int curW = 0; curW < PRECEPTCOUNT; curW++){
        weight[curW] = 0;
        
    }
}

void Brain::storeWeights(char* filename){
    std::ofstream file;
    file.open(filename);
    file.write((char*)(&weights), sizeof(float)*PRECEPTCOUNT);
}

void Brain::readWeights(char* filename){
    std::ifstream file;
    file.open(filename);
    file.read((char*)(&weights), sizeof(float)*PRECEPTCOUNT);
    file.close();
}

void Brain::scrambleWeights(){

    float* weight = (float*)(&weights);
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(0,200); 

    float rand;

    for(int curW = 0; curW < PRECEPTCOUNT; curW++){
        rand = (dist6(rng)/GRANULARITY) - .1;
        weight[curW] += rand;
        
    }
}

int32_t Brain::evaluate(boardInstance boardInst, int32_t* hardwin){
    
    int32_t val = 0;
    precepts vals = gatherPrecepts(boardInst);

    float* curWeight = (float*)(&weights);
    uint32_t* curPrecept = (uint32_t*)(&vals);

    for(int mv = 0; mv < PRECEPTCOUNT; mv++){
        val += (curWeight[mv])*(curPrecept[mv]);
    }
    if(vals.win > 0){
        *hardwin = 1;
    }
    if(vals.lose > 0){
        *hardwin = -1;
    }

    return val;
}

precepts Brain::gatherPrecepts(boardInstance boardInst){
    
    precepts ps = {0};
    
    bool** checked;

    checked = (bool**)calloc(boardInst.x_lim, sizeof(bool*));
    for(int set = 0; set < boardInst.x_lim; set++){
        checked[set] = (bool*)calloc(boardInst.y_lim, sizeof(bool));
    }

    for(int x = 0; x < boardInst.x_lim; x++){
        for(int y = 0; y < boardInst.y_lim; y++){
            if(checked[x][y] == false){
                if(boardInst.board[x][y].type == ADJTILE){
                    gatherFromAdj(&ps,checked, boardInst, x, y, 0, 0, 0, ADJTILE);
                }
                else if(boardInst.board[x][y].type == EMPTYTILE){                    
                    gatherFromEmpty(&ps,checked, boardInst, x, y, 0, 0, 0, EMPTYTILE);
                }
            }
        }
    }

    
    for(int set = 0; set < boardInst.x_lim; set++){
        free(checked[set]);
    }
    free(checked);

    return ps;

}

void Brain::gatherFromEmpty(precepts* traits, bool** checkfield, boardInstance boardInst, int x, int y, uint32_t len, int dx, int dy, uint32_t prevType){

    
    if(!checkfield[x+dx][y+dy]){
    switch(boardInst.board[x+dx][y+dy].type){
        case EMPTYTILE:
            checkfield[x+dx][y+dy] = true;
            traits->openness+=len+1;
            gatherFromEmpty(traits, checkfield, boardInst, x+dx, y+dy, len+1, -1, -1, EMPTYTILE);
            gatherFromEmpty(traits, checkfield, boardInst, x+dx, y+dy, len+1,  1, -1, EMPTYTILE);
            gatherFromEmpty(traits, checkfield, boardInst, x+dx, y+dy, len+1, -1,  1, EMPTYTILE);
            gatherFromEmpty(traits, checkfield, boardInst, x+dx, y+dy, len+1,  1,  1, EMPTYTILE);
            gatherFromEmpty(traits, checkfield, boardInst, x+dx, y+dy, len+1,  0, -1, EMPTYTILE);
            gatherFromEmpty(traits, checkfield, boardInst, x+dx, y+dy, len+1,  0,  1, EMPTYTILE);
            gatherFromEmpty(traits, checkfield, boardInst, x+dx, y+dy, len+1,  1,  0, EMPTYTILE);
            gatherFromEmpty(traits, checkfield, boardInst, x+dx, y+dy, len+1, -1,  0, EMPTYTILE);
        break;

        case CLAIMEDTILE:
            if(boardInst.board[x+dx][y+dy].owner == playerID && (prevType == SELFCLAIM || prevType == EMPTYTILE)){
                checkfield[x+dx][y+dy] = true;
                if(prevType == EMPTYTILE) len = 0;
                if(len == 0){
                    if(dx != 0 && dy != 0){
                        traits->selfclaimdiagempty++;
                    }
                    else{
                        traits->selfclaimadjempty++;
                    }
                }
                else if(len == 1){
                    if(dx != 0 && dy != 0){
                        traits->selfclaimpairdiagempty++;
                    }
                    else{
                        traits->selfclaimpairadjempty++;
                    }
                }
                else{
                    traits->win++;
                }
                gatherFromEmpty(traits, checkfield, boardInst, x+dx, y+dy, len+1, dx, dy, SELFCLAIM);
            }
            else if(boardInst.board[x+dx][y+dy].owner != playerID && (prevType == OTHERCLAIM || prevType == EMPTYTILE)){
                checkfield[x+dx][y+dy] = true;
                if(prevType == EMPTYTILE) len = 0;
                if(len == 0){
                    if(dx != 0 && dy != 0){
                        traits->otherclaimdiagempty++;
                    }
                    else{
                        traits->otherclaimadjempty++;
                    }
                }
                else if(len == 1){
                    if(dx != 0 && dy != 0){
                        traits->otherclaimpairdiagempty++;
                    }
                    else{
                        traits->otherclaimpairadjempty++;
                    }
                }
                else{
                    traits->lose++;
                }
                gatherFromEmpty(traits, checkfield, boardInst, x+dx, y+dy, len+1, dx, dy, OTHERCLAIM);
            }
       
        break;
        default:
        break;
    }
    }

}

void Brain::gatherFromAdj(precepts* traits, bool** checkfield, boardInstance boardInst, int x, int y, uint32_t len, int dx, int dy, uint32_t prevType){

    

    if(!checkfield[x+dx][y+dy]){
    switch(boardInst.board[x+dx][y+dy].type){
        case ADJTILE:
            checkfield[x+dx][y+dy] = true;
            traits->surface+=1;
            gatherFromAdj(traits, checkfield, boardInst, x+dx, y+dy, len, -1, -1, ADJTILE);
            gatherFromAdj(traits, checkfield, boardInst, x+dx, y+dy, len,  1, -1, ADJTILE);
            gatherFromAdj(traits, checkfield, boardInst, x+dx, y+dy, len, -1,  1, ADJTILE);
            gatherFromAdj(traits, checkfield, boardInst, x+dx, y+dy, len,  1,  1, ADJTILE);
            gatherFromAdj(traits, checkfield, boardInst, x+dx, y+dy, len,  0, -1, ADJTILE);
            gatherFromAdj(traits, checkfield, boardInst, x+dx, y+dy, len,  0,  1, ADJTILE);
            gatherFromAdj(traits, checkfield, boardInst, x+dx, y+dy, len,  1,  0, ADJTILE);
            gatherFromAdj(traits, checkfield, boardInst, x+dx, y+dy, len, -1,  0, ADJTILE);
        break;

        case CLAIMEDTILE:
            if(boardInst.board[x+dx][y+dy].owner == playerID && (prevType == SELFCLAIM || prevType == ADJTILE)){
                checkfield[x+dx][y+dy] = true;
                if(prevType == ADJTILE) len = 0;
                if(len == 0){
                    if(dx != 0 && dy != 0){
                        traits->selfclaimdiagadj++;
                    }
                    else{
                        traits->selfclaimadjadj++;
                    }
                }
                else if(len == 1){
                    if(dx != 0 && dy != 0){
                        traits->selfclaimpairdiagadj++;
                    }
                    else{
                        traits->selfclaimpairadjadj++;
                    }
                }
                else{
                    traits->win++;
                }
                gatherFromEmpty(traits, checkfield, boardInst, x+dx, y+dy, len+1, dx, dy, SELFCLAIM);
            }
            else if(boardInst.board[x+dx][y+dy].owner != playerID && (prevType == OTHERCLAIM || prevType == EMPTYTILE)){
                checkfield[x+dx][y+dy] = true;
                if(prevType == ADJTILE) len = 0;
                if(len == 0){
                    if(dx != 0 && dy != 0){
                        traits->otherclaimdiagadj++;
                    }
                    else{
                        traits->otherclaimadjadj++;
                    }
                }
                else if(len == 1){
                    if(dx != 0 && dy != 0){
                        traits->otherclaimpairdiagadj++;
                    }
                    else{
                        traits->otherclaimpairadjadj++;
                    }
                }
                else{
                    traits->lose++;
                }
                gatherFromEmpty(traits, checkfield, boardInst, x+dx, y+dy, len+1, dx, dy, OTHERCLAIM);
            }
       
        break;
        default:
        break;
    }
    }
}
