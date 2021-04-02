#ifndef ADAPTER_H
#define ADAPTER_H

#include "archive/archive.h"

class Adapter
{
public:
  Adapter();

  static void InitializeAdapters();

  static void DestroyAdapters();

  static Archive *ReadArchive(const QString &filename);

  static bool WriteArchive(const QString &filename, const Archive *archive);

  static QByteArray ExtractFile(const Item *item);

protected:
  virtual Archive *ReadArchiveInternal(const QString &filename) = 0;

private:
  static QVector<Adapter*> adapters_;

};

#endif // ADAPTER_H
