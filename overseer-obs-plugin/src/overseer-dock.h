#ifndef OVERSEER_DOCK_H
#include <obs.h>

#include <obs-source.h>

#define OVERSEER_DOCK_H

#include <QtWidgets/QDockWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QMap>
#include <QSettings>
#include <QList>

// Overlay types
enum OverlayType {
    LOGO,
    LOWER_THIRD,
    LIVE_BUG,
    BIG_LOWER_3RD,
    COUNTDOWN
};

// Overlay instance structure
struct OverlayInstance {
    int id;
    OverlayType type;
    obs_source_t* browserSource;
    QMap<QString, QVariant> params;
    bool visible;
};

class OverseerDock : public QDockWidget {
    Q_OBJECT

public:
    OverseerDock(QWidget* parent = nullptr);
    ~OverseerDock();

private slots:
    void onAddOverlayClicked();
    void onIngestDataClicked();

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

    void setupUI();
    void addOverlay(OverlayType type);
    void addOverlayControl(const OverlayInstance& overlay);
    void toggleVisibility(int id, bool visible);
    void loadSettings();
    void saveSettings();
    void ingestData();
};

#endif // OVERSEER_DOCK_H
