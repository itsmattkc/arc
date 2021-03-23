#ifndef ARCHIVE_H
#define ARCHIVE_H

#include <QAbstractItemModel>

#include "item.h"

class Archive : public QAbstractItemModel
{
public:
  Archive();

  virtual ~Archive() override
  {
    delete root_;
  }

  enum Column {
    kColumnName,
    kColumnSize,
    kColumnType,
    kColumnCount
  };

  // Disable copy/move
  Archive(const Archive&) = delete;
  Archive(Archive&&) = delete;
  Archive& operator=(const Archive&) = delete;
  Archive& operator=(Archive&&) = delete;

  virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
  virtual QModelIndex parent(const QModelIndex &child) const override;
  virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

  virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

  Item *GetRoot() const
  {
    return root_;
  }

  QModelIndex GetRootIndex(int column = 0) const
  {
    return createIndex(0, column, root_);
  }

private:
  Item *root_;

};

#endif // ARCHIVE_H