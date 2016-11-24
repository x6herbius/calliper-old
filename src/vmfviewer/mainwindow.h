#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QOpenGLWindow>
#include "stores/shaderstore.h"
#include "stores/texturestore.h"
#include "scene/basicscene.h"
#include "camera/scenecamera.h"
#include "simplerenderpassclassifier.h"
#include "scenerenderer/scenerenderer.h"
#include "camera/cameracontroller.h"
#include "controller-adapters/keymap.h"
#include "controller-adapters/mouseeventmap.h"

class QPushButton;
class QGridLayout;

class MainWindow : public QOpenGLWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const QString& filename);
    ~MainWindow();

    void setVpkPath(const QString& path);

protected:
    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void mousePressEvent(QMouseEvent *e) override;
    virtual void mouseReleaseEvent(QMouseEvent *e) override;

private:
    void destroy();
    void initShaders();
    void initTextures();
    void initLocalOpenGlSettings();
    void initRenderer();
    void initScene();
    void initSceneRenderer();
    void initCameraController();
    void initKeyMap();
    void initMouseEventMap();

    void loadVMF();
    void processBrushes();

    QString m_strFilename;
    QString m_strVpkPath;

    Model::ShaderStore* m_pShaderStore;
    Model::TextureStore* m_pTextureStore;
    quint32 m_iDefaultTexture;

    Model::BasicScene* m_pScene;
    Model::SceneCamera* m_pCamera;
    SimpleRenderPassClassifier m_RenderPassClassifier;
    Model::SceneRenderer* m_pSceneRenderer;
    Model::ShaderPalette m_DefaultShaderPalette;

    Model::CameraController* m_pCameraController;
    Model::KeyMap* m_pKeyMap;
    Model::MouseEventMap* m_pMouseEventMap;
};

#endif // MAINWINDOW_H
