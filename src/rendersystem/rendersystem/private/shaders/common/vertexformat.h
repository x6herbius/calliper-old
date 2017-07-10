#ifndef VERTEXFORMAT_H
#define VERTEXFORMAT_H

#include "privateshaderdefs.h"

class VertexFormat
{
public:
    VertexFormat();
    VertexFormat(int positions, int normals, int colors, int texCoords);

    // Each attribute can have a maximum of 4 components,
    // as each shader location is a vec4.
    // 0 components indicates that this attribute is not supported.
    // The final position component will be used to store the
    // object ID if batching occurs.
    int positionComponents() const;
    int normalComponents() const;
    int colorComponents() const;
    int textureCoordinateComponents() const;

    int components(PrivateShaderDefs::VertexArrayAttribute attribute) const;

    int totalVertexComponents() const;
    bool isValid() const;

    bool operator ==(const VertexFormat& other) const;
    bool operator !=(const VertexFormat& other) const;

private:
    int m_nComponents[PrivateShaderDefs::VertexAttributeLocationCount];
};

#endif // VERTEXFORMAT_H
