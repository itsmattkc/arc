#include "lzmaadapter.h"

#ifdef USE_LZMA

#include <lzma.h>

LZMAAdapter::LZMAAdapter()
{
}

Archive *LZMAAdapter::ReadArchiveInternal(const QString &filename)
{
  Archive *archive = nullptr;

  Q_UNUSED(filename)

  return archive;
}

#endif
