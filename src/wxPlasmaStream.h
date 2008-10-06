#ifndef _WXPLASMASTREAM_H
#define _WXPLASMASTREAM_H

#include <wx/wxprec.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <Stream/hsStream.h>

class wxPlasmaInputStream : public wxInputStream {
protected:
    hsStream* fStream;

public:
    wxPlasmaInputStream(hsStream* S);
    virtual ~wxPlasmaInputStream();

    virtual char Peek();
    virtual bool CanRead() const;
    virtual bool Eof() const;
    virtual wxFileOffset SeekI(wxFileOffset pos, wxSeekMode mode = wxFromStart);
    virtual wxFileOffset TellI() const;

    virtual wxInputStream& Read(void* buffer, size_t size);

protected:
    virtual size_t OnSysRead(void *buffer, size_t size);
    DECLARE_NO_COPY_CLASS(wxPlasmaInputStream)
};

class wxPlasmaOutputStream : public wxOutputStream {
protected:
    hsStream* fStream;

public:
    wxPlasmaOutputStream(hsStream* S);
    virtual ~wxPlasmaOutputStream();

    virtual wxFileOffset SeekO(wxFileOffset pos, wxSeekMode mode = wxFromStart);
    virtual wxFileOffset TellO() const;

    virtual wxOutputStream& Write(const void* buffer, size_t size);

protected:
    virtual size_t OnSysWrite(const void* buffer, size_t size);
    DECLARE_NO_COPY_CLASS(wxPlasmaOutputStream)
};

#endif
