#include "FieldManager.hpp"


FieldManager::FieldManager(float ratio){

    vertex tempv;

    this->ratio = ratio;


    vfieldSize = 100000;
    vcount = 0;
    vertField = (vertex*)calloc(sizeof(vertex), vfieldSize);   

    tileOffset = SCALAR/2; 
    lineWidth = LINEWIDTH;

    /*******************************/
    /*
    for(float i = 0; i<2*M_PI; i+=0.01){
        Line l1(i, lineWidth*8, ratio, tileOffset*16);
        for(int verts = 0; verts<6; verts++){
            vertField[vcount].pos.x = l1.get(verts).pos.x;
            vertField[vcount].pos.y = l1.get(verts).pos.y;
            vertField[vcount++].col = l1.get(verts).col;
            
        }
    }*/

}

//TODO: vertex buffer overrun
//TODO: magicNum
bool FieldManager::placeTile(int x, int y){

    float scaledX = (x*SCALAR-2)/ratio;
    float scaledY = (y*SCALAR-1);

    fprintf(stderr, "scaledX: %f, scaledY: %f from %d %d\n", scaledX, scaledY, x, y);

    if(!(x>0 && field[y][x-1].type != NULL)){
        //generate left side vertices

        generateRectangle(scaledX, scaledY, M_PI/2);
    }
    if(!(y>0 && field[y-1][x].type != NULL)){
        //generate bottom side vertices
        generateRectangle(scaledX, scaledY, 0);

    }
    if(!(x<fieldSize && field[y][x+1].type != NULL)){
        //generate right side vertices
        generateRectangle(3*lineWidth+scaledX+tileOffset, scaledY, M_PI/2);

    }
    if(!(y<fieldSize && field[y+1][x].type != NULL)){
        //generate top side vertices
        generateRectangle(scaledX,  scaledY+(3*lineWidth+tileOffset)*ratio, 0);

    }
    fprintf(stderr, "done making reectangle\n");
}

bool FieldManager::blockTile(int x, int y){
    float scaledX = (x*SCALAR-2)/ratio;
    float scaledY = (y*SCALAR-1);
    float length = tileOffset + 3*lineWidth;

    bool ret = true;

    fprintf(stderr, "scaledX: %f, scaledY: %f from %d %d\n", scaledX, scaledY, x, y);

    vertField[vcount].pos.x = scaledX;
    vertField[vcount].pos.y = scaledY;
    vertField[vcount++].col = COLORWHITE;

    vertField[vcount].pos.x = scaledX+length;
    vertField[vcount].pos.y = scaledY;
    vertField[vcount++].col = COLORWHITE;

    vertField[vcount].pos.x = scaledX;
    vertField[vcount].pos.y = (scaledY+length*ratio);
    vertField[vcount++].col = COLORWHITE;



    vertField[vcount].pos.x = scaledX+length;
    vertField[vcount].pos.y = (scaledY+length*ratio);
    vertField[vcount++].col = COLORWHITE;

    vertField[vcount].pos.x = scaledX;
    vertField[vcount].pos.y = (scaledY+length*ratio);
    vertField[vcount++].col = COLORWHITE;

    vertField[vcount].pos.x = scaledX+length;
    vertField[vcount].pos.y = scaledY;
    vertField[vcount++].col = COLORWHITE;

    return ret;

}

void FieldManager::claimTile(int x, int y, int id){

    if(id){
        placeTri(x, y);
    }
    else{
        placeX(x, y);
    }
   
}

