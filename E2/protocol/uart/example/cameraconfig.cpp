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
static Map* GetVfrControlStrMap(void) {static Map map; return &map;}
static Map* GetOnOffStrMap(void) {static Map map; return &map;}
static Map* GetEnableDisableMap(void) {static Map map; return &map;}
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
static Map* GetMetaCameraIDStrMap(void) {static Map map; return &map;}
static Map* GetCvbsSystemStrMap(void) {static Map map; return &map;}
static Map* GetShutterSpeedStrMap(void) {static Map map; return &map;}
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
static Map* GetOoftMap(void) {static Map map; return &map;}
static Map* GetExtendedVideoShutterTimeStrMap(void) {static Map map; return &map;}
static Map* GetNativeIsoMap(void) {static Map map; return &map;}
static Map* GetAeSpeedMap(void) {static Map map; return &map;}

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
static Map *GetAudioInputMap()
{
    static Map map;
    if (map.empty()) {
        map.insert(Entry(ACODEC_IN_MODE_OFF, QT_TRANSLATE_NOOP("MenuItem", "Off")));
        map.insert(Entry(ACODEC_IN_MODE_MIC_IN, QT_TRANSLATE_NOOP("MenuItem", "Microphone")));
        map.insert(Entry(ACODEC_IN_MODE_XLR, QT_TRANSLATE_NOOP("MenuItem", "XLR")));
        map.insert(Entry(ACODEC_IN_MODE_LINE_IN, QT_TRANSLATE_NOOP("MenuItem", "Line In")));
        map.insert(Entry(ACODEC_IN_MODE_MIC_LEFT_AND_XLR_RIGHT, QT_TRANSLATE_NOOP("MenuItem", "Mic Left + XLR Right")));
        map.insert(Entry(ACODEC_IN_MODE_MIC_RIGHT_AND_XLR_LEFT, QT_TRANSLATE_NOOP("MenuItem", "Mic Right + XLR Left")));
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

static Map *GetDesqueezeMap()
{
    static Map map;
    if (map.empty()) {
        map.insert(Entry(IMVT_DESQUEEZE_FACTOR_1X, QT_TRANSLATE_NOOP("MenuItem", "H 1x")));
        map.insert(Entry(IMVT_DESQUEEZE_FACTOR_1P33X, QT_TRANSLATE_NOOP("MenuItem", "H 1.33x")));
        map.insert(Entry(IMVT_DESQUEEZE_FACTOR_1P5X, QT_TRANSLATE_NOOP("MenuItem", "H 1.5x")));
        map.insert(Entry(IMVT_DESQUEEZE_FACTOR_1P8X, QT_TRANSLATE_NOOP("MenuItem", "H 1.8x")));
        map.insert(Entry(IMVT_DESQUEEZE_FACTOR_2X, QT_TRANSLATE_NOOP("MenuItem", "H 2x")));
        map.insert(Entry(IMVT_DESQUEEZE_VERTICAL_FACTOR_1X, QT_TRANSLATE_NOOP("MenuItem", "V 1x")));
        map.insert(Entry(IMVT_DESQUEEZE_VERTICAL_FACTOR_1P33X, QT_TRANSLATE_NOOP("MenuItem", "V 1.33x")));
        map.insert(Entry(IMVT_DESQUEEZE_VERTICAL_FACTOR_1P5X, QT_TRANSLATE_NOOP("MenuItem", "V 1.5x")));
        map.insert(Entry(IMVT_DESQUEEZE_VERTICAL_FACTOR_1P8X, QT_TRANSLATE_NOOP("MenuItem", "V 1.8x")));
        map.insert(Entry(IMVT_DESQUEEZE_VERTICAL_FACTOR_2X, QT_TRANSLATE_NOOP("MenuItem", "V 2x")));
    }
    return &map;
}


static Map *GetVFRMap()
{
    static Map map;
    if (map.empty()) {
        map.insert(Entry(0, QT_TRANSLATE_NOOP("MenuItem", "Off")));
        for (int i = 1; i <= 240; i++) {
            char * leaked_buf = (char*)malloc(4);
            snprintf(leaked_buf, 4, "%d", i);
            map.insert(Entry(i, QT_TRANSLATE_NOOP("MenuItem", leaked_buf)));
        }
    }
    return &map;
}

#ifdef CONFIG_IMVT_ASSITOOL
static Map *GetAssitoolDisplayMap()
{
    static Map map;
    if (map.empty()) {
        map.insert(Entry(ASSITOOL_DISPLAY_ON_NULL, QT_TRANSLATE_NOOP("MenuItem", "Off")));
        map.insert(Entry(ASSITOOL_DISPLAY_ON_DEVICE, QT_TRANSLATE_NOOP("MenuItem", "On")));
    }

   return &map;
}

static Map *GetAssitoolColorMap()
{
    static Map map;
    if (map.empty()) {
        map.insert(Entry(IMVT_ASSITOOL_COLOR_RED, QT_TRANSLATE_NOOP("MenuItem", "Red")));
        map.insert(Entry(IMVT_ASSITOOL_COLOR_GREEN, QT_TRANSLATE_NOOP("MenuItem", "Green")));
        map.insert(Entry(IMVT_ASSITOOL_COLOR_BLUE, QT_TRANSLATE_NOOP("MenuItem", "Blue")));
        map.insert(Entry(IMVT_ASSITOOL_COLOR_ORANGE, QT_TRANSLATE_NOOP("MenuItem", "Orange")));
        map.insert(Entry(IMVT_ASSITOOL_COLOR_WHITE, QT_TRANSLATE_NOOP("MenuItem", "White")));
    }

   return &map;
}

static Map *GetAssitoolExposureMap()
{
    static Map map;
    if (map.empty()) {
        map.insert(Entry(ASSITOOL_EXPOSURE_NONE, QT_TRANSLATE_NOOP("MenuItem", "Disable")));
        map.insert(Entry(ASSITOOL_EXPOSURE_ZEBRA, QT_TRANSLATE_NOOP("MenuItem", "Zebra")));
        map.insert(Entry(ASSITOOL_EXPOSURE_FC, QT_TRANSLATE_NOOP("MenuItem", "False Color")));
        map.insert(Entry(ASSITOOL_EXPOSURE_MONO, QT_TRANSLATE_NOOP("MenuItem", "Monochrome")));
    }

   return &map;
}

static Map *GetAssitoolScopeMap()
{
    static Map map;
    if (map.empty()) {
        map.insert(Entry(ASSITOOL_SCOPE_WAVEFORM, QT_TRANSLATE_NOOP("MenuItem", "Waveform")));
        map.insert(Entry(ASSITOOL_SCOPE_VECTORSCOPE, QT_TRANSLATE_NOOP("MenuItem", "Vectorscope")));
        map.insert(Entry(ASSITOOL_SCOPE_PARADE, QT_TRANSLATE_NOOP("MenuItem", "Parade")));
        map.insert(Entry(ASSITOOL_SCOPE_NONE, QT_TRANSLATE_NOOP("MenuItem", "Disable")));
    }

   return &map;
}

static Map *GetAssitoolFrameLineMap()
{
    static Map map;
    if (map.empty()) {
        map.insert(Entry(IMVT_ASSITOOL_FRAME_LINE_TYPE_NONE, QT_TRANSLATE_NOOP("MenuItem", "None")));
        map.insert(Entry(IMVT_ASSITOOL_FRAME_LINE_TYPE_2P4_1, QT_TRANSLATE_NOOP("MenuItem", "2.4:1")));
        map.insert(Entry(IMVT_ASSITOOL_FRAME_LINE_TYPE_2P35_1, QT_TRANSLATE_NOOP("MenuItem", "2.35:1")));
        map.insert(Entry(IMVT_ASSITOOL_FRAME_LINE_TYPE_1P9_1, QT_TRANSLATE_NOOP("MenuItem", "1.9:1")));
        map.insert(Entry(IMVT_ASSITOOL_FRAME_LINE_TYPE_1P85_1, QT_TRANSLATE_NOOP("MenuItem", "1.85:1")));
        map.insert(Entry(IMVT_ASSITOOL_FRAME_LINE_TYPE_1P33_1, QT_TRANSLATE_NOOP("MenuItem", "4:3")));
    }

   return &map;
}

#endif

static Map * GetMonitorLutMap()
{
    static Map map;
    if (map.empty()) {
        map.insert(Entry(IMVT_BUILDIN_LUT_NONE, QT_TRANSLATE_NOOP("MenuItem", "None")));
        map.insert(Entry(IMVT_BUILDIN_LUT_REC709, QT_TRANSLATE_NOOP("MenuItem", "Rec709")));
        map.insert(Entry(IMVT_BUILDIN_LUT_V709, QT_TRANSLATE_NOOP("MenuItem", "V709")));
        map.insert(Entry(IMVT_BUILDIN_LUT_LC709A, QT_TRANSLATE_NOOP("MenuItem", "LC709A")));
        map.insert(Entry(IMVT_BUILDIN_LUT_ZRGBAR, QT_TRANSLATE_NOOP("MenuItem", "zRGB_AR")));
        map.insert(Entry(IMVT_BUILDIN_LUT_ZRGBAX2, QT_TRANSLATE_NOOP("MenuItem", "zRGB_AX2")));
        map.insert(Entry(IMVT_BUILDIN_LUT_SONYCINE_709, QT_TRANSLATE_NOOP("MenuItem", "sCINE_709")));
        map.insert(Entry(IMVT_BUILDIN_LUT_SRGB, QT_TRANSLATE_NOOP("MenuItem", "sRGB")));
        map.insert(Entry(IMVT_BUILDIN_LUT_REC2020, QT_TRANSLATE_NOOP("MenuItem", "Rec2020")));
        map.insert(Entry(IMVT_BUILDIN_LUT_HLG, QT_TRANSLATE_NOOP("MenuItem", "HLG")));
        map.insert(Entry(IMVT_BUILDIN_LUT_P3D60, QT_TRANSLATE_NOOP("MenuItem", "P3D60")));
        map.insert(Entry(IMVT_BUILDIN_LUT_P3D65, QT_TRANSLATE_NOOP("MenuItem", "P3D65")));
        map.insert(Entry(IMVT_BUILDIN_LUT_P3DCI, QT_TRANSLATE_NOOP("MenuItem", "P3DCI")));
        map.insert(Entry(IMVT_BUILDIN_LUT_REC709LIN, QT_TRANSLATE_NOOP("MenuItem", "Rec709LIN")));
        map.insert(Entry(IMVT_BUILDIN_LUT_ACESAP0, QT_TRANSLATE_NOOP("MenuItem", "ACESAP0")));
        map.insert(Entry(IMVT_BUILDIN_LUT_XYZ, QT_TRANSLATE_NOOP("MenuItem", "XYZ")));
        map.insert(Entry(IMVT_BUILDIN_LUT_F709, QT_TRANSLATE_NOOP("MenuItem", "F709")));
        map.insert(Entry(IMVT_BUILDIN_LUT_USER_DEFINE, QT_TRANSLATE_NOOP("MenuItem", "User")));
    }
    return &map;
}

static Map *GetReocrdFramerateMap()
{
    static Map map;
    if (map.empty()) {
        map.insert(Entry(IMVT_OFF, QT_TRANSLATE_NOOP("MenuItem", "Default")));
        map.insert(Entry(IMVT_ON, QT_TRANSLATE_NOOP("MenuItem", "VFR")));
    }

   return &map;
}

static Map *GetAfAreaMap()
{
    static Map map;
    if (map.empty()) {
        map.insert(Entry(N3A_AF_ROI_SMALL, QT_TRANSLATE_NOOP("MenuItem", "Small")));
        map.insert(Entry(N3A_AF_ROI_MIDDLE, QT_TRANSLATE_NOOP("MenuItem", "Medium")));
        map.insert(Entry(N3A_AF_ROI_LARGE, QT_TRANSLATE_NOOP("MenuItem", "Large")));
    }

    return &map;
}

static Map *GetFanModeMap()
{
    static Map map;
    if (map.empty()) {
        map.insert(Entry(IMVT_FAN_MODE_AUTO,   QT_TRANSLATE_NOOP("MenuItem", "Auto")));
        map.insert(Entry(IMVT_FAN_MODE_MANUAL, QT_TRANSLATE_NOOP("MenuItem", "manual")));
    }

    return &map;
}

static Map *GetWBCalibMap()
{
    static Map map;
    if (map.empty()) {
        map.insert(Entry(N3A_WB_CALIB_DISABLE, QT_TRANSLATE_NOOP("MenuItem", "Disable")));
        map.insert(Entry(N3A_WB_CALIB_ENABLE, QT_TRANSLATE_NOOP("MenuItem", "Enable")));
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
    sConfigKey_->insert(Entry(CAMERA_CONFIG_BATTERY_VOLTAGE, "battery_voltage"));
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
    sConfigKey_->insert(Entry(CAMERA_CONFIG_MAX_ISO_LIMIT,    "max_iso"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_MIN_ISO_LIMIT, "min_iso"));
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
    sConfigKey_->insert(Entry(CAMERA_CONFIG_X_MWB_RGAIN, "mwb_r"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_X_MWB_GGAIN, "mwb_g"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_X_MWB_BGAIN, "mwb_b"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_META_CAMERA_ID, "camera_id"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_META_REELNAME, "reelname"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_FILELIST_ENABLE, "filelist"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_PRERECORD_ENABLE, "preroll"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_ND_STOP, "eND"));

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
    //sConfigKey_->insert(Entry(CAMERA_CONFIG_DUAL_NATIVE_ISO_MODE, "dual_iso"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_ISO_OPTION_CTRL, "iso_ctrl"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_AE_SPEED_RATIO, "ae_speed"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_AE_FREEZE, "ae_freeze"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_AF_LOCK, "af_lock"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_GAMMA, "gamma"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_CLUT, "clut"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_3D_LUT_TYPE, "3d_lut"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_LENS_ZOOM_POSITION, "lens_zoom_pos"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_LUMA_LEVEL, "luma_level"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_EV, "ev_choice"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_MOVIE_VFR, "movvfr"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_VFR_CONTROL_TYPE, "vfr_ctrl"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_RECORD_FRAMERTAE, "rec_fps"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_LIVE_CAF, "live_caf"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_WB_FREEZE, "wb_freeze"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_WB_MWB_AUTO_DETECT, "mwb_detect"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_DESQUEEZE_FACTOR, "desqueeze"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_OOTF, "oetf"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_EXTEND_VIDEO_SHUTTER_TIME, "extended_shutter_time"));

    /// Local config key
    sConfigKey_->insert(Entry(CAMERA_CONFIG_AUTO_OFF, "auto_off"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_NP_BATTERY_THRESHOLD, "np_bat_th"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_VMOUNT_BATTERY_THRESHOLD, "vmount_bat_th"));
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
    sConfigKey_->insert(Entry(CAMERA_CONFIG_TIME_CODE_DROP_FRAME, "tc_drop_frame"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_ACODEC_INPUT_CHANNEL, "audio_channel"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_LVDS_CROP_SETTING, "lvds_crop"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_UART_ROLE, "uart_role"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_RECORD_PROXY_FILE_ONOFF, "rec_proxy_file"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_ASSITOOL_DISPLAY, "assitool_display"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_ASSITOOL_PEAK_ONOFF, "assitool_peak_onoff"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_ASSITOOL_PEAK_COLOR, "assitool_peak_color"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_ASSITOOL_PEAK_BW_BACKGROUND, "assitool_peak_mono"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_ASSITOOL_PEAK_THD, "assitool_peak_thd"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_ASSITOOL_EXPOSURE_TOOL, "assitool_exposure"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_ASSITOOL_SCOPE_TOOL, "assitool_scope"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_ASSITOOL_SCOPE_OPACITY, "assitool_scope_opacity"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_ASSITOOL_ZEBRA_TH1, "assitool_zera_th1"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_ASSITOOL_ZEBRA_TH2, "assitool_zera_th2"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_LANC, "lanc"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_RESTORE_LENS_POSITION, "restore_lens_pos"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_AUDIO_PHANTOM_POWER, "audio_phantom_power"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_AUDIO_INPUT_LEVEL_DISPLAY, "audio_level_display"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_MF_ASSIST_PREVIEW, "mf_mag"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_MF_ASSIST_RECORDING, "mf_recording"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_ASSITOOL_OVERLAY_FRAME_LINE, "assitool_frame_line"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_ASSITOOL_OVERLAY_CENTER_MARK, "assitool_center_mark"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_TIME_CODE_SOURCE, "tc_source"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_ASSITOOL_FRAME_LINE_COLOR, "assitool_frame_line_color"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_ASSITOOL_CENTER_MARK_COLOR, "assitool_center_mark_color"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_AUDIO_SUPPORT_AGC, "ain_gain_type"));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_HDMI_USE_EDID, "use_edid"));


    // color lut
    sConfigKey_->insert(Entry(CAMERA_CONFIG_COLOR_LUT, "monitor_lut"));
    //
    sConfigKey_->insert(Entry(CAMERA_CONFIG_HDMI_OSD_ONOFF, PREF_HDMI_OSD));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_ROTATION_MOVIE_SQUARE_FORMAT, PREF_KEY_ROTATION_MOVIE_SQUARE_FORMAT));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_PREF_WIFI_MODE, PREF_KEY_WIFI_MODE));
    sConfigKey_->insert(Entry(CAMERA_CONFIG_WIFI_CHANNEL, "wifi_channel"));

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
    sMovResolutionStr_->insert(Entry(IMVT_MOVIE_RES_4KP_LN,  QT_TRANSLATE_NOOP("MenuItem","4K (Low Noise)")));
    sMovResolutionStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P_LN,  QT_TRANSLATE_NOOP("MenuItem","C4K (Low Noise)")));
    sMovResolutionStr_->insert(Entry(IMVT_MOVIE_RES_6K_16_9_P,   QT_TRANSLATE_NOOP("MenuItem","6K")));
    sMovResolutionStr_->insert(Entry(IMVT_MOVIE_RES_6K_DCI_P,    QT_TRANSLATE_NOOP("MenuItem","C6K")));
    sMovResolutionStr_->insert(Entry(IMVT_MOVIE_RES_6K_2P4_1_P,  QT_TRANSLATE_NOOP("MenuItem","6K 2.4:1")));
    sMovResolutionStr_->insert(Entry(IMVT_MOVIE_RES_6244_4168_P,  QT_TRANSLATE_NOOP("MenuItem","6244x4168")));
    sMovResolutionStr_->insert(Entry(IMVT_MOVIE_RES_5K_4_3_P,  QT_TRANSLATE_NOOP("MenuItem","5K 4:3")));
    sMovResolutionStr_->insert(Entry(IMVT_MOVIE_RES_5K_6_5_P,  QT_TRANSLATE_NOOP("MenuItem","5K 6:5")));
    sMovResolutionStr_->insert(Entry(IMVT_MOVIE_RES_SUPER_16_P,  QT_TRANSLATE_NOOP("MenuItem","S16")));
    sMovResolutionStr_->insert(Entry(IMVT_MOVIE_RES_6064_4040_P,  QT_TRANSLATE_NOOP("MenuItem","6064x4040")));
    sMovResolutionStr_->insert(Entry(IMVT_MOVIE_RES_4K_UHD_2P4_1_P,   QT_TRANSLATE_NOOP("MenuItem","4K 2.4:1")));
    sMovResolutionStr_->insert(Entry(IMVT_MOVIE_RES_4K_DCI_2P4_1_P,  QT_TRANSLATE_NOOP("MenuItem","C4K 2.4:1")));
    sMovResolutionStr_->insert(Entry(IMVT_MOVIE_RES_8K_P,            QT_TRANSLATE_NOOP("MenuItem","8K")));
    sMovResolutionStr_->insert(Entry(IMVT_MOVIE_RES_8K_DCI_2P4_1_P,  QT_TRANSLATE_NOOP("MenuItem","C8K 2.4:1")));
    sMovResolutionStr_->insert(Entry(IMVT_MOVIE_RES_3696_2772P_LN,  QT_TRANSLATE_NOOP("MenuItem","3696x2772 (Low Noise)")));
    sMovResolutionStr_->insert(Entry(IMVT_MOVIE_RES_2488_1400P,     QT_TRANSLATE_NOOP("MenuItem","2488x1400")));
    sMovResolutionStr_->insert(Entry(IMVT_MOVIE_RES_S16_16_9_P,   QT_TRANSLATE_NOOP("MenuItem","S16 16:9")));

    //fps
    Map* sMovFpsStr_ = GetMovFpsStrMap();
    sMovFpsStr_->insert(Entry(IMVT_MOVIE_FPS_2397,  QT_TRANSLATE_NOOP("MenuItem","23.98")));
    sMovFpsStr_->insert(Entry(IMVT_MOVIE_FPS_24,  QT_TRANSLATE_NOOP("MenuItem","24")));
    sMovFpsStr_->insert(Entry(IMVT_MOVIE_FPS_25,  QT_TRANSLATE_NOOP("MenuItem","25")));
    //sMovFpsStr_->insert(Entry(IMVT_MOVIE_FPS_2997,  QT_TRANSLATE_NOOP("MenuItem","29.97")));
    sMovFpsStr_->insert(Entry(IMVT_MOVIE_FPS_30,  QT_TRANSLATE_NOOP("MenuItem","29.97")));
    sMovFpsStr_->insert(Entry(IMVT_MOVIE_FPS_50,  QT_TRANSLATE_NOOP("MenuItem","50")));
    //sMovFpsStr_->insert(Entry(IMVT_MOVIE_FPS_5994,  QT_TRANSLATE_NOOP("MenuItem","59.94")));
    sMovFpsStr_->insert(Entry(IMVT_MOVIE_FPS_60,  QT_TRANSLATE_NOOP("MenuItem","59.94")));

    //  NTSC
    Map* sMovFmtStr_ = GetMovFmtStrMap();
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4KP30, "4KP29.97"));
    //sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4KP2997, "4KP29.97"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4KP24, "4KP24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_1080P60, "1080P59.94"));
    //sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_1080P5994, "1080P59.94"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_1080P30, "1080P29.97"));
    //sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_1080P2997, "1080P29.97"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_1080P24, "1080P24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_1440_1080P30, "1080P29.97 (4:3)"));
    //sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_1440_1080P2997, "1080P29.97 (4:3)"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_1440_1080P24, "1080P24 (4:3)"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_720P60, "720P59.94"));
    //sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_720P5994, "720P59.94"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_720P30, "720P29.97"));
    //sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_720P2997, "720P29.97"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_720P24, "720P24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2704_1520P30, "2.7KP29.97"));
    //sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2704_1520P2997, "2.7KP29.97"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2704_1520P24, "2.7KP24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_1920_1440P30, "1440P29.97"));
    //sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_1920_1440P2997, "1440P29.97"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_1920_1440P24, "1440P24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2048_2048P30, "2048P29.97 (1:1)"));
    //sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2048_2048P2997, "2048P29.97 (1:1)"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2048_2048P24, "2048P24 (1:1)"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2688_2688P30, "2688P29.97 (1:1)"));
    //sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2688_2688P2997, "2688P29.97 (1:1)"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2688_2688P24, "2688P24 (1:1)"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2880_2880P30, "2880P29.97 (1:1)"));
    //sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2880_2880P2997, "2880P29.97 (1:1)"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2880_2880P24, "2880P24 (1:1)"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P30, "C4KP29.97"));
    //sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P2997, "C4KP29.97"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P24, "C4KP24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3680_2428P30, "3680x2428P29.97"));
    //sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3680_2428P2997, "3680x2428P29.97"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3680_2428P24, "3680x2428P24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2048P30, "4096x2048P29.97"));
    //sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2048P2997, "4096x2048P29.97"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2048P24, "4096x2048P24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2120_1248P60, "2120x1248P59.94"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3296_2472P30, "3296x2472P29.97"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3296_2472P25, "3296x2472P25"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3296_2472P24, "3296x2472P24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4056_2304P30, "4056x2304P29.97"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4056_2304P24, "4056x2304P24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4KP30_WDR, "4KP29.97 WDR"));
    //sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4KP2997_WDR, "4KP29.97 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4KP24_WDR, "4KP24 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3840_1920P30, "3840x1920P29.97"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3840_1920P24, "3840x1920P24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2700_1800P60, "2700x1800P59.94"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4KP60, "4KP59.94"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3376_2768P30, "3376x2768P29.97"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3376_2768P24, "3376x2768P24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2880_1524P60, "2880x1524P59.94"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4KP2397, "4KP23.98"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4KP60_WDR, "4KP59.94 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4KP2397_WDR, "4KP23.98 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P60, "C4KP59.94"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P2397, "C4KP23.98"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P60_WDR, "C4KP59.94 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P30_WDR, "C4KP29.97 WDR"));
    //sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P2997_WDR, "C4KP29.97 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P24_WDR, "C4KP24 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P2397_WDR, "C4KP23.98 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3696_2772P60, "3696x2772P59.94"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3696_2772P30, "3696x2772P29.97"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3696_2772P24, "3696x2772P24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3696_2772P2397, "3696x2772P23.98"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3696_2772P30_WDR, "3696x2772P29.97 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3696_2772P24_WDR, "3696x2772P24 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3696_2772P2397_WDR, "3696x2772P23.98 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_1080P2397, "1080P23.98"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_720P2397,  "720P23.98"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3312_2760P60, "3312x2760P59.94"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3312_2760P30, "3312x2760P29.97"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3312_2760P24, "3312x2760P24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3312_2760P2397, "3312x2760P23.98"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3312_2760P30_WDR, "3312x2760P29.97 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3312_2760P24_WDR, "3312x2760P24 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3312_2760P2397_WDR, "3312x2760P23.98 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_1080P30_WDR, "1080P29.97 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_1080P24_WDR, "1080P24 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_1080P2397_WDR, "1080P23.98 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_1080P60_WDR,  "1080P59.94 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2880_2880P2397, "2880P23.98 (1:1)"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4KP30_LN, "4KP29.97 (Low Noise)"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4KP2397_LN, "4KP23.98 (Low Noise)"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4KP24_LN, "4KP24 (Low Noise)"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P30_LN, "C4KP29.97 (Low Noise)"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P2397_LN, "C4KP23.98 (Low Noise)"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P24_LN, "C4KP24 (Low Noise)"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_6K_16_9_P30,   "6KP29.97"));
    //sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_6K_16_9_P2997, "6KP29.97"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_6K_16_9_P24,   "6KP24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_6K_16_9_P2398, "6KP23.98"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_6K_DCI_P30,     "C6KP29.97"));
    //sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_6K_DCI_P2997,   "C6KP29.97"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_6K_DCI_P24,     "C6KP24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_6K_DCI_P2398,   "C6KP23.98"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_6K_2P4_1_P30,   "6K 2.4:1P29.97"));
    //sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_6K_2P4_1_P2997, "6K 2.4:1P29.97"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_6K_2P4_1_P24,   "6K 2.4:1P24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_6K_2P4_1_P2398, "6K 2.4:1P23.98"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_6K_2P4_1_P60,   "6K 2.4:1P59.94"));

    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_6244_4168_P24,   "6244x4168P24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_6244_4168_P2398, "6244x4168P23.98"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_6244_4168_P30,   "6244x4168P29.97"));
    //sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_6244_4168_P2997, "6244x4168P29.97"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_6244_4168_P25,   "6244x4168P25"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_5K_4_3_P24,   "5K 4:3P24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_5K_4_3_P2398, "5K 4:3P23.98"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_5K_4_3_P30,   "5K 4:3P29.97"));
    //sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_5K_4_3_P2997, "5K 4:3P29.97"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_5K_4_3_P25,   "5K 4:3P25"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_5K_6_5_P24,   "5K 6:5P24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_5K_6_5_P2398, "5K 6:5P23.98"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_5K_6_5_P30,   "5K 6:5P29.97"));
    //sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_5K_6_5_P2997, "5K 6:5P29.97"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_5K_6_5_P25,   "5K 6:5P25"));

    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_SUPER_16_P24,   "S16 24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_SUPER_16_P2398, "S16 23.98"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_SUPER_16_P30,   "S16 29.97"));
    //sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_SUPER_16_P2997, "S16 29.97"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_SUPER_16_P25,   "S16 25"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_SUPER_16_P60,   "S16 59.94"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_SUPER_16_P50,   "S16 50"));

    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_6064_4040_P24,   "6064x4040P24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_6064_4040_P2398, "6064x4040P23.98"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_6064_4040_P30,   "6064x4040P29.97"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_6064_4040_P25,   "6064x4040P25"));

    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4K_UHD_2P4_1_P2398,  "4K 2.4:1P23.98"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4K_UHD_2P4_1_P24,    "4K 2.4:1P24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4K_UHD_2P4_1_P25,    "4K 2.4:1P25"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4K_UHD_2P4_1_P30,    "4K 2.4:1P29.97"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4K_UHD_2P4_1_P50,    "4K 2.4:1P50"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4K_UHD_2P4_1_P60,    "4K 2.4:1P59.94"));;

    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4K_DCI_2P4_1_P2398,  "C4K 2.4:1P23.98"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4K_DCI_2P4_1_P24,    "C4K 2.4:1P24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4K_DCI_2P4_1_P25,    "C4K 2.4:1P25"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4K_DCI_2P4_1_P30,    "C4K 2.4:1P29.97"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4K_DCI_2P4_1_P50,    "C4K 2.4:1P50"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4K_DCI_2P4_1_P60,    "C4K 2.4:1P59.94"));

    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_8K_P2398,            "8KP23.98"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_8K_P24,              "8KP24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_8K_P25,              "8KP25"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_8K_P30,              "8KP29.97"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_8K_DCI_2P4_1_P2398,  "C8K 2.4:1P23.98"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_8K_DCI_2P4_1_P24,    "C8K 2.4:1P24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_8K_DCI_2P4_1_P25,    "C8K 2.4:1P25"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_8K_DCI_2P4_1_P30,    "C8K 2.4:1P29.97"));

    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3696_2772P30_LN, "3696x2772P29.97 (Low Noise)"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3696_2772P25_LN, "3696x2772P25 (Low Noise)"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3696_2772P24_LN, "3696x2772P24 (Low Noise)"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3696_2772P2397_LN, "3696x2772P23.98 (Low Noise)"));

    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_S16_16_9_P24,   "S16 16:9 24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_S16_16_9_P2398, "S16 16:9 23.98"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_S16_16_9_P30,   "S16 16:9 29.97"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_S16_16_9_P25,   "S16 16:9 25"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_S16_16_9_P60,   "S16 16:9 59.94"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_S16_16_9_P50,   "S16 16:9 50"));

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
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3376_2768P25, "3376x2768P25"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2880_1524P50, "2880x1524P50"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4KP50_WDR, "4KP50 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P50, "C4KP50"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P50_WDR, "C4KP50 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P25_WDR, "C4KP25 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3696_2772P50, "3696x2772P50"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3696_2772P25, "3696x2772P25"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3696_2772P25_WDR, "3696x2772P25 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3312_2760P50, "3312x2760P50"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3312_2760P25, "3312x2760P25"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_3312_2760P25_WDR, "3312x2760P25 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_1080P25_WDR, "1080P25 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_1080P50_WDR, "1080P50 WDR"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4KP25_LN, "4KP25 (Low Noise)"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_4096_2160P25_LN, "C4KP25 (Low Noise)"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_6K_16_9_P25,    "6KP25"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_6K_DCI_P25,     "C6KP25"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_6K_2P4_1_P25,   "6K 2.4:1P25"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_6K_2P4_1_P50,   "6K 2.4:1P50"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2488_1400P30,   "2488x1400P29.97"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2488_1400P25,   "2488x1400P25"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2488_1400P24,   "2488x1400P24"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2488_1400P2398, "2488x1400P23.98"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2488_1400P60,   "2488x1400P59.94"));
    sMovFmtStr_->insert(Entry(IMVT_MOVIE_RES_2488_1400P50,   "2488x1400P50"));

    Map* sPhotoSizeStr_ = GetPhotoSizeStrMap();
    sPhotoSizeStr_->insert(Entry(IMVT_PHOTO_RES_16M, "16M"));
    sPhotoSizeStr_->insert(Entry(IMVT_PHOTO_RES_12M, "12M"));
    sPhotoSizeStr_->insert(Entry(IMVT_PHOTO_RES_8M, "8M"));
    sPhotoSizeStr_->insert(Entry(IMVT_PHOTO_RES_6M, "6M"));
    sPhotoSizeStr_->insert(Entry(IMVT_PHOTO_RES_4K, "4K"));


    Map* sVfrControlTypeStr_ = GetVfrControlStrMap();
    sVfrControlTypeStr_->insert(Entry(IMVT_VFR_CONTROL_TYPE_COARSE, QT_TRANSLATE_NOOP("MenuItem", "Coarse")));
    sVfrControlTypeStr_->insert(Entry(IMVT_VFR_CONTROL_TYPE_FINE, QT_TRANSLATE_NOOP("MenuItem", "Fine")));


    // XXX: Caution !!!!!!!!!!! read above comment first
    Map* sOnOffStr_ = GetOnOffStrMap();
    sOnOffStr_->insert(Entry(IMVT_OFF, QT_TRANSLATE_NOOP("MenuItem", "Off")));
    sOnOffStr_->insert(Entry(IMVT_ON, QT_TRANSLATE_NOOP("MenuItem", "On")));

    Map* sEnableDisableStr_ = GetEnableDisableMap();
    sEnableDisableStr_->insert(Entry(IMVT_DISABLE, QT_TRANSLATE_NOOP("MenuItem", "Disable")));
    sEnableDisableStr_->insert(Entry(IMVT_ENABLE, QT_TRANSLATE_NOOP("MenuItem", "Enable")));

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
    sWbStr_->insert(Entry(IMVT_WB_LIGHT_SHOW,      QT_TRANSLATE_NOOP("MenuItem", "LightShow")));
    sWbStr_->insert(Entry(IMVT_WB_MANUAL,          QT_TRANSLATE_NOOP("MenuItem", "Manual")));
    sWbStr_->insert(Entry(IMVT_WB_EXPERT_MANUAL,   QT_TRANSLATE_NOOP("MenuItem", "Expert")));

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
    sIsoStr_->insert(Entry(IMVT_ISO_32000, "32000"));
    sIsoStr_->insert(Entry(IMVT_ISO_40000, "40000"));
    sIsoStr_->insert(Entry(IMVT_ISO_51200, "51200"));
    sIsoStr_->insert(Entry(IMVT_ISO_64000, "64000"));
    sIsoStr_->insert(Entry(IMVT_ISO_80000, "80000"));
    sIsoStr_->insert(Entry(IMVT_ISO_102400,"102400"));
    sIsoStr_->insert(Entry(IMVT_ISO_128000,"128000"));
    sIsoStr_->insert(Entry(IMVT_ISO_160000,"160000"));
    sIsoStr_->insert(Entry(IMVT_ISO_204800,"204800"));

    // XXX: Caution !!!!!!!!!!! read above comment first
    Map* sRotationStr_ = GetRotationStrMap();
    sRotationStr_->insert(Entry(ROTATE_NONE, QT_TRANSLATE_NOOP("MenuItem", "Normal")));
    sRotationStr_->insert(Entry(ROTATE_90, QT_TRANSLATE_NOOP("MenuItem", "H Flip")));
    sRotationStr_->insert(Entry(ROTATE_270, QT_TRANSLATE_NOOP("MenuItem", "V Flip")));
    sRotationStr_->insert(Entry(ROTATE_180, QT_TRANSLATE_NOOP("MenuItem", "Upside Down")));

    // XXX: Caution !!!!!!!!!!! read above comment first
    Map* sNoiseReductionStr_ = GetNoiseReductionStrMap();
