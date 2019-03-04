#include <cstddef>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "cameraconfig.h"
#include "uart_def.h"
#include "imvt_cam_def.h"

#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0])

#ifndef QT_TRANSLATE_NOOP
#define QT_TRANSLATE_NOOP(scope, x) (x)
#endif

/*
 * AE Anti Flicker Mode definition
 */
#define ANTI_FLICKER_AUTO    0
#define ANTI_FLICKER_60HZ    1
#define ANTI_FLICKER_50HZ    2

//////////////////////////////////////////////////////////////////////////////

const char kUnknown[] = "unknown";
const char kConfigKeySSID[] = "ssid";
const char kConfigKeyApPasswd[] = "ap_psw";

static Map* GetConfigKeyMap(void) {static Map map; return &map;}
static Map* GetMovResolutionStrMap(void) {static Map map; return &map;}
static Map* GetMovFpsStrMap(void) {static Map map; return &map;}
static Map* GetMovFmtStrMap(void) {static Map map; return &map;}
static Map* GetPhotoSizeStrMap(void) {static Map map; return &map;}
static Map* GetOnOffStrMap(void) {static Map map; return &map;}
static Map* GetWbStrMap(void) {static Map map; return &map;}
static Map* GetFlickerStrMap(void) {static Map map; return &map;}
static Map* GetAeMeteringStrMap(void) {static Map map; return &map;}
static Map* GetIsoStrMap(void) {static Map map; return &map;}
static Map* GetRotationStrMap(void) {static Map map; return &map;}
static Map* GetQualityStrMap(void) {static Map map; return &map;}
static Map* GetFocusMethodMap(void) {static Map map; return &map;}
static Map* GetFnumberStrMap(void) {static Map map; return &map;}
static Map* GetAfModeStrMap(void) {static Map map; return &map;}
static Map* GetBurstStrMap(void) {static Map map; return &map;}
static Map* GetMaxExposureTimeStrMap(void) {static Map map; return &map;}
static Map* GetShutterAngleStrMap(void) {static Map map; return &map;}
static Map* GetLensZoomStrMap(void) {static Map map; return &map;}
static Map* GetDcfNumberingStrMap(void) {static Map map; return &map;}
static Map* GetCvbsSystemStrMap(void) {static Map map; return &map;}
static Map* GetStillShutterSpeedStrMap(void) {static Map map; return &map;}
static Map* GetCafRangeStrMap(void) {static Map map; return &map;}
static Map* GetCafSensitivityStrMap(void) {static Map map; return &map;}
static Map* GetPhotoBurstSpeedStrMap(void) {static Map map; return &map;}
static Map* GetLutTypeStrMap(void) {static Map map; return &map;}
static Map* GetCameraRotationStrMap(void) {static Map map; return &map;}
static Map* GetUsbDeviceRoleStrMap(void) {static Map map; return &map;}
static Map* GetCameraMultipleModeStrMap(void) {static Map map; return &map;}
static Map* GetRecordModeStrMap(void) {static Map map; return &map;}
//static Map* GetHdmiOutputFormatStrMap(void) {static Map map; return &map;}
static Map* GetNoiseReductionStrMap(void) {static Map map; return &map;}
static Map* GetShootingModeStrMap(void) {static Map map; return &map;}
static Map* GetLensOisModeStrMap(void) {static Map map; return &map;}
static Map* GetCardFormatTypeStrMap(void) {static Map map; return &map;}
static Map* GetMovieRecordDurationStrMap(void) {static Map map; return &map;}
static Map* GetMovieAudioEncTypeStrMap(void) {static Map map; return &map;}
static Map* GetMovieEncoderStrMap(void) {static Map map; return &map;}
static Map* GetPhotoDriveModeStrMap(void) {static Map map; return &map;}
static Map* GetAutoOffStrMap(void) {static Map map; return &map;}
static Map* GetSendVideoStreamStrMap(void) {static Map map; return &map;}
static Map* GetN3aUnionAeAwbStrMap(void) {static Map map; return &map;}
static Map* GetN3aUnionDeltaStrMap(void) {static Map map; return &map;}
static Map* GetVideoShutterTimeStrMap(void) {static Map map; return &map;}
static Map* GetNetworkModeStrMap(void) {static Map map; return &map;}
static Map* GetSharpnessStrMap(void) {static Map map; return &map;}
static Map* GetBirateLevelMap(void) {static Map map; return &map;}
static Map* GetAeLockMap(void) {static Map map; return &map;}
static Map* GetRecordFileFormat(void) {static Map map; return &map;}
static Map* GetHdmiFmtStrMap(void) {static Map map; return &map;}
static Map* GetWbTintPriorityStrMap(void) {static Map map; return &map;}
static Map* GetDualNativeIsoModeMap(void) {static Map map; return &map;}
static Map* GetStyleMap(void) {static Map map; return &map;}
static Map* GetEvMap(void) {static Map map; return &map;}

////
//// Initialize map when really need it
static Map* GetShutterUnitMap()
{
    static Map map;
    if (map.empty()) {
        map.insert(Entry(IMVT_SHUTTER_UNIT_SPEED, QT_TRANSLATE_NOOP("MenuItem", "Speed")));
        map.insert(Entry(IMVT_SHUTTER_UNIT_ANGLE, QT_TRANSLATE_NOOP("MenuItem", "Angle")));
    }
    return &map;
}
static Map *GetLumaLevelMap()
{
    static Map map;
    if (map.empty()) {
        map.insert(Entry(IMVT_IQ_CSC_TV_MODE_DISABLE, QT_TRANSLATE_NOOP("MenuItem", "Full")));
        map.insert(Entry(IMVT_IQ_CSC_TV_MODE_ENABLE, QT_TRANSLATE_NOOP("MenuItem", "Limited")));
    }
    return &map;
}
static Map *GetVFRMap()
{
    static Map map;
    if (map.empty()) {
        map.insert(Entry(0, QT_TRANSLATE_NOOP("MenuItem", "Off")));
        map.insert(Entry(10, QT_TRANSLATE_NOOP("MenuItem", "10")));
        map.insert(Entry(15, QT_TRANSLATE_NOOP("MenuItem", "15")));
        map.insert(Entry(20, QT_TRANSLATE_NOOP("MenuItem", "20")));
        map.insert(Entry(21, QT_TRANSLATE_NOOP("MenuItem", "21")));
        map.insert(Entry(22, QT_TRANSLATE_NOOP("MenuItem", "22")));
        map.insert(Entry(23, QT_TRANSLATE_NOOP("MenuItem", "23")));
        map.insert(Entry(24, QT_TRANSLATE_NOOP("MenuItem", "24")));
        map.insert(Entry(25, QT_TRANSLATE_NOOP("MenuItem", "25")));
        map.insert(Entry(30, QT_TRANSLATE_NOOP("MenuItem", "30")));
        map.insert(Entry(45, QT_TRANSLATE_NOOP("MenuItem", "45")));
        map.insert(Entry(48, QT_TRANSLATE_NOOP("MenuItem", "48")));
        map.insert(Entry(50, QT_TRANSLATE_NOOP("MenuItem", "50")));
        map.insert(Entry(60, QT_TRANSLATE_NOOP("MenuItem", "60")));
        map.insert(Entry(72, QT_TRANSLATE_NOOP("MenuItem", "72")));
        map.insert(Entry(75, QT_TRANSLATE_NOOP("MenuItem", "75")));
        map.insert(Entry(84, QT_TRANSLATE_NOOP("MenuItem", "84")));
        map.insert(Entry(90, QT_TRANSLATE_NOOP("MenuItem", "90")));
        map.insert(Entry(96, QT_TRANSLATE_NOOP("MenuItem", "96")));
        map.insert(Entry(100, QT_TRANSLATE_NOOP("MenuItem", "100")));
        map.insert(Entry(105, QT_TRANSLATE_NOOP("MenuItem", "105")));
        map.insert(Entry(108, QT_TRANSLATE_NOOP("MenuItem", "108")));
        map.insert(Entry(120, QT_TRANSLATE_NOOP("MenuItem", "120")));
        map.insert(Entry(200, QT_TRANSLATE_NOOP("MenuItem", "200")));
        map.insert(Entry(240, QT_TRANSLATE_NOOP("MenuItem", "240")));
    }
    return &map;
}

