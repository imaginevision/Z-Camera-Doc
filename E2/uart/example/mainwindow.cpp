#include "mainwindow.h"

#include <QApplication>
#include <QSerialPortInfo>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QCheckBox>

#include "camera.h"
#include "uart_def.h"
#include "cameraconfig.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      camera_(new Camera())
{
    CameraConfig::init();
    createWidgets();
}

MainWindow::~MainWindow()
{
    delete camera_;
}

void MainWindow::createWidgets()
{
    QWidget * mainWidget = new QWidget(this);
    QHBoxLayout * mainHLayout = new QHBoxLayout(mainWidget);
    QVBoxLayout * vboxLayout = NULL;
    vboxLayout = new QVBoxLayout();

    // connection
    vboxLayout->addWidget(createConnectionGroup());
    // ptz
    vboxLayout->addWidget(createPTZGroup());
    // camera
    vboxLayout->addWidget(createCameraGroup());
    vboxLayout->addStretch();
    mainHLayout->addLayout(vboxLayout);

    // Movie
    mainHLayout->addWidget(createMovieGroup());

    vboxLayout = new QVBoxLayout();
    // exposure
    vboxLayout->addWidget(createExposureGroup());
    // wb
    vboxLayout->addWidget(createWhiteBalanceGroup());
    vboxLayout->addStretch();
    mainHLayout->addLayout(vboxLayout);

    vboxLayout = new QVBoxLayout();
    //
    vboxLayout->addWidget(createImageGroup());
    vboxLayout->addWidget(createMiscGroup());
    vboxLayout->addStretch();
    mainHLayout->addLayout(vboxLayout);

    enableCameraControl(false);
    this->setCentralWidget(mainWidget);
}

QWidget * MainWindow::createConnectionGroup()
{
    QGroupBox * groupBox = new QGroupBox(tr("Connection"));
    QVBoxLayout * vboxLayout = new QVBoxLayout();
    serial_box_ = new QComboBox();
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    for (int i = 0; i < ports.size(); i++) {
        serial_box_->addItem(ports.at(i).portName());
    }

    crc_enable_ = new QCheckBox("CRC Enable");

    connect(crc_enable_, &QCheckBox::stateChanged, this, [this]() {
        if(crc_enable_->checkState() == Qt::Checked){
            camera_->setCrcEnable(true);
        } else {
            camera_->setCrcEnable(false);
        }
        if (camera_->isConnected()){
            if(camera_->isCrcEnable()){
                camera_->enableCRC();
            } else {
                camera_->disableCRC();
            }
        }
    });

    connect_btn_ = new QPushButton(tr("Connect"));
    vboxLayout->addWidget(serial_box_);
    vboxLayout->addWidget(crc_enable_);
    vboxLayout->addWidget(connect_btn_);
    groupBox->setLayout(vboxLayout);

    connect(connect_btn_, &QPushButton::clicked, this, [this]() {
        if (camera_->isConnected()) {
            camera_->disconnect();

            connect_btn_->setText(tr("Connect"));
            serial_box_->setEnabled(true);

            enableCameraControl(false);
        } else{
            if (camera_->connect(serial_box_->currentText()) == 0) {
                if (camera_->getMode() < 0) {
                    camera_->disconnect();
                    serial_box_->setEnabled(true);

                    QMessageBox::warning(this, QApplication::applicationName(),
                                               tr("Please connect the camera to serail port!"));
                } else {
                    connect_btn_->setText(tr("Disconnect"));
                    serial_box_->setEnabled(false);

                    updateAllGroup();
                    enableCameraControl(true);
                }
            } else {
                serial_box_->setEnabled(true);
                QMessageBox::warning(this, QApplication::applicationName(),
                                           tr("The serial port is not available!"));
            }
        }
    });

    return groupBox;
}

