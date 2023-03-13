#pragma once
#include <QWidget>
#include "notewidget.h"
#include <QListWidgetItem>

namespace Ui {
class NoteListWidget;
}


class NoteListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NoteListWidget(QWidget *parent = nullptr);
    ~NoteListWidget();

    void addNote(const Note& note);
    void removeCurrentNote();
    void updateCurrentNote(const Note& note);

    int currentNoteId();

signals:
    void selectedNoteChanged(int id);
    void removeNote(int id);
    void renameNote(int id, const QString& newTitle);


private:
    void onItemSelectionChanged();

    void moveCurrentItemToTop(const Note& note);
    void setupNoteItem(const Note& note, QListWidgetItem* item);

private:
    Ui::NoteListWidget *ui;

};
