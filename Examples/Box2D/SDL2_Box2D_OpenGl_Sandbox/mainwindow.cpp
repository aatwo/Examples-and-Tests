#include "mainwindow.h"
#include "timer.h"
#include <sstream>
#include <cstdlib>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <GL/glew.h>
#include <iostream>
#include <fstream>


MainWindow::MainWindow( SDL_Window* window, SDL_GLContext glcontext, int windowWidth, int windowHeight )
    :   mWindow( window ),
        mGlContext( glcontext ),
        mWindowWidth( windowWidth ),
        mWindowHeight( windowHeight )
{
    camPos              = b2Vec2( 0.0, 0.0 );
    minCamScale         = 0.4f;
    maxCamScale         = 12.0f;
    camScale            = minCamScale + ( 0.5 * ( maxCamScale - minCamScale ) );
    widgetCenterPos     = b2Vec2( 0.5 * windowWidth, 0.5 * windowHeight );
    speedMultiplier     = 1.0;
    worldTimeStep       = 1.0f / 30.0f; // value used in physics engine to determins world time step (i.e. 1.0 / x implies we are stepping at x Hz)
    velocityIterations  = 6; // times per step to compute impulses and resolve forces
    positionIterations  = 2; // times per step to compute and resolve body positions
    stepCount           = 0;

    if( !LoadFile( "Resources/vertexShader", mVertexShader ) )
        SDL_Log( "Failed to load vertex shader" );

    if( !LoadFile( "Resources/fragmentShader",   mFragmentShader ) )
        SDL_Log( "Failed to load fragment shader" );

    InitialiseOpenGl();
    InitialiseWorld();
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
        SimulateStep();
        currentElapsedMs -= simulationStepInterval;
    }

    //SDL_Log( "Update duration: %f seconds", timer.ElapsedS() );
}

void MainWindow::Render()
{
    static Timer renderTimer;
    static unsigned long long renderCount = 0;
    static unsigned long long elapsedMs = 0;
    static bool firstTimeRun = true;

    if( firstTimeRun )
    {
        renderTimer.Start();
        firstTimeRun = false;
    }

    renderCount++;
    elapsedMs = renderTimer.ElapsedMs();
    double fps = static_cast<double>( renderCount ) / ( static_cast<double>( elapsedMs ) / 1000.0 );

    if( elapsedMs >= 1000 ) {

        renderTimer.Restart();
        elapsedMs = 0;
        renderCount = 0;
    }

    static int bodyCount = 0;
    int newBodyCount = world->GetBodyCount();

    if( newBodyCount != bodyCount )
        bodyCount = newBodyCount;

    SDL_Log( "Fps: %f, Body count: %i", fps, bodyCount );

    // Rendering
    glViewport( 0, 0, mWindowWidth, mWindowHeight );
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );

    //glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
    glDrawArrays(GL_TRIANGLES, 0, 3);






    SDL_GL_SwapWindow( mWindow );

//    // Fill the screen blue
//    SDL_SetRenderDrawColor( mRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
//    SDL_RenderClear( mRenderer );
//    SDL_SetRenderDrawBlendMode( mRenderer, SDL_BLENDMODE_BLEND );

//    // Box2D
//    PaintWorld();

//    // Present to the screen
//    SDL_RenderPresent( mRenderer );


}

