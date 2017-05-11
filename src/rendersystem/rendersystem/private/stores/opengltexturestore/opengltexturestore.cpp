#include "opengltexturestore.h"

OpenGLTextureStore::TextureId OpenGLTextureStore::addTexture(const QImage& image, const QString& path)
{
    TextureId existingTexture = m_PathToTextureIdMap.value(path, INVALID_ID);
    if ( existingTexture != INVALID_ID )
    {
        destroy(existingTexture);
    }

    return create(path, image);
}

void OpenGLTextureStore::removeTexture(const TextureId id)
{
    if ( id == INVALID_ID )
    {
        return;
    }

    destroy(id);
}

bool OpenGLTextureStore::containsTexture(const TextureId id) const
{
    return contains(id);
}

OpenGLTextureStore::TextureId OpenGLTextureStore::textureIdFromPath(const QString& path) const
{
    return objectIdFromPath(path);
}

QString OpenGLTextureStore::texturePathFromId(const TextureId id) const
{
    return objectPathFromId(id);
}

void OpenGLTextureStore::objectCreated(const ObjectId id)
{
    storePathForCreatedObject(id);
}

void OpenGLTextureStore::objectAboutToBeDestroyed(const ObjectId id)
{
    removePathForDestroyedObject(id);
}
