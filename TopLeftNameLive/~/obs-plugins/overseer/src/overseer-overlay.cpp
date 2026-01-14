#include "overseer-overlay.h"
#include <obs-source.h>
#include <QDebug>

OverseerOverlay::OverseerOverlay(const QString& overlayId, OverlayType type)
    : overlayId(overlayId), type(type)
{
    // Create appropriate parameter object based on type
    switch (type) {
        case OverlayType::LOGO:
            parameters = new LogoParameters(overlayId, "Logo Overlay");
            break;
        case OverlayType::LOWER_THIRD:
            parameters = new LowerThirdParameters(overlayId, "Lower Third");
            break;
        case OverlayType::LIVE_BUG:
            parameters = new LiveBugParameters(overlayId, "Live Bug");
            break;
        case OverlayType::BIG_LOWER_3RD:
            parameters = new BigLower3rdParameters(overlayId, "Big Lower 3rd");
            break;
        case OverlayType::COUNTDOWN:
            parameters = new CountdownParameters(overlayId, "Countdown");
            break;
    }
}

OverseerOverlay::~OverseerOverlay() {
    cleanup();
}

void OverseerOverlay::cleanup() {
    if (nestedScene) {
        delete nestedScene;
        nestedScene = nullptr;
    }
    if (parameters) {
        delete parameters;
        parameters = nullptr;
    }
}

bool OverseerOverlay::createOBSSource() {
    if (nestedScene) {
        qWarning() << "OBS source already created for overlay:" << overlayId;
        return false;
    }

    // Create nested scene manager
    QString sceneName = QString("OS-%1-%2").arg(overlayId).arg(QDateTime::currentDateTime().toMSecsSinceEpoch());
    nestedScene = new OverseerNestedScene(sceneName);

    if (!nestedScene->createNestedScene()) {
        qCritical() << "Failed to create nested scene for overlay:" << overlayId;
        delete nestedScene;
        nestedScene = nullptr;
        return false;
    }

    // Create type-specific overlay
    bool success = false;
    switch (type) {
        case OverlayType::LOGO:
            success = createLogoOverlay();
            break;
        case OverlayType::LOWER_THIRD:
            success = createLowerThirdOverlay();
            break;
        case OverlayType::LIVE_BUG:
            success = createLiveBugOverlay();
            break;
        case OverlayType::BIG_LOWER_3RD:
            success = createBigLower3rdOverlay();
            break;
        case OverlayType::COUNTDOWN:
            success = createCountdownOverlay();
            break;
    }

    if (!success) {
        qCritical() << "Failed to create overlay of type:" << static_cast<int>(type);
        cleanup();
        return false;
    }

    qDebug() << "Created OBS source for overlay:" << overlayId;
    return true;
}

obs_source_t* OverseerOverlay::getOBSSource() const {
    return nestedScene ? nestedScene->getSceneSource() : nullptr;
}

void OverseerOverlay::updateParameters(OverlayParameters* newParams) {
    if (!newParams || newParams->type != type) {
        return;
    }

    if (parameters) {
        delete parameters;
    }
    parameters = newParams;

    // Update nested scene properties
    if (nestedScene) {
        setMainCanvasPosition(parameters->mainCanvasX, parameters->mainCanvasY);
        setMainCanvasScale(parameters->scale, parameters->scale);
        setVisible(parameters->visible);
    }
}

void OverseerOverlay::updatePosition(int x, int y) {
    if (parameters) {
        parameters->mainCanvasX = x;
        parameters->mainCanvasY = y;
    }
    if (nestedScene) {
        nestedScene->setMainCanvasPosition((float)x, (float)y);
    }
}

void OverseerOverlay::updateScale(float scale) {
    if (parameters) {
        parameters->scale = scale;
    }
    if (nestedScene) {
        nestedScene->setMainCanvasScale(scale, scale);
    }
}

void OverseerOverlay::setVisible(bool visible) {
    if (parameters) {
        parameters->visible = visible;
    }
    if (nestedScene) {
        nestedScene->setVisible(visible);
    }
}

bool OverseerOverlay::createLowerThirdOverlay() {
    auto* params = dynamic_cast<LowerThirdParameters*>(parameters);
    if (!params) return false;

    // Set nested scene bounds
    nestedScene->setMainCanvasBounds(params->overlayWidth, params->overlayHeight);
    nestedScene->setMainCanvasPosition(params->mainCanvasX, params->mainCanvasY);
    nestedScene->setMainCanvasScale(params->scale, params->scale);
    nestedScene->setVisible(params->visible);

    // Create background color source (full width/height)
    obs_source_t* background = createColorSource("background", params->backgroundColor,
                                                params->overlayWidth, params->overlayHeight);
    if (background) {
        nestedScene->addSourceToNested(background, "background", 0, 0,
                                      params->overlayWidth, params->overlayHeight);
    }

    // Create logo on LEFT side (x:0-200)
    if (!params->logoImageFile.isEmpty()) {
        obs_source_t* logo = createImageSource("logo", params->logoImageFile);
        if (logo) {
            nestedScene->addSourceToNested(logo, "logo", 0, 0,
                                          params->logoWidth, params->logoHeight);
        }
    }

    // Create top text on RIGHT side (x:220-1280, y:0-100)
    obs_source_t* topText = createTextSource("topText", params->topText,
                                           params->fontFamily, params->fontSize, params->textColor);
    if (topText) {
        nestedScene->addSourceToNested(topText, "topText", 220, 0,
                                      params->overlayWidth - 220, 100);
    }

    // Create bottom text on RIGHT side (x:220-1280, y:100-200)
    obs_source_t* bottomText = createTextSource("bottomText", params->bottomText,
                                              params->fontFamily, params->fontSize, params->textColor);
    if (bottomText) {
        nestedScene->addSourceToNested(bottomText, "bottomText", 220, 100,
                                      params->overlayWidth - 220, 100);
    }

    return true;
}

