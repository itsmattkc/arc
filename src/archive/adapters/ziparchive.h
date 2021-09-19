#ifndef ZIPARCHIVE_H
#define ZIPARCHIVE_H

#ifdef USE_LIBZIP

#include <zip.h>

#include "archive/archive.h"

class ZipArchive : public Archive
{
public:
  ZipArchive() = default;

  ARCHIVE_DEFAULT_DESTRUCTOR(ZipArchive)

protected:
  virtual bool OpenInternal(const QString &filename) override;

  virtual bool ExtractInternal(const Item *item, const QString &output_filename) override;

  virtual void CloseInternal() override;

private:
  zip_t *zip_;

};

#endif // USE_LIBZIP

#endif // ZIPARCHIVE_H
