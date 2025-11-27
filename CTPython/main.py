import sys
from PySide6.QtGui import QGuiApplication
from PySide6.QtGui import QIcon
from PySide6.QtQml import QQmlApplicationEngine
from PySide6.QtCore import QUrl
from PySide6.QtCore import qInstallMessageHandler


import os
os.environ["QSG_RHI_BACKEND"] = "opengl"  # force OpenGL scene graph
os.environ["QT_QUICK_CONTROLS_STYLE"] = "Fusion"  # optional, avoid missing style

def qt_message_handler(mode, context, message):
    print(message)

qInstallMessageHandler(qt_message_handler)


def handle_warnings(warnings):
    for w in warnings:
        print(f"[QML Warning] {w.toString()}")

if __name__ == "__main__":
    app = QGuiApplication(sys.argv)
    app.setWindowIcon(QIcon("icon.png"))
    engine = QQmlApplicationEngine()
    engine.warnings.connect(handle_warnings)

    engine.addImportPath(r"C:/Qt/6.9.1/msvc2022_64/qml")
    engine.addImportPath(r"C:/Qt/6.9.1/msvc2022_64/qml/QtQuick/Controls.2")
    engine.addImportPath(r"C:/Qt/6.9.1/msvc2022_64/qml/QtQuick/Layouts")

    engine.addImportPath("qml")

    qml_file = QUrl.fromLocalFile("qml/main.qml")
    engine.load(qml_file)

    if not engine.rootObjects():
        print("Error: QML failed to load.")
        sys.exit(-1)

    sys.exit(app.exec())
