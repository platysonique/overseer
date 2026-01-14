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
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QColorDialog>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QCompleter>
#include <QMap>
#include <QList>
#include <QSettings>
#include <QTimer>

#include "overseer-types.h"
#include "overseer-overlay.h"
#include "overseer-data.h"

// Forward declarations
class OverlayParameters;

class OverseerDock : public QDockWidget {
    Q_OBJECT

public:
    OverseerDock(QWidget* parent = nullptr);
    ~OverseerDock();

    // Core functionality
    void addOverlay(OverlayType type);
    void removeOverlay(int overlayId);
    void updateOverlayParameters(int overlayId, OverlayParameters* params);

    // Data ingestion
    void loadDataFile();

    // Settings persistence
    void saveSettings();
    void loadSettings();

private slots:
    // UI event handlers
    void onAddOverlayClicked();
    void onIngestDataClicked();
    void onOverlayTypeChanged(int index);
    void onPositionXChanged(int value);
    void onPositionYChanged(int value);
    void onScaleChanged(double value);
    void onVisibilityChanged(bool visible);
    void onRemoveOverlayClicked();

    // Type-specific controls
    void onTextChanged();
    void onColorChanged();
    void onFontChanged();
    void onImageFileChanged();
    void onCountdownChanged();

    // Auto-fill functionality
    void onAutoFillTextChanged(const QString& text);

private:
    // Core UI components
    QVBoxLayout* mainLayout;
    QComboBox* overlayTypeSelect;
    QPushButton* addOverlayBtn;
    QPushButton* ingestDataBtn;
    QScrollArea* scrollArea;
    QWidget* scrollContent;
    QVBoxLayout* overlayLayout;

    // Data and overlays
    QList<OverseerOverlay*> overlays;
    DataIngestor* dataIngestor;
    QSettings* settings;

    // Current overlay being edited
    int currentOverlayId = -1;

    // UI creation methods
    void createUI();
    void createOverlayControls(OverseerOverlay* overlay);
    void updateOverlayControlPanel(int overlayId);

    // Control creation helpers
    QWidget* createPositionControls(OverlayParameters* params);
    QWidget* createScaleControls(OverlayParameters* params);
    QWidget* createVisibilityControls();
    QWidget* createTypeSpecificControls(OverlayType type, OverlayParameters* params);
    QWidget* createLowerThirdControls(LowerThirdParameters* params);
    QWidget* createLogoControls(LogoParameters* params);
    QWidget* createLiveBugControls(LiveBugParameters* params);
    QWidget* createBigLower3rdControls(BigLower3rdParameters* params);
    QWidget* createCountdownControls(CountdownParameters* params);

    QPushButton* createColorPickerButton(const QString& text, const QColor& initialColor);
    QPushButton* createFilePickerButton(const QString& text, const QString& initialFile);

    // Utility methods
    QString getUniqueOverlayName(OverlayType type);
    int generateOverlayId();
    OverseerOverlay* findOverlayById(int id);
    void updateAutoFillCompleters();
    void applyAutoFill(const IngestedDataRow& row);

    // Timer for countdown updates
    QTimer* countdownTimer;
};

#endif // OVERSEER_DOCK_H
