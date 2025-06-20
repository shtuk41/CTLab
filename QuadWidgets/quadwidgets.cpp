#include "quadwidgets.h"
#include "MyGLView.h"
#include <QStandardItemModel>

QuadWidgets::QuadWidgets(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    populateTree();

    // Replace placeholders with MyGLView instances
    struct ViewInfo {
        QString placeholderName;
        QColor color;
    };

    QVector<ViewInfo> views = {
        { "widgetTopLeft", Qt::red },
        { "widgetTopRight", Qt::green },
        { "widgetBottomLeft", Qt::blue },
        { "widgetBottomRight", Qt::yellow }
    };

    for (const auto& view : views) {
        QWidget* placeholder = ui.centralWidget->findChild<QWidget*>(view.placeholderName);
        if (placeholder) {
            auto* glView = new MyGLView(view.color, placeholder->parentWidget());
            glView->setMinimumSize(100, 100);
            glView->setObjectName(view.placeholderName);
            auto* layout = new QVBoxLayout(placeholder->parentWidget());
            layout->setContentsMargins(0, 0, 0, 0);
            placeholder->parentWidget()->setLayout(layout);
            layout->addWidget(glView);
            placeholder->deleteLater();
        }
    }

    ui.toolboxDock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);

    // Set the initial check state based on visibility
    ui.actionToolbox->setChecked(ui.toolboxDock->isVisible());

    // Connect the toggle behavior
    connect(ui.actionToolbox, &QAction::triggered, this, [this](bool checked) {
        ui.toolboxDock->setVisible(checked);
        });

    this->showMaximized();

}

QuadWidgets::~QuadWidgets()
{}

void QuadWidgets::populateTree()
{
    auto* model = new QStandardItemModel(this);
    auto* rootItem = model->invisibleRootItem();

    rootItem->appendRow(new QStandardItem("Item 1"));
    rootItem->appendRow(new QStandardItem("Item 2"));
    rootItem->appendRow(new QStandardItem("Item 3"));

    ui.treeView->setModel(model);
}

