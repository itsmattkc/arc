#include "adapter.h"

#include <QFileInfo>

#include "adapter/libzip/libzipadapter.h"
#include "adapter/lzma/lzmaadapter.h"

QVector<Adapter*> Adapter::adapters_;

Adapter::Adapter()
{
}

void Adapter::InitializeAdapters()
{
#ifdef USE_LIBZIP
  adapters_.append(new LibZipAdapter());
#endif
#ifdef USE_LZMA
  adapters_.append(new LZMAAdapter());
#endif
}

void Adapter::DestroyAdapters()
{
  qDeleteAll(adapters_);
  adapters_.clear();
}

Archive *Adapter::ReadArchive(const QString &filename)
{
  foreach (Adapter* a, adapters_) {
    Archive *attempt = a->ReadArchiveInternal(filename);
    if (attempt) {
      attempt->SetAdapter(a->id());
      attempt->GetRoot()->SetFilename(QFileInfo(filename).fileName());
      return attempt;
    }
  }

  return nullptr;
}

QByteArray Adapter::ExtractFile(const QString &adapter_id, const Item *item)
{
  Adapter *adapter = GetAdapterFromID(adapter_id);

  if (!adapter) {
    return QByteArray();
  }

  return adapter->ExtractFileInternal(item);
}

Adapter *Adapter::GetAdapterFromID(const QString &id)
{
  foreach (Adapter *a, adapters_) {
    if (a->id() == id) {
      return a;
    }
  }

  return nullptr;
}
