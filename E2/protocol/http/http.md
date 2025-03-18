# Content

[Basic of API](#Basic)

[Get camera information](#Get-camera-information)

[Session](#Session)

[Set Data/Time](#Date/Time)

[System control](#System-control)

[Working mode](#Working-mode)

[Video record control](#Video-record-control)

[Network setting](#Network-setting)

[Network streaming](#Network-streaming)

[Camera settings](#Camera-settings)

[Focus & Zoom control](#Focus--Zoom-control)

[Magnify the preview](#Magnify-the-preview)

[Pan Tilt control](#Pan-Tilt-control)

[Card management](#Card-management)

[File management](#File-management)

[Asyc notification](#Asyc-notification)

[Firmware upgrade](#Firmware-upgrade)

[Examples](#Examples)

## Must read !!!

- Based on Z CAM E2 (firmware 0.93 or above)
- This document is not update to day, please check the /www/html/controller.html for the latest command and config key.

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

## Get camera information
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

## Date/Time
It's recommend that sync the date/time every time you connect to the camera.
```HTTP
GET /datetime?date=YYYY-MM-DD&time=hh:mm:ss
```

## NTP time
```HTTP
GET /ctrl/sntp?action=start&ip_addr=192.aa.bb.cc&port=123
GET /ctrl/sntp?action=stop
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
| standby   | standby                           |

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

Switch to standby
```HTTP
GET /ctrl/mode?action=to_standby
```

Any command to /ctrl/* will exit the standby mode.

Exit standby
```HTTP
GET /ctrl/mode?action=exit_standby
```

## Video record control
It will start video record/video timelapse record
```HTTP
GET /ctrl/rec?action=start
```

It will stop video record/video timelapse record
```HTTP
GET /ctrl/rec?action=stop
```

Query the maximun recording time, in minutes.
```HTTP
GET /ctrl/rec?action=remain
```

## Camera settings
You can control most of the settings in the camera, just like what you see in the camera's GUI.

There are three data types of the camera settings.

| Type              | Description                                                   |
| :---              |:----                                                          |
| choice            | Use the item from the option list                             |
| range             | Numberic value, from mininum value to maxinum value with step |
| string            | Text value                                                    |

You should get the data type and status(readonly or not) of the setting before you change it.

Each of the setting is bind to a key. 

### Get
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

### Set
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

### Clear
Depends on different firmware implementaion, the camera would be reboot.
```HTTP
GET /ctrl/set?action=clear
```

### Supported keys

#### Video
| key               | type    | description                         |
| :---              |:----    |:----                                |
| ~~movfmt~~        | choice  | 4KP30/4KP60/...                     |
| ~~resolution~~    | choice  | 4K/C4K/...                          |
| project_fps       | choice  | 23.98/24/...                        |
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
| mwb_r             | range   | manual white banlance r gain        |
| mwb_g             | range   | manual white banlance g gain        |
| mwb_b             | range   | manual white banlance b gain        |

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
| ain_gain_type         | choice  | AGC/MGC                         |

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
| auto_off_lcd      | choice  | LCD auto off                        |
| sn                | string  | serial number of the camera         |
| desqueeze         | choice  | desqueeze display:1x/1.33x/1.5x/2x  |

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

## Network setting
E2's Ethernet supports three IP modes:
- Router, work as DHCP client, get IP from your local router.

    ```HTTP
    GET /ctrl/network?action=set&mode=Router
    ```

- Direct, work as DHCP server, assin IP to the connected computer. IP is 10.98.32.1.

    ```HTTP
    GET /ctrl/network?action=set&mode=Direct
    ```

- Static

    ```HTTP
    GET /ctrl/network?action=set&mode=Static&ipaddr=192.168.1.100&netmask=255.255.255.0&gateway=192.168.1.1
    ```

Get infomation about the currently-selected IP mode:

```HTTP
GET /ctrl/network?action=query
```

Get infomation about the Ethernet and TCP/IP configuration

```HTTP
GET /ctrl/network?action=info
```

## Network streaming

In Z CAM E2, there are two streams could be used for streaming.

Stream 0

By default, it's used as the file recording in stroage card. The resolution and fps is controlled by 'movfmt' and 'movvfr'. The encoder is set by 'video_encoder' in /ctrl/set interface.

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

- SSP

  See [libssp](https://github.com/imaginevision/libssp)

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
| fps               | fps of the stream data            |
| venc              | video encoder                     |
| bitwidth          | bit width of the H.265            |

As metion above, by default, stream 1 is used by network streaming, you can change the setting of the stream.

Change the resolution
```HTTP
GET /ctrl/stream_setting?index=stream1&width=1920&height=1080
GET /ctrl/stream_setting?index=stream1&width=848&height=480
GET /ctrl/stream_setting?index=stream1&width=3840&height=2160
```
***The width/height must be 2 pixel alignment***

***You must take care of the aspect ratio***

***You must stop the streaming before you change the size***

Change the encoder
```HTTP
GET /ctrl/stream_setting?index=stream1&venc=h265
```

Change the bit width of H.265
```HTTP
GET /ctrl/stream_setting?index=stream1&bitwidth=10
GET /ctrl/stream_setting?index=stream1&bitwidth=8
```

Query the setting
```HTTP
GET /ctrl/stream_setting?action=query
```

### Change the bitrate on the fly
We don't need to stop the stream, if we only change the bitrate
```HTTP
GET /ctrl/stream_setting?index=stream1&bitrate=2000000
```

### Recommend stream settings

| Stream resolutin & fps    | File resolution & fps  | send_stream | Notes                                     |
|:--                        | :--                    | :--         | :--                                       |
| 4KP30 or smaller          | 4KP30                  | Stream1     | set movfmt=4KP30                          |
| 4KP30 or smaller          | 4KP60                  | Stream1     | set movfmt=4KP60                          |
| 4KP60                     | not supported          | Stream0     | fw <= 0.82, set movfmt=4KP60              |
| 4KP60                     | 4KP60                  | Stream1     | fw > 0.82, set movfmt=4KP60               |
| 4KP120                    | not supported          | Stream0     | fw > 0.82, set movfmt=4KP30, movfr=120    |

### Streaming 4KP30/1080P30/720P30
Check two item first:
1. make sure the movie format is 4KP30 and VFR is off.
    ```HTTP
    GET /ctrl/get?k=movfmt
    GET /ctrl/get?k=movvfr
    ```
    If not, use /ctrl/set to force it to 4KP30, VFR off.
    ```HTTP
    GET /ctrl/set?movfmt=4KP30
    GET /ctrl/set?movvfr=Off
    ```
2. the stream 1 is idle
    ```HTTP
    GET /ctrl/stream_setting?index=stream1&action=query
    ```

3840x2160, 40Mbps
```HTTP
GET /ctrl/stream_setting?index=stream1&width=3840&height=2160&bitrate=40000000
```

1920x1080, 10Mbps
```HTTP
GET /ctrl/stream_setting?index=stream1&width=1920&height=1080&bitrate=10000000
```

1280x720, 2Mbps
```HTTP
GET /ctrl/stream_setting?index=stream1&width=1280&height=720&bitrate=2000000
```

### Stream the 4KP60
Check two item first:
1. make sure the movie format is 4KP60 and VFR is off.
    ```HTTP
    GET /ctrl/get?k=movfmt
    GET /ctrl/get?k=movvfr
    ```
    If not, use /ctrl/set to force it to 4KP60, VFR off.
    ```HTTP
    GET /ctrl/set?movfmt=4KP60
    GET /ctrl/set?k=movvfr=Off
    ```
2. the stream 0 is idle (fw <= 0.82 )
    ```HTTP
    GET /ctrl/stream_setting?index=stream0&action=query
    ```
Firmware older than 0.82(included), stream out any fps higher than 30, we need to use the stream 0.
In this case, we can not record the stream to the file.
```HTTP
GET /ctrl/set?send_stream=Stream0
```

Firmware newer than 0.83(included), E2 support recording the 4KP60 file and stream out 4K60 at the same time.
```HTTP
GET /ctrl/stream_setting?index=stream1&width=3840&height=2160&bitrate=30000000
GET /ctrl/stream_setting?index=stream1&fps=60
```

Both of the above command is supported with 0.83 or later firmware, our suggestion is to use the method 1.

### Stream the 4KP120
Check two item first:
1. make sure the movie format is 4K and VFR is 120.
    ```HTTP
    GET /ctrl/get?k=movfmt
    GET /ctrl/get?k=movvfr
    ```

    If not, use /ctrl/set to force it to 4KP, VFR 120.
    ```HTTP
    GET /ctrl/set?movfmt=4KP30
    GET /ctrl/set?k=movvfr=120
    ```
2. the stream 0 is idle
    ```HTTP
    GET /ctrl/stream_setting?index=stream0&action=query
    ```
We need to use the stream 0 to send out the 120 fps stream. In this case, we can not record the stream to the file.
```HTTP
GET /ctrl/set?send_stream=Stream0
```

## Focus & Zoom control
### Auto Focus
Triger Auto Focus
```HTTP
GET /ctrl/af
```

Update the ROI of AF
```HTTP
GET /ctrl/af?action=update_roi&x=0&y=0&w=100&h=100
```
The [x,y,w,h] is normalized value scaled on 1000. 

For example, if the resolution is 3840x2160, then

ROI.X = (0/1000) * 3840 = 0

ROI.Y = (0/1000) * 2160 = 0

ROI.W = (100/1000) * 3840 = 384

ROI.H = (100/1000) *  = 216

You can update the center of the ROI in a simpler way
```HTTP
GET /ctrl/af?action=update_roi_center&x=500&y=500
```

Query the ROI of AF
```HTTP
GET /ctrl/af?action=query
```

### Manual Focus
When we set the focus method to manula focus. You can control the focus far/near.
```HTTP
GET /ctrl/set?mf_drive=1
GET /ctrl/set?mf_drive=2
GET /ctrl/set?mf_drive=3
GET /ctrl/set?mf_drive=-3
GET /ctrl/set?mf_drive=-2
GET /ctrl/set?mf_drive=-1
```

You can set the focus plane to a specific value.
```HTTP
GET /ctrl/set?lens_focus_pos=x
```

*Note: To control the focus manually with HTTP, the lens must be in the AF mode.*

```HTTP
GET /ctrl/set?focus=AF
```

### Zoom
#### Wide/Tele zoom
Zoom in
```HTTP
GET /ctrl/set?lens_zoom=in
```

Zoom out
```HTTP
GET /ctrl/set?lens_zoom=out
```

Stop zoom
```HTTP
GET /ctrl/set?lens_zoom=stop
```

#### Zoom to a posiiton
```HTTP
GET /ctrl/set?lens_zoom_pos=0
```
The range of the zoom position is 0-31


## Magnify the preview
If the camera is not recording (stream 0 is not running). You can magnify the preview data.

```HTTP
GET /ctrl/mag?action=enable
GET /ctrl/mag?action=disable
GET /ctrl/mag?action=query
```

## Pan Tilt control
If pan-tilt device is connect to the camera, the camera can control it via the Pelco-D protocal. We provide the HTTP command to control the pan-tilt.
```HTTP
GET /ctrl/pt?action=direction&speed=N
```
| action            | description                |
| :---              |:----                       |
| left              | pan left                   |
| right             | pan right                  |
| up                | tilt up                    |
| down              | tilt             |
| leftup            |   |
| leftdown          |   |
| rightup           |   |
| rightdown         |   |
| stop              | stop pan-tilt              |

The range of speed is 0-0x3f, larger value means faster speed.

**The UART role must be set to PelcoD to use this feature**

## Card management

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

## File management
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

### Download
```HTTP
GET /DCIM/110ZCAME/ZCAM0099_0000_201811121522.MOV
```
### Delete
```HTTP
GET /DCIM/110ZCAME/ZCAM0099_0000_201811121522.MOV?act=rm
```

### Get thumbnail
```HTTP
GET /DCIM/110ZCAME/ZCAM0100_0000_201811121522.MOV?act=thm
```
The response is JPEG data
### Get screennail
```HTTP
GET /DCIM/110ZCAME/ZCAM0100_0000_201811121522.MOV?act=scr
```
Screennail is a larger JPEG than the thumbnail

### Get create time of the file.
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

### Get file information
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

## Asyc notification
We use the websocket as the notification channel, check the source code on /www/html/controller.html to see how it work.

```
ws://host:81
```

## Firmware upgrade
You can do a firmware upgrade via HTTP.

1. Upload the firmware to camera.

```javascript
Upload.prototype.doUpload = function (args) {
    var that = this;
    var formData = new FormData();

    // add assoc key values, this will be posts values
    formData.append("file", this.file, this.getName());
    // formData.append("upload_file", true);

    $.ajax({
        type: "POST",
        url: "/uploadfirmware",
        xhr: function () {
            var myXhr = $.ajaxSettings.xhr();
            if (myXhr.upload) {
                myXhr.upload.addEventListener('progress', /*that.progressHandling*/function (event) {

                }, false);
            }
            return myXhr;
        },
        success: function (data) {
            // your callback here
            args.done();
        },
        error: function (error) {
            // handle error
        },
        async: true,
        data: formData,
        cache: false,
        contentType: false,
        processData: false,
        timeout: 60000
    });
};
```
2. check if the firmware is valid
```HTTP
GET /ctrl/upgrade?action=fw_check
```

3. do the upgrade.
```HTTP
GET /ctrl/upgrade?action=run
```

## Examples
For controlling the camera, we put a plain web page in /www/index.html to show how it work.

### Sequence to start controlling the camera
```HTTP
GET /info
GET /ctrl/session
GET /datetime?date=YYYY-MM-DD&time=hh:mm:ss
GET /ctrl/mode?action=query
GET /ctrl/get?k=iso
GET /ctrl/get?k=movfmt
```

Verify the session before any changes will help you to avoid multiple control client issue.

*Take /www/html/controller.html as your reference*

### Change the movie format
Movie format in firmware means the resolution and fps in the record file. Just like other /ctrl/set settings, we need to get first then set it later.
```HTTP
GET /ctrl/get?k=movfmt
```
Parse the JSON to find out what movie formats are supported, then set it to camera.
```HTTP
GET /ctrl/set?movfmt=4KP30
```

The supported list of movie format depends on many items:
- Video System (PAL/NTSC/CINEMA)
- WDR mode

**You can NOT change the movfmt & movvfr if the stream 0 is working!!!**

*Take /www/html/controller.html as your reference*

### Set the split duration to a shorter value
To avoid corrupted file, Z CAM E2 split the record file to many clips if it do a long duration recording.
Some of the users would like to use a very short split duration.

We consider the duration in playback domain.

If VFR is Off, it will split to a new file every 5 seconds(wall clock).
```HTTP
GET /ctrl/stream_setting?index=stream0&split=5
```

If VFR is on and playback frame is VFR, it will split to a new file every 5 seconds(wall clock).
```HTTP
GET /ctrl/stream_setting?index=stream0&split=5
```

If VFR is on, and playback frame is default. e.g. VFR is 120, movie format is 4K30. It will split to a new file every 20 seconds(wall clock).
```HTTP
GET /ctrl/stream_setting?index=stream0&split=5
```
## Knee/Slope based curve
It's used for some broadcast case, it could be adjusted by remote controller.

Set the image profile to 'customized curve 2', to activate Gamma/Black Level/Black Gamma/Knee features
```HTTP
GET /ctrl/set?lut=Customized 2
```

#### Gamma
Choose the base curve for your adjustment, options:
- Rec709
- sRGB
- Power of 2

Get infomations
```HTTP
GET /ctrl/cusomized_image_profile?action=get&option=gamma
```

Set base curve to Rec709
```HTTP
GET /ctrl/cusomized_image_profile?action=set&option=gamma&base=0
```

Set base curve to sRGB
```HTTP
GET /ctrl/cusomized_image_profile?action=set&option=gamma&base=1
```

Set base curve to  2^2.2
```HTTP
GET /ctrl/cusomized_image_profile?action=set&option=gamma&base=2&power=22
```

#### Black level
```HTTP
GET /ctrl/cusomized_image_profile?action=get&option=black_level
```

Set level
```HTTP
GET /ctrl/cusomized_image_profile?action=set&option=black_level&enable=1&level=10
```

#### Black gamma
```HTTP
GET /ctrl/cusomized_image_profile?action=get&option=black_gamma
```

Set range and level
```HTTP
GET /ctrl/cusomized_image_profile?action=set&option=black_gamma&enable=1&range=0&level=0
```

#### Knee
```HTTP
GET /ctrl/cusomized_image_profile?action=get&option=knee
```

Set point and slope
```HTTP
GET /ctrl/cusomized_image_profile?action=set&option=knee&enable=1&point=0&slope=10
```

### Manual black level and rgb gain

#### Manual black level adjustment

Run on the raw data, substract the dark current value.

Get status
```HTTP
GET /ctrl/manual_blc?action=get
```

```javascript
{
    "code":0,
    "desc":"",
    "msg":"",
    "enable":0,
    "r" :256,
    "gr":256,
    "gb":256,
    "b" :256
}
```

Enable
```HTTP
GET /ctrl/manual_blc?action=set&enable=1&rggb=r_value,gr_value,gb_value,b_value
```

Disable
```HTTP
GET /ctrl/manual_blc?action=set&enable=0&rggb=0,0,0,0
```

#### Set manual R/G/B gain
Set white balance mode to expert mode first.
```HTTP
GET /ctrl/set?wb=Expert
```

Take R channel as example, get manual R gain then set it later.
```HTTP
GET /ctrl/get?k=mwb_r
```

```javascript
{
    "code": 0,
    "desc": "string",
    "key": "mwb_r",
    "type": 2,
    "ro": 0,
    "value": 2467,  // current value
    "min": 1024,    // mininum value
    "max": 4096,    // maxinum value
    "step": 1
}
```

```HTTP
GET /ctrl/set?mwb_r=3000
```

For B channel, please use mwb_b.
