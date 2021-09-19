#include "ziparchive.h"

#ifdef USE_LIBZIP

#include <QFile>

bool ZipArchive::OpenInternal(const QString &filename)
{
  int err;

  zip_ = zip_open(filename.toLocal8Bit(), ZIP_RDONLY, &err);

  if (!zip_) {
    return false;
  }

  // Get number of "entries"
  zip_int64_t nb_entries = zip_get_num_entries(zip_, ZIP_FL_UNCHANGED);

  // Loop through entries for files and folders
  for (zip_int64_t i=0; i<nb_entries; i++) {

    // Get entry name and separate it into folders (delineated by '/')
    const char *name = zip_get_name(zip_, i, 0);
    QString file_full_path = QString::fromUtf8(name);
    QStringList path_split = file_full_path.split('/');

    // Find folder
    Item *folder = GetRoot();
    for (int j=0; j<path_split.size(); j++) {
      const QString &this_entry_name = path_split.at(j);

      if (j == path_split.size() - 1) {
        // Last entry, this must be the actual file
        if (!this_entry_name.isEmpty()) {
          zip_stat_t stat;
          zip_stat(zip_, name, 0, &stat);

          folder->AddChild(new Item(Item::kFile, this_entry_name, stat.size, i));
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

  return true;
}

bool ZipArchive::ExtractInternal(const Item *item, const QString &output_filename)
{
  QFile out(output_filename);
  if (!out.open(QFile::WriteOnly)) {
    return false;
  }

  zip_int64_t entry = item->id().value<zip_int64_t>();

  zip_stat_t stat;
  if (zip_stat_index(zip_, entry, 0, &stat) != 0) {
    return false;
  }

  zip_file_t *file = zip_fopen_index(zip_, entry, 0);
  if (!file) {
    return false;
  }

  QByteArray buffer(1048576, Qt::Uninitialized);
  zip_uint64_t written = 0;
  while (written < stat.size) {
    zip_int64_t read_count = zip_fread(file, buffer.data(), buffer.size());
    out.write(buffer.data(), read_count);
    written += read_count;
  }

  zip_fclose(file);

  out.close();

  return true;
}

void ZipArchive::CloseInternal()
{
  zip_close(zip_);
}

#endif // USE_LIBZIP
