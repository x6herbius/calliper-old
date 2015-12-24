#include "sceneobject.h"
#include "callipermath.h"

SceneObject::SceneObject(SceneObject *parent) : QObject(parent)
{
    m_pGeometry.reset(new GeometryData());
    m_bMatricesStale = true;
}

GeometryData* SceneObject::geometry() const
{
    return m_pGeometry.data();
}

void SceneObject::setGeometry(GeometryData *data)
{
    if ( !data || m_pGeometry.data() == data ) return;

    m_pGeometry.reset(data);
}

QVector3D SceneObject::position() const
{
    return m_vecPosition;
}

void SceneObject::setPosition(const QVector3D &pos)
{
    if ( m_vecPosition == pos ) return;

    m_vecPosition = pos;
    m_bMatricesStale = true;
}

void SceneObject::rebuildMatrices() const
{
    m_matParentToLocal = Math::matrixTranslate(-m_vecPosition);
    m_matLocalToParent = m_matParentToLocal.inverted();
    m_bMatricesStale = false;
}

QMatrix4x4 SceneObject::parentToLocal() const
{
    if ( m_bMatricesStale )
        rebuildMatrices();

    return m_matParentToLocal;
}

QMatrix4x4 SceneObject::localToParent() const
{
    if ( m_bMatricesStale )
        rebuildMatrices();

    return m_matLocalToParent;
}

bool SceneObject::isEmpty() const
{
    return m_pGeometry->isEmpty();
}
