#include <glview.h>

GLView::GLView(Context *c, const QColor& color) 
    : initialized(false),
    context(c),
    border(0.015f, color)

{
}
