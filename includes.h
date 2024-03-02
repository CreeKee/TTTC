

#define GLFW_INCLUDE_GLCOREARB

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>

#define DISPLAYBOARD(x_dist, y_dist, code)for(int x = 0; x<x_dist; x++){\
                for(int y = 0; y<y_dist; y++){\
                    code;\
                }\
                fprintf(stderr, "\n");\
            }

//used in getAllMoves() in boardInstance
#define LISTERATION(code)for(int x = 0; x < x_lim; x++){\
        for(int y = 0; y < y_lim; y++){\
            code\
        }\
    }


#define INITKIDS (1<<6)

#define MAXMOVES 3

#define PRUNING 1
#define DBG 0

#define BIAS 1.775

#define END 0
#define READY 1
#define ACTIVE 2

#define INITFIELDSIZE 100

#define NULLTILE 0
#define EMPTYTILE 1
#define CLAIMEDTILE 2
#define BLOCKEDTILE 3
#define ADJTILE 4
#define EBLOCK 5

#define CLAIM 0 
#define PLACE 1
#define BLOCK 2

#define PLACETHREE 0
#define BLOCKTWO 1
#define CLAIMONE 2
#define PLACEBLOCK 3
#define SKIPCHOICE 4
#define NULLACTION -1

#define GRANULARITY 1000.0
#define ATROPHYRATIO 1.0


//normal width = 1280
#define HEIGHT 720
#define WIDTH 1280
#define INITDIM 20
#define RATIO (WIDTH/HEIGHT);
#define BOARDINSTINIT 7

#define TILE   1
#define CROSS  2
#define CIRCLE 3
#define FILL   4
#define VERT 0
#define SLANT 1
#define HORIZ 2
#define SCALAR 1.0f/INITDIM
#define LINEWIDTH 0.001f

#define DEPTH 2

#define MOVELIMIT 15

#ifndef INCLUDES
#define INCLUDES

/*
struct boardData{
    uint32_t emptyTiles;
    uint32_t blockedTiles;
    bool wincon;
    bool losecon;
    
    uint32_t openPair[2];
    uint32_t
};
*/
struct move{
    uint32_t action;
    int32_t x;
    int32_t y;
};

struct tile{
    uint8_t type = NULLTILE;
    uint8_t owner = 0;
};

struct coord{
    int x;
    int y;
};

struct tnode{
    coord crds;
    tnode* next;
    tnode* prev;
};

struct bounds{
    uint32_t xmax;
    uint32_t xmin;
    uint32_t ymax;
    uint32_t ymin;
};

struct __attribute__ ((packed)) position{
    float x, y, z;
};

struct __attribute__ ((packed)) color{
    float r, g, b;
};

struct __attribute__ ((packed)) vertex{
    position pos;
    color col;
};

static const char* WINDOW_NAME = "TicTacTotalConquest";

static const char* vertex_shader_text =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos, 1.0);\n"
"    ourColor = aColor;\n"
"}\n";

static const char* fragment_shader_text =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(ourColor, 1.0f);\n"
"}\n";

#endif

/*


#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor;
}
*/



/*
#version 330 core
out vec4 FragColor;

in vec3 ourColor;

void main()
{
    FragColor = vec4(ourColor, 1.0f);
}
*/