#pragma once



#include <QDockWidget>

#include <QComboBox>

#include <QPushButton>

#include <QScrollArea>

#include <QVBoxLayout>

#include <QSlider>

#include <QSpinBox>

#include <QMap>

#include <memory>

#include <obs-frontend-api.h>

#include "overseer-types.h"

#include "overseer-overlay.h"

#include "overseer-data.h"



class OverseerDock : public QDockWidget {

    Q_OBJECT

public:

    explicit OverseerDock(QWidget* parent = nullptr);

    ~OverseerDock();

    void initializeWithOBS();

    void loadSettings();

    void saveSettings();



private slots:

    void onAddOverlay();

    void onIngestData();

    void onOverlayTypeChanged(int index);

    void onOverlayRemoved(const QString& overlayId);

    void onFieldTextChanged(const QString& overlayId, const QString& fieldName, const QString& text);



private:

    QComboBox* typeSelector_ = nullptr;

    QPushButton* addOverlayBtn_ = nullptr;

    QPushButton* ingestDataBtn_ = nullptr;

    QScrollArea* scrollArea_ = nullptr;

    QVBoxLayout* controlLayout_ = nullptr;

    std::unique_ptr<DataIngestor> dataIngestor_;

    QMap<QString, std::unique_ptr<OverseerOverlay>> overlays_;

    int nextOverlayNumber_ = 1;

    OverlayType currentSelectedType_ = OverlayType::LOWER_THIRD;

    void createUI();

    void setupConnections();

    void updateOverlayControlPanel(const QString& overlayId);

    QWidget* createLowerThirdControls(OverseerOverlay* overlay);

    QWidget* createLogoControls(OverseerOverlay* overlay);

    QWidget* createLiveBugControls(OverseerOverlay* overlay);

    QWidget* createBigLower3rdControls(OverseerOverlay* overlay);

    QWidget* createCountdownControls(OverseerOverlay* overlay);

};
