#include <quadwidgets.h>
#include <glview.h>
#include <context.h>
#include <QStandardItemModel>


QuadWidgets::QuadWidgets(QWidget *parent)
    : QMainWindow(parent)
{

    Context context;

    ui.setupUi(this);

    ui.quad_horizontal_top_splitter->setSizes({ 100, 100 });
    ui.quad_horizontal_bottom_splitter->setSizes({ 100, 100 });
    ui.quad_splitter->setSizes({ 100, 100 });

    populateTree();

    enum QUAD_VIEW
    {
        AXIAL = 0,
        SAGITTAL = 1,
        CORONAL = 2,
        V3D = 3

    };

    // Replace placeholders with MyGLView instances
    struct ViewInfo {
        QString placeholderName;
        QColor color;
        QUAD_VIEW view;
    };

    

    QVector<ViewInfo> views = {
        { "quadGLTopLeft_axial", Qt::red, QUAD_VIEW::AXIAL},
        { "quadGLTopRight_sagittal", Qt::blue, QUAD_VIEW::SAGITTAL},
        { "quadGLBottomLeft_coronal", Qt::green, QUAD_VIEW::CORONAL},
        { "quadGLBottomRight_3D", Qt::yellow, QUAD_VIEW::V3D}
    };

    for (const auto& view : views) {
        QWidget* placeholder = ui.centralWidget->findChild<QWidget*>(view.placeholderName);

        GLView* glView = nullptr;
        
        if (placeholder) {
            if (view.view == QUAD_VIEW::AXIAL)
            {
                context.glViewQuadAxial = new GLViewQuadAxial(view.color, placeholder->parentWidget());
                glView = (GLView*)context.glViewQuadAxial;
            }
            else if (view.view == QUAD_VIEW::SAGITTAL)
            {
                context.glViewQuadSagittal = new GLViewQuadSagittal(view.color, placeholder->parentWidget());
                glView = (GLView*)context.glViewQuadSagittal;
            }
            else if (view.view == QUAD_VIEW::CORONAL)
            {
                context.glViewQuadCoronal = new GLViewQuadCoronal(view.color, placeholder->parentWidget());
                glView = (GLView*)context.glViewQuadCoronal;
            }
            else if (view.view == QUAD_VIEW::V3D)
            {
                context.glViewQuad3d = new GLViewQuad3D(view.color, placeholder->parentWidget());
                glView = (GLView*)context.glViewQuad3d;
            }
            else
                throw std::exception("view doesn't exists");

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

