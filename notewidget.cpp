#include "notewidget.h"
#include "ui_notewidget.h"
#include "QMenu"


NoteWidget::NoteWidget(const Note &note, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NoteWidget),
    id(note.id), title(note.title), lastModified(note.lastModified.toString(dateTimeFormat))
{
    ui->setupUi(this);
    ui->title->setText(title);
    setToolTip(QString("%0\n%1").arg(title, lastModified));

}

int NoteWidget::noteId()
{
    return id;
}

void NoteWidget::updateContent(const Note &note)
{
    title = note.title;
    lastModified = note.lastModified.toString(dateTimeFormat);

    ui->title->setText(title);
    setToolTip(QString("%0\n%1").arg(title, lastModified));
}

NoteWidget::~NoteWidget()
{
    delete ui;
}

void NoteWidget::contextMenuEvent(QContextMenuEvent *event)
{
    showContextMenu(event->globalPos());
}

void NoteWidget::showContextMenu(const QPoint &pos)
{
    QMenu contextMenu(this);
    QAction *renameAction = contextMenu.addAction("Rename note");
    connect(renameAction, &QAction::triggered,
            this, &NoteWidget::onRenameActionTriggered);

    QAction *removeAction = contextMenu.addAction("Remove note");
    connect(removeAction, &QAction::triggered,
            this, &NoteWidget::onRemoveActionTriggered);

    contextMenu.exec(pos);
}

void NoteWidget::onRemoveActionTriggered()
{
    emit removeNote(id);
}

void NoteWidget::onRenameActionTriggered()
{
    RenameDialog renameDlg(title, this);
    if(renameDlg.exec() == QDialog::Accepted)
        emit renameNote(id, renameDlg.newName());
}