void MainWindow::KeyDownEvent( SDL_KeyboardEvent* event )
{
    switch( event->keysym.scancode )
    {
        case SDL_SCANCODE_ESCAPE:
        {
            creatingChain = false;
            chainPoints.clear();
            break;
        }

        case SDL_SCANCODE_RETURN:
        {
            if( chainPoints.size() > 1 )
            {
                creatingChain = false;
                CreateStaticChain( chainPoints );
                chainPoints.clear();
            }
            break;
        }

        case SDL_SCANCODE_P:
        {
            mPaused = !mPaused;
            break;
        }

        case SDL_SCANCODE_1:
        {
            CreatePolygonAtPos( currentMouseScreenPos, true );
            break;
        }

        case SDL_SCANCODE_2:
        {
            CreateRandomPolygonAtPos( currentMouseScreenPos, true );
            break;
        }

        case SDL_SCANCODE_3:
        {
            CreateCircleAtPos( currentMouseScreenPos, true );
            break;
        }

        case SDL_SCANCODE_4:
        {
            CreateRagDollAtPos( currentMouseScreenPos );
            break;
        }

        case SDL_SCANCODE_9:
        {
            for( int i = 0; i < 50; i++ )
            {
                int x = currentMouseScreenPos.x - ( i * 10 );
                int y = currentMouseScreenPos.y;
                b2Vec2 pos( x, y );
                CreateCircleAtPos( pos, true );
            }

            for( int i = 1; i < 50; i++ )
            {
                int x = currentMouseScreenPos.x + ( i * 10 );
                int y = currentMouseScreenPos.y;
                b2Vec2 pos( x, y );
                CreateCircleAtPos( pos, true );
            }
            break;
        }

        case SDL_SCANCODE_R:
        {
            CleanUpWorld();
            InitialiseWorld();
            camPos = b2Vec2( 0.0f, 0.0f );
            break;
        }

        case SDL_SCANCODE_KP_MINUS:
        {
            camScale -= 1.0;
            if( camScale < minCamScale )
                camScale = minCamScale;
            break;
        }

        case SDL_SCANCODE_KP_PLUS:
        {
            camScale += 1.0;
            if( camScale > maxCamScale )
                camScale = maxCamScale;
            break;
        }

        case SDL_SCANCODE_UP:
        {
            camPos.y += 10;
            break;
        }

        case SDL_SCANCODE_DOWN:
        {
            camPos.y -= 10;
            break;
        }

        case SDL_SCANCODE_LEFT:
        {
            camPos.x -= 10;
            break;
        }

        case SDL_SCANCODE_RIGHT:
        {
            camPos.x += 10;
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
    panningWithMouse = false;
    if( mouseManipulationBody )
    {
        world->DestroyBody( mouseManipulationBody );
        mouseManipulationBody = NULL;
        mouseManipulationJoint = NULL;
    }
}

void MainWindow::MouseButtonDownEvent( SDL_MouseButtonEvent* event )
{
    currentMouseScreenPos.x = event->x;
    currentMouseScreenPos.y = event->y;

    switch( event->button )
    {
        case SDL_BUTTON_LEFT:
        {
            b2Vec2 screenPos( event->x, event->y );
            b2Vec2 worldPos;
            ConvertWidgetCoordToWorldCoord( screenPos, worldPos );
            selectedBody = GetBodyAtPos( worldPos );

            if( selectedBody )
            {
                mouseManipulationBody = CreateStaticCircleSensor( worldPos, 0.2f );
                b2MouseJointDef def;
                def.bodyA = mouseManipulationBody;
                def.bodyB = selectedBody;
                def.maxForce = 1000.0f * selectedBody->GetMass();
                def.target = mouseManipulationBody->GetPosition();

                mouseManipulationJoint = (b2MouseJoint*)world->CreateJoint( &def );
            }

            else
            {
                panStartScreenPos = currentMouseScreenPos;
                panningWithMouse = true;
            }

            break;
        }

        case SDL_BUTTON_RIGHT:
        {
            creatingChain = true;
            b2Vec2 screenPos( event->x, event->y );
            b2Vec2 worldPos;
            ConvertWidgetCoordToWorldCoord( screenPos, worldPos );
            chainPoints.push_back( worldPos );
            break;
        }

        default: {}
    }
}

void MainWindow::MouseMoveEvent( SDL_MouseMotionEvent* event )
{
    currentMouseScreenPos.x = event->x;
    currentMouseScreenPos.y = event->y;

    if( mouseManipulationBody )
    {
        b2Vec2 screenPos( event->x, event->y );
        b2Vec2 worldPos;
        ConvertWidgetCoordToWorldCoord( screenPos, worldPos );

        mouseManipulationBody->SetTransform( worldPos, mouseManipulationBody->GetAngle() );
        mouseManipulationJoint->SetTarget( mouseManipulationBody->GetPosition() );
    }

    else if( panningWithMouse )
    {
        int xDelta = event->x - panStartScreenPos.x;
        int yDelta = event->y - panStartScreenPos.y;

        camPos.x = ( camPos.x - (xDelta / camScale) );
        camPos.y = ( camPos.y + (yDelta / camScale) );

        panStartScreenPos = currentMouseScreenPos;
    }
}

void MainWindow::MouseWheelEvent( SDL_MouseWheelEvent* event )
{
    camScale += event->y;

    if( camScale < minCamScale )
        camScale = minCamScale;
    if( camScale > maxCamScale )
        camScale = maxCamScale;
}

void MainWindow::ResizeEvent( int newWidth, int newHeight )
{
    SDL_Log( "Width: %i, Height: %i", newWidth, newHeight );

    widgetCenterPos.x = 0.5 * newWidth;
    widgetCenterPos.y = 0.5 * newHeight;

    mWindowWidth    = newWidth;
    mWindowHeight   = newHeight;

    glViewport( 0, 0, mWindowWidth, mWindowHeight );
}

void MainWindow::InitialiseOpenGl()
{
    glViewport( 0, 0, mWindowWidth, mWindowHeight );

    // Shader sources
    const GLchar* vertexSource = R"glsl(
                                 #version 150

                                 in vec2 position;

                                 void main()
                                 {
                                     gl_Position = vec4(position, 0.0, 1.0);
                                 }
    )glsl";
    const GLchar* fragmentSource = R"glsl(
                                   #version 150

                                   out vec4 outColor;

                                   void main()
                                   {
                                       outColor = vec4(1.0, 1.0, 1.0, 1.0);
                                   }
    )glsl";


    float vertices[] = {
         0.0f,  0.5f, // Vertex 1 (X, Y)
         0.5f, -0.5f, // Vertex 2 (X, Y)
        -0.5f, -0.5f  // Vertex 3 (X, Y)
    };

    // Copy vertex buffer to graphics card
    GLuint vbo;
    glGenBuffers(1, &vbo); // Generate 1 buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Compile shaders
    GLint status;

    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if( status != GL_TRUE ) {

        char buffer[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
        SDL_Log( "Error compiling vertex shader (%s)", buffer );
    }

    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if( status != GL_TRUE ) {

        char buffer[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
        SDL_Log( "Error compiling fragment shader (%s)", buffer );
    }

    // Create and link shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Use the program (this can be called when ever we need to switch programs)
    glUseProgram(shaderProgram);

    // Vertex array objects
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Attributes
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(posAttrib);
}

void MainWindow::InitialiseWorld()
{
    stepCount = 0;
    b2Vec2 gravity( 0.0f, -10.0f );
    world = new b2World( gravity );

    CreateStaticEdge( b2Vec2(-1000.0f, 0.0f), b2Vec2(1000.0f, 0.0f) );
}

void MainWindow::CleanUpWorld()
{
    selectedBody = nullptr;

    if( creatingChain )
        creatingChain = false;
    chainPoints.clear();

    if( world )
    {
        delete world;
        world = nullptr;
    }

    mouseManipulationBody = nullptr;
    mouseManipulationJoint = nullptr;
}

void MainWindow::SimulateStep()
{
    if( !world || mPaused )
        return;

    world->Step( worldTimeStep * speedMultiplier, velocityIterations, positionIterations );

    stepCount++;
}

void MainWindow::CreatePolygonAtPos( b2Vec2& pos, bool dynamic )
{
    b2Vec2 b2WorldPos;
    ConvertWidgetCoordToWorldCoord( pos, b2WorldPos );

    int     vertexCount = math::GetRandomInt( 3, b2_maxPolygonVertices );
    float   radius      = math::GetRandomFloat( 1.0f, 10.0f );

    if( dynamic )
        CreateDynamicPolygon( b2WorldPos, radius, 0.0f, 1.0f, 1.3f, 0.4f, vertexCount );
    else
        CreateStaticPolygon( b2WorldPos, radius, 0.0f, vertexCount );
}

void MainWindow::CreateRandomPolygonAtPos( b2Vec2& pos, bool dynamic )
{
    b2Vec2 b2WorldPos;
    ConvertWidgetCoordToWorldCoord( pos, b2WorldPos );

    int vertexCount = math::GetRandomInt( 3, b2_maxPolygonVertices );

    float radius = 2.0f + (math::GetRandomFloat(0.0f, 6.0f)); // Note: Careful not to make concave shapes. Box2D does not support this
    float minRad = radius;
    float maxRad = radius + 2.0f;

    if( dynamic )
        CreateDynamicPolygon( b2WorldPos, minRad, maxRad, 0.0f, 1.0f, 0.3f, 0.4f, vertexCount );
    else
        CreateStaticPolygon( b2WorldPos, minRad, maxRad, 0.0f, vertexCount );
}

void MainWindow::CreateCircleAtPos( b2Vec2& pos, bool dynamic )
{
    b2Vec2 b2WorldPos;
    ConvertWidgetCoordToWorldCoord( pos, b2WorldPos );

    float radius = math::GetRandomFloat( 1.0f, 10.0f );

    if( dynamic )
        CreateDynamicCircle( b2WorldPos, radius, 0.0f, 1.0f, 0.3f, 0.4f );
    else
        CreateStaticCircle( b2WorldPos, radius );
}

void MainWindow::CreateRagDollAtPos( b2Vec2& pos )
{
    b2Vec2 b2WorldPos;
    ConvertWidgetCoordToWorldCoord( pos, b2WorldPos );

    CreateRagDoll( b2WorldPos, 0.0f );
}

//------------------------------------------------------------------------------
b2Body* MainWindow::CreateStaticCircleSensor( b2Vec2 pos, float radius )
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position = pos;
    b2Body* body = world->CreateBody( &bodyDef );

    b2CircleShape shape;
    shape.m_radius = radius;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.isSensor = true;

    body->CreateFixture( &fixtureDef );

    return body;
}

//------------------------------------------------------------------------------
b2Body* MainWindow::CreateStaticCircle( b2Vec2 pos, float radius )
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position = pos;
    b2Body* body = world->CreateBody( &bodyDef );

    b2CircleShape shape;
    shape.m_radius = radius;

    body->CreateFixture( &shape, 0.0f );

    return body;
}

