#ifndef LIBZIPADAPTER_H
#define LIBZIPADAPTER_H

#ifdef USE_LIBZIP

#include "adapter/adapter.h"

class LibZipAdapter : public Adapter
{
public:
  LibZipAdapter();

protected:
  virtual Archive *ReadArchiveInternal(const QString &filename) override;

};

#endif

#endif // LIBZIPADAPTER_H
