/****************************************************************************
**
** Copyright (C) 2017 Crimson AS <info@crimson.no>
** Contact: https://www.qt.io/licensing/
**
** This file is part of the qmlbench tool.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <iostream>
#include <iomanip>

#include <QtCore>
#include <QtGui>

class FpsDecider : public QWindow
{
public:
    FpsDecider()
        : gl(0)
        , frameCount(0)
    {
        setSurfaceType(OpenGLSurface);
        QSurfaceFormat format;
#if QT_VERSION >= 0x050300
        format.setSwapInterval(1);
#endif
        setFormat(format);
    }

    void exposeEvent(QExposeEvent *) override {
        if (isExposed())
            render();
    }

    bool event(QEvent *e) override
    {
        if (e->type() == QEvent::UpdateRequest) {
            render();
            return true;
        }
        return QWindow::event(e);
    }

    void render()
    {
        if (!gl) {
            gl = new QOpenGLContext();
            gl->setFormat(format());
            gl->create();
            timer.start();
        }

        gl->makeCurrent(this);
        QOpenGLFunctions *func = gl->functions();

        if (frameCount == 0) {
#if QT_VERSION >= 0x050300
            std::cout << "GL_VENDOR:   " << (const char *) func->glGetString(GL_VENDOR) << std::endl;
            std::cout << "GL_RENDERER: " << (const char *) func->glGetString(GL_RENDERER) << std::endl;
            std::cout << "GL_VERSION:  " << (const char *) func->glGetString(GL_VERSION) << std::endl;
#else
            Q_UNUSED(func);
            std::cout << "GL_VENDOR:   " << (const char *) glGetString(GL_VENDOR) << std::endl;
            std::cout << "GL_RENDERER: " << (const char *) glGetString(GL_RENDERER) << std::endl;
            std::cout << "GL_VERSION:  " << (const char *) glGetString(GL_VERSION) << std::endl;
#endif
        }

        ++frameCount;
        int c = frameCount % 2;

#if QT_VERSION >= 0x050300
        func->glClearColor(c, 0, 1 - c, 1);
        func->glClear(GL_COLOR_BUFFER_BIT);
#else
        glClearColor(c, 0, 1 - c, 1);
        glClear(GL_COLOR_BUFFER_BIT);
#endif

        gl->swapBuffers(this);

        int time = timer.elapsed();
        if (time > 5000) {
            qreal msPerFrame = time / float(frameCount);
            qreal screenRate = 1000.0 / screen()->refreshRate();
            std::cout << std::endl
                     << "FPS: " << frameCount * 1000 / float(time)
                     << " -- " << frameCount << " frames in " << time << "ms; "
                     <<  msPerFrame << " ms/frame"
                     << "; QScreen says: " << screenRate << std::endl;
            if (qAbs(screenRate - msPerFrame) > msPerFrame * 0.1)
                std::cout << " (not accurate, run benchmarks with '--fps-override " << int(1000/msPerFrame) << "')" << std::endl << std::endl;
            else
                std::cout << std::endl << "That should be close enough :)" << std::endl << std::endl;
            exit(0);

        } else {
            QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
        }
    }

private:
    QOpenGLContext *gl;
    QElapsedTimer timer;
    int frameCount;
};

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);
    FpsDecider fpsDecider;

    if (app.arguments().contains("--fullscreen"))
        fpsDecider.showFullScreen();
    else
        fpsDecider.show();

    fpsDecider.raise();
    return app.exec();
}