//------------------------------------------------------------------------------
b2Body* MainWindow::CreateStaticRectangle( b2Vec2 pos, b2Vec2 size, float angle )
{
    // Make the ground body
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set( pos.x, pos.y );
    bodyDef.angle = angle;
    b2Body* body = world->CreateBody( &bodyDef );

    // Set the ground body shape
    b2PolygonShape bodyShape;
    bodyShape.SetAsBox( size.x / 2.0f, size.y / 2.0f );

    // Fix the ground body to the ground shape
    body->CreateFixture( &bodyShape, 0.0f );

    return body;
}

//------------------------------------------------------------------------------
b2Body* MainWindow::CreateStaticPolygon( b2Vec2 pos, float radius, float angle, int32 vertexCount )
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.angle = angle;
    bodyDef.position = pos;
    b2Body* body = world->CreateBody( &bodyDef );
    b2PolygonShape shape;

    static b2Vec2 vectors[ 64 ];

    float angleIncrement = TWO_PIE / vertexCount;
    float cumulativeAngle = 0.0f;
    for( int i = 0; i < vertexCount; i++ )
    {
        geom::GetCoordForAngleAroundPoint( 0.0f, 0.0f, radius, cumulativeAngle, vectors[ i ] );
        cumulativeAngle += angleIncrement;
    }

    shape.Set( vectors, vertexCount );

    body->CreateFixture( &shape, 0.0f );

    return body;
}

//------------------------------------------------------------------------------
b2Body* MainWindow::CreateStaticPolygon( b2Vec2 pos, float radiusMin, float radiusMax, float angle, int32 vertexCount )
{
    // TODO - guard against concave polygons (ensuring internal angle at each vertex <= 180 degrees)

    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.angle = angle;
    bodyDef.position = pos;
    b2Body* body = world->CreateBody( &bodyDef );
    b2PolygonShape shape;

    b2Vec2* vectors = new b2Vec2[vertexCount];

    float angleIncrement = TWO_PIE / vertexCount;
    float cumulativeAngle = 0.0f;
    float radius;

    for( int i = 0; i < vertexCount; i++ )
    {
        radius = math::GetRandomFloat( radiusMin, radiusMax );
        geom::GetCoordForAngleAroundPoint( 0.0f, 0.0f, radius, cumulativeAngle, vectors[i] );
        cumulativeAngle += angleIncrement;
    }

    shape.Set( vectors, vertexCount );
    delete [] vectors;

    body->CreateFixture( &shape, 0.0f );

    return body;
}

