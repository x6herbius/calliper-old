#ifndef VPKOTHERMD5ITEM_H
#define VPKOTHERMD5ITEM_H

#include "file-formats_global.h"
#include "common/streamdatacontainer.h"

namespace FileFormats
{
    class FILEFORMATSSHARED_EXPORT VPKOtherMD5Item : public StreamDataContainer
    {
    public:
        VPKOtherMD5Item();
        ~VPKOtherMD5Item();

        virtual bool populate(QDataStream& stream, QString* errorHint = nullptr) override;
        void clear();

        QByteArray treeChecksum() const;
        QByteArray archiveMD5SectionChecksum() const;
        QByteArray unknownProperty1() const;

        static quint32 staticSize();

    protected:
        virtual QString containerName() const override;

    private:
        struct Data;
        Data* m_pData;
    };
}

#endif // VPKOTHERMD5ITEM_H