bool OverseerOverlay::createLogoOverlay() {
    auto* params = dynamic_cast<LogoParameters*>(parameters);
    if (!params) return false;

    // Set nested scene bounds
    nestedScene->setMainCanvasBounds(params->width, params->height);
    nestedScene->setMainCanvasPosition(params->mainCanvasX, params->mainCanvasY);
    nestedScene->setMainCanvasScale(params->scale, params->scale);
    nestedScene->setVisible(params->visible);

    // Create logo image source (centered)
    if (!params->imageFile.isEmpty()) {
        obs_source_t* logo = createImageSource("logo", params->imageFile);
        if (logo) {
            nestedScene->addSourceToNested(logo, "logo", 0, 0,
                                          params->width, params->height);
        }
    }

    return true;
}

bool OverseerOverlay::createLiveBugOverlay() {
    auto* params = dynamic_cast<LiveBugParameters*>(parameters);
    if (!params) return false;

    // Set nested scene bounds
    nestedScene->setMainCanvasBounds(params->width, params->height);
    nestedScene->setMainCanvasPosition(params->mainCanvasX, params->mainCanvasY);
    nestedScene->setMainCanvasScale(params->scale, params->scale);
    nestedScene->setVisible(params->visible);

    // Create background color source
    obs_source_t* background = createColorSource("background", params->backgroundColor,
                                                params->width, params->height);
    if (background) {
        nestedScene->addSourceToNested(background, "background", 0, 0,
                                      params->width, params->height);
    }

    // Create text source
    obs_source_t* text = createTextSource("text", params->text,
                                        params->fontFamily, params->fontSize, params->textColor);
    if (text) {
        nestedScene->addSourceToNested(text, "text", 0, 0,
                                      params->width, params->height);
    }

    return true;
}

bool OverseerOverlay::createBigLower3rdOverlay() {
    auto* params = dynamic_cast<BigLower3rdParameters*>(parameters);
    if (!params) return false;

    // Set nested scene bounds
    nestedScene->setMainCanvasBounds(params->width, params->height);
    nestedScene->setMainCanvasPosition(params->mainCanvasX, params->mainCanvasY);
    nestedScene->setMainCanvasScale(params->scale, params->scale);
    nestedScene->setVisible(params->visible);

    // Create background color source
    obs_source_t* background = createColorSource("background", params->backgroundColor,
                                                params->width, params->height);
    if (background) {
        nestedScene->addSourceToNested(background, "background", 0, 0,
                                      params->width, params->height);
    }

    // Create title text (top half)
    obs_source_t* titleText = createTextSource("titleText", params->titleText,
                                             params->fontFamily, params->titleFontSize, params->textColor);
    if (titleText) {
        nestedScene->addSourceToNested(titleText, "titleText", 0, 0,
                                      params->width, params->height / 2);
    }

    // Create subtitle text (bottom half)
    obs_source_t* subtitleText = createTextSource("subtitleText", params->subtitleText,
                                                params->fontFamily, params->subtitleFontSize, params->textColor);
    if (subtitleText) {
        nestedScene->addSourceToNested(subtitleText, "subtitleText", 0, params->height / 2,
                                      params->width, params->height / 2);
    }

    return true;
}

bool OverseerOverlay::createCountdownOverlay() {
    auto* params = dynamic_cast<CountdownParameters*>(parameters);
    if (!params) return false;

    // Set nested scene bounds
    nestedScene->setMainCanvasBounds(params->width, params->height);
    nestedScene->setMainCanvasPosition(params->mainCanvasX, params->mainCanvasY);
    nestedScene->setMainCanvasScale(params->scale, params->scale);
    nestedScene->setVisible(params->visible);

    // Create background color source
    obs_source_t* background = createColorSource("background", params->backgroundColor,
                                                params->width, params->height);
    if (background) {
        nestedScene->addSourceToNested(background, "background", 0, 0,
                                      params->width, params->height);
    }

    // Create countdown text
    obs_source_t* countdownText = createTextSource("countdownText", params->getFormattedTime(),
                                                 params->fontFamily, params->fontSize, params->textColor);
    if (countdownText) {
        nestedScene->addSourceToNested(countdownText, "countdownText", 0, 0,
                                      params->width, params->height);
    }

    return true;
}

