#include <glview.h>

GLView::GLView(std::shared_ptr<Context> c, const QColor& color) 
    : initialized(false),
    context(c),
    border(0.015f, color)

{
}
