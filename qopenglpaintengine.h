#ifndef QOPENGLPAINTENGINE_H
#define QOPENGLPAINTENGINE_H

#include <QPaintEngine>

class QOpenGLPaintEngine : public QPaintEngine
{
public:
    QOpenGLPaintEngine();

    void drawImage(const QRectF &r, const QImage &image,
                                  const QRectF &sr, Qt::ImageConversionFlags);
};

#endif // QOPENGLPAINTENGINE_H
