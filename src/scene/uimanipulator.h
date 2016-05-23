#ifndef UIMANIPULATOR_H
#define UIMANIPULATOR_H

#include "sceneobject.h"
#include <QList>
#include <QVector3D>
#include "callipermath.h"

class UIManipulator : public SceneObject
{
	Q_OBJECT
    friend class BaseScene;
public:
    enum ManipulationAxisFlag
    {
        AxisX = 0x1,
        AxisY = 0x2,
        AxisZ = 0x4,

        AxisXY = AxisX | AxisY,
        AxisXZ = AxisX | AxisZ,
        AxisYZ = AxisY | AxisZ,

        AxisXYZ = AxisX | AxisY | AxisZ
    };

    static QList<QVector3D> manipulationAxes(int axisFlags);
    static void clampToNearestMultiple(QVector3D &vec, qint64 multiple, int axisFlags);
	static Math::AxisIdentifier planeConstraintAxis(int axisFlags, const HierarchicalObject &camera);

    virtual SceneObject* clone() const;

    // UI elements should not be serialised.
    virtual bool serialiseToJson(QJsonObject &) const;

    virtual bool editable() const;
    virtual void draw(ShaderStack *stack);

    static int axisFlagsFromPickColor(QRgb colour);
    static int axisIdentifierFromPickColor(QRgb colour);

protected:
    explicit UIManipulator(BaseScene* scene, SceneObject* parent);
    UIManipulator(const UIManipulator &cloneFrom);
    virtual ~UIManipulator();

    void initDefaults();

    const static QRgb PICKCOLOUR_X;
    const static QRgb PICKCOLOUR_Y;
    const static QRgb PICKCOLOUR_Z;
    const static QRgb PICKCOLOUR_XY;
    const static QRgb PICKCOLOUR_YZ;
    const static QRgb PICKCOLOUR_XZ;
    const static unsigned int PICKMASK;

    const static QRgb HOVERCOLOUR_X;
    const static QRgb HOVERCOLOUR_Y;
    const static QRgb HOVERCOLOUR_Z;
    const static QRgb HOVERCOLOUR_XY;
    const static QRgb HOVERCOLOUR_YZ;
    const static QRgb HOVERCOLOUR_XZ;

    QVector<int>    m_SectionsToDraw;
};

#endif // UIMANIPULATOR_H
