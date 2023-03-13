#pragma once

#include <QString>
#include <vector>
#include "Note.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

class XmlStorage
{
public:
    XmlStorage();

    std::vector<Note> read();
    bool write(std::vector<std::reference_wrapper<Note> > notes);

private:
    void writeNote(QXmlStreamWriter& stream, const Note& note);
    void readNote(QXmlStreamReader& stream, std::vector<Note>& notes);

private:
    QString fullFilePath;

    const QString filename = "notes.xml";
    const QString dateTimeFormat = "dd.MM.yyyy - hh:mm:ss";
    const QString xmlRootToken = "NotesList";
    const QString noteToken = "Note";
    const QString titleToken = "title";
    const QString lastModified = "modified";
    const QString contentToken = "content";
};
