#include <iostream>
#include <vector>
// NOTE: temporary
#include <ctime>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "FieldView.h"
#include "TextRenderer.h"
#include "TextObject.h"
#include "TextBox.h"
#include "Shader.h"
#include "constants.h"

// Использование вектора типа double здесь неоправданно. Лишняя память.
const char* WINDOW_TITLE = "TicTacToe Tournament";
int flag = 0;

// NOTE: temporary func to set values in field.
void keyCallback( GLFWwindow* window, int key, int scancode, int action, int mode )
{
    flag = 1;
}

void OpenGlInit()
{
    GLFWwindow* window;

    if (!glfwInit())
        return;

    window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL );
    if (!window) {
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent( window );
    glfwSetKeyCallback( window, keyCallback );

    if (glewInit() != GLEW_OK)
        return;

    FieldView field;
    field.init();

    Box a( 100, 100, 200, 200, 0, 1, 0);
    Box a1(  0, 0, 100, 50, 1, 0, 0 );
    TextObject b( "FFF", { 150, 100 }, DEFAULT_FONT, 24, { 1, 1, 0 } );
    TextObject b1( "K", { 50, 100 }, DEFAULT_FONT, 24, { 1, 0, 0 } );

    a.init( { 0, 0 } );
    a1.init( { 0, 0 } );
    b.init( { 0, 0 } );
    b1.init( { 0, 0 } );

    while (!glfwWindowShouldClose( window )) {
        glClearColor( 0, 0.f, 0, 1 );
        glClear( GL_COLOR_BUFFER_BIT );

        a.render();
        a1.render();
        //b.render();
        //b1.render();
       
        if (rand() % 2)
            field.setTurn( 'x', rand() % FIELD_SIZE, rand() % FIELD_SIZE );
        else
            field.setTurn( 'o', rand() % FIELD_SIZE, rand() % FIELD_SIZE );
        field.render();
        glfwSwapBuffers( window );

        glfwPollEvents();
    }

    glfwTerminate();
}


int main( void )
{
    srand( static_cast<unsigned>(time( 0 )) );

    OpenGlInit();
    return 0;
}
