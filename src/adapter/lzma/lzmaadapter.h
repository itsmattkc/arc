#ifndef LZMAADAPTER_H
#define LZMAADAPTER_H

#ifdef USE_LZMA

#include "adapter/adapter.h"

class LZMAAdapter : public Adapter
{
public:
  LZMAAdapter();

  virtual QString id() override
  {
    return QStringLiteral("lzma");
  }

protected:
  virtual Archive *ReadArchiveInternal(const QString &filename) override;

};

#endif

#endif // LZMAADAPTER_H
