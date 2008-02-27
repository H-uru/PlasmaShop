/////////////////////////////////////////////////////////////////////////////
// Name:        imagdds.cpp
// Purpose:     wxImage DDS handler
// Author:      Gregory Smith
// Copyright:   (c) Gregory Smith
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// currently this file has some Aleph One specializations; some formats are
// ignored or treated differently...caveat emptor

#include "imagdds.h"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#ifndef __WIN32__
#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES 1
#endif
#endif
#include <wx/glcanvas.h>
#endif

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include <squish.h>

#include "math.h"
#include <algorithm>
#include <vector>
using namespace std;

#define MAKE_FOURCC(a,b,c,d) (((wxUint32(d) << 24) | (wxUint32)(c) << 16) | ((wxUint32)(b) << 8) | (wxUint32)(a))

static inline int NextPowerOfTwo(int n)
{
        int p = 1;
        while(p < n) {p <<= 1;}
        return p;
}

bool HasS3TC()
{
    return ((void *) squish::CompressImage != (void *) squish::DecompressImage);
}

bool wxDDSHandler::ReadHeader(wxInputStream& stream, DDSURFACEDESC2 &ddsd)
{
    // try to read the whole thing, then swap it
    stream.Read(&ddsd, sizeof(ddsd));
    if (stream.LastRead() != sizeof(ddsd)) return FALSE;
    
    ddsd.dwSize = wxINT32_SWAP_ON_BE(ddsd.dwSize);
    ddsd.dwFlags = wxINT32_SWAP_ON_BE(ddsd.dwFlags);
    ddsd.dwHeight = wxINT32_SWAP_ON_BE(ddsd.dwHeight);
    ddsd.dwWidth = wxINT32_SWAP_ON_BE(ddsd.dwWidth);
    ddsd.dwPitchOrLinearSize = wxINT32_SWAP_ON_BE(ddsd.dwPitchOrLinearSize);
    ddsd.dwDepth = wxINT32_SWAP_ON_BE(ddsd.dwDepth);
    ddsd.dwMipMapCount = wxINT32_SWAP_ON_BE(ddsd.dwMipMapCount);
    
    ddsd.ddpfPixelFormat.dwSize = wxINT32_SWAP_ON_BE(ddsd.ddpfPixelFormat.dwSize);
    ddsd.ddpfPixelFormat.dwFlags = wxINT32_SWAP_ON_BE(ddsd.ddpfPixelFormat.dwFlags);
    ddsd.ddpfPixelFormat.dwFourCC = wxINT32_SWAP_ON_BE(ddsd.ddpfPixelFormat.dwFourCC);
    ddsd.ddpfPixelFormat.dwRGBBitCount = wxINT32_SWAP_ON_BE(ddsd.ddpfPixelFormat.dwRGBBitCount);
    
    ddsd.ddsCaps.dwCaps1 = wxINT32_SWAP_ON_BE(ddsd.ddsCaps.dwCaps1);
    ddsd.ddsCaps.dwCaps2 = wxINT32_SWAP_ON_BE(ddsd.ddsCaps.dwCaps2);
    
    return TRUE;
    
}

