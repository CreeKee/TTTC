/* Ask for an OpenGL Core Context */

#include "includes.h"
#include "WindowManager.hpp"
#include "FieldManager.hpp"
#include "GameMaster.hpp"
#include "bot_gen0.hpp"

#define AGENTID 1

struct AgentData{
    BotG0* agent0;
    BotG0* agent1;

    uint32_t wins = 0;
    uint32_t gamesPlayed = 0;
    uint32_t movesTaken = 0;

    char filename[22] = DEFAULTFILE;
};

AgentData runGeneration();
void runGame(AgentData** players);
void humanVsAgent();
void displayMove(MoveList moves){

    //display info about the move
    fprintf(stderr,"\nmove count: ");
    fprintf(stderr,"%d\n",moves.moveDex);

    //display the move itself
    for(int i = 0; i<moves.moveDex; i++){
        fprintf(stderr,"%d@(%d, %d) | ",moves.moves[i].action, moves.moves[i].x, moves.moves[i].y);
    }

    fprintf(stderr,"\n");
}

int main(int argc, char** argv){
    AgentData bestAgent;
    uint8_t digitA = '0';
    uint8_t digitB = '0';
    uint8_t digitC = '0';
    char filename[] = DEFAULTFILE;

    humanVsAgent();


    /*
    position p;
    int a = 0;
    int b = 0;
    int winner = -1;
    bool winmsg = false;
    MoveList moves;
    uint32_t movenum = 0;

    BotG0* players[2] = {new BotG0(1, "weights1.txt"), new BotG0(1, "weights1.txt")};
    bool curplayer = 1;

    //players[0]->dive();

    GameMaster gamemast(2);
    gamemast.startGame();

    players[0]->displayWeights();

    fprintf(stderr, "game has been started\n");
    
    gamemast.displayBoard();

    moves = players[0]->getNextAction();
    displayMove(moves);
    
    fprintf(stderr, "\n\nsubmitting first move\n");
    
    //submit move
    while((winner = gamemast.takeTurn(moves)) == -1){

        //display board and announce move number
        gamemast.displayBoard();
        fprintf(stderr, "move number: %d\n\n",++movenum);
        

        //get next move
        fprintf(stderr, "getting next action\n");
        moves = players[curplayer]->getNextAction(moves);
        
        //alternate player
        curplayer = !curplayer;
    }


    gamemast.displayBoard();
    fprintf(stderr, "winner is %d\n",winner);
*/
    /*
    WindowManager winman;
    FieldManager fieldman((((float)WIDTH)/HEIGHT));
    GameMaster gamemast(2);
    gamemast.setFM(&fieldman);
    gamemast.setWM(&winman);

    fieldman.setField(gamemast.getField());
    //GLint mvp_location, vpos_location, vcol_location;

    winman.createWindow(WINDOW_NAME, WIDTH, HEIGHT);
    winman.initBuff(1);
    winman.initShaders(vertex_shader_text, fragment_shader_text);
    winman.initAttributes();

    gamemast.startGame();

    while (!winman.shouldClose()){

        winman.bufferData(fieldman.getVCount()*sizeof(vertex), (void*)fieldman.getVField());

        winman.uDisp(fieldman.getVCount());

        if(winner == -1) winner = gamemast.takeTurn();
        else if(!winmsg){
            std::cout << std::endl <<"***Player " << winner+1 <<" wins!***" << std::endl;
            winmsg = true;
        }
        

    }
 
    winman.end();

*/
    
    /*
    //get and display first move--------------------
    moves = bot.getNextAction();
    displayMove(moves);
    
    //get and display second move--------------------
    moves = bot.getNextAction(moves);
    displayMove(moves);

    //get and display third move--------------------
    moves = bot.getNextAction(moves);
    displayMove(moves);

    for(int i = 0; i<20; i++){
        fprintf(stderr, "doing action #%d\n", i);
        moves = bot.getNextAction(moves);
        displayMove(moves);
    }
    
    bot.updateWeights();
    bot.saveWeights("weights1.txt");
    */
    /*
    bestAgent = runGeneration();
    
    fprintf(stderr, "best file was: %s\n", bestAgent.filename);

    for(int agent = 0; agent < POPULATION; agent++){
        
        bestAgent.agent0->saveWeights(filename);
        bestAgent.agent0->updateWeights();

        digitA = (agent+1)%10+'0';
        digitB = ((agent+1)/10)%10+'0';
        digitC = ((agent+1)/100)%10+'0';

        filename[14] = digitC;
        filename[15] = digitB;
        filename[16] = digitA;

    }
    */
    
    fprintf(stderr, "ending main\n");
    return 0;
}