QWidget * MainWindow::createPTZGroup()
{
    QGroupBox * groupBox = new QGroupBox(tr("PTZ"));
    QGridLayout * gridLayout = new QGridLayout();

    gridLayout->setAlignment(Qt::AlignTop);
    up_btn_ = new QPushButton(tr("Up"));
    connect(up_btn_, &QPushButton::pressed, this, [this] {

    });
    connect(up_btn_, &QPushButton::released, this, [this] {

    });
    gridLayout->addWidget(up_btn_, 0, 1);

    down_btn_ = new QPushButton(tr("Down"));
    connect(down_btn_, &QPushButton::pressed, this, [this] {

    });
    connect(up_btn_, &QPushButton::released, this, [this] {

    });
    gridLayout->addWidget(down_btn_, 2, 1);

    left_btn_ = new QPushButton(tr("Left"));
    connect(left_btn_, &QPushButton::pressed, this, [this] {

    });
    connect(left_btn_, &QPushButton::released, this, [this] {

    });
    gridLayout->addWidget(left_btn_, 1, 0);

    right_btn_ = new QPushButton(tr("Right"));
    connect(right_btn_, &QPushButton::pressed, this, [this] {

    });
    connect(right_btn_, &QPushButton::released, this, [this] {

    });
    gridLayout->addWidget(right_btn_, 1, 2);

    groupBox->setLayout(gridLayout);

    ptz_group_ = groupBox;
    return groupBox;
}

QWidget * MainWindow::createCameraGroup()
{
    QGroupBox * groupBox = new QGroupBox(tr("Camera"));
    QVBoxLayout * vboxLayout = new QVBoxLayout();
    rec_btn_ = new QPushButton(tr("Record"));
    vboxLayout->addWidget(rec_btn_);
    format_btn_ = new QPushButton(tr("Format"));
    vboxLayout->addWidget(format_btn_);
    clr_btn_ = new QPushButton(tr("Clear Settings"));
    vboxLayout->addWidget(clr_btn_);
    read_info_btn_ = new QPushButton(tr("Refresh"));
    vboxLayout->addWidget(read_info_btn_);
    groupBox->setLayout(vboxLayout);


    connect(rec_btn_, &QPushButton::clicked, this, [this]() {
        camera_->getStatus();
        if(camera_->isRecording()){
            if (camera_->stopRec() == 0) {
                rec_btn_->setText(tr("Start"));
            }
        } else {
            if (camera_->startRec() == 0) {
                rec_btn_->setText(tr("Stop"));
            }
        }
    });

    connect(format_btn_, &QPushButton::clicked, this, [this]() {
        camera_->formatSdCard();
    });

    connect(clr_btn_, &QPushButton::clicked, this, [this]() {
        camera_->clearSetting();
    });

    connect(read_info_btn_, &QPushButton::clicked, this, [this]() {
        this->updateAllGroup();
    });

    general_group_ = groupBox;
    return groupBox;
}

QWidget * MainWindow::createMovieGroup()
{
    QGroupBox * groupBox = new QGroupBox(tr("Movie"));
    QGridLayout * gridLayout = new QGridLayout();
    int row = 0;

    gridLayout->setAlignment(Qt::AlignTop);

//    gridLayout->addWidget(new QLabel(tr("Format")), row, 0, 1, 1);
//    format_box_ = new QComboBox();
//    gridLayout->addWidget(format_box_, row, 1, 1, 1);
//    row++;

    gridLayout->addWidget(new QLabel(tr("Resolution")), row, 0, 1, 1);
    resolution_box_ = new QComboBox();
    gridLayout->addWidget(resolution_box_, row, 1, 1, 1);
    row++;
    gridLayout->addWidget(new QLabel(tr("Project FPS")), row, 0, 1, 1);
    project_fps_box_ = new QComboBox();
    gridLayout->addWidget(project_fps_box_, row, 1, 1, 1);
    row++;

    gridLayout->addWidget(new QLabel(tr("VFR")), row, 0, 1, 1);
    vfr_box_ = new QComboBox();
    gridLayout->addWidget(vfr_box_, row, 1, 1, 1);
    row++;

    gridLayout->addWidget(new QLabel(tr("File Format")), row, 0, 1, 1);
    mov_fmt_box_ = new QComboBox();
    gridLayout->addWidget(mov_fmt_box_, row, 1, 1, 1);
    row++;

    gridLayout->addWidget(new QLabel(tr("Video Encoder")), row, 0, 1, 1);
    video_enc_box_ = new QComboBox();
    gridLayout->addWidget(video_enc_box_, row, 1, 1, 1);
    row++;

    gridLayout->addWidget(new QLabel(tr("Video Bitrate")), row, 0, 1, 1);
    video_br_box_ = new QComboBox();
    gridLayout->addWidget(video_br_box_, row, 1, 1, 1);
    row++;

    groupBox->setLayout(gridLayout);

    movie_group_ = groupBox;
    return groupBox;
}

