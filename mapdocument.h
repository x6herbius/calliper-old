#ifndef MAPDOCUMENT_H
#define MAPDOCUMENT_H

#include <QObject>
#include <QColor>
#include <QSet>

class Scene;
class SceneObject;
class BaseTool;

class MapDocument : public QObject
{
    Q_OBJECT
public:
    explicit MapDocument(QObject* parent = 0);
    ~MapDocument();

    Scene* scene() const;

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &col);

    const QSet<SceneObject*>& selectedSet() const;
    void selectedSetInsert(SceneObject* object);
    void selectedSetRemove(SceneObject* object);
    void selectedSetClear();

    QColor selectedColor() const;
    void setSelectedColor(const QColor &col);

    BaseTool* tool(int index) const;
    BaseTool* tool(const QString &name) const;
    int toolCount() const;

    int activeTool() const;
    void setActiveTool(int index);

private:
    void createTools();
    void switchTool(BaseTool* oldTool, BaseTool* newTool);

    Scene*  m_pScene;
    QColor  m_colBackground;
    QSet<SceneObject*>  m_SelectedSet;
    QColor  m_colSelected;
    QList<BaseTool*>    m_Tools;
    int m_iActiveTool;
};

#endif // MAPDOCUMENT_H
