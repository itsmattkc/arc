#include "item.h"

Item::Item(Type type, const QString &filename) :
  parent_(nullptr),
  type_(type),
  filename_(filename)
{
}
