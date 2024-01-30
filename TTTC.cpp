/* Ask for an OpenGL Core Context */


#include "includes.h"
#include "WindowManager.hpp"
#include "FieldManager.hpp"
#include "GameMaster.hpp"
#include "bot_gen0.hpp"


#define BUFFER_OFFSET(i) ((char *)NULL + (i))


int getPlayerOption();

int main(int argc, char** argv){
    position p;
    int a = 0;
    int b = 0;
    int winner = -1;
    bool winmsg = false;

    BotG0 bot;
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
    
    fprintf(stderr, "ending main\n");
    return 0;
}

int getPlayerOption(){
    //wait until key input
    return 0;
}

