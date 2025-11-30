#include "quadcontrol_global.h"
#include <QQmlExtensionPlugin>
#include <qqml.h>
#include "glview3d.h"
#include <contextWrapper.h>
#include "glviewxy.h"
#include "glviewyz.h"
#include "glviewxz.h"

class QuadControlPlugin : public QQmlExtensionPlugin {
    Q_OBJECT
        Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)
public:
    void registerTypes(const char* uri) override 
    {
        QString path = R"(D:\Files\Cesars\Scissors_Test 2025-7-2 15-11-21.uint16_scv)";
        //QString path = R"(D:\Files\CTLab\SaveVolumeToFile\volumeHeader.uint16_scv)";

        qmlRegisterSingletonType<ContextWrapper>("QuadControl", 1, 0, "VolumeContext",
            [path](QQmlEngine*, QJSEngine*) -> QObject* {
                return new ContextWrapper(path);
            });

        // URI must match qmldir
       qmlRegisterType<GLView3D>("QuadControl", 1, 0, "GLView3D");
       qmlRegisterType<GLViewXY>("QuadControl", 1, 0, "GLViewXY");
       qmlRegisterType<GLViewYZ>("QuadControl", 1, 0, "GLViewYZ");
       qmlRegisterType<GLViewXZ>("QuadControl", 1, 0, "GLViewXZ");
    }
};

#include "plugin.moc"