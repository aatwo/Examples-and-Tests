#include "mainwindow.h"
#include "timer.h"
#include <sstream>
#include <cstdlib>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include "resourcemanager.h"
#include <glm/gtc/matrix_transform.hpp>


MainWindow::MainWindow( SDL_Window* window, SDL_GLContext glcontext, int windowWidth, int windowHeight )
    :   mWindow( window ),
        mGlContext( glcontext ),
        mWindowWidth( windowWidth ),
        mWindowHeight( windowHeight )
{
    Init();
}

MainWindow::~MainWindow()
{
}

void MainWindow::Update( double elapsedS, int elapsedMs )
{   
    static long long currentElapsedMs = 0;
    static int simulationStepInterval = 1000 / 60;

    currentElapsedMs += elapsedMs;
    if( currentElapsedMs >= simulationStepInterval )
    {
        // TODO - progress game

        currentElapsedMs -= simulationStepInterval;
    }

    //SDL_Log( "Update duration: %f seconds", timer.ElapsedS() );
}

void MainWindow::Render()
{
    static Timer renderTimer;
    static unsigned long long renderCount   = 0;
    static unsigned long long elapsedMs     = 0;

    renderCount++;
    elapsedMs = renderTimer.ElapsedMs();
    double fps = static_cast<double>( renderCount ) / ( static_cast<double>( elapsedMs ) / 1000.0 );

    if( elapsedMs >= 1000 ) {

        renderTimer.Restart();
        elapsedMs = 0;
        renderCount = 0;
    }

    // Rendering
    glViewport( 0, 0, mWindowWidth, mWindowHeight );
    glClearColor( 0.8f, 0.2f, 0.2f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );


    // TODO - render
    mSpriteRenderer->DrawSprite( ResourceManager::GetTexture("face"),  glm::vec2(200, 200), glm::vec2(300, 400), 0.0, glm::vec3(0.0f, 1.0f, 0.0f) );



    SDL_GL_SwapWindow( mWindow );
}

void MainWindow::KeyDownEvent( SDL_KeyboardEvent* event )
{
    switch( event->keysym.scancode )
    {
        case SDL_SCANCODE_ESCAPE:
        {
            break;
        }

        case SDL_SCANCODE_RETURN:
        {
            break;
        }

        case SDL_SCANCODE_P:
        {
            break;
        }

        case SDL_SCANCODE_1:
        {
            break;
        }

        case SDL_SCANCODE_2:
        {
            break;
        }

        case SDL_SCANCODE_3:
        {
            break;
        }

        case SDL_SCANCODE_4:
        {
            break;
        }

        case SDL_SCANCODE_9:
        {
            break;
        }

        case SDL_SCANCODE_R:
        {
            break;
        }

        case SDL_SCANCODE_KP_MINUS:
        {
            break;
        }

        case SDL_SCANCODE_KP_PLUS:
        {
            break;
        }

        case SDL_SCANCODE_UP:
        {
            break;
        }

        case SDL_SCANCODE_DOWN:
        {
            break;
        }

        case SDL_SCANCODE_LEFT:
        {
            break;
        }

        case SDL_SCANCODE_RIGHT:
        {
            break;
        }

        default: {}
    }
}

void MainWindow::KeyUpEvent( SDL_KeyboardEvent* event )
{
    // TODO
}

void MainWindow::MouseButtonUpEvent( SDL_MouseButtonEvent* event )
{
    // TODO
}

void MainWindow::MouseButtonDownEvent( SDL_MouseButtonEvent* event )
{
    switch( event->button )
    {
        case SDL_BUTTON_LEFT:
        {
            break;
        }

        case SDL_BUTTON_RIGHT:
        {
            break;
        }

        default: {}
    }
}

void MainWindow::MouseMoveEvent( SDL_MouseMotionEvent* event )
{
    // TODO
}

void MainWindow::MouseWheelEvent( SDL_MouseWheelEvent* event )
{
    // TODO
}

void MainWindow::ResizeEvent( int newWidth, int newHeight )
{
    SDL_Log( "Width: %i, Height: %i", newWidth, newHeight );

    mWindowWidth    = newWidth;
    mWindowHeight   = newHeight;

    glViewport( 0, 0, mWindowWidth, mWindowHeight );
}

void MainWindow::Init()
{
    glViewport( 0, 0, mWindowWidth, mWindowHeight );

    // Load shaders
    ResourceManager::LoadShader("Resources/shaders/spriteVertexShader.vs", "Resources/shaders/spriteFragmentShader.frag", nullptr, "sprite");

    // Configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->mWindowWidth), static_cast<GLfloat>(this->mWindowHeight), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // Set render-specific controls

    mSpriteRenderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    // Load textures
    ResourceManager::LoadTexture("Resources/textures/face.png", GL_TRUE, "face");
}

bool MainWindow::LoadFile( const char* filePath, std::string& fileData_OUT ) {

    bool            ret = true;
    std::ifstream   file;
    std::string     line;

    file.open( filePath );

    if( file.is_open() )
    {
        while( file.good() )
        {
            std::getline( file, line );
            fileData_OUT.append( line + "\n" );
        }
    }

    else
    {
        ret = false;
    }

    file.close();
    return ret;
}
