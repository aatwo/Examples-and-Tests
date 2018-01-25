#include "box2dsimulationwidget.h"

Box2DSimulationWidget::Box2DSimulationWidget(QWidget *parent) :
    QWidget(parent)
{
    setMouseTracking(true);

    world = NULL;

    camPos = QPointF( 0.0, 0.0 );
    minCamScale = 0.4f;
    maxCamScale = 12.0f;
    camScale = minCamScale + ( 0.5 * ( maxCamScale - minCamScale ) );

    speedMultiplier = 1.0;
    worldTimeStep = 1.0f / 30.0f; // value used in physics engine to determins world time step (i.e. 1.0 / x implies we are stepping at x Hz)
    velocityIterations = 6; // times per step to compute impulses and resolve forces
    positionIterations = 2; // times per step to compute and resolve body positions
    stepCount = 0;

    simulationTimer = new QTimer( this );
    simulationTimer->setInterval( 10 ); // aim for approx 30 fps for the physics engine
    connect( simulationTimer, SIGNAL(timeout()), this, SLOT(SimulateStep()) );

    renderTimer = new QTimer( this );
    renderTimer->setInterval( 1000.0 / 60.0 ); // aim for approx 60 fps for the render engine
    connect( renderTimer, SIGNAL(timeout()), this, SLOT(Render()) );
    renderTimer->start();

    mouseManipulationBody = NULL;
    mouseManipulationJoint = NULL;
    selectedBody = NULL;
    creatingChain = false;
    scrollingWithMouse = false;
    panningWithMouse = false;

    qsrand(QTime::currentTime().msec());

    speedSlider                 = new QSlider( Qt::Horizontal, this );
    createStandardShapeButton   = new QPushButton( QIcon( ":/Resources/standardShape.png" ), "",    this );
    createRandomShapeButton     = new QPushButton( QIcon( ":/Resources/randomShape.png" ), "",      this );
    createCircleButton          = new QPushButton( QIcon( ":/Resources/circleShape.png" ), "",      this );
    createRagdollButton         = new QPushButton( QIcon( ":/Resources/ragdoll.png" ), "",          this );
    zoomInButton                = new QPushButton( QIcon( ":/Resources/zoomIn.png" ), "",           this );
    zoomOutButton               = new QPushButton( QIcon( ":/Resources/zoomOut.png" ), "",          this );
    resetButton                 = new QPushButton( QIcon( ":/Resources/restart.png" ), "",          this );
    finishEdgeButton            = new QPushButton( QIcon( ":/Resources/done.png" ), "",             this );
    edgeButton                  = new QPushButton( QIcon( ":/Resources/edge.png" ), "",             this );
    cancelEdgeButton            = new QPushButton( QIcon( ":/Resources/cancel.png" ), "",           this );
    staticShapesButton          = new QPushButton( QIcon( ":/Resources/lock.png" ), "",             this );
    pauseButton                 = new QPushButton( QIcon( ":/Resources/pause.png" ), "",            this );

    speedSlider->setFixedWidth( 200 );
    speedSlider->setRange( 1, 100 );
    speedSlider->setValue( 100 );

    createStandardShapeButton->setFixedSize( buttonDim, buttonDim );
    createRandomShapeButton->setFixedSize( buttonDim, buttonDim );
    createCircleButton->setFixedSize( buttonDim, buttonDim );
    createRagdollButton->setFixedSize( buttonDim, buttonDim );
    zoomInButton->setFixedSize( buttonDim, buttonDim );
    zoomOutButton->setFixedSize( buttonDim, buttonDim );
    resetButton->setFixedSize( buttonDim, buttonDim );
    finishEdgeButton->setFixedSize( buttonDim, buttonDim );
    edgeButton->setFixedSize( buttonDim, buttonDim );
    cancelEdgeButton->setFixedSize( buttonDim, buttonDim );
    staticShapesButton->setFixedSize( buttonDim, buttonDim );
    pauseButton->setFixedSize( buttonDim, buttonDim );

    createStandardShapeButton->setFlat( true );
    createRandomShapeButton->setFlat( true );
    createCircleButton->setFlat( true );
    createRagdollButton->setFlat( true );
    zoomInButton->setFlat( true );
    zoomOutButton->setFlat( true );
    resetButton->setFlat( true );
    finishEdgeButton->setFlat( true );
    edgeButton->setFlat( true );
    cancelEdgeButton->setFlat( true );
    staticShapesButton->setFlat( true );
    pauseButton->setFlat( true );

    createStandardShapeButton->setCheckable( true );
    createRandomShapeButton->setCheckable( true );
    createCircleButton->setCheckable( true );
    createRagdollButton->setCheckable( true );
    edgeButton->setCheckable( true );
    staticShapesButton->setCheckable( true );
    pauseButton->setCheckable( true );

    cancelEdgeButton->setEnabled( false );
    finishEdgeButton->setEnabled( false );

    createStandardShapeButton->setIconSize( QSize( buttonDim, buttonDim ) );
    createRandomShapeButton->setIconSize( QSize( buttonDim, buttonDim ) );
    createCircleButton->setIconSize( QSize( buttonDim, buttonDim ) );
    createRagdollButton->setIconSize( QSize( buttonDim, buttonDim ) );
    zoomInButton->setIconSize( QSize( buttonDim, buttonDim ) );
    zoomOutButton->setIconSize( QSize( buttonDim, buttonDim ) );
    resetButton->setIconSize( QSize( buttonDim, buttonDim ) );
    finishEdgeButton->setIconSize( QSize( buttonDim, buttonDim ) );
    edgeButton->setIconSize( QSize( buttonDim, buttonDim ) );
    cancelEdgeButton->setIconSize( QSize( buttonDim, buttonDim ) );
    staticShapesButton->setIconSize( QSize( buttonDim, buttonDim ) );
    pauseButton->setIconSize( QSize( buttonDim, buttonDim ) );

    cancelEdgeButton->setVisible( false );
    finishEdgeButton->setVisible( false );

    connect( createStandardShapeButton, &QPushButton::toggled, this, &Box2DSimulationWidget::ControlButtonToggled );
    connect( createRandomShapeButton, &QPushButton::toggled, this, &Box2DSimulationWidget::ControlButtonToggled );
    connect( createCircleButton, &QPushButton::toggled, this, &Box2DSimulationWidget::ControlButtonToggled );
    connect( createRagdollButton, &QPushButton::toggled, this, &Box2DSimulationWidget::ControlButtonToggled );
    connect( zoomInButton, &QPushButton::clicked, this, &Box2DSimulationWidget::ZoomInPressed );
    connect( zoomOutButton, &QPushButton::clicked, this, &Box2DSimulationWidget::ZoomOutPressed );
    connect( resetButton, &QPushButton::clicked, this, &Box2DSimulationWidget::RestartPressed );
    connect( finishEdgeButton, &QPushButton::clicked, this, &Box2DSimulationWidget::FinishEdgePressed );
    connect( cancelEdgeButton, &QPushButton::clicked, this, &Box2DSimulationWidget::CancelEdgePressed );
    connect( edgeButton, &QPushButton::toggled, this, &Box2DSimulationWidget::CreateEdgeToggled );
    connect( pauseButton, &QPushButton::toggled, this, &Box2DSimulationWidget::PauseToggled );
    connect( speedSlider, &QSlider::valueChanged, this, &Box2DSimulationWidget::SpeedSliderValueChanged );

    InitialiseWorld();
    RepositionButtons();

    grabGesture( Qt::PinchGesture );
}

