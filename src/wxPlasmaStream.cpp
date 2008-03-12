#include "wxPlasmaStream.h"

/* wxPlasmaInputStream */
wxPlasmaInputStream::wxPlasmaInputStream(hsStream* S) : fStream(S) { }
wxPlasmaInputStream::~wxPlasmaInputStream() { }

char wxPlasmaInputStream::Peek() {
    if (fStream->eof()) {
        m_lastcount = 0;
        return 0;
    } else {
        char ch = fStream->readByte();
        fStream->skip(-1);
        m_lastcount = 1;
        return ch;
    }
}

bool wxPlasmaInputStream::CanRead() const { return !Eof(); }
bool wxPlasmaInputStream::Eof() const { return fStream->eof(); }

wxFileOffset wxPlasmaInputStream::SeekI(wxFileOffset pos, wxSeekMode mode) {
    switch (mode) {
    case wxFromStart:
        fStream->seek(pos);
        return fStream->pos();
        break;
    case wxFromEnd:
        fStream->seek(fStream->size() - pos);
        return fStream->pos();
        break;
    case wxFromCurrent:
        fStream->skip(pos);
        return fStream->pos();
        break;
    default:
        return wxInvalidOffset;
    }
}

wxFileOffset wxPlasmaInputStream::TellI() const {
    return fStream->pos();
}

wxInputStream& wxPlasmaInputStream::Read(void* buffer, size_t size) {
    m_lastcount = OnSysRead(buffer, size);
    return *this;
}

size_t wxPlasmaInputStream::OnSysRead(void *buffer, size_t size) {
    if (Eof()) {
        m_lasterror = wxSTREAM_EOF;
        return 0;
    } else if (size + fStream->pos() > fStream->size()) {
        size_t count = fStream->size() - fStream->pos();
        fStream->read(count, buffer);
        return count;
    } else {
        size_t begin = fStream->pos();
        fStream->read(size, buffer);
        return (fStream->pos() - begin);
    }
}


/* wxPlasmaOutputStream */
wxPlasmaOutputStream::wxPlasmaOutputStream(hsStream* S) : fStream(S) { }
wxPlasmaOutputStream::~wxPlasmaOutputStream() { }

wxFileOffset wxPlasmaOutputStream::SeekO(wxFileOffset pos, wxSeekMode mode) {
    switch (mode) {
    case wxFromStart:
        fStream->seek(pos);
        return fStream->pos();
        break;
    case wxFromEnd:
        fStream->seek(fStream->size() - pos);
        return fStream->pos();
        break;
    case wxFromCurrent:
        fStream->skip(pos);
        return fStream->pos();
        break;
    default:
        return wxInvalidOffset;
    }
}

wxFileOffset wxPlasmaOutputStream::TellO() const {
    return fStream->pos();
}

wxOutputStream& wxPlasmaOutputStream::Write(const void* buffer, size_t size) {
    m_lastcount = OnSysWrite(buffer, size);
    return *this;
}

size_t wxPlasmaOutputStream::OnSysWrite(const void* buffer, size_t size) {
    size_t begin = fStream->pos();
    fStream->write(size, buffer);
    return (fStream->pos() - begin);
}
