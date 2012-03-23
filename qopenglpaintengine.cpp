#include "qopenglpaintengine.h"

QOpenGLPaintEngine::QOpenGLPaintEngine() : QPaintEngine(){

}

void QOpenGLPaintEngine::drawImage(const QRectF &r, const QImage &image,
                              const QRectF &sr, Qt::ImageConversionFlags)
{
    /*Q_D(QOpenGLPaintEngine);
    if (d->composition_mode > QPainter::CompositionMode_Plus
         || d->high_quality_antialiasing && !d->isFastRect(r))
        d->drawImageAsPath(r, image, sr);
    else {
        GLenum target = (QGLExtensions::glExtensions
                         & QGLExtensions::TextureRectangle)
                        ? GL_TEXTURE_RECTANGLE_NV
                        : GL_TEXTURE_2D;
        if (r.size() != image.size())
            target = GL_TEXTURE_2D;
        d->flushDrawQueue();
        d->drawable.bindTexture(image, target);
        drawTextureRect(image.width(), image.height(), r, sr, target);
    }*/
}
