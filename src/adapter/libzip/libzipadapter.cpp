#include "libzipadapter.h"

#ifdef USE_LIBZIP

#include <zip.h>

LibZipAdapter::LibZipAdapter()
{

}

Archive *LibZipAdapter::ReadArchiveInternal(const QString &filename)
{
  int err;

  zip_t *zip = zip_open(filename.toLocal8Bit(), ZIP_RDONLY, &err);

  if (!zip) {
    return nullptr;
  }

  zip_int64_t nb_entries = zip_get_num_entries(zip, ZIP_FL_UNCHANGED);

  Archive *archive = new Archive();

  for (zip_int64_t i=0; i<nb_entries; i++) {
    const char *name = zip_get_name(zip, i, 0);
    QString file_full_path = QString::fromUtf8(name);
    QStringList path_split = file_full_path.split('/');

    Item *folder = archive->GetRoot();
    for (int j=0; j<path_split.size(); j++) {
      const QString &this_entry_name = path_split.at(j);

      if (j == path_split.size() - 1) {
        // Last entry, this must be the actual file
        if (!this_entry_name.isEmpty()) {
          zip_stat_t stat;
          zip_stat(zip, name, 0, &stat);

          folder->AddChild(new Item(Item::kFile, this_entry_name, stat.size));
        }
      } else {
        Item *subfolder = folder->GetChildWithName(this_entry_name);

        if (!subfolder) {
          subfolder = new Item(Item::kFolder, this_entry_name);
          folder->AddChild(subfolder);
        }

        folder = subfolder;
      }
    }
  }

  zip_close(zip);

  return archive;
}


#endif
