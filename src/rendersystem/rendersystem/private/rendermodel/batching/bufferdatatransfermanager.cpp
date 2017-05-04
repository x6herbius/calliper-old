#include "bufferdatatransfermanager.h"

#include "rendersystem/private/shaders/common/privateshaderdefs.h"
#include "rendersystem/private/opengl/openglerrors.h"

namespace
{
    const quint32 SIZEOF_MATRIX_4X4 = 4 * 4 * sizeof(float);

    // Assumes destination is large enough to hold all required elements.
    template<typename T>
    void consolidate(const QVector<T> &source,
                     QVector<T> &destination,
                     int components,
                     int offset,
                     int stride)
    {
        if ( stride <= 0 )
        {
            stride = 1;
        }

        for ( int srcItem = 0, destItem = offset; srcItem < source.count(); srcItem += components, destItem += stride)
        {
            for ( int curComponent = 0; curComponent < components; ++curComponent )
            {
                destination[destItem + curComponent] = source[srcItem + curComponent];
            }
        }
    }

    inline quint32 maskFromNumberOfBits(int numBits)
    {
        return ~(static_cast<quint32>(0)) >> ((sizeof(quint32) * 8) - numBits);
    }

    inline quint8 bitsRequired(int maxValue)
    {
        for ( quint32 i = 0; i < sizeof(quint32) * 8; i++ )
        {
            if ( (1 << i) >= maxValue )
            {
                return i+1;
            }
        }

        return sizeof(quint32) * 8;
    }
}

BufferDataTransferManager::BufferDataTransferManager(OpenGLBufferCollection &buffers,
                                                     BufferDataContainer &data,
                                                     const VertexFormat &vertexFormat)
    : m_Buffers(buffers),
      m_Data(data),
      m_VertexFormat(vertexFormat),
      m_nVertexFloatsUploadedSoFar(0),
      m_nIndexIntsUploadedSoFar(0),
      m_pVertexBufferPointer(Q_NULLPTR),
      m_pIndexBufferPointer(Q_NULLPTR)
{

}

bool BufferDataTransferManager::ensureUploaded()
{
    if ( !m_Buffers.isCreated() )
    {
        return false;
    }

    if ( !anyDirtyObjectData() )
    {
        return true;
    }

    // We don't do any clever optimisations here - if anything is dirty,
    // reallocate the buffer. Profiling may show whether this approach
    // is too slow later on.

    m_nVertexFloatsUploadedSoFar = 0;
    m_nIndexIntsUploadedSoFar = 0;

    if ( !allocateAndMapBuffers() )
    {
        unmapBuffers();
        return false;
    }

    for ( int object = 0; object < m_Data.totalBatchItems(); ++object )
    {
        uploadData(object);
    }

    unmapBuffers();
    return true;
}

void BufferDataTransferManager::uploadData(int index)
{
    QSharedPointer<ObjectSectionGeometryData> objectGeometry = m_Data.objectData(index);

    GLTRY(m_Buffers.uniformBuffer().write(index * SIZEOF_MATRIX_4X4, objectGeometry->modelToWorldMatrix().constData(), SIZEOF_MATRIX_4X4));

    QVector<float> vertexData;
    QVector<quint32> indexData;
    quint8 batchId = index % m_Data.maxItemsPerBatch();

    consolidateVetexData(objectGeometry, vertexData, batchId);
    consolidateIndexData(objectGeometry, indexData);

    quint32 indexIncrement = m_nVertexFloatsUploadedSoFar / m_VertexFormat.totalVertexComponents();

    uploadVertexData(vertexData, m_nVertexFloatsUploadedSoFar);
    uploadIndexData(indexData, m_nIndexIntsUploadedSoFar, indexIncrement);

    m_nVertexFloatsUploadedSoFar += vertexData.count();
    m_nIndexIntsUploadedSoFar += indexData.count();

    objectGeometry->markAsCleaned();
}

bool BufferDataTransferManager::allocateAndMapBuffers()
{
    if ( !m_Buffers.vertexBuffer().bind() || !m_Buffers.indexBuffer().bind() )
    {
        return false;
    }

    quint32 vertexBufferSize = totalRequiredVertexBytes();
    m_Buffers.vertexBuffer().allocate(vertexBufferSize);
    m_pVertexBufferPointer = reinterpret_cast<float*>(m_Buffers.vertexBuffer().map(QOpenGLBuffer::WriteOnly));

    quint32 indexBufferSize = totalRequiredIndexBytes();
    m_Buffers.indexBuffer().allocate(indexBufferSize);
    m_pIndexBufferPointer = reinterpret_cast<quint32*>(m_Buffers.indexBuffer().map(QOpenGLBuffer::WriteOnly));

    OpenGLUniformBuffer& uniformBuffer = m_Buffers.uniformBuffer();

    // Warning: The specific order of these commands may be important.
    // UBOs are finnicky.

    GLTRY(uniformBuffer.bind());
    GLTRY(uniformBuffer.allocate(m_Data.totalBatchItems() * SIZEOF_MATRIX_4X4));
    GLTRY(uniformBuffer.release());
    GLTRY(uniformBuffer.bindToIndex(PrivateShaderDefs::LocalUniformBlockBindingPoint));

    return m_pVertexBufferPointer && m_pIndexBufferPointer;
}

