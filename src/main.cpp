#include "mainwindow.h"

#include <QApplication>

#include "adapter/adapter.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  Adapter::InitializeAdapters();

  MainWindow w;
  w.show();

  int ret = a.exec();

  Adapter::DestroyAdapters();

  return ret;
}
