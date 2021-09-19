#ifndef ITEM_H
#define ITEM_H

#include <QString>
#include <QVariant>
#include <QVector>

class Item
{
public:
  enum Type {
    kFile,
    kFolder
  };

  Item(Type type = kFile, const QString &filename = QString(), qint64 size = 0, const QVariant &id = QVariant());

  virtual ~Item()
  {
    qDeleteAll(children_);
  }

  // Disable copy/move
  Item(const Item&) = delete;
  Item(Item&&) = delete;
  Item& operator=(const Item&) = delete;
  Item& operator=(Item&&) = delete;

  Type type() const
  {
    return type_;
  }

  void AddChild(Item *child)
  {
    // If child parent is already this, disregard
    if (child->parent_ == this) {
      return;
    }

    // If child belongs to another parent, remove it from that parent first
    if (child->parent_) {
      child->parent_->RemoveChild(child);
    }

    children_.append(child);
    child->parent_ = this;
  }

  void RemoveChild(Item *child)
  {
    if (child->parent_ != this) {
      return;
    }

    RemoveChild(GetIndexOfChild(child));
  }

  void RemoveChild(int index)
  {
    // Remove child from vector
    Item *child = children_.takeAt(index);

    // Set parent to null
    child->parent_ = nullptr;
  }

  int GetIndexOfChild(Item *child) const
  {
    return children_.indexOf(child);
  }

  Item *GetChild(int index) const
  {
    return children_.at(index);
  }

  Item *GetParent() const
  {
    return parent_;
  }

  int GetChildCount() const
  {
    return children_.size();
  }

  const QVector<Item *>& GetChildren() const
  {
    return children_;
  }

  const QString &GetFilename() const
  {
    return filename_;
  }

  void SetFilename(const QString &filename)
  {
    filename_ = filename;
  }

  qint64 GetFileSize() const
  {
    return file_size_;
  }

  Item *GetChildWithName(const QString& name) const
  {
    foreach (Item *c, children_) {
      if (c->GetFilename() == name) {
        return c;
      }
    }

    return nullptr;
  }

  const QVariant &id() const
  {
    return id_;
  }

private:
  Item *parent_;

  Type type_;

  QString filename_;

  QString external_path_;

  QVector<Item *> children_;

  qint64 file_size_;

  QVariant id_;

};

#endif // ITEM_H
