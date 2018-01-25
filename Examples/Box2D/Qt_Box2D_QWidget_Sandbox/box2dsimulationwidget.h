#ifndef BOX2DSIMULATIONWIDGET_H
#define BOX2DSIMULATIONWIDGET_H

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Box2DSimulationWidget
//      A test widget combining Box2D along with some widget rendering.
//
//                                                                  Aaron Wilson
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

#include <QtWidgets>
#include <Box2D/Box2D.h>

#include "common.h"

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
class Box2DSimulationWidget : public QWidget
{
    Q_OBJECT

    //--------------------------------------------------------------------------
    public:
        explicit                Box2DSimulationWidget(QWidget *parent = 0);
                                ~Box2DSimulationWidget();

    //--------------------------------------------------------------------------
    public slots:
        void                    slot_TogglePause();

    //--------------------------------------------------------------------------
    private slots:
        void                    Render();
        void                    SimulateStep();

        void                    CreatePolygonAtMouse( bool dynamic = true );
        void                    CreateRandomPolygonAtMouse( bool dynamic = true );
        void                    CreateCircleAtMouse( bool dynamic = true );
        void                    CreateRagDollAtMouse();

    //--------------------------------------------------------------------------
    private:

        struct ShapeDef
        {
            // This struct is used to describe a shape and the parameters of its fixture to some arbitrary body
            ShapeDef()
            {
                restitution = 0.4f;
                density = 1.0f;
                friction = 0.3f;
                relativeCenter = b2Vec2( 0.0f, 0.0f );
                type = b2Shape::e_circle;
                radius = 1.0f;
            }

            b2Shape::Type type;     // e_circle, e_edge, e_polygon, e_chain
            b2Vec2 relativeCenter;  // The shapes center pos relative to the body
            float restitution;      // 0.0 => no bounce, 1.0 => lots of bounce
            float density;
            float friction;

            // circle only stuff
            float radius;

            // polygon only stuff
            QList<b2Vec2> vertices; // these should be relative to the shapes center, not the body. The offset from the body center will be computed using relativeCenter.
        };

        // Events
        bool                    event(QEvent *event);
        void                    wheelEvent(QWheelEvent *event);
        void                    mouseMoveEvent(QMouseEvent *event);
        void                    mouseReleaseEvent(QMouseEvent *event);
        void                    mousePressEvent(QMouseEvent *event);
        void                    keyPressEvent(QKeyEvent *event);
        void                    resizeEvent(QResizeEvent *event);
        void                    paintEvent(QPaintEvent *event);

        // Physics methods
        void                    InitialiseWorld();
        void                    CleanUpWorld();

        void                    SetPause( bool pause );

        // TODO Kinematic shapes

        //------------------------
        // Static shapes
        //------------------------

            // Single Shapes
            b2Body*                 CreateStaticCircleSensor( b2Vec2 pos, float radius );
            b2Body*                 CreateStaticCircle( b2Vec2 pos, float radius );
            b2Body*                 CreateStaticRectangle( b2Vec2 pos, b2Vec2 size, float angle );
            b2Body*                 CreateStaticPolygon( b2Vec2 pos, float radius, float angle, int32 vertexCount );
            b2Body*                 CreateStaticPolygon( b2Vec2 pos, float radiusMin, float radiusMax, float angle, int32 vertexCount );

            b2Body*                 CreateStaticEdge( b2Vec2 pos1, b2Vec2 pos2 );
            b2Body*                 CreateStaticChain( QList<b2Vec2> posList );

            // TODO Compound Shapes


        //------------------------
        // Dynamic shapes
        //------------------------

            // Single Shapes
            b2Body*                 CreateDynamicCircle( b2Vec2 pos, float radius, float angle, float density, float friction, float restitution );
            b2Body*                 CreateDynamicRectangle( b2Vec2 pos, b2Vec2 size, float angle, float density, float friction, float restitution );
            b2Body*                 CreateDynamicPolygon( b2Vec2 pos, float radius, float angle, float density, float friction, float restitution, int32 vertexCount );
            b2Body*                 CreateDynamicPolygon( b2Vec2 pos, float radiusMin, float radiusMax, float angle, float density, float friction, float restitution, int32 vertexCount );

