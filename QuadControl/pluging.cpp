#include "quadcontrol_global.h"
#include <QQmlExtensionPlugin>
#include <qqml.h>
#include "glview3d.h"
//#include "glviewquadaxial.h"
//#include "glviewquadcoronal.h"
//#include "glviewquadsagittal.h"

class QuadLibPlugin : public QQmlExtensionPlugin {
    Q_OBJECT
        Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)
public:
    void registerTypes(const char* uri) override {
        // URI must match qmldir
        qmlRegisterType<GLView3D>("QuadControl", 1, 0, "GLView3D");
       // qmlRegisterType<GLViewQuad3D>("QuadControl", 1, 0, "GLViewQuadAxial");
       // qmlRegisterType<GLViewQuad3D>("QuadControl", 1, 0, "GLViewQuadCoronal");
       // qmlRegisterType<GLViewQuad3D>("QuadControl", 1, 0, "GLViewQuadSagittal");
    }
};

//#include "plugin.moc"