#if 0
    sNoiseReductionStr_->insert(Entry(IMVT_NOISE_REDUCTION_WEAK,     QT_TRANSLATE_NOOP("MenuItem", "Weak")));
    sNoiseReductionStr_->insert(Entry(IMVT_NOISE_REDUCTION_NORMAL,   QT_TRANSLATE_NOOP("MenuItem", "Medium")));
    sNoiseReductionStr_->insert(Entry(IMVT_NOISE_REDUCTION_STRONG,   QT_TRANSLATE_NOOP("MenuItem", "Strong")));
#else
    sNoiseReductionStr_->insert(Entry(IMVT_CAM_IQ_DENOISE_OFF,  QT_TRANSLATE_NOOP("MenuItem", "Off")));
    sNoiseReductionStr_->insert(Entry(IMVT_CAM_IQ_DENOISE_ON,   QT_TRANSLATE_NOOP("MenuItem", "On")));
#endif

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
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_UNKNOWN, "-"  ));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_0P7,     "0.7"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_0P8,     "0.8"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_0P9,     "0.9"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_1P0,     "1"  ));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_1P1,     "1.1"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_1P18,    "1.18"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_1P2,     "1.2"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_1P4,     "1.4"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_1P6,     "1.6"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_1P7,     "1.7"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_1P8,     "1.8"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_2P0,     "2"  ));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_2P2,     "2.2"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_2P4,     "2.4"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_2P5,     "2.5"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_2P8,     "2.8"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_3P2,     "3.2"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_3P3,     "3.3"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_3P5,     "3.5"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_4P0,     "4"  ));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_4P5,     "4.5"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_4P8,     "4.8"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_5P0,     "5"  ));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_5P6,     "5.6"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_6P3,     "6.3"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_6P7,     "6.7"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_7P1,     "7.1"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_8P0,     "8"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_9P0,     "9"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_9P5,     "9.5"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_10P0,    "10"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_11P0,    "11"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_13P0,    "13"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_13P4,    "13.4"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_14P0,    "14"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_16P0,    "16"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_18P0,    "18"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_19P0,    "19"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_20P0,    "20"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_22P0,    "22"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_25P0,    "25"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_27P0,    "27"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_29P0,    "29"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_32P0,    "32"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_36P0,    "36"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_38P0,    "38"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_40P0,    "40"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_45P0,    "45"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_51P0,    "51"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_54P0,    "54"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_57P0,    "57"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_64P0,    "64"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_72P0,    "72"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_76P0,    "76"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_80P0,    "80"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_90P0,    "90"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_107P0,   "107"));
    sFnumberStr_->insert(Entry(IMVT_FNUMBER_128P0,   "128"));

    // XXX: Caution !!!!!!!!!!! read above comment first
    Map* sAfModeStr_ = GetAfModeStrMap();
    sAfModeStr_->insert(Entry(IMVT_AF_SPOT, QT_TRANSLATE_NOOP("MenuItem", "Spot")));
    sAfModeStr_->insert(Entry(IMVT_AF_SELECTION, QT_TRANSLATE_NOOP("MenuItem", "Flexible Zone")));
    sAfModeStr_->insert(Entry(IMVT_AF_FACEDETCTION, QT_TRANSLATE_NOOP("MenuItem", "Face")));
    sAfModeStr_->insert(Entry(IMVT_AF_HUMAN_TRACKING, QT_TRANSLATE_NOOP("MenuItem", "Human Track")));

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
    sShutterAngleStr_->insert(Entry(IMVT_SHUTTER_ANGLE_1,  QT_TRANSLATE_NOOP("MenuItem", "1°")));
    sShutterAngleStr_->insert(Entry(IMVT_SHUTTER_ANGLE_2,  QT_TRANSLATE_NOOP("MenuItem", "2°")));
    sShutterAngleStr_->insert(Entry(IMVT_SHUTTER_ANGLE_4,  QT_TRANSLATE_NOOP("MenuItem", "4°")));
    sShutterAngleStr_->insert(Entry(IMVT_SHUTTER_ANGLE_5,  QT_TRANSLATE_NOOP("MenuItem", "5°")));
    sShutterAngleStr_->insert(Entry(IMVT_SHUTTER_ANGLE_8,  QT_TRANSLATE_NOOP("MenuItem", "8°")));
    sShutterAngleStr_->insert(Entry(IMVT_SHUTTER_ANGLE_10, QT_TRANSLATE_NOOP("MenuItem", "10°")));
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

    Map* sMetaCameraIDStr_ = GetMetaCameraIDStrMap();
    sMetaCameraIDStr_->insert(Entry(IMVT_META_CAMERA_ID_A, "A"));
    sMetaCameraIDStr_->insert(Entry(IMVT_META_CAMERA_ID_B, "B"));
    sMetaCameraIDStr_->insert(Entry(IMVT_META_CAMERA_ID_C, "C"));
    sMetaCameraIDStr_->insert(Entry(IMVT_META_CAMERA_ID_D, "D"));
    sMetaCameraIDStr_->insert(Entry(IMVT_META_CAMERA_ID_E, "E"));
    sMetaCameraIDStr_->insert(Entry(IMVT_META_CAMERA_ID_F, "F"));
    sMetaCameraIDStr_->insert(Entry(IMVT_META_CAMERA_ID_G, "G"));
    sMetaCameraIDStr_->insert(Entry(IMVT_META_CAMERA_ID_H, "H"));
    sMetaCameraIDStr_->insert(Entry(IMVT_META_CAMERA_ID_I, "I"));
    sMetaCameraIDStr_->insert(Entry(IMVT_META_CAMERA_ID_J, "J"));
    sMetaCameraIDStr_->insert(Entry(IMVT_META_CAMERA_ID_K, "K"));
    sMetaCameraIDStr_->insert(Entry(IMVT_META_CAMERA_ID_L, "L"));
    sMetaCameraIDStr_->insert(Entry(IMVT_META_CAMERA_ID_M, "M"));
    sMetaCameraIDStr_->insert(Entry(IMVT_META_CAMERA_ID_N, "N"));
    sMetaCameraIDStr_->insert(Entry(IMVT_META_CAMERA_ID_O, "O"));
    sMetaCameraIDStr_->insert(Entry(IMVT_META_CAMERA_ID_P, "P"));
    sMetaCameraIDStr_->insert(Entry(IMVT_META_CAMERA_ID_Q, "Q"));
    sMetaCameraIDStr_->insert(Entry(IMVT_META_CAMERA_ID_R, "R"));
    sMetaCameraIDStr_->insert(Entry(IMVT_META_CAMERA_ID_S, "S"));
    sMetaCameraIDStr_->insert(Entry(IMVT_META_CAMERA_ID_T, "T"));
    sMetaCameraIDStr_->insert(Entry(IMVT_META_CAMERA_ID_U, "U"));
    sMetaCameraIDStr_->insert(Entry(IMVT_META_CAMERA_ID_V, "V"));
    sMetaCameraIDStr_->insert(Entry(IMVT_META_CAMERA_ID_W, "W"));
    sMetaCameraIDStr_->insert(Entry(IMVT_META_CAMERA_ID_X, "X"));
    sMetaCameraIDStr_->insert(Entry(IMVT_META_CAMERA_ID_Y, "Y"));
    sMetaCameraIDStr_->insert(Entry(IMVT_META_CAMERA_ID_Z, "Z"));

    Map* sCvbsSystemStr_ = GetCvbsSystemStrMap();
    sCvbsSystemStr_->insert(Entry(IMVT_VIDEO_SYSTEM_NTSC, QT_TRANSLATE_NOOP("MenuItem", "NTSC")));
    sCvbsSystemStr_->insert(Entry(IMVT_VIDEO_SYSTEM_PAL, QT_TRANSLATE_NOOP("MenuItem", "PAL")));
    sCvbsSystemStr_->insert(Entry(IMVT_VIDEO_SYSTEM_CINEMA, QT_TRANSLATE_NOOP("MenuItem", "CINEMA")));

    Map* sShutterSpeedStr_ = GetShutterSpeedStrMap();
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_AUTO  , QT_TRANSLATE_NOOP("MenuItem", "Auto")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_32    , QT_TRANSLATE_NOOP("MenuItem", "32\"")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_30    , QT_TRANSLATE_NOOP("MenuItem", "30\"")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_25    , QT_TRANSLATE_NOOP("MenuItem", "25\"")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_20    , QT_TRANSLATE_NOOP("MenuItem", "20\"")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_16    , QT_TRANSLATE_NOOP("MenuItem", "16\"")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_15    , QT_TRANSLATE_NOOP("MenuItem", "15\"")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_13    , QT_TRANSLATE_NOOP("MenuItem", "13\"")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_10    , QT_TRANSLATE_NOOP("MenuItem", "10\"")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_8     , QT_TRANSLATE_NOOP("MenuItem", "8\"")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_7     , QT_TRANSLATE_NOOP("MenuItem", "7\"")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_6P5   , QT_TRANSLATE_NOOP("MenuItem", "6.5\"")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_6P    , QT_TRANSLATE_NOOP("MenuItem", "6\"")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_5     , QT_TRANSLATE_NOOP("MenuItem", "5\"")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_4     , QT_TRANSLATE_NOOP("MenuItem", "4\"")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_3P2   , QT_TRANSLATE_NOOP("MenuItem", "3.2\"")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_3     , QT_TRANSLATE_NOOP("MenuItem", "3\"")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_2P5   , QT_TRANSLATE_NOOP("MenuItem", "2.5\"")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_2     , QT_TRANSLATE_NOOP("MenuItem", "2\"")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1P6   , QT_TRANSLATE_NOOP("MenuItem", "1.6\"")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1P3   , QT_TRANSLATE_NOOP("MenuItem", "1.3\"")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1     , QT_TRANSLATE_NOOP("MenuItem", "1\"")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D1P25, QT_TRANSLATE_NOOP("MenuItem", "1/1.25")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D1P67, QT_TRANSLATE_NOOP("MenuItem", "1/1.67")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D2   , QT_TRANSLATE_NOOP("MenuItem", "1/2")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D2P5 , QT_TRANSLATE_NOOP("MenuItem", "1/2.5")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D3   , QT_TRANSLATE_NOOP("MenuItem", "1/3")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D4   , QT_TRANSLATE_NOOP("MenuItem", "1/4")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D5   , QT_TRANSLATE_NOOP("MenuItem", "1/5")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D6   , QT_TRANSLATE_NOOP("MenuItem", "1/6")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D6P25, QT_TRANSLATE_NOOP("MenuItem", "1/6.25")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D7   , QT_TRANSLATE_NOOP("MenuItem", "1/7")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D8   , QT_TRANSLATE_NOOP("MenuItem", "1/8")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D9   , QT_TRANSLATE_NOOP("MenuItem", "1/9")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D10  , QT_TRANSLATE_NOOP("MenuItem", "1/10")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D11  , QT_TRANSLATE_NOOP("MenuItem", "1/11")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D12  , QT_TRANSLATE_NOOP("MenuItem", "1/12")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D12P5, QT_TRANSLATE_NOOP("MenuItem", "1/12.5")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D13  , QT_TRANSLATE_NOOP("MenuItem", "1/13")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D14  , QT_TRANSLATE_NOOP("MenuItem", "1/14")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D15  , QT_TRANSLATE_NOOP("MenuItem", "1/15")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D16  , QT_TRANSLATE_NOOP("MenuItem", "1/16")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D17  , QT_TRANSLATE_NOOP("MenuItem", "1/17")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D18  , QT_TRANSLATE_NOOP("MenuItem", "1/18")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D19  , QT_TRANSLATE_NOOP("MenuItem", "1/19")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D20  , QT_TRANSLATE_NOOP("MenuItem", "1/20")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D21  , QT_TRANSLATE_NOOP("MenuItem", "1/21")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D22  , QT_TRANSLATE_NOOP("MenuItem", "1/22")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D23  , QT_TRANSLATE_NOOP("MenuItem", "1/23")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D24  , QT_TRANSLATE_NOOP("MenuItem", "1/24")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D25  , QT_TRANSLATE_NOOP("MenuItem", "1/25")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D30  , QT_TRANSLATE_NOOP("MenuItem", "1/30")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D40  , QT_TRANSLATE_NOOP("MenuItem", "1/40")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D48  , QT_TRANSLATE_NOOP("MenuItem", "1/48")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D50  , QT_TRANSLATE_NOOP("MenuItem", "1/50")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D60  , QT_TRANSLATE_NOOP("MenuItem", "1/60")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D80  , QT_TRANSLATE_NOOP("MenuItem", "1/80")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D100 , QT_TRANSLATE_NOOP("MenuItem", "1/100")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D120 , QT_TRANSLATE_NOOP("MenuItem", "1/120")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D125 , QT_TRANSLATE_NOOP("MenuItem", "1/125")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D160 , QT_TRANSLATE_NOOP("MenuItem", "1/160")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D200 , QT_TRANSLATE_NOOP("MenuItem", "1/200")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D240 , QT_TRANSLATE_NOOP("MenuItem", "1/240")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D250 , QT_TRANSLATE_NOOP("MenuItem", "1/250")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D320 , QT_TRANSLATE_NOOP("MenuItem", "1/320")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D400 , QT_TRANSLATE_NOOP("MenuItem", "1/400")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D500 , QT_TRANSLATE_NOOP("MenuItem", "1/500")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D640 , QT_TRANSLATE_NOOP("MenuItem", "1/640")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D800 , QT_TRANSLATE_NOOP("MenuItem", "1/800")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D1000, QT_TRANSLATE_NOOP("MenuItem", "1/1000")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D1250, QT_TRANSLATE_NOOP("MenuItem", "1/1250")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D1600, QT_TRANSLATE_NOOP("MenuItem", "1/1600")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D2000, QT_TRANSLATE_NOOP("MenuItem", "1/2000")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D2500, QT_TRANSLATE_NOOP("MenuItem", "1/2500")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D3200, QT_TRANSLATE_NOOP("MenuItem", "1/3200")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D4000, QT_TRANSLATE_NOOP("MenuItem", "1/4000")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D5000, QT_TRANSLATE_NOOP("MenuItem", "1/5000")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D6400, QT_TRANSLATE_NOOP("MenuItem", "1/6400")));
    sShutterSpeedStr_->insert(Entry(IMVT_SHUTTER_1D8000, QT_TRANSLATE_NOOP("MenuItem", "1/8000")));

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
#ifdef CONFIG_LUT_TYPE_ZLOG_STRING_IS_ZLOG2
    sLutTypeStr_->insert(Entry(IMVT_LUT_TYPE_ZLOG, QT_TRANSLATE_NOOP("MenuItem", "Z-Log2")));
    //Applications can only see the Z-Log2,
    //IMVT_LUT_TYPE_ZRAW exists only at the bottom
    sLutTypeStr_->insert(Entry(IMVT_LUT_TYPE_ZRAW, QT_TRANSLATE_NOOP("MenuItem", "Z-Log2")));
