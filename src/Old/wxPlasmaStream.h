/* This file is part of PlasmaShop.
 *
 * PlasmaShop is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PlasmaShop is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PlasmaShop.  If not, see <http://www.gnu.org/licenses/>.
 */

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
