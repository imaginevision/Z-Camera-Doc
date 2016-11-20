### Genaral
E1 has an expansion port to extend the function. It contains the following port:

- SPI
- UART
- I2C
- GPIO
- CVBS
- VCC
- GND

The port is multiplexed. We need an EEPROM in the extendtion board to indentify how to configurate the pin.
When the camera booting, it will read the first byte in external EEPROM(address 0xa3) to indentify the expansion role. Please note here, hot plug is not supported. Currently the following role is supported:

- CVBS + shutter line (0x00 in first byte)
- CVBS + SBUS (0x01 in first byte)
- CVBS + VISCA (0x02 in first byte, desperated)
- CVBS + UART (0x03 in first byte)

### Hardware
Please refer to the pin.jpg and USB-LG12-0245.pdf<br>
S01 GPIO TEST.pdf is an example for external board. 

### UART command
Please make sure the 1st byte in eeprom is 0x03.<br>
Under this mode, the E1 work as a server while the external work as client.<br>
baud rate is 115200, 8 bits data, 1 bit stop, none parity.

Command format:

	req: 0xea | 0x02 | len | cmd | param0 | param1 | param2 |
	akc: 0xea | 0x02 | len | (cmd|0x80) | {ok/ng} | param, if it has

	len = sizeof(cmd) + sizeof(param) + sizeof(ok/ng)
	
	e.g.
		switch mode to rec command: 0x02
		req: 0xea 0x02 0x01 0x02
		ack: 0xea 0x02 0x02 0x82 0x00

