#pragma once
#include <QMainWindow>
#include "notesmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(NotesManager& manager, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_newNoteBtn_clicked();

    void on_removeNoteBtn_clicked();


    void onNewNoteCreated(int id);
    void onNewNoteContendChanged(int id);



    void onSelectedNoteChanged(int id);
    void onRemoveNote(int id);

    void onRenameNote(int id, const QString& newTitle);

private:
    void addNoteToList(const Note& note);
    void removeNote(int id);

    void init();

    void createConnect();

private:
    Ui::MainWindow *ui;

    NotesManager& notesManager;
};
