#include "archive.h"

#include <QApplication>
#include <QFileIconProvider>

Archive::Archive()
{
  root_ = new Item(Item::kFolder, tr("(untitled)"));
}

QModelIndex Archive::index(int row, int column, const QModelIndex &parent) const
{
  if (parent.isValid()) {
    return createIndex(row, column, static_cast<Item *>(parent.internalPointer())->GetChild(row));
  } else {
    return GetRootIndex(column);
  }
}

QModelIndex Archive::parent(const QModelIndex &child) const
{
  Item* item = static_cast<Item *>(child.internalPointer());

  if (item == root_) {
    return QModelIndex();
  }

  Item* par = item->GetParent();

  int parent_index;

  if (par == root_) {
    parent_index = 0;
  } else{
    parent_index = par->GetParent()->GetIndexOfChild(par);
  }

  // If the parent isn't root, it must have a parent too which will give us an index
  return createIndex(parent_index, child.column(), par);
}

int Archive::rowCount(const QModelIndex &parent) const
{
  if (!parent.isValid()) {
    // The root
    return 1;
  }

  return static_cast<Item *>(parent.internalPointer())->GetChildCount();
}

int Archive::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent)

  return kColumnCount;
}

QVariant Archive::data(const QModelIndex &index, int role) const
{
  Item *item = static_cast<Item*>(index.internalPointer());

  switch (index.column()) {
  case kColumnName:
    switch (role) {
    case Qt::DisplayRole:
      return item->GetFilename();
    case Qt::DecorationRole:
    {
      QFileIconProvider icon_provider;

      if (item->type() == Item::kFolder) {
        return icon_provider.icon(QFileIconProvider::Folder);
      } else {
        return icon_provider.icon(QFileIconProvider::File);
      }
    }
    }

    break;
  case kColumnSize:
    if (role == Qt::DisplayRole && item->type() == Item::kFile) {
      return QString::number(item->GetFileSize());
    }
    break;
  case kColumnType:
    if (role == Qt::DisplayRole) {
      if (item->type() == Item::kFolder) {
        return tr("Folder");
      } else {
        return QFileIconProvider().type(item->GetFilename());
      }
    }
    break;
  }

  return QVariant();
}

QVariant Archive::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
    switch (section) {
    case kColumnName:
      return tr("Name");
    case kColumnSize:
      return tr("Size");
    case kColumnType:
      return tr("Type");
    }
  }

  return QVariant();
}
