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

    fprintf(stderr, "opening file: %s... ",filename);
    file.open(filename);
    file.read((char*)(&weights), sizeof(float)*PRECEPTCOUNT);
    file.close();

    fprintf(stderr, "file has been closed\n");

    //TODO temporary biasing
    
    float* weight = (float*)(&weights);
    for(int curW = 20; curW < 2; curW++){
        weight[curW] = -10;
    }
    for(int curW = 2; curW < 6; curW++){
        weight[curW] = 1000 * curW;
    }
    for(int curW = 6; curW < 10; curW++){
        weight[curW] = 0;
    }
    for(int curW = 10; curW < 14; curW++){
        weight[curW] = 500;
    }
    for(int curW = 14; curW < 18; curW++){
        weight[curW] = 0;
    }
    
}

void Brain::scrambleWeights(){

    float* weight = (float*)(&weights);
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(0,2*MAXTWEAK); 

    float rand;

    for(int curW = 0; curW < PRECEPTCOUNT; curW++){
        rand = (dist6(rng)/GRANULARITY) - MAXTWEAK;
        weight[curW] += rand;
    }
}

int32_t Brain::evaluate(boardInstance* boardInst, int32_t* hardwin){
    
    int32_t val = 0;
    precepts vals = gatherPrecepts(boardInst);
    bool wasWin = false;
    coord crds;

    float* curWeight = (float*)(&weights);
    uint32_t* curPrecept = (uint32_t*)(&vals);

    for(int mv = 0; mv < PRECEPTCOUNT; mv++){ 
        val += (curWeight[mv])*(curPrecept[mv]);
    }

    for(crds.x = 0; crds.x < boardInst->x_lim && !wasWin; crds.x++){
        for(crds. y = 0; crds.y < boardInst->y_lim && !wasWin; crds.y++){

            if(boardInst->board[crds.x][crds.y].type == CLAIMEDTILE){

                if(boardInst->board[crds.x][crds.y].owner == playerID) val+=100;
                if(boardInst->board[crds.x][crds.y].owner == playerID){
                    boardInst->checkClaim(crds, playerID, &wasWin);
                    if(wasWin) vals.win = 1;
                }
            }
        }
    }


    if(vals.win > 0){
        *hardwin = 1;
    }
    if(vals.lose > 0){
        *hardwin = -1;
    }
    return val;
}

int32_t Brain::evaluateLight(boardInstance* boardInst, int32_t* hardwin){
    
    int32_t val = 0;
    precepts vals = gatherPreceptsLight(boardInst);
    coord crds;
    bool wasWin = false;

    float* curWeight = (float*)(&weights);
    uint32_t* curPrecept = (uint32_t*)(&vals);

    for(int mv = 0; mv < PRECEPTCOUNT; mv++){ 
        val += (curWeight[mv])*(curPrecept[mv]);
    }

    for(crds.x = 0; crds.x < boardInst->x_lim && !wasWin; crds.x++){
        for(crds. y = 0; crds.y < boardInst->y_lim && !wasWin; crds.y++){

            if(boardInst->board[crds.x][crds.y].type == CLAIMEDTILE){

                
                if(boardInst->board[crds.x][crds.y].owner == playerID){
                    boardInst->checkClaim(crds, playerID, &wasWin);
                    if(wasWin) vals.win = 1;
                }
                else{
                    boardInst->checkClaim(crds, !playerID, &wasWin);
                    if(wasWin) vals.lose = 1;
                }
            }
        }
    }


    if(vals.win > 0){
        *hardwin = 1;
    }
    if(vals.lose > 0){
        *hardwin = -1;
    }

    return val;
}

precepts Brain::gatherPreceptsLight(boardInstance* boardInst){

    bool checkEnd = false;
    coord crds;
    precepts vals = {0};


    for(crds.x = 0; crds.x < boardInst->x_lim && !checkEnd; crds.x++){
        for(crds. y = 0; crds.y < boardInst->y_lim && !checkEnd; crds.y++){

            if(boardInst->board[crds.x][crds.y].type == CLAIMEDTILE){

                
                if(boardInst->board[crds.x][crds.y].owner == playerID){
                    vals.selfclaimadjadj += boardInst->checkClaim(crds, playerID, &checkEnd);
                }
                else{
                    vals.otherclaimadjadj += boardInst->checkClaim(crds, !playerID, &checkEnd);
                }
            }
        }
    }

    if(checkEnd){
        if(boardInst->board[crds.x][crds.y].owner == playerID) vals.win++;
        else vals.lose++;
    } 

    return vals;
}

precepts Brain::gatherPrecepts(boardInstance* boardInst){
    
    precepts ps = {0};
    
    bool** checked;

    checked = new bool*[boardInst->x_lim];
    for(int set = 0; set < boardInst->x_lim; set++){
        checked[set] = new bool[boardInst->y_lim];
    }

    for(int x = 0; x < boardInst->x_lim; x++){
        for(int y = 0; y < boardInst->y_lim; y++){
            if(checked[x][y] == false){
                if(boardInst->board[x][y].type == ADJTILE){
                    gatherFromAdj(&ps,checked, boardInst, x, y, 0, 0, 0, ADJTILE);
                }
                else if(boardInst->board[x][y].type == EMPTYTILE){                    
                    gatherFromEmpty(&ps,checked, boardInst, x, y, 0, 0, 0, EMPTYTILE);
                }
            }
        }
    }

    
    for(int set = 0; set < boardInst->x_lim; set++){
        delete checked[set];
    }
    delete checked;

    return ps;

}

void Brain::gatherFromEmpty(precepts* traits, bool** checkfield, boardInstance* boardInst, int x, int y, uint32_t len, int dx, int dy, uint32_t prevType){

    
    if(!checkfield[x+dx][y+dy]){
    switch(boardInst->board[x+dx][y+dy].type){
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
            if(boardInst->board[x+dx][y+dy].owner == playerID && (prevType == SELFCLAIM || prevType == EMPTYTILE)){
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
            else if(boardInst->board[x+dx][y+dy].owner != playerID && (prevType == OTHERCLAIM || prevType == EMPTYTILE)){
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

void Brain::gatherFromAdj(precepts* traits, bool** checkfield, boardInstance* boardInst, int x, int y, uint32_t len, int dx, int dy, uint32_t prevType){

    

    if(!checkfield[x+dx][y+dy]){
    switch(boardInst->board[x+dx][y+dy].type){
        case ADJTILE:
            checkfield[x+dx][y+dy] = true;
            traits->surface += 1;
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
            if(boardInst->board[x+dx][y+dy].owner == playerID && (prevType == SELFCLAIM || prevType == ADJTILE)){
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
            else if(boardInst->board[x+dx][y+dy].owner != playerID && (prevType == OTHERCLAIM || prevType == EMPTYTILE)){
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