QWidget * MainWindow::createExposureGroup()
{
    QGroupBox * groupBox = new QGroupBox(tr("Exposure"));

    QGridLayout * gridLayout = new QGridLayout();
    int row = 0;

    gridLayout->setAlignment(Qt::AlignTop);

    gridLayout->addWidget(new QLabel(tr("Shutter")), row, 0, 1, 1);
    shutter_box_ = new QComboBox();
    gridLayout->addWidget(shutter_box_, row, 1, 1, 1);
    row++;

    gridLayout->addWidget(new QLabel(tr("Aperture")), row, 0, 1, 1);
    aperture_box_ = new QComboBox();
    gridLayout->addWidget(aperture_box_, row, 1, 1, 1);
    row++;

    gridLayout->addWidget(new QLabel(tr("ISO")), row, 0, 1, 1);
    iso_box_ = new QComboBox();
    gridLayout->addWidget(iso_box_, row, 1, 1, 1);
    row++;

    gridLayout->addWidget(new QLabel(tr("EV")), row, 0, 1, 1);
    ev_box_ = new QComboBox();
    gridLayout->addWidget(ev_box_, row, 1, 1, 1);
    row++;

    gridLayout->addWidget(new QLabel(tr("Max ISO")), row, 0, 1, 1);
    max_iso_box_ = new QComboBox();
    gridLayout->addWidget(max_iso_box_, row, 1, 1, 1);
    row++;

    gridLayout->addWidget(new QLabel(tr("Dual Native ISO")), row, 0, 1, 1);
    dual_native_iso_box_ = new QComboBox();
    gridLayout->addWidget(dual_native_iso_box_, row, 1, 1, 1);
    row++;

    groupBox->setLayout(gridLayout);

    exposure_group_ = groupBox;
    return groupBox;
}

QWidget * MainWindow::createWhiteBalanceGroup()
{
    QGroupBox * groupBox = new QGroupBox(tr("White Balance"));

    QGridLayout * gridLayout = new QGridLayout();
    int row = 0;

    gridLayout->setAlignment(Qt::AlignTop);

    gridLayout->addWidget(new QLabel(tr("Mode")), row, 0, 1, 1);
    wb_mode_box_ = new QComboBox();
    gridLayout->addWidget(wb_mode_box_, row, 1, 1, 1);
    row++;

    gridLayout->addWidget(new QLabel(tr("Priority")), row, 0, 1, 1);
    wb_priority_box_ = new QComboBox();
    gridLayout->addWidget(wb_priority_box_, row, 1, 1, 1);
    row++;

    gridLayout->addWidget(new QLabel(tr("Kelvin")), row, 0, 1, 1);
    wb_kelvin_box_ = new QSpinBox();
    gridLayout->addWidget(wb_kelvin_box_, row, 1, 1, 1);
    row++;

    gridLayout->addWidget(new QLabel(tr("Tint")), row, 0, 1, 1);
    wb_tint_box_ = new QSpinBox();
    gridLayout->addWidget(wb_tint_box_, row, 1, 1, 1);
    row++;

    groupBox->setLayout(gridLayout);

    wb_group_ = groupBox;
    return groupBox;
}

