#ifndef PIXMAPLABEL_H
#define PIXMAPLABEL_H


#include <QWidget>


class PixmapLabel : public QWidget
{
    Q_OBJECT
    public:

        explicit        PixmapLabel( QWidget* parent = nullptr );
        explicit        PixmapLabel( const QPixmap& pixmap, QWidget* parent = nullptr );

        void            setPixmap( const QPixmap& pixmap );
        void            setAngleDegrees( qreal angle );
        void            setAngleRadians( qreal angle );


    private:

        virtual void    paintEvent( QPaintEvent* ) override;
        virtual void    resizeEvent( QResizeEvent* ) override;
        void            recomputePixmap();
        QPointF         getRotatedPoint( QPointF p, QPointF center, qreal angleRads );
        QRectF          getMinimumBoundingRect( QRect r );
        qreal           getMin( qreal x1, qreal x2, qreal x3, qreal x4 );
        qreal           getMax( qreal x1, qreal x2, qreal x3, qreal x4 );
        QPixmap         getRotatedPixmap( const QPixmap& pixmap );


        QPixmap         pixmap;
        QPixmap         rotatedPixmap;
        QPixmap         scaledPixmap;
        qreal           angleDegrees = 0.0;
        qreal           angleRads = 0.0;


}; // PixmapLabel


#endif // PIXMAPLABEL_H
