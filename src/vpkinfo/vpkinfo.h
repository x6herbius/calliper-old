#ifndef VPKINFO_H
#define VPKINFO_H

#include <QDataStream>
#include "vpk/vpkheader.h"
#include "vpk/vpkindex.h"
#include "vpk/vpkarchivemd5collection.h"
#include "vpk/vpkothermd5item.h"
#include <QStringList>

namespace VPKInfo
{
    void printHeaderData(const FileFormats::VPKHeader& header, const QStringList& siblingArchives);
    void printIndexData(const FileFormats::VPKIndex& index);
    void printArchiveMD5Data(const FileFormats::VPKArchiveMD5Collection& collection,
                             const QStringList& archives, bool verbose = false);
    void printOtherMD5Data(const FileFormats::VPKOtherMD5Item& md5s,
                           const QByteArray& treeData, const QByteArray& archiveMD5Data);
}

#endif // VPKINFO_H
