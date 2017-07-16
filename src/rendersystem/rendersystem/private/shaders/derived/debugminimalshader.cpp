#include "debugminimalshader.h"
#include "rendersystem/private/shaders/common/vertexformat.h"

DebugMinimalShader::DebugMinimalShader(QObject *parent)
    : OpenGLShaderProgram(DebugMinimalShader::staticMetaObject.className(), parent)
{

}

DebugMinimalShader::~DebugMinimalShader()
{

}


RenderSystem::ShaderDefs::ShaderId DebugMinimalShader::shaderId()
{
    return RenderSystem::ShaderDefs::DebugMinimalShaderId;
}

void DebugMinimalShader::construct()
{
    addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/debugminimalshader.vert");
    addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/debugminimalshader.frag");
    link();
}

VertexFormat DebugMinimalShader::vertexFormat() const
{
    return VertexFormat(2, 0, 0, 0);
}

int DebugMinimalShader::maxBatchedItems() const
{
    return 1;
}
