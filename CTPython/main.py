import sys
import os
import PySide6
from PySide6.QtGui import QGuiApplication, QIcon
from PySide6.QtQml import QQmlApplicationEngine
from PySide6.QtCore import QUrl, qInstallMessageHandler

# Force OpenGL scene graph and Fusion style across platforms
os.environ["QSG_RHI_BACKEND"] = "opengl"
os.environ["QT_QUICK_CONTROLS_STYLE"] = "Fusion"

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

    # Use PySide6's bundled QML path exclusively
    pyside_qml_path = os.path.join(os.path.dirname(PySide6.__file__), "qml")
    engine.addImportPath(pyside_qml_path)

    # Windows-specific custom Qt path if needed
    if sys.platform == "win32":
        engine.addImportPath(r"C:/Qt/6.9.1/msvc2022_64/qml")
        engine.addImportPath(r"C:/Qt/6.9.1/msvc2022_64/qml/QtQuick/Controls.2")
        engine.addImportPath(r"C:/Qt/6.9.1/msvc2022_64/qml/QtQuick/Layouts")

    # Local QML path works uniformly on both OS environments
    engine.addImportPath("qml")

    qml_file = QUrl.fromLocalFile("qml/main.qml")
    engine.load(qml_file)

    if not engine.rootObjects():
        print("Error: QML failed to load.")
        sys.exit(-1)

    sys.exit(app.exec())