QWidget * MainWindow::createImageGroup()
{
    int row = 0;
    QGroupBox * groupBox = new QGroupBox(tr("Image"));
    QGridLayout * gridLayout = new QGridLayout();

    gridLayout->setAlignment(Qt::AlignTop);

    gridLayout->addWidget(new QLabel(tr("Saturation")), row, 0, 1, 1);
    img_sat_box_ = new QSpinBox();
    gridLayout->addWidget(img_sat_box_, row, 1, 1, 1);
    row++;

    gridLayout->addWidget(new QLabel(tr("Brightness")), row, 0, 1, 1);
    img_brightness_box_ = new QSpinBox();
    gridLayout->addWidget(img_brightness_box_, row, 1, 1, 1);
    row++;

    gridLayout->addWidget(new QLabel(tr("Contrast")), row, 0, 1, 1);
    img_contrast_box_ = new QSpinBox();
    gridLayout->addWidget(img_contrast_box_, row, 1, 1, 1);
    row++;

    gridLayout->addWidget(new QLabel(tr("Sharpness")), row, 0, 1, 1);
    img_sharpness_box_ = new QComboBox();
    gridLayout->addWidget(img_sharpness_box_, row, 1, 1, 1);
    row++;

    gridLayout->addWidget(new QLabel(tr("Luminace")), row, 0, 1, 1);
    img_luminace_box_ = new QComboBox();
    gridLayout->addWidget(img_luminace_box_, row, 1, 1, 1);
    row++;

    gridLayout->addWidget(new QLabel(tr("Lut")), row, 0, 1, 1);
    img_lut_box_ = new QComboBox();
    gridLayout->addWidget(img_lut_box_, row, 1, 1, 1);
    row++;

    groupBox->setLayout(gridLayout);

    img_group_ = groupBox;
    return groupBox;
}

QWidget * MainWindow::createMiscGroup()
{
    QGroupBox * groupBox = new QGroupBox(tr("System"));

    QGridLayout * gridLayout = new QGridLayout();
    int row = 0;

    gridLayout->setAlignment(Qt::AlignTop);

    gridLayout->addWidget(new QLabel(tr("Video System")), row, 0, 1, 1);
    video_sys_box_ = new QComboBox();
    gridLayout->addWidget(video_sys_box_, row, 1, 1, 1);
    row++;

    gridLayout->addWidget(new QLabel(tr("Flicker Reduction")), row, 0, 1, 1);
    video_flicker_box_ = new QComboBox();
    gridLayout->addWidget(video_flicker_box_, row, 1, 1, 1);
    row++;

    gridLayout->addWidget(new QLabel(tr("Shutter Operation")), row, 0, 1, 1);
    shutter_op_box_ = new QComboBox();
    gridLayout->addWidget(shutter_op_box_, row, 1, 1, 1);
    row++;

    gridLayout->addWidget(new QLabel(tr("Auto Power Off")), row, 0, 1, 1);
    pwr_off_box_ = new QComboBox();
    gridLayout->addWidget(pwr_off_box_, row, 1, 1, 1);
    row++;

    gridLayout->addWidget(new QLabel(tr("WiFi")), row, 0, 1, 1);
    wifi_box_ = new QComboBox();
    gridLayout->addWidget(wifi_box_, row, 1, 1, 1);
    row++;

    gridLayout->addWidget(new QLabel(tr("HDMI Resolution")), row, 0, 1, 1);
    hdmi_format_box_ = new QComboBox();
    gridLayout->addWidget(hdmi_format_box_, row, 1, 1, 1);
    row++;

    gridLayout->addWidget(new QLabel(tr("HDMI OSD")), row, 0, 1, 1);
    hdmi_osd_box_ = new QComboBox();
    gridLayout->addWidget(hdmi_osd_box_, row, 1, 1, 1);
    row++;

    gridLayout->addWidget(new QLabel(tr("LED")), row, 0, 1, 1);
    led_box_ = new QComboBox();
    gridLayout->addWidget(led_box_, row, 1, 1, 1);
    row++;

    groupBox->setLayout(gridLayout);

    sys_group_ = groupBox;
    return groupBox;
}