//------------------------------------------------------------------------------
Box2DSimulationWidget::~Box2DSimulationWidget()
{
    CleanUpWorld();
}

//------------------------------------------------------------------------------
void Box2DSimulationWidget::slot_TogglePause()
{
    SetPause( simulationTimer->isActive() );
}

//------------------------------------------------------------------------------
void Box2DSimulationWidget::Render()
{
    update();
}

//------------------------------------------------------------------------------
void Box2DSimulationWidget::SimulateStep()
{
    if( !world )
        return;

    world->Step( worldTimeStep * speedMultiplier, velocityIterations, positionIterations );

    stepCount++;
}

//------------------------------------------------------------------------------
void Box2DSimulationWidget::CreatePolygonAtMouse( bool dynamic )
{
    QPointF worldPos;
    ConvertWidgetCoordToWorldCoord( currentMousePos, worldPos );
    b2Vec2 b2WorldPos( worldPos.x(), worldPos.y() );

    int vertexCount = GetRandomInt( 3, b2_maxPolygonVertices );
    float radius = GetRandomFloat( 1.0f, 10.0f );

    if( dynamic )
        CreateDynamicPolygon( b2WorldPos, radius, 0.0f, 1.0f, 1.3f, 0.4f, vertexCount );
    else
        CreateStaticPolygon( b2WorldPos, radius, 0.0f, vertexCount );
}

//------------------------------------------------------------------------------
void Box2DSimulationWidget::CreateRandomPolygonAtMouse( bool dynamic )
{
    QPointF worldPos;
    ConvertWidgetCoordToWorldCoord( currentMousePos, worldPos );
    b2Vec2 b2WorldPos( worldPos.x(), worldPos.y() );

    int vertexCount = GetRandomInt( 3, b2_maxPolygonVertices );

    float radius = 2.0f + (GetRandomFloat(0.0f, 6.0f)); // Note: Careful not to make concave shapes. Box2D does not support this
    float minRad = radius;
    float maxRad = radius + 2.0f;

    if( dynamic )
        CreateDynamicPolygon( b2WorldPos, minRad, maxRad, 0.0f, 1.0f, 0.3f, 0.4f, vertexCount );
    else
        CreateStaticPolygon( b2WorldPos, minRad, maxRad, 0.0f, vertexCount );
}

//------------------------------------------------------------------------------
void Box2DSimulationWidget::CreateCircleAtMouse( bool dynamic )
{
    QPointF worldPos;
    ConvertWidgetCoordToWorldCoord( currentMousePos, worldPos );
    b2Vec2 b2WorldPos( worldPos.x(), worldPos.y() );

    float radius = GetRandomFloat( 1.0f, 10.0f );

    if( dynamic )
        CreateDynamicCircle( b2WorldPos, radius, 0.0f, 1.0f, 0.3f, 0.4f );
    else
        CreateStaticCircle( b2WorldPos, radius );
}

//------------------------------------------------------------------------------
void Box2DSimulationWidget::CreateRagDollAtMouse()
{
    QPointF worldPos;
    ConvertWidgetCoordToWorldCoord( currentMousePos, worldPos );
    b2Vec2 b2WorldPos( worldPos.x(), worldPos.y() );

    CreateRagDoll( b2WorldPos, 0.0f );
}

