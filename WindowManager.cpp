#include "WindowManager.hpp"

bool readReady = false;
int32_t curAction = -1;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    else if(key == GLFW_KEY_SPACE && action == GLFW_PRESS) 
    {
        double xpos, ypos;
        //getting cursor position
        glfwGetCursorPos(window, &xpos, &ypos);
        //printf("Cursor Position at (%f, %f)\n", xpos, ypos);

        WindowManager* obj = (WindowManager*)glfwGetWindowUserPointer(window);
        obj->click.x = xpos;
        obj->click.y = ypos;
        obj->click.pressed = true;
    }
    else  if(readReady == true){
        if (key == GLFW_KEY_1 && action == GLFW_PRESS)     curAction = 0;
        else if(key == GLFW_KEY_2 && action == GLFW_PRESS) curAction = 1;
        else if(key == GLFW_KEY_3 && action == GLFW_PRESS) curAction = 2;
        else if(key == GLFW_KEY_4 && action == GLFW_PRESS) curAction = 3;
        else curAction = -1;
    }
    else curAction = -1;

}

WindowManager::WindowManager(){

      /* Initialize the library */
    if ( !glfwInit() ){
       exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    click.pressed = false;

}

void WindowManager::createWindow(const char* name, int x_dim, int y_dim){

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow( x_dim, y_dim, name, NULL, NULL );
    if (!window)
    {
        glfwTerminate();
        exit(-1);
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowUserPointer(window, this);
 
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);
 
}

void WindowManager::initBuff(int count){

    glGenVertexArrays(1, &vertexArray);
    glGenBuffers(1, &vertexBuffer);

    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    /*
    for(int buffs = 0; buffs < count; buffs++){
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[buffs]);
    }
    */
    
}

void WindowManager::bufferData(int buffSize, void* buffer){
    glBufferData(GL_ARRAY_BUFFER, buffSize, buffer, GL_STATIC_DRAW);
}

void WindowManager::initShaders(const char* v_shader, const char* f_shader){
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &v_shader, NULL);
    glCompileShader(vertex_shader);
 
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &f_shader, NULL);
    glCompileShader(fragment_shader);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
}

void WindowManager::initAttributes(){
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
}

void WindowManager::uDisp(int vcount){
    glDisable(GL_CULL_FACE);
    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // render the triangle
    glUseProgram(program);
    glBindVertexArray(vertexArray);
    glDrawArrays(GL_TRIANGLES, 0, vcount);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void WindowManager::end(){
    glfwDestroyWindow(window);
    glfwTerminate();

    if(vertexBuffer){
        //free(vertexBuffer);
    }
}


void WindowManager::readIns(bool toggle){
    readReady = toggle;
}

int32_t WindowManager::getAction(){
    return curAction;
}

void WindowManager::setAction(int32_t newAct){
    curAction = newAct;
}