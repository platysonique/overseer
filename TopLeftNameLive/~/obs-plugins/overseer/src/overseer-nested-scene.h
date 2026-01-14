#pragma once



#include <obs.h>

#include <QString>

#include <QMap>

#include <QVariant>

#include <memory>



class OverseerNestedScene {

public:

    OverseerNestedScene(const QString& overlayId, int width, int height);

    ~OverseerNestedScene();

    

    bool createNestedScene();

    obs_sceneitem_t* addSourceToNested(obs_source_t* source, const QString& sourceName,

                                       int x, int y, int width, int height);

    obs_scene_t* getNestedScene() { return nestedScene_; }

    obs_sceneitem_t* getSceneItem() { return sceneItem_; }

    void setMainCanvasPosition(float x, float y);

    void getMainCanvasPosition(float& x, float& y) const;

    void setMainCanvasScale(float scaleX, float scaleY = -1.0f);

    void getMainCanvasScale(float& scaleX, float& scaleY) const;

    void setVisible(bool visible);

    bool isVisible() const;

    QString getNestedSceneName() const { return nestedSceneName_; }

    obs_source_t* getChildSource(const QString& sourceName);

    void updateSourceProperty(const QString& sourceName, const QString& propertyName, const QVariant& value);



private:

    QString overlayId_;

    QString nestedSceneName_;

    int nestedWidth_;

    int nestedHeight_;

    obs_scene_t* nestedScene_ = nullptr;

    obs_sceneitem_t* sceneItem_ = nullptr;

    QMap<QString, obs_sceneitem_t*> childItems_;

    obs_scene_t* getMainScene();

};