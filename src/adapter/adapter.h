#ifndef ADAPTER_H
#define ADAPTER_H

#include "archive/archive.h"

class Adapter
{
public:
  Adapter();

  virtual QString id() = 0;

  static void InitializeAdapters();

  static void DestroyAdapters();

  static Archive *ReadArchive(const QString &filename);

  static bool WriteArchive(const QString &filename, const Archive *archive);

  static QByteArray ExtractFile(const QString &adapter, const Item *item);

  static Adapter *GetAdapterFromID(const QString &id);

protected:
  virtual Archive *ReadArchiveInternal(const QString &filename) = 0;

  virtual QByteArray ExtractFileInternal(const Item *item) = 0;

private:
  static QVector<Adapter*> adapters_;

};

#endif // ADAPTER_H
