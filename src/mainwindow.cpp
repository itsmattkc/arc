#include "mainwindow.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QSplitter>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent),
    archive_(nullptr)
{
  setWindowTitle(QStringLiteral("Arc"));
  //setWindowState(Qt::WindowMaximized);

  QMenuBar *menu_bar = new QMenuBar();
  setMenuBar(menu_bar);

  QMenu *file_menu = menu_bar->addMenu(tr("&File"));
  file_menu->addAction(tr("&New"));
  file_menu->addAction(tr("&Open"));
  file_menu->addAction(tr("&Save"));
  file_menu->addAction(tr("Save &As"));
  file_menu->addSeparator();
  QAction *file_exit_action = file_menu->addAction(tr("E&xit"));
  connect(file_exit_action, &QAction::triggered, this, &MainWindow::close);

  QSplitter *splitter = new QSplitter();
  setCentralWidget(splitter);

  dir_tree_ = new QTreeView();
  dir_model_ = new DirViewSortFilterProxyModel(this);
  dir_tree_->setSortingEnabled(true);
  dir_tree_->setModel(dir_model_);
  dir_tree_->sortByColumn(0, Qt::AscendingOrder);
  splitter->addWidget(dir_tree_);

  file_tree_ = new QTreeView();
  file_model_ = new FileViewSortFilterProxyModel(this);
  file_tree_->setModel(file_model_);
  file_tree_->setSortingEnabled(true);
  file_tree_->sortByColumn(0, Qt::AscendingOrder);
  file_tree_->setItemsExpandable(false);
  file_tree_->setRootIsDecorated(false);
  splitter->addWidget(file_tree_);

  connect(dir_tree_, &QTreeView::clicked, this, &MainWindow::DirViewSelectionChanged);
  connect(file_tree_, &QTreeView::activated, this, &MainWindow::FileViewFolderActivated);

  splitter->setSizes({splitter->width()/4, splitter->width()/4*3});

  // Set up initial project
  NewArchive();
}

MainWindow::~MainWindow()
{
}

void MainWindow::SetArchive(Archive *archive)
{
  archive_ = archive;
  dir_model_->setSourceModel(archive_);
  file_model_->setSourceModel(archive_);
  dir_tree_->expand(dir_model_->mapFromSource(archive->GetRootIndex()));
  file_tree_->setRootIndex(file_model_->mapFromSource(archive->GetRootIndex()));
}

void MainWindow::NewArchive()
{
  Archive *new_archive = new Archive();
  SetArchive(new_archive);
}

void MainWindow::DirViewSelectionChanged(const QModelIndex &index)
{
  file_tree_->setRootIndex(file_model_->mapFromSource(dir_model_->mapToSource(index)));
}

void MainWindow::FileViewFolderActivated(const QModelIndex &index)
{
  QModelIndex source_index = file_model_->mapToSource(index);

  if (static_cast<Item*>(source_index.internalPointer())->type() == Item::kFolder) {
    file_tree_->setRootIndex(index);

    QModelIndex dir_index = dir_model_->mapFromSource(source_index);
    dir_tree_->selectionModel()->select(dir_index, QItemSelectionModel::ClearAndSelect);

    // Expand entire tree to get to this folder
    QModelIndex parent_index = dir_index;
    do {
      dir_tree_->expand(parent_index);
      parent_index = dir_model_->parent(parent_index);
    } while (parent_index.isValid());
  }
}