void BufferDataTransferManager::unmapBuffers()
{
    m_Buffers.vertexBuffer().unmap();
    m_Buffers.indexBuffer().unmap();

    m_Buffers.vertexBuffer().release();
    m_Buffers.indexBuffer().release();
    m_Buffers.uniformBuffer().release();
}

void BufferDataTransferManager::uploadVertexData(const QVector<float> &vertexData, int offsetInFloats)
{
    float* bufferPointer = m_pVertexBufferPointer + offsetInFloats;
    memcpy(bufferPointer, vertexData.constData(), vertexData.count() * sizeof(float));
}

void BufferDataTransferManager::uploadIndexData(const QVector<quint32> &indexData, int offsetInInts, quint32 indexIncrement)
{
    quint32* bufferPointer = m_pIndexBufferPointer + offsetInInts;

    for ( int i = 0; i < indexData.count(); ++i )
    {
        bufferPointer[i] = indexData.at(i) + indexIncrement;
    }
}

bool BufferDataTransferManager::anyDirtyObjectData() const
{
    for ( int i = 0; i < m_Data.totalBatchItems(); ++i )
    {
        if ( m_Data.objectData(i)->isDirty() )
        {
            return true;
        }
    }

    return false;
}

void BufferDataTransferManager::markAllObjectsDirty()
{
    for ( int i = 0; i < m_Data.totalBatchItems(); ++i )
    {
        m_Data.objectData(i)->markAsDirty();
    }
}

quint32 BufferDataTransferManager::totalRequiredVertexBytes() const
{
    quint32 bytes = 0;

    for ( int i = 0; i < m_Data.totalBatchItems(); ++i )
    {
        bytes += m_Data.objectData(i)->computeTotalVertexBytes();
    }

    return bytes;
}

quint32 BufferDataTransferManager::totalRequiredIndexBytes() const
{
    quint32 bytes = 0;

    for ( int i = 0; i < m_Data.totalBatchItems(); ++i )
    {
        bytes += m_Data.objectData(i)->computeTotalIndexBytes();
    }

    return bytes;
}

void BufferDataTransferManager::consolidateVetexData(const QSharedPointer<ObjectSectionGeometryData> &objectGeometry,
                                                     QVector<float> &vertexData,
                                                     quint8 batchId)
{
    quint32 totalVertexDataBytes = objectGeometry->computeTotalVertexBytes();
    int totalVertexComponents = m_VertexFormat.totalVertexComponents();

    vertexData.resize(totalVertexDataBytes);

    int beginOffset = 0;

    consolidate<float>(objectGeometry->positions(),
                         vertexData,
                         m_VertexFormat.positionComponents(),
                         beginOffset,
                         totalVertexComponents);

    insertBatchId(vertexData, batchId, beginOffset + m_VertexFormat.positionComponents() - 1, totalVertexComponents);

    beginOffset += m_VertexFormat.positionComponents();

    consolidate<float>(objectGeometry->normals(),
                         vertexData,
                         m_VertexFormat.normalComponents(),
                         beginOffset,
                         totalVertexComponents);

    beginOffset += m_VertexFormat.normalComponents();

    consolidate<float>(objectGeometry->colors(),
                         vertexData,
                         m_VertexFormat.colorComponents(),
                         beginOffset,
                         totalVertexComponents);

    beginOffset += m_VertexFormat.colorComponents();

    consolidate<float>(objectGeometry->textureCoordinates(),
                         vertexData,
                         m_VertexFormat.textureCoordinateComponents(),
                         beginOffset,
                         totalVertexComponents);
}

void BufferDataTransferManager::consolidateIndexData(const QSharedPointer<ObjectSectionGeometryData> &objectGeometry,
                                                     QVector<quint32> &indexData)
{
    quint32 totalIndexBytes = objectGeometry->computeTotalIndexBytes();
    indexData.resize(totalIndexBytes);

    consolidate<quint32>(objectGeometry->indices(),
                         indexData,
                         1,
                         0,
                         1);
}

void BufferDataTransferManager::insertBatchId(QVector<float>& vertexData, quint8 batchId, int offset, int stride)
{
    quint32 batchIdMask = maskFromNumberOfBits(bitsRequired(m_Data.maxItemsPerBatch()));
    quint32 batchIdToWrite = static_cast<quint32>(batchId) & batchIdMask;

    for ( int i = offset; i < vertexData.count(); i += stride )
    {
        vertexData[i] = static_cast<float>(static_cast<quint32>(vertexData[i]) | batchIdToWrite);
    }
}
