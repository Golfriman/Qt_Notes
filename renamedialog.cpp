#include "renamedialog.h"
#include "ui_renamedialog.h"
#include "QPushButton"
RenameDialog::RenameDialog(const QString &oldName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RenameDialog)
{
    ui->setupUi(this);
    ui->newName->setText(oldName);
    ui->newName->setSelection(0, ui->newName->text().size());
}

RenameDialog::~RenameDialog()
{
    delete ui;
}

QString RenameDialog::newName()
{
   return ui->newName->text();
}

void RenameDialog::on_newName_textChanged(const QString &arg1)
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(arg1.size() != 0);
}

