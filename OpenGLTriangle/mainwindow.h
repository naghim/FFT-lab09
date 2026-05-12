#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

class MainWindow : public QOpenGLWindow, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    using QOpenGLWindow::QOpenGLWindow;

    void initializeGL() override;
    void paintGL() override;

    virtual bool event(QEvent *event) override;

private:
    GLint m_posAttr = 0;
    GLint m_colAttr = 0;
    GLint m_matrixUniform = 0;

    // A kamera pozíciója
    QVector3D translate{0.0, -0.0, -2.0};

    QOpenGLShaderProgram *m_program = nullptr;
    int m_frame = 0;
};
#endif // MAINWINDOW_H
