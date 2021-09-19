#include "item.h"

Item::Item(Type type, const QString &filename, qint64 size, const QVariant &id) :
  parent_(nullptr),
  type_(type),
  filename_(filename),
  file_size_(size),
  id_(id)
{
}
