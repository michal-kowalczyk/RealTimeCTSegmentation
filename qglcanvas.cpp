#include "qglcanvas.h"
#include <QDebug>
#include <QtOpenGL>
#include <QGLWidget>
#include <QPainter>
#include <QtGui/qmatrix4x4.h>

QGLCanvas::QGLCanvas(QWidget* parent)
    : //QGLWidget(parent)
QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    /*setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);
    setAutoBufferSwap(false);

    initialized = false;

    m_fAngle = 0.0f;*/

    //p = new QPainter(this);
}


QGLCanvas::~QGLCanvas(){
}

/*
GLfloat afTexCoord[] = {
    1.0f,1.0f, 1.0f,0.0f, 0.0f,1.0f,
    0.0f,0.0f, 0.0f,1.0f, 1.0f,0.0f
};


GLfloat afVertices[] = {
    0.5,0.5,0.5,
    0.5,-0.5,0.5,
    -0.5,0.5,0.5,

    -0.5,-0.5,0.5,
    -0.5,0.5,0.5,
    0.5,-0.5,0.5
};


GLfloat afNormals[] = {
    0,0,1, 0,0,1, 0,0,1,
    0,0,1, 0,0,1, 0,0,1
};
*/

/*
    const char* DepthShadowMapVertexShader =
            GLSL_VERSION_STRING \
            "precision highp float;\n" \
            "\n" \
            "attribute vec3 in_coord;\n" \
            "uniform mat4 mvp_matrix;\n" \
            "\n" \
            "void main()\n" \
            "{\n" \
            "   gl_Position = mvp_matrix * vec4(in_coord, 1.0);\n" \
            "}\n";

    const char* DepthShadowMapFragmentShader =
            GLSL_VERSION_STRING \
            "precision highp float;\n" \
            "\n" \
            "void main()\n" \
            "{\n" \
            "}\n";
*/
void QGLCanvas::initializeGL (){
    /*glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_SRC_COLOR);

    QGLShader *vshader2 = new QGLShader(QGLShader::Vertex);
    const char *vsrc2 =
        "attribute highp vec4 vertex;\n"
        "attribute highp vec4 texCoord;\n"
        "attribute mediump vec3 normal;\n"
        "uniform mediump mat4 matrix;\n"
        "varying highp vec4 texc;\n"
        "varying mediump float angle;\n"
        "void main(void)\n"
        "{\n"
        "    vec3 toLight = normalize(vec3(0.0, 0.3, 1.0));\n"
        "    angle = max(dot(normal, toLight), 0.0);\n"
        "    gl_Position = matrix * vertex;\n"
        "    texc = texCoord;\n"
        "}\n";
    vshader2->compileSourceCode(vsrc2);
    program2.addShader(vshader2);

    QGLShader *fshader2 = new QGLShader(QGLShader::Fragment);
    const char *fsrc2 =
        "varying highp vec4 texc;\n"
        "uniform sampler2D tex;\n"
        "varying mediump float angle;\n"
        "void main(void)\n"
        "{\n"
        "    highp vec3 color = texture2D(tex, texc.st).rgb;\n"
        "    color = color * 0.2 + color * 0.8 * angle;\n"
        "    gl_FragColor = vec4(clamp(color, 0.0, 1.0), 1.0);\n"
        "}\n";
    fshader2->compileSourceCode(fsrc2);
    program2.addShader(fshader2);
    program2.link();

    vertexAttr2 = program2.attributeLocation("vertex");
    normalAttr2 = program2.attributeLocation("normal");
    texCoordAttr2 = program2.attributeLocation("texCoord");
    matrixUniform2 = program2.uniformLocation("matrix");
    textureUniform2 = program2.uniformLocation("tex");

    QImage image = QImage(640, 480, QImage::Format_ARGB32);
    image.fill(qRgba (0, 0, 255, 255));

    glGenTextures(1, &m_uiTexture);
    glBindTexture(GL_TEXTURE_2D, m_uiTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image.bits());

    initialized = true;*/
}


void QGLCanvas::setImage(QImage* image)
{
    /*img = QImage(640, 480, QImage::Format_ARGB32);
    for (int x = 0; x < 640; x++)
        for (int y = 0; y < 480; y++)
            img.setPixel(x, y, image.pixel(x, y));*/
    img2 = image;//QImage("/sdcard/fail.jpg");//
    //qDebug () << time;
    update();
    //updateGL ();
}


void QGLCanvas::paintEvent(QPaintEvent*)
{
    if (img2 == 0)
        return;
    if (img2 -> width() < 1 || img2 -> height() < 1)
        return;

    QPainter p(this);
    p.begin(this);
    p.drawImage(QRect(0, 0, 640, 480), *img2);//QImage("/sdcard/fail.jpg")
    p.end();
}



void QGLCanvas::paintGL(){
/*    QPainter painter;
    painter.begin(this);

    painter.beginNativePainting();

    glBindTexture(GL_TEXTURE_2D, m_uiTexture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, img.width(), img.height(), GL_RGBA, GL_UNSIGNED_BYTE, img.bits());

    QMatrix4x4 modelview;

    program2.bind();

    program2.setUniformValue(matrixUniform2, modelview);
    program2.setAttributeArray(vertexAttr2, afVertices, 3);
    program2.setAttributeArray(texCoordAttr2, afTexCoord, 2);
    program2.setAttributeArray(normalAttr2, afNormals, 3);
    program2.setUniformValue(textureUniform2, 0);

    program2.enableAttributeArray(vertexAttr2);
    program2.enableAttributeArray(normalAttr2);
    program2.enableAttributeArray(texCoordAttr2);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    program2.disableAttributeArray(vertexAttr2);
    program2.disableAttributeArray(normalAttr2);
    program2.disableAttributeArray(texCoordAttr2);

    program2.release();

    painter.endNativePainting();*/
}
