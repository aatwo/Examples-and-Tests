#ifndef IMAGELABEL_H
#define IMAGELABEL_H


#include <QWidget>
#include <QImage>


class ImageLabel : public QWidget
{
    Q_OBJECT
    public:

        explicit            ImageLabel( QWidget* parent = nullptr );
        explicit            ImageLabel( const QImage& image, QWidget* parent = nullptr );

        void                setImage( const QImage& image );


    private:

        virtual void        paintEvent( QPaintEvent* ) override;
        virtual void        resizeEvent( QResizeEvent* ) override;
        void                recomputeCachedImage();

        QImage              image;
        QImage              cachedImage;


}; // ImageLabel


#endif // IMAGELABEL_H