bool Box2DSimulationWidget::event(QEvent* event)
{
    if( event->type() == QEvent::Gesture )
    {
        QGestureEvent* gestureEvent = static_cast<QGestureEvent*>( event );
        QGesture* gesture = gestureEvent->gesture( Qt::PinchGesture );
        if( gesture )
        {
            QPinchGesture* pinchGesture = static_cast<QPinchGesture*>( gesture );
            double deltaScale = ( pinchGesture->scaleFactor() - 1.0 ) * ( maxCamScale - minCamScale );

            qDebug() << QString( "scaleFactor: %1, deltaScale: %2" ).arg( 1.0 - pinchGesture->scaleFactor() ).arg( deltaScale );

            camScale += deltaScale;

            if( camScale < minCamScale )
                camScale = minCamScale;

            if( camScale > maxCamScale )
                camScale = maxCamScale;

            //camScale = minCamScale + ( pinchGesture->scaleFactor() * ( maxCamScale - minCamScale ) );
        }
    }

    return QWidget::event(event);
}

//------------------------------------------------------------------------------
void Box2DSimulationWidget::wheelEvent(QWheelEvent *event)
{
    event->accept();

    int numDegrees = event->delta() / 8;
    int maxDelta = height() / 2;
    int actualDelta = numDegrees / 2;

    float percent = (float)actualDelta / (float)maxDelta;
    float scaleRange = maxCamScale - minCamScale;
    float scaleDelta = scaleRange * percent;

    camScale += scaleDelta;

    if( camScale < minCamScale )
     camScale = minCamScale;
    if( camScale > maxCamScale )
        camScale = maxCamScale;
}

//------------------------------------------------------------------------------
void Box2DSimulationWidget::mouseMoveEvent(QMouseEvent *event)
{
    event->accept();

    currentMousePos = event->pos();

    if( mouseManipulationBody )
    {
        QPointF screenPos( event->pos().x(), event->pos().y() );
        QPointF worldPos;
        ConvertWidgetCoordToWorldCoord(screenPos, worldPos);
        b2Vec2 pos;
        pos.Set( worldPos.x(), worldPos.y() );
        mouseManipulationBody->SetTransform( pos, mouseManipulationBody->GetAngle() );
        mouseManipulationJoint->SetTarget( mouseManipulationBody->GetPosition() );
    }
    else if( scrollingWithMouse )
    {
        int maxDelta = height() / 2;
        int delta = currentMouseGlobalPos.y() - event->globalY();
        float percent = (float)delta / (float)maxDelta;
        float scaleRange = maxCamScale - minCamScale;
        float scaleDelta = scaleRange * percent;

        camScale += scaleDelta;
        if( camScale < minCamScale )
            camScale = minCamScale;
        if( camScale > maxCamScale )
            camScale = maxCamScale;

        currentMouseGlobalPos = event->globalPos();
    }
    else if( panningWithMouse )
    {
        int xDelta = event->globalX() - currentMouseGlobalPos.x();
        int yDelta = event->globalY() - currentMouseGlobalPos.y();

        camPos.setX( camPos.x() - (xDelta / camScale) );
        camPos.setY( camPos.y() + (yDelta / camScale) );

        currentMouseGlobalPos = event->globalPos();
    }
}

//------------------------------------------------------------------------------
void Box2DSimulationWidget::mouseReleaseEvent(QMouseEvent *event)
{
    event->accept();

    scrollingWithMouse = false;
    panningWithMouse = false;
    if( mouseManipulationBody )
    {
        world->DestroyBody( mouseManipulationBody );
        mouseManipulationBody = NULL;
        mouseManipulationJoint = NULL;
    }
}

//------------------------------------------------------------------------------
void Box2DSimulationWidget::mousePressEvent(QMouseEvent *event)
{
    setFocus();
    event->accept();

    currentMousePos = event->pos();

    switch( event->button() )
    {
        case Qt::LeftButton:
        {
            QPointF screenPos( event->pos().x(), event->pos().y() );
            QPointF worldPos;
            ConvertWidgetCoordToWorldCoord(screenPos, worldPos);
            b2Vec2 pos;
            pos.Set( worldPos.x(), worldPos.y() );
            selectedBody = GetBodyAtPos( world, pos );

            if( selectedBody )
            {
                mouseManipulationBody = CreateStaticCircleSensor( pos, 0.2f );
                b2MouseJointDef def;
                def.bodyA = mouseManipulationBody;
                def.bodyB = selectedBody;
                def.maxForce = 1000.0f * selectedBody->GetMass();
                def.target = mouseManipulationBody->GetPosition();

                mouseManipulationJoint = (b2MouseJoint*)world->CreateJoint( &def );
            }

            else if( creatingChain )
            {
                chainPoints.append(worldPos);
                if( chainPoints.size() > 1 )
                    finishEdgeButton->setEnabled( true );
            }

            else if( createStandardShapeButton->isChecked() )
            {
                CreatePolygonAtMouse(!staticShapesButton->isChecked());
            }

            else if( createRandomShapeButton->isChecked() )
            {
                CreateRandomPolygonAtMouse(!staticShapesButton->isChecked());
            }

            else if( createCircleButton->isChecked() )
            {
                CreateCircleAtMouse(!staticShapesButton->isChecked());
            }

            else if( createRagdollButton->isChecked() )
            {
                CreateRagDollAtMouse();
            }

            else
            {
                currentMouseGlobalPos = event->globalPos();
                panningWithMouse = true;
            }
        }
    }
}

