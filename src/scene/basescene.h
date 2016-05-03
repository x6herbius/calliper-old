#ifndef BASESCENE_H
#define BASESCENE_H

#include <QObject>

class SceneObject;
class SceneCamera;
class MapScene;
class UIScene;
class MapDocument;

#define LOG_SCENE_OBJECT_LIFETIMES

#ifdef LOG_SCENE_OBJECT_LIFETIMES
#include <QtDebug>
#endif

class BaseScene : public QObject
{
    Q_OBJECT
    friend class SceneObject;
public:
    enum SceneType
    {
        TypeBaseScene = 0,
        TypeMapScene,
        TypeUIScene
    };

    explicit BaseScene(MapDocument* doc);
    virtual ~BaseScene();
    virtual SceneType type() const;

    // Convenience
    MapScene* mapScene();
    const MapScene* mapScene() const;
    UIScene* uiScene();
    const UIScene* uiScene() const;

    SceneObject* root() const;
    void clear();
    MapDocument* document() const;

    QList<SceneObject*> findByName(const QString &name);
    QList<SceneCamera*> findCameras() const;

    template<typename T, typename... Args>
    T* createSceneObject(Args... args)
    {
        T* obj = new T(this, std::move(args)...);
        Q_ASSERT(qobject_cast<SceneObject*>(obj));
        Q_ASSERT(obj->parentObject());

#ifdef LOG_SCENE_OBJECT_LIFETIMES
        qDebug() << "Scene object" << obj << "created";
#endif

        emit sceneObjectCreated(obj);
        return obj;
    }

    template<typename T>
    T* cloneSceneObject(const T* cloneFrom)
    {
        Q_ASSERT(cloneFrom->m_pScene == this);
        T* obj = new T(*cloneFrom);
        Q_ASSERT(qobject_cast<SceneObject*>(obj));

#ifdef LOG_SCENE_OBJECT_LIFETIMES
        qDebug() << "Scene object" << cloneFrom << "cloned.";
#endif

        emit sceneObjectCreated(obj);
        return obj;
    }

    SceneObject* unserialiseSceneObject(const QJsonObject &serialisedData, SceneObject* parent);
    void destroySceneObject(SceneObject* obj);

signals:
    void sceneObjectCreated(SceneObject* object);
    void subtreeDestroyed(SceneObject* object);

protected:
    SceneObject*    m_pRootObject;

private:
    void createRoot();
    void destroyRoot();
};

#endif // BASESCENE_H
