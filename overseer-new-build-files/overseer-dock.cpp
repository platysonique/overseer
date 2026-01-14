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

    QVBoxLayout* groupLayout = new QVBoxLayout(overlayGroup);

    QWidget* controls = nullptr;

    switch (overlay->getType()) {

        case OverlayType::LOWER_THIRD: controls = createLowerThirdControls(overlay); break;

        case OverlayType::LOGO: controls = createLogoControls(overlay); break;

        case OverlayType::LIVE_BUG: controls = createLiveBugControls(overlay); break;

        case OverlayType::BIG_LOWER_3RD: controls = createBigLower3rdControls(overlay); break;

        case OverlayType::COUNTDOWN: controls = createCountdownControls(overlay); break;

    }

    if (controls) groupLayout->addWidget(controls);

    QFormLayout* commonLayout = new QFormLayout();

    QSpinBox* posX = new QSpinBox(); posX->setRange(-1920, 1920); posX->setValue(0);

    commonLayout->addRow("Position X:", posX);

    connect(posX, QOverload<int>::of(&QSpinBox::valueChanged), this, [this, overlayId](int x) {

        auto it = overlays_.find(overlayId);

        if (it != overlays_.end()) { float y; it->value()->getMainCanvasPosition(y, y); it->value()->setMainCanvasPosition((float)x, y); }

    });

    QSpinBox* posY = new QSpinBox(); posY->setRange(-1080, 1080); posY->setValue(0);

    commonLayout->addRow("Position Y:", posY);

    connect(posY, QOverload<int>::of(&QSpinBox::valueChanged), this, [this, overlayId](int y) {

        auto it = overlays_.find(overlayId);

        if (it != overlays_.end()) { float x; it->value()->getMainCanvasPosition(x, x); it->value()->setMainCanvasPosition(x, (float)y); }

    });

    QSlider* scale = new QSlider(Qt::Horizontal); scale->setRange(10, 500); scale->setValue(100);

    commonLayout->addRow("Scale:", scale);

    connect(scale, &QSlider::valueChanged, this, [this, overlayId](int val) {

        auto it = overlays_.find(overlayId);

        if (it != overlays_.end()) it->value()->setMainCanvasScale((float)val / 100.0f);

    });

    QCheckBox* visible = new QCheckBox("Visible"); visible->setChecked(true);

    commonLayout->addRow(visible);

    connect(visible, &QCheckBox::toggled, this, [this, overlayId](bool checked) {

        auto it = overlays_.find(overlayId);

        if (it != overlays_.end()) it->value()->setVisible(checked);

    });

    QPushButton* removeBtn = new QPushButton("Remove Overlay");

    commonLayout->addRow(removeBtn);

    connect(removeBtn, &QPushButton::clicked, this, [this, overlayId]() { onOverlayRemoved(overlayId); });

    groupLayout->addLayout(commonLayout);

    controlLayout_->insertWidget(controlLayout_->count() - 1, overlayGroup);

}



QWidget* OverseerDock::createLowerThirdControls(OverseerOverlay* overlay) {

    QWidget* w = new QWidget();

    QFormLayout* l = new QFormLayout(w);

    auto* p = dynamic_cast<LowerThirdParameters*>(overlay->getParameters());

    if (!p) return w;

    QLineEdit* top = new QLineEdit(); top->setText(p->topText);

    l->addRow("Top Text:", top);

    connect(top, &QLineEdit::textChanged, this, [this, overlay](const QString& t) {

        auto* p = dynamic_cast<LowerThirdParameters*>(overlay->getParameters());

        if (p) { p->topText = t; overlay->updateChildSourceProperty("TopText", "text", t); }

    });

    QLineEdit* bot = new QLineEdit(); bot->setText(p->bottomText);

    l->addRow("Bottom Text:", bot);

    connect(bot, &QLineEdit::textChanged, this, [this, overlay](const QString& t) {

        auto* p = dynamic_cast<LowerThirdParameters*>(overlay->getParameters());

        if (p) { p->bottomText = t; overlay->updateChildSourceProperty("BottomText", "text", t); }

    });

    QSpinBox* sz = new QSpinBox(); sz->setRange(8, 72); sz->setValue(p->fontSize);

    l->addRow("Font Size:", sz);

    QPushButton* logo = new QPushButton("Select Logo"); l->addRow("Logo:", logo);

    connect(logo, &QPushButton::clicked, this, [this, overlay]() {

        QString fp = QFileDialog::getOpenFileName(this, "Select Logo", "", "Images (*.png *.jpg)");

        if (!fp.isEmpty()) {

            auto* p = dynamic_cast<LowerThirdParameters*>(overlay->getParameters());

            if (p) { p->logoImagePath = fp; overlay->updateChildSourceProperty("Logo-Left", "file", fp); }

        }

    });

    return w;

}



