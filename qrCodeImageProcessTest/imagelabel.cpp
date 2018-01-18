#include "imagelabel.h"
#include <QtGui>


ImageLabel::ImageLabel( QWidget* parent )
    : QWidget( parent )
{
    recomputeCachedImage();
}

ImageLabel::ImageLabel( const QImage& image, QWidget* parent )
    : QWidget( parent ),
      image( image )
{
}

void ImageLabel::setImage( const QImage& image )
{
    this->image = image;
    recomputeCachedImage();
}

void ImageLabel::paintEvent( QPaintEvent* )
{
    QPainter p( this );
    p.save();

    int cachedImageWidth    = 0.5 * width() - 0.5 * cachedImage.width();
    int cachedImageHeight   = 0.5 * height() - 0.5 * cachedImage.height();

    p.drawImage( cachedImageWidth, cachedImageHeight, cachedImage );

    p.restore();
}

void ImageLabel::resizeEvent( QResizeEvent* )
{
    recomputeCachedImage();
}

void ImageLabel::recomputeCachedImage()
{
    if( image.isNull() )
        return;

    cachedImage = image.scaled( width(), height(), Qt::KeepAspectRatio, Qt::SmoothTransformation );
    update();
}