// XXX: Caution !!!!!!!!!!!
//      Don't change the Entry string value what ever you what.
//      The HTTP client use the same string, if you change it the iOS/Android app may not work.
//      If you want to change the string displayed on LCD, you should modify the lang/*.ts
//      Don't modify here!!!
void CameraConfig::init(void)
{
    Map* sConfigKey_ = GetConfigKeyMap();
    sConfigKey_->insert(Entry(CAMERA_CONFIG_MOVIE_FORMAT, "movfmt"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_MOVIE_RESOLUTION, "resolution"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_MOVIE_PROJECT_FPS, "project_fps"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_PHOTO_SIZE, "photosize"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_WB, "wb"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_ISO, "iso"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_SHARPNESS, "sharpness"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_CONTRAST, "contrast"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_SATURATION, "saturation"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_BRIGHTNESS, "brightness"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_AE_METER_MODE, "meter_mode"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_SCENCE, "scence"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_DIGITAL_EFFECT, "de"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_FLICKER_REDUCTION, "flicker"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_CVBS_VIDEO_SYSTEM, "video_system"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_CVBS_OUTPUT_ENABLE, "video_output"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_WIFI_ONOFF, "wifi"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_EV_BIAS, "ev"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_BATTERY, "battery"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_LCD_ONOFF, "lcd"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_ROTATION, "rotation"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_MAGNIFY_POSITION, "mag_pos"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_FOCUS_METHOD, "focus"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_IRIS, "iris"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_AF_MODE, "af_mode"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_AF_AREA, "af_area"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_MF_DRIVE, "mf_drive"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_PHOTO_QUALITY, "photo_q"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_LED_ENABLE, "led"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_BEEPER_ENABLE, "beep"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_PHOTO_BURST, "burst"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_MAX_SHUTTER_SPEED, "max_exp"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_SHUTTER_ANGLE, "shutter_angle"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_MANUAL_WB, "mwb"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_LENS_ZOOM, "lens_zoom"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_LENS_FOCUS_POSITION, "lens_focus_pos"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_LENS_FOCUS_SPEED, "lens_focus_spd"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_STILL_SHUTTER_SPEED, "shutter_spd"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_CAF_RANGE, "caf_range"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_CAF_SENSITIVITY, "caf_sens"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_PHOTO_BURST_SPEED, "burst_spd"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_LUT_TYPE, "lut"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_DCF_LAST_FILE_NAME, "last_file_name"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_IMU_ROTATION, "camera_rot"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_CAMERA_IS_MULTIPLE, "multiple_mode"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_DEWARP_ONOFF, "dewarp"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_VIGNETTE_ONOFF, "vignette"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_NOISE_REDUCTION, "noise_reduction"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_MANUAL_WB_TINT, "tint"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_DCF_FILE_NUMBERING, "file_number"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_LCD_BACKLIGHT_LEVEL, "lcd_backlight"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_HDMI_PREFER_FORMAT, "hdmi_fmt"));/////
    sConfigKey_->insert(Entry(CAMERA_CONFIG_OLED_ONOFF_ENABLE, "oled"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_MULTIPLE_CONTROL_ID, "multiple_id"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_MULTIPLE_CAPTURE_DELAY, "multiple_delay"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_AE_EXPOSURE_MDOE, "shoot_mode"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_RECORD_INT_CAP, "record_mode"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_OIS_MODE, "ois_mode"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_MOVIE_RECORD_DURATION, "split_duration"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_MULTIPLE_TIMEOUT_ENABLE, "multiple_to"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_LIVEVIEW_WITH_AUDIO, "liveview_audio"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_MAX_ISO_LIMIT, "max_iso"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_BITRATE_LEVEL, "bitrate_level"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_RECORD_FILE_FORMAT, "record_file_format"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_FAN_MODE_SWITCH, "fan_mode"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_PRIMARY_STREAM_BITRATE, "primary_bitrate"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_SECONDARY_B_FRAME, "sec_gop_m"));  // not support
    sConfigKey_->insert(Entry(CAMERA_CONFIG_SECONDARY_STREAM_BITRATE_TYPE, "sec_rate_control")); // not support
    sConfigKey_->insert(Entry(CAMERA_CONFIG_SECONDARY_STREAM_RESOLUTION, "sec_resolution"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_SECONDARY_STREAM_BITRATE, "sec_bitrate"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_SECONDRY_STREAM_GOP, "sec_gop_n"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_SECONDARY_AUDIO_TYPE, "sec_audio"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_ENC_ROTATION, "sec_rotate"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_MAX_EXPOSURE_SHR_ANGLE, "max_exp_shutter_angle"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_MAX_EXPOSURE_SHR_TIME, "max_exp_shutter_time"));

    // //TODO
    sConfigKey_->insert(Entry(CAMERA_CONFIG_UNION_AE, "union_ae"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_UNION_AWB, "union_awb"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_VIDEO_SHUTTER_TIME, "shutter_time"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_PRODUCT_SN, "sn"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_GET_IS_SUPPORT_DUAL_STREAM, "support_sec"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_SEND_STREAM, "send_stream"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_UNION_AE_DELTA, "union_ae_delta"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_UNION_AWB_DELTA, "union_awb_delta"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_UNION_AE_PRIORITY, "union_ae_priority"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_WB_CALIB, "wb_calib"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_UNION_AWB_PRIORITY, "union_awb_priority"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_LIVE_AE_INFO_FNO,     "live_ae_fno"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_LIVE_AE_INFO_ISO,     "live_ae_iso"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_LIVE_AE_INFO_SHUTTER, "live_ae_shutter"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_LIVE_AE_INFO_SHUTTER_ANGLE, "live_ae_shutter_angle"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_AE_LOCK, "ae_lock"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_WB_PRIORITY, "wb_priority"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_HDMI_OSD_ONOFF, "hdmi_osd"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_USB_DEVICE_ROLE, "usb_device_role"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_VIDEO_ENCODER, "video_encoder"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_PRIMARY_AUDIO_TYPE, "primary_audio"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_SHUTTER_UNIT, "sht_operation"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_DUAL_NATIVE_ISO_MODE, "dual_iso"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_AE_FREEZE, "ae_freeze"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_AF_LOCK, "af_lock"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_GAMMA, "gamma"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_CLUT, "clut"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_3D_LUT_TYPE, "3d_lut"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_LENS_ZOOM_POSITION, "lens_zoom_pos"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_LUMA_LEVEL, "luma_level"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_EV, "ev_choice"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_MOVIE_VFR, "movvfr"));

    /// Local config key
    sConfigKey_->insert(Entry(CAMERA_CONFIG_AUTO_OFF, "auto_off"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_DRIVE_MODE, "drive_mode"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_PHOTO_TIMELAPSE_INTERVAL, "photo_tl_interval"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_PHOTO_TIMELAPSE_NUM, "photo_tl_num"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_PHOTO_SELF_TIMER_INTERVAL, "photo_self_interval"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_AUTO_OFF_LCD, "auto_off_lcd"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_GRID_DISPLAY_ONOFF, "grid_display"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_FOCUS_AREA_OPTION, "focus_area"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_LEVEL_CORRECTION_ONOFF, "level_correction"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_RECORD_TIMELAPSE_INTERVAL, "video_tl_interval"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_CAF_ONOFF_CONFIG, "caf"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_CROP_SETTING, "crop"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_BLC_CALIB_ADJUST, "blc_calib_adjust"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_ENABLE_VIDEO_TIMELAPSE, "enable_video_tl"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_MULTIPLE_CONTROL_VR_SNAP_ENABLE, "enable_vr_snap"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_MULTIPLE_CONTROL_VR_SNAP_INTERVAL, "vr_snap_interval"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_MULTIPLE_CONTROL_VR_SNAP_COUNT, "vr_snap_count"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_RECORD_DURATION, "rec_duration"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_VR_SNAP_PHOTO_TOKEN, "vr_snap_token"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_COMPOSE_MODE, "compose_mode"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_AUDIO_INPUT_GAIN, "audio_input_gain"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_AUDIO_OUTPUT_GAIN, "audio_output_gain"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_TIME_CODE_COUNT_UP, "tc_count_up"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_TIME_CODE_HDMI_DISPALY, "tc_hdmi_dispaly"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_ACODEC_INPUT_CHANNEL, "audio_channel"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_LVDS_CROP_SETTING, "lvds_crop"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_UART_ROLE, "uart_role"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_RECORD_PROXY_FILE_ONOFF, "rec_proxy_file"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_ASSITOOL_DISPLAY, "assitool_display"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_ASSITOOL_PEAK_ONOFF, "assitool_peak_onoff"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_ASSITOOL_PEAK_COLOR, "assitool_peak_color"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_ASSITOOL_EXPOSURE_TOOL, "assitool_exposure"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_ASSITOOL_ZEBRA_TH1, "assitool_zera_th1"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_ASSITOOL_ZEBRA_TH2, "assitool_zera_th2"));
    //

    //resolution
    Map* sMovResolutionStr_ = GetMovResolutionStrMap();
    sMovResolutionStr_->insert(Entry(IMVT_MOVIE_RES_4KP,  QT_TRANSLATE_NOOP("MenuItem","4K")));
    sMovResolutionStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P,  QT_TRANSLATE_NOOP("MenuItem","C4K")));
    sMovResolutionStr_->insert(Entry(IMVT_MOVIE_RES_3696_2772P,  QT_TRANSLATE_NOOP("MenuItem","3696x2772")));
    sMovResolutionStr_->insert(Entry(IMVT_MOVIE_RES_3312_2760P,  QT_TRANSLATE_NOOP("MenuItem","3312x2760")));
    sMovResolutionStr_->insert(Entry(IMVT_MOVIE_RES_1080P,  QT_TRANSLATE_NOOP("MenuItem","1920x1080")));
    sMovResolutionStr_->insert(Entry(IMVT_MOVIE_RES_720P,  QT_TRANSLATE_NOOP("MenuItem","1280x720")));
    sMovResolutionStr_->insert(Entry(IMVT_MOVIE_RES_2880_2880P,  QT_TRANSLATE_NOOP("MenuItem","2880x2880")));
    sMovResolutionStr_->insert(Entry(IMVT_MOVIE_RES_2704_1520P,  QT_TRANSLATE_NOOP("MenuItem","2.7K")));
    sMovResolutionStr_->insert(Entry(IMVT_MOVIE_RES_2688_2688P,  QT_TRANSLATE_NOOP("MenuItem","2688x2668")));
    sMovResolutionStr_->insert(Entry(IMVT_MOVIE_RES_2048_2048P,  QT_TRANSLATE_NOOP("MenuItem","2048x2048")));
    sMovResolutionStr_->insert(Entry(IMVT_MOVIE_RES_1920_1440P,  QT_TRANSLATE_NOOP("MenuItem","1920x1440")));
    sMovResolutionStr_->insert(Entry(IMVT_MOVIE_RES_1440_1080P,  QT_TRANSLATE_NOOP("MenuItem","1440x1080")));
    sMovResolutionStr_->insert(Entry(IMVT_MOVIE_RES_3680_2428P,  QT_TRANSLATE_NOOP("MenuItem","3680x2428")));
    sMovResolutionStr_->insert(Entry(IMVT_MOVIE_RES_4096_2048P,  QT_TRANSLATE_NOOP("MenuItem","4096x2048")));
    sMovResolutionStr_->insert(Entry(IMVT_MOVIE_RES_3840_1920P,  QT_TRANSLATE_NOOP("MenuItem","3840x1920")));
    sMovResolutionStr_->insert(Entry(IMVT_MOVIE_RES_2120_1248P,  QT_TRANSLATE_NOOP("MenuItem","2120x1248")));
    sMovResolutionStr_->insert(Entry(IMVT_MOVIE_RES_3296_2472P,  QT_TRANSLATE_NOOP("MenuItem","3296x2472")));
    sMovResolutionStr_->insert(Entry(IMVT_MOVIE_RES_2700_1800P,  QT_TRANSLATE_NOOP("MenuItem","2700x1800")));
    sMovResolutionStr_->insert(Entry(IMVT_MOVIE_RES_4056_2304P,  QT_TRANSLATE_NOOP("MenuItem","4056x2304")));
    sMovResolutionStr_->insert(Entry(IMVT_MOVIE_RES_3376_2768P,  QT_TRANSLATE_NOOP("MenuItem","3376x2768")));
    sMovResolutionStr_->insert(Entry(IMVT_MOVIE_RES_2880_1524P,  QT_TRANSLATE_NOOP("MenuItem","2880x1524")));
    sMovResolutionStr_->insert(Entry(IMVT_MOVIE_RES_4KP_LN,  QT_TRANSLATE_NOOP("MenuItem","4K(Low Noise)")));
    sMovResolutionStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P_LN,  QT_TRANSLATE_NOOP("MenuItem","C4K(Low Noise)")));

    //fps
    Map* sMovFpsStr_ = GetMovFpsStrMap();
    sMovFpsStr_->insert(Entry(IMVT_MOVIE_FPS_2397,  QT_TRANSLATE_NOOP("MenuItem","23.98")));
    sMovFpsStr_->insert(Entry(IMVT_MOVIE_FPS_24,  QT_TRANSLATE_NOOP("MenuItem","24")));
    sMovFpsStr_->insert(Entry(IMVT_MOVIE_FPS_25,  QT_TRANSLATE_NOOP("MenuItem","25")));
    //sMovFpsStr_->insert(Entry(IMVT_MOVIE_FPS_2997,  QT_TRANSLATE_NOOP("MenuItem","29.97")));
    sMovFpsStr_->insert(Entry(IMVT_MOVIE_FPS_30,  QT_TRANSLATE_NOOP("MenuItem","29.97")));
    sMovFpsStr_->insert(Entry(IMVT_MOVIE_FPS_48,  QT_TRANSLATE_NOOP("MenuItem","48")));
    sMovFpsStr_->insert(Entry(IMVT_MOVIE_FPS_50,  QT_TRANSLATE_NOOP("MenuItem","50")));
    //sMovFpsStr_->insert(Entry(IMVT_MOVIE_FPS_5994,  QT_TRANSLATE_NOOP("MenuItem","59.94")));
    sMovFpsStr_->insert(Entry(IMVT_MOVIE_FPS_60,  QT_TRANSLATE_NOOP("MenuItem","59.94")));
    sMovFpsStr_->insert(Entry(IMVT_MOVIE_FPS_100,  QT_TRANSLATE_NOOP("MenuItem","100")));
    sMovFpsStr_->insert(Entry(IMVT_MOVIE_FPS_120,  QT_TRANSLATE_NOOP("MenuItem","120")));
    sMovFpsStr_->insert(Entry(IMVT_MOVIE_FPS_200,  QT_TRANSLATE_NOOP("MenuItem","200")));
    sMovFpsStr_->insert(Entry(IMVT_MOVIE_FPS_240,  QT_TRANSLATE_NOOP("MenuItem","240")));
    //  NTSC
    Map* sMovFmtStr_ = GetMovFmtStrMap();
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4KP30, "4KP30"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4KP2997, "4KP29.97"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4KP24, "4KP24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_1080P240, "1080P240"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_1080P200, "1080P200"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_1080P60, "1080P60"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_1080P5994, "1080P59.94"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_1080P30, "1080P30"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_1080P2997, "1080P29.97"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_1080P24, "1080P24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_1440_1080P30, "1080P30 (4:3)"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_1440_1080P2997, "1080P29.97 (4:3)"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_1440_1080P24, "1080P24 (4:3)"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_720P60, "720P60"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_720P5994, "720P59.94"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_720P30, "720P30"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_720P2997, "720P29.97"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_720P24, "720P24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2704_1520P30, "2.7KP30"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2704_1520P2997, "2.7KP29.97"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2704_1520P24, "2.7KP24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_1920_1440P30, "1440P30"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_1920_1440P2997, "1440P29.97"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_1920_1440P24, "1440P24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2048_2048P30, "2048P30 (1:1)"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2048_2048P2997, "2048P29.97 (1:1)"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2048_2048P24, "2048P24 (1:1)"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2688_2688P30, "2688P30 (1:1)"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2688_2688P2997, "2688P29.97 (1:1)"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2688_2688P24, "2688P24 (1:1)"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2880_2880P30, "2880P30 (1:1)"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2880_2880P2997, "2880P29.97 (1:1)"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2880_2880P24, "2880P24 (1:1)"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P30, "C4KP30"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P2997, "C4KP29.97"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P24, "C4KP24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3680_2428P30, "3680x2428P30"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3680_2428P2997, "3680x2428P29.97"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3680_2428P24, "3680x2428P24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2048P30, "4096x2048P30"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2048P2997, "4096x2048P2997"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2048P24, "4096x2048P24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2120_1248P60, "2120x1248P60"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3296_2472P30, "3296x2472P30"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3296_2472P25, "3296x2472P25"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3296_2472P24, "3296x2472P24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4056_2304P30, "4056x2304P30"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4056_2304P24, "4056x2304P24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4KP30_WDR, "4KP30 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4KP2997_WDR, "4KP29.97 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4KP24_WDR, "4KP24 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3840_1920P30, "3840x1920P30"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3840_1920P24, "3840x1920P24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2700_1800P60, "2700x1800P60"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4KP60, "4KP60"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4KP120, "4KP120"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4KP100, "4KP100"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3376_2768P30, "3376x2768P30"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3376_2768P24, "3376x2768P24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2880_1524P60, "2880x1524P60"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4KP22, "4KP22"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4KP20, "4KP20"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4KP2397, "4KP23.98"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4KP60_WDR, "4KP60 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4KP2397_WDR, "4KP23.98 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4KP22_WDR, "4KP22 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4KP20_WDR, "4KP20 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P120, "C4KP120"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P60, "C4KP60"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P2397, "C4KP23.98"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P22, "C4KP22"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P20, "C4KP20"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P60_WDR, "C4KP60 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P30_WDR, "C4KP30 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P2997_WDR, "C4KP29.97 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P24_WDR, "C4KP24 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P2397_WDR, "C4KP23.98 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P22_WDR, "C4KP22 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P20_WDR, "C4KP20 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3696_2772P60, "3696x2772P60"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3696_2772P30, "3696x2772P30"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3696_2772P24, "3696x2772P24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3696_2772P2397, "3696x2772P23.98"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3696_2772P22, "3696x2772P22"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3696_2772P20, "3696x2772P20"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4KP48, "4KP48"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P48, "C4KP48"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3696_2772P48, "3696x2772P48"));
    // PAL
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4KP25, "4KP25"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_1080P50, "1080P50"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_1080P25, "1080P25"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_1440_1080P25, "1080P25 (4:3)"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_720P50, "720P50"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_720P25, "720P25"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2704_1520P25, "2.7KP25"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_1920_1440P25, "1440P25"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2048_2048P25, "2048P25 (1:1)"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2688_2688P25, "2688P25 (1:1)"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2880_2880P25, "2880P25 (1:1)"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P25, "C4KP25"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3680_2428P25, "3680x2428P25"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2048P25, "4096x2048P25"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2120_1248P50, "2120x1248P50"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4056_2304P25, "4056x2304P25"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4KP25_WDR, "4KP25 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3840_1920P25, "3840x1920P25"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2700_1800P50, "2700x1800P50"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4KP50, "4KP50"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4KP100, "4KP100"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3376_2768P25, "3376x2768P25"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2880_1524P50, "2880x1524P50"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4KP50_WDR, "4KP50 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P100, "C4KP100"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P50, "C4KP50"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P50_WDR, "C4KP50 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P25_WDR, "C4KP25 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3696_2772P50, "3696x2772P50"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3696_2772P25, "3696x2772P25"));

    Map* sPhotoSizeStr_ = GetPhotoSizeStrMap();
    sPhotoSizeStr_->insert(Entry(IMVT_PHOTO_RES_16M, "16M"));
    sPhotoSizeStr_->insert(Entry(IMVT_PHOTO_RES_12M, "12M"));
    sPhotoSizeStr_->insert(Entry(IMVT_PHOTO_RES_8M, "8M"));
    sPhotoSizeStr_->insert(Entry(IMVT_PHOTO_RES_6M, "6M"));
    sPhotoSizeStr_->insert(Entry(IMVT_PHOTO_RES_4K, "4K"));

    // XXX: Caution !!!!!!!!!!! read above comment first
    Map* sOnOffStr_ = GetOnOffStrMap();
    sOnOffStr_->insert(Entry(IMVT_OFF, QT_TRANSLATE_NOOP("MenuItem", "Off")));
    sOnOffStr_->insert(Entry(IMVT_ON, QT_TRANSLATE_NOOP("MenuItem", "On")));

    // XXX: Caution !!!!!!!!!!! read above comment first
    Map* sWbStr_ = GetWbStrMap();
    sWbStr_->insert(Entry(IMVT_WB_AUTO,            QT_TRANSLATE_NOOP("MenuItem", "Auto")));
    sWbStr_->insert(Entry(IMVT_WB_INCANDESCENT,    QT_TRANSLATE_NOOP("MenuItem", "Incandescent")));
    sWbStr_->insert(Entry(IMVT_WB_TUNGSTEN,        QT_TRANSLATE_NOOP("MenuItem", "Tungsten")));
    sWbStr_->insert(Entry(IMVT_WB_D4000,           QT_TRANSLATE_NOOP("MenuItem", "D4000")));
    sWbStr_->insert(Entry(IMVT_WB_D5000,           QT_TRANSLATE_NOOP("MenuItem", "D5000")));
    sWbStr_->insert(Entry(IMVT_WB_SUNNY,           QT_TRANSLATE_NOOP("MenuItem", "Sunny")));
    sWbStr_->insert(Entry(IMVT_WB_CLOUDY,          QT_TRANSLATE_NOOP("MenuItem", "Cloudy")));
    sWbStr_->insert(Entry(IMVT_WB_D9000,           QT_TRANSLATE_NOOP("MenuItem", "D9000")));
    sWbStr_->insert(Entry(IMVT_WB_D10000,          QT_TRANSLATE_NOOP("MenuItem", "D10000")));
    sWbStr_->insert(Entry(IMVT_WB_FLASH,           QT_TRANSLATE_NOOP("MenuItem", "Flash")));
    sWbStr_->insert(Entry(IMVT_WB_FLUORESCENT,     QT_TRANSLATE_NOOP("MenuItem", "Fluorescent")));
    sWbStr_->insert(Entry(IMVT_WB_FLUORESCENT_2,   QT_TRANSLATE_NOOP("MenuItem", "Fluorescent2")));
    sWbStr_->insert(Entry(IMVT_WB_FLUORESCENT_3,   QT_TRANSLATE_NOOP("MenuItem", "Fluorescent3")));
    sWbStr_->insert(Entry(IMVT_WB_FLUORESCENT_4,   QT_TRANSLATE_NOOP("MenuItem", "Fluorescent4")));
    sWbStr_->insert(Entry(IMVT_WB_UNDER_WATER,     QT_TRANSLATE_NOOP("MenuItem", "UnderWater")));
    sWbStr_->insert(Entry(IMVT_WB_OUTDOOR,         QT_TRANSLATE_NOOP("MenuItem", "Outdoor")));
    sWbStr_->insert(Entry(IMVT_WB_LASTMODE,        QT_TRANSLATE_NOOP("MenuItem", "Lastmode")));
    sWbStr_->insert(Entry(IMVT_WB_DAYLIGHT,        QT_TRANSLATE_NOOP("MenuItem", "Daylight")));
    sWbStr_->insert(Entry(IMVT_WB_DIRECT_SUNLIGHT, QT_TRANSLATE_NOOP("MenuItem", "DirectSunLight")));
    sWbStr_->insert(Entry(IMVT_WB_SHADE,           QT_TRANSLATE_NOOP("MenuItem", "Shade")));
    sWbStr_->insert(Entry(IMVT_WB_BLUE_SKY,        QT_TRANSLATE_NOOP("MenuItem", "BlueSky")));
    sWbStr_->insert(Entry(IMVT_WB_SUNRISE_SUNSET,  QT_TRANSLATE_NOOP("MenuItem", "SunriseSunset")));
    sWbStr_->insert(Entry(IMVT_WB_MANUAL,          QT_TRANSLATE_NOOP("MenuItem", "Manual")));

    // XXX: Caution !!!!!!!!!!! read above comment first
    Map* sFlickerStr_ = GetFlickerStrMap();
    sFlickerStr_->insert(Entry(IMVT_FLICKER_REDUCTION_AUTO, QT_TRANSLATE_NOOP("MenuItem", "Auto")));
    sFlickerStr_->insert(Entry(IMVT_FLICKER_REDUCTION_60HZ, "60Hz"));
    sFlickerStr_->insert(Entry(IMVT_FLICKER_REDUCTION_50HZ, "50Hz"));

    // XXX: Caution !!!!!!!!!!! read above comment first
    Map* sAeMeteringStr_ = GetAeMeteringStrMap();
    sAeMeteringStr_->insert(Entry(IMVT_AE_METERING_CENTER, QT_TRANSLATE_NOOP("MenuItem", "Center")));
    sAeMeteringStr_->insert(Entry(IMVT_AE_METERING_AVERAGE, QT_TRANSLATE_NOOP("MenuItem", "Average")));
    sAeMeteringStr_->insert(Entry(IMVT_AE_METERING_SPOT, QT_TRANSLATE_NOOP("MenuItem", "Spot")));
    sAeMeteringStr_->insert(Entry(IMVT_AE_METERING_MANUAL, QT_TRANSLATE_NOOP("MenuItem", "Manual")));

    // XXX: Caution !!!!!!!!!!! read above comment first
    Map* sIsoStr_ = GetIsoStrMap();
    sIsoStr_->insert(Entry(IMVT_ISO_AUTO, QT_TRANSLATE_NOOP("MenuItem", "Auto")));
    sIsoStr_->insert(Entry(IMVT_ISO_3,   "3"));
    sIsoStr_->insert(Entry(IMVT_ISO_6,   "6"));
    sIsoStr_->insert(Entry(IMVT_ISO_12,  "12"));
    sIsoStr_->insert(Entry(IMVT_ISO_25,  "25"));
    sIsoStr_->insert(Entry(IMVT_ISO_50,  "50"));
    sIsoStr_->insert(Entry(IMVT_ISO_64,  "64"));
    sIsoStr_->insert(Entry(IMVT_ISO_80,  "80"));
    sIsoStr_->insert(Entry(IMVT_ISO_100, "100"));
    sIsoStr_->insert(Entry(IMVT_ISO_125, "125"));
    sIsoStr_->insert(Entry(IMVT_ISO_160, "160"));
    sIsoStr_->insert(Entry(IMVT_ISO_200, "200"));
    sIsoStr_->insert(Entry(IMVT_ISO_250, "250"));
    sIsoStr_->insert(Entry(IMVT_ISO_320, "320"));
    sIsoStr_->insert(Entry(IMVT_ISO_400, "400"));
    sIsoStr_->insert(Entry(IMVT_ISO_500, "500"));
    sIsoStr_->insert(Entry(IMVT_ISO_640, "640"));
    sIsoStr_->insert(Entry(IMVT_ISO_800, "800"));
    sIsoStr_->insert(Entry(IMVT_ISO_1000, "1000"));
    sIsoStr_->insert(Entry(IMVT_ISO_1250, "1250"));
    sIsoStr_->insert(Entry(IMVT_ISO_1600, "1600"));
    sIsoStr_->insert(Entry(IMVT_ISO_2000, "2000"));
    sIsoStr_->insert(Entry(IMVT_ISO_2500, "2500"));
    sIsoStr_->insert(Entry(IMVT_ISO_3200, "3200"));
    sIsoStr_->insert(Entry(IMVT_ISO_4000, "4000"));
    sIsoStr_->insert(Entry(IMVT_ISO_5000, "5000"));
    sIsoStr_->insert(Entry(IMVT_ISO_6400, "6400"));
    sIsoStr_->insert(Entry(IMVT_ISO_8000, "8000"));
    sIsoStr_->insert(Entry(IMVT_ISO_10000, "10000"));
    sIsoStr_->insert(Entry(IMVT_ISO_12800, "12800"));
    sIsoStr_->insert(Entry(IMVT_ISO_16000, "16000"));
    sIsoStr_->insert(Entry(IMVT_ISO_20000, "20000"));
    sIsoStr_->insert(Entry(IMVT_ISO_25600, "25600"));
    sIsoStr_->insert(Entry(IMVT_ISO_51200, "51200"));
    sIsoStr_->insert(Entry(IMVT_ISO_102400,"102400"));
    sIsoStr_->insert(Entry(IMVT_ISO_204800,"204800"));

    // XXX: Caution !!!!!!!!!!! read above comment first
    Map* sRotationStr_ = GetRotationStrMap();
    sRotationStr_->insert(Entry(IMVT_ROTATION_NORMAL, QT_TRANSLATE_NOOP("MenuItem", "Normal")));
    sRotationStr_->insert(Entry(IMVT_ROTATION_H_FLIP, QT_TRANSLATE_NOOP("MenuItem", "H Flip")));
    sRotationStr_->insert(Entry(IMVT_ROTATION_V_FLIP, QT_TRANSLATE_NOOP("MenuItem", "V Flip")));
    sRotationStr_->insert(Entry(IMVT_ROTATION_UPSIDE_DOWN, QT_TRANSLATE_NOOP("MenuItem", "Upside Down")));

    // XXX: Caution !!!!!!!!!!! read above comment first
    Map* sNoiseReductionStr_ = GetNoiseReductionStrMap();
    sNoiseReductionStr_->insert(Entry(IMVT_NOISE_REDUCTION_WEAK, QT_TRANSLATE_NOOP("MenuItem", "Weak")));
    sNoiseReductionStr_->insert(Entry(IMVT_NOISE_REDUCTION_NORMAL, QT_TRANSLATE_NOOP("MenuItem", "Medium")));
    sNoiseReductionStr_->insert(Entry(IMVT_NOISE_REDUCTION_STRONG, QT_TRANSLATE_NOOP("MenuItem", "Strong")));

    Map* sSharpnessStr_ = GetSharpnessStrMap();
    sSharpnessStr_->insert(Entry(0, QT_TRANSLATE_NOOP("MenuItem", "Strong")));
    sSharpnessStr_->insert(Entry(1, QT_TRANSLATE_NOOP("MenuItem", "Medium")));
    sSharpnessStr_->insert(Entry(2, QT_TRANSLATE_NOOP("MenuItem", "Weak")));

    // XXX: Caution !!!!!!!!!!! read above comment first
    Map* sQualityStr_ = GetQualityStrMap();
    sQualityStr_->insert(Entry(IMVT_PHOTO_QUALITY_JPEG, QT_TRANSLATE_NOOP("MenuItem", "JPEG")));
    sQualityStr_->insert(Entry(IMVT_PHOTO_QUALITY_RAW, QT_TRANSLATE_NOOP("MenuItem", "RAW")));

    // XXX: Caution !!!!!!!!!!! read above comment first
    Map* sFocusMethod_ = GetFocusMethodMap();
    sFocusMethod_->insert(Entry(IMVT_FOCUS_METHOD_MF, QT_TRANSLATE_NOOP("MenuItem", "MF")));
    sFocusMethod_->insert(Entry(IMVT_FOCUS_METHOD_AF, QT_TRANSLATE_NOOP("MenuItem", "AF")));

    Map* sFnumberStr_ = GetFnumberStrMap();
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_0P7, "0.7"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_0P8, "0.8"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_0P9, "0.9"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_1P0, "1"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_1P1, "1.1"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_1P2, "1.2"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_1P4, "1.4"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_1P6, "1.6"));
//    sFnumberStr_->insert(Entry(IMVT_FNUMBER_1P7, "1.7"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_1P8, "1.8"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_2P0, "2"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_2P2, "2.2"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_2P5, "2.5"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_2P8, "2.8"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_3P2, "3.2"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_3P5, "3.5"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_4P0, "4"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_4P5, "4.5"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_5P0, "5"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_5P6, "5.6"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_6P3, "6.3"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_7P1, "7.1"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_8P0, "8"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_9P0, "9"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_10P0, "10"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_11P0, "11"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_13P0, "13"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_14P0, "14"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_16P0, "16"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_18P0, "18"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_20P0, "20"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_22P0, "22"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_25P0, "25"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_29P0, "29"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_32P0, "32"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_36P0, "36"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_40P0, "40"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_45P0, "45"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_51P0, "51"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_57P0, "57"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_64P0, "64"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_72P0, "72"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_80P0, "80"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_90P0, "90"));

    // XXX: Caution !!!!!!!!!!! read above comment first
    Map* sAfModeStr_ = GetAfModeStrMap();
    sAfModeStr_->insert(Entry(IMVT_AF_NORMAL, QT_TRANSLATE_NOOP("MenuItem", "Normal")));
    sAfModeStr_->insert(Entry(IMVT_AF_SELECTION, QT_TRANSLATE_NOOP("MenuItem", "Selection")));
    sAfModeStr_->insert(Entry(IMVT_AF_FACEDETCTION, QT_TRANSLATE_NOOP("MenuItem", "Face")));

    // XXX: Caution !!!!!!!!!!! read above comment first
    Map* sBurstStr_ = GetBurstStrMap();
    sBurstStr_->insert(Entry(0, QT_TRANSLATE_NOOP("MenuItem", "Disable")));
    sBurstStr_->insert(Entry(3, QT_TRANSLATE_NOOP("MenuItem", "3")));
    sBurstStr_->insert(Entry(5, QT_TRANSLATE_NOOP("MenuItem", "5")));
    sBurstStr_->insert(Entry(7, QT_TRANSLATE_NOOP("MenuItem", "7")));
    sBurstStr_->insert(Entry(10, QT_TRANSLATE_NOOP("MenuItem", "10")));
    sBurstStr_->insert(Entry(15, QT_TRANSLATE_NOOP("MenuItem", "15")));
    sBurstStr_->insert(Entry(20, QT_TRANSLATE_NOOP("MenuItem", "20")));
    sBurstStr_->insert(Entry(25, QT_TRANSLATE_NOOP("MenuItem", "25")));
    sBurstStr_->insert(Entry(30, QT_TRANSLATE_NOOP("MenuItem", "30")));

    // XXX: Caution !!!!!!!!!!! read above comment first
    // compatiable with """static unsigned short still_max_shutter_time_table [] = {}""" in n3a_config_ae.c
    Map* sMaxExposureTimeStr_ = GetMaxExposureTimeStrMap();
    sMaxExposureTimeStr_->insert(Entry(0, QT_TRANSLATE_NOOP("MenuItem", "Auto")));
    sMaxExposureTimeStr_->insert(Entry(1, QT_TRANSLATE_NOOP("MenuItem", "1/2s")));
    sMaxExposureTimeStr_->insert(Entry(2, QT_TRANSLATE_NOOP("MenuItem", "1/3s")));
    sMaxExposureTimeStr_->insert(Entry(3, QT_TRANSLATE_NOOP("MenuItem", "1/4s")));
    sMaxExposureTimeStr_->insert(Entry(4, QT_TRANSLATE_NOOP("MenuItem", "1/5s")));
    sMaxExposureTimeStr_->insert(Entry(5, QT_TRANSLATE_NOOP("MenuItem", "1/8s")));
    sMaxExposureTimeStr_->insert(Entry(6, QT_TRANSLATE_NOOP("MenuItem", "1/10s")));
    sMaxExposureTimeStr_->insert(Entry(7, QT_TRANSLATE_NOOP("MenuItem", "1/15s")));
    sMaxExposureTimeStr_->insert(Entry(8, QT_TRANSLATE_NOOP("MenuItem", "1/20s")));
    sMaxExposureTimeStr_->insert(Entry(9, QT_TRANSLATE_NOOP("MenuItem", "1/25s")));
    sMaxExposureTimeStr_->insert(Entry(10, QT_TRANSLATE_NOOP("MenuItem", "1/30s")));
    sMaxExposureTimeStr_->insert(Entry(11, QT_TRANSLATE_NOOP("MenuItem", "1/40s")));
    sMaxExposureTimeStr_->insert(Entry(12, QT_TRANSLATE_NOOP("MenuItem", "1/50s")));
    sMaxExposureTimeStr_->insert(Entry(13, QT_TRANSLATE_NOOP("MenuItem", "1/60s")));
    sMaxExposureTimeStr_->insert(Entry(14, QT_TRANSLATE_NOOP("MenuItem", "1/80s")));
    sMaxExposureTimeStr_->insert(Entry(15, QT_TRANSLATE_NOOP("MenuItem", "1/100s")));
    sMaxExposureTimeStr_->insert(Entry(16, QT_TRANSLATE_NOOP("MenuItem", "1/125s")));
    sMaxExposureTimeStr_->insert(Entry(17, QT_TRANSLATE_NOOP("MenuItem", "1/160s")));
    sMaxExposureTimeStr_->insert(Entry(18, QT_TRANSLATE_NOOP("MenuItem", "1/200s")));
    sMaxExposureTimeStr_->insert(Entry(19, QT_TRANSLATE_NOOP("MenuItem", "1/250s")));
    sMaxExposureTimeStr_->insert(Entry(20, QT_TRANSLATE_NOOP("MenuItem", "1/320s")));

    Map* sShutterAngleStr_ = GetShutterAngleStrMap();
    sShutterAngleStr_->insert(Entry(IMVT_SHUTTER_ANGLE_AUTO, QT_TRANSLATE_NOOP("MenuItem", "Auto")));
    sShutterAngleStr_->insert(Entry(IMVT_SHUTTER_ANGLE_11P25, QT_TRANSLATE_NOOP("MenuItem", "11.25°")));
    sShutterAngleStr_->insert(Entry(IMVT_SHUTTER_ANGLE_15, QT_TRANSLATE_NOOP("MenuItem", "15°")));
    sShutterAngleStr_->insert(Entry(IMVT_SHUTTER_ANGLE_22P5, QT_TRANSLATE_NOOP("MenuItem", "22.5°")));
    sShutterAngleStr_->insert(Entry(IMVT_SHUTTER_ANGLE_30, QT_TRANSLATE_NOOP("MenuItem", "30°")));
    sShutterAngleStr_->insert(Entry(IMVT_SHUTTER_ANGLE_37P5, QT_TRANSLATE_NOOP("MenuItem", "37.5°")));
    sShutterAngleStr_->insert(Entry(IMVT_SHUTTER_ANGLE_45, QT_TRANSLATE_NOOP("MenuItem", "45°")));
    sShutterAngleStr_->insert(Entry(IMVT_SHUTTER_ANGLE_60, QT_TRANSLATE_NOOP("MenuItem", "60°")));
    sShutterAngleStr_->insert(Entry(IMVT_SHUTTER_ANGLE_72, QT_TRANSLATE_NOOP("MenuItem", "72°")));
    sShutterAngleStr_->insert(Entry(IMVT_SHUTTER_ANGLE_75, QT_TRANSLATE_NOOP("MenuItem", "75°")));
    sShutterAngleStr_->insert(Entry(IMVT_SHUTTER_ANGLE_90, QT_TRANSLATE_NOOP("MenuItem", "90°")));
    sShutterAngleStr_->insert(Entry(IMVT_SHUTTER_ANGLE_108, QT_TRANSLATE_NOOP("MenuItem", "108°")));
    sShutterAngleStr_->insert(Entry(IMVT_SHUTTER_ANGLE_120, QT_TRANSLATE_NOOP("MenuItem", "120°")));
    sShutterAngleStr_->insert(Entry(IMVT_SHUTTER_ANGLE_144, QT_TRANSLATE_NOOP("MenuItem", "144°")));
    sShutterAngleStr_->insert(Entry(IMVT_SHUTTER_ANGLE_150, QT_TRANSLATE_NOOP("MenuItem", "150°")));
    sShutterAngleStr_->insert(Entry(IMVT_SHUTTER_ANGLE_172P8, QT_TRANSLATE_NOOP("MenuItem", "172.8°")));
    sShutterAngleStr_->insert(Entry(IMVT_SHUTTER_ANGLE_180, QT_TRANSLATE_NOOP("MenuItem", "180°")));
    sShutterAngleStr_->insert(Entry(IMVT_SHUTTER_ANGLE_216, QT_TRANSLATE_NOOP("MenuItem", "216°")));
    sShutterAngleStr_->insert(Entry(IMVT_SHUTTER_ANGLE_324, QT_TRANSLATE_NOOP("MenuItem", "324°")));
    sShutterAngleStr_->insert(Entry(IMVT_SHUTTER_ANGLE_270, QT_TRANSLATE_NOOP("MenuItem", "270°")));
    sShutterAngleStr_->insert(Entry(IMVT_SHUTTER_ANGLE_360, QT_TRANSLATE_NOOP("MenuItem", "360°")));

    Map* sLensZoomStr_ = GetLensZoomStrMap();
    sLensZoomStr_->insert(Entry(IMVT_LENS_ZOOM_STOP, QT_TRANSLATE_NOOP("MenuItem", "stop")));
    sLensZoomStr_->insert(Entry(IMVT_LENS_ZOOM_IN, QT_TRANSLATE_NOOP("MenuItem", "in")));
    sLensZoomStr_->insert(Entry(IMVT_LENS_ZOOM_OUT, QT_TRANSLATE_NOOP("MenuItem", "out")));

    Map* sDcfNumberingStr_ = GetDcfNumberingStrMap();
    sDcfNumberingStr_->insert(Entry(IMVT_DCF_FILE_NUMBERING_AUTO_RESET, QT_TRANSLATE_NOOP("MenuItem", "Auto reset")));
    sDcfNumberingStr_->insert(Entry(IMVT_DCF_FILE_NUMBERING_CONTINUOUS, QT_TRANSLATE_NOOP("MenuItem", "Continuous")));

    Map* sCvbsSystemStr_ = GetCvbsSystemStrMap();
    sCvbsSystemStr_->insert(Entry(IMVT_VIDEO_SYSTEM_NTSC, QT_TRANSLATE_NOOP("MenuItem", "NTSC")));
    sCvbsSystemStr_->insert(Entry(IMVT_VIDEO_SYSTEM_PAL, QT_TRANSLATE_NOOP("MenuItem", "PAL")));
    sCvbsSystemStr_->insert(Entry(IMVT_VIDEO_SYSTEM_CINEMA, QT_TRANSLATE_NOOP("MenuItem", "CINEMA")));

    Map* sStillShutterSpeedStr_ = GetStillShutterSpeedStrMap();
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_AUTO, QT_TRANSLATE_NOOP("MenuItem", "Auto")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_30, QT_TRANSLATE_NOOP("MenuItem", "30\"")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_25, QT_TRANSLATE_NOOP("MenuItem", "25\"")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_20, QT_TRANSLATE_NOOP("MenuItem", "20\"")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_15, QT_TRANSLATE_NOOP("MenuItem", "15\"")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_13, QT_TRANSLATE_NOOP("MenuItem", "13\"")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_10, QT_TRANSLATE_NOOP("MenuItem", "10\"")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_8, QT_TRANSLATE_NOOP("MenuItem", "8\"")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_6, QT_TRANSLATE_NOOP("MenuItem", "6\"")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_5, QT_TRANSLATE_NOOP("MenuItem", "5\"")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_4, QT_TRANSLATE_NOOP("MenuItem", "4\"")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_3P2, QT_TRANSLATE_NOOP("MenuItem", "3.2\"")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_2P5, QT_TRANSLATE_NOOP("MenuItem", "2.5\"")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_2, QT_TRANSLATE_NOOP("MenuItem", "2\"")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1P6, QT_TRANSLATE_NOOP("MenuItem", "1.6\"")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1P3, QT_TRANSLATE_NOOP("MenuItem", "1.3\"")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1, QT_TRANSLATE_NOOP("MenuItem", "1\"")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D1P25, QT_TRANSLATE_NOOP("MenuItem", "0.8\"")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D1P67, QT_TRANSLATE_NOOP("MenuItem", "0.6\"")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D2, QT_TRANSLATE_NOOP("MenuItem", "0.5\"")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D2P5, QT_TRANSLATE_NOOP("MenuItem", "0.4\"")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D3, QT_TRANSLATE_NOOP("MenuItem", "0.3\"")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D4, QT_TRANSLATE_NOOP("MenuItem", "1/4")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D5, QT_TRANSLATE_NOOP("MenuItem", "1/5")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D6, QT_TRANSLATE_NOOP("MenuItem", "1/6")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D8, QT_TRANSLATE_NOOP("MenuItem", "1/8")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D10, QT_TRANSLATE_NOOP("MenuItem", "1/10")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D13, QT_TRANSLATE_NOOP("MenuItem", "1/13")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D15, QT_TRANSLATE_NOOP("MenuItem", "1/15")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D20, QT_TRANSLATE_NOOP("MenuItem", "1/20")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D21, QT_TRANSLATE_NOOP("MenuItem", "1/21")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D22, QT_TRANSLATE_NOOP("MenuItem", "1/22")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D23, QT_TRANSLATE_NOOP("MenuItem", "1/23")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D24, QT_TRANSLATE_NOOP("MenuItem", "1/24")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D25, QT_TRANSLATE_NOOP("MenuItem", "1/25")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D30, QT_TRANSLATE_NOOP("MenuItem", "1/30")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D40, QT_TRANSLATE_NOOP("MenuItem", "1/40")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D50, QT_TRANSLATE_NOOP("MenuItem", "1/50")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D60, QT_TRANSLATE_NOOP("MenuItem", "1/60")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D80, QT_TRANSLATE_NOOP("MenuItem", "1/80")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D100, QT_TRANSLATE_NOOP("MenuItem", "1/100")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D125, QT_TRANSLATE_NOOP("MenuItem", "1/125")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D160, QT_TRANSLATE_NOOP("MenuItem", "1/160")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D200, QT_TRANSLATE_NOOP("MenuItem", "1/200")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D240, QT_TRANSLATE_NOOP("MenuItem", "1/240")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D250, QT_TRANSLATE_NOOP("MenuItem", "1/250")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D320, QT_TRANSLATE_NOOP("MenuItem", "1/320")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D400, QT_TRANSLATE_NOOP("MenuItem", "1/400")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D500, QT_TRANSLATE_NOOP("MenuItem", "1/500")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D640, QT_TRANSLATE_NOOP("MenuItem", "1/640")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D800, QT_TRANSLATE_NOOP("MenuItem", "1/800")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D1000, QT_TRANSLATE_NOOP("MenuItem", "1/1000")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D1250, QT_TRANSLATE_NOOP("MenuItem", "1/1250")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D1600, QT_TRANSLATE_NOOP("MenuItem", "1/1600")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D2000, QT_TRANSLATE_NOOP("MenuItem", "1/2000")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D2500, QT_TRANSLATE_NOOP("MenuItem", "1/2500")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D3200, QT_TRANSLATE_NOOP("MenuItem", "1/3200")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D4000, QT_TRANSLATE_NOOP("MenuItem", "1/4000")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D5000, QT_TRANSLATE_NOOP("MenuItem", "1/5000")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D6400, QT_TRANSLATE_NOOP("MenuItem", "1/6400")));
    sStillShutterSpeedStr_->insert(Entry(IMVT_STILL_SHUTTER_1D8000, QT_TRANSLATE_NOOP("MenuItem", "1/8000")));

    Map* sCafRangeStr_ = GetCafRangeStrMap();
    sCafRangeStr_->insert(Entry(IMVT_CAF_RANGE_NORMAL, QT_TRANSLATE_NOOP("MenuItem", "Normal")));
    sCafRangeStr_->insert(Entry(IMVT_CAF_RANGE_FAR, QT_TRANSLATE_NOOP("MenuItem", "Far")));
    sCafRangeStr_->insert(Entry(IMVT_CAF_RANGE_NEAR, QT_TRANSLATE_NOOP("MenuItem", "Near")));

    Map* sCafSensitivityStr_ = GetCafSensitivityStrMap();
    sCafSensitivityStr_->insert(Entry(IMVT_CAF_SENSITIVITY_HIGH, QT_TRANSLATE_NOOP("MenuItem", "High")));
    sCafSensitivityStr_->insert(Entry(IMVT_CAF_SENSITIVITY_MIDDLE, QT_TRANSLATE_NOOP("MenuItem", "Middle")));
    sCafSensitivityStr_->insert(Entry(IMVT_CAF_SENSITIVITY_LOW, QT_TRANSLATE_NOOP("MenuItem", "Low")));

    Map* sPhotoBurstSpeedStr_ = GetPhotoBurstSpeedStrMap();
    sPhotoBurstSpeedStr_->insert(Entry(IMVT_PHOTO_BURST_SPEED_LOW0, QT_TRANSLATE_NOOP("MenuItem", "L")));
    sPhotoBurstSpeedStr_->insert(Entry(IMVT_PHOTO_BURST_SPEED_LOW1, QT_TRANSLATE_NOOP("MenuItem", "L1")));
    sPhotoBurstSpeedStr_->insert(Entry(IMVT_PHOTO_BURST_SPEED_MID, QT_TRANSLATE_NOOP("MenuItem", "M")));
    sPhotoBurstSpeedStr_->insert(Entry(IMVT_PHOTO_BURST_SPEED_HIGH0, QT_TRANSLATE_NOOP("MenuItem", "H")));
    sPhotoBurstSpeedStr_->insert(Entry(IMVT_PHOTO_BURST_SPEED_HIGH1, QT_TRANSLATE_NOOP("MenuItem", "H1")));

    Map* sLutTypeStr_ = GetLutTypeStrMap();
    sLutTypeStr_->insert(Entry(IMVT_LUT_TYPE_SRGB, QT_TRANSLATE_NOOP("MenuItem", "Rec.709")));
    sLutTypeStr_->insert(Entry(IMVT_LUT_TYPE_ZLOG, QT_TRANSLATE_NOOP("MenuItem", "Z-LOG")));
    sLutTypeStr_->insert(Entry(IMVT_LUT_TYPE_LINEAR, QT_TRANSLATE_NOOP("MenuItem", "Linear")));
    sLutTypeStr_->insert(Entry(IMVT_LUT_TYPE_FLAT, QT_TRANSLATE_NOOP("MenuItem", "FLAT")));
    sLutTypeStr_->insert(Entry(IMVT_LUT_TYPE_BT2020, QT_TRANSLATE_NOOP("MenuItem", "Rec.2020")));
    sLutTypeStr_->insert(Entry(IMVT_LUT_TYPE_CUSTOMIZED, QT_TRANSLATE_NOOP("MenuItem", "Customized")));

    Map* sCameraRotationStr_ = GetCameraRotationStrMap();
    sCameraRotationStr_->insert(Entry(IMVT_IMU_ROTATE_0, QT_TRANSLATE_NOOP("MenuItem", "0")));
    sCameraRotationStr_->insert(Entry(IMVT_IMU_ROTATE_90, QT_TRANSLATE_NOOP("MenuItem", "90")));
    sCameraRotationStr_->insert(Entry(IMVT_IMU_ROTATE_180, QT_TRANSLATE_NOOP("MenuItem", "180")));
    sCameraRotationStr_->insert(Entry(IMVT_IMU_ROTATE_270, QT_TRANSLATE_NOOP("MenuItem", "270")));

    Map* sCameraMultipleModeStr_ = GetCameraMultipleModeStrMap();
    sCameraMultipleModeStr_->insert(Entry(IMVT_CAMERA_NOT_MULTIPLE, QT_TRANSLATE_NOOP("MenuItem", "single")));
    sCameraMultipleModeStr_->insert(Entry(IMVT_CAMERA_AS_MULTIPLE_MASTER, QT_TRANSLATE_NOOP("MenuItem", "master")));
    sCameraMultipleModeStr_->insert(Entry(IMVT_CAMERA_AS_MULTIPLE_SLAVE, QT_TRANSLATE_NOOP("MenuItem", "slave")));

    Map* sShootingModeStr_ = GetShootingModeStrMap();
    sShootingModeStr_->insert(Entry(IMVT_AE_EXP_AV_SHUTTER_ISO, QT_TRANSLATE_NOOP("MenuItem", "Aperture priority")));
    sShootingModeStr_->insert(Entry(IMVT_AE_EXP_TV_IRIS_ISO, QT_TRANSLATE_NOOP("MenuItem", "Shutter priority")));
    sShootingModeStr_->insert(Entry(IMVT_AE_EXP_P_LEICA_SHUTTER_IRIS_ISO, QT_TRANSLATE_NOOP("MenuItem", "Program AE")));
    sShootingModeStr_->insert(Entry(IMVT_AE_EXP_M, QT_TRANSLATE_NOOP("MenuItem", "Manual mode")));

    Map* sLensOisModeStr_ = GetLensOisModeStrMap();
    sLensOisModeStr_->insert(Entry(IMVT_LENS_OIS_OFF, QT_TRANSLATE_NOOP("MenuItem", "Off")));
    sLensOisModeStr_->insert(Entry(IMVT_LENS_OIS_HORIZONTAL, QT_TRANSLATE_NOOP("MenuItem", "Normal")));
    sLensOisModeStr_->insert(Entry(IMVT_LENS_OIS_VERTICAL, QT_TRANSLATE_NOOP("MenuItem", "Vertical")));

    Map* sCardFormatTypeStr_ = GetCardFormatTypeStrMap();
    sCardFormatTypeStr_->insert(Entry(IMVT_FAT_TYPE_FAT32, QT_TRANSLATE_NOOP("MenuItem", "FAT32")));
    sCardFormatTypeStr_->insert(Entry(IMVT_FAT_TYPE_EXFAT, QT_TRANSLATE_NOOP("MenuItem", "exFAT")));

    Map* sMovieRecordDurationStr_ = GetMovieRecordDurationStrMap();
    sMovieRecordDurationStr_->insert(Entry(IMVT_MOVIE_RECORD_SPLIT_DURATION_0SEC, QT_TRANSLATE_NOOP("MenuItem", "Disable")));
    sMovieRecordDurationStr_->insert(Entry(IMVT_MOVIE_RECORD_SPLIT_DURATION_3SEC, QT_TRANSLATE_NOOP("MenuItem", "3s")));
    sMovieRecordDurationStr_->insert(Entry(IMVT_MOVIE_RECORD_SPLIT_DURATION_5SEC, QT_TRANSLATE_NOOP("MenuItem", "5s")));
    sMovieRecordDurationStr_->insert(Entry(IMVT_MOVIE_RECORD_SPLIT_DURATION_10SEC, QT_TRANSLATE_NOOP("MenuItem", "10s")));
    sMovieRecordDurationStr_->insert(Entry(IMVT_MOVIE_RECORD_SPLIT_DURATION_3MIN, QT_TRANSLATE_NOOP("MenuItem", "3min")));
    sMovieRecordDurationStr_->insert(Entry(IMVT_MOVIE_RECORD_SPLIT_DURATION_5MIN, QT_TRANSLATE_NOOP("MenuItem", "5min")));
    sMovieRecordDurationStr_->insert(Entry(IMVT_MOVIE_RECORD_SPLIT_DURATION_10MIN, QT_TRANSLATE_NOOP("MenuItem", "10min")));
    sMovieRecordDurationStr_->insert(Entry(IMVT_MOVIE_RECORD_SPLIT_DURATION_20MIN, QT_TRANSLATE_NOOP("MenuItem", "20min")));
    sMovieRecordDurationStr_->insert(Entry(IMVT_MOVIE_RECORD_SPLIT_DURATION_30MIN, QT_TRANSLATE_NOOP("MenuItem", "30min")));

    Map* sMovieAudioEncTypeStr_ = GetMovieAudioEncTypeStrMap();
    sMovieAudioEncTypeStr_->insert(Entry(IMVT_AUDIO_ENCODER_NONE, QT_TRANSLATE_NOOP("MenuItem", "None")));
    sMovieAudioEncTypeStr_->insert(Entry(IMVT_AUDIO_ENCODER_AAC, QT_TRANSLATE_NOOP("MenuItem", "AAC")));
    sMovieAudioEncTypeStr_->insert(Entry(IMVT_AUDIO_ENCODER_PCM, QT_TRANSLATE_NOOP("MenuItem", "PCM")));

    Map* sRecordModeStr_ = GetRecordModeStrMap();
    sRecordModeStr_->insert(Entry(0, QT_TRANSLATE_NOOP("MenuItem", "Normal")));
    sRecordModeStr_->insert(Entry(1, QT_TRANSLATE_NOOP("MenuItem", "Timelapse")));

    Map* sMovieEncoderStr_ = GetMovieEncoderStrMap();
    sMovieEncoderStr_->insert(Entry(IMVT_VIDEO_ENCODER_H264, QT_TRANSLATE_NOOP("MenuItem", "H.264")));
    sMovieEncoderStr_->insert(Entry(IMVT_VIDEO_ENCODER_RAW, QT_TRANSLATE_NOOP("MenuItem", "RAW")));
    sMovieEncoderStr_->insert(Entry(IMVT_VIDEO_ENCODER_H265, QT_TRANSLATE_NOOP("MenuItem", "H.265")));
    sMovieEncoderStr_->insert(Entry(IMVT_VIDEO_ENCODER_PRORES_422, QT_TRANSLATE_NOOP("MenuItem", "ProRes 422")));
    sMovieEncoderStr_->insert(Entry(IMVT_VIDEO_ENCODER_PRORES_422_HQ, QT_TRANSLATE_NOOP("MenuItem", "ProRes HQ")));
    sMovieEncoderStr_->insert(Entry(IMVT_VIDEO_ENCODER_PRORES_422_LT, QT_TRANSLATE_NOOP("MenuItem", "ProRes LT")));
    sMovieEncoderStr_->insert(Entry(IMVT_VIDEO_ENCODER_PRORES_422_PROXY, QT_TRANSLATE_NOOP("MenuItem", "ProRes Proxy")));

    Map* sPhotoDriveModeStr_ = GetPhotoDriveModeStrMap();
    sPhotoDriveModeStr_->insert(Entry(IMVT_DRIVE_MODE_SINGLE, QT_TRANSLATE_NOOP("MenuItem", "Single")));
    sPhotoDriveModeStr_->insert(Entry(IMVT_DRIVE_MODE_BURST, QT_TRANSLATE_NOOP("MenuItem", "Burst Shooting")));
    sPhotoDriveModeStr_->insert(Entry(IMVT_DRIVE_MODE_TIMELAPSE, QT_TRANSLATE_NOOP("MenuItem", "Timelapse")));
    sPhotoDriveModeStr_->insert(Entry(IMVT_DRIVE_MODE_SELF_TIMER, QT_TRANSLATE_NOOP("MenuItem", "Self-timer")));

    Map* sAutoOffStr_ = GetAutoOffStrMap();
    sAutoOffStr_->insert(Entry(IMVT_AUTO_OFF_TIME_MANUAL, QT_TRANSLATE_NOOP("MenuItem", "Manual")));
    sAutoOffStr_->insert(Entry(IMVT_AUTO_OFF_TIME_30_SECONDS, QT_TRANSLATE_NOOP("MenuItem", "30s")));
    sAutoOffStr_->insert(Entry(IMVT_AUTO_OFF_TIME_1MIN, QT_TRANSLATE_NOOP("MenuItem", "1min")));
    sAutoOffStr_->insert(Entry(IMVT_AUTO_OFF_TIME_2MIN, QT_TRANSLATE_NOOP("MenuItem", "2min")));
    sAutoOffStr_->insert(Entry(IMVT_AUTO_OFF_TIME_3MIN, QT_TRANSLATE_NOOP("MenuItem", "3min")));
    sAutoOffStr_->insert(Entry(IMVT_AUTO_OFF_TIME_4MIN, QT_TRANSLATE_NOOP("MenuItem", "4min")));
    sAutoOffStr_->insert(Entry(IMVT_AUTO_OFF_TIME_5MIN, QT_TRANSLATE_NOOP("MenuItem", "5min")));
    sAutoOffStr_->insert(Entry(IMVT_AUTO_OFF_TIME_7MIN, QT_TRANSLATE_NOOP("MenuItem", "7min")));
    sAutoOffStr_->insert(Entry(IMVT_AUTO_OFF_TIME_8MIN, QT_TRANSLATE_NOOP("MenuItem", "8min")));
    sAutoOffStr_->insert(Entry(IMVT_AUTO_OFF_TIME_15MIN, QT_TRANSLATE_NOOP("MenuItem", "15min")));

    Map* sSendVideoStreamStr_ = GetSendVideoStreamStrMap();
    sSendVideoStreamStr_->insert(Entry(IMVT_STREAM_STYLE_MAIN, QT_TRANSLATE_NOOP("MenuItem", "Stream0")));
    sSendVideoStreamStr_->insert(Entry(IMVT_STREAM_STYLE_SEC, QT_TRANSLATE_NOOP("MenuItem", "Stream1")));

    Map* sN3aUnionAeAwbStr_ = GetN3aUnionAeAwbStrMap();
    sN3aUnionAeAwbStr_->insert(Entry(IMVT_N3A_UNION_MODE_DISABLE, QT_TRANSLATE_NOOP("MenuItem", "Disable")));
    sN3aUnionAeAwbStr_->insert(Entry(IMVT_N3A_UNION_MODE_MASTER, QT_TRANSLATE_NOOP("MenuItem", "Master")));
    sN3aUnionAeAwbStr_->insert(Entry(IMVT_N3A_UNION_MODE_WEIGHT, QT_TRANSLATE_NOOP("MenuItem", "Weight")));
    sN3aUnionAeAwbStr_->insert(Entry(IMVT_N3A_UNION_MODE_AVERAGE, QT_TRANSLATE_NOOP("MenuItem", "Average")));
    sN3aUnionAeAwbStr_->insert(Entry(IMVT_N3A_UNION_MODE_ENABLE, QT_TRANSLATE_NOOP("MenuItem", "Enable")));

    Map* sN3aUnionDeltaStr_ = GetN3aUnionDeltaStrMap();
    sN3aUnionDeltaStr_->insert(Entry(IMVT_N3A_UNION_DELTA_MIN, QT_TRANSLATE_NOOP("MenuItem", "None")));
    sN3aUnionDeltaStr_->insert(Entry(IMVT_N3A_UNION_DELTA_MEDIAN, QT_TRANSLATE_NOOP("MenuItem", "Medium")));
    sN3aUnionDeltaStr_->insert(Entry(IMVT_N3A_UNION_DELTA_MAX, QT_TRANSLATE_NOOP("MenuItem", "High")));
    sN3aUnionDeltaStr_->insert(Entry(IMVT_N3A_UNION_DELTA_AUTO, QT_TRANSLATE_NOOP("MenuItem", "Auto")));

    //!!! must the same with "static unsigned short video_shutter_time_to_shutter_speed[] = {}" in n3a_config_ae.c
    Map* sVideoShutterTimeStr_ = GetVideoShutterTimeStrMap();
    sVideoShutterTimeStr_->insert(Entry(IMVT_VIDEO_SHUTTER_TIME_AUTO, QT_TRANSLATE_NOOP("MenuItem", "Auto")));
    sVideoShutterTimeStr_->insert(Entry(IMVT_VIDEO_SHUTTER_TIME_1D20, QT_TRANSLATE_NOOP("MenuItem", "1/20")));
    sVideoShutterTimeStr_->insert(Entry(IMVT_VIDEO_SHUTTER_TIME_1D21, QT_TRANSLATE_NOOP("MenuItem", "1/21")));
    sVideoShutterTimeStr_->insert(Entry(IMVT_VIDEO_SHUTTER_TIME_1D22, QT_TRANSLATE_NOOP("MenuItem", "1/22")));
    sVideoShutterTimeStr_->insert(Entry(IMVT_VIDEO_SHUTTER_TIME_1D23, QT_TRANSLATE_NOOP("MenuItem", "1/23")));
    sVideoShutterTimeStr_->insert(Entry(IMVT_VIDEO_SHUTTER_TIME_1D24, QT_TRANSLATE_NOOP("MenuItem", "1/24")));
    sVideoShutterTimeStr_->insert(Entry(IMVT_VIDEO_SHUTTER_TIME_1D25, QT_TRANSLATE_NOOP("MenuItem", "1/25")));
    sVideoShutterTimeStr_->insert(Entry(IMVT_VIDEO_SHUTTER_TIME_1D30, QT_TRANSLATE_NOOP("MenuItem", "1/30")));
    sVideoShutterTimeStr_->insert(Entry(IMVT_VIDEO_SHUTTER_TIME_1D40, QT_TRANSLATE_NOOP("MenuItem", "1/40")));
    sVideoShutterTimeStr_->insert(Entry(IMVT_VIDEO_SHUTTER_TIME_1D48, QT_TRANSLATE_NOOP("MenuItem", "1/48")));
    sVideoShutterTimeStr_->insert(Entry(IMVT_VIDEO_SHUTTER_TIME_1D50, QT_TRANSLATE_NOOP("MenuItem", "1/50")));
    sVideoShutterTimeStr_->insert(Entry(IMVT_VIDEO_SHUTTER_TIME_1D60, QT_TRANSLATE_NOOP("MenuItem", "1/60")));
    sVideoShutterTimeStr_->insert(Entry(IMVT_VIDEO_SHUTTER_TIME_1D80, QT_TRANSLATE_NOOP("MenuItem", "1/80")));
    sVideoShutterTimeStr_->insert(Entry(IMVT_VIDEO_SHUTTER_TIME_1D100, QT_TRANSLATE_NOOP("MenuItem", "1/100")));
    sVideoShutterTimeStr_->insert(Entry(IMVT_VIDEO_SHUTTER_TIME_1D120, QT_TRANSLATE_NOOP("MenuItem", "1/120")));
    sVideoShutterTimeStr_->insert(Entry(IMVT_VIDEO_SHUTTER_TIME_1D125, QT_TRANSLATE_NOOP("MenuItem", "1/125")));
    sVideoShutterTimeStr_->insert(Entry(IMVT_VIDEO_SHUTTER_TIME_1D160, QT_TRANSLATE_NOOP("MenuItem", "1/160")));
    sVideoShutterTimeStr_->insert(Entry(IMVT_VIDEO_SHUTTER_TIME_1D200, QT_TRANSLATE_NOOP("MenuItem", "1/200")));
    sVideoShutterTimeStr_->insert(Entry(IMVT_VIDEO_SHUTTER_TIME_1D240, QT_TRANSLATE_NOOP("MenuItem", "1/240")));
    sVideoShutterTimeStr_->insert(Entry(IMVT_VIDEO_SHUTTER_TIME_1D250, QT_TRANSLATE_NOOP("MenuItem", "1/250")));
    sVideoShutterTimeStr_->insert(Entry(IMVT_VIDEO_SHUTTER_TIME_1D320, QT_TRANSLATE_NOOP("MenuItem", "1/320")));
    sVideoShutterTimeStr_->insert(Entry(IMVT_VIDEO_SHUTTER_TIME_1D400, QT_TRANSLATE_NOOP("MenuItem", "1/400")));
    sVideoShutterTimeStr_->insert(Entry(IMVT_VIDEO_SHUTTER_TIME_1D500, QT_TRANSLATE_NOOP("MenuItem", "1/500")));
    sVideoShutterTimeStr_->insert(Entry(IMVT_VIDEO_SHUTTER_TIME_1D640, QT_TRANSLATE_NOOP("MenuItem", "1/640")));
    sVideoShutterTimeStr_->insert(Entry(IMVT_VIDEO_SHUTTER_TIME_1D800, QT_TRANSLATE_NOOP("MenuItem", "1/800")));
    sVideoShutterTimeStr_->insert(Entry(IMVT_VIDEO_SHUTTER_TIME_1D1000, QT_TRANSLATE_NOOP("MenuItem", "1/1000")));
    sVideoShutterTimeStr_->insert(Entry(IMVT_VIDEO_SHUTTER_TIME_1D1250, QT_TRANSLATE_NOOP("MenuItem", "1/1250")));
    sVideoShutterTimeStr_->insert(Entry(IMVT_VIDEO_SHUTTER_TIME_1D1600, QT_TRANSLATE_NOOP("MenuItem", "1/1600")));
    sVideoShutterTimeStr_->insert(Entry(IMVT_VIDEO_SHUTTER_TIME_1D2000, QT_TRANSLATE_NOOP("MenuItem", "1/2000")));
    sVideoShutterTimeStr_->insert(Entry(IMVT_VIDEO_SHUTTER_TIME_1D2500, QT_TRANSLATE_NOOP("MenuItem", "1/2500")));
    sVideoShutterTimeStr_->insert(Entry(IMVT_VIDEO_SHUTTER_TIME_1D3200, QT_TRANSLATE_NOOP("MenuItem", "1/3200")));
    sVideoShutterTimeStr_->insert(Entry(IMVT_VIDEO_SHUTTER_TIME_1D4000, QT_TRANSLATE_NOOP("MenuItem", "1/4000")));
    sVideoShutterTimeStr_->insert(Entry(IMVT_VIDEO_SHUTTER_TIME_1D5000, QT_TRANSLATE_NOOP("MenuItem", "1/5000")));
    sVideoShutterTimeStr_->insert(Entry(IMVT_VIDEO_SHUTTER_TIME_1D6400, QT_TRANSLATE_NOOP("MenuItem", "1/6400")));
    sVideoShutterTimeStr_->insert(Entry(IMVT_VIDEO_SHUTTER_TIME_1D8000, QT_TRANSLATE_NOOP("MenuItem", "1/8000")));

    Map* sNetworkModeStr_ = GetNetworkModeStrMap();
    sNetworkModeStr_->insert(Entry(IMVT_CAMERA_NETWORK_MODE_ROUTER, QT_TRANSLATE_NOOP("MenuItem", "Router")));
    sNetworkModeStr_->insert(Entry(IMVT_CAMERA_NETWORK_MODE_DIRECT, QT_TRANSLATE_NOOP("MenuItem", "Direct")));
    sNetworkModeStr_->insert(Entry(IMVT_CAMERA_NETWORK_MODE_STATIC, QT_TRANSLATE_NOOP("MenuItem", "Static")));

    Map* sBitrateLevelStr_ = GetBirateLevelMap();
    sBitrateLevelStr_->insert(Entry(IMVT_BITRATE_LEVEL_HIGHT,  QT_TRANSLATE_NOOP("MenuItem","High")));
    sBitrateLevelStr_->insert(Entry(IMVT_BITRATE_LEVEL_MIDDLE,  QT_TRANSLATE_NOOP("MenuItem","Middle")));
    sBitrateLevelStr_->insert(Entry(IMVT_BITRATE_LEVEL_LOW,  QT_TRANSLATE_NOOP("MenuItem","Low")));

    Map* sAeLockStr_ = GetAeLockMap();
    sAeLockStr_->insert(Entry(IMVT_AE_UNLOCK,  QT_TRANSLATE_NOOP("MenuItem","Unlock")));
    sAeLockStr_->insert(Entry(IMVT_AE_LOCK,  QT_TRANSLATE_NOOP("MenuItem","Lock")));

    Map* sStyleStr_ = GetStyleMap();
    sStyleStr_->insert(Entry(IMVT_STYLE_SYSTEM,  QT_TRANSLATE_NOOP("MenuItem","System")));
    sStyleStr_->insert(Entry(IMVT_STYLE_USER,  QT_TRANSLATE_NOOP("MenuItem","User")));

    Map* sRecordFileFormatStr_ = GetRecordFileFormat();
    sRecordFileFormatStr_->insert(Entry(IMVT_REC_FILE_FMT_MOV,  QT_TRANSLATE_NOOP("MenuItem","MOV")));
    sRecordFileFormatStr_->insert(Entry(IMVT_REC_FILE_FMT_MP4,  QT_TRANSLATE_NOOP("MenuItem","MP4")));

    Map *sHdmiFmtStr_ = GetHdmiFmtStrMap();
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_RESERVED, "Auto"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_DMT0659, "DMT0659"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_480P, "480P60"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_480P_WIDE, "480P60_WIDE"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_720P, "720P60"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_1080, "1080I60"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_480I, "480I60"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_480I_WIDE, "480I60_WIDE"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_240P, "240P60"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_240P_WIDE, "240P60_WIDE"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_480I4X, "480I4X"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_480I4X_WIDE, "480I4X_WIDE"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_240P4X, "240P4X"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_240P4X_WIDE, "240P4X_WIDE"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_480P2X, "480P2X"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_480P2X_WIDE, "480P2X_WIDE"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_1080P60, "1080P60"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_576P, "576P50"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_576P_WIDE, "576P50_WIDE"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_480P_WIDE, "480P60_WIDE"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_720P50, "720P50"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_1080I25, "1080I25"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_576I, "576I50"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_576I_WIDE, "576I50_WIDE"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_288P, "288P50"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_288P_WIDE, "288P50_WIDE"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_576I4X, "576I4X"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_576I4X_WIDE, "576I4X_WIDE"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_288P4X, "288P4X"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_288P4X_WIDE, "288P4X_WIDE"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_576P2X, "576P2X"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_576P2X_WIDE, "576P2X_WIDE"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_1080P50, "1080P50"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_1080P24, "1080P24"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_1080P25, "1080P25"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_1080P30, "1080P30"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_480P4X, "480P4X"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_480P4X_WIDE, "480P4X_WIDE"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_576P4X, "576P4X"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_576P4X_WIDE, "576P4X_WIDE"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_1080I25_AS, "1080I25_AS"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_1080I50, "1080I50"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_720P100, "720P100"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_576P100, "576P100"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_576P100_WIDE, "576P100_WIDE"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_576I50, "576I50"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_576I50_WIDE, "576I50_WIDE"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_1080I60, "1080I60"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_720P120, "720P120"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_480P119, "480P119"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_480P119_WIDE, "480P119_WIDE"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_480I59, "480I59"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_480I59_WIDE, "480I59_WIDE"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_576P200, "576P200"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_576P200_WIDE, "576P200_WIDE"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_576I100, "576I100"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_576I100_WIDE, "576I100_WIDE"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_480P239, "480P239"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_480P239_WIDE, "480P239_WIDE"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_480I119, "480I119"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_480I119_WIDE, "480I119_WIDE"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_720P24, "720P24"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_720P25, "720P25"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_720P30, "720P30"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_1080P120, "1080P120"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_1080P100, "1080P100"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_2160P24, "4K UHD P24"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_2160P25, "4K UHD P25"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_2160P30, "4K UHD P30"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_2160P50, "4K UHD P50"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_2160P60, "4K UHD P60"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_4096_2160P24, "4K DCI P24"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_4096_2160P25, "4K DCI P25"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_4096_2160P30, "4K DCI P30"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_4096_2160P50, "4K DCI P50"));
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_4096_2160P60, "4K DCI P60"));

    Map *sWbTintPriorityStr_ = GetWbTintPriorityStrMap();
    sWbTintPriorityStr_->insert(Entry(IMVT_WB_PRIORITY_AMBIANCE, QT_TRANSLATE_NOOP("MenuItem", "Ambiance")));
    sWbTintPriorityStr_->insert(Entry(IMVT_WB_PRIORITY_WHITE, QT_TRANSLATE_NOOP("MenuItem", "White")));

    Map* sDualNativeIsoModeStr_ = GetDualNativeIsoModeMap();
    sDualNativeIsoModeStr_->insert(Entry(IMVT_DUAL_NATIVE_ISO_MODE_AUTO,  QT_TRANSLATE_NOOP("MenuItem","Auto")));
    sDualNativeIsoModeStr_->insert(Entry(IMVT_DUAL_NATIVE_ISO_MODE_LOW,   QT_TRANSLATE_NOOP("MenuItem","Low")));
    sDualNativeIsoModeStr_->insert(Entry(IMVT_DUAL_NATIVE_ISO_MODE_HIGH,  QT_TRANSLATE_NOOP("MenuItem","High")));

    Map *sEvStr_ = GetEvMap();
    sEvStr_->insert(Entry(IMVT_EV_N96, QT_TRANSLATE_NOOP("MenuItem", "-3.0")));
    sEvStr_->insert(Entry(IMVT_EV_N86, QT_TRANSLATE_NOOP("MenuItem", "-2.7")));
    sEvStr_->insert(Entry(IMVT_EV_N74, QT_TRANSLATE_NOOP("MenuItem", "-2.3")));
    sEvStr_->insert(Entry(IMVT_EV_N64, QT_TRANSLATE_NOOP("MenuItem", "-2.0")));
    sEvStr_->insert(Entry(IMVT_EV_N54, QT_TRANSLATE_NOOP("MenuItem", "-1.7")));
    sEvStr_->insert(Entry(IMVT_EV_N42, QT_TRANSLATE_NOOP("MenuItem", "-1.3")));
    sEvStr_->insert(Entry(IMVT_EV_N32, QT_TRANSLATE_NOOP("MenuItem", "-1.0")));
    sEvStr_->insert(Entry(IMVT_EV_N22, QT_TRANSLATE_NOOP("MenuItem", "-0.7")));
    sEvStr_->insert(Entry(IMVT_EV_N10, QT_TRANSLATE_NOOP("MenuItem", "-0.3")));
    sEvStr_->insert(Entry(IMVT_EV_ZREO, QT_TRANSLATE_NOOP("MenuItem", "0")));
    sEvStr_->insert(Entry(IMVT_EV_P10, QT_TRANSLATE_NOOP("MenuItem", "0.3")));
    sEvStr_->insert(Entry(IMVT_EV_P22, QT_TRANSLATE_NOOP("MenuItem", "0.7")));
    sEvStr_->insert(Entry(IMVT_EV_P32, QT_TRANSLATE_NOOP("MenuItem", "1.0")));
    sEvStr_->insert(Entry(IMVT_EV_P42, QT_TRANSLATE_NOOP("MenuItem", "1.3")));
    sEvStr_->insert(Entry(IMVT_EV_P54, QT_TRANSLATE_NOOP("MenuItem", "1.7")));
    sEvStr_->insert(Entry(IMVT_EV_P64, QT_TRANSLATE_NOOP("MenuItem", "2.0")));
    sEvStr_->insert(Entry(IMVT_EV_P74, QT_TRANSLATE_NOOP("MenuItem", "2.3")));
    sEvStr_->insert(Entry(IMVT_EV_P86, QT_TRANSLATE_NOOP("MenuItem", "2.7")));
    sEvStr_->insert(Entry(IMVT_EV_P96, QT_TRANSLATE_NOOP("MenuItem", "3.0")));
}

