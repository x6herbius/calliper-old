#ifndef VMF_H
#define VMF_H

#include "map-importers_global.h"
#include "scene/sceneobject.h"

namespace MapImporters
{
    namespace VMF
    {
        MAPIMPORTERSSHARED_EXPORT void createBrushes(const QJsonDocument &doc, NS_MODEL::SceneObject* parent);
    }
}

#endif // VMF_H