Command list:

	command                  command format                      example        

	UART_CMD_NONE = 0x0,

    UART_EMULATE_KEY,        // | cmd | RC_CODE_* |              0xea 0x02 0x02 0x01 0x00
                             // | ack | ok/ng |                  0xea 0x02 0x02 0x81 0x00

    UART_SWITCH_TO_REC,      // | cmd |                          0xea 0x02 0x01 0x02
                             // | ack | ok/ng |					 0xea 0x02 0x02 0x82 0x00

    UART_SWITCH_TO_STILL,    // | cmd |                          0xea 0x02 0x01 0x03
                             // | ack | ok/ng |                  0xea 0x02 0x02 0x83 0x00

    UART_SWITCH_TO_PB,       // | cmd |                          0xea 0x02 0x01 0x04
                             // | ack | ok/ng |                  0xea 0x02 0x02 0x84 0x00

    UART_START_REC,          // | cmd |                          0xea 0x02 0x01 0x05
                             // | ack | ok/ng |                  0xea 0x02 0x02 0x85 0x00

    UART_STOP_REC,           // | cmd |                          0xea 0x02 0x01 0x06
                             // | ack | ok/ng |                  0xea 0x02 0x02 0x86 0x00

    UART_CAPTURE,            // | cmd |                          0xea 0x02 0x01 0x07
                             // | ack | ok/ng |                  0xea 0x02 0x02 0x87 0x00

    UART_CAPTURE_AF,         // | cmd |                          0xea 0x02 0x01 0x08
                             // | ack | ok/ng |                  0xea 0x02 0x02 0x88 0x00

    UART_AF,                 // invalid and deserted

    UART_START_PB,           // | cmd |                          0xea 0x02 0x01 0x0a
                             // | ack | ok/ng |                  0xea 0x02 0x02 0x8a 0x00

    UART_STOP_PB,            // | cmd |                          0xea 0x02 0x01 0x0b
                             // | ack | ok/ng |                  0xea 0x02 0x02 0x8b 0x00

    UART_PAUSE_PB,           // | cmd |                          0xea 0x02 0x01 0x0c
                             // | ack | ok/ng |                  0xea 0x02 0x02 0x8c 0x00

    UART_RESUME_PB,          // | cmd |                          0xea 0x02 0x01 0x0d
                             // | ack | ok/ng |                  0xea 0x02 0x02 0x8d 0x00

    UART_SET_CONFIG,         // | cmd | key | type | value[n]
                             // | ack | ok/ng |

    UART_GET_CONFIG,         // | cmd | key |
							 // | ack | ok/ng | type | current config | config list |

    UART_SET_WIFI,           // | cmd | on/off(1/0) |            0xea 0x02 0x02 0x10 0x01
                             // | ack | ok/ng |                  0xea 0x02 0x02 0x90 0x00

    UART_GET_WIFI,           // | cmd |                          0xea 0x02 0x01 0x11
                             // | ack | ok/ng | on/off |         0xea 0x02 0x03 0x91 0x00 0x01

    UART_GET_BATTERY,        // | cmd |                                         0xea 0x02 0x01 0x12
                             // | ack | ok/ng | battery(1 byte, range:0~100) |  0xea 0x02 0x03 0x92 0x00 0x50

    UART_GET_CARD_STATUS,    // | cmd |                          0xea 0x02 0x01 0x13
                             // | ack | ok/ng | ready/not(1/0) | 0xea 0x02 0x03 0x93 0x00 0x01

    UART_GET_MODE,           // | cmd |
                             // | ack | ok/ng | mode(0:capture,1:record,3:palyback) |

    UART_GET_STATUS,         // | cmd |
                             // | ack | ok/ng | status |
    status' value and meaning list below:
    unknown,                   0x00
    record_mode,               0x10
    recording,                 0x11
    playback_mode,             0x20
    playing,                   0x21
    playback_paused,           0x22
    still_mode,                0x40
    still_mode_timelapse_idle, 0x41
    still_mode_timelapse_ing,  0x42

    UART_GET_REC_REMAINING,  // | cmd |
                             // | ack | ok/ng | 4 byte(minutes, high|low)|

    UART_GET_STILL_REMAINING,// | cmd |
                             // | ack | ok/ng | 4 byte (amount of capture, high|low) |

    UART_FORMAT_CARD,        // | cmd |                          0xea 0x02 0x01 0x18
                             // | ack | ok/ng |                  0xea 0x02 0x02 0x98 0x00

    UART_GET_BT_VERSION,     // invalid and deserted

    UART_SWITCH_TO_MULTIPLE_MODE_CAPTURE, // | cmd |             0xea 0x02 0x01 0x1a
                                          // | ack | ok /ng |    0xea 0x02 0x02 0x9a 0x00

    UART_SET_X_CONFIG,       // | cmd | key | type | value[n]
                             // | ack | ok/ng |

    UART_GET_X_CONFIG,       // | cmd | key |
                             // | ack | ok/ng | type | current config | config list |

    UART_QUERY_IS_RECORDING, // | cmd |                          0xea 0x02 0x01 0x1d
                             // | ack | recording/not(1/0) |     0xea 0x02 0x02 0x9d 0x01

    UART_BURST_CAPTURE_START,  // | cmd |                        0xea 0x02 0x01 0x1e
                               // | ack | ok/ng |                0xea 0x02 0x02 0x9e 0x00

    UART_BURST_CAPTURE_CANCLE, // | cmd |                        0xea 0x02 0x01 0x1f
                               // | ack | ok/ng |                0xea 0x02 0x02 0x9f 0x00

    UART_CLEAR_SETTING,      // | cmd |                          0xea 0x02 0x01 0x20
                             // | ack | ok/ng |                  0xea 0x02 0x02 0xa0 0x00

    UART_INIT_PBMEDIA_FILE,  // | cmd |                          0xea 0x02 0x01 0x21
                             // | ack | ok/ng |                  0xea 0x02 0x02 0xa1 0x00
  
    UART_PBMEDIA_NEXT_FILE,  // | cmd |                          0xea 0x02 0x01 0x22
                             // | cmd | ok/ng |                  0xea 0x02 0x02 0xa2 0x00

    UART_PBMEDIA_PREV_FILE,  // | cmd |                          0xea 0x02 0x01 0x23
                             // | cmd | ok/ng |                  0xea 0x02 0x02 0xa3 0x00

    UART_PBMEDIA_DELETE_FILE,// | cmd |                          0xea 0x02 0x01 0x24
                             // | cmd | ok/ng |                  0xea 0x02 0x02 0xa4 0x00
    
    UART_SHUTDOWN,           // | cmd |                          0xea 0x02 0x01 0x25
                             // | ack | ok/ng |                  0xea 0x02 0x02 0xa5 0x00

UART GET CONFIG is used for get the camera setting. For example, the ISO.
There are three types of config in camera

- choice, e.g. ISO, IRIS..
- range, e.g. brightness, saturation... It should have max, min, step
- string, not supported in UART now.

format:

	req : | 0xea | 0x02 | len | get_config | config key | 
	ack : if type is 0x01(choice), | 0xea | 0x02 | len | ack_cmd | ok/ng | type | current config | config list |
		  if type is 0x02(range), | 0xea | 0x02 | len | ack_cmd | ok/ng | type | current value (4 bytes, high|low) | max (4 bytes, hig|low) | min (4 bytes, high|low) | step (4 bytes, high|low) |
	
	e.g.
		get iso
		| 0xea | 0x02 | len | 0x0f | 0x03 |
		| 0xea | 0x02 | len | 0x8f | 0x00 | 0x01 | 0x00 | 0x00 | 0x01 | 0x02 | ....

		get brightness
		| 0xea | 0x02 | len | 0x0f | 0x0f |
		| 0xea | 0x02 | len | 0x8f | 0x00 | 0x02 | 45 (4 bytes, high|low) | 100 (4 bytes, hig|low) | 0 (4 bytes, high|low) | 1 (4bytes, high|low) |

