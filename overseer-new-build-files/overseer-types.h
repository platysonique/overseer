#pragma once

#include <QString>
#include <QColor>
#include <QMap>

enum class OverlayType {
    LOGO,
    LOWER_THIRD,
    LIVE_BUG,
    BIG_LOWER_3RD,
    COUNTDOWN
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
    int positionX = 0;
    int positionY = 0;
    int width = 640;
    int height = 480;
    float opacity = 100.0f;
    int rotation = 0;
    QString blendMode = "Normal";
};

class LowerThirdParameters : public OverlayParameters {
public:
    LowerThirdParameters() { type = OverlayType::LOWER_THIRD; width = 1280; height = 200; }
    QString topText = "Name";
    QString bottomText = "Title";
    QString fontFamily = "Arial";
    int fontSize = 24;
    QString fontWeight = "Regular";
    QColor textColor = Qt::white;
    QColor backgroundColor = Qt::black;
    float bgOpacity = 80.0f;
    int outlineWidth = 2;
    QColor outlineColor = Qt::black;
    QString logoImagePath = "";
};
