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

    UART_SWITCH_TO_PB,       // | cmd |
                             // | ack | ok/ng |

    UART_START_REC,          // | cmd |
                             // | ack | ok/ng |

    UART_STOP_REC,           // | cmd |
                             // | ack | ok/ng |

    UART_CAPTURE,            // | cmd |
                             // | ack | ok/ng |

    UART_CAPTURE_AF,         // | cmd |
                             // | ack | ok/ng |

    UART_AF,                 // | cmd |
                             // | ack | ok/ng |

    UART_START_PB,           // | cmd | folder index | file index[1] | file index[0]
                             // | ack | ok/ng |

    UART_STOP_PB,            // | cmd |
                             // | ack | ok/ng |

    UART_PAUSE_PB,           // | cmd |
                             // | ack | ok/ng |

    UART_RESUME_PB,          // | cmd |
                             // | ack | ok/ng |

    UART_SET_CONFIG,         // | cmd | key | type | value[n]
                             // | ack | ok/ng |

    UART_GET_CONFIG,         // | cmd | key |
							 // | ack | ok/ng |

    UART_SET_WIFI,           // | cmd | on/off |
                             // | ack | ok/ng |

    UART_GET_WIFI,           // | cmd |
                             // | ack | ok/ng | on/off |

    UART_GET_BATTERY,        // | cmd |
                             // | ack | ok/ng | battery(1 byte) |

    UART_GET_CARD_STATUS,    // | cmd |
                             // | ack | 0/1 |

    UART_GET_MODE,           // | cmd |
                             // | ack | ok/ng | mode |

    UART_GET_STATUS,         // | cmd |
                             // | ack | ok/ng | status |

    UART_GET_REC_REMAINING,  // | cmd |
                             // | ack | ok/ng | 4 byte(minutes, high|low)|

    UART_GET_STILL_REMAINING,// | cmd |
                             // | ack | ok/ng | 4 byte (amount of capture, high|low) |

    UART_FORMAT_CARD,        // | cmd |
                             // | ack | ok/ng |

    UART_GET_BT_VERSION,     // | cmd |
                             // | ack | ok/ng | major | minor |

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

supported config

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
    CAMERA_CONFIG_NONE,

### shutter line
It works like half pressed/full pressed on the shutter button. (pin5/pin6)<br>
Pressed - Low level<br>
Released - High level<br>
	
### SBUS
channel 4 - mode switching<br>
channel 5 - capure/rec<br>

Please leave your message if you need more information. Thanks.
		
		