void MainWindow::enableCameraControl(bool enable)
{
    general_group_->setEnabled(enable);
    ptz_group_->setEnabled(enable);
    movie_group_->setEnabled(enable);
    exposure_group_->setEnabled(enable);
    wb_group_->setEnabled(enable);
    img_group_->setEnabled(enable);
    sys_group_->setEnabled(enable);
}

void MainWindow::updateCameraRangeConfig(QSpinBox * widget, struct camera_config * config)
{
    if (widget == NULL) {
        return;
    }

    if (camera_->getConfig(config) != 0) {
        return;
    }

    if (config->type != CAMERA_CONFIG_TYPE_RANGE) {
        return;
    }

    widget->setEnabled(!(config->ro == 1));

    disconnect(widget,  SIGNAL(valueChanged(int)), 0, 0);

    // install the string to spin box
    widget->setMinimum(config->u.range.min);
    widget->setMaximum(config->u.range.max);
    widget->setSingleStep(config->u.range.step);
    widget->setValue(config->u.range.current_value);

    int key = config->key;
    connect(widget, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this, key] (int val) {
        struct camera_config cfg;

        cfg.key = key;
        cfg.type = CAMERA_CONFIG_TYPE_RANGE;
        cfg.u.range.current_value = val;

        camera_->setConfig(&cfg);
    });
}

void MainWindow::updateCameraChoiceConfig(QComboBox * widget, struct camera_config * config)
{
    if (widget == NULL) {
        return;
    }

    if (camera_->getConfig(config) != 0) {
        return;
    }

    if (config->type != CAMERA_CONFIG_TYPE_CHOICE) {
        return;
    }
    widget->setEnabled(!(config->ro == 1));
    disconnect(widget, SIGNAL(currentIndexChanged(int)), 0, 0);

    widget->clear();

    // install the string to combo box
    for (unsigned int i = 0; i < config->u.choice.nr_opt; i++) {
        const char * it = CameraConfig::trValueFromInt(config->key, config->u.choice.options[i]);
        widget->addItem(QObject::tr("%1").arg(it), QString(it));
    }
    widget->setCurrentText(QObject::tr("%1").arg(
                           CameraConfig::trValueFromInt(config->key, config->u.choice.current_value)));

    int key = config->key;
    connect(widget, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [this, widget, key](int index) {
        struct camera_config cfg;
        cfg.key = key;
        cfg.type = CAMERA_CONFIG_TYPE_CHOICE;

        QVariant data = widget->itemData(index);
        QString str = data.toString();
        cfg.u.choice.current_value = CameraConfig::trValueFromString(key, str.toLatin1());
        camera_->setConfig(&cfg);
    });
}

void MainWindow::updateAllGroup(void)
{
    if(camera_->isCrcEnable()) {
        camera_ -> enableCRC();
    } else {
        camera_ -> disableCRC();
    }
    camera_->getStatus();
    if(camera_->isRecording()){
        rec_btn_->setText(tr("Stop"));
    } else {
        rec_btn_->setText(tr("Start"));
    }
    updateMovieGroup();
    updateExposureGroup();
    updateWhiteBalanceGroup();
    updateImageGroup();
    updateSysGroup();
}

void MainWindow::updateMovieGroup(void)
{
    struct camera_config config;

//    config.key = CAMERA_CONFIG_MOVIE_FORMAT;
//    updateCameraChoiceConfig(format_box_, &config);

    config.key = CAMERA_CONFIG_MOVIE_RESOLUTION;
    updateCameraChoiceConfig(resolution_box_, &config);

    config.key = CAMERA_CONFIG_MOVIE_PROJECT_FPS;
    updateCameraChoiceConfig(project_fps_box_, &config);

    config.key = CAMERA_CONFIG_MOVIE_VFR;
    updateCameraChoiceConfig(vfr_box_, &config);

    config.key = CAMERA_CONFIG_RECORD_FILE_FORMAT;
    updateCameraChoiceConfig(mov_fmt_box_, &config);

    config.key = CAMERA_CONFIG_VIDEO_ENCODER;
    updateCameraChoiceConfig(video_enc_box_, &config);

    config.key = CAMERA_CONFIG_BITRATE_LEVEL;
    updateCameraChoiceConfig(video_br_box_, &config);
}