bool wxDDSHandler::WriteHeader(wxOutputStream &stream, DDSURFACEDESC2 &ddsd)
{
    wxUint32 dwMagic = (wxINT32_SWAP_ON_BE(MAKE_FOURCC('D', 'D', 'S', ' ')));
    stream.Write(&dwMagic, sizeof(dwMagic));

    ddsd.dwSize = wxINT32_SWAP_ON_BE(ddsd.dwSize);
    ddsd.dwFlags = wxINT32_SWAP_ON_BE(ddsd.dwFlags);
    ddsd.dwHeight = wxINT32_SWAP_ON_BE(ddsd.dwHeight);
    ddsd.dwWidth = wxINT32_SWAP_ON_BE(ddsd.dwWidth);
    ddsd.dwPitchOrLinearSize = wxINT32_SWAP_ON_BE(ddsd.dwPitchOrLinearSize);
    ddsd.dwDepth = wxINT32_SWAP_ON_BE(ddsd.dwDepth);
    ddsd.dwMipMapCount = wxINT32_SWAP_ON_BE(ddsd.dwMipMapCount);
    
    ddsd.ddpfPixelFormat.dwSize = wxINT32_SWAP_ON_BE(ddsd.ddpfPixelFormat.dwSize);
    ddsd.ddpfPixelFormat.dwFlags = wxINT32_SWAP_ON_BE(ddsd.ddpfPixelFormat.dwFlags);
    ddsd.ddpfPixelFormat.dwFourCC = wxINT32_SWAP_ON_BE(ddsd.ddpfPixelFormat.dwFourCC);
    ddsd.ddpfPixelFormat.dwRGBBitCount = wxINT32_SWAP_ON_BE(ddsd.ddpfPixelFormat.dwRGBBitCount);
    
    ddsd.ddsCaps.dwCaps1 = wxINT32_SWAP_ON_BE(ddsd.ddsCaps.dwCaps1);
    ddsd.ddsCaps.dwCaps2 = wxINT32_SWAP_ON_BE(ddsd.ddsCaps.dwCaps2);

    stream.Write(&ddsd, sizeof(ddsd));
    return TRUE;
}

bool wxDDSHandler::DoCanRead(wxInputStream& stream)
{
    wxUint32 dwMagic;
    stream.Read(&dwMagic, sizeof(dwMagic));
    if (stream.LastRead() != sizeof(dwMagic)) return FALSE;
    
    if (wxINT32_SWAP_ON_BE(dwMagic) != MAKE_FOURCC('D', 'D', 'S', ' ')) return FALSE;

    DDSURFACEDESC2 ddsd;
    if (!ReadHeader(stream, ddsd)) return FALSE;

    // validate the sizes
    if (ddsd.dwSize != 124) return FALSE;
    if (ddsd.ddpfPixelFormat.dwSize != 32) return FALSE;

    if (ddsd.ddsCaps.dwCaps2 & DDSCAPS2_VOLUME) return FALSE;
    if (ddsd.ddsCaps.dwCaps2 & DDSCAPS2_CUBEMAP) return FALSE;
    
    if (ddsd.ddpfPixelFormat.dwFlags & DDPF_RGB) {
        return (ddsd.ddpfPixelFormat.dwRGBBitCount == 24 || ddsd.ddpfPixelFormat.dwRGBBitCount == 32);
    }
    else if (HasS3TC() && (ddsd.ddpfPixelFormat.dwFlags & DDPF_FOURCC) &&
             (ddsd.ddpfPixelFormat.dwFourCC == MAKE_FOURCC('D', 'X', 'T', '1') ||
              ddsd.ddpfPixelFormat.dwFourCC == MAKE_FOURCC('D', 'X', 'T', '2') ||
              ddsd.ddpfPixelFormat.dwFourCC == MAKE_FOURCC('D', 'X', 'T', '3') ||
              ddsd.ddpfPixelFormat.dwFourCC == MAKE_FOURCC('D', 'X', 'T', '4') ||
              ddsd.ddpfPixelFormat.dwFourCC == MAKE_FOURCC('D', 'X', 'T', '5')))
        return TRUE;
    else
        return FALSE;
    
}

static void UnpremultiplyAlpha(wxImage *image)
{
    for (int x = 0; x < image->GetWidth(); x++) {
        for (int y = 0; y < image->GetHeight(); y++) {
            if (image->GetAlpha(x, y) == 0) continue;
            short red = image->GetRed(x, y);
            short green = image->GetGreen(x, y);
            short blue = image->GetBlue(x, y);

            red = std::min(255, 255 * red / image->GetAlpha(x, y));
            green = std::min(255, 255 * green / image->GetAlpha(x, y));
            blue = std::min(255, 255 * blue / image->GetAlpha(x, y));

            image->SetRGB(x, y, (unsigned char) red, (unsigned char) green, (unsigned char) blue);
        }
    }
}

