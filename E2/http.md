This document describes the HTTP command to control the Z CAM E2 (firmware 0.82).

## Basic
 ```HTTP
 GET /url
 ```

```javascript
{
    "code":0,  // 0 is OK, 1 is NG
    "desc":"demo",
    "msg":"..."
}
```

## Get basic information
You can use this command as 'ping' to see if the camera is OK.

```HTTP
GET /info
```

```javascript
{
	"model": "...",   // model of camera
	"number": "1",// how many deivce in camera
	"sw": "0.82", // fw version
	"hw": "1",// hw version
	"mac": "4e:4:b8:2d:78:db", // mac address
	"eth_ip": "192.168.9.81",  // ethernet ip address
	"sn": "329A0010009"// camera serial number
	//...
}
```
## Session
Only one client can control the camera at the same time.

You can try to get the session in the following interface, status code 409 means that you are failed to get the session.
```HTTP
GET /ctrl/session
```
Most of the command under /ctrl/* requeir the ownership of session.

Quit the session. 

Once you leave your control application, you'd better to quit the session.
```HTTP
GET /ctrl/session?action=quit 
```

## Date/time
It's recommend that sync the date/time every time you connect to the camera.
```HTTP
GET /datetime?date=YYYY-MM-DD&time=hh:mm:ss
```
## System control
```HTTP
GET /ctrl/shutdown
GET /ctrl/reboot
```

## Working mode
Basically, the camera works in two major modes: video record and playback.

### Query the working mode
```HTTP
GET /ctrl/mode?action=query
```
```javascript
{
    "code": 0,
    "desc": "",
    "msg": "rec",   // rec, rec_ing, rec_paused 
                    // cap
                    // pb, pb_ing, pb_paused     
                    // cap_tl_idle, cap_tl_ing
                    // cap_burst
                    // cap_snap_idle, cap_snap_ing
                    // rec_tl_idle, rec_tl_ing
                    // unknown
}
```

| msg       | description                       |
| :---      |    :----                          |
| rec       | video record                      |
| rec_tl    | video timelapse record            |
| pb        | playback mode                     |
| cap       | still capture (not in E2)         |
| cap_burst | burst still capture (not in E2)   |

*_ing means it's recording or playbacking video


### Change the working mode
Switch to video record mode
```HTTP
GET /ctrl/mode?action=to_rec
```
Switch to playback mode
```HTTP
GET /ctrl/mode?action=to_pb
```

## Video record control
It will start video record/video timelapse record
```HTTP
/ctrl/rec?action=start
```

It will stop video record/video timelapse record
```HTTP
GET /ctrl/rec?action=stop
```

Query the maximun recording time, in minutes.
```HTTP
GET /ctrl/rec?action=remain
```

## Network streaming

In Z CAM E2, there are two streams could be used for streaming.

Stream 0

By default, it's used as the file recording in stroage card. The resolution and fps is controlled by MovieFormat and VFR. The encoder is set by the VideoEncoder in GUI or by video_encoder in /ctrl/set interface.

Stream 1

By default, it's used by the network streaming. The resolution is limited by the stream 0, it can NOT be larger than the stream 0. By default, the fps is 25 or 30, the encoder is H.264. The maxinum resolution and fps is 4KP30.


### Alter the stream source

Using the stream 1 as the network streaming source is recommend.
If the stream 1 can NOT meet your requirement, you can use the stream 0 as the network stream source. 

However, in this case, you can't record the stream to file.

Change the stream source of network streaming.
```HTTP
GET /ctrl/set?send_stream=Stream0
GET /ctrl/set?send_stream=Stream1
```

### Buildin stream service
We have build some stream services inside the camera. You can use them in an easy way.

- MJPEG over HTTP
  Basically, it's for debug. As the compression efficiency of MJPEG is not good, it use a larger bandwidth than H.264/H.265. You can use it as a quick solution to see what happen in the scene.
    ```HTTP
    GET /mjpeg_stream
    ```

- RTSP
    ```
    rtsp://ip/live_stream
    ```

- SSP (NDA is need, and you need to know how to decode and sync)

### Advance setting

Change the setting
```HTTP
GET /ctrl/stream_setting
```
| param             | description                       |
| :---              |:----                              |
| index             | stream0/stream1                   |
| width             | video width                       |
| height            | video height                      |
| bitrate           | encode bitrate (bps)              |
| split             | in seconds (less than 5 minutes)  |

As metion above, by default, stream 1 is used by network streaming, you can change the setting of the stream.

Change the resolution
```HTTP
GET /ctrl/stream_setting?index=stream1&width=1920&height=1080
GET /ctrl/stream_setting?index=stream1&width=848&height=480
GET /ctrl/stream_setting?index=stream1&width=3840&height=2160
```
***The width must be 32 pixel alignment***

***You must take care of the aspect ratio***

***You must stop the streaming before you change the size***

Change the bitrate. e.g. change to 10Mbps

```HTTP
GET /ctrl/stream_setting?index=stream1&bitrate=10000000
```

Query the setting
```HTTP
GET /ctrl/stream_setting?action=query
```

## Camera settings
You can control most of the settings in the camera, just like what you see in the camera's GUI.

There are three data types of the camera settings.

- Choice
    
    It contains an option list, you can choose the item you need.

- Range

    It describes the mininum value, maxinum value, and the step of the setting.

- String

    It's a string. e.g. The name of your SSID.

You should get the data type and status of the setting before you change it.

Each of the setting is bind to a key. 

### Get setting
Use the following interface to get setting.
```HTTP
GET /ctrl/get?k=key
```

Common get response.
```javascript
{
    "code": 0,          // 0: key is supported; -1: key is not supported
    "desc": "...", 
    "key": "...",       // the key you want to get
    "type": 1,          // 1: choice; 2: range; 3: string
    "ro": 0,            // 1: read only, you can NOT change it; 
                        // 0: you can change it
    "value": "...",    // current value
    // ...
}
```

For different types of the setting, it contains different JSON body.
- Choice
We use video_system for reference.
```javascript
{
    "code": 0,
    "desc": "...", 
    "key": "vieo_system",               // key
    "type": 1,                          // choice type
    "ro": 0,
    "value": "NTSC",                    // current value
    "opts": ["NTSC", "PAL", "CINEMA"]   // valid choices
}
```

- Range
We use contrast for reference.
```javascript
{
    "code": 0,
    "desc": "...",
    "key": "contrast",  // key
    "type": 2,          // range type
    "ro": 0,
    "value": 52,        // current value
    "min": 0,           // min value
    "max": 100,         // max value
    "step": 1           // min step
}
```

- String
```javascript
{
    "code": 0,
    "desc": "string",
    "key": "sn",
    "type": 3,              //string type
    "ro": 1,
    "value": "329A0010009"  // current value
}
```

### Set setting
To set setting, use the following interface.
```HTTP
GET /ctrl/set?key=value
```
```javascript
{
    "code": 0, // 0:OK; 
    "desc": "",
    "msg": ""
}
```

e.g.

```HTTP
GET /ctrl/set?video_system=PAL
GET /ctrl/set?contrast=95
```

### Clear setting
Depends on different firmware implementaion, the camera would be reboot.
```HTTP
GET /ctrl/set?action=clear
```

### Supported keys

#### Video
| key               | type    | description                         |
| :---              |:----    |:----                                |
| movfmt            | choice  | 4KP30/4KP60/...                     |
| record_file_format| choice  | MOV/MP4                             |
| rec_proxy_file    | choice  | Record the proxy file               |
| video_encoder     | choice  | h264/h265/..                        |
| split_duration    | choice  | video record split duration         |
| bitrate_level     | choice  | low/medium/high                     |
| compose_mode      | choice  | Normal/WDR                          |
| movvfr            | choice  | Variable Framerate                  |
| rec_fps           | choice  | Playback Framerate                  |
| video_tl_interval | range   | video timelapse interval            |
| enable_video_tl   | choice  | query is the camera support video timelapse  |
| rec_duration      | range   | query the recording duration, in second |
| last_file_name    | string  | query the last record file name     |

#### Focus & Zoom
| key               | type    | description                         |
| :---              |:----    |:----                                |
| focus             | choice  | AF/MF                               |
| af_mode           | choice  | Flexiable Zone/Human Detection      |
| mag_pos           | range   | magnify position                    |
| mf_drive          | range   | move the focus plane far/near       |
| lens_zoom         | choice  | lens zoom in/out                    |
| ois_mode          | choice  | lens ois mode                       |
| af_lock           | choice  | af lock/unlock                      |
| lens_zoom_pos     | range   | lens zoom position                  |
| lens_focus_pos    | range   | lens focus position                 |
| lens_focus_spd    | range   | control the speed of mf_drive/lens_focus_pos |
| caf               | choice  | Continious AF On/Off                     |
| caf_sens          | choice  | CAF sensitivity                          |
| live_caf          | choice  | On/Off                                   |
| mf_mag            | choice  | Magnify the preview if we tunning the MF |
| restore_lens_pos  | choice  | Restore lens position after reboot       |

#### Exposure
| key                   | type    | description                         |
| :---                  |:----    |:----                                |
| meter_mode            | choice  | AE meter mode                       |
| max_iso               | choice  | max iso                             |
| ev                    | range   | **deprecated**                      |
| ev_choice             | choice  | EV -3/.../0/.../3                   |
| iso                   | choice  | Auto/../200/../max iso              |
| iris                  | choice  | aperture                            |
| shutter_angle         | choice  | Auto/../45/90/../360                |
| max_exp_shutter_angle | choice  | max video shutter angle             |
| shutter_time          | choice  | shutter time                        |
| max_exp_shutter_time  | choice  | max video shutter time              |
| sht_operation         | choice  | speed/angle                         |
| dual_iso              | choice  | Auto/Low/High                       |
| ae_freeze             | choice  | ae lock/unlock                      |
| live_ae_fno           | choice  | live value of F-number, readonly    |
| live_ae_iso           | choice  | live value of ISO, readonly         |
| live_ae_shutter       | choice  | live value of shutter time, readonly|
| live_ae_shutter_angle | choice  | live value of shutter angle, readonly|

#### White Balance
| key               | type    | description                         |
| :---              |:----    |:----                                |
| wb                | choice  | Auto/Manual                         |
| mwb               | range   | manual white banlance kelvin        |
| tint              | range   | manual white banlance tint          |
| wb_priority       | choice  | Ambiance/White                      |

#### Image
| key               | type    | description                         |
| :---              |:----    |:----                                |
| sharpness         | choice  | Strong/Normal/Weak                  |
| contrast          | range   | contrast                            |
| saturation        | range   | saturation                          |
| brightness        | range   | brightness                          |
| lut               | choice  | rec709/zlog                         |
| luma_level        | choice  | 0-255/16-235                        |
| vignette          | choice  | **Not support in E2**               |

#### Stream
| key               | type    | description                         |
| :---              |:----    |:----                                |
| send_stream       | choice | stream0/stream1                      |

#### Audio
| key                   | type    | description                     |
| :---                  |:----    |:----                            |
| primary_audio         | choice  | AAC/PCM                         |
| audio_channel         | choice  | audio input channel             |
| audio_input_gain      | range   | audio input gain                |
| audio_output_gain     | range   | audio output gain               |
| audio_phantom_power   | choice  | On/Off                          |

#### Timecode
| key               | type    | description                         |
| :---              |:----    |:----                                |
| tc_count_up       | choice  | free run / record run               |
| tc_hdmi_dispaly   | choice  | Display TC on HDMI                  |
| tc_drop_frame     | choice  | DF/NDF                              |

#### Assist tool
| key                   | type   | description                      |
| :---                  |:----   |:----                             |
| assitool_display      | choice | On/Off                           |
| assitool_peak_onoff   | choice | On/Off                           |
| assitool_peak_color   | choice | peak color                       |
| assitool_exposure     | choice | Zebra/False Color                |
| assitool_zera_th1     | range  | Zebra high value                 |
| assitool_zera_th2     | range  | Zebra low value                  |

#### Misc
| key               | type    |description                          |
| :---              |:----    |:----                                |
| ssid              | string  | Wi-Fi SSID                          |
| flicker           | choice  | Flicker reduction, 50Hz/60Hz        |
| video_system      | choice  | NTSC/PAL/CINEMA                     |
| wifi              | choice  | On/Off                              |
| battery           | range   | battery percentage                  |
| led               | choice  | LED on/off                          |
| lcd_backlight     | range   | lcd backlignt level                 |
| hdmi_fmt          | choice  | Auto/4KP60/4KP30/...                |
| hdmi_osd          | choice  | HDMI On Screen Display              |
| usb_device_role   | choice  | Host/Mass storage/Network           |
| uart_role         | choice  | Pelco D/Controller                  |
| auto_off          | choice  | Camera auto off                     |
| auto_off_lcd      | choice  | LCD auto off                     |
| sn                | string  | serial number of the camera         |

##### Multiple Camera
| key               | type    |description                          |
| :---              |:----    |:----                                |
| multiple_mode     | choice  | single/master/slave                 |
| multiple_id       | range   | multiple camera id                  |

**Photo Settings are not supported in E2**

| key               | type    | description                         |
| :---              |:----    |:----                                |
| photosize         | choice  | photo resolution                    |
| photo_q           | choice  | JPEG/RAW                            |
| burst             | choice  |                                     |
| max_exp           | choice  | max exposure time                   |
| shoot_mode        | choice  | AE exposuer mode:P/A/S/M            |
| drive_mode        | choice  | single/burst/timelpase              |
| photo_tl_interval | range   | photo timelpase interval            |
| photo_tl_num      | range   | photo timelpase number              |
| photo_self_interval| range  | interval for selfie                 |



### Card manangement

Check if the card is present.
```HTTP
GET /ctrl/card?action=present
```

Format the storage card, the camera will determine the filesystem format base on card capacity.
```HTTP
GET /ctrl/card?action=format
```

Format the card to fat32/exfat
```HTTP
GET /ctrl/card?action=fat32
GET /ctrl/card?action=exfat
```

Query the space
```HTTP
GET /ctrl/card?action=query_free
GET /ctrl/card?action=query_total
```

### Manage the files in camera
The layout of the folder is based on DCF rule, but we change the naming rule.

To get files from the camera, you need to list out the folders first.

```HTTP
GET /DCIM/
```
```javascript
{
    "code": 0,
    "desc": "",
    "files": [
        "100ZCAME",
        "101ZCAME"
    ]
}
```

Then, you can list the files under the folder.
```HTTP
GET /DCIM/100ZCAME
```
```javascript
{
    "code": 0,
    "desc": "",
    "files": [
        "ZCAM0099_0000_201811121522.MOV",
        "ZCAM0100_0000_201811131522.MOV"
    ]
}
```

#### Download
```HTTP
GET /DCIM/110ZCAME/ZCAM0099_0000_201811121522.MOV
```
#### Delete
```HTTP
GET /DCIM/110ZCAME/ZCAM0099_0000_201811121522.MOV?act=rm
```

#### Get thumbnail
```HTTP
GET /DCIM/110ZCAME/ZCAM0100_0000_201811121522.MOV?act=thm
```
The response is JPEG data
#### Get screennail
```HTTP
GET /DCIM/110ZCAME/ZCAM0100_0000_201811121522.MOV?act=scr
```
Screennail is a larger JPEG than the thumbnail

#### Get create time of the file.
```HTTP
GET/DCIM/110ZCAME/ZCAM0101_0000_201811121539.MOV?act=ct 
```
```javascript
{
    "code": 0,
    "desc": "",
    "msg": 1542037145, //Unix timestamp
}
```

#### Get file information
You can get the duration and size of the video file.
```HTTP
GET /DCIM/110ZCAME/ZCAM0100_0000_201811121522.MOV?act=info
```
```javascript
{
	"code": 0,
	"desc": "",
	"msg": "",
	"w": 2880,   // video width
	"h": 2880,   // video height
	"vts": 30000, // video timescale
	"vcnt": 107,   // video packet count
	"dur": 107107 // video duration.
}
```


