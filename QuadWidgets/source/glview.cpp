#include <glview.h>

GLView::GLView(QWidget* parent, Context *c, const QColor& color)
    : QOpenGLWidget(parent),
    context(c),
    border(0.015f, color)

{
}

GLView::~GLView()
{
 
}
