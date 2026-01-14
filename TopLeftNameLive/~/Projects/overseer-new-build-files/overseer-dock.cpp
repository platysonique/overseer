#include "overseer-dock.h"

#include <QLabel>

#include <QLineEdit>

#include <QGroupBox>

#include <QFormLayout>

#include <QFileDialog>

#include <QUuid>

#include <QCheckBox>

#include <obs.h>



OverseerDock::OverseerDock(QWidget* parent) : QDockWidget("Overseer Control", parent) {

    dataIngestor_ = std::make_unique<DataIngestor>();

    createUI();

    setupConnections();

    loadSettings();

}



OverseerDock::~OverseerDock() { saveSettings(); }



void OverseerDock::createUI() {

    QWidget* mainWidget = new QWidget(this);

    QVBoxLayout* mainLayout = new QVBoxLayout(mainWidget);

    QHBoxLayout* headerLayout = new QHBoxLayout();

    headerLayout->addWidget(new QLabel("Overlay Type:"));

    typeSelector_ = new QComboBox();

    typeSelector_->addItem("Logo", static_cast<int>(OverlayType::LOGO));

    typeSelector_->addItem("Lower Third", static_cast<int>(OverlayType::LOWER_THIRD));

    typeSelector_->addItem("Live Bug", static_cast<int>(OverlayType::LIVE_BUG));

    typeSelector_->addItem("Big Lower 3rd", static_cast<int>(OverlayType::BIG_LOWER_3RD));

    typeSelector_->addItem("Countdown", static_cast<int>(OverlayType::COUNTDOWN));

    headerLayout->addWidget(typeSelector_);

    mainLayout->addLayout(headerLayout);

    QHBoxLayout* buttonLayout = new QHBoxLayout();

    addOverlayBtn_ = new QPushButton("Add Overlay");

    ingestDataBtn_ = new QPushButton("Ingest Data");

    buttonLayout->addWidget(addOverlayBtn_);

    buttonLayout->addWidget(ingestDataBtn_);

    mainLayout->addLayout(buttonLayout);

    scrollArea_ = new QScrollArea();

    QWidget* scrollWidget = new QWidget();

    controlLayout_ = new QVBoxLayout(scrollWidget);

    controlLayout_->addStretch();

    scrollArea_->setWidget(scrollWidget);

    scrollArea_->setWidgetResizable(true);

    mainLayout->addWidget(scrollArea_);

    setWidget(mainWidget);

}



void OverseerDock::setupConnections() {

    connect(typeSelector_, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &OverseerDock::onOverlayTypeChanged);

    connect(addOverlayBtn_, &QPushButton::clicked, this, &OverseerDock::onAddOverlay);

    connect(ingestDataBtn_, &QPushButton::clicked, this, &OverseerDock::onIngestData);

}



void OverseerDock::onOverlayTypeChanged(int index) {

    currentSelectedType_ = static_cast<OverlayType>(typeSelector_->currentData().toInt());

}



void OverseerDock::onAddOverlay() {

    OverlayType type = static_cast<OverlayType>(typeSelector_->currentData().toInt());

    QString overlayId = QString("Overlay_%1_%2").arg(nextOverlayNumber_++).arg(QUuid::createUuid().toString().mid(1, 8));

    auto overlay = std::make_unique<OverseerOverlay>(overlayId, type);

    overlay->createOBSSource();

    overlays_[overlayId] = std::move(overlay);

    updateOverlayControlPanel(overlayId);

    saveSettings();

}



void OverseerDock::onIngestData() {

    QString filePath = QFileDialog::getOpenFileName(this, "Load Data File", "", "CSV Files (*.csv);;Text Files (*.txt);;All Files (*)");

    if (!filePath.isEmpty()) dataIngestor_->loadFile(filePath);

}



void OverseerDock::updateOverlayControlPanel(const QString& overlayId) {

    auto it = overlays_.find(overlayId);

    if (it == overlays_.end()) return;

    OverseerOverlay* overlay = it->value().get();

    QGroupBox* overlayGroup = new QGroupBox(QString("Overlay: %1").arg(overlayId), this);

    Q
