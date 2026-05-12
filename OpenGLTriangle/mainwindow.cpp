#include "mainwindow.h"
#include <QScreen>
#include <QKeyEvent>

// Ez a shader minden csúcspont pozícióját a transzformációs
// mátrix segítségével számítja ki, és átadja a színt a
// fragment shadernek.
static const char *vertexShaderSource =
    "attribute highp vec4 posAttr;\n"
    "attribute lowp vec4 colAttr;\n"
    "varying lowp vec4 col;\n"
    "uniform highp mat4 matrix;\n"
    "void main() {\n"
    "   col = colAttr;\n"
    "   gl_Position = matrix * posAttr;\n"
    "}\n";

// Ez a shader állítja be a fragmentek végső színét.
// A színt a vertex shaderből kapja.
static const char *fragmentShaderSource =
    "varying lowp vec4 col;\n"
    "void main() {\n"
    "   gl_FragColor = col;\n"
    "}\n";

void MainWindow::initializeGL()
{
    initializeOpenGLFunctions();

    // Alapértelmezett shaderek beállítása
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();

    // A CPU ezeken az attribútumokon keresztül adja át a GPU-nak az adatokat:
    // posAttr: a csúcspontok pozíciója.
    // colAttr: a csúcspontok színe.
    // matrix: a transzformációs mátrix, amivel a képet a képernyőre vetítjük.
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    m_matrixUniform = m_program->uniformLocation("matrix");
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    setTitle("Háromszög OpenGL-ben");
    resize(500, 500);

    // Ha folyamatos animáció kell, egy időzítőből hívhatjuk az update() függvényt.
}

void MainWindow::paintGL()
{
    // Beállítjuk a renderelési felület méretét a képernyő DPI-jéhez igazítva.
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    // Töröljük az előző frame tartalmát.
    glClear(GL_COLOR_BUFFER_BIT);

    m_program->bind();

    // Perspektivikus vetítés és eltolás.
    // Ez a mátrix viselkedik úgy, mint egy egyszerű kamera/transzformáció.
    QMatrix4x4 matrix;
    matrix.perspective(60.0f, (float)width() / height(), 0.1f, 100.0f);
    matrix.translate(translate);

    m_program->setUniformValue(m_matrixUniform, matrix);

    // Háromszög csúcspontjai: (x, y) koordináták a térben.
    static const GLfloat vertices[] = {
        -0.5f, -0.5f,
        0.5f, 0.5f,
        0.5f, -0.5f};

    // A csúcspontokhoz tartozó színek: (r, g, b) 0 és 1 között.
    static const GLfloat colors[] = {
        1.0f, 0.0f, 0.0f, // Piros
        0.0f, 1.0f, 0.0f, // Zöld
        0.0f, 0.0f, 1.0f  // Kék
    };

    // Átadjuk a shader programnak a pozíció- és színattribútumokat.
    glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

    glEnableVertexAttribArray(m_posAttr);
    glEnableVertexAttribArray(m_colAttr);

    // Kirajzolunk 3 csúcsot a 0-s indexről indulva.
    // A paraméterek sorrendje: primitív típus, kezdőindex, darabszám.
    // Itt tehát az indexek 0, 1 és 2 kerülnek felhasználásra.
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(m_colAttr);
    glDisableVertexAttribArray(m_posAttr);

    m_program->release();

    ++m_frame;
}

bool MainWindow::event(QEvent *event)
{
    if (event->type() != QEvent::KeyPress)
    {
        return QOpenGLWindow::event(event);
    }

    QKeyEvent *ke = static_cast<QKeyEvent *>(event);

    // Billentyűnyomásra módosítjuk a kamera pozícióját,
    // majd újrarajzolást kérünk.
    switch (ke->key())
    {
    case Qt::Key_W:
        translate.setZ(translate.z() + 0.5);
        update();
        break;
    case Qt::Key_S:
        translate.setZ(translate.z() - 0.5);
        update();
        break;
    }

    return QOpenGLWindow::event(event);
}
