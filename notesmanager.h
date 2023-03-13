#pragma once
#include <QObject>

#include "Note.h"
#include <unordered_map>
#include <utility>
#include <memory>
#include <QTextDocument>
#include <QSignalMapper>
#include <vector>

class NotesManager : public QObject
{
    Q_OBJECT
public:
    explicit NotesManager(QObject *parent = nullptr);
    ~NotesManager();

    void createNewNote();
    void removeNote(int id);
    void renameNote(int id, const QString& newTitle);

    const Note &note(int id)const;


    QTextDocument* noteDocument(int id)const;

    std::vector<Note> noteCollection();

    size_t count()const;

signals:
    void newNoteCreated(int id);
    void noteContentChanged(int id);

private:
    void onNoteContentChanged(int id);

    void readNotes();
    void writeNotes();
    std::unique_ptr<QTextDocument> createNewTextDocument(const Note& note);


    void initNote(Note& note, const int &id);

private:
    std::unordered_map<int, std::pair<Note, std::unique_ptr<QTextDocument>>> notes;

    std::unique_ptr<QSignalMapper> mapChangedSignalToNoteId;
};
