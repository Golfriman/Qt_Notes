#pragma once
#include <QWidget>
#include "Note.h"
#include "QContextMenuEvent"
#include "renamedialog.h"

namespace Ui {
class NoteWidget;
}

class NoteWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NoteWidget(const Note& note, QWidget *parent = nullptr);
    int noteId();
    void updateContent(const Note& note);
    ~NoteWidget();
signals:
    void removeNote(int id);
    void renameNote(int id, const QString& newTitle);
protected:
    void contextMenuEvent(QContextMenuEvent* event) override;

private:
    void showContextMenu(const QPoint& pos);
    void onRemoveActionTriggered();
    void onRenameActionTriggered();

private:
    Ui::NoteWidget *ui;
    const QString dateTimeFormat = "dd.MM.yyyy - hh:mm:ss";
    const int id;
    QString title;
    QString lastModified;
};