//------------------------------------------------------------------------------
void Box2DSimulationWidget::keyPressEvent(QKeyEvent *event)
{
    event->accept();

    bool controlPressed = event->modifiers() &= Qt::ControlModifier;

    switch(event->key())
    {
        case Qt::Key_Escape:
        {
            exit(0);
        }
        case Qt::Key_Pause:
        case Qt::Key_P:
        {
            slot_TogglePause();
            break;
        }
        case Qt::Key_1:
        {
            CreatePolygonAtMouse(!controlPressed);
            break;
        }
        case Qt::Key_2:
        {
            CreateRandomPolygonAtMouse(!controlPressed);
            break;
        }
        case Qt::Key_3:
        {
            CreateCircleAtMouse(!controlPressed);
            break;
        }
        case Qt::Key_4:
        {
            CreateRagDollAtMouse();
            break;
        }
        case Qt::Key_R:
        {
            CleanUpWorld();
            InitialiseWorld();
            camPos = QPointF(0.0f, 0.0f);
            break;
        }
        case Qt::Key_Left:
        {
//            if( selectedBody )
//            {
//                selectedBody->ApplyTorque( 4000000 );
//            }
            break;
        }
        case Qt::Key_Right:
        {
//            if( selectedBody )
//            {
//                selectedBody->ApplyTorque( -4000000 );
//            }
            break;
        }
        case Qt::Key_Up:
        {
//            if( selectedBody && !isAutoRepeat )
//            {
//                b2Vec2 impulse(0.0f, 400.0f);
//                b2Vec2 point(0.0f, 0.0f);
//                selectedBody->ApplyLinearImpulse( impulse, point );
//            }
            break;
        }
    }
}

//------------------------------------------------------------------------------
void Box2DSimulationWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)

    widgetCenterPos = QPointF( width() / 2.0, height() / 2.0 );
    RepositionButtons();
}

//------------------------------------------------------------------------------
void Box2DSimulationWidget::paintEvent(QPaintEvent *event)
{
    static QElapsedTimer renderTimer;
    static qint64 renderCount = 0;
    static qint64 elapsedMs = 0;
    static bool firstTimeRun = true;

    if( firstTimeRun )
    {
        renderTimer.start();
        firstTimeRun = false;
    }

    renderCount++;
    elapsedMs = renderTimer.elapsed();
    double fps = static_cast<double>( renderCount ) / ( static_cast<double>( elapsedMs ) / 1000.0 );

    if( elapsedMs >= 1000 ) {

        renderTimer.restart();
        elapsedMs = 0;
        renderCount = 0;
    }


    Q_UNUSED(event)

    QPainter painter(this);
    QPainter* p = &painter;

    p->save();

    p->setBrush(Qt::white);
    p->drawRect( rect() );

    if( world )
    {
        QPointF axisPos = QPointF( 0.0, 0.0 );
        QSizeF axisSize = QSize( 10.0, 10.0 );
        qreal axisLineWidth = 1.0;
        QColor colour = Qt::red;

        World_PaintCross(p, axisPos, axisSize, colour, axisLineWidth );

        if( creatingChain )
        {
            for( int i = 0; i < chainPoints.size(); i++ )
            {
                World_PaintCross( p, chainPoints.at(i), axisSize, Qt::blue, 1 );
            }
        }

        PaintBodyWireframes(p, world, QColor(0,0,255,150), 1, Qt::black );
        if( mouseManipulationBody )
        {
            PaintBodyWireframe(p, mouseManipulationBody, Qt::blue, 1, Qt::black );

            b2Vec2 b2pos1, b2pos2;
            QPointF pos1, pos2;

            b2pos1 = mouseManipulationBody->GetPosition();
            b2pos2 = selectedBody->GetPosition();

            ConvertWorldCoordToWidgetCoord( b2pos1, pos1 );
            ConvertWorldCoordToWidgetCoord( b2pos2, pos2 );

            p->save();
            p->setPen( Qt::red );
            p->drawLine( pos1, pos2 );
            p->restore();
        }

        if( camScale >= 5.0 )
            PaintJoints( p, world, Qt::green );
    }

    // Draw FPS
    p->setPen( Qt::red );
    p->drawText( rect(), Qt::AlignLeft | Qt::AlignTop, QString( "\n     %1 fps (%2 physics bodies)" ).arg( qRound( fps ) ).arg( world->GetBodyCount() ) );

    p->restore();
}

//------------------------------------------------------------------------------
void Box2DSimulationWidget::InitialiseWorld()
{
    // Initialise the world
    stepCount = 0;
    b2Vec2 gravity( 0.0f, -10.0f );
    world = new b2World( gravity );

    CreateStaticEdge( b2Vec2(-1000.0f, 0.0f), b2Vec2(1000.0f, 0.0f) );

    // Start the timer to progress the simulation
    simulationTimer->start();
}

//------------------------------------------------------------------------------
void Box2DSimulationWidget::CleanUpWorld()
{
    selectedBody = NULL;

    if( creatingChain )
        creatingChain = false;
    chainPoints.clear();

    if( simulationTimer->isActive() )
        simulationTimer->stop();

    if( world )
    {
        delete world;
        world = NULL;
    }

    mouseManipulationBody = NULL;
    mouseManipulationJoint = NULL;
}

//------------------------------------------------------------------------------
void Box2DSimulationWidget::SetPause( bool pause )
{
    if( pause )
    {
        if( simulationTimer->isActive() )
            simulationTimer->stop();
    }
    else
    {
        if( !simulationTimer->isActive() && world )
            simulationTimer->start();
    }
}