static IntMap *getBurstSpeedMap()
{
    static IntMap map;
    if (map.empty()) {
        map.insert(IntEntry(IMVT_PHOTO_BURST_SPEED_LOW0, 5));
        map.insert(IntEntry(IMVT_PHOTO_BURST_SPEED_LOW1, 5));
        map.insert(IntEntry(IMVT_PHOTO_BURST_SPEED_MID, 2));
        map.insert(IntEntry(IMVT_PHOTO_BURST_SPEED_HIGH0, 1));
        map.insert(IntEntry(IMVT_PHOTO_BURST_SPEED_HIGH1, 1));
    }
    return &map;
}

IntMap *CameraConfig::getIntMapByKey(int key)
{
    IntMap *map = nullptr;
    switch (key) {
    case CAMERA_CONFIG_PHOTO_BURST_SPEED:
        map = getBurstSpeedMap();
        break;
    default:
        break;
    }
    return map;
}

int CameraConfig::trValueFromIndex(int key, int index)
{
    IntMap *map = getIntMapByKey(key);
    if (map) {
        IntMap::iterator it = map->find(index);
        if (it != map->end()) {
            return it->second;
        }
    }
    return -1;
}

const char* CameraConfig::trKeyFromInt(int key)
{
    Map* sConfigKey_ = GetConfigKeyMap();
    Map::iterator it = sConfigKey_->find(key);
    if (it == sConfigKey_->end()) {
        return NULL;
    }
    return it->second;
}