//------------------------------------------------------------------------------
b2Body* MainWindow::CreateStaticEdge( b2Vec2 pos1, b2Vec2 pos2 )
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    b2Body* body = world->CreateBody( &bodyDef );

    b2EdgeShape shape;
    shape.Set( pos1, pos2 );

    b2FixtureDef fixDef;
    fixDef.shape = &shape;

    body->CreateFixture( &fixDef );

    return body;
}

//------------------------------------------------------------------------------
b2Body* MainWindow::CreateStaticChain( std::vector<b2Vec2> posList )
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    b2Body* body = world->CreateBody( &bodyDef );

    b2ChainShape shape;
    b2Vec2* posArray = new b2Vec2[posList.size()];
    for( int i = 0; i < posList.size(); i++ )
        posArray[i] = posList.at(i);
    shape.CreateChain(posArray, posList.size());

    body->CreateFixture( &shape, 0.0f );

    delete posArray;

    return body;
}

//------------------------------------------------------------------------------
b2Body* MainWindow::CreateDynamicCircle( b2Vec2 pos, float radius, float angle, float density, float friction, float restitution )
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set( pos.x, pos.y );
    bodyDef.angle = angle;
    b2Body* body = world->CreateBody( &bodyDef );

    b2CircleShape shape;
    shape.m_p.Set( 0.0f, 0.0f);
    shape.m_radius = radius;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    fixtureDef.restitution = restitution;

    body->CreateFixture( &fixtureDef );

    return body;
}

//------------------------------------------------------------------------------
b2Body* MainWindow::CreateDynamicRectangle( b2Vec2 pos, b2Vec2 size, float angle, float density, float friction, float restitution )
{
    b2BodyDef def;
    def.type = b2_dynamicBody;
    def.position.Set( pos.x, pos.y );
    def.angle = angle;

    b2Body* body = world->CreateBody( &def );

    b2PolygonShape shape;
    shape.SetAsBox( size.x / 2.0f, size.y / 2.0f );

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    fixtureDef.restitution = restitution;

    body->CreateFixture( &fixtureDef );

    return body;
}

//------------------------------------------------------------------------------
b2Body* MainWindow::CreateDynamicPolygon( b2Vec2 pos, float radius, float angle, float density, float friction, float restitution, int32 vertexCount )
{
    if( vertexCount < 3 )
        vertexCount = 3;

    b2BodyDef def;
    def.type = b2_dynamicBody;
    def.position.Set( pos.x, pos.y );
    def.angle = angle;

    b2Body* body = world->CreateBody( &def );

    b2Vec2* vectors = new b2Vec2[vertexCount];

    float angleIncrement = TWO_PIE / vertexCount;
    float cumulativeAngle = 0.0f;
    for( int i = 0; i < vertexCount; i++ )
    {
        geom::GetCoordForAngleAroundPoint( 0.0f, 0.0f, radius, cumulativeAngle, vectors[i] );
        cumulativeAngle += angleIncrement;
    }

    b2PolygonShape shape;
    shape.Set( vectors, vertexCount );

    delete [] vectors;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    fixtureDef.restitution = restitution;

    body->CreateFixture( &fixtureDef );

    return body;
}

//------------------------------------------------------------------------------
b2Body* MainWindow::CreateDynamicPolygon( b2Vec2 pos, float radiusMin, float radiusMax, float angle, float density, float friction, float restitution, int32 vertexCount )
{
    // TODO - guard against concave polygons (ensuring internal angle at each vertex <= 180 degrees)

    if( vertexCount < 3 )
        vertexCount = 3;

    b2BodyDef def;
    def.type = b2_dynamicBody;
    def.position.Set( pos.x, pos.y );
    def.angle = angle;

    b2Body* body = world->CreateBody( &def );

    b2Vec2* vectors = new b2Vec2[vertexCount];

    float angleIncrement = TWO_PIE / vertexCount;
    float cumulativeAngle = 0.0f;
    float radius;

    for( int i = 0; i < vertexCount; i++ )
    {
        radius = math::GetRandomFloat( radiusMin, radiusMax );
        geom::GetCoordForAngleAroundPoint( 0.0f, 0.0f, radius, cumulativeAngle, vectors[i] );
        cumulativeAngle += angleIncrement;
    }

    b2PolygonShape shape;
    shape.Set( vectors, vertexCount );

    delete [] vectors;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    fixtureDef.restitution = restitution;

    body->CreateFixture( &fixtureDef );

    return body;
}

