// treeviewmodel.h
#pragma once
#include <QStandardItemModel>

class TreeViewModel : public QStandardItemModel
{
    Q_OBJECT
public:
    TreeViewModel(QObject* parent = nullptr) : QStandardItemModel(parent)
    {
        // Create root items with children
        QStandardItem* root1 = new QStandardItem("Root Item 1");
        root1->appendRow(new QStandardItem("Child 1.1"));
        QStandardItem* child12 = new QStandardItem("Child 1.2");
        child12->appendRow(new QStandardItem("Grandchild 1.2.1"));
        child12->appendRow(new QStandardItem("Grandchild 1.2.2"));
        root1->appendRow(child12);
        root1->appendRow(new QStandardItem("Child 1.3"));

        QStandardItem* root2 = new QStandardItem("Root Item 2");
        root2->appendRow(new QStandardItem("Child 2.1"));
        root2->appendRow(new QStandardItem("Child 2.2"));

        QStandardItem* root3 = new QStandardItem("Root Item 3 (No Children)");

        // Add roots to model
        appendRow(root1);
        appendRow(root2);
        appendRow(root3);
    }
};