UART SET CONFIG is used for set the camera. For example, the ISO, IRIS and so on.
There are three types of config in camera

- choice, e.g. ISO, IRIS..
- range, e.g. brightness, saturation...
- string, not supported in UART now.

format:

	req : if type is 0x01(choice), | 0xea | 0x02 | len | set_config | config key | type | current config |
          if type is 0x02(range), | 0xea | 0x02 | len | set_config | config key | type | current value （4 bytes, high|low）|
	ack : | 0xea | 0x02 | len | ack_cmd | ok/ng | 
	
	e.g.
		set iso
		| 0xea | 0x02 | 0x04 | 0x0e | 0x03 | 0x01 | 0x00 |               Here value 0x00 means ISOAuto
        | 0xea | 0x02 | 0x02 | 0x8e | 0x00 |

		set iris
		| 0xea | 0x02 | 0x04 | 0x0e | 0x15 | 0x01 | 0x00 |               Here value 0x00 means F0.7           
		| 0xea | 0x02 | 0x02 | 0x8e | 0x00 |
        
        set focus method
        | 0xea | 0x02 | 0x04 | 0x0e | 0x16 | 0x01 | 0x00 |               Here value 0x00 means MF
        | 0xea | 0x02 | 0x02 | 0x8e | 0x00 |
        
        set MF drive
        | 0xea | 0x02 | 0x04 | 0x0e | 0x21 | 0x02 | 0x00 | 0x00 | 0x00 | 0x03 |
        | 0xea | 0x02 | 0x02 | 0x8e | 0x00 |

        set lens zoom
        | 0xea | 0x02 | 0x04 | 0x0e | 0x30 | 0x01 | 0x00 |
        | 0xea | 0x02 | 0x02 | 0x8e | 0x00 |