void FieldManager::placeX(int x, int y){

    float scaledX = (x*SCALAR-2)/ratio;
    float scaledY = (y*SCALAR-1);

    Line l1(M_PI/4, lineWidth*1.2, ratio, 2*tileOffset/3);
    Line l2(3*M_PI/4, lineWidth*1.2, ratio, 2*tileOffset/3);

    l1.add(scaledX+tileOffset/3, scaledY+tileOffset/3);
    l2.add(scaledX+tileOffset, scaledY+tileOffset/3);

    for(int verts = 0; verts<6; verts++){
        vertField[vcount].pos.x = l1.get(verts).pos.x;
        vertField[vcount].pos.y = l1.get(verts).pos.y;
        vertField[vcount++].col = l1.get(verts).col;
    }

    for(int verts = 0; verts<6; verts++){
        vertField[vcount].pos.x = l2.get(verts).pos.x;
        vertField[vcount].pos.y = l2.get(verts).pos.y;
        vertField[vcount++].col = l2.get(verts).col;
    }

}


void FieldManager::placeTri(int x, int y){

    float scaledX = (x*SCALAR-2)/ratio;
    float scaledY = (y*SCALAR-1);

    Line l1(M_PI/3, lineWidth, ratio, tileOffset/2);
    Line l2(M_PI, lineWidth, ratio, tileOffset/2);
    Line l3(0, lineWidth, ratio, tileOffset/2);

    l1.add(scaledX+tileOffset/2, scaledY+tileOffset/2);
    l2.add(scaledX+tileOffset/2, scaledY+tileOffset/2);
    l3.add(scaledX+tileOffset/2, scaledY+tileOffset/2);

    for(int verts = 0; verts<6; verts++){
        vertField[vcount].pos.x = l1.get(verts).pos.x;
        vertField[vcount].pos.y = l1.get(verts).pos.y;
        vertField[vcount++].col = l1.get(verts).col;
    }

    for(int verts = 0; verts<6; verts++){
        vertField[vcount].pos.x = l2.get(verts).pos.x;
        vertField[vcount].pos.y = l2.get(verts).pos.y;
        vertField[vcount++].col = l2.get(verts).col;
    }

    for(int verts = 0; verts<6; verts++){
        vertField[vcount].pos.x = l3.get(verts).pos.x;
        vertField[vcount].pos.y = l3.get(verts).pos.y;
        vertField[vcount++].col = l3.get(verts).col;
    }
}