//------------------------------------------------------------------------------
b2Body* MainWindow::CreateDynamicBody( b2Vec2 centerPos, float angle, std::vector<ShapeDef>& shapeDefinitions )
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.angle = angle;
    bodyDef.position = centerPos;

    b2Body* body = world->CreateBody( &bodyDef );

    for( int i = 0; i < shapeDefinitions.size(); i++ )
    {
        ShapeDef shapeDef = shapeDefinitions.at(i);
        switch( shapeDef.type )
        {
            case b2Shape::e_polygon:
            {
                b2PolygonShape shape;

                // Offset the polygon vertices by the relativeCenter
                b2Vec2* vertices = new b2Vec2[shapeDef.vertices.size()];
                for( int j = 0; j < shapeDef.vertices.size(); j++ )
                    vertices[j] = b2Vec2( shapeDef.vertices.at(j).x + shapeDef.relativeCenter.x, shapeDef.vertices.at(j).y + shapeDef.relativeCenter.y );

                shape.Set( vertices, shapeDef.vertices.size() );
                delete [] vertices;

                b2FixtureDef fixtureDef;
                fixtureDef.restitution = shapeDef.restitution;
                fixtureDef.density = shapeDef.density;
                fixtureDef.friction = shapeDef.friction;
                fixtureDef.shape = &shape;

                body->CreateFixture( &fixtureDef );

                break;
            }
            case b2Shape::e_circle:
            {
                b2CircleShape shape;

                shape.m_p = shapeDef.relativeCenter;
                shape.m_radius = shapeDef.radius;

                b2FixtureDef fixtureDef;
                fixtureDef.restitution = shapeDef.restitution;
                fixtureDef.density = shapeDef.density;
                fixtureDef.friction = shapeDef.friction;
                fixtureDef.shape = &shape;

                body->CreateFixture( &fixtureDef );
                break;
            }

            default: {}
        }
    }

    return body;
}

