#ifndef QGLCANVAS_H
#define QGLCANVAS_H

#include <QGLWidget>
#include <QtOpenGL/qglshaderprogram.h>
#include <QGLBuffer>

class QGLCanvas : public QGLWidget
{
    Q_OBJECT
public:
    QGLCanvas(QWidget* parent = NULL);
    ~QGLCanvas();
    void setImage(QImage *image);
    //void setImage(unsigned char* rgb);

    bool initialized;

protected:
    void paintEvent(QPaintEvent*);
    void paintGL ();
    void initializeGL ();

private:
    QImage *img2;
    clock_t time;
    QPainter *p;

    QImage img;
    GLuint  m_uiTexture;
    qreal   m_fAngle;
    QGLBuffer glbuffer;
    QGLShaderProgram program2;
    int matrixUniform1;
    int vertexAttr2;
    int normalAttr2;
    int texCoordAttr2;
    int matrixUniform2;
    int textureUniform2;
};

#endif // QGLCANVAS_H