obs_source_t* OverseerOverlay::createTextSource(const QString& name, const QString& text,
                                              const QString& fontFamily, int fontSize, const QColor& color) {
    obs_data_t* settings = obs_data_create();

    // Set text content
    obs_data_set_string(settings, "text", text.toUtf8().constData());

    // Set font properties
    obs_data_set_string(settings, "font_face", fontFamily.toUtf8().constData());
    obs_data_set_int(settings, "font_size", fontSize);

    // Set color (OBS expects RGBA as int)
    uint32_t rgba = ((uint32_t)color.alpha() << 24) |
                   ((uint32_t)color.red() << 16) |
                   ((uint32_t)color.green() << 8) |
                   (uint32_t)color.blue();
    obs_data_set_int(settings, "color", (long long)rgba);

    // Create the text source
    obs_source_t* source = obs_source_create("text_gdiplus_v2", name.toUtf8().constData(), settings, nullptr);

    obs_data_release(settings);
    return source;
}

obs_source_t* OverseerOverlay::createColorSource(const QString& name, const QColor& color, int width, int height) {
    obs_data_t* settings = obs_data_create();

    // Set color (OBS expects RGBA as int)
    uint32_t rgba = ((uint32_t)color.alpha() << 24) |
                   ((uint32_t)color.red() << 16) |
                   ((uint32_t)color.green() << 8) |
                   (uint32_t)color.blue();
    obs_data_set_int(settings, "color", (long long)rgba);

    // Set dimensions
    obs_data_set_int(settings, "width", width);
    obs_data_set_int(settings, "height", height);

    // Create the color source
    obs_source_t* source = obs_source_create("color_source", name.toUtf8().constData(), settings, nullptr);

    obs_data_release(settings);
    return source;
}

obs_source_t* OverseerOverlay::createImageSource(const QString& name, const QString& filePath) {
    obs_data_t* settings = obs_data_create();

    // Set file path
    obs_data_set_string(settings, "file", filePath.toUtf8().constData());

    // Create the image source
    obs_source_t* source = obs_source_create("image_source", name.toUtf8().constData(), settings, nullptr);

    obs_data_release(settings);
    return source;
}

void OverseerOverlay::updateTextProperty(const QString& textType, const QString& text) {
    if (!nestedScene) return;

    if (textType == "topText" || textType == "bottomText") {
        nestedScene->updateSourceProperty(textType, "text", text);
    } else if (textType == "titleText" || textType == "subtitleText") {
        nestedScene->updateSourceProperty(textType, "text", text);
    } else if (textType == "text") {
        nestedScene->updateSourceProperty("text", "text", text);
    } else if (textType == "countdownText") {
        nestedScene->updateSourceProperty("countdownText", "text", text);
    }
}

void OverseerOverlay::updateColorProperty(const QString& colorType, const QColor& color) {
    if (!nestedScene) return;

    uint32_t rgba = ((uint32_t)color.alpha() << 24) |
                   ((uint32_t)color.red() << 16) |
                   ((uint32_t)color.green() << 8) |
                   (uint32_t)color.blue();

    if (colorType == "textColor") {
        // Update text color for all text sources
        nestedScene->updateSourceProperty("topText", "color", (qlonglong)rgba);
        nestedScene->updateSourceProperty("bottomText", "color", (qlonglong)rgba);
        nestedScene->updateSourceProperty("titleText", "color", (qlonglong)rgba);
        nestedScene->updateSourceProperty("subtitleText", "color", (qlonglong)rgba);
        nestedScene->updateSourceProperty("text", "color", (qlonglong)rgba);
        nestedScene->updateSourceProperty("countdownText", "color", (qlonglong)rgba);
    } else if (colorType == "backgroundColor") {
        nestedScene->updateSourceProperty("background", "color", (qlonglong)rgba);
    }
}

void OverseerOverlay::updateFontProperty(const QString& fontFamily, int fontSize) {
    if (!nestedScene) return;

    // Update font for all text sources
    nestedScene->updateSourceProperty("topText", "font_face", fontFamily);
    nestedScene->updateSourceProperty("topText", "font_size", fontSize);
    nestedScene->updateSourceProperty("bottomText", "font_face", fontFamily);
    nestedScene->updateSourceProperty("bottomText", "font_size", fontSize);
    nestedScene->updateSourceProperty("titleText", "font_face", fontFamily);
    nestedScene->updateSourceProperty("titleText", "font_size", fontSize);
    nestedScene->updateSourceProperty("subtitleText", "font_face", fontFamily);
    nestedScene->updateSourceProperty("subtitleText", "font_size", fontSize);
    nestedScene->updateSourceProperty("text", "font_face", fontFamily);
    nestedScene->updateSourceProperty("text", "font_size", fontSize);
    nestedScene->updateSourceProperty("countdownText", "font_face", fontFamily);
    nestedScene->updateSourceProperty("countdownText", "font_size", fontSize);
}

void OverseerOverlay::updateImageProperty(const QString& imageFile) {
    if (!nestedScene) return;

    // Update image file for logo sources
    nestedScene->updateSourceProperty("logo", "file", imageFile);
}
