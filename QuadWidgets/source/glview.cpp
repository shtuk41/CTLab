#include <glview.h>

GLView::GLView(QWidget* parent, Context *c)
    : QOpenGLWidget(parent),
    context(c)

{
}

GLView::~GLView()
{
 
}