Map* CameraConfig::getMapBykey(int key)
{
    Map *map = NULL;
    switch (key) {
    case CAMERA_CONFIG_MOVIE_RESOLUTION:
        map = GetMovResolutionStrMap();
        break;
    case CAMERA_CONFIG_MOVIE_PROJECT_FPS:
        map = GetMovFpsStrMap();
        break;
    case CAMERA_CONFIG_MOVIE_FORMAT:
    case CAMERA_CONFIG_SECONDARY_STREAM_RESOLUTION:
        map = GetMovFmtStrMap();
        break;
    case CAMERA_CONFIG_PHOTO_SIZE:
        map = GetPhotoSizeStrMap();
        break;
    case CAMERA_CONFIG_WIFI_ONOFF:
    case CAMERA_CONFIG_ASSITOOL_PEAK_ONOFF:
        map = GetOnOffStrMap();
        break;
    case CAMERA_CONFIG_WB:
        map = GetWbStrMap();
        break;
    case CAMERA_CONFIG_FLICKER_REDUCTION:
        map = GetFlickerStrMap();
        break;
    case CAMERA_CONFIG_AE_METER_MODE:
        map = GetAeMeteringStrMap();
        break;
    case CAMERA_CONFIG_ISO:
    case CAMERA_CONFIG_MAX_ISO_LIMIT:
        map = GetIsoStrMap();
        break;
    case CAMERA_CONFIG_LCD_ONOFF:
    case CAMERA_CONFIG_LED_ENABLE:
    case CAMERA_CONFIG_BEEPER_ENABLE:
    case CAMERA_CONFIG_HDMI_OSD_ONOFF:
    case CAMERA_CONFIG_OLED_ONOFF_ENABLE:
    case CAMERA_CONFIG_PHOTO_BURST:
    case CAMERA_CONFIG_CVBS_OUTPUT_ENABLE:
    case CAMERA_CONFIG_DEWARP_ONOFF:
    case CAMERA_CONFIG_VIGNETTE_ONOFF:
    case CAMERA_CONFIG_MULTIPLE_TIMEOUT_ENABLE:
    case CAMERA_CONFIG_LIVEVIEW_WITH_AUDIO:
    case CAMERA_CONFIG_CAF_ONOFF_CONFIG:
    case CAMERA_CONFIG_LEVEL_CORRECTION_ONOFF:
    case CAMERA_CONFIG_GRID_DISPLAY_ONOFF:
        map = GetOnOffStrMap();
        break;
    case CAMERA_CONFIG_ROTATION:
        map = GetRotationStrMap();
        break;
    case CAMERA_CONFIG_PHOTO_QUALITY:
        map = GetQualityStrMap();
        break;
    case CAMERA_CONFIG_FOCUS_METHOD:
        map = GetFocusMethodMap();
        break;
    case CAMERA_CONFIG_IRIS:
        map = GetFnumberStrMap();
        break;
    case CAMERA_CONFIG_AF_MODE:
        map = GetAfModeStrMap();
        break;
    case CAMERA_CONFIG_MAX_SHUTTER_SPEED:
        map = GetMaxExposureTimeStrMap();
        break;
    case CAMERA_CONFIG_SHUTTER_ANGLE:
    case CAMERA_CONFIG_MAX_EXPOSURE_SHR_ANGLE:
        map = GetShutterAngleStrMap();
        break;
    case CAMERA_CONFIG_LENS_ZOOM:
        map = GetLensZoomStrMap();
        break;
    case CAMERA_CONFIG_DCF_FILE_NUMBERING:
        map = GetDcfNumberingStrMap();
        break;
    case CAMERA_CONFIG_CVBS_VIDEO_SYSTEM:
        map = GetCvbsSystemStrMap();
        break;
    case CAMERA_CONFIG_STILL_SHUTTER_SPEED:
        map = GetStillShutterSpeedStrMap();
        break;
    case CAMERA_CONFIG_CAF_RANGE:
        map = GetCafRangeStrMap();
        break;
    case CAMERA_CONFIG_CAF_SENSITIVITY:
        map = GetCafSensitivityStrMap();
        break;
    case CAMERA_CONFIG_PHOTO_BURST_SPEED:
        map = GetPhotoBurstSpeedStrMap();
        break;
    case CAMERA_CONFIG_LUT_TYPE:
        map = GetLutTypeStrMap();
        break;
    case CAMERA_CONFIG_IMU_ROTATION:
    case CAMERA_CONFIG_ENC_ROTATION:
        map = GetCameraRotationStrMap();
        break;
    case CAMERA_CONFIG_USB_DEVICE_ROLE:
        map = GetUsbDeviceRoleStrMap();
        break;
    case CAMERA_CONFIG_CAMERA_IS_MULTIPLE:
        map = GetCameraMultipleModeStrMap();
        break;
    case CAMERA_CONFIG_RECORD_INT_CAP:
        map = GetRecordModeStrMap();
        break;
    case CAMERA_CONFIG_NOISE_REDUCTION:
        map = GetNoiseReductionStrMap();
        break;
    case CAMERA_CONFIG_SHARPNESS:
        map = GetSharpnessStrMap();
        break;
    case CAMERA_CONFIG_AE_EXPOSURE_MDOE:
        map = GetShootingModeStrMap();
        break;
    case CAMERA_CONFIG_OIS_MODE:
        map = GetLensOisModeStrMap();
        break;
    case CAMERA_CONFIG_MOVIE_RECORD_DURATION:
        map = GetMovieRecordDurationStrMap();
        break;
    case CAMERA_CONFIG_SECONDARY_AUDIO_TYPE:
    case CAMERA_CONFIG_PRIMARY_AUDIO_TYPE:
        map = GetMovieAudioEncTypeStrMap();
        break;
    case CAMERA_CONFIG_VIDEO_ENCODER:
        map = GetMovieEncoderStrMap();
        break;
    case CAMERA_CONFIG_DRIVE_MODE:
        map = GetPhotoDriveModeStrMap();
        break;
    case CAMERA_CONFIG_AUTO_OFF:
    case CAMERA_CONFIG_AUTO_OFF_LCD:
        map = GetAutoOffStrMap();
        break;
    case CAMERA_CONFIG_UNION_AE:
    case CAMERA_CONFIG_UNION_AE_PRIORITY:
    case CAMERA_CONFIG_UNION_AWB:
    case CAMERA_CONFIG_UNION_AWB_PRIORITY:
        map = GetN3aUnionAeAwbStrMap();
        break;
    case CAMERA_CONFIG_UNION_AE_DELTA:
    case CAMERA_CONFIG_UNION_AWB_DELTA:
        map = GetN3aUnionDeltaStrMap();
        break;
    case CAMERA_CONFIG_SEND_STREAM:
        map = GetSendVideoStreamStrMap();
        break;
    case CAMERA_CONFIG_VIDEO_SHUTTER_TIME:
    case CAMERA_CONFIG_MAX_EXPOSURE_SHR_TIME:
        map = GetVideoShutterTimeStrMap();
        break;
    case CAMERA_CONFIG_NETWORK_MODE:
        map = GetNetworkModeStrMap();
        break;
    case CAMERA_CONFIG_BITRATE_LEVEL:
        map = GetBirateLevelMap();
        break;
    case CAMERA_CONFIG_AE_LOCK:
    case CAMERA_CONFIG_AE_FREEZE:
    case CAMERA_CONFIG_AF_LOCK:
        map = GetAeLockMap();
        break;
    case CAMERA_CONFIG_GAMMA:
    case CAMERA_CONFIG_CLUT:
        map = GetStyleMap();
        break;
    case CAMERA_CONFIG_RECORD_FILE_FORMAT:
        map = GetRecordFileFormat();
        break;
    case CAMERA_CONFIG_HDMI_PREFER_FORMAT:
        map = GetHdmiFmtStrMap();
        break;
    case CAMERA_CONFIG_WB_PRIORITY:
        map = GetWbTintPriorityStrMap();
        break;
    case CAMERA_CONFIG_SHUTTER_UNIT:
        map = GetShutterUnitMap();
        break;
    case CAMERA_CONFIG_DUAL_NATIVE_ISO_MODE:
        map = GetDualNativeIsoModeMap();
        break;
    case CAMERA_CONFIG_LUMA_LEVEL:
        map = GetLumaLevelMap();
        break;
    case CAMERA_CONFIG_EV:
        map = GetEvMap();
        break;
    case CAMERA_CONFIG_MOVIE_VFR:
        map = GetVFRMap();
        break;
    }
    return map;
}

