#include "dirviewsortfilterproxymodel.h"

#include "archive/archive.h"
#include "archive/item.h"

#define super QSortFilterProxyModel

DirViewSortFilterProxyModel::DirViewSortFilterProxyModel(QObject *parent) :
  QSortFilterProxyModel(parent)
{
}

int DirViewSortFilterProxyModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent)

  return 1;
}

bool DirViewSortFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
  if (source_parent.isValid()) {
    Item *parent = static_cast<Item *>(source_parent.internalPointer());
    return (parent->GetChild(source_row)->type() == Item::kFolder);
  } else {
    return true;
  }
}