bool wxDDSHandler::LoadFile(wxImage *image, wxInputStream& stream, bool verbose, int index)
{
    wxUint32 dwMagic;
    stream.Read(&dwMagic, sizeof(dwMagic));
    if (stream.LastRead() != sizeof(dwMagic)) return FALSE;

    if (wxINT32_SWAP_ON_BE(dwMagic) != MAKE_FOURCC('D', 'D', 'S', ' ')) return FALSE;
    
    DDSURFACEDESC2 ddsd;
    if (!ReadHeader(stream, ddsd)) return FALSE;
    
    bool unpremultiplyAlpha = false;
    
    // just read the first mipmap
    GLenum internalFormat = GL_NONE;
    if (ddsd.ddpfPixelFormat.dwFlags & DDPF_FOURCC) {
        if (ddsd.ddpfPixelFormat.dwFourCC == MAKE_FOURCC('D', 'X', 'T', '1'))
            internalFormat = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
        else if (ddsd.ddpfPixelFormat.dwFourCC == MAKE_FOURCC('D', 'X', 'T', '2')) {
            unpremultiplyAlpha = true;
            internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        }
        else if (ddsd.ddpfPixelFormat.dwFourCC == MAKE_FOURCC('D', 'X', 'T', '3'))
            internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        else if (ddsd.ddpfPixelFormat.dwFourCC == MAKE_FOURCC('D', 'X', 'T', '4')) {
            unpremultiplyAlpha = true;
            internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        }
        else if (ddsd.ddpfPixelFormat.dwFourCC == MAKE_FOURCC('D', 'X', 'T', '5'))
            internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
    } else if (ddsd.ddpfPixelFormat.dwFlags & DDPF_RGB) {
        if (ddsd.ddpfPixelFormat.dwRGBBitCount == 24) {
            internalFormat = GL_RGB;
        } else if (ddsd.ddpfPixelFormat.dwRGBBitCount == 32) {
            internalFormat = GL_RGBA;
        }
    }
    if (internalFormat == GL_NONE) return FALSE;
    if (!HasS3TC() && (internalFormat == GL_COMPRESSED_RGB_S3TC_DXT1_EXT || internalFormat == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT || internalFormat == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT)) return FALSE;

    int width = ddsd.dwWidth;
    int height = ddsd.dwHeight;

    if (internalFormat == GL_RGB || internalFormat == GL_RGBA) {
        int pitch;
        if (ddsd.dwFlags & DDSD_PITCH) {
            pitch = ddsd.dwPitchOrLinearSize;
        } else {
            pitch = ddsd.dwPitchOrLinearSize / ddsd.dwHeight;
        }

        if (pitch != ((internalFormat == GL_RGB) ? 3 : 4) * width)
        {
            fprintf(stderr, "we don't know how to do weird pitch\n");
            return FALSE;
        }

        image->Create(width, height);
        if (internalFormat == GL_RGBA) 
            image->InitAlpha();
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                unsigned char b = stream.GetC();
                if (stream.LastRead() != 1) {
                    return FALSE;
                }
                unsigned char g = stream.GetC();
                if (stream.LastRead() != 1) {
                    return FALSE;
                }
                unsigned char r = stream.GetC();
                if (stream.LastRead() != 1) {
                    return FALSE;
                }

                image->SetRGB(x, y, r, g, b);

                if (internalFormat == GL_RGBA)
                {
                    image->SetAlpha(x, y, stream.GetC());
                    if (stream.LastRead() != 1) {
                        return FALSE;
                    }
                }
            }
        }
    
        return TRUE;
    }
    
    int bpp = (internalFormat == GL_COMPRESSED_RGB_S3TC_DXT1_EXT) ? 4 : 8;
    int potWidth = NextPowerOfTwo(width);
    int potHeight = NextPowerOfTwo(height);

    int compressedBufferSize = (potWidth * potHeight * bpp) / 8;

    vector<GLbyte> compressedBuffer(width * height * bpp / 8);
    stream.Read(&compressedBuffer.front(), compressedBuffer.size());
    if (stream.LastRead() != compressedBuffer.size()) {
        return FALSE;
    }

    image->Create(width, height);
    vector<unsigned char> uncompressedBuffer(width * height * 4);
    int flags;
    if (internalFormat == GL_COMPRESSED_RGB_S3TC_DXT1_EXT) {
        flags = squish::kDxt1;
    } else {
        if (internalFormat == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT) {
            flags = squish::kDxt3;
        } else {
            flags = squish::kDxt5;
        }
        image->InitAlpha();
    }
          
    squish::DecompressImage(&uncompressedBuffer.front(), width, height, &compressedBuffer.front(), flags);
    
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            image->GetData()[(x + y * width) * 3 + 0] = uncompressedBuffer[(x + y * width) * 4 + 0];
            image->GetData()[(x + y * width) * 3 + 1] = uncompressedBuffer[(x + y * width) * 4 + 1];
            image->GetData()[(x + y * width) * 3 + 2] = uncompressedBuffer[(x + y * width) * 4 + 2];
            if (image->HasAlpha()) {
                image->GetAlpha()[(x + y * width)] = uncompressedBuffer[(x + y * width) * 4 + 3];
            }
        }
        
    }

    if (unpremultiplyAlpha)
        UnpremultiplyAlpha(image);

    return TRUE;
}