//------------------------------------------------------------------------------
void MainWindow::CreateRagDoll( b2Vec2 pos, float angle )
{
    // Test function

    b2Vec2 headPos = pos;
    headPos.x += 1.0;
    b2Body* head = CreateDynamicCircle( headPos, 2.0f, 0.0f, 1.0f, 0.3f, 0.4f );
    b2Body* neck = CreateDynamicRectangle( b2Vec2(pos.x + 1.0f, pos.y - 2.2f), b2Vec2(2.0f, 2.0f), 0.0f, 1.0f, 0.3f, 0.4f );

    // Neck-Head Joint
    b2RevoluteJointDef neckJointDef;
    neckJointDef.enableLimit = true;
    neckJointDef.lowerAngle = -PIE_OVER_FOUR;
    neckJointDef.upperAngle = PIE_OVER_FOUR;
    b2Vec2 neckJointPos( pos.x + 1.0f, pos.y - 2.2f + 1.0f );
    neckJointDef.Initialize( head, neck, neckJointPos );
    world->CreateJoint( &neckJointDef );

    b2Body* torso = CreateDynamicRectangle( b2Vec2(pos.x + 1.0f, pos.y - 2.2f - 5.6f), b2Vec2(3.0f, 10.0f), 0.0f, 1.0f, 0.3f, 0.4f );

    // Torso-Neck joint
    b2RevoluteJointDef torsoJointDef;
    torsoJointDef.enableLimit = true;
    torsoJointDef.lowerAngle = -PIE_OVER_FOUR;
    torsoJointDef.upperAngle = PIE_OVER_FOUR;
    b2Vec2 torsoJointPos( pos.x + 1.0f, pos.y - 2.2f - 3.6f + 2.0f );
    torsoJointDef.Initialize( neck, torso, torsoJointPos );
    world->CreateJoint( &torsoJointDef );

    b2Body* leftArmTop = CreateDynamicRectangle( b2Vec2( pos.x - 2.5f, pos.y - 3.8f ), b2Vec2( 5.0f, 1.5f ), 0.0f, 1.0f, 0.3f, 0.4f );
    b2Body* leftArmBottom = CreateDynamicRectangle( b2Vec2( pos.x - 7.0f, pos.y - 3.8f ), b2Vec2( 5.0f, 1.5f ), 0.0f, 1.0f, 0.3f, 0.4f );

    b2Body* rightArmTop = CreateDynamicRectangle( b2Vec2( pos.x + 4.6f, pos.y - 3.8f ), b2Vec2( 5.0f, 1.5f ), 0.0f, 1.0f, 0.3f, 0.4f );
    b2Body* rightArmBottom = CreateDynamicRectangle( b2Vec2( pos.x + 9.1f, pos.y - 3.8f ), b2Vec2( 5.0f, 1.5f ), 0.0f, 1.0f, 0.3f, 0.4f );

    // Top Left Shoulder Joint
    b2RevoluteJointDef leftShoulderJointDef;
    leftShoulderJointDef.enableLimit = true;
    leftShoulderJointDef.lowerAngle = -DEGREES_TO_RADIANS(90);
    leftShoulderJointDef.upperAngle = DEGREES_TO_RADIANS(90);
    b2Vec2 leftShoulderJointPos( pos.x - 0.5f, pos.y - 2.2f - 3.6f + 2.0f );
    leftShoulderJointDef.Initialize( leftArmTop, torso, leftShoulderJointPos );
    world->CreateJoint( &leftShoulderJointDef );

    // Left Arm Joint
    b2RevoluteJointDef leftArmJointDef;
    leftArmJointDef.enableLimit = true;
    leftArmJointDef.lowerAngle = -DEGREES_TO_RADIANS(90);
    leftArmJointDef.upperAngle = DEGREES_TO_RADIANS(90);
    b2Vec2 leftArmJointPos( pos.x - 4.6f, pos.y - 2.2f - 3.6f + 2.0f );
    leftArmJointDef.Initialize( leftArmBottom, leftArmTop, leftArmJointPos );
    world->CreateJoint( &leftArmJointDef );

    // Top Right Shoulder Joint
    b2RevoluteJointDef rightShoulderJointDef;
    rightShoulderJointDef.enableLimit = true;
    rightShoulderJointDef.lowerAngle = -DEGREES_TO_RADIANS(90);
    rightShoulderJointDef.upperAngle = DEGREES_TO_RADIANS(90);
    b2Vec2 rightShoulderJointPos( pos.x + 2.5f, pos.y - 2.2f - 3.6f + 2.0f );
    rightShoulderJointDef.Initialize( rightArmTop, torso, rightShoulderJointPos );
    world->CreateJoint( &rightShoulderJointDef );

    b2RevoluteJointDef rightArmJointDef;
    rightArmJointDef.enableLimit = true;
    rightArmJointDef.lowerAngle = -DEGREES_TO_RADIANS(90);
    rightArmJointDef.upperAngle = DEGREES_TO_RADIANS(90);
    b2Vec2 rightArmJointPos( pos.x + 6.9f, pos.y - 2.2f - 3.6f + 2.0f );
    rightArmJointDef.Initialize( rightArmBottom, rightArmTop, rightArmJointPos );
    world->CreateJoint( &rightArmJointDef );// Right Arm Joint


    b2Body* leftLegTop = CreateDynamicRectangle( b2Vec2( pos.x, pos.y - 14.8f ), b2Vec2( 1.5f, 5.0f ), 0.0f, 1.0f, 0.3f, 0.4f );
    b2Body* leftLegBottom = CreateDynamicRectangle( b2Vec2( pos.x, pos.y - 19.3f ), b2Vec2( 1.5f, 5.0f ), 0.0f, 1.0f, 0.3f, 0.4f );

    b2Body* rightLegTop = CreateDynamicRectangle( b2Vec2( pos.x + 2.0f, pos.y - 14.8f ), b2Vec2( 1.5f, 5.0f ), 0.0f, 1.0f, 0.3f, 0.4f );
    b2Body* rightLegBottom = CreateDynamicRectangle( b2Vec2( pos.x + 2.0f, pos.y - 19.3f ), b2Vec2( 1.5f, 5.0f ), 0.0f, 1.0f, 0.3f, 0.4f );

    // Left Hip Joint
    b2RevoluteJointDef leftHipJointDef;
    leftHipJointDef.enableLimit = true;
    leftHipJointDef.lowerAngle = -DEGREES_TO_RADIANS(90);
    leftHipJointDef.upperAngle = DEGREES_TO_RADIANS(90);
    b2Vec2 leftHipJointPos( pos.x + 0.0f, pos.y - 12.6f );
    leftHipJointDef.Initialize( leftLegTop, torso, leftHipJointPos );
    world->CreateJoint( &leftHipJointDef );

    // Left Leg Joint
    b2RevoluteJointDef leftLegJointDef;
    leftLegJointDef.enableLimit = true;
    leftLegJointDef.lowerAngle = -DEGREES_TO_RADIANS(90);
    leftLegJointDef.upperAngle = DEGREES_TO_RADIANS(90);
    b2Vec2 leftLegJointPos( pos.x + 0.0f, pos.y - 17.1f );
    leftLegJointDef.Initialize( leftLegBottom, leftLegTop, leftLegJointPos );
    world->CreateJoint( &leftLegJointDef );

    // Right Hip Joint
    b2RevoluteJointDef rightHipJointDef;
    rightHipJointDef.enableLimit = true;
    rightHipJointDef.lowerAngle = -DEGREES_TO_RADIANS(90);
    rightHipJointDef.upperAngle = DEGREES_TO_RADIANS(90);
    b2Vec2 rightHipJointPos( pos.x + 2.0f, pos.y - 12.6f );
    rightHipJointDef.Initialize( rightLegTop, torso, rightHipJointPos );
    world->CreateJoint( &rightHipJointDef );

    // Right Leg Joint
    b2RevoluteJointDef rightLegJointDef;
    rightLegJointDef.enableLimit = true;
    rightLegJointDef.lowerAngle = -DEGREES_TO_RADIANS(90);
    rightLegJointDef.upperAngle = DEGREES_TO_RADIANS(90);
    b2Vec2 rightLegJointPos( pos.x + 2.0f, pos.y - 17.1f );
    rightLegJointDef.Initialize( rightLegBottom, rightLegTop, rightLegJointPos );
    world->CreateJoint( &rightLegJointDef );

}

b2Body* MainWindow::GetBodyAtPos( b2Vec2 pos )
{
    b2Shape* shape;

    for( b2Body* body = world->GetBodyList(); body; body = body->GetNext() )
    {
        for( b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext() )
        {
            shape = fixture->GetShape();
            switch( shape->GetType() )
            {
                case b2Shape::e_circle:
                case b2Shape::e_polygon:
                {
                    b2Transform transform;
                    transform.SetIdentity();
                    transform.Set( body->GetPosition(), body->GetAngle() );
                    if( shape->TestPoint( transform, pos ) )
                        return body;
                    break;
                }
                default: {}
            }

        }
    }

    return nullptr;
}

void MainWindow::ConvertWorldDistanceToPixels( float &in_distance, float& out_pixelDistance )
{
    b2Vec2 worldPosA( 0.0, 0.0 );
    b2Vec2 worldPosB( 1.0, 0.0 );
    b2Vec2 widgetPosA;
    b2Vec2 widgetPosB;

    ConvertWorldCoordToWidgetCoord( worldPosA, widgetPosA );
    ConvertWorldCoordToWidgetCoord( worldPosB, widgetPosB );

    out_pixelDistance = in_distance * std::abs( widgetPosB.x - widgetPosA.x );
}

