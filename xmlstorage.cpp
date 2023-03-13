#include "xmlstorage.h"
#include <QFile>
#include <QStandardPaths>
#include <QDir>

XmlStorage::XmlStorage()
{
    QString path = ".";
    QDir dir;

    if(!dir.exists(path))
        dir.mkdir(path);
    fullFilePath = QString("%0/%1").arg(path, filename);
}

std::vector<Note> XmlStorage::read()
{
    std::vector<Note> notes;

    QFile inputFile(fullFilePath);

    if(inputFile.open(QIODevice::ReadOnly))
    {
        QXmlStreamReader reader(&inputFile);
        if(reader.readNextStartElement())
        {
            if(reader.name() == xmlRootToken)
            {
                while(reader.readNextStartElement())
                {
                    readNote(reader, notes);

                }
            }
            else {
                reader.skipCurrentElement();
            }
        }
    }
    return notes;
}

bool XmlStorage::write(std::vector<std::reference_wrapper<Note> > notes)
{
    QFile outputFile(fullFilePath);

    if(!outputFile.open(QIODevice::WriteOnly))
    {
        return false;
    }

    QXmlStreamWriter writer(&outputFile);
    writer.setAutoFormatting(true);

    writer.writeStartDocument();
    writer.writeStartElement(xmlRootToken);

    for(auto& note: notes)
    {
        writeNote(writer, note);
    }

    writer.writeEndElement();
    writer.writeEndDocument();

    return true;
}

void XmlStorage::writeNote(QXmlStreamWriter &stream, const Note &note)
{
    stream.writeStartElement(noteToken);

    stream.writeAttribute(titleToken, note.title);
    stream.writeAttribute(lastModified, note.lastModified.toString(dateTimeFormat));
    stream.writeTextElement(contentToken, note.content);

    stream.writeEndElement();
}

void XmlStorage::readNote(QXmlStreamReader &stream, std::vector<Note> &notes)
{
    if(stream.name() == noteToken)
    {
        Note note;
        auto attributes = stream.attributes();

        note.title = attributes.value(titleToken).toString();
        note.lastModified = QDateTime::fromString(attributes.value(lastModified).toString(), dateTimeFormat);

        stream.readNextStartElement();
        if(stream.name() == contentToken)
        {
            note.content = stream.readElementText();
        }
        notes.push_back(note);
    }
    stream.skipCurrentElement();
}