static void PremultiplyAlpha(wxImage *image)
{
    for (int x = 0; x < image->GetWidth(); x++) {
        for (int y = 0; y < image->GetHeight(); y++) {
            short red = image->GetRed(x, y);
            short green = image->GetGreen(x, y);
            short blue = image->GetBlue(x, y);
            
            red = (image->GetAlpha(x, y) * red + 127) / 255;
            green = (image->GetAlpha(x, y) * green + 127) / 255;
            blue = (image->GetAlpha(x, y) * blue + 127) / 255;
            
            image->SetRGB(x, y, (unsigned char) red, (unsigned char) green, (unsigned char) blue);
        }    
    }
}

bool wxDDSHandler::SaveFile(wxImage *image, wxOutputStream& stream, bool verbose)
{

    wxBusyCursor getBusy;

    bool mipmap = image->HasOption(wxIMAGE_OPTION_DDS_USE_MIPMAPS) && 
        image->GetOptionInt(wxIMAGE_OPTION_DDS_USE_MIPMAPS);
    bool compress = image->HasOption(wxIMAGE_OPTION_DDS_COMPRESS) &&
        image->GetOptionInt(wxIMAGE_OPTION_DDS_COMPRESS);
    bool premultiply = image->HasOption(wxIMAGE_OPTION_DDS_PREMULTIPLY_ALPHA) &&
        image->GetOptionInt(wxIMAGE_OPTION_DDS_PREMULTIPLY_ALPHA);

    if (!HasS3TC() && compress) return FALSE;

    if (compress)  {
        if ((image->GetHeight() & 3) || (image->GetWidth() & 3)) {
            image->Rescale((image->GetWidth() + 3) & ~3, (image->GetHeight() + 3) & ~3);
        }
    }
    
    DDSURFACEDESC2 ddsd;
    memset(&ddsd, 0, sizeof(ddsd));
    ddsd.dwSize = 124;
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
    ddsd.dwHeight = image->GetHeight();
    ddsd.dwWidth = image->GetWidth();
    if (compress) {
        ddsd.dwFlags |= DDSD_LINEARSIZE;
        if (image->HasAlpha()) {
            ddsd.dwPitchOrLinearSize = image->GetWidth() / 4 * image->GetHeight() / 4 * 16;
            
            ddsd.ddpfPixelFormat.dwFourCC = MAKE_FOURCC('D', 'X', 'T', '5');
        } else {
            ddsd.dwPitchOrLinearSize = image->GetWidth() / 4 * image->GetHeight() / 4 * 8;
            ddsd.ddpfPixelFormat.dwFourCC = MAKE_FOURCC('D', 'X', 'T', '1');
        }
    } else {
        ddsd.dwFlags |= DDSD_PITCH;
        if (image->HasAlpha()) {
            ddsd.dwPitchOrLinearSize = image->GetWidth() * 4;
        } else {
            ddsd.dwPitchOrLinearSize = image->GetWidth() * 3;
        }
    }
    
    int mipmap_count;
    
    if (mipmap) {
            mipmap_count = ddsd.dwMipMapCount = NumMipmaps(*image);
            ddsd.dwFlags |= DDSD_MIPMAPCOUNT;
    }
    
    ddsd.ddpfPixelFormat.dwSize = 32;
    if (compress) {
        ddsd.ddpfPixelFormat.dwFlags = DDPF_FOURCC;
    } else {
        ddsd.ddpfPixelFormat.dwFlags = DDPF_RGB;
        ddsd.ddpfPixelFormat.dwRBitMask = wxINT32_SWAP_ON_BE(0x00ff0000);
        ddsd.ddpfPixelFormat.dwGBitMask = wxINT32_SWAP_ON_BE(0x0000ff00);
        ddsd.ddpfPixelFormat.dwBBitMask = wxINT32_SWAP_ON_BE(0x000000ff);
        if (image->HasAlpha())
        {
            ddsd.ddpfPixelFormat.dwRGBBitCount = 32;
            ddsd.ddpfPixelFormat.dwRGBAlphaBitMask = wxINT32_SWAP_ON_BE(0xff000000);
            ddsd.ddpfPixelFormat.dwFlags |= DDPF_ALPHAPIXELS;
        } else {
            ddsd.ddpfPixelFormat.dwRGBBitCount = 24;
        }
    }
    
    ddsd.ddsCaps.dwCaps1 = DDSCAPS_TEXTURE;
    if (mipmap) ddsd.ddsCaps.dwCaps1 |= DDSCAPS_MIPMAP | DDSCAPS_COMPLEX;
    
    WriteHeader(stream, ddsd);
    
    if (premultiply) 
        PremultiplyAlpha(image);
    wxImage minImage = *image;
    
    for (int level = 0; level < ((mipmap) ? mipmap_count : 1); level++) {
        minImage = Minify(*image, level);
        if (compress) {
            if (image->HasAlpha()) {
                WriteDXT5(minImage, stream);
            } else {
                WriteDXT1(minImage, stream);
            }
        } else {
            WriteRGBA(minImage, stream);
        }
    }
    return TRUE;
}

