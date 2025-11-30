#include <quadwidgets.h>
#include <glview.h>
#include <context.h>
#include <QStandardItemModel>
#include <qmimedata.h>

Context context(R"(D:\Files\Cesars\Scissors_Test 2025-7-2 15-11-21.uint16_scv)");
//Context context(R"(D:\Files\CTLab\SaveVolumeToFile\volumeHeader.uint16_scv)");

QuadWidgets::QuadWidgets(QWidget *parent)
    : QMainWindow(parent)
{
    setAcceptDrops(true);

    
    context.volumeData.saveHeaderToFile("volumeHeader.txt");
    context.volumeData.fillBuffer();

    ui.setupUi(this);

    ui.quad_horizontal_top_splitter->setSizes({ 100, 100 });
    ui.quad_horizontal_bottom_splitter->setSizes({ 100, 100 });
    ui.quad_splitter->setSizes({ 100, 100 });

    populateTree();

    enum QUAD_VIEW
    {
        XY = 0,
        XZ = 1,
        YZ = 2,
        V3D = 3

    };

    // Replace placeholders with MyGLView instances
    struct ViewInfo {
        QString placeholderName;
        QColor color;
        QUAD_VIEW view;
    };

    

    QVector<ViewInfo> views = {
        { "quadGLTopLeft_xy", Qt::blue, QUAD_VIEW::XY},
        { "quadGLTopRight_xz", Qt::green, QUAD_VIEW::XZ},
        { "quadGLBottomLeft_yz", Qt::red, QUAD_VIEW::YZ},
        { "quadGLBottomRight_3D", Qt::yellow, QUAD_VIEW::V3D}
    };

    QSurfaceFormat format;
    format.setAlphaBufferSize(8); // crucial!
    format.setRenderableType(QSurfaceFormat::OpenGL);
    QSurfaceFormat::setDefaultFormat(format);

    for (const auto& view : views) {
        QWidget* placeholder = ui.centralWidget->findChild<QWidget*>(view.placeholderName);

        GLView* glView = nullptr;
        
        if (placeholder) {
            if (view.view == QUAD_VIEW::XY)
            {
                glViewQuadXY = new GLViewQuadXY(view.color, placeholder->parentWidget(), &context);
                glView = (GLView*)glViewQuadXY;
            }
            else if (view.view == QUAD_VIEW::XZ)
            {
                glViewQuadXZ = new GLViewQuadXZ(view.color, placeholder->parentWidget(), &context);
                glView = (GLView*)glViewQuadXZ;
            }
            else if (view.view == QUAD_VIEW::YZ)
            {
                glViewQuadYZ = new GLViewQuadYZ(view.color, placeholder->parentWidget(), &context);
                glView = (GLView*)glViewQuadYZ;
            }
            else if (view.view == QUAD_VIEW::V3D)
            {
                glViewQuad3d = new GLViewQuad3D(view.color, placeholder->parentWidget(), &context);
                glView = (GLView*)glViewQuad3d;
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

    connect(ui.minVoxelValueSlider,
        &QSlider::valueChanged, this,
        [this](int value)
            { 
                ui.minVoxelValueLabel->setText(QString::number(value));
            
                if (ui.maxVoxelValueSlider->value() < value)
                {
                    ui.maxVoxelValueSlider->setValue(value + 1);
                }

                glViewQuad3d->UpdateMinMaxVoxelValues(value, ui.maxVoxelValueSlider->value());
                glViewQuadXY->UpdateMinMaxVoxelValues(value, ui.maxVoxelValueSlider->value());
                glViewQuadYZ->UpdateMinMaxVoxelValues(value, ui.maxVoxelValueSlider->value());
                glViewQuadXZ->UpdateMinMaxVoxelValues(value, ui.maxVoxelValueSlider->value());
            }
        );

    ui.minVoxelValueSlider->setValue(0);

    connect(ui.maxVoxelValueSlider,
        &QSlider::valueChanged, this,
        [this](int value)
        {
            ui.maxVoxelValueLabel->setText(QString::number(value));

            if (ui.minVoxelValueSlider->value() > value)
            {
                ui.minVoxelValueSlider->setValue(value - 1);
            }

            glViewQuad3d->UpdateMinMaxVoxelValues(ui.minVoxelValueSlider->value(), value);
            glViewQuadXY->UpdateMinMaxVoxelValues(ui.minVoxelValueSlider->value(), value);
            glViewQuadYZ->UpdateMinMaxVoxelValues(ui.minVoxelValueSlider->value(), value);
            glViewQuadXZ->UpdateMinMaxVoxelValues(ui.minVoxelValueSlider->value(), value);
        }
    );

    ui.maxVoxelValueSlider->setValue(std::numeric_limits<unsigned short>::max());
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

void QuadWidgets::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void QuadWidgets::dropEvent(QDropEvent* event)
{
    for (const QUrl& url : event->mimeData()->urls()) {
        QString path = url.toLocalFile();
        
    }
}