void FieldManager::generateRectangle(float x, float y, double rotation){

    Line l1(rotation, lineWidth, ratio, tileOffset);
    l1.add(x,y);

    for(int verts = 0; verts<6; verts++){
        vertField[vcount].pos.x = l1.get(verts).pos.x;
        vertField[vcount].pos.y = l1.get(verts).pos.y;
        vertField[vcount++].col = l1.get(verts).col;
    }
/*
    switch(rotation){
        case HORIZ:
            vertField[vcount].pos.x = (x+lineWidth)*ratio;
            vertField[vcount].pos.y = y+tileOffset+lineWidth;
            vertField[vcount++].col = COLORWHITE;

            vertField[vcount].pos.x = (x-lineWidth)*ratio;
            vertField[vcount].pos.y = y+tileOffset+lineWidth;
            vertField[vcount++].col = COLORWHITE;

            vertField[vcount].pos.x = (x+lineWidth)*ratio;
            vertField[vcount].pos.y = y-tileOffset-lineWidth;
            vertField[vcount++].col = COLORWHITE;

            vertField[vcount].pos.x = (x-lineWidth)*ratio;
            vertField[vcount].pos.y = y+tileOffset+lineWidth;
            vertField[vcount++].col = COLORWHITE;

            vertField[vcount].pos.x = (x+lineWidth)*ratio;
            vertField[vcount].pos.y = y-tileOffset-lineWidth;
            vertField[vcount++].col = COLORWHITE;

            vertField[vcount].pos.x = (x-lineWidth)*ratio;
            vertField[vcount].pos.y = y-tileOffset-lineWidth;
            vertField[vcount++].col = COLORWHITE;
        break;

        case SLANT:
        break;

        case VERT:
            vertField[vcount].pos.x = (x+tileOffset+lineWidth)*ratio;
            vertField[vcount].pos.y = y+lineWidth;
            vertField[vcount++].col = COLORWHITE;

            vertField[vcount].pos.x = (x+tileOffset+lineWidth)*ratio;
            vertField[vcount].pos.y = y-lineWidth;
            vertField[vcount++].col = COLORWHITE;

            vertField[vcount].pos.x = (x-tileOffset-lineWidth)*ratio;
            vertField[vcount].pos.y = y+lineWidth;
            vertField[vcount++].col = COLORWHITE;

            vertField[vcount].pos.x = (x+tileOffset+lineWidth)*ratio;
            vertField[vcount].pos.y = y-lineWidth;
            vertField[vcount++].col = COLORWHITE;

            vertField[vcount].pos.x = (x-tileOffset-lineWidth)*ratio;
            vertField[vcount].pos.y = y+lineWidth;
            vertField[vcount++].col = COLORWHITE;

            vertField[vcount].pos.x = (x-tileOffset-lineWidth)*ratio;
            vertField[vcount].pos.y = y-lineWidth;
            vertField[vcount++].col = COLORWHITE;
        break;

        default:
        break;
    }
*/
}
/*
shape FieldManager::getTile(float x, float y){

    printf("\n %f, %f\n", x, y);
    x = fieldSize*x/WIDTH + tileOffset*fieldSize/2;
    y = fieldSize-fieldSize*y/WIDTH - 9 + tileOffset*fieldSize/2;

    printf("Mouse was clicked at %d, %d. This was a ", (int)x, (int)y);


    return (x>=0 && x<fieldSize && y>=0 && y<fieldSize) ? field[(int)y][(int)x] : shape();
}
*/
//TODO: magic nums
position FieldManager::pix2field(float x, float y){

    position pos;
    pos.x = x/WIDTH;
    pos.y = y/HEIGHT;
    pos.z = 0;
    printf("x: %f y: %f\n", pos.x, pos.y);

    pos.x -= 0.5;
    pos.y -= 0.5;
    printf("x: %f y: %f\n", pos.x, pos.y);

    pos.x *= fieldSize;
    pos.y *= fieldSize;
    printf("x: %f y: %f\n", pos.x, pos.y);

    return pos;
}

position FieldManager::field2pix(int x, int y){
    position pos;
    pos.x = (float)(x+xshift)/WIDTH;
    pos.y = (float)(y+yshift)/HEIGHT;
    pos.z = 0;

    return pos;
}

/***********************/

Line::Line(double angle, double linewidth, double ratio, double lgth){
    int vcnt = 0;
    this->angle = angle;
    this->length = lgth+3*linewidth;

    
    verts[vcnt].pos.x = 0;
    verts[vcnt].pos.y = std::cos(angle)*linewidth*ratio;
    verts[vcnt++].col = COLORWHITE;

    verts[vcnt].pos.x = std::sin(angle)*linewidth;
    verts[vcnt].pos.y = 0;
    verts[vcnt++].col = COLORWHITE;

    verts[vcnt].pos.x = std::cos(angle)*length;
    verts[vcnt].pos.y = std::sin(angle)*length*ratio;
    verts[vcnt++].col = COLORWHITE;



    verts[vcnt].pos.x = std::sin(angle)*linewidth;
    verts[vcnt].pos.y = std::cos(angle)*linewidth*ratio;
    verts[vcnt++].col = COLORWHITE;

    verts[vcnt].pos.x = (std::cos(angle)*length + std::sin(angle)*linewidth);
    verts[vcnt].pos.y = std::sin(angle)*length*ratio;
    verts[vcnt++].col = COLORWHITE;

    verts[vcnt].pos.x = (std::cos(angle)*length);
    verts[vcnt].pos.y = (std::sin(angle)*length + std::cos(angle)*linewidth)*ratio;
    verts[vcnt++].col = COLORWHITE;


}

void Line::add(float x, float y){
    for(int i = 0; i<6; i++){
        verts[i].pos.x += x;
        verts[i].pos.y += y;
    }
}