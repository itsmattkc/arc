#ifndef ARCHIVETEST_H
#define ARCHIVETEST_H

#include <QRandomGenerator>
static int hierarchy_len = 0;

#include "archive/archive.h"

void ArchiveAddRandomData(Archive* archive, Item *folder = nullptr)
{
  if (!folder) {
    folder = archive->GetRoot();
  }

  static const QStringList names = {
    QStringLiteral("whirl"),
    QStringLiteral("vulgar"),
    QStringLiteral("crown"),
    QStringLiteral("pink"),
    QStringLiteral("miscreant"),
    QStringLiteral("command"),
    QStringLiteral("holiday"),
    QStringLiteral("undress"),
    QStringLiteral("desk"),
    QStringLiteral("cagey"),
    QStringLiteral("driving"),
    QStringLiteral("aunt"),
    QStringLiteral("nation"),
    QStringLiteral("coach"),
    QStringLiteral("drink"),
    QStringLiteral("three"),
    QStringLiteral("uneven"),
    QStringLiteral("scratch"),
    QStringLiteral("house"),
    QStringLiteral("debt"),
    QStringLiteral("zippy"),
    QStringLiteral("rake"),
    QStringLiteral("fine"),
    QStringLiteral("shoe"),
    QStringLiteral("scrape"),
    QStringLiteral("unwritten"),
    QStringLiteral("popcorn"),
    QStringLiteral("jazzy"),
    QStringLiteral("hour"),
    QStringLiteral("wonderful"),
    QStringLiteral("reading"),
    QStringLiteral("abstracted")
  };

  static const QStringList exts = {
    QStringLiteral(".txt"),
    QStringLiteral(".doc"),
    QStringLiteral(".exe"),
    QStringLiteral(".gif"),
    QStringLiteral(".jpg"),
    QStringLiteral(".mp4"),
    QStringLiteral(".mp3"),
    QStringLiteral(".png"),
    QString()
  };

  int c = QRandomGenerator::global()->bounded(20);
  for (int i=0; i<c; i++) {
    Item::Type type;
    if (hierarchy_len < 5 && QRandomGenerator::global()->bounded(4) < 1) {
      type = Item::kFolder;
    } else {
      type = Item::kFile;
    }

    QString name = names.at(QRandomGenerator::global()->bounded(names.size()));

    if (type == Item::kFile) {
      name.append(exts.at(QRandomGenerator::global()->bounded(exts.size())));
    }

    Item* item = new Item(type, name);
    folder->AddChild(item);

    if (type == Item::kFolder) {
      hierarchy_len++;
      AddRandomData(item);
      hierarchy_len--;
    }
  }
}

#endif // ARCHIVETEST_H