//------------------------------------------------------------------------------
b2Body* Box2DSimulationWidget::CreateStaticCircleSensor( b2Vec2 pos, float radius )
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
b2Body* Box2DSimulationWidget::CreateStaticCircle( b2Vec2 pos, float radius )
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
b2Body* Box2DSimulationWidget::CreateStaticRectangle( b2Vec2 pos, b2Vec2 size, float angle )
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
b2Body* Box2DSimulationWidget::CreateStaticPolygon( b2Vec2 pos, float radius, float angle, int32 vertexCount )
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.angle = angle;
    bodyDef.position = pos;
    b2Body* body = world->CreateBody( &bodyDef );
    b2PolygonShape shape;

    b2Vec2* vectors = new b2Vec2[vertexCount];

    float angleIncrement = TWO_PIE / vertexCount;
    float cumulativeAngle = 0.0f;
    for( int i = 0; i < vertexCount; i++ )
    {
        vectors[i] = GetCoordForAngleAroundPoint( 0.0f, 0.0f, radius, cumulativeAngle );
        cumulativeAngle += angleIncrement;
    }

    shape.Set( vectors, vertexCount );
    delete [] vectors;

    body->CreateFixture( &shape, 0.0f );

    return body;
}

//------------------------------------------------------------------------------
b2Body* Box2DSimulationWidget::CreateStaticPolygon( b2Vec2 pos, float radiusMin, float radiusMax, float angle, int32 vertexCount )
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
        radius = GetRandomFloat( radiusMin, radiusMax );
        vectors[i] = GetCoordForAngleAroundPoint( 0.0f, 0.0f, radius, cumulativeAngle );
        cumulativeAngle += angleIncrement;
    }

    shape.Set( vectors, vertexCount );
    delete [] vectors;

    body->CreateFixture( &shape, 0.0f );

    return body;
}

//------------------------------------------------------------------------------
b2Body* Box2DSimulationWidget::CreateStaticEdge( b2Vec2 pos1, b2Vec2 pos2 )
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
b2Body* Box2DSimulationWidget::CreateStaticChain( QList<b2Vec2> posList )
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
b2Body* Box2DSimulationWidget::CreateDynamicCircle( b2Vec2 pos, float radius, float angle, float density, float friction, float restitution )
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set( pos.x, pos.y );
    bodyDef.angle = angle;
    b2Body* body = world->CreateBody( &bodyDef );

    b2CircleShape shape;
    shape.m_p.Set( 1.0f, 0.0f);
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
b2Body* Box2DSimulationWidget::CreateDynamicRectangle( b2Vec2 pos, b2Vec2 size, float angle, float density, float friction, float restitution )
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
b2Body* Box2DSimulationWidget::CreateDynamicPolygon( b2Vec2 pos, float radius, float angle, float density, float friction, float restitution, int32 vertexCount )
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
        vectors[i] = GetCoordForAngleAroundPoint( 0.0f, 0.0f, radius, cumulativeAngle );
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
b2Body* Box2DSimulationWidget::CreateDynamicPolygon( b2Vec2 pos, float radiusMin, float radiusMax, float angle, float density, float friction, float restitution, int32 vertexCount )
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
        radius = GetRandomFloat( radiusMin, radiusMax );
        vectors[i] = GetCoordForAngleAroundPoint( 0.0f, 0.0f, radius, cumulativeAngle );
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
b2Body* Box2DSimulationWidget::CreateDynamicBody( b2Vec2 centerPos, float angle, QList<ShapeDef>& shapeDefinitions )
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
void Box2DSimulationWidget::CreateRagDoll( b2Vec2 pos, float angle )
{
    Q_UNUSED( angle );

    // Test function

    b2Body* head = CreateDynamicCircle( pos, 2.0f, 0.0f, 1.0f, 0.3f, 0.4f );
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

//------------------------------------------------------------------------------
b2Vec2 Box2DSimulationWidget::GetCoordForAngleAroundPoint( float centerX, float centerY, float radius, float angle )
{
    float x, y;

    if( angle >= 0.0 && angle < PIE_OVER_TWO )
    {
        x = qSin(angle) * radius;
        y = -qCos(angle) * radius;
    }
    else if( angle >= PIE_OVER_TWO && angle < PIE )
    {
        x = qCos(angle - PIE_OVER_TWO) * radius;
        y = qSin(angle - PIE_OVER_TWO) * radius;
    }
    else if( angle >= PIE && angle < (3 * PIE_OVER_TWO) )
    {
        x = -qSin(angle - (PIE)) * radius;
        y = qCos(angle - (PIE)) * radius;
    }
    else
    {
        x = -qCos(angle - (3 * PIE_OVER_TWO)) * radius;
        y = -qSin(angle - (3 * PIE_OVER_TWO)) * radius;
    }

    x += centerX;
    y += centerY;

    return b2Vec2( x, y );
}

//------------------------------------------------------------------------------
b2Vec2 Box2DSimulationWidget::GetHalfWayPointBetweenPoints( b2Vec2& p1, b2Vec2 p2 )
{
    float x, y;

    x = p1.x + (p2.x - p1.x) / 2.0f;
    y = p1.y + (p2.y - p1.y) / 2.0f;

    return b2Vec2( x, y );
}

//------------------------------------------------------------------------------
b2Body* Box2DSimulationWidget::GetBodyAtPos( b2World* world, b2Vec2 pos )
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
    return NULL;
}