const char* CameraConfig::trValueFromInt(int key, int value)
{
    Map *map = getMapBykey(key);
    if (map == NULL) {
        return kUnknown;
    }
    Map::iterator it = map->find(value);
    if (it == map->end()) {
        return kUnknown;
    }
    return it->second;
}

unsigned int CameraConfig::trKeyFromString(const char *key)
{
    if (key == NULL) {
        return CAMERA_CONFIG_NONE;
    }
    Map* sConfigKey_ = GetConfigKeyMap();
    for (Map::iterator it = sConfigKey_->begin(); it != sConfigKey_->end(); ++it) {
        if (!strcmp(key, it->second)) {
            return it->first;
        }
    }
    return CAMERA_CONFIG_NONE;
}

int CameraConfig::trValueFromString(unsigned int key, const char *value)
{
    Map *map = getMapBykey(key);
    if (map == NULL) {
        return -1;
    }

    for (Map::iterator it = map->begin(); it != map->end(); ++it) {
        if (!strcmp(value, it->second)) {
            return it->first;
        }
    }

    return -1;
}

const char* CameraConfig::trReplace(const char * str, const char * pattern,  const char *target, char * buf, unsigned int len)
{

    char *ptr = NULL;
    char buff2[256];
    unsigned int i = 0;

    if (strstr(str, pattern) == NULL) {
        return str;
    }

    if (str != NULL) {
        strcpy(buff2, str);
    } else {
        printf("str_replace err!\n");
        return NULL;
    }
    while ((ptr = strstr( buff2, pattern)) !=NULL) {
        if (ptr-buff2 != 0) {
            memcpy(&buf[i], buff2, ptr - buff2);
        }
        memcpy(&buf[i + ptr - buff2], target, strlen(target));
        i += ptr - buff2 + strlen(target);
        if (i > len) {
            return NULL;
        }
        strcpy(buff2, ptr + strlen(pattern));
    }
    strcat(buf,buff2);
    return (char* )buf;
}

// C api begin
extern "C" {
const char *config_tr_value_from_key(int key, int value)
{
    return CameraConfig::trValueFromInt(key, value);
}
int config_tr_value_from_index(int key, int index)
{
    return CameraConfig::trValueFromIndex(key, index);
}
}
// C api end
