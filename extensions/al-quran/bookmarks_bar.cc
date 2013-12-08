#include "bookmarks_bar.h"
#include "ui_bookmarks_bar.h"
#include <QStandardItemModel>
#include <QMenu>
#include <QAction>
#include "core/settings_loader.h"
#include "core/memory.h"
#include "core/quran/chapter.h"

BookmarksBar::BookmarksBar(const QString& settingsKeyPrefix, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BookmarksBar),
    m_contextMenu(nullptr),
    m_model(nullptr),
    m_settingsKeyPrefix(settingsKeyPrefix)
{
    ui->setupUi(this);
    
    m_model = new QStandardItemModel(this);
    m_model->setColumnCount(2);
    m_model->setHorizontalHeaderLabels(QStringList() << "Name" << "Location");
    ui->treeBookmarks->setModel(m_model);
    
    ui->treeBookmarks->setContextMenuPolicy(Qt::CustomContextMenu);
    
    // Load bookmarks
    QString bookmarksStr = SettingsLoader().get(m_settingsKeyPrefix + "bookmarks").toString();
    if (!bookmarksStr.isEmpty()) {
        QStringList allBookmarksStr = bookmarksStr.split(",");
        for (QString bookmarkStr : allBookmarksStr) {
            Bookmark bm;
            if (bm.deserialize(bookmarkStr)) {
                LOG(INFO) << "Loading bookmark [" << bm << "]";
                m_bookmarks.push_back(bm);
                
                QStandardItem* bookmarkStandardItemName = new QStandardItem(bm.name());
                QString location = bm.serialize().mid(bm.name().length() + 1 /* name= */);
                QStandardItem* bookmarkStandardItemLocation = new QStandardItem(location);
                bookmarkStandardItemName->setData(bm.serialize());
                bookmarkStandardItemName->setFlags(bookmarkStandardItemName->flags() & ~Qt::ItemIsEditable);
                bookmarkStandardItemLocation->setFlags(bookmarkStandardItemLocation->flags() & ~Qt::ItemIsEditable);
                
                int currRow = m_model->rowCount();
                m_model->setItem(currRow, 0, bookmarkStandardItemName);
                m_model->setItem(currRow, 1, bookmarkStandardItemLocation);
            }
        }
    }
    QObject::connect(ui->treeBookmarks, SIGNAL(doubleClicked(QModelIndex)),
                     this, SLOT(onSelectionChanged(QModelIndex)));
    QObject::connect(ui->treeBookmarks, SIGNAL(customContextMenuRequested(QPoint)),
                     this, SLOT(onCustomContextMenuRequested(QPoint)));
    m_contextMenu = new QMenu();
    QAction* actionOpen = m_contextMenu->addAction("Open");
    QObject::connect(actionOpen, SIGNAL(triggered()), this, SLOT(openSelected()));
    QAction* actionEdit = m_contextMenu->addAction("Edit");
    QObject::connect(actionEdit, SIGNAL(triggered()), this, SLOT(editSelected()));
    QAction* actionDelete = m_contextMenu->addAction("Delete");
    QObject::connect(actionDelete, SIGNAL(triggered()), this, SLOT(deleteSelected()));
    m_contextMenu->hide();
}

BookmarksBar::~BookmarksBar()
{
    // Save
    QString serializedForm;
    int i = 0;
    for (Bookmark bm : m_bookmarks) {
        serializedForm.append(bm.serialize());
        if (++i != m_bookmarks.size()) {
            serializedForm.append(",");
        }
    }
    SettingsLoader().saveSettings(m_settingsKeyPrefix + "bookmarks", serializedForm);
    memory::deleteAll(m_contextMenu, ui);
}

void BookmarksBar::onSelectionChanged(const QModelIndex& modelIndex)
{
    QVector<Bookmark>::Iterator bmIter = 
            std::find_if(m_bookmarks.begin(), m_bookmarks.end(), [&](const Bookmark& b) {
        QString serialized = modelIndex.sibling(modelIndex.row(), 0).data().toString() + "=" +
                modelIndex.sibling(modelIndex.row(), 1).data().toString();
        return b.serialize() == serialized;
    });
    if (bmIter != m_bookmarks.end()) {
        emit selectionChanged(bmIter);
    }
}

void BookmarksBar::onCustomContextMenuRequested(const QPoint& pos)
{
    int x = pos.x();
    int y = pos.y();
    x += parentWidget()->pos().x() + 75;
    y += parentWidget()->pos().y() + 150;
    m_contextMenu->move(x, y);
    m_contextMenu->show();
}

void BookmarksBar::openSelected()
{
}

void BookmarksBar::editSelected()
{
    
}

void BookmarksBar::deleteSelected()
{
    
}