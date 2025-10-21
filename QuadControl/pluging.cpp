#include "quadcontrol_global.h"
#include <QQmlExtensionPlugin>
#include <qqml.h>
#include "glview3d.h"
#include "glviewaxial.h"
//#include "glviewcoronal.h"
//#include "glviewsagittal.h"

class QuadLibPlugin : public QQmlExtensionPlugin {
    Q_OBJECT
        Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)
public:
    void registerTypes(const char* uri) override {
        // URI must match qmldir
       qmlRegisterType<GLView3D>("QuadControl", 1, 0, "GLView3D");
       qmlRegisterType<GLViewAxial>("QuadControl", 1, 0, "GLViewAxial");
       //qmlRegisterType<GLViewCoronal>("QuadControl", 1, 0, "GLViewCoronal");
       //qmlRegisterType<GLViewSagittal>("QuadControl", 1, 0, "GLViewSagittal");
    }
};

//#include "plugin.moc"