//------------------------------------------------------------------------------
void Box2DSimulationWidget::ConvertWorldDistanceToPixels( float &in_distance, float &out_pixelDistance )
{
    QPointF worldPosA( 0.0, 0.0 );
    QPointF worldPosB( 1.0, 0.0 );
    QPointF widgetPosA;
    QPointF widgetPosB;

    ConvertWorldCoordToWidgetCoord( worldPosA, widgetPosA );
    ConvertWorldCoordToWidgetCoord( worldPosB, widgetPosB );

    out_pixelDistance = in_distance * qAbs( widgetPosB.x() - widgetPosA.x() );
}

//------------------------------------------------------------------------------
void Box2DSimulationWidget::ConvertWorldSizeToPixels( QSizeF& in_size, QSizeF& out_size )
{
    float worldWidth, worldHeight, widgetWidth, widgetHeight;
    worldWidth = in_size.width();
    worldHeight = in_size.height();
    ConvertWorldDistanceToPixels( worldWidth, widgetWidth );
    ConvertWorldDistanceToPixels( worldHeight, widgetHeight);
    out_size.setWidth( widgetWidth );
    out_size.setHeight( widgetHeight);
}

//------------------------------------------------------------------------------
void Box2DSimulationWidget::ConvertWorldCoordToWidgetCoord( QPointF& in_worldCoord, QPointF& out_widgetCoord )
{
    qreal deltaX = in_worldCoord.x() - camPos.x();
    qreal deltaY = in_worldCoord.y() - camPos.y();

    out_widgetCoord.setX( widgetCenterPos.x() + (deltaX * camScale) );
    out_widgetCoord.setY( widgetCenterPos.y() + (-deltaY * camScale) );
}

//------------------------------------------------------------------------------
void Box2DSimulationWidget::ConvertWorldCoordToWidgetCoord( b2Vec2& in_worldCoord, QPointF& out_widgetCoord )
{
    QPointF worldCoord( in_worldCoord.x, in_worldCoord.y );
    ConvertWorldCoordToWidgetCoord( worldCoord, out_widgetCoord );
}

//------------------------------------------------------------------------------
void Box2DSimulationWidget::ConvertWidgetCoordToWorldCoord( QPointF& in_widgetCoord, QPointF& out_worldCoord )
{
    qreal deltaX = (in_widgetCoord.x() - widgetCenterPos.x()) / camScale;
    qreal deltaY = (in_widgetCoord.y() - widgetCenterPos.y()) / camScale;

    out_worldCoord.setX( deltaX + camPos.x() );
    out_worldCoord.setY( -deltaY + camPos.y() );
}

//------------------------------------------------------------------------------
void Box2DSimulationWidget::PaintBodyWireframes( QPainter* p, b2World *world, QColor fillColour, int borderWidth, QColor borderColour )
{
    for( b2Body* body = world->GetBodyList(); body; body = body->GetNext() )
        PaintBodyWireframe( p, body, fillColour, borderWidth, borderColour );
}

//------------------------------------------------------------------------------
void Box2DSimulationWidget::PaintJoints( QPainter* p, b2World* world, QColor colour )
{
    for( b2Joint* joint = world->GetJointList(); joint; joint = joint->GetNext() )
        Paintjoint( p, joint, colour );
}

//------------------------------------------------------------------------------
void Box2DSimulationWidget::PaintBodyWireframe( QPainter* p, b2Body* body, QColor fillColour, int borderWidth, QColor borderColour )
{
    static QPen pen;
    p->save();

    float angle = RADIANS_TO_DEGREES(body->GetAngle());
    QPointF widgetPos;
    b2Vec2 pos = body->GetPosition();
    ConvertWorldCoordToWidgetCoord( pos, widgetPos );

    /*
        A Quick note on rotations
            1. Save the painter state (using p->save()).
            2. Translate so object pos is at 0,0 (using p->translate()).
            3. Rotate to desired angle (using p->rotate())...
                    - don't forget box 2D rotates the inverse of Qt.
                    - also don't forget p->rotate() uses degrees, not radians
            4. Translate so objects center is at its original pos.
            5. Paint your object.
            6. Restore the original painter state (using p->restore()).
     */

    p->translate( widgetPos );
    p->rotate( -angle );
    p->translate( -widgetPos.x(), -widgetPos.y() );

    p->setBrush( fillColour );
    if( body == selectedBody )
        pen.setWidth(borderWidth * 4);
    else
        pen.setWidth(borderWidth);
    pen.setColor(borderColour);

    p->setPen( pen );

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

                b2Vec2 worldCenter( pos.x + circle->m_p.x, pos.y + circle->m_p.y );
                b2Vec2 worldLineEnd( pos.x + circle->m_p.x, pos.y + circle->m_p.y + circle->m_radius );
                QPointF widgetCenter, widgetLineEnd;
                ConvertWorldCoordToWidgetCoord( worldCenter, widgetCenter );
                ConvertWorldCoordToWidgetCoord( worldLineEnd, widgetLineEnd);

                p->drawEllipse( widgetCenter, radius, radius );
                p->drawLine( widgetCenter, widgetLineEnd );

                break;
            }
            case b2Shape::e_edge:
            {
                b2EdgeShape* edge = (b2EdgeShape*)shape;

                // Note:    vertices 0 and 3 are ghost vertices used internally to prevent ghost collisions
                //          which occurs when a body moves from one edge to another, causing a slight body
                //          movement even if those edges are perfectly aligned.
                //          Vertices 1 and 2 are the edge describing vertices.
                QPointF pos1;
                QPointF pos2;

                ConvertWorldCoordToWidgetCoord( edge->m_vertex1, pos1 );
                ConvertWorldCoordToWidgetCoord( edge->m_vertex2, pos2 );
                p->drawLine( pos1, pos2 );

                break;
            }
            case b2Shape::e_polygon:
            {
                b2PolygonShape* polygon = (b2PolygonShape*)fixture->GetShape();

                QPointF worldPos;
                QPointF widgetPos;
                b2Vec2 b2Pos;

                // Note:    The polygon winding order for Qt is the opposite to box2Ds so the vertices must be
                //          listed backwards.

                QPointF* points = new QPointF[polygon->GetVertexCount()];
                for(int i = polygon->GetVertexCount() - 1; i >= 0; i-- )
                {
                    b2Pos = polygon->GetVertex( i );

                    worldPos.setX( pos.x + b2Pos.x );
                    worldPos.setY( pos.y + b2Pos.y );

                    ConvertWorldCoordToWidgetCoord( worldPos, widgetPos );

                    points[i] = QPointF( widgetPos.x(), widgetPos.y() );
                }

                p->drawPolygon(points, polygon->GetVertexCount() );
                delete [] points;
                break;
            }
            case b2Shape::e_chain:
            {
                b2ChainShape* chain = (b2ChainShape*)shape;
                QPointF pos1;
                QPointF pos2;
                for( int32 i = 0; i < chain->GetChildCount(); ++i )
                {
                    b2EdgeShape edge;
                    chain->GetChildEdge(&edge, i);

                    ConvertWorldCoordToWidgetCoord( edge.m_vertex1, pos1 );
                    ConvertWorldCoordToWidgetCoord( edge.m_vertex2, pos2 );
                    p->drawLine( pos1, pos2 );
                }

                break;
            }

            default: {}
        }
    }

    p->restore();
}

