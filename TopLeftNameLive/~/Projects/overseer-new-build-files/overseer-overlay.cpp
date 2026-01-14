#include "overseer-overlay.h"



OverseerOverlay::OverseerOverlay(const QString& overlayId, OverlayType type)

    : overlayId_(overlayId), type_(type) {

    switch (type) {

        case OverlayType::LOGO: params_ = std::make_unique<LogoParameters>(); break;

        case OverlayType::LOWER_THIRD: params_ = std::make_unique<LowerThirdParameters>(); break;

        case OverlayType::LIVE_BUG: params_ = std::make_unique<LiveBugParameters>(); break;

        case OverlayType::BIG_LOWER_3RD: params_ = std::make_unique<BigLower3rdParameters>(); break;

        case OverlayType::COUNTDOWN: params_ = std::make_unique<CountdownParameters>(); break;

    }

}



OverseerOverlay::~OverseerOverlay() { removeOBSSource(); }



void OverseerOverlay::createOBSSource() {

    switch (type_) {

        case OverlayType::LOWER_THIRD: createLowerThirdNested(); break;

        case OverlayType::LOGO: createLogoNested(); break;

        case OverlayType::LIVE_BUG: createLiveBugNested(); break;

        case OverlayType::BIG_LOWER_3RD: createBigLower3rdNested(); break;

        case OverlayType::COUNTDOWN: createCountdownNested(); break;

    }

}



void OverseerOverlay::createLowerThirdNested() {

    auto nested = std::make_unique<OverseerNestedScene>(overlayId_, 1280, 200);

    if (!nested->createNestedScene()) return;

    auto* p = dynamic_cast<LowerThirdParameters*>(params_.get());

    if (!p) return;

    

    obs_data_t* bg_settings = obs_data_create();

    obs_data_set_int(bg_settings, "width", 1280);

    obs_data_set_int(bg_settings, "height", 200);

    obs_data_set_int(bg_settings, "color", 0x000000FF);

    obs_source_t* bg = obs_source_create("color_source", "Background", bg_settings, nullptr);

    obs_data_release(bg_settings);

    if (bg) { nested->addSourceToNested(bg, "Background", 0, 0, 1280, 200); obs_source_release(bg); }

    

    if (!p->logoImagePath.isEmpty()) {

        obs_data_t* logo_settings = obs_data_create();

        obs_data_set_string(logo_settings, "file", p->logoImagePath.toUtf8().constData());

        obs_source_t* logo = obs_source_create("image_source", "Logo-Left", logo_settings, nullptr);

        obs_data_release(logo_settings);

        if (logo) { nested->addSourceToNested(logo, "Logo-Left", 0, 0, 200, 200); obs_source_release(logo); }

    }

    

    obs_data_t* top_settings = obs_data_create();

    obs_data_set_string(top_settings, "text", p->topText.toUtf8().constData());

    obs_data_set_string(top_settings, "font", p->fontFamily.toUtf8().constData());

    obs_data_set_int(top_settings, "font_size", p->fontSize);

    obs_source_t* top = obs_source_create("text_gdiplus_v2", "TopText", top_settings, nullptr);

    obs_data_release(top_settings);

    if (top) { nested->addSourceToNested(top, "TopText", 220, 0, 1060, 100); obs_source_release(top); }

    

    obs_data_t* bot_settings = obs_data_create();

    obs_data_set_string(bot_settings, "text", p->bottomText.toUtf8().constData());

    obs_data_set_string(bot_settings, "font", p->fontFamily.toUtf8().constData());

    obs_data_set_int(bot_settings, "font_size", p->fontSize);

    obs_source_t* bot = obs_source_create("text_gdiplus_v2", "BottomText", bot_settings, nullptr);

    obs_data_release(bot_settings);

    if (bot) { nested->addSourceToNested(bot, "BottomText", 220, 100, 1060, 100); obs_source_release(bot); }

    

    nestedScene_ = std::move(nested);

}



