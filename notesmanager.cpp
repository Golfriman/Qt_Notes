#include "notesmanager.h"
#include "xmlstorage.h"

int nextNoteId()
{
    static int i = 0;
    return ++i;
}

NotesManager::NotesManager(QObject *parent)
    : QObject{parent}
{
    mapChangedSignalToNoteId = std::make_unique<QSignalMapper>(new QSignalMapper(this));
    connect(mapChangedSignalToNoteId.get(), &QSignalMapper::mappedInt,
            this, &NotesManager::onNoteContentChanged);
    readNotes();

    if(notes.empty())
        createNewNote();
}

NotesManager::~NotesManager()
{
    writeNotes();
    qDebug() << "Save";
}

void NotesManager::createNewNote()
{
    int id= nextNoteId();
    auto& [note, textDocument] = notes[id];

    initNote(note, id);

    textDocument = createNewTextDocument(note);
    qDebug() << "Note created";

    emit newNoteCreated(id);
}

void NotesManager::removeNote(int id)
{
    notes.erase(id);
    if(notes.empty())
        createNewNote();
}

void NotesManager::renameNote(int id, const QString &newTitle)
{
    auto found = notes.find(id);
    if(found != notes.end())
    {
        auto& [note, textDocument] = found->second;
        note.title = newTitle;
        note.lastModified = QDateTime::currentDateTime();
    }
}

const Note &NotesManager::note(int id) const
{
    return notes.at(id).first;
}

QTextDocument *NotesManager::noteDocument(int id) const
{
    auto found = notes.find(id);
    if(found != notes.end())
    {
        return found->second.second.get();
    }
    return nullptr;
}

std::vector<Note> NotesManager::noteCollection()
{
    std::vector<Note> out;
    for(auto& i:  notes)
    {
        auto& [note, textDocument] = i.second;
        note.content = textDocument->toMarkdown();
        out.push_back(note);
    }
    return out;
}

size_t NotesManager::count() const
{
    return notes.size();
}

void NotesManager::onNoteContentChanged(int id)
{
    notes.at(id).first.lastModified = QDateTime::currentDateTime();
    qDebug() << "ContentChange";
    emit noteContentChanged(id);
}

void NotesManager::readNotes()
{
    XmlStorage storage;
    auto savedNotes =  storage.read();
    for(auto& n: savedNotes)
    {
        n.id = nextNoteId();
        auto& [note, textDocument] = notes[n.id];
        note = n;
        textDocument = createNewTextDocument(note);
    }
}

void NotesManager::writeNotes()
{
    XmlStorage storage;
    storage.write(noteCollection());
}

std::unique_ptr<QTextDocument> NotesManager::createNewTextDocument(const Note& note)
{
    auto textDocument = std::make_unique<QTextDocument>(note.content);
    connect(textDocument.get(), &QTextDocument::contentsChanged,
            mapChangedSignalToNoteId.get(), qOverload<>(&QSignalMapper::map));

    mapChangedSignalToNoteId->setMapping(textDocument.get(), note.id);

    return textDocument;
}

void NotesManager::initNote(Note &note, const int& id)
{
    note.id = id;
    note.title = "New Note";
    note.lastModified = QDateTime::currentDateTime();

}
