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

#ifndef _QPLASMAPAKFILE_H
#define _QPLASMAPAKFILE_H

#include "QPlasmaDocument.h"
#include <PRP/Surface/plFont.h>
#include <QTreeWidget>
#include <QAction>
#include <vector>

struct PlasmaPackage
{
    struct FileBlob
    {
        struct _data
        {
            const uint8_t* fData;
            size_t fSize;
            unsigned int fRefs;
        } *fData;

        FileBlob() : fData() { }

        FileBlob(const uint8_t* data, size_t size)
        {
            fData = new _data;
            fData->fData = data;
            fData->fSize = size;
            fData->fRefs = 1;
        }

        ~FileBlob()
        {
            if (fData != NULL && (--fData->fRefs == 0))
                delete fData;
        }

        FileBlob(const FileBlob& copy) : fData(copy.fData)
        {
            if (fData != NULL)
                ++fData->fRefs;
        }

        FileBlob& operator=(const FileBlob& copy)
        {
            if (fData != NULL && (--fData->fRefs == 0))
                delete fData;
            fData = copy.fData;
            if (fData != NULL)
                ++fData->fRefs;
            return *this;
        }

        const uint8_t* getData() const { return fData ? fData->fData : NULL; }
        size_t getSize() const { return fData ? fData->fSize : 0; }
    };

    struct FileEntry
    {
        ST::string fName;
        uint32_t fOffset;
        FileBlob fData;
        plFont fFontData;

        FileEntry() { }
    };

    enum PackageType
    {
        kPythonPak, kCursorsDat, kFontsPfp,
        kMyst5Arc = 0xCBBCF00D,
    };

    PackageType fType;
    uint32_t fUnknown;
    std::vector<FileEntry> fEntries;

    PlasmaPackage() : fType(kPythonPak), fUnknown(1) { }

    void read(hsStream* S);
    void write(hsStream* S);

    void addFrom(QString filename);
    void writeToFile(const FileEntry& ent, QString filename);

    QString displayName(const FileEntry& ent) const;
    QString displaySize(const FileEntry& ent) const;
    QString getFilter() const;
};

class QPlasmaPakFile : public QPlasmaDocument
{
    Q_OBJECT

public:
    enum OverwritingConfirmation { Ask, YesToAll, NoToAll };
    
    QPlasmaPakFile(QWidget* parent);

    bool loadFile(const QString& filename) override;
    bool saveTo(const QString& filename) override;

    void setPackageType(PlasmaPackage::PackageType type)
    {
        fPackage.fType = type;
    }
    PlasmaPackage::PackageType packageType() const { return fPackage.fType; }
    
    /**
     * Decompile a pyc file.
     * @param parent the widget to use as aprent for questions and messages
     * @param filename the file to decompyle
     * @param confirmation a point to the confirmation state for overwriting files: Set it to NULL if the user should be
     *      always asked without a way to say yes/no to all.
     * @return whether the file was successfully decompiled
     */
    static bool decompylePyc(QWidget *parent, QString filename, OverwritingConfirmation *confirmation = NULL);

private:
    QTreeWidget* fFileList;
    PlasmaPackage fPackage;

    enum
    {
        kAdd, kDel, kExtract, kExtractAll, kANumActions
    };
    QAction* fActions[kANumActions];

    bool loadPakData(hsStream* S);
    bool savePakData(hsStream* S);
    void extract(const PlasmaPackage::FileEntry &entry, QString dir, OverwritingConfirmation *confirmation);

private slots:
    void onContextMenu(QPoint pos);
    void onAdd();
    void onDel();
    void onExtract();
    void onExtractAll();
};

#endif
