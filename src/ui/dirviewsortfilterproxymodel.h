#ifndef DIRVIEWSORTFILTERPROXYMODEL_H
#define DIRVIEWSORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class DirViewSortFilterProxyModel : public QSortFilterProxyModel
{
public:
  DirViewSortFilterProxyModel(QObject *parent = nullptr);

  virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;

protected:
  virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

};

#endif // DIRVIEWSORTFILTERPROXYMODEL_H