QWidget* OverseerDock::createLogoControls(OverseerOverlay* overlay) {

    QWidget* w = new QWidget();

    QFormLayout* l = new QFormLayout(w);

    QPushButton* img = new QPushButton("Select Image"); l->addRow("Image:", img);

    connect(img, &QPushButton::clicked, this, [this, overlay]() {

        QString fp = QFileDialog::getOpenFileName(this, "Select Image", "", "Images (*.png *.jpg)");

        if (!fp.isEmpty()) {

            auto* p = dynamic_cast<LogoParameters*>(overlay->getParameters());

            if (p) { p->imagePath = fp; overlay->updateChildSourceProperty("LogoImage", "file", fp); }

        }

    });

    return w;

}



QWidget* OverseerDock::createLiveBugControls(OverseerOverlay* overlay) {

    QWidget* w = new QWidget();

    QFormLayout* l = new QFormLayout(w);

    auto* p = dynamic_cast<LiveBugParameters*>(overlay->getParameters());

    if (!p) return w;

    QLineEdit* txt = new QLineEdit(); txt->setText(p->text); l->addRow("Text:", txt);

    connect(txt, &QLineEdit::textChanged, this, [this, overlay](const QString& t) {

        auto* p = dynamic_cast<LiveBugParameters*>(overlay->getParameters());

        if (p) { p->text = t; overlay->updateChildSourceProperty("LiveText", "text", t); }

    });

    return w;

}



QWidget* OverseerDock::createBigLower3rdControls(OverseerOverlay* overlay) {

    QWidget* w = new QWidget();

    QFormLayout* l = new QFormLayout(w);

    auto* p = dynamic_cast<BigLower3rdParameters*>(overlay->getParameters());

    if (!p) return w;

    QLineEdit* title = new QLineEdit(); title->setText(p->titleText); l->addRow("Title:", title);

    connect(title, &QLineEdit::textChanged, this, [this, overlay](const QString& t) {

        auto* p = dynamic_cast<BigLower3rdParameters*>(overlay->getParameters());

        if (p) { p->titleText = t; overlay->updateChildSourceProperty("TitleText", "text", t); }

    });

    QLineEdit* sub = new QLineEdit(); sub->setText(p->subtitleText); l->addRow("Subtitle:", sub);

    connect(sub, &QLineEdit::textChanged, this, [this, overlay](const QString& t) {

        auto* p = dynamic_cast<BigLower3rdParameters*>(overlay->getParameters());

        if (p) { p->subtitleText = t; overlay->updateChildSourceProperty("SubtitleText", "text", t); }

    });

    return w;

}



QWidget* OverseerDock::createCountdownControls(OverseerOverlay* overlay) {

    QWidget* w = new QWidget();

    QFormLayout* l = new QFormLayout(w);

    auto* p = dynamic_cast<CountdownParameters*>(overlay->getParameters());

    if (!p) return w;

    QSpinBox* m = new QSpinBox(); m->setRange(0, 59); m->setValue(p->durationMinutes); l->addRow("Minutes:", m);

    QSpinBox* s = new QSpinBox(); s->setRange(0, 59); s->setValue(p->durationSeconds); l->addRow("Seconds:", s);

    return w;

}



void OverseerDock::onOverlayRemoved(const QString& overlayId) {

    overlays_.erase(overlays_.find(overlayId));

    saveSettings();

}



void OverseerDock::initializeWithOBS() {}

void OverseerDock::loadSettings() {}

void OverseerDock::saveSettings() {}
