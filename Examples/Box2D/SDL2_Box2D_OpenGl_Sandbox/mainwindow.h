#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "sdleventdelegate.h"
#include "timer.h"
#include <string>
#include <vector>
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <Box2D/Box2D.h>
#include <Box2D/Common/b2Math.h>
#include "global.h"


class Player;
class Enemy;
class FpsRenderer;
class PathFinder;


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


        struct ShapeDef
        {
            // This struct is used to describe a shape and the parameters of its fixture to some arbitrary body
            ShapeDef()
            {
                restitution     = 0.4f;
                density         = 1.0f;
                friction        = 0.3f;
                relativeCenter  = b2Vec2( 0.0f, 0.0f );
                type            = b2Shape::e_circle;
                radius          = 1.0f;
            }

            b2Shape::Type       type;               // e_circle, e_edge, e_polygon, e_chain
            b2Vec2              relativeCenter;     // The shapes center pos relative to the body
            float               restitution;        // 0.0 => no bounce, 1.0 => lots of bounce
            float               density;
            float               friction;
            float               radius;             // Circle only
            std::vector<b2Vec2> vertices;           // Polygon only - these should be relative to the shapes center, not the body. The offset from the body center will be computed using relativeCenter.
        };


        void                                            InitialiseOpenGl();
        void                                            InitialiseWorld();
        void                                            CleanUpWorld();

        void                                            SimulateStep();
        void                                            CreatePolygonAtPos( b2Vec2& pos, bool dynamic = true );
        void                                            CreateRandomPolygonAtPos( b2Vec2& pos, bool dynamic = true );
        void                                            CreateCircleAtPos( b2Vec2& pos, bool dynamic = true );
        void                                            CreateRagDollAtPos( b2Vec2& pos );

        // Static shapes
        b2Body*                                         CreateStaticCircleSensor( b2Vec2 pos, float radius );
        b2Body*                                         CreateStaticCircle( b2Vec2 pos, float radius );
        b2Body*                                         CreateStaticRectangle( b2Vec2 pos, b2Vec2 size, float angle );
        b2Body*                                         CreateStaticPolygon( b2Vec2 pos, float radius, float angle, int32 vertexCount );
        b2Body*                                         CreateStaticPolygon( b2Vec2 pos, float radiusMin, float radiusMax, float angle, int32 vertexCount );
        b2Body*                                         CreateStaticEdge( b2Vec2 pos1, b2Vec2 pos2 );
        b2Body*                                         CreateStaticChain( std::vector<b2Vec2> posList );

        // Dynamic shapes
        b2Body*                                         CreateDynamicCircle( b2Vec2 pos, float radius, float angle, float density, float friction, float restitution );
        b2Body*                                         CreateDynamicRectangle( b2Vec2 pos, b2Vec2 size, float angle, float density, float friction, float restitution );
        b2Body*                                         CreateDynamicPolygon( b2Vec2 pos, float radius, float angle, float density, float friction, float restitution, int32 vertexCount );
        b2Body*                                         CreateDynamicPolygon( b2Vec2 pos, float radiusMin, float radiusMax, float angle, float density, float friction, float restitution, int32 vertexCount );
        b2Body*                                         CreateDynamicBody( b2Vec2 centerPos, float angle, std::vector<ShapeDef>& shapeDefinitions );
        void                                            CreateRagDoll( b2Vec2 pos, float angle );

        // Coordinate system methods
        b2Body*                                         GetBodyAtPos( b2Vec2 pos );
        void                                            ConvertWorldDistanceToPixels( float &in_distance, float&out_pixelDistance );
        void                                            ConvertWorldSizeToPixels( b2Vec2& in_size, b2Vec2& out_size );
        void                                            ConvertWorldCoordToWidgetCoord( b2Vec2& in_worldCoord, b2Vec2& out_widgetCoord );
        void                                            ConvertWidgetCoordToWorldCoord( b2Vec2& in_widgetCoord, b2Vec2& out_worldCoord );

        // Paint methods
        void                                            PaintWorld();
        void                                            PaintBodyWireframes( SDL_Color& fillColour, int borderWidth, SDL_Color& borderColour );
        void                                            PaintBodyWireframe( b2Body* body, SDL_Color& fillColour, int borderWidth, SDL_Color& borderColour );
        void                                            PaintJoints( SDL_Color& colour );
        void                                            Paintjoint( b2Joint* joint, SDL_Color& colour );
        void                                            World_PaintCross( b2Vec2 pos, b2Vec2 size, SDL_Color& colour, float lineWidth );
        void                                            Widget_PaintCross( b2Vec2 pos, b2Vec2 size, SDL_Color& colour, float lineWidth );
        bool                                            LoadFile( const char* filePath, std::string& fileData_OUT );



        SDL_Renderer*                                   mRenderer       = nullptr;
        SDL_Window*                                     mWindow         = nullptr;
        SDL_GLContext                                   mGlContext      = nullptr;
        int                                             mWindowWidth    = 0;
        int                                             mWindowHeight   = 0;
        bool                                            mPaused         = false;
        std::string                                     mVertexShader;
        std::string                                     mFragmentShader;

        // Box2D stuff
        float                                           speedMultiplier;
        float                                           worldTimeStep;
        int                                             velocityIterations;
        int                                             positionIterations;
        long long int                                   stepCount;
        b2World*                                        world = nullptr;

        // Render stuff
        b2Vec2                                          camPos; // position of the camera in physics model coords
        float                                           camScale; // effectivly the camera zoom
        float                                           minCamScale;
        float                                           maxCamScale;
        b2Vec2                                          widgetCenterPos; // The windows center pos in screen coords
        b2Vec2                                          currentMouseScreenPos;

        // Some control related items
        b2Body*                                         mouseManipulationBody   = nullptr;
        b2MouseJoint*                                   mouseManipulationJoint  = nullptr;
        b2Body*                                         selectedBody            = nullptr;
        std::vector<b2Vec2>                             chainPoints;
        bool                                            creatingChain           = false;
        bool                                            panningWithMouse        = false;
        b2Vec2                                          panStartScreenPos;


}; // MainWindow


#endif // MAINWINDOW_H