supported config key

	CAMERA_CONFIG_MOVIE_FORMAT = 0x0,
    CAMERA_CONFIG_PHOTO_SIZE,
    CAMERA_CONFIG_WB,
    CAMERA_CONFIG_ISO,
    CAMERA_CONFIG_SHARPNESS,
    CAMERA_CONFIG_CONTRAST,         // 5
    CAMERA_CONFIG_AE_METER_MODE,
    CAMERA_CONFIG_SCENCE,
    CAMERA_CONFIG_DIGITAL_EFFECT,
    CAMERA_CONFIG_FLICKER_REDUCTION,
    CAMERA_CONFIG_VIDEO_SYSTEM,     // 10
    CAMERA_CONFIG_WIFI_ONOFF,
    CAMERA_CONFIG_EV_BIAS,
    CAMERA_CONFIG_BATTERY,
    CAMERA_CONFIG_SATURATION,
    CAMERA_CONFIG_BRIGHTNESS,       // 15
    CAMERA_CONFIG_NOISE_REDUCTION,
    CAMERA_CONFIG_PHOTO_QUALITY,
    CAMERA_CONFIG_LCD_ONOFF,
    CAMERA_CONFIG_ROTATION,
    CAMERA_CONFIG_VERSION,          // 20
    CAMERA_CONFIG_IRIS,
    CAMERA_CONFIG_FOCUS_METHOD,
    CAMERA_CONFIG_AF_AREA,
    CAMERA_CONFIG_MAGNIFY_POSITION,
    CAMERA_CONFIG_NEW_FW,           // 25
    CAMERA_CONFIG_HW_VERSION,
    CAMERA_CONFIG_DO_AF,
    CAMERA_CONFIG_CAF_ONOFF,
    CAMERA_CONFIG_LENS_ATTACHED,
    CAMERA_CONFIG_LED_ENABLE,       // 30
    CAMERA_CONFIG_BEEPER_ENABLE,
    CAMERA_CONFIG_AF_MODE,
    CAMERA_CONFIG_MF_DRIVE,
    CAMERA_CONFIG_MODEL_NAME,
    CAMERA_CONFIG_LCD_BACKLIGHT_LEVEL,  // 35
    CAMERA_CONFIG_PHOTO_BURST,
    CAMERA_CONFIG_RTC_TIME,
    CAMERA_CONFIG_BT_MAC,
    CAMERA_CONFIG_MAX_SHUTTER_TIME,
    CAMERA_CONFIG_PC_CONNECTED,         // 40
    CAMERA_CONFIG_USB_CABLE_STATUS,
    CAMERA_CONFIG_OLED_ONOFF_ENABLE,
    CAMERA_CONFIG_SHUTTER_ANGLE,
    CAMERA_CONFIG_DCF_REACH_MAX_NUMBER,
    CAMERA_CONFIG_MANUAL_WB,            // 45
    CAMERA_CONFIG_HDMI_OSD_ONOFF,
    CAMERA_CONFIG_STILL_SHUTTER_SPEED,  //
    CAMERA_CONFIG_LENS_ZOOM,
    CAMERA_CONFIG_DCF_FILE_NUMBERING,
    CAMERA_CONFIG_CVBS_VIDEO_SYSTEM,   //50
    CAMERA_CONFIG_CVBS_OUTPUT_ENBLE,
    CAMERA_CONFIG_LENS_FOCUS_POSITION,
    CAMERA_CONFIG_LENS_FOCUS_SPEED,
    CAMERA_CONFIG_MANUAL_WB_TINT,
    CAMERA_CONFIG_CAF_RANGE,            // 55
    CAMERA_CONFIG_CAF_SENSITIVITY,
    CAMERA_CONFIG_ENC_ROTATION,
    CAMERA_CONFIG_VIDEO_QUALITY,
    CAMERA_CONFIG_DUAL_STREAM_ENABLE,
    CAMERA_CONFIG_PHOTO_AEB,          // 60
    CAMERA_CONFIG_CAPTURE_TIMESTAMP,
    CAMERA_CONFIG_RECORD_TIMESTAMP,
    CAMERA_CONFIG_IMU_ROTATION,
    CAMERA_CONFIG_PHOTO_BURST_SPEED,
    CAMERA_CONFIG_LUT_TYPE,          //65
    CAMERA_CONFIG_DCF_LAST_FILE_NAME,
    CAMERA_CONFIG_UART_COMMAND_SUPPORTED,
    CAMERA_CONFIG_LCD_RUNTIME_ONOFF,
    CAMERA_CONFIG_MOV_CONTAINER_ROTATION,
    CAMERA_CONFIG_UI_TIMELAPSE_STATUS,  // 70, internal use
    CAMERA_CONFIG_USB_CHARGE_DETECTION, 
    CAMERA_CONFIG_USB_DEVICE_ROLE,
    CAMERA_CONFIG_IC_TEMPERATURE,
    CAMERA_CONFIG_DCF_NAME_MODE,// 1、ABCD0001.JPG 2、ABCD_201501011633_0001.JPG 3、ABCD_0001_201501011633.JPG
    CAMERA_CONFIG_CAMERA_IS_MULTIPLE, // 75  // camera is multiple master or slave
    CAMERA_CONFIG_DEWARP_ONOFF, 
    CAMERA_CONFIG_MAX_RECORD_TEMPERATURE_LIMIT, 
    CAMERA_CONFIG_VIGNETTE_ONOFF,    // vignette correction
    CAMERA_CONFIG_SECONDRY_STREAM_RESOLUTION,
    CAMERA_CONFIG_SECONDRY_STREAM_BITRATE,  // 80
    CAMERA_CONFIG_RECORD_INT_CAP,
    CAMERA_CONFIG_HDMI_PREFER_FORMAT,
    CAMERA_CONFIG_MULTIPLE_CONTROL_ID,
    CAMERA_CONFIG_MULTIPLE_CAPTURE_DELAY,
    CAMERA_CONFIG_CAMERA_DEFLECTION_ANGLE,   // 85
    CAMERA_CONFIG_VOLUME_CONTROL,
    CAMERA_CONFIG_AE_EXPOSURE_MDOE,
    CAMERA_CONFIG_OIS_MODE,
    CAMERA_CONFIG_MOVIE_RECORD_SPLIT_DURATION,
    CAMERA_CONFIG_MULTIPLE_TIMEOUT_ENABLE,  // 90
    CAMERA_CONFIG_MULTIPLE_CONTROL_ENABLE,
    CAMERA_CONFIG_AEB_NUMCAPTURE,
    CAMERA_CONFIG_LIVEVIEW_WITH_AUDIO,
    CAMERA_CONFIG_SECONDRY_STREAM_GOP,
    CAMERA_CONFIG_SEND_TO_LNX_STREAM,    // 95
    CAMERA_CONFIG_PRIMARY_STREAM_BITRATE,
    CAMERA_CONFIG_SECONDARY_AUDIO_TYPE,
    CAMERA_CONFIG_SECONDARY_B_FRAME,
    CAMERA_CONFIG_AELOCK,
    CAMERA_CONFIG_SECONDARY_STREAM_BITRARE_TYPE, // 100, value is CBR, VBR
    CAMERA_CONFIG_GROUP_INDEX,
    CAMERA_CONFIG_MAX_ISO_LIMIT,
    CAMERA_CONFIG_ETHERNET_IP,
    CAMERA_CONFIG_NONE = 0xFF,

### shutter line
It works like half pressed/full pressed on the shutter button. (pin5/pin6)<br>
Pressed - Low level<br>
Released - High level<br>
	
### SBUS
channel 4 - mode switching<br>
channel 5 - capure/rec<br>

Please leave your message if you need more information. Thanks.
