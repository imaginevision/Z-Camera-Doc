#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <QMainWindow>

class QGroupBox;
class QComboBox;
class QPushButton;
class QCheckBox;
class QSpinBox;
class Camera;
struct camera_config;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void createWidgets();
    QWidget * createConnectionGroup();
    QWidget * createCameraGroup();
    QWidget * createPTZGroup();
    QWidget * createMovieGroup();
    QWidget * createExposureGroup();
    QWidget * createWhiteBalanceGroup();
    QWidget * createImageGroup();
    QWidget * createMiscGroup();

    void enableCameraControl(bool enable);

    // update
    void updateCameraRangeConfig(QSpinBox * widget, struct camera_config * config);
    void updateCameraChoiceConfig(QComboBox * widget, struct camera_config * config);

    void updateAllGroup(void);
    void updateMovieGroup(void);
    void updateExposureGroup(void);
    void updateWhiteBalanceGroup(void);
    void updateImageGroup(void);
    void updateSysGroup(void);

    Camera * camera_;

    QComboBox * serial_box_;
    QPushButton * connect_btn_;
    QCheckBox * crc_enable_;

    QGroupBox * general_group_;
    QPushButton * rec_btn_;
    QPushButton * format_btn_;
    QPushButton * clr_btn_;
    QPushButton * read_info_btn_;

    // movie
    QGroupBox * movie_group_;
    QComboBox * resolution_box_;
    QComboBox * project_fps_box_;
    QComboBox * format_box_;
    QComboBox * vfr_box_;
    QComboBox * mov_fmt_box_;
    QComboBox * video_enc_box_;
    QComboBox * video_br_box_;

    // ptz
    QGroupBox * ptz_group_;
    QPushButton * up_btn_;
    QPushButton * down_btn_;
    QPushButton * left_btn_;
    QPushButton * right_btn_;

    // exposure
    QGroupBox * exposure_group_;
    QComboBox * shutter_box_;
    QComboBox * aperture_box_;
    QComboBox * iso_box_;
    QComboBox * ev_box_;
    QComboBox * max_iso_box_;
    QComboBox * dual_native_iso_box_;

    // wb
    QGroupBox * wb_group_;
    QComboBox * wb_mode_box_;
    QComboBox * wb_priority_box_;
    QSpinBox * wb_kelvin_box_;
    QSpinBox * wb_tint_box_;

    // img
    QGroupBox * img_group_;
    QSpinBox * img_sat_box_;
    QSpinBox * img_contrast_box_;
    QSpinBox * img_brightness_box_;
    QComboBox * img_sharpness_box_;
    QComboBox * img_luminace_box_;
    QComboBox * img_lut_box_;

    // sys
    QGroupBox * sys_group_;
    QComboBox * video_sys_box_;
    QComboBox * video_flicker_box_;
    QComboBox * shutter_op_box_;
    QComboBox * wifi_box_;
    QComboBox * pwr_off_box_;
    QComboBox * led_box_;
    QComboBox * hdmi_osd_box_;
    QComboBox * hdmi_format_box_;
};

#endif // MAINWINDOW_H