void OverseerOverlay::createLogoNested() {

    auto nested = std::make_unique<OverseerNestedScene>(overlayId_, 400, 400);

    if (!nested->createNestedScene()) return;

    auto* p = dynamic_cast<LogoParameters*>(params_.get());

    if (!p) return;

    

    obs_data_t* bg_settings = obs_data_create();

    obs_data_set_int(bg_settings, "width", 400);

    obs_data_set_int(bg_settings, "height", 400);

    obs_data_set_int(bg_settings, "color", 0x00000000);

    obs_source_t* bg = obs_source_create("color_source", "Background", bg_settings, nullptr);

    obs_data_release(bg_settings);

    if (bg) { nested->addSourceToNested(bg, "Background", 0, 0, 400, 400); obs_source_release(bg); }

    

    if (!p->imagePath.isEmpty()) {

        obs_data_t* logo_settings = obs_data_create();

        obs_data_set_string(logo_settings, "file", p->imagePath.toUtf8().constData());

        obs_source_t* logo = obs_source_create("image_source", "LogoImage", logo_settings, nullptr);

        obs_data_release(logo_settings);

        if (logo) { nested->addSourceToNested(logo, "LogoImage", 0, 0, 400, 400); obs_source_release(logo); }

    }

    

    nestedScene_ = std::move(nested);

}



void OverseerOverlay::createLiveBugNested() {

    auto nested = std::make_unique<OverseerNestedScene>(overlayId_, 300, 80);

    if (!nested->createNestedScene()) return;

    auto* p = dynamic_cast<LiveBugParameters*>(params_.get());

    if (!p) return;

    

    obs_data_t* bg_settings = obs_data_create();

    obs_data_set_int(bg_settings, "width", 300);

    obs_data_set_int(bg_settings, "height", 80);

    obs_data_set_int(bg_settings, "color", 0xFF0000FF);

    obs_source_t* bg = obs_source_create("color_source", "Background", bg_settings, nullptr);

    obs_data_release(bg_settings);

    if (bg) { nested->addSourceToNested(bg, "Background", 0, 0, 300, 80); obs_source_release(bg); }

    

    obs_data_t* text_settings = obs_data_create();

    obs_data_set_string(text_settings, "text", p->text.toUtf8().constData());

    obs_data_set_string(text_settings, "font", p->fontFamily.toUtf8().constData());

    obs_data_set_int(text_settings, "font_size", p->fontSize);

    obs_source_t* text = obs_source_create("text_gdiplus_v2", "LiveText", text_settings, nullptr);

    obs_data_release(text_settings);

    if (text) { nested->addSourceToNested(text, "LiveText", 10, 10, 280, 60); obs_source_release(text); }

    

    nestedScene_ = std::move(nested);

}



void OverseerOverlay::createBigLower3rdNested() {

    auto nested = std::make_unique<OverseerNestedScene>(overlayId_, 1920, 300);

    if (!nested->createNestedScene()) return;

    auto* p = dynamic_cast<BigLower3rdParameters*>(params_.get());

    if (!p) return;

    

    obs_data_t* bg_settings = obs_data_create();

    obs_data_set_int(bg_settings, "width", 1920);

    obs_data_set_int(bg_settings, "height", 300);

    obs_data_set_int(bg_settings, "color", 0x000000FF);

    obs_source_t* bg = obs_source_create("color_source", "Background", bg_settings, nullptr);

    obs_data_release(bg_settings);

    if (bg) { nested->addSourceToNested(bg, "Background", 0, 0, 1920, 300); obs_source_release(bg); }

    

    obs_data_t* title_settings = obs_data_create();

    obs_data_set_string(title_settings, "text", p->titleText.toUtf8().constData());

    obs_data_set_string(title_settings, "font", p->titleFontFamily.toUtf8().constData());

    obs_data_set_int(title_settings, "font_size", p->titleFontSize);

    obs_source_t* title = obs_source_create("text_gdiplus_v2", "TitleText", title_settings, nullptr);

    obs_data_release(title_settings);

    if (title) { nested->addSourceToNested(title, "TitleText", 420, 0, 1500, 150); obs_source_release(title); }

    

    obs_data_t* sub_settings = obs_data_create();

    obs_data_set_string(sub_settings, "text", p->subtitleText.toUtf8().constData());

    obs_data_set_string(sub_settings, "font", p->subtitleFontFamily.toUtf8().constData());

    obs_data_set_int(sub_settings, "font_size", p->subtitleFontSize);

    obs_source_t* sub = obs_source_create("text_gdiplus_v2", "SubtitleText", sub_settings, nullptr);

    obs_data_release(sub_settings);

    if (sub) { nested->addSourceToNested(sub, "SubtitleText", 420, 150, 1500, 150); obs_source_release(sub); }

    

    nestedScene_ = std::move(nested);

}



