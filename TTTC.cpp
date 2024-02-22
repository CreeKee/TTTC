/* Ask for an OpenGL Core Context */


#include "includes.h"
#include "WindowManager.hpp"
#include "FieldManager.hpp"
#include "GameMaster.hpp"
#include "bot_gen0.hpp"


#define BUFFER_OFFSET(i) ((char *)NULL + (i))


void displayMove(MoveList moves){

    //display info about the move
    fprintf(stderr,"\nmove count: ");
    fprintf(stderr,"%d\n",moves.moveDex);

    //display the move itself
    for(int i = 0; i<moves.moveDex; i++){
        fprintf(stderr,"%d@(%d, %d) | ",moves.moves[i].action, moves.moves[i].x, moves.moves[i].y);
    }
}

int main(int argc, char** argv){
    position p;
    int a = 0;
    int b = 0;
    int winner = -1;
    bool winmsg = false;
    MoveList moves;

    BotG0* players[2] = {new BotG0(0, "weights1.txt"), new BotG0(1, "weights1.txt")};
    bool curplayer = 1;

    GameMaster gamemast(2);
    gamemast.startGame();

    fprintf(stderr, "game has been started\n");

    moves = players[0]->getNextAction();
    displayMove(moves);
    
    fprintf(stderr, "\n\nsubmitting first move\n");

    while(winner = gamemast.takeTurn(moves)){
        fprintf(stderr, "getting next action\n");
        moves = players[curplayer]->getNextAction(moves);
        displayMove(moves);
        curplayer = !curplayer;
    }
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
    fprintf(stderr, "finished creating bot\n");
    
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
    fprintf(stderr, "ending main\n");
    return 0;
}

