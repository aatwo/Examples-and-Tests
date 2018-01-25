#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "sdleventdelegate.h"
#include "timer.h"
#include <string>
#include <vector>
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "global.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include "spriterenderer.h"


class MainWindow : public SDLEventDelegate
{
    public:

                                                        MainWindow( SDL_Window* window, SDL_GLContext glcontext, int windowWidth, int windowHeight );
        virtual                                         ~MainWindow();

        void                                            Update( double elapsedSeconds, int elapsedMs );
        void                                            Render();


        virtual void                                    KeyDownEvent( SDL_KeyboardEvent* event ) override;
        virtual void                                    KeyUpEvent( SDL_KeyboardEvent* event ) override;
        virtual void                                    MouseButtonUpEvent( SDL_MouseButtonEvent* event ) override;
        virtual void                                    MouseButtonDownEvent( SDL_MouseButtonEvent* event ) override;
        virtual void                                    MouseMoveEvent( SDL_MouseMotionEvent* event ) override;
        virtual void                                    MouseWheelEvent( SDL_MouseWheelEvent* event ) override;
        virtual void                                    ResizeEvent( int newWidth, int newHeight ) override;


    private:


        void                                            Init();
        bool                                            LoadFile( const char* filePath, std::string& fileData_OUT );

        // Represents the current state of the game
        enum class GameState {

            GAME_ACTIVE,
            GAME_MENU,
            GAME_WIN
        };

        GameState                                       State = GameState::GAME_ACTIVE;
        GLboolean                                       Keys[1024];


        SpriteRenderer*                                 mSpriteRenderer = nullptr;
        SDL_Renderer*                                   mRenderer       = nullptr;
        SDL_Window*                                     mWindow         = nullptr;
        SDL_GLContext                                   mGlContext      = nullptr;
        GLuint                                          mWindowWidth    = 0;
        GLuint                                          mWindowHeight   = 0;


}; // MainWindow


#endif // MAINWINDOW_H
