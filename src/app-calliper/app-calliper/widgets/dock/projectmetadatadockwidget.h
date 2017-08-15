#ifndef PROJECTMETADATADOCKWIDGET_H
#define PROJECTMETADATADOCKWIDGET_H

#include <QPointer>

#include "model/projects/calliperprojectmetadata.h"

#include "user-interface/widgets/visibility-action/visibilityactiondockwidget.h"

class ProjectMetadataDockWidget : public UserInterface::VisibilityActionDockWidget
{
    Q_OBJECT
public:
    ProjectMetadataDockWidget(QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = Qt::WindowFlags());

    Model::CalliperProjectMetadata* projectMetadata() const;
    void setProjectMetadata(Model::CalliperProjectMetadata* metadata);

private slots:
    void uiDelegateEdited();
    void propertyUpdated(int propertyIndex);

private:
    void setupUi();
    void addLineEditRow(const QString& labelText, QWidget* widget, const char* widgetSignal, const QString& propertyName);
    void updateUiEnabledState();
    void transitionSignals(Model::CalliperProjectMetadata* oldObj, Model::CalliperProjectMetadata* newObj);
    void clearUiDelegates();
    void updateAllUiDelegates();

    QPointer<Model::CalliperProjectMetadata> m_pProjectMetadata;
    QWidget* m_pContentsWidget;

    QHash<QWidget*, QString> m_WidgetToPropertyName;
    QHash<QString, QWidget*> m_PropertyNameToWidget;
};

#endif // PROJECTMETADATADOCKWIDGET_H
