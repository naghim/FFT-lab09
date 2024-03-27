#include "mainwindow.h"

#include <QScreen>
#include <QKeyEvent>

// Ez a shader minden vertexre (csúcspontra) kiszámítja
// a vertex pozícióját a transzformációs mátrix (kamera)
// függvényében, és átadja a fragment shadernek a vertex
// színét.
static const char *vertexShaderSource =
    "attribute highp vec4 posAttr;\n"
    "attribute lowp vec4 colAttr;\n"
    "varying lowp vec4 col;\n"
    "uniform highp mat4 matrix;\n"
    "void main() {\n"
    "   col = colAttr;\n"
    "   gl_Position = matrix * posAttr;\n"
    "}\n";

// Ez a shader beállítja a fragment (pixel, képpont) színét.
// A színt a vertex shadertől kapja.
static const char *fragmentShaderSource =
    "varying lowp vec4 col;\n"
    "void main() {\n"
    "   gl_FragColor = col;\n"
    "}\n";

void MainWindow::initialize() {
    // Alapértelmezett shaderek beállítása
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();

    // A CPU ezeken az attribútumokon keresztül állítja be a GPU-nak, hogy:
    // posAttr: milyen pozícióban jelenjenek meg a csúcspontok. A GPU összeköti ezeket a pontokat.
    // colAttr: milyen színe legyen egy csúcspontnak.
    // matrix: hogyan transzformáljuk a képet a képernyőre (kamera beállítások).
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    m_matrixUniform = m_program->uniformLocation("matrix");
    setTitle("Rectangles and Triangles");
    resize(500, 500);

    // Ha szeretnénk, hogy minden pillanatban automatikusan frissüljön a kép,
    // használjuk az OpenGLWindow-unk a setAnimating függvényét
    //setAnimating(true);
}

void MainWindow::render() {
    // Beállítjuk a tényleges ablak dimenziót, ahol megjelenítjük a képet.
    // Az ablak dimenziója és az OpenGL által renderelt kép dimenziója meg kell egyezzen.
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    // Színek resetelése, hogy ne mutassuk az előző frame színeit.
    glClear(GL_COLOR_BUFFER_BIT);

    m_program->bind();

    // Kamera (végülis egy transzformációs mátrix) beállítása
    // Úgy kell elképzelni, mintha a virtuális térben lenne egy láthatatlan kamera
    // amely egy bizonyos pontra néz, és így vetíti a képet a képernyőre.
    QMatrix4x4 matrix;
    matrix.perspective(60.0f, (float) width() / height(), 0.1f, 100.0f);
    matrix.translate(translate);

    m_program->setUniformValue(m_matrixUniform, matrix);

    // Háromszög kirajzolása
    // Meg kell adjuk a csúcspontokat: (x, y) koordináták a térben
    // illetve a színeket: (r, g, b) intenzitásértékek 0-tól 1-ig
    static const GLfloat vertices[] = {
        -0.5f, -0.5f,
        0.5f, 0.5f,
        0.5f, -0.5f
    };

    static const GLfloat colors[] = {
        1.0f, 0.0f, 0.0f, // Piros
        0.0f, 1.0f, 0.0f, // Zöld
        0.0f, 0.0f, 1.0f  // Kék
    };

    // Átadjuk a shader programnak a pozíciő és szín attribútumokat
    glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

    glEnableVertexAttribArray(m_posAttr);
    glEnableVertexAttribArray(m_colAttr);

    // Megkérjük a GPU-t hogy rajzolja ki a háromszöget, összekötve a pontokat:
    // a legelső vertex a 0-ik indexen található, az utolsó a 3-dik indexen
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(m_colAttr);
    glDisableVertexAttribArray(m_posAttr);

    m_program->release();

    ++m_frame;
}

bool MainWindow::event(QEvent *event) {
    if (event->type() != QEvent::KeyPress) {
        return OpenGLWindow::event(event);
    }

    QKeyEvent *ke = static_cast<QKeyEvent *>(event);

    // Billentyűnyomásra frissíthető a kamera pozíciója
    // Ezután megkérjük, hogy rajzoljon egy új frame-et
    switch (ke->key()) {
    case Qt::Key_W:
        translate.setZ(translate.z() + 0.5);
        requestUpdate();
        break;
    case Qt::Key_S:
        translate.setZ(translate.z() - 0.5);
        requestUpdate();
        break;
    }

    return OpenGLWindow::event(event);
}
