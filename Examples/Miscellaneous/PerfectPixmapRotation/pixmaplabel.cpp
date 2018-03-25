#include "pixmaplabel.h"
#include <QtGui>


PixmapLabel::PixmapLabel( QWidget* parent )
    : QWidget( parent )
{
    pixmap = QPixmap( 100, 20 );
    QPainter p( &pixmap );
    p.save();
    p.fillRect( pixmap.rect(), Qt::red );
    p.restore();

    recomputePixmap();
}

PixmapLabel::PixmapLabel( const QPixmap& pixmap, QWidget* parent )
    : QWidget( parent ),
      pixmap( pixmap )
{
    recomputePixmap();
}

void PixmapLabel::setPixmap( const QPixmap& pixmap )
{
    this->pixmap = pixmap;
    recomputePixmap();
    update();
}

void PixmapLabel::setAngleDegrees( qreal angle )
{
    angleDegrees    = angle;
    angleRads       = qDegreesToRadians( angle );

    recomputePixmap();
    update();
}

void PixmapLabel::setAngleRadians( qreal angle )
{
    angleDegrees    = qRadiansToDegrees( angle );
    angleRads       = angle;

    recomputePixmap();
    update();
}

void PixmapLabel::paintEvent( QPaintEvent* )
{
    QPainter p( this );
    p.save();

    QRect renderRect( 0.5 * width() - 0.5 * scaledPixmap.width(),
                      0.5 * height() - 0.5 * scaledPixmap.height(),
                      scaledPixmap.width(),
                      scaledPixmap.height() );

    p.drawPixmap( renderRect, scaledPixmap, scaledPixmap.rect() );

    p.restore();
}

void PixmapLabel::resizeEvent( QResizeEvent* )
{
    recomputePixmap();
}

void PixmapLabel::recomputePixmap()
{
    rotatedPixmap = getRotatedPixmap( pixmap );
    scaledPixmap = rotatedPixmap.scaled( size(), Qt::KeepAspectRatio, Qt::SmoothTransformation );
}

QPointF PixmapLabel::getRotatedPoint( QPointF p, QPointF center, qreal angleRads )
{
    qreal x = p.x();
    qreal y = p.y();

    float s = qSin( angleRads );
    float c = qCos( angleRads );

    // translate point back to origin:
    x -= center.x();
    y -= center.y();

    // rotate point
    float xnew = x * c - y * s;
    float ynew = x * s + y * c;

    // translate point back:
    x = xnew + center.x();
    y = ynew + center.y();

    return QPointF( x, y );
}

QRectF PixmapLabel::getMinimumBoundingRect( QRect r )
{
    QPointF topLeft     = getRotatedPoint( r.topLeft(),     r.center(), angleRads );
    QPointF bottomRight = getRotatedPoint( r.bottomRight(), r.center(), angleRads );
    QPointF topRight    = getRotatedPoint( r.topRight(),    r.center(), angleRads );
    QPointF bottomLeft  = getRotatedPoint( r.bottomLeft(),  r.center(), angleRads );

    qreal minX = getMin( topLeft.x(), bottomRight.x(), topRight.x(), bottomLeft.x() );
    qreal minY = getMin( topLeft.y(), bottomRight.y(), topRight.y(), bottomLeft.y() );

    qreal maxX = getMax( topLeft.x(), bottomRight.x(), topRight.x(), bottomLeft.x() );
    qreal maxY = getMax( topLeft.y(), bottomRight.y(), topRight.y(), bottomLeft.y() );

    return QRectF( QPointF( minX, minY ), QPointF( maxX, maxY ) );
}

qreal PixmapLabel::getMin( qreal x1, qreal x2, qreal x3, qreal x4 )
{
    qreal ret = x1;

    if( x2 < ret )
        ret = x2;
    if( x3 < ret )
        ret = x3;
    if( x4 < ret )
        ret = x4;

    return ret;
}

qreal PixmapLabel::getMax( qreal x1, qreal x2, qreal x3, qreal x4 )
{
    qreal ret = x1;

    if( x2 > ret )
        ret = x2;
    if( x3 > ret )
        ret = x3;
    if( x4 > ret )
        ret = x4;

    return ret;
}

QPixmap PixmapLabel::getRotatedPixmap( const QPixmap& pixmap )
{
    QRectF rotatedRect = getMinimumBoundingRect( pixmap.rect() );

    rotatedPixmap = QPixmap( rotatedRect.width(), rotatedRect.height() );

    QPainter p( &rotatedPixmap );
    p.save();

    p.fillRect( rotatedPixmap.rect(), Qt::white );
    p.translate( 0.5 * rotatedPixmap.width(), 0.5 * rotatedPixmap.height() );
    p.rotate( angleDegrees );
    p.translate( -0.5 * rotatedPixmap.width(), -0.5 * rotatedPixmap.height() );

    QRectF renderRect( 0.5 * rotatedRect.width() - 0.5 * pixmap.width(),
                       0.5 * rotatedRect.height() - 0.5 * pixmap.height(),
                       pixmap.width(),
                       pixmap.height() );
    p.drawPixmap( renderRect, pixmap, pixmap.rect() );

    p.restore();
    return rotatedPixmap;
}