void MainWindow::ConvertWorldSizeToPixels( b2Vec2& in_size, b2Vec2& out_size )
{
    float worldWidth, worldHeight, widgetWidth, widgetHeight;

    worldWidth  = in_size.x;
    worldHeight = in_size.y;

    ConvertWorldDistanceToPixels( worldWidth, widgetWidth );
    ConvertWorldDistanceToPixels( worldHeight, widgetHeight);

    out_size.x = widgetWidth;
    out_size.y = widgetHeight;
}

void MainWindow::ConvertWorldCoordToWidgetCoord( b2Vec2& in_worldCoord, b2Vec2& out_widgetCoord )
{
    float deltaX = in_worldCoord.x - camPos.x;
    float deltaY = in_worldCoord.y - camPos.y;

    out_widgetCoord.x = widgetCenterPos.x + (deltaX * camScale);
    out_widgetCoord.y = widgetCenterPos.y + (-deltaY * camScale);
}

void MainWindow::ConvertWidgetCoordToWorldCoord( b2Vec2& in_widgetCoord, b2Vec2& out_worldCoord )
{
    float deltaX = (in_widgetCoord.x - widgetCenterPos.x) / camScale;
    float deltaY = (in_widgetCoord.y - widgetCenterPos.y) / camScale;

    out_worldCoord.x = ( deltaX + camPos.x );
    out_worldCoord.y = ( -deltaY + camPos.y );
}

void MainWindow::PaintWorld()
{
    if( !world )
        return;
        
    b2Vec2      axisPos( 0.0, 0.0 );
    b2Vec2      axisSize( 10.0, 10.0 );
    float       axisLineWidth   = 1.0;
    SDL_Color   red             = { 255, 0, 0, 255 };
    SDL_Color   blue            = { 0, 0, 255, 255 };
    SDL_Color   black           = { 0, 0, 0, 255 };
    SDL_Color   transparentBlue = { 0, 0, 255, 150 };
    SDL_Color   green           = { 0, 255, 0, 255 };

    World_PaintCross( axisPos, axisSize, red, axisLineWidth );

    if( creatingChain )
    {
        for( int i = 0; i < chainPoints.size(); i++ )
        {
            World_PaintCross( chainPoints.at(i), axisSize, blue, 1 );
        }
    }

    PaintBodyWireframes( transparentBlue, 1, black );

    if( mouseManipulationBody )
    {
        PaintBodyWireframe( mouseManipulationBody, blue, 1, black );

        b2Vec2 b2pos1, b2pos2;
        b2Vec2 pos1, pos2;

        b2pos1 = mouseManipulationBody->GetPosition();
        b2pos2 = selectedBody->GetPosition();

        ConvertWorldCoordToWidgetCoord( b2pos1, pos1 );
        ConvertWorldCoordToWidgetCoord( b2pos2, pos2 );

        SDL_SetRenderDrawColor( mRenderer, red.r, red.g, red.b, red.a );
        SDL_RenderDrawLine( mRenderer, pos1.x, pos1.y, pos2.x, pos2.y );
    }

    if( camScale >= 5.0 )
        PaintJoints( green );
}

void MainWindow::PaintBodyWireframes( SDL_Color& fillColour, int borderWidth, SDL_Color& borderColour )
{
    for( b2Body* body = world->GetBodyList(); body; body = body->GetNext() )
        PaintBodyWireframe( body, fillColour, borderWidth, borderColour );
}

