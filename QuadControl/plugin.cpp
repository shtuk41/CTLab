#include "quadcontrol_global.h"
#include <QQmlExtensionPlugin>
#include <qqml.h>
#include "glview3d.h"
#include <contextWrapper.h>
#include "glviewaxial.h"
//#include "glviewcoronal.h"
//#include "glviewsagittal.h"

class QuadControlPlugin : public QQmlExtensionPlugin {
    Q_OBJECT
        Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)
public:
    void registerTypes(const char* uri) override 
    {
        qmlRegisterSingletonType<ContextWrapper>("QuadControl", 1, 0, "VolumeContext",
            [](QQmlEngine*, QJSEngine*) -> QObject* {
                return new ContextWrapper(R"(D:\Files\Cesars\Scissors_Test 2025-7-2 15-11-21.uint16_scv)");
            });

        // URI must match qmldir
       qmlRegisterType<GLView3D>("QuadControl", 1, 0, "GLView3D");
       qmlRegisterType<GLViewAxial>("QuadControl", 1, 0, "GLViewAxial");
       //qmlRegisterType<GLViewCoronal>("QuadControl", 1, 0, "GLViewCoronal");
       //qmlRegisterType<GLViewSagittal>("QuadControl", 1, 0, "GLViewSagittal");
    }
};

#include "plugin.moc"