#else
    sLutTypeStr_->insert(Entry(IMVT_LUT_TYPE_ZLOG, QT_TRANSLATE_NOOP("MenuItem", "Z-Log")));
#endif
    sLutTypeStr_->insert(Entry(IMVT_LUT_TYPE_LINEAR,     QT_TRANSLATE_NOOP("MenuItem", "Linear")));
    sLutTypeStr_->insert(Entry(IMVT_LUT_TYPE_FLAT,       QT_TRANSLATE_NOOP("MenuItem", "Flat")));
    //sLutTypeStr_->insert(Entry(IMVT_LUT_TYPE_BT2020,   QT_TRANSLATE_NOOP("MenuItem", "Rec.2020")));
    sLutTypeStr_->insert(Entry(IMVT_LUT_TYPE_ZLOG2_CINE, QT_TRANSLATE_NOOP("MenuItem", "Z-Log2 Cine")));
    sLutTypeStr_->insert(Entry(IMVT_LUT_TYPE_CUSTOMIZED, QT_TRANSLATE_NOOP("MenuItem", "Customized")));
    sLutTypeStr_->insert(Entry(IMVT_LUT_TYPE_HDR10,      QT_TRANSLATE_NOOP("MenuItem", "HDR10")));
    sLutTypeStr_->insert(Entry(IMVT_LUT_TYPE_HLG,        QT_TRANSLATE_NOOP("MenuItem", "HLG")));

    Map* sCameraRotationStr_ = GetCameraRotationStrMap();
    sCameraRotationStr_->insert(Entry(IMVT_IMU_ROTATE_0, QT_TRANSLATE_NOOP("MenuItem", "0")));
    sCameraRotationStr_->insert(Entry(IMVT_IMU_ROTATE_90, QT_TRANSLATE_NOOP("MenuItem", "90")));
    sCameraRotationStr_->insert(Entry(IMVT_IMU_ROTATE_180, QT_TRANSLATE_NOOP("MenuItem", "180")));
    sCameraRotationStr_->insert(Entry(IMVT_IMU_ROTATE_270, QT_TRANSLATE_NOOP("MenuItem", "270")));

    Map* sUsbDeviceRoleStr_ = GetUsbDeviceRoleStrMap();
    sUsbDeviceRoleStr_->insert(Entry(IMVT_USB_CONTROL_ROLE_MSC, QT_TRANSLATE_NOOP("MenuItem", "Mass storage")));
    sUsbDeviceRoleStr_->insert(Entry(IMVT_USB_CONTROL_ROLE_RNDIS, QT_TRANSLATE_NOOP("MenuItem", "Network")));
    sUsbDeviceRoleStr_->insert(Entry(IMVT_USB_CONTROL_ROLE_SERIAL, QT_TRANSLATE_NOOP("MenuItem", "Serial")));
    sUsbDeviceRoleStr_->insert(Entry(IMVT_USB_CONTROL_ROLE_NONE, QT_TRANSLATE_NOOP("MenuItem", "Host")));

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
    sMovieRecordDurationStr_->insert(Entry(IMVT_MOVIE_RECORD_SPLIT_DURATION_1MIN, QT_TRANSLATE_NOOP("MenuItem", "1min")));
    sMovieRecordDurationStr_->insert(Entry(IMVT_MOVIE_RECORD_SPLIT_DURATION_3MIN, QT_TRANSLATE_NOOP("MenuItem", "3min")));
    sMovieRecordDurationStr_->insert(Entry(IMVT_MOVIE_RECORD_SPLIT_DURATION_5MIN, QT_TRANSLATE_NOOP("MenuItem", "5min")));
    sMovieRecordDurationStr_->insert(Entry(IMVT_MOVIE_RECORD_SPLIT_DURATION_10MIN, QT_TRANSLATE_NOOP("MenuItem", "10min")));
    sMovieRecordDurationStr_->insert(Entry(IMVT_MOVIE_RECORD_SPLIT_DURATION_15MIN, QT_TRANSLATE_NOOP("MenuItem", "15min")));
    sMovieRecordDurationStr_->insert(Entry(IMVT_MOVIE_RECORD_SPLIT_DURATION_20MIN, QT_TRANSLATE_NOOP("MenuItem", "20min")));
    sMovieRecordDurationStr_->insert(Entry(IMVT_MOVIE_RECORD_SPLIT_DURATION_30MIN, QT_TRANSLATE_NOOP("MenuItem", "30min")));
    sMovieRecordDurationStr_->insert(Entry(IMVT_MOVIE_RECORD_SPLIT_DURATION_60MIN, QT_TRANSLATE_NOOP("MenuItem", "60min")));

    Map* sMovieAudioEncTypeStr_ = GetMovieAudioEncTypeStrMap();
    sMovieAudioEncTypeStr_->insert(Entry(IMVT_AUDIO_ENCODER_NONE, QT_TRANSLATE_NOOP("MenuItem", "None")));
    sMovieAudioEncTypeStr_->insert(Entry(IMVT_AUDIO_ENCODER_AAC, QT_TRANSLATE_NOOP("MenuItem", "AAC")));
    sMovieAudioEncTypeStr_->insert(Entry(IMVT_AUDIO_ENCODER_PCM, QT_TRANSLATE_NOOP("MenuItem", "PCM")));

    Map* sRecordModeStr_ = GetRecordModeStrMap();
    sRecordModeStr_->insert(Entry(IMVT_RECORD_MODE_NORMAL, QT_TRANSLATE_NOOP("MenuItem", "Normal")));
    sRecordModeStr_->insert(Entry(IMVT_RECORD_MODE_TIMELAPSE, QT_TRANSLATE_NOOP("MenuItem", "Timelapse")));

    Map* sMovieEncoderStr_ = GetMovieEncoderStrMap();
    sMovieEncoderStr_->insert(Entry(IMVT_VIDEO_ENCODER_H264, QT_TRANSLATE_NOOP("MenuItem", "H.264")));
    sMovieEncoderStr_->insert(Entry(IMVT_VIDEO_ENCODER_RAW, QT_TRANSLATE_NOOP("MenuItem", "RAW")));
    sMovieEncoderStr_->insert(Entry(IMVT_VIDEO_ENCODER_ZRAW, QT_TRANSLATE_NOOP("MenuItem", "ZRAW")));
    sMovieEncoderStr_->insert(Entry(IMVT_VIDEO_ENCODER_H265, QT_TRANSLATE_NOOP("MenuItem", "H.265")));
    sMovieEncoderStr_->insert(Entry(IMVT_VIDEO_ENCODER_PRORES_422, QT_TRANSLATE_NOOP("MenuItem", "ProRes 422")));
    sMovieEncoderStr_->insert(Entry(IMVT_VIDEO_ENCODER_PRORES_422_HQ, QT_TRANSLATE_NOOP("MenuItem", "ProRes 422 HQ")));
    sMovieEncoderStr_->insert(Entry(IMVT_VIDEO_ENCODER_PRORES_422_LT, QT_TRANSLATE_NOOP("MenuItem", "ProRes 422 LT")));
    sMovieEncoderStr_->insert(Entry(IMVT_VIDEO_ENCODER_PRORES_422_PROXY, QT_TRANSLATE_NOOP("MenuItem", "ProRes 422 Proxy")));

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

	//!!! must the same with "static unsigned short fixed_extend_shutter_pair_maping[] = {}" in n3a_config_ae.c
    Map* sExtendedVideoShutterTimeStr_ = GetExtendedVideoShutterTimeStrMap();
    sExtendedVideoShutterTimeStr_->insert(Entry( 0, QT_TRANSLATE_NOOP("MenuItem", "Disable")));
    sExtendedVideoShutterTimeStr_->insert(Entry( 1, QT_TRANSLATE_NOOP("MenuItem", "1/20")));
    sExtendedVideoShutterTimeStr_->insert(Entry( 2, QT_TRANSLATE_NOOP("MenuItem", "1/21")));
    sExtendedVideoShutterTimeStr_->insert(Entry( 3, QT_TRANSLATE_NOOP("MenuItem", "1/22")));
    sExtendedVideoShutterTimeStr_->insert(Entry( 4, QT_TRANSLATE_NOOP("MenuItem", "1/23")));
    sExtendedVideoShutterTimeStr_->insert(Entry( 5, QT_TRANSLATE_NOOP("MenuItem", "1/24")));
    sExtendedVideoShutterTimeStr_->insert(Entry( 6, QT_TRANSLATE_NOOP("MenuItem", "1/25")));
    sExtendedVideoShutterTimeStr_->insert(Entry( 7, QT_TRANSLATE_NOOP("MenuItem", "1/26")));
    sExtendedVideoShutterTimeStr_->insert(Entry( 8, QT_TRANSLATE_NOOP("MenuItem", "1/27")));
    sExtendedVideoShutterTimeStr_->insert(Entry( 9, QT_TRANSLATE_NOOP("MenuItem", "1/28")));
    sExtendedVideoShutterTimeStr_->insert(Entry(10, QT_TRANSLATE_NOOP("MenuItem", "1/30")));
    sExtendedVideoShutterTimeStr_->insert(Entry(11, QT_TRANSLATE_NOOP("MenuItem", "1/31")));
    sExtendedVideoShutterTimeStr_->insert(Entry(12, QT_TRANSLATE_NOOP("MenuItem", "1/33")));
    sExtendedVideoShutterTimeStr_->insert(Entry(13, QT_TRANSLATE_NOOP("MenuItem", "1/34")));
    sExtendedVideoShutterTimeStr_->insert(Entry(14, QT_TRANSLATE_NOOP("MenuItem", "1/36")));
    sExtendedVideoShutterTimeStr_->insert(Entry(15, QT_TRANSLATE_NOOP("MenuItem", "1/37")));
    sExtendedVideoShutterTimeStr_->insert(Entry(16, QT_TRANSLATE_NOOP("MenuItem", "1/39")));
    sExtendedVideoShutterTimeStr_->insert(Entry(17, QT_TRANSLATE_NOOP("MenuItem", "1/40")));
    sExtendedVideoShutterTimeStr_->insert(Entry(18, QT_TRANSLATE_NOOP("MenuItem", "1/42")));
    sExtendedVideoShutterTimeStr_->insert(Entry(19, QT_TRANSLATE_NOOP("MenuItem", "1/44")));
    sExtendedVideoShutterTimeStr_->insert(Entry(20, QT_TRANSLATE_NOOP("MenuItem", "1/46")));
    sExtendedVideoShutterTimeStr_->insert(Entry(21, QT_TRANSLATE_NOOP("MenuItem", "1/48")));
    sExtendedVideoShutterTimeStr_->insert(Entry(22, QT_TRANSLATE_NOOP("MenuItem", "1/50")));
    sExtendedVideoShutterTimeStr_->insert(Entry(23, QT_TRANSLATE_NOOP("MenuItem", "1/53")));
    sExtendedVideoShutterTimeStr_->insert(Entry(24, QT_TRANSLATE_NOOP("MenuItem", "1/55")));
    sExtendedVideoShutterTimeStr_->insert(Entry(25, QT_TRANSLATE_NOOP("MenuItem", "1/57")));
    sExtendedVideoShutterTimeStr_->insert(Entry(26, QT_TRANSLATE_NOOP("MenuItem", "1/60")));
    sExtendedVideoShutterTimeStr_->insert(Entry(27, QT_TRANSLATE_NOOP("MenuItem", "1/63")));
    sExtendedVideoShutterTimeStr_->insert(Entry(28, QT_TRANSLATE_NOOP("MenuItem", "1/65")));
    sExtendedVideoShutterTimeStr_->insert(Entry(29, QT_TRANSLATE_NOOP("MenuItem", "1/68")));
    sExtendedVideoShutterTimeStr_->insert(Entry(30, QT_TRANSLATE_NOOP("MenuItem", "1/71")));
    sExtendedVideoShutterTimeStr_->insert(Entry(31, QT_TRANSLATE_NOOP("MenuItem", "1/74")));
    sExtendedVideoShutterTimeStr_->insert(Entry(32, QT_TRANSLATE_NOOP("MenuItem", "1/78")));
    sExtendedVideoShutterTimeStr_->insert(Entry(33, QT_TRANSLATE_NOOP("MenuItem", "1/81")));
    sExtendedVideoShutterTimeStr_->insert(Entry(34, QT_TRANSLATE_NOOP("MenuItem", "1/85")));
    sExtendedVideoShutterTimeStr_->insert(Entry(35, QT_TRANSLATE_NOOP("MenuItem", "1/88")));
    sExtendedVideoShutterTimeStr_->insert(Entry(36, QT_TRANSLATE_NOOP("MenuItem", "1/92")));
    sExtendedVideoShutterTimeStr_->insert(Entry(37, QT_TRANSLATE_NOOP("MenuItem", "1/96")));
    sExtendedVideoShutterTimeStr_->insert(Entry(38, QT_TRANSLATE_NOOP("MenuItem", "1/100")));
    sExtendedVideoShutterTimeStr_->insert(Entry(39, QT_TRANSLATE_NOOP("MenuItem", "1/105")));
    sExtendedVideoShutterTimeStr_->insert(Entry(40, QT_TRANSLATE_NOOP("MenuItem", "1/110")));
    sExtendedVideoShutterTimeStr_->insert(Entry(41, QT_TRANSLATE_NOOP("MenuItem", "1/115")));
    sExtendedVideoShutterTimeStr_->insert(Entry(42, QT_TRANSLATE_NOOP("MenuItem", "1/120")));
    sExtendedVideoShutterTimeStr_->insert(Entry(43, QT_TRANSLATE_NOOP("MenuItem", "1/125")));
    sExtendedVideoShutterTimeStr_->insert(Entry(44, QT_TRANSLATE_NOOP("MenuItem", "1/130")));
    sExtendedVideoShutterTimeStr_->insert(Entry(45, QT_TRANSLATE_NOOP("MenuItem", "1/136")));
    sExtendedVideoShutterTimeStr_->insert(Entry(46, QT_TRANSLATE_NOOP("MenuItem", "1/142")));
    sExtendedVideoShutterTimeStr_->insert(Entry(47, QT_TRANSLATE_NOOP("MenuItem", "1/148")));
    sExtendedVideoShutterTimeStr_->insert(Entry(48, QT_TRANSLATE_NOOP("MenuItem", "1/156")));
    sExtendedVideoShutterTimeStr_->insert(Entry(49, QT_TRANSLATE_NOOP("MenuItem", "1/162")));
    sExtendedVideoShutterTimeStr_->insert(Entry(50, QT_TRANSLATE_NOOP("MenuItem", "1/170")));
    sExtendedVideoShutterTimeStr_->insert(Entry(51, QT_TRANSLATE_NOOP("MenuItem", "1/177")));
    sExtendedVideoShutterTimeStr_->insert(Entry(52, QT_TRANSLATE_NOOP("MenuItem", "1/185")));
    sExtendedVideoShutterTimeStr_->insert(Entry(53, QT_TRANSLATE_NOOP("MenuItem", "1/193")));
    sExtendedVideoShutterTimeStr_->insert(Entry(54, QT_TRANSLATE_NOOP("MenuItem", "1/200")));
    sExtendedVideoShutterTimeStr_->insert(Entry(55, QT_TRANSLATE_NOOP("MenuItem", "1/211")));
    sExtendedVideoShutterTimeStr_->insert(Entry(56, QT_TRANSLATE_NOOP("MenuItem", "1/220")));
    sExtendedVideoShutterTimeStr_->insert(Entry(57, QT_TRANSLATE_NOOP("MenuItem", "1/230")));
    sExtendedVideoShutterTimeStr_->insert(Entry(58, QT_TRANSLATE_NOOP("MenuItem", "1/240")));
    sExtendedVideoShutterTimeStr_->insert(Entry(59, QT_TRANSLATE_NOOP("MenuItem", "1/250")));
    sExtendedVideoShutterTimeStr_->insert(Entry(60, QT_TRANSLATE_NOOP("MenuItem", "1/262")));
    sExtendedVideoShutterTimeStr_->insert(Entry(61, QT_TRANSLATE_NOOP("MenuItem", "1/273")));
    sExtendedVideoShutterTimeStr_->insert(Entry(62, QT_TRANSLATE_NOOP("MenuItem", "1/285")));
    sExtendedVideoShutterTimeStr_->insert(Entry(63, QT_TRANSLATE_NOOP("MenuItem", "1/298")));
    sExtendedVideoShutterTimeStr_->insert(Entry(64, QT_TRANSLATE_NOOP("MenuItem", "1/311")));
    sExtendedVideoShutterTimeStr_->insert(Entry(65, QT_TRANSLATE_NOOP("MenuItem", "1/325")));
    sExtendedVideoShutterTimeStr_->insert(Entry(66, QT_TRANSLATE_NOOP("MenuItem", "1/339")));
    sExtendedVideoShutterTimeStr_->insert(Entry(67, QT_TRANSLATE_NOOP("MenuItem", "1/354")));
    sExtendedVideoShutterTimeStr_->insert(Entry(68, QT_TRANSLATE_NOOP("MenuItem", "1/370")));
    sExtendedVideoShutterTimeStr_->insert(Entry(69, QT_TRANSLATE_NOOP("MenuItem", "1/386")));
    sExtendedVideoShutterTimeStr_->insert(Entry(70, QT_TRANSLATE_NOOP("MenuItem", "1/400")));
    sExtendedVideoShutterTimeStr_->insert(Entry(71, QT_TRANSLATE_NOOP("MenuItem", "1/421")));
    sExtendedVideoShutterTimeStr_->insert(Entry(72, QT_TRANSLATE_NOOP("MenuItem", "1/440")));
    sExtendedVideoShutterTimeStr_->insert(Entry(73, QT_TRANSLATE_NOOP("MenuItem", "1/460")));
    sExtendedVideoShutterTimeStr_->insert(Entry(74, QT_TRANSLATE_NOOP("MenuItem", "1/480")));
    sExtendedVideoShutterTimeStr_->insert(Entry(75, QT_TRANSLATE_NOOP("MenuItem", "1/500")));
    sExtendedVideoShutterTimeStr_->insert(Entry(76, QT_TRANSLATE_NOOP("MenuItem", "1/523")));
    sExtendedVideoShutterTimeStr_->insert(Entry(77, QT_TRANSLATE_NOOP("MenuItem", "1/546")));
    sExtendedVideoShutterTimeStr_->insert(Entry(78, QT_TRANSLATE_NOOP("MenuItem", "1/570")));
    sExtendedVideoShutterTimeStr_->insert(Entry(79, QT_TRANSLATE_NOOP("MenuItem", "1/596")));
    sExtendedVideoShutterTimeStr_->insert(Entry(80, QT_TRANSLATE_NOOP("MenuItem", "1/622")));
    sExtendedVideoShutterTimeStr_->insert(Entry(81, QT_TRANSLATE_NOOP("MenuItem", "1/650")));
    sExtendedVideoShutterTimeStr_->insert(Entry(82, QT_TRANSLATE_NOOP("MenuItem", "1/678")));
    sExtendedVideoShutterTimeStr_->insert(Entry(83, QT_TRANSLATE_NOOP("MenuItem", "1/708")));
    sExtendedVideoShutterTimeStr_->insert(Entry(84, QT_TRANSLATE_NOOP("MenuItem", "1/740")));
    sExtendedVideoShutterTimeStr_->insert(Entry(85, QT_TRANSLATE_NOOP("MenuItem", "1/773")));
    sExtendedVideoShutterTimeStr_->insert(Entry(86, QT_TRANSLATE_NOOP("MenuItem", "1/807")));
    sExtendedVideoShutterTimeStr_->insert(Entry(87, QT_TRANSLATE_NOOP("MenuItem", "1/843")));
    sExtendedVideoShutterTimeStr_->insert(Entry(88, QT_TRANSLATE_NOOP("MenuItem", "1/880")));
    sExtendedVideoShutterTimeStr_->insert(Entry(89, QT_TRANSLATE_NOOP("MenuItem", "1/920")));
    sExtendedVideoShutterTimeStr_->insert(Entry(90, QT_TRANSLATE_NOOP("MenuItem", "1/960")));
    sExtendedVideoShutterTimeStr_->insert(Entry(91, QT_TRANSLATE_NOOP("MenuItem", "1/1000")));

    Map* sNetworkModeStr_ = GetNetworkModeStrMap();
    sNetworkModeStr_->insert(Entry(IMVT_CAMERA_NETWORK_MODE_ROUTER, QT_TRANSLATE_NOOP("MenuItem", "Router")));
    sNetworkModeStr_->insert(Entry(IMVT_CAMERA_NETWORK_MODE_DIRECT, QT_TRANSLATE_NOOP("MenuItem", "Direct")));
    sNetworkModeStr_->insert(Entry(IMVT_CAMERA_NETWORK_MODE_STATIC, QT_TRANSLATE_NOOP("MenuItem", "Static")));

    Map* sBitrateLevelStr_ = GetBirateLevelMap();
    sBitrateLevelStr_->insert(Entry(IMVT_BITRATE_LEVEL_HIGHT,  QT_TRANSLATE_NOOP("MenuItem","High")));
    sBitrateLevelStr_->insert(Entry(IMVT_BITRATE_LEVEL_MIDDLE,  QT_TRANSLATE_NOOP("MenuItem","Medium")));
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
    sHdmiFmtStr_->insert(Entry(IMVT_EDID_VIDEO_ID_RESERVED, QT_TRANSLATE_NOOP("MenuItem", "Auto")));
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
    sWbTintPriorityStr_->insert(Entry(IMVT_WB_PRIORITY_AMBIANCE, QT_TRANSLATE_NOOP("PriorityItem", "Ambiance")));
    sWbTintPriorityStr_->insert(Entry(IMVT_WB_PRIORITY_WHITE, QT_TRANSLATE_NOOP("PriorityItem", "White")));

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

    Map* sOotfStr_ = GetOoftMap();
    sOotfStr_->insert(Entry(IMVT_OOTF_SDR_GAMMA,  QT_TRANSLATE_NOOP("MenuItem","SDR Gamma")));
    sOotfStr_->insert(Entry(IMVT_OOTF_HDR_PQ,     QT_TRANSLATE_NOOP("MenuItem","HDR10")));
    sOotfStr_->insert(Entry(IMVT_OOTF_HDR_HLG,    QT_TRANSLATE_NOOP("MenuItem","HLG")));

    Map* sNativeIsoStr_ = GetNativeIsoMap();
    sNativeIsoStr_->insert(Entry(IMVT_ISO_OPTION_FINE,  QT_TRANSLATE_NOOP("MenuItem","Fine")));
    sNativeIsoStr_->insert(Entry(IMVT_ISO_OPTION_NATIVE_ONLY,   QT_TRANSLATE_NOOP("MenuItem","Native ISO")));

    Map* sAeSpeedStr_ = GetAeSpeedMap();
    sAeSpeedStr_->insert(Entry(IMVT_AE_SPEED_RATIO_SLOW,  QT_TRANSLATE_NOOP("MenuItem","Slow")));
    sAeSpeedStr_->insert(Entry(IMVT_AE_SPEED_RATIO_NORMAL,  QT_TRANSLATE_NOOP("MenuItem","Normal")));
    sAeSpeedStr_->insert(Entry(IMVT_AE_SPEED_RATIO_FAST,  QT_TRANSLATE_NOOP("MenuItem","Fast")));

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
    case CAMERA_CONFIG_LIVE_CAF:
    case CAMERA_CONFIG_ASSITOOL_OVERLAY_CENTER_MARK:
    case CAMERA_CONFIG_ASSITOOL_PEAK_BW_BACKGROUND:
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
    case CAMERA_CONFIG_MIN_ISO_LIMIT:
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
    case CAMERA_CONFIG_FILELIST_ENABLE:
    case CAMERA_CONFIG_PRERECORD_ENABLE:
    case CAMERA_CONFIG_ENABLE_CROP_SENSOR:
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
    case CAMERA_CONFIG_META_CAMERA_ID:
        map = GetMetaCameraIDStrMap();
        break;
    case CAMERA_CONFIG_CVBS_VIDEO_SYSTEM:
        map = GetCvbsSystemStrMap();
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
    case CAMERA_CONFIG_AE_SPEED_RATIO:
        map = GetAeSpeedMap();
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
    case CAMERA_CONFIG_STILL_SHUTTER_SPEED:
        map = GetShutterSpeedStrMap();
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
    case CAMERA_CONFIG_WB_FREEZE:
        map = GetAeLockMap();
        break;
    case CAMERA_CONFIG_WB_MWB_AUTO_DETECT:
        map = GetOnOffStrMap();
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
    case CAMERA_CONFIG_ISO_OPTION_CTRL:
        map = GetNativeIsoMap();
        break;
    case CAMERA_CONFIG_ACODEC_INPUT_CHANNEL:
        map = GetAudioInputMap();
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
    case CAMERA_CONFIG_VFR_CONTROL_TYPE:
        map = GetVfrControlStrMap();
        break;
 #ifdef CONFIG_IMVT_ASSITOOL
    case CAMERA_CONFIG_ASSITOOL_DISPLAY:
        map = GetAssitoolDisplayMap();
        break;
    case CAMERA_CONFIG_ASSITOOL_PEAK_COLOR:
    case CAMERA_CONFIG_ASSITOOL_FRAME_LINE_COLOR:
    case CAMERA_CONFIG_ASSITOOL_CENTER_MARK_COLOR:
        map = GetAssitoolColorMap();
        break;
    case CAMERA_CONFIG_ASSITOOL_EXPOSURE_TOOL:
        map = GetAssitoolExposureMap();
        break;
    case CAMERA_CONFIG_ASSITOOL_SCOPE_TOOL:
        map = GetAssitoolScopeMap();
        break;
    case CAMERA_CONFIG_ASSITOOL_OVERLAY_FRAME_LINE:
        map = GetAssitoolFrameLineMap();
        break;
 #endif
    case CAMERA_CONFIG_RECORD_FRAMERTAE:
        map = GetReocrdFramerateMap();
        break;
    case CAMERA_CONFIG_COLOR_LUT:
        map = GetMonitorLutMap();
        break;
    case CAMERA_CONFIG_DESQUEEZE_FACTOR:
        map = GetDesqueezeMap();
        break;
    case CAMERA_CONFIG_AF_AREA:
        map = GetAfAreaMap();
        break;
    case CAMERA_CONFIG_FAN_MODE_SWITCH:
        map = GetFanModeMap();
        break;
    case CAMERA_CONFIG_WB_CALIB:
        map = GetWBCalibMap();
        break;
    case CAMERA_CONFIG_OOTF:
        map = GetOoftMap();
        break;
    case CAMERA_CONFIG_HDMI_USE_EDID:
        map = GetEnableDisableMap();
        break;

    default:
        map = NULL;
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

int CameraConfig::trKeyFromString(const char *key)
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

int CameraConfig::trValueFromString(int key, const char *value)
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


int CameraConfig::isNeedConvertedToOption(int key, int type)
{
    int isConvertOption = 0;
    if (type == CAMERA_CONFIG_TYPE_RANGE) {

        if (key == CAMERA_CONFIG_MOVIE_VFR) {
            isConvertOption = 1;
        }

    }

    return isConvertOption;
}

//int CameraConfig::rangeConvertedToOption(int key, std::vector<short> &optionList)
//{
//    int ret = -1;

//    struct camera_config config;
//    memset(&config, 0, sizeof(config));
//    config.key = key;
//    ret = imvt_cam_app_get_config(&config);
//    if (ret == 0 && config.type == CAMERA_CONFIG_TYPE_RANGE) {
//        if (key == CAMERA_CONFIG_MOVIE_VFR) {
//            optionList.push_back(0);
//            if (config.u.range.min != config.u.range.max) {
//                for (int value = config.u.range.min; value <= config.u.range.max; value++) {
//                    optionList.push_back(value);
//                }
//            }
//        } else {
//            ret = -1;
//        }
//    }

//    return ret;
//}

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