//------------------------------------------------------------------------------
void Box2DSimulationWidget::Paintjoint( QPainter* p, b2Joint* joint, QColor colour )
{
    p->save();

    switch( joint->GetType() )
    {
        case e_distanceJoint:
        {
            //b2DistanceJoint* j = (b2DistanceJoint*)joint;
            // TODO
            break;
        }
        case e_revoluteJoint:
        {
            b2RevoluteJoint* j = (b2RevoluteJoint*)joint;
            b2Vec2 anchorPos = j->GetAnchorA();
            QPointF wPos;
            ConvertWorldCoordToWidgetCoord( anchorPos, wPos );
            Widget_PaintCross( p, wPos, QSize(10, 10), colour, 1.0f );
            break;
        }
        case e_prismaticJoint:
        {
            // TODO
            //b2PrismaticJoint* j = (b2PrismaticJoint*)joint;
            break;
        }
        case e_pulleyJoint:
        {
            // TODO
            //b2PulleyJoint* j = (b2PulleyJoint*)joint;
            break;
        }
        case e_gearJoint:
        {
            // TODO
            //b2GearJoint* j = (b2GearJoint*)joint;
            break;
        }
        case e_mouseJoint:
        {
            b2MouseJoint* j = (b2MouseJoint*)joint;
            b2Vec2 worldPos = j->GetTarget();
            QPointF wPos;
            ConvertWorldCoordToWidgetCoord( worldPos, wPos );
            Widget_PaintCross( p, wPos, QSize(10, 10), colour, 1.0f );
            break;
        }
        case e_wheelJoint:
        {
            // TODO
            //b2WheelJoint* j = (b2WheelJoint*)joint;
            break;
        }
        case e_weldJoint:
        {
            // TODO
            //b2WeldJoint* j = (b2WeldJoint*)joint;
            break;
        }
        case e_ropeJoint:
        {
            // TODO
            //b2RopeJoint* j = (b2RopeJoint*)joint;
            break;
        }
        case e_frictionJoint:
        {
            // TODO
            //b2FrictionJoint* j = (b2FrictionJoint*)joint;
            break;
        }
        default: {}
    }

    p->restore();
}

//------------------------------------------------------------------------------
void Box2DSimulationWidget::World_PaintCross( QPainter* p, QPointF pos, QSizeF size, QColor colour, qreal lineWidth )
{
    p->save();

    QPointF widgetPos;
    ConvertWorldCoordToWidgetCoord( pos, widgetPos );

    QSizeF widgetSize;
    ConvertWorldSizeToPixels( size, widgetSize );

    Widget_PaintCross( p, widgetPos, widgetSize, colour, lineWidth );

    p->restore();
}

//------------------------------------------------------------------------------
void Box2DSimulationWidget::Widget_PaintCross( QPainter* p, QPointF pos, QSizeF size, QColor colour, qreal lineWidth )
{
    static QPen pen;

    p->save();

    pen.setWidth( lineWidth );
    pen.setColor( colour );
    p->setPen( pen );

    p->drawLine( pos.x() - size.width() / 2, pos.y(), pos.x() + size.width() / 2, pos.y() );
    p->drawLine( pos.x(), pos.y() - size.height() / 2, pos.x(), pos.y() + size.height() / 2 );

    p->restore();
}

//------------------------------------------------------------------------------
float Box2DSimulationWidget::GetRandomFloat( float min, float max )
{
    return (min + (float)qrand()/((float)RAND_MAX/(max - min)));
}

