#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeView>

#include "archive/archive.h"
#include "ui/dirviewsortfilterproxymodel.h"
#include "ui/errordialog.h"
#include "ui/fileviewsortfilterproxymodel.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  void SetArchive(Archive* archive);

  void ExtractItem(ErrorDialog *ed, const QString &destination, Item *item);

  Archive *archive_;

  QTreeView *dir_tree_;
  QTreeView *file_tree_;

  DirViewSortFilterProxyModel* dir_model_;
  FileViewSortFilterProxyModel* file_model_;

private slots:
  void NewArchive();

  void OpenArchive();

  void ExtractSelected();

  void DirViewSelectionChanged(const QModelIndex &index);

  void FileViewFolderActivated(const QModelIndex &index);

};

#endif // MAINWINDOW_H