void MainWindow::updateExposureGroup(void)
{
    struct camera_config config;

    // TODO: get shutter info base on sht type: angle/speed
    config.key = CAMERA_CONFIG_VIDEO_SHUTTER_TIME;
    updateCameraChoiceConfig(shutter_box_, &config);

    config.key = CAMERA_CONFIG_ISO;
    updateCameraChoiceConfig(iso_box_, &config);

    config.key = CAMERA_CONFIG_IRIS;
    updateCameraChoiceConfig(aperture_box_, &config);

    config.key = CAMERA_CONFIG_EV;
    updateCameraChoiceConfig(ev_box_, &config);

    config.key = CAMERA_CONFIG_MAX_ISO_LIMIT;
    updateCameraChoiceConfig(max_iso_box_, &config);

    config.key = CAMERA_CONFIG_DUAL_NATIVE_ISO_MODE;
    updateCameraChoiceConfig(dual_native_iso_box_, &config);
}

void MainWindow::updateWhiteBalanceGroup(void)
{
    struct camera_config config;

    config.key = CAMERA_CONFIG_WB;
    updateCameraChoiceConfig(wb_mode_box_, &config);

    config.key = CAMERA_CONFIG_WB_PRIORITY;
    updateCameraChoiceConfig(wb_priority_box_, &config);

    config.key = CAMERA_CONFIG_MANUAL_WB;
    updateCameraRangeConfig(wb_kelvin_box_, &config);

    config.key = CAMERA_CONFIG_MANUAL_WB_TINT;
    updateCameraRangeConfig(wb_tint_box_, &config);
}

void MainWindow::updateImageGroup(void)
{
    struct camera_config config;

    config.key = CAMERA_CONFIG_SATURATION;
    updateCameraRangeConfig(img_sat_box_, &config);

    config.key = CAMERA_CONFIG_CONTRAST;
    updateCameraRangeConfig(img_contrast_box_, &config);

    config.key = CAMERA_CONFIG_BRIGHTNESS;
    updateCameraRangeConfig(img_brightness_box_, &config);

    config.key = CAMERA_CONFIG_SHARPNESS;
    updateCameraChoiceConfig(img_sharpness_box_, &config);

    config.key = CAMERA_CONFIG_LUMA_LEVEL;
    updateCameraChoiceConfig(img_luminace_box_, &config);

    config.key = CAMERA_CONFIG_LUT_TYPE;
    updateCameraChoiceConfig(img_lut_box_, &config);
}

void MainWindow::updateSysGroup(void)
{
    struct camera_config config;

    config.key = CAMERA_CONFIG_CVBS_VIDEO_SYSTEM;
    updateCameraChoiceConfig(video_sys_box_, &config);

    config.key = CAMERA_CONFIG_FLICKER_REDUCTION;
    updateCameraChoiceConfig(video_flicker_box_, &config);

    config.key = CAMERA_CONFIG_SHUTTER_UNIT;
    updateCameraChoiceConfig(shutter_op_box_, &config);

    config.key = CAMERA_CONFIG_AUTO_OFF;
    updateCameraChoiceConfig(pwr_off_box_, &config);

    config.key = CAMERA_CONFIG_WIFI_ONOFF;
    updateCameraChoiceConfig(wifi_box_, &config);

    config.key = CAMERA_CONFIG_LED_ENABLE;
    updateCameraChoiceConfig(led_box_, &config);

    config.key = CAMERA_CONFIG_HDMI_OSD_ONOFF;
    updateCameraChoiceConfig(hdmi_osd_box_, &config);

    config.key = CAMERA_CONFIG_HDMI_PREFER_FORMAT;
    updateCameraChoiceConfig(hdmi_format_box_, &config);
}

