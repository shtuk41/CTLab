#include "quadlib_global.h"
#include <QQmlExtensionPlugin>
#include <qqml.h>
#include "volumeViewControl.h"

class QuadLibPlugin : public QQmlExtensionPlugin {
    Q_OBJECT
        Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)
public:
    void registerTypes(const char* uri) override {
        // URI must match qmldir
        qmlRegisterType<VolumeViewControl>("QuadLib", 1, 0, "VolumeViewControl");
    }
};

#include "plugin.moc"