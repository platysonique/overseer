#pragma once

#include <obs.h>
#include <QString>
#include <QColor>
#include <QMap>
#include <QVariant>
#include <memory>

enum class OverlayType {
    LOGO,
    LOWER_THIRD,
    LIVE_BUG,
    BIG_LOWER_3RD,
    COUNTDOWN
};

enum class OverlayPlayState {
    Idle,
    Queued,
    Live
};

inline QString overlayTypeToString(OverlayType type) {
    switch (type) {
        case OverlayType::LOGO: return "logo";
        case OverlayType::LOWER_THIRD: return "lower_third";
        case OverlayType::LIVE_BUG: return "live_bug";
        case OverlayType::BIG_LOWER_3RD: return "big_lower_3rd";
        case OverlayType::COUNTDOWN: return "countdown";
    }
    return "unknown";
}

struct IngestedDataRow {
    QMap<QString, QString> fields;
};

class OverlayParameters {
public:
    virtual ~OverlayParameters() = default;
    QString overlayId;
    OverlayType type;
    int  positionX = 0;
    int  positionY = 0;
    int  width     = 640;
    int  height    = 480;
    float opacity  = 100.0f;
    int  rotation  = 0;
    QString blendMode = "Normal";
};

class LowerThirdParameters : public OverlayParameters {
public:
    LowerThirdParameters() {
        type  = OverlayType::LOWER_THIRD;
        width = 1280;
        height = 200;
    }

    // Text
    QString topText    = "Line 1";
    QString bottomText = "Line 2";

    QString fontFamily = "Arial";
    int     fontSize   = 24;
    QString fontWeight = "Regular";

    QColor topTextColor    = Qt::white;
    QColor bottomTextColor = Qt::white;

    // Backgrounds
    QColor topBackgroundColor    = QColor(0, 0, 0, 192);
    QColor bottomBackgroundColor = QColor(0, 0, 0, 192);

    // Logo placement
    QString logoImagePath;
    int     logoWidth      = 200;
    int     logoHeight     = 200;
    int     logoMarginLeft = 0;
    int     logoMarginTop  = 0;

    // Timing / animation
    int onAirDurationSec = 5;     // visible time
    int slideDurationMs  = 300;   // slide-in duration

    // Play state
    OverlayPlayState state = OverlayPlayState::Idle;
};

class LogoParameters : public OverlayParameters {
public:
    LogoParameters() { type = OverlayType::LOGO; width = 400; height = 400; }
    QString imagePath = "";
    float scale = 1.0f;
    bool lockAspectRatio = true;
};

class LiveBugParameters : public OverlayParameters {
public:
    LiveBugParameters() { type = OverlayType::LIVE_BUG; width = 300; height = 80; }
    QString text = "LIVE";
    QString positionPreset = "TopRight";
    QString sizePreset = "Medium";
    QString fontFamily = "Arial";
    int fontSize = 32;
    QString fontWeight = "Bold";
    QColor textColor = Qt::white;
    QColor backgroundColor = Qt::red;
    float bgOpacity = 100.0f;
};

class BigLower3rdParameters : public OverlayParameters {
public:
    BigLower3rdParameters() { type = OverlayType::BIG_LOWER_3RD; width = 1920; height = 300; }
    QString titleText = "Title";
    QString subtitleText = "Subtitle";
    QString titleFontFamily = "Arial";
    int titleFontSize = 48;
    QString titleFontWeight = "Bold";
    QColor titleTextColor = Qt::white;
    QString subtitleFontFamily = "Arial";
    int subtitleFontSize = 24;
    QColor subtitleTextColor = Qt::white;
    QString backgroundImagePath = "";
    QColor backgroundColor = Qt::black;
    float bgOpacity = 90.0f;
};

class CountdownParameters : public OverlayParameters {
public:
    CountdownParameters() { type = OverlayType::COUNTDOWN; width = 600; height = 150; }
    int durationHours = 0;
    int durationMinutes = 5;
    int durationSeconds = 0;
    QString format = "MM:SS";
    QString fontFamily = "Courier New";
    int fontSize = 64;
    QString fontWeight = "Bold";
    QColor textColor = Qt::white;
    QColor backgroundColor = Qt::black;
    float bgOpacity = 80.0f;
};

// Utility functions
OverlayType stringToOverlayType(const QString& str);
QString overlayTypeToString(OverlayType type);
QString getOverlayTypeDisplayName(OverlayType type);