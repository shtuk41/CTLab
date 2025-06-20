#include "quadwidgets.h"
#include "MyGLView.h"
#include <QStandardItemModel>

QuadWidgets::QuadWidgets(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    ui.quad_horizontal_top_splitter->setSizes({ 100, 100 });
    ui.quad_horizontal_bottom_splitter->setSizes({ 100, 100 });
    ui.quad_splitter->setSizes({ 100, 100 });

    populateTree();

    // Replace placeholders with MyGLView instances
    struct ViewInfo {
        QString placeholderName;
        QColor color;
    };

    QVector<ViewInfo> views = {
        { "quadGLTopLeft_axial", Qt::red },
        { "quadGLTopRight_sagittal", Qt::blue },
        { "quadGLBottomLeft_coronal", Qt::green },
        { "quadGLBottomRight_3D", Qt::yellow }
    };

    for (const auto& view : views) {
        QWidget* placeholder = ui.centralWidget->findChild<QWidget*>(view.placeholderName);
        if (placeholder) {
            auto* glView = new MyGLView(view.color, placeholder->parentWidget());
            glView->setObjectName(view.placeholderName);
            glView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            glView->setMinimumSize(0, 0);

            if (auto* layout = placeholder->parentWidget()->layout()) {
                layout->removeWidget(placeholder);
                layout->addWidget(glView);
            }

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