            // Compound Shapes
            b2Body*                 CreateDynamicBody( b2Vec2 centerPos, float angle, QList<ShapeDef>& shapeDefinitions );
            void                    CreateRagDoll( b2Vec2 pos, float angle );


        // Useful geometry methods
        b2Vec2                  GetCoordForAngleAroundPoint( float centerX, float centerY, float radius, float angle );
        b2Vec2                  GetHalfWayPointBetweenPoints( b2Vec2& p1, b2Vec2 p2 );

        // Coordinate system methods
        b2Body*                 GetBodyAtPos( b2World* world, b2Vec2 pos );
        void                    ConvertWorldDistanceToPixels( float &in_distance, float&out_pixelDistance );
        void                    ConvertWorldSizeToPixels( QSizeF& in_size, QSizeF& out_size );
        void                    ConvertWorldCoordToWidgetCoord( QPointF& in_worldCoord, QPointF& out_widgetCoord );
        void                    ConvertWorldCoordToWidgetCoord( b2Vec2& in_worldCoord, QPointF& out_widgetCoord );
        void                    ConvertWidgetCoordToWorldCoord( QPointF& in_widgetCoord, QPointF& out_worldCoord );

        // Paint methods

            // iterative paint methods
            void                PaintBodyWireframes( QPainter* p, b2World* world, QColor fillColour, int borderWidth, QColor borderColour );
            void                PaintJoints( QPainter* p, b2World* world, QColor colour );

            // Individual paint methods
            void                PaintBodyWireframe( QPainter* p, b2Body* body, QColor fillColour, int borderWidth, QColor borderColour = Qt::black );
            void                Paintjoint( QPainter* p, b2Joint* joint, QColor colour );

        void                    World_PaintCross( QPainter* p, QPointF pos, QSizeF size, QColor colour, qreal lineWidth );
        void                    Widget_PaintCross( QPainter* p, QPointF pos, QSizeF size, QColor colour, qreal lineWidth );

        // Other Utility Methods
        float                   GetRandomFloat( float min, float max );
        int                     GetRandomInt( int min, int max );

        void                    RepositionButtons();
        void                    ControlButtonToggled( bool enabled );
        void                    ZoomInPressed();
        void                    ZoomOutPressed();
        void                    RestartPressed();
        void                    CreateEdgeToggled( bool enabled );
        void                    FinishEdgePressed();
        void                    CancelEdgePressed();
        void                    PauseToggled( bool enabled );
        void                    SpeedSliderValueChanged( int value );


        //----------------------------------------------------------------------
        QPointF                 camPos;             // position of the camera in physics model coords
        float                   camScale;           // effectivly the camera zoom
        float                   minCamScale;
        float                   maxCamScale;
        QPointF                 widgetCenterPos;

        QTimer*                 renderTimer;
        QTimer*                 simulationTimer;
        float                   worldTimeStep;
        int32                   velocityIterations;
        int32                   positionIterations;
        quint64                 stepCount;
        b2World*                world;

        // Some control related items
        b2Body*                 mouseManipulationBody;
        b2MouseJoint*           mouseManipulationJoint;
        b2Body*                 selectedBody;
        QList<QPointF>          chainPoints;
        bool                    creatingChain;
        bool                    scrollingWithMouse;
        bool                    panningWithMouse;
        QPointF                 currentMousePos;
        QPoint                  currentMouseGlobalPos;

        // Buttons
        QPushButton*            createStandardShapeButton;
        QPushButton*            createRandomShapeButton;
        QPushButton*            createCircleButton;
        QPushButton*            createRagdollButton;
        QPushButton*            zoomInButton;
        QPushButton*            zoomOutButton;
        QPushButton*            resetButton;
        QPushButton*            finishEdgeButton;
        QPushButton*            edgeButton;
        QPushButton*            cancelEdgeButton;
        QPushButton*            staticShapesButton;
        QPushButton*            pauseButton;
        QSlider*                speedSlider;
        float                   speedMultiplier;

        const int               buttonDim = 100;

};

#endif // BOX2DSIMULATIONWIDGET_H