int wxDDSHandler::NumMipmaps(const wxImage &image)
{
    return (1 + (int) floor(log(std::max(image.GetWidth(), image.GetHeight())) / log(2)));
}

static vector<unsigned char> BuildRGBAImage(const wxImage& image)
{
    vector<unsigned char> buffer(image.GetWidth() * image.GetHeight() * 4);
    for (int x = 0; x < image.GetWidth(); x++) {
        for (int y = 0; y < image.GetHeight(); y++) {
            buffer[(x + y * image.GetWidth()) * 4 + 0] = image.GetRed(x, y);
            buffer[(x + y * image.GetWidth()) * 4 + 1] = image.GetGreen(x, y);
            buffer[(x + y * image.GetWidth()) * 4 + 2] = image.GetBlue(x, y);
            if (image.HasAlpha())
                buffer[(x + y * image.GetWidth()) * 4 + 3] = image.GetAlpha(x, y);
            else
                buffer[(x + y * image.GetWidth()) * 4 + 3] = 0xff;
        }
    }

    return buffer;
}


void wxDDSHandler::WriteDXT1(const wxImage& image, wxOutputStream& stream)
{
    // use DXT3 to force 4 colors
    vector<unsigned char> compressedBuffer(squish::GetStorageRequirements(image.GetWidth(), image.GetHeight(), squish::kDxt3));
    squish::CompressImage(&BuildRGBAImage(image).front(), image.GetWidth(), image.GetHeight(), &compressedBuffer.front(), squish::kDxt3 | squish::kColourIterativeClusterFit);

    // skip the alpha blocks, and just write the colors
    unsigned int i = 0;
    while (i <= compressedBuffer.size() - 16)
    {
        i += 8;
        stream.Write(&compressedBuffer[i], 8);
        i += 8;
    }
}


