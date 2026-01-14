#ifndef OVERSEER_DOCK_H
#define OVERSEER_DOCK_H

#include <QtWidgets/QDockWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QColorDialog>
#include <QMap>
#include <QSettings>
#include <QList>

// OBS headers
#include <obs.h>
#include <obs-source.h>

// Overlay types
enum OverlayType {
    LOGO,
    LOWER_THIRD,
    LIVE_BUG,
    BIG_LOWER_3RD,
    COUNTDOWN
};

// Position types
enum Position {
    UP_LEFT,
    UP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT,
    BOTTOM_CENTER
};

// Overlay instance structure
struct OverlayInstance {
    QString id;
    OverlayType type;
    Position position;
    bool visible;
    QVariantMap params;
    obs_source_t* source;
    OverlayInstance() : visible(false), source(nullptr) {}
    ~OverlayInstance() {
        if (source) {
            obs_source_release(source);
        }
    }
};

class OverseerDock : public QDockWidget {
    Q_OBJECT

public:
    OverseerDock(QWidget* parent = nullptr);
    ~OverseerDock();

private slots:
    void onAddOverlayClicked();
    void onIngestDataClicked();

public:
    void setupUI();
    void loadSettings();

private:
    QList<OverlayInstance> overlays_;
    QVBoxLayout* mainLayout_;
    QComboBox* overlayTypeSelect_;
    QPushButton* addOverlayBtn_;
    QPushButton* ingestDataBtn_;
    QScrollArea* scrollArea_;
    QWidget* scrollContent_;
    QVBoxLayout* controlLayout_;
    QSettings* settings_;

    void addOverlay(OverlayType type);
    void createOverlaySource(OverlayInstance& instance);
    QWidget* createOverlayControl(const OverlayInstance& instance);
    void createParameterControls(QVBoxLayout* layout, const OverlayInstance& instance);
    void toggleVisibility(const QString& id, bool visible);
    void removeOverlay(const QString& id);
    QString getOverlayTypeName(OverlayType type);
    int getOverlayIndex(const QString& id);
    void saveSettings();
    void ingestData();
};

#endif // OVERSEER_DOCK_H

