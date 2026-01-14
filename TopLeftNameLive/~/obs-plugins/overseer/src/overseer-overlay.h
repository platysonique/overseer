#ifndef OVERSEER_OVERLAY_H
#define OVERSEER_OVERLAY_H

#include "overseer-types.h"
#include "overseer-nested-scene.h"
#include <obs.h>

// Forward declarations
class OverlayParameters;

class OverseerOverlay {
public:
    OverseerOverlay(const QString& overlayId, OverlayType type);
    ~OverseerOverlay();

    // Core overlay management
    bool createOBSSource();
    obs_source_t* getOBSSource() const;
    OverlayParameters* getParameters() const { return parameters; }

    // Parameter updates
    void updateParameters(OverlayParameters* newParams);
    void updatePosition(int x, int y);
    void updateScale(float scale);
    void setVisible(bool visible);

    // Type-specific creation methods
    bool createLowerThirdOverlay();
    bool createLogoOverlay();
    bool createLiveBugOverlay();
    bool createBigLower3rdOverlay();
    bool createCountdownOverlay();

    // Property updates for real-time changes
    void updateTextProperty(const QString& textType, const QString& text);
    void updateColorProperty(const QString& colorType, const QColor& color);
    void updateFontProperty(const QString& fontFamily, int fontSize);
    void updateImageProperty(const QString& imageFile);

private:
    QString overlayId;
    OverlayType type;
    OverlayParameters* parameters = nullptr;
    OverseerNestedScene* nestedScene = nullptr;

    // Helper methods for source creation
    obs_source_t* createTextSource(const QString& name, const QString& text,
                                  const QString& fontFamily, int fontSize, const QColor& color);
    obs_source_t* createColorSource(const QString& name, const QColor& color, int width, int height);
    obs_source_t* createImageSource(const QString& name, const QString& filePath);

    // Cleanup
    void cleanup();
};

#endif // OVERSEER_OVERLAY_H