void wxDDSHandler::WriteDXT5(const wxImage& image, wxOutputStream& stream)
{
    vector<unsigned char> compressedBuffer(squish::GetStorageRequirements(image.GetWidth(), image.GetHeight(), squish::kDxt5));
    squish::CompressImage(&BuildRGBAImage(image).front(), image.GetWidth(), image.GetHeight(), &compressedBuffer.front(), squish::kDxt5);
    stream.Write(&compressedBuffer.front(), compressedBuffer.size());
}

void wxDDSHandler::WriteRGBA(const wxImage& image, wxOutputStream& stream)
{
    for (int y = 0; y < image.GetHeight(); y++) {
        for (int x = 0; x < image.GetWidth(); x++) {
            stream.PutC(image.GetBlue(x, y));
            stream.PutC(image.GetGreen(x, y));
            stream.PutC(image.GetRed(x, y));
            if (image.HasAlpha())
            {
                stream.PutC(image.GetAlpha(x, y));
            }
        }
    }        
}

typedef        unsigned char        Pixel;

typedef struct {
        int        xsize;                /* horizontal size of the image in Pixels */
        int        ysize;                /* vertical size of the image in Pixels */
        Pixel *        data;                /* pointer to first scanline of image */
        int        span;                /* byte offset between two scanlines */
} Image;

extern "C" {
#define triangle_support        (1.0)
#define Mitchell_support        (2.0)
#define Lanczos3_support        (3.0)

    extern double box_filter(double);
    extern double triangle_filter(double);
    extern double Lanczos3_filter(double);
    extern double Mitchell_filter(double);
    
    extern void zoom(Image *dst, Image *src, double (*filterf)(double), double fwidth);
}

wxImage wxDDSHandler::Minify(wxImage &image, int level)
{
    wxImage minifiedImage;
    if (level == 0) return image;

    minifiedImage.Create(std::max(image.GetWidth() >> level, 1), std::max(image.GetHeight() >> level, 1));
    
    // to use filtered scales, we need to do a channel at a time
    vector<Pixel> srcPlane(image.GetWidth() * image.GetHeight());
    vector<Pixel> dstPlane(minifiedImage.GetWidth() * minifiedImage.GetHeight());

    double (*f)(double);
    double s;

    if (image.HasOption(wxIMAGE_OPTION_DDS_MIPMAP_FILTER))
    {
        if (image.GetOptionInt(wxIMAGE_OPTION_DDS_MIPMAP_FILTER) == wxIMAGE_OPTION_DDS_FILTER_TRIANGLE) {
            f = triangle_filter;
            s = triangle_support;
        } else if (image.GetOptionInt(wxIMAGE_OPTION_DDS_MIPMAP_FILTER) == wxIMAGE_OPTION_DDS_FILTER_LANCZOS) {
            f = Lanczos3_filter;
            s = Lanczos3_support;
        } else {
            f = Mitchell_filter;
            s = Mitchell_support;
        }
    } else {
        f = Mitchell_filter;
        s = Mitchell_support;
    }

    Image src = { image.GetWidth(), image.GetHeight(), &srcPlane.front(), image.GetWidth() };
    Image dst = { minifiedImage.GetWidth(), minifiedImage.GetHeight(), &dstPlane.front(), minifiedImage.GetWidth() };

    // copy the plane into the vector, zoom, then copy it out
    for (int c = 0; c < 3; c++) {
        for (int i = 0; i < image.GetWidth() * image.GetHeight(); i++)
        {
            srcPlane[i] = image.GetData()[i * 3 + c];
        }
        
        zoom(&dst, &src, f, s);

        for (int i = 0; i < minifiedImage.GetWidth() * minifiedImage.GetHeight(); i++)
        {
            minifiedImage.GetData()[i * 3 + c] = dstPlane[i];
        }
    }

    if (image.HasAlpha())
    {
        minifiedImage.InitAlpha();
        src.data = image.GetAlpha();
        dst.data = minifiedImage.GetAlpha();

        zoom(&dst, &src, f, s);
    }

    return minifiedImage;
}
