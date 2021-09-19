#include "mainwindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QMessageBox>
#include <QSplitter>
#include <QToolBar>

#include "archive/adapters/ziparchive.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent),
    archive_(nullptr)
{
  //setWindowState(Qt::WindowMaximized);

  QMenuBar *menu_bar = new QMenuBar();
  setMenuBar(menu_bar);

  QToolBar *toolbar = new QToolBar(this);
  QAction *toolbar_new_action = toolbar->addAction(tr("New"));
  toolbar_new_action->setToolTip(tr("Create new archive"));
  connect(toolbar_new_action, &QAction::triggered, this, &MainWindow::NewArchive);
  QAction *toolbar_open_action = toolbar->addAction(tr("Open"));
  toolbar_open_action->setToolTip(tr("Open existing archive"));
  connect(toolbar_open_action, &QAction::triggered, this, &MainWindow::OpenArchive);
  QAction *toolbar_save_action = toolbar->addAction(tr("Save"));
  toolbar_save_action->setToolTip(tr("Save current archive"));
  toolbar->addSeparator();
  QAction *toolbar_type_action = toolbar->addAction(tr("ZIP"));
  toolbar_type_action->setToolTip(tr("Change archive type"));
  QAction *toolbar_add_action = toolbar->addAction(tr("Add"));
  toolbar_add_action->setToolTip(tr("Add files to archive"));
  QAction *toolbar_extract_action = toolbar->addAction(tr("Extract"));
  toolbar_extract_action->setToolTip(tr("Extract selected files from archive"));
  connect(toolbar_extract_action, &QAction::triggered, this, &MainWindow::ExtractSelected);
  addToolBar(toolbar);

  QMenu *file_menu = menu_bar->addMenu(tr("&File"));
  file_menu->addAction(tr("&New"), this, &MainWindow::NewArchive, QStringLiteral("Ctrl+N"));
  file_menu->addAction(tr("&Open"), this, &MainWindow::OpenArchive, QStringLiteral("Ctrl+O"));
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
  dir_tree_->setDragEnabled(true);
  splitter->addWidget(dir_tree_);

  file_tree_ = new QTreeView();
  file_model_ = new FileViewSortFilterProxyModel(this);
  file_tree_->setModel(file_model_);
  file_tree_->setSortingEnabled(true);
  file_tree_->sortByColumn(0, Qt::AscendingOrder);
  file_tree_->setItemsExpandable(false);
  file_tree_->setRootIsDecorated(false);
  file_tree_->setDragEnabled(true);
  file_tree_->setSelectionMode(QAbstractItemView::ExtendedSelection);
  splitter->addWidget(file_tree_);

  connect(dir_tree_, &QTreeView::clicked, this, &MainWindow::DirViewSelectionChanged);
  connect(file_tree_, &QTreeView::activated, this, &MainWindow::FileViewFolderActivated);

  splitter->setSizes({splitter->width()/4, splitter->width()/4*3});

  // Set up initial project
  NewArchive();
}

void MainWindow::SetArchive(Archive *archive)
{
  dir_model_->setSourceModel(archive);
  file_model_->setSourceModel(archive);
  dir_tree_->expand(dir_model_->mapFromSource(archive->GetRootIndex()));
  file_tree_->setRootIndex(file_model_->mapFromSource(archive->GetRootIndex()));
  setWindowTitle(tr("Arc - %1").arg(archive->GetRoot()->GetFilename()));
  archive_.reset(archive);
}

void MainWindow::NewArchive()
{
  SetArchive(new ZipArchive());
}

void MainWindow::OpenArchive()
{
  QString s = QFileDialog::getOpenFileName(this, tr("Open Archive"));

  if (!s.isEmpty()) {
    Archive *archive = Archive::OpenArchive(s);
    if (archive) {
      SetArchive(archive);
    } else {
      QMessageBox::critical(this, tr("Failed to open archive"),
                            tr("Arc is not compatible with this type of archive"));
    }
  }
}

void MainWindow::ExtractItem(ErrorDialog *ed, const QString &destination, Item *item)
{
  QDir dest_dir(destination);
  QString child_filename = dest_dir.filePath(item->GetFilename());

  if (item->type() == Item::kFolder) {
    QDir child_dir(child_filename);
    if (child_dir.mkpath(QStringLiteral("."))) {
      // Recursively follow directory
      foreach (Item *grandchild, item->GetChildren()) {
        ExtractItem(ed, child_filename, grandchild);
      }
    } else {
      ed->AddEntry(ErrorDialog::kError, tr("Failed to create directory: %1").arg(child_filename));
    }
  } else if (item->type() == Item::kFile) {
    archive_->Extract(item, child_filename);
  }
}

void MainWindow::ExtractSelected()
{
  QModelIndexList selected_rows = file_tree_->selectionModel()->selectedRows();

  if (selected_rows.isEmpty()) {
    QMessageBox::critical(this, tr("Failed to extract"), tr("No files selected to extract."));
    return;
  }

  QString dir = QFileDialog::getExistingDirectory(this, tr("Extract Files"));

  if (!dir.isEmpty()) {
    ErrorDialog ed(this);

    foreach (QModelIndex selected_index, selected_rows) {
      Item *selected_item = static_cast<Item *>(file_model_->mapToSource(selected_index).internalPointer());

      ExtractItem(&ed, dir, selected_item);
    }

    if (ed.GetEntryCount()) {
      ed.exec();
    }
  }
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

