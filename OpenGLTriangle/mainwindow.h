#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QOpenGLShaderProgram>

#include "openglwindow.h"

class MainWindow : public OpenGLWindow {
    Q_OBJECT

public:
    using OpenGLWindow::OpenGLWindow;

    void initialize() override;
    void render() override;

    virtual bool event(QEvent *event) override;
private:
    GLint m_posAttr = 0;
    GLint m_colAttr = 0;
    GLint m_matrixUniform = 0;

    // A kamera pozíciója
    QVector3D translate {0.0, -0.0, -2.0};

    QOpenGLShaderProgram *m_program = nullptr;
    int m_frame = 0;
};
#endif // MAINWINDOW_H