void humanVsAgent(){
    bool flop = true;
    MoveList moves;
    int winner = -1;
    WindowManager winman;
    FieldManager fieldman((((float)WIDTH)/HEIGHT));
    GameMaster gamemast(2);
    BotG0 agent(1, DEFAULTFILE);
    
    gamemast.setFM(&fieldman);
    gamemast.setWM(&winman);

    fieldman.setField(gamemast.getField());
    //GLint mvp_location, vpos_location, vcol_location;

    winman.createWindow(WINDOW_NAME, WIDTH, HEIGHT);
    winman.initBuff(1);
    winman.initShaders(vertex_shader_text, fragment_shader_text);
    winman.initAttributes();
    fprintf(stderr, "starting game\n");
    gamemast.startGame();
    
    while (!winman.shouldClose() && winner == -1){

        winman.bufferData(fieldman.getVCount()*sizeof(vertex), (void*)fieldman.getVField());

        winman.uDisp(fieldman.getVCount());

        //human takes turn
        if(gamemast.getCurPlayer() != AGENTID){ 
            if(winner == -1) winner = gamemast.takeTurn();
            if(flop){
                gamemast.displayBoard();
                fprintf(stderr, "taking human's turn\n");
                flop = false;
            }
            }
        
        //agent takes turn
        else if(winner == -1){
            flop = true;
            gamemast.getPrevMoves().display();
            gamemast.displayBoard();
            fprintf(stderr, "\ntaking agent's turn\n");
            agent.showCurrentBoard();
            moves = agent.getNextAction(gamemast.getPrevMoves());
            agent.showCurrentBoard();
            moves.display();
            winner = gamemast.takeTurn(moves);
        }

    }
    if(winner != -1) std::cout << std::endl <<"***Player " << winner+1 <<" wins!***" << std::endl;
    while (!winman.shouldClose()){
         winman.bufferData(fieldman.getVCount()*sizeof(vertex), (void*)fieldman.getVField());

        winman.uDisp(fieldman.getVCount());
    }

    winman.end();
}


AgentData runGeneration(){
    uint8_t digitA = '0';
    uint8_t digitB = '0';
    uint8_t digitC = '0';
    uint32_t best = 0;

    char filename[] = DEFAULTFILE;
    AgentData agents[POPULATION];
    AgentData* players[2];
    
    for(int agent = 0; agent < POPULATION; agent++){
        
        agents[agent].agent0 = new BotG0(0,filename);
        agents[agent].agent1 = new BotG0(1,filename);

        digitA = (agent+1)%10+'0';
        digitB = ((agent+1)/10)%10+'0';
        digitC = ((agent+1)/100)%10+'0';

        filename[14] = digitC;
        filename[15] = digitB;
        filename[16] = digitA;

        agents[agent].filename[14] = digitC;
        agents[agent].filename[15] = digitB;
        agents[agent].filename[16] = digitA;


    }

    for(int agentA = 0; agentA<POPULATION; agentA++){
        for(int agentB = agentA+1; agentB<POPULATION; agentB++){
            players[0] = &agents[agentA];
            players[1] = &agents[agentB];
            runGame(players);

            players[0] = &agents[agentB];
            players[1] = &agents[agentA];
            runGame(players);
        }
    }
    
    for(int cur = 1; cur<POPULATION; cur++){
        if(agents[cur].wins > agents[best].wins){
            best = cur;
        }
        else if(agents[cur].wins == agents[best].wins && agents[cur].movesTaken < agents[best].movesTaken){
            best = cur;
        }
    }

    return agents[best];
}

void runGame(AgentData** players){

    bool curplayer = 1;
    int winner = -1;
    MoveList moves;
    GameMaster gamemast(2);
    uint32_t movenum = 0;

    gamemast.startGame();

    //fprintf(stderr, "game has been started\n");
    
    gamemast.displayBoard();

    moves = players[0]->agent0->getNextAction();
    players[0]->movesTaken++;
    //displayMove(moves);
    
    //fprintf(stderr, "\n\nsubmitting first move\n");
    
    //submit move
    while(((winner = gamemast.takeTurn(moves)) == -1) && movenum<MOVELIMIT){

        //display board and announce move number
        gamemast.displayBoard();
        fprintf(stderr, "player %d - move number: %d\n\n",curplayer, movenum+1);
        

        //get next move
        //fprintf(stderr, "getting next action\n");

        if(curplayer){
            moves = players[curplayer]->agent1->getNextAction(moves);
        }
        else{
            moves = players[curplayer]->agent0->getNextAction(moves);
        }
        players[curplayer]->movesTaken++;
        
        
        //alternate player
        curplayer = !curplayer;

        movenum++;
    }


    players[0]->gamesPlayed++;
    players[1]->gamesPlayed++;

    players[0]->agent0->reset();
    players[1]->agent1->reset();

    if(winner != -1){
        players[winner]->wins++;
        fprintf(stderr, "winner was %d\n",winner);
        gamemast.displayBoard();
    }
    else{
        fprintf(stderr, "game ended in draw\n");
    }
    //gamemast.displayBoard();
    //fprintf(stderr, "winner is %d\n",winner);
}