void OverseerOverlay::createCountdownNested() {

    auto nested = std::make_unique<OverseerNestedScene>(overlayId_, 600, 150);

    if (!nested->createNestedScene()) return;

    auto* p = dynamic_cast<CountdownParameters*>(params_.get());

    if (!p) return;

    

    obs_data_t* bg_settings = obs_data_create();

    obs_data_set_int(bg_settings, "width", 600);

    obs_data_set_int(bg_settings, "height", 150);

    obs_data_set_int(bg_settings, "color", 0x000000FF);

    obs_source_t* bg = obs_source_create("color_source", "Background", bg_settings, nullptr);

    obs_data_release(bg_settings);

    if (bg) { nested->addSourceToNested(bg, "Background", 0, 0, 600, 150); obs_source_release(bg); }

    

    obs_data_t* timer_settings = obs_data_create();

    obs_data_set_string(timer_settings, "text", "MM:SS");

    obs_data_set_string(timer_settings, "font", p->fontFamily.toUtf8().constData());

    obs_data_set_int(timer_settings, "font_size", p->fontSize);

    obs_source_t* timer = obs_source_create("text_gdiplus_v2", "TimerText", timer_settings, nullptr);

    obs_data_release(timer_settings);

    if (timer) { nested->addSourceToNested(timer, "TimerText", 50, 25, 500, 100); obs_source_release(timer); }

    

    nestedScene_ = std::move(nested);

}



void OverseerOverlay::removeOBSSource() { nestedScene_.reset(); }



void OverseerOverlay::setVisible(bool visible) {

    if (nestedScene_) nestedScene_->setVisible(visible);

}



bool OverseerOverlay::isVisible() const {

    return nestedScene_ ? nestedScene_->isVisible() : false;

}



obs_sceneitem_t* OverseerOverlay::getSceneItem() const {

    return nestedScene_ ? nestedScene_->getSceneItem() : nullptr;

}



void OverseerOverlay::updateChildSourceProperty(const QString& sourceName, const QString& propertyName, const QVariant& value) {

    if (nestedScene_) nestedScene_->updateSourceProperty(sourceName, propertyName, value);

}



void OverseerOverlay::setMainCanvasPosition(float x, float y) {

    if (nestedScene_) nestedScene_->setMainCanvasPosition(x, y);

}



void OverseerOverlay::getMainCanvasPosition(float& x, float& y) const {

    if (nestedScene_) nestedScene_->getMainCanvasPosition(x, y);

    else { x = 0; y = 0; }

}



void OverseerOverlay::setMainCanvasScale(float scale) {

    if (nestedScene_) nestedScene_->setMainCanvasScale(scale, scale);

}



void OverseerOverlay::getMainCanvasScale(float& scale) const {

    if (nestedScene_) {

        float sx, sy;

        nestedScene_->getMainCanvasScale(sx, sy);

        scale = sx;

    } else scale = 1.0f;

}