void MainWindow::PaintBodyWireframe( b2Body* body, SDL_Color& fillColour, int borderWidth, SDL_Color& borderColour )
{
    float angleRads = body->GetAngle();
    b2Vec2 widgetPos;
    b2Vec2 pos = body->GetPosition();
    ConvertWorldCoordToWidgetCoord( pos, widgetPos );

    /*
        A Quick note on Box2D bodies...

            - A body may be composed of one or more shapes
            - Each body has a position and all shapes are defined in local coordinates relative to the body position
            - Each shape has a rotation value which is also relative to the body position
            - So to get the screen position of a vertex for some shape you have to do the following:
                - Rotate each local shape vertex around the shape center
                - Translate this result by the body center
            - This has to be done manually because SDLs doesn't support primitive transformations

    */

    SDL_SetRenderDrawColor( mRenderer, 0, 0, 0, 255 );
    SDL_Color lineColour = { 0, 0, 0, 255 };

    // Render all the shapes that are part of this body
    b2Shape* shape;
    for( b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext() )
    {
        shape = fixture->GetShape();
        switch( shape->GetType() )
        {
            case b2Shape::e_circle:
            {
                b2CircleShape* circle = (b2CircleShape*)fixture->GetShape();

                float radius;
                ConvertWorldDistanceToPixels( circle->m_radius, radius );

                b2Vec2 worldLineEnd( pos.x + circle->m_p.x, pos.y + circle->m_p.y + circle->m_radius );
                geom::RotatePoint( pos.x, pos.y, angleRads, worldLineEnd.x, worldLineEnd.y );
                
                b2Vec2 widgetCenter, widgetLineEnd;
                ConvertWorldCoordToWidgetCoord( pos, widgetCenter );
                ConvertWorldCoordToWidgetCoord( worldLineEnd, widgetLineEnd);

                filledCircleRGBA( mRenderer,  widgetCenter.x, widgetCenter.y, radius, fillColour.r, fillColour.g, fillColour.b, fillColour.a );
                circleRGBA( mRenderer, widgetCenter.x, widgetCenter.y, radius, lineColour.r, lineColour.g, lineColour.b, lineColour.a );
                SDL_RenderDrawLine( mRenderer, widgetCenter.x, widgetCenter.y, widgetLineEnd.x, widgetLineEnd.y );

                break;
            }
            case b2Shape::e_edge:
            {
                b2EdgeShape* edge = (b2EdgeShape*)shape;

                // Note:    vertices 0 and 3 are ghost vertices used internally to prevent ghost collisions
                //          which occurs when a body moves from one edge to another, causing a slight body
                //          movement even if those edges are perfectly aligned.
                //          Vertices 1 and 2 are the edge describing vertices.
                b2Vec2 pos1;
                b2Vec2 pos2;

                ConvertWorldCoordToWidgetCoord( edge->m_vertex1, pos1 );
                ConvertWorldCoordToWidgetCoord( edge->m_vertex2, pos2 );
                SDL_RenderDrawLine( mRenderer, pos1.x, pos1.y, pos2.x, pos2.y );
                break;
            }
            case b2Shape::e_polygon:
            {
                b2PolygonShape* polygon = (b2PolygonShape*)fixture->GetShape();

                b2Vec2 worldPos;
                b2Vec2 widgetPos;
                b2Vec2 localPos;

                static Sint16 x_array[ 64 ];
                static Sint16 y_array[ 64 ];
                
                for( int i = polygon->GetVertexCount() - 1; i >= 0; i-- )
                {
                    localPos = polygon->GetVertex( i );
                    geom::RotatePoint( 0.0, 0.0, angleRads, localPos.x, localPos.y );

                    worldPos.x = ( pos.x + localPos.x );
                    worldPos.y = ( pos.y + localPos.y );

                    ConvertWorldCoordToWidgetCoord( worldPos, widgetPos );

                    x_array[ i ] = widgetPos.x;
                    y_array[ i ] = widgetPos.y;
                }
                    
                filledPolygonRGBA( mRenderer, x_array, y_array, polygon->GetVertexCount(), fillColour.r, fillColour.g, fillColour.b, fillColour.a );
                polygonRGBA( mRenderer, x_array, y_array, polygon->GetVertexCount(), lineColour.r, lineColour.g, lineColour.b, lineColour.a );
                break;
            }
            case b2Shape::e_chain:
            {
                b2ChainShape* chain = (b2ChainShape*)shape;
                b2Vec2 pos1;
                b2Vec2 pos2;
                for( int32 i = 0; i < chain->GetChildCount(); ++i )
                {
                    b2EdgeShape edge;
                    chain->GetChildEdge(&edge, i);

                    ConvertWorldCoordToWidgetCoord( edge.m_vertex1, pos1 );
                    ConvertWorldCoordToWidgetCoord( edge.m_vertex2, pos2 );
                    SDL_RenderDrawLine( mRenderer, pos1.x, pos1.y, pos2.x, pos2.y );
                }

                break;
            }

            default: {}
        }
    }
}

void MainWindow::PaintJoints( SDL_Color& colour )
{
    for( b2Joint* joint = world->GetJointList(); joint; joint = joint->GetNext() )
        Paintjoint( joint, colour );
}

void MainWindow::Paintjoint( b2Joint* joint, SDL_Color& colour )
{
    switch( joint->GetType() )
    {
        case e_revoluteJoint:
        {
            b2RevoluteJoint* j = (b2RevoluteJoint*)joint;
            b2Vec2 anchorPos = j->GetAnchorA();
            b2Vec2 wPos;
            ConvertWorldCoordToWidgetCoord( anchorPos, wPos );
            Widget_PaintCross( wPos, b2Vec2(10, 10), colour, 1.0f );
            break;
        }

        case e_mouseJoint:
        {
            b2MouseJoint* j = (b2MouseJoint*)joint;
            b2Vec2 worldPos = j->GetTarget();
            b2Vec2 wPos;
            ConvertWorldCoordToWidgetCoord( worldPos, wPos );
            Widget_PaintCross( wPos, b2Vec2(10, 10), colour, 1.0f );
            break;
        }

        default: {}
    }
}

void MainWindow::World_PaintCross( b2Vec2 pos, b2Vec2 size, SDL_Color& colour, float lineWidth )
{
    b2Vec2 widgetPos;
    ConvertWorldCoordToWidgetCoord( pos, widgetPos );

    b2Vec2 widgetSize;
    ConvertWorldSizeToPixels( size, widgetSize );

    Widget_PaintCross( widgetPos, widgetSize, colour, lineWidth );
}

void MainWindow::Widget_PaintCross( b2Vec2 pos, b2Vec2 size, SDL_Color& colour, float lineWidth )
{
    float a_x1 = pos.x - size.x / 2;
    float a_y1 = pos.y;
    float a_x2 = pos.x + size.x / 2;
    float a_y2 = pos.y;

    float b_x1 = pos.x;
    float b_y1 = pos.y - size.y / 2;
    float b_x2 = pos.x;
    float b_y2 = pos.y + size.y / 2;

    SDL_SetRenderDrawColor( mRenderer, colour.r, colour.g, colour.b, colour.a );

    SDL_RenderDrawLine( mRenderer, a_x1, a_y1, a_x2, a_y2 );
    SDL_RenderDrawLine( mRenderer, b_x1, b_y1, b_x2, b_y2 );
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
