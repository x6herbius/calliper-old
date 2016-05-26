#include "crosshair3d.h"
#include "pervertexcolorshader.h"

Crosshair3D::Crosshair3D(BaseScene *scene, SceneObject *parent) : SceneObject(scene, parent)
{
    constructGeometry();
}

Crosshair3D::Crosshair3D(const Crosshair3D &cloneFrom) : SceneObject(cloneFrom)
{
    // TODO
}

Crosshair3D::~Crosshair3D()
{

}

void Crosshair3D::constructGeometry()
{
    GeometryData* geom = new GeometryData();
    geom->setDrawMode(GL_LINES);
    geom->setShaderOverride(PerVertexColorShader::staticName());

    geom->appendVertex(QVector3D(1,0,0), QVector3D(), QColor::fromRgb(0xffffffff));
    geom->appendVertex(QVector3D(-1,0,0), QVector3D(), QColor::fromRgb(0xffffffff));

    geom->appendVertex(QVector3D(0,1,0), QVector3D(), QColor::fromRgb(0xffffffff));
    geom->appendVertex(QVector3D(0,-1,0), QVector3D(), QColor::fromRgb(0xffffffff));

    geom->appendVertex(QVector3D(0,0,1), QVector3D(), QColor::fromRgb(0xffffffff));
    geom->appendVertex(QVector3D(0,0,-1), QVector3D(), QColor::fromRgb(0xffffffff));

    appendGeometry(geom);
}
