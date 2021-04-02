#include "errordialog.h"

#include <QApplication>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QVBoxLayout>

ErrorDialog::ErrorDialog(QWidget *parent)
  : QDialog(parent),
    entry_count_(0)
{
  QVBoxLayout *layout = new QVBoxLayout(this);

  list_widget_ = new QListWidget();
  layout->addWidget(list_widget_);

  QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok);
  connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
  layout->addWidget(buttons);
}

void ErrorDialog::AddEntry(ErrorDialog::Type type, const QString &msg)
{
  QStyle::StandardPixmap standard_icon;

  switch (type) {
  case kError:
    standard_icon = QStyle::SP_MessageBoxCritical;
    break;
  case kWarning:
    standard_icon = QStyle::SP_MessageBoxWarning;
    break;
  case kInformation:
    standard_icon = QStyle::SP_MessageBoxInformation;
    break;
  }

  list_widget_->addItem(new QListWidgetItem(qApp->style()->standardIcon(standard_icon), msg));

  entry_count_++;
}
