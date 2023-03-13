#pragma once

#include <QDialog>

namespace Ui {
class RenameDialog;
}

class RenameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RenameDialog(const QString& oldName, QWidget *parent = nullptr);
    ~RenameDialog();

    QString newName();

private slots:
    void on_newName_textChanged(const QString &arg1);

private:
    Ui::RenameDialog *ui;
};
