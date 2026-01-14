#include "overseer-nested-scene.h"

#include <QUuid>



OverseerNestedScene::OverseerNestedScene(const QString& overlayId, int width, int height)

    : overlayId_(overlayId), nestedWidth_(width), nestedHeight_(height) {

    nestedSceneName_ = QString("OS-%1-%2").arg(overlayId).arg(QUuid::createUuid().toString().mid(1, 8));

}



OverseerNestedScene::~OverseerNestedScene() {

    if (sceneItem_) {

        obs_scene_t* main = getMainScene();

        if (main) obs_scene_remove_item(main, sceneItem_);

        obs_sceneitem_release(sceneItem_);

        sceneItem_ = nullptr;

    }

    if (nestedScene_) {

        obs_scene_release(nestedScene_);

        nestedScene_ = nullptr;

    }

}



bool OverseerNestedScene::createNestedScene() {

    obs_scene_t* scene = obs_scene_create(nestedSceneName_.toUtf8().constData());

    if (!scene) return false;

    nestedScene_ = scene;

    obs_source_t* scene_source = obs_scene_get_source(nestedScene_);

    if (!scene_source) return false;

    obs_scene_t* main = getMainScene();

    if (!main) return false;

    sceneItem_ = obs_scene_add(main, scene_source);

    obs_source_release(scene_source);

    if (!sceneItem_) return false;

    struct obs_transform_info info = {};

    info.bounds.x = (float)nestedWidth_;

    info.bounds.y = (float)nestedHeight_;

    info.bounds_type = OBS_BOUNDS_FIXED;

    info.bounds_alignment = OBS_ALIGN_TOP_LEFT;

    info.alignment = OBS_ALIGN_TOP_LEFT;

    info.pos = {0, 0};

    info.rot = 0;

    info.scale = {1.0f, 1.0f};

    obs_sceneitem_set_info2(sceneItem_, &info);

    return true;

}



obs_sceneitem_t* OverseerNestedScene::addSourceToNested(obs_source_t* source, const QString& sourceName,

                                                       int x, int y, int width, int height, uint32_t alignment) {

    if (!nestedScene_ || !source) return nullptr;

    obs_sceneitem_t* item = obs_scene_add(nestedScene_, source);

    if (!item) return nullptr;

    struct obs_transform_info info = {};

    info.pos = {(float)x, (float)y};

    uint32_t src_width = obs_source_get_width(source);

    uint32_t src_height = obs_source_get_height(source);

    if (src_width > 0 && src_height > 0) {

        info.scale = {(float)width / (float)src_width, (float)height / (float)src_height};

    } else {

        info.scale = {1.0f, 1.0f};

    }

    info.rot = 0;

    info.alignment = alignment;

    obs_sceneitem_set_info2(item, &info);

    childItems_[sourceName] = item;

    return item;

}



obs_scene_t* OverseerNestedScene::getMainScene() {

    return obs_frontend_get_current_scene();

}



void OverseerNestedScene::setMainCanvasPosition(float x, float y) {

    if (!sceneItem_) return;

    struct vec2 pos = {x, y};

    obs_sceneitem_set_pos(sceneItem_, &pos);

}



void OverseerNestedScene::getMainCanvasPosition(float& x, float& y) const {

    if (!sceneItem_) { x = 0; y = 0; return; }

    struct vec2 pos;

    obs_sceneitem_get_pos(sceneItem_, &pos);

    x = pos.x; y = pos.y;

}



void OverseerNestedScene::setMainCanvasScale(float scaleX, float scaleY) {

    if (!sceneItem_) return;

    if (scaleY < 0) scaleY = scaleX;

    struct vec2 scale = {scaleX, scaleY};

    obs_sceneitem_set_scale(sceneItem_, &scale);

}



void OverseerNestedScene::getMainCanvasScale(float& scaleX, float& scaleY) const {

    if (!sceneItem_) { scaleX = 1.0f; scaleY = 1.0f; return; }

    struct vec2 scale;

    obs_sceneitem_get_scale(sceneItem_, &scale);

    scaleX = scale.x; scaleY = scale.y;

}



void OverseerNestedScene::setVisible(bool visible) {

    if (!sceneItem_) return;

    obs_sceneitem_set_visible(sceneItem_, visible);

}



bool OverseerNestedScene::isVisible() const {

    if (!sceneItem_) return false;

    return obs_sceneitem_visible(sceneItem_);

}



obs_source_t* OverseerNestedScene::getChildSource(const QString& sourceName) {

    auto it = childItems_.find(sourceName);

    if (it == childItems_.end()) return nullptr;

    return obs_sceneitem_get_source(*it);

}



void OverseerNestedScene::updateSourceProperty(const QString& sourceName, const QString& propertyName, const QVariant& value) {

    auto it = childItems_.find(sourceName);

    if (it == childItems_.end()) return;

    obs_source_t* source = obs_sceneitem_get_source(*it);

    if (!source) return;

    obs_data_t* settings = obs_source_get_settings(source);

    if (!settings) return;

    if (value.type() == QVariant::String) {

        obs_data_set_string(settings, propertyName.toUtf8().constData(), value.toString().toUtf8().constData());

    } else if (value.type() == QVariant::Int) {

        obs_data_set_int(settings, propertyName.toUtf8().constData(), value.toInt());

    } else if (value.type() == QVariant::Double) {

        obs_data_set_double(settings, propertyName.toUtf8().constData(), value.toDouble());

    } else if (value.type() == QVariant::Bool) {

        obs_data_set_bool(settings, propertyName.toUtf8().constData(), value.toBool());

    }

    obs_source_update(source, settings);

    obs_data_release(settings);

}
