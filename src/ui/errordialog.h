#ifndef ERRORDIALOG_H
#define ERRORDIALOG_H

#include <QDialog>
#include <QListWidget>

class ErrorDialog : public QDialog
{
  Q_OBJECT
public:
  enum Type {
    kError,
    kWarning,
    kInformation
  };

  explicit ErrorDialog(QWidget *parent = nullptr);

  void AddEntry(Type type, const QString& msg);

  int GetEntryCount() const
  {
    return entry_count_;
  }

signals:

private:
  QListWidget *list_widget_;

  int entry_count_;

};

#endif // ERRORDIALOG_H
