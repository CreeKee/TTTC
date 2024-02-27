#include "includes.h"
#include <GLFW/glfw3.h>

#include "glad/include/glad/glad.h"

#ifndef WINDOW_MANAGER
#define WINDOW_MANAGER

struct mouseClick{
    bool pressed;
    float x;
    float y;
};

class WindowManager{
    private:
    GLFWwindow* window;
    GLuint vertexBuffer, vertexArray;
    GLuint vertex_shader, fragment_shader, program;

    public:

    mouseClick click; 

    WindowManager();
    void createWindow(const char* name, int x_dim, int y_dim);
    bool shouldClose(){return glfwWindowShouldClose(window);}
    void initBuff(int count);
    void bufferData(int buffSize, void* buffer);
    void initShaders(const char* v_shader, const char* f_shader);
    void initAttributes();
    void uDisp(int vcount);
    void end();
    GLFWwindow* getWindow(){return window;}
    

    void readIns(bool toggle);
    int32_t getAction();
    void setAction(int32_t newAct);

};

#endif
