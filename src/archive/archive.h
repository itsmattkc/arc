#ifndef ARCHIVE_H
#define ARCHIVE_H

#include <memory>
#include <QAbstractItemModel>

#include "item.h"

#define ARCHIVE_DEFAULT_DESTRUCTOR(x) \
  virtual ~x() override {Close();}

class Archive : public QAbstractItemModel
{
public:
  Archive();

  ARCHIVE_DEFAULT_DESTRUCTOR(Archive)

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

  static Archive *OpenArchive(const QString &filename);

  bool Open(const QString &filename);
  bool Extract(const Item *item, const QString &filename);
  void Close();

  virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
  virtual QModelIndex parent(const QModelIndex &child) const override;
  virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

  virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

  // Enable drag/drop
  virtual Qt::ItemFlags flags(const QModelIndex& index) const override;
  virtual QStringList mimeTypes() const override;
  virtual QMimeData *mimeData(const QModelIndexList &indexes) const override;

  Item *GetRoot() const
  {
    return root_.get();
  }

  QModelIndex GetRootIndex(int column = 0) const
  {
    return createIndex(0, column, root_.get());
  }

protected:
  virtual bool OpenInternal(const QString &filename) = 0;

  virtual bool ExtractInternal(const Item *item, const QString &output_filename) = 0;

  virtual void CloseInternal() = 0;

private:
  void ResetRoot();

  std::unique_ptr<Item> root_;

  bool open_;

};

#endif // ARCHIVE_H
