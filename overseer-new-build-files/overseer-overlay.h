#pragma once



#include <obs.h>

#include <QString>

#include <memory>

#include "overseer-types.h"

#include "overseer-nested-scene.h"



class OverseerOverlay {

public:

    OverseerOverlay(const QString& overlayId, OverlayType type);

    ~OverseerOverlay();

    

    void createOBSSource();

    void removeOBSSource();

    void setVisible(bool visible);

    bool isVisible() const;

    QString getOverlayId() const { return overlayId_; }

    OverlayType getType() const { return type_; }

    obs_sceneitem_t* getSceneItem() const;

    OverlayParameters* getParameters() { return params_.get(); }

    void updateChildSourceProperty(const QString& sourceName, const QString& propertyName, const QVariant& value);

    void setMainCanvasPosition(float x, float y);

    void getMainCanvasPosition(float& x, float& y) const;

    void setMainCanvasScale(float scale);

    void getMainCanvasScale(float& scale) const;

    OverseerNestedScene* getNestedScene() { return nestedScene_.get(); }



private:

    QString overlayId_;

    OverlayType type_;

    std::unique_ptr<OverlayParameters> params_;

    std::unique_ptr<OverseerNestedScene> nestedScene_;

    void createLowerThirdNested();

    void createLogoNested();

    void createLiveBugNested();

    void createBigLower3rdNested();

    void createCountdownNested();

};