//------------------------------------------------------------------------------
int Box2DSimulationWidget::GetRandomInt( int min, int max )
{
    return (min + (qrand() % (max - min)));
}

void Box2DSimulationWidget::RepositionButtons()
{
    int startX = width() - buttonDim - 10;
    int startY = 10;

    speedSlider->setFixedWidth( 0.3 * width() );

    speedSlider->move(                  0.5 * width() - 0.5 * speedSlider->width(), height() - 20 - speedSlider->height() );

    pauseButton->move(                  startX - buttonDim - 10, startY + 0 * ( buttonDim + 10 ) );
    cancelEdgeButton->move(             startX - buttonDim - 10, startY + 1 * ( buttonDim + 10 ) );
    finishEdgeButton->move(             startX - buttonDim - 10, startY + 2 * ( buttonDim + 10 ) );

    resetButton->move(                  startX, startY + 0 * ( buttonDim + 10 ) );
    zoomInButton->move(                 startX, startY + 1 * ( buttonDim + 10 ) );
    zoomOutButton->move(                startX, startY + 2 * ( buttonDim + 10 ) );
    staticShapesButton->move(           startX, startY + 3 * ( buttonDim + 10 ) );
    createStandardShapeButton->move(    startX, startY + 4 * ( buttonDim + 10 ) );
    createRandomShapeButton->move(      startX, startY + 5 * ( buttonDim + 10 ) );
    createCircleButton->move(           startX, startY + 6 * ( buttonDim + 10 ) );
    createRagdollButton->move(          startX, startY + 7 * ( buttonDim + 10 ) );
    edgeButton->move(                   startX, startY + 8 * ( buttonDim + 10 ) );
}

void Box2DSimulationWidget::ControlButtonToggled( bool enabled )
{
    if( !enabled )
        return;

    QPushButton* button = qobject_cast<QPushButton*>( QObject::sender() );
    if( !button )
        return;

    if( button == createStandardShapeButton )
    {
        createRandomShapeButton->setChecked( false );
        createCircleButton->setChecked( false );
        createRagdollButton->setChecked( false );
        edgeButton->setChecked( false );
    }

    else if( button == createRandomShapeButton )
    {
        createStandardShapeButton->setChecked( false );
        createCircleButton->setChecked( false );
        createRagdollButton->setChecked( false );
        edgeButton->setChecked( false );
    }

    else if( button == createCircleButton )
    {
        createStandardShapeButton->setChecked( false );
        createRandomShapeButton->setChecked( false );
        createRagdollButton->setChecked( false );
        edgeButton->setChecked( false );
    }

    else if( button == createRagdollButton )
    {
        createStandardShapeButton->setChecked( false );
        createRandomShapeButton->setChecked( false );
        createCircleButton->setChecked( false );
        edgeButton->setChecked( false );
    }
}

void Box2DSimulationWidget::ZoomInPressed()
{
    camScale += 1.0;
    if( camScale >= maxCamScale )
        camScale = maxCamScale;
}

void Box2DSimulationWidget::ZoomOutPressed()
{
    camScale -= 1.0;
    if( camScale < minCamScale )
        camScale = minCamScale;
}

void Box2DSimulationWidget::RestartPressed()
{
    CleanUpWorld();
    InitialiseWorld();
    camPos.setX( 0.0 );
    camPos.setY( 0.0 );
    camScale = 5.0;
}

void Box2DSimulationWidget::CreateEdgeToggled( bool enabled )
{
    if( !enabled )
    {
        creatingChain = false;
        chainPoints.clear();

        cancelEdgeButton->setEnabled( false );
        finishEdgeButton->setEnabled( false );
        cancelEdgeButton->setVisible( false );
        finishEdgeButton->setVisible( false );
    }

    else
    {
        creatingChain = true;
        cancelEdgeButton->setEnabled( true );
        finishEdgeButton->setEnabled( false );
        cancelEdgeButton->setVisible( true );
        finishEdgeButton->setVisible( true );

        createStandardShapeButton->setChecked( false );
        createRandomShapeButton->setChecked( false );
        createCircleButton->setChecked( false );
        createRagdollButton->setChecked( false );
    }
}

void Box2DSimulationWidget::FinishEdgePressed()
{
    if( !creatingChain || chainPoints.size() <= 1 )
        return;

    QList<b2Vec2> posList;
    for( int i = 0; i < chainPoints.size(); i++ )
        posList.append( b2Vec2( chainPoints.at(i).x(), chainPoints.at(i).y() ) );

    CreateStaticChain( posList );
    creatingChain = false;
    chainPoints.clear();

    edgeButton->setChecked( false );
    cancelEdgeButton->setEnabled( false );
    finishEdgeButton->setEnabled( false );
    cancelEdgeButton->setVisible( false );
    finishEdgeButton->setVisible( false );
}

void Box2DSimulationWidget::CancelEdgePressed()
{
    creatingChain = false;
    chainPoints.clear();

    edgeButton->setChecked( false );
    cancelEdgeButton->setEnabled( false );
    finishEdgeButton->setEnabled( false );
    cancelEdgeButton->setVisible( false );
    finishEdgeButton->setVisible( false );
}

void Box2DSimulationWidget::PauseToggled( bool enabled )
{
    SetPause( enabled );
}

void Box2DSimulationWidget::SpeedSliderValueChanged(int value)
{
    speedMultiplier = static_cast<double>( value ) / 100.0;
}
