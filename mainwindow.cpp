#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(NotesManager &manager, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , notesManager(manager)
{
    ui->setupUi(this);
    init();
    createConnect();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_newNoteBtn_clicked()
{
    notesManager.createNewNote();
}

void MainWindow::on_removeNoteBtn_clicked()
{
    removeNote(ui->noteListWidget->currentNoteId());
}

void MainWindow::onNewNoteCreated(int id)
{
    addNoteToList(notesManager.note(id));
}

void MainWindow::onNewNoteContendChanged(int id)
{
    ui->noteListWidget->updateCurrentNote(notesManager.note(id));
}

void MainWindow::onSelectedNoteChanged(int id)
{
    auto* doc = notesManager.noteDocument(id);
    if(doc != nullptr)
    {
        ui->textEdit->setDocument(doc);
        auto cursor = ui->textEdit->textCursor();
        cursor.movePosition(QTextCursor::End);
        ui->textEdit->setTextCursor(cursor);
    }
}

void MainWindow::onRemoveNote(int id)
{
    removeNote(id);
}

void MainWindow::onRenameNote(int id, const QString &newTitle)
{
    notesManager.renameNote(id, newTitle);
    ui->noteListWidget->updateCurrentNote(notesManager.note(id));
}

void MainWindow::addNoteToList(const Note &note)
{
    ui->noteListWidget->addNote(note);
}

void MainWindow::removeNote(int id)
{
    QString noteTitle = notesManager.note(id).title;

    auto pressBtn = QMessageBox::information(this, "Remove note?",
                                                QString("Are you sure you want to remove %0?").arg(noteTitle),
                                                QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if(pressBtn == QMessageBox::Yes)
    {
        if(notesManager.count() == 1)
            ui->textEdit->setDocument(nullptr);

        ui->noteListWidget->removeCurrentNote();
        notesManager.removeNote(id);
    }
}

void MainWindow::init()
{
    auto notesList = notesManager.noteCollection();
    std::sort(notesList.begin(), notesList.end(), [](const Note& l, const Note& r)
    {
        return l.lastModified < r.lastModified;
    });

    for(auto& note: notesList)
    {
        addNoteToList(note);
    }
    ui->textEdit->setDocument(notesManager.noteDocument(1));
}

void MainWindow::createConnect()
{
    connect(&notesManager, &NotesManager::newNoteCreated,
            this, &MainWindow::onNewNoteCreated);
    connect(&notesManager, &NotesManager::noteContentChanged,
            this, &MainWindow::onNewNoteContendChanged);

    connect(ui->noteListWidget, &NoteListWidget::selectedNoteChanged,
            this, &MainWindow::onSelectedNoteChanged);

    connect(ui->noteListWidget, &NoteListWidget::removeNote,
            this, &MainWindow::onRemoveNote);
    connect(ui->noteListWidget, &NoteListWidget::renameNote,
            this, &MainWindow::onRenameNote);
}
