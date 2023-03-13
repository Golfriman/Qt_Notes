#include "notelistwidget.h"
#include "ui_notelistwidget.h"

NoteListWidget::NoteListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NoteListWidget)
{
    ui->setupUi(this);
    connect(ui->noteList, &QListWidget::itemSelectionChanged,
            this, &NoteListWidget::onItemSelectionChanged);
}

NoteListWidget::~NoteListWidget()
{
    delete ui;
}

void NoteListWidget::addNote(const Note &note)
{
    auto* item = new QListWidgetItem();
    ui->noteList->insertItem(0, item);

    setupNoteItem(note, item);
}

void NoteListWidget::removeCurrentNote()
{
    auto currentItem = ui->noteList->currentItem();
    if (currentItem != nullptr) delete currentItem;
}

void NoteListWidget::updateCurrentNote(const Note &note)
{
    if(ui->noteList->currentRow() != 0)
        moveCurrentItemToTop(note);
    else {
        auto widget = static_cast<NoteWidget*>(ui->noteList->itemWidget(ui->noteList->currentItem()));
        qDebug() << widget->noteId();
        widget->updateContent(note);
    }
}

int NoteListWidget::currentNoteId()
{
    auto* currentItem = ui->noteList->currentItem();
    qDebug() << currentItem;
    int noteId = dynamic_cast<NoteWidget*>(ui->noteList->itemWidget(currentItem))->noteId();
    return noteId;
}

void NoteListWidget::onItemSelectionChanged()
{
    auto* currentItem = ui->noteList->currentItem();
    if(currentItem != nullptr)
    {
        int id = currentNoteId();
        emit selectedNoteChanged(id);
    }
}

void NoteListWidget::moveCurrentItemToTop(const Note &note)
{
    auto item = ui->noteList->takeItem(ui->noteList->currentRow());
    ui->noteList->insertItem(0, item);
    setupNoteItem(note, item);
}

void NoteListWidget::setupNoteItem(const Note &note, QListWidgetItem *item)
{
    NoteWidget* widget = new NoteWidget(note);

    connect(widget, &NoteWidget::removeNote,
            this, &NoteListWidget::removeNote);

    connect(widget, &NoteWidget::renameNote,
            this, &NoteListWidget::renameNote);

    item->setSizeHint(widget->sizeHint());

    ui->noteList->setItemWidget(item, widget);
    ui->noteList->setCurrentItem(item);

}
