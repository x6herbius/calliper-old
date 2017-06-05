#include "rendermodel.h"
#include "rendermodel-defs.h"

#include "rendersystem/interface-classes/definitions/materialdefs.h"

RenderModel::RenderModel()
{

}

RenderModel::~RenderModel()
{
    clear();
}

void RenderModel::setGeometry(const RenderSystem::GeometryBuilder &geometry)
{
    removeGeometry(geometry.objectId());

    for ( int i = 0; i < geometry.sectionCount(); ++i )
    {
        setGeometry(geometry.section(i));
    }
}

void RenderModel::setGeometry(const QSharedPointer<RenderSystem::GeometrySection> &section)
{
    if ( section->isEmpty() )
    {
        return;
    }

    RenderSystem::MaterialDefs::MaterialId key = section->materialId();
    RenderGroupPointer renderGroup = m_RenderGroups.value(key, RenderGroupPointer());

    if ( !renderGroup )
    {
        renderGroup = RenderGroupPointer::create(m_Context, key);
        m_RenderGroups.insert(key, renderGroup);
    }

    renderGroup->setGeometry(section);
}

void RenderModel::removeGeometry(RenderSystem::RenderModelDefs::ObjectId objectId)
{
    QList<RenderGroupPointer> renderGroups = m_ObjectIdToRenderGroup.values(objectId);

    for ( const RenderGroupPointer& group : renderGroups )
    {
        group->removeGeometry(objectId);
    }

    m_ObjectIdToRenderGroup.remove(objectId);
}

void RenderModel::clear()
{
    m_RenderGroups.clear();
    m_ObjectIdToRenderGroup.clear();
}

void RenderModel::draw(RenderSystem::ShaderDefs::RenderMode renderMode,
                       const QMatrix4x4 &worldToCameraMatrix,
                       const QMatrix4x4 &projectionMatrix)
{
    m_Context.setRenderMode(renderMode);

    static_assert(false, "Use global shader uniforms here.");

    for ( RenderGroupHash::const_iterator itGroup = m_RenderGroups.constBegin();
          itGroup != m_RenderGroups.constEnd();
          ++itGroup )
    {
        itGroup.value()->draw();
    }
}
