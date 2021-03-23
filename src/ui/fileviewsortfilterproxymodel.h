#ifndef FILEVIEWSORTFILTERPROXYMODEL_H
#define FILEVIEWSORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class FileViewSortFilterProxyModel : public QSortFilterProxyModel
{
public:
  FileViewSortFilterProxyModel(QObject *parent = nullptr);

};

#endif // FILEVIEWSORTFILTERPROXYMODEL_H
