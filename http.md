### Content
+ [`/info`](#info)
+ [`session`](#session)
+ [`upgrade`](#upgrade)
+ [`shutdown`](#shutdown)
+ [`reboot`](#reboot)
+ [`datetime`](#datetime)
+ [`mode`](#mode)
+ [`Gallery interface`](#gallery)
+ [`still`](#still)
+ [`movie`](#recording)
+ [`setting`](#setting)
+ [`streaming`](#streaming)
+ [`magnify`](#mag)
+ [`AF`](#af)

### Genaral
The interface is base on HTTP, most of the request is GET. The format of response is JSON.<br>
The IP is 10.98.32.1, port is 80.


The format of request is：
	
	http://{ip}:{port}/*?param1={p1}[&parame2={p2}]

    
Response:
	
	{
        "code":0                <-- 0:OK, non 0: NG
        "desc":"demo"
        ... other data
    }

<a name="info"> </a>
### Query the basic info of camera.
/info, You can verify if the HTTP is ok or not.

	{
		"model":"Demo",			// model of camera
		"sw":"0.0.1"			// fw version
	}

### Get camera photo number in photo timelapse mode
/ctrl/timelapse_stat

### Get camera temperature
/ctrl/temperature

<a name="session"> </a>
### Session
/ctrl/session[?action=quit]

/ctrl/* interface is for controlling the camera. Only one client can control the camera at the same time. The client own the session can control the camera, otherwise, HTTP status code is 409.</br>


	action=quit, quick current session
	/ctrl/session withouw arg means heart beat.

** We suggest the App owns the session when it's in foreground**

<a name="upgrade"> </a>
### Upgrade
/uploadfirmware</br>
This interface will upload upgrade file to the camera using standard POST, the file will be placed into the root of SDCARD directory。

/ctrl/upgrade?action={action}

    action=ui_check， # Camera will check the file, and need user to confirm upgrade in LCD
	action=run， # Camera will check file and do upgrade but not display in LCD

<a name="shutdown"> </a>
### shutdown
/ctrl/shutdown</br>

<a name="reboot"> </a>
### reboot
/ctrl/reboot</br>

<a name="datetime"> </a>
### Date/time
/datetime?date={YYYY-MM-DD}&time={hh:mm:ss}</br>
Set the data/time of camera.

<a name="gallery"> </a>
### Gallery interface
#### list out the DCIM folder
/DCIM/

	{
		"code":0,
		"desc":"",
		"files":[
			"100MEDIA", "101MEDIA"
		]
	}
It means there are two folder under DCIM : 100MEDIA，101MEDIA.

#### list out the files under folder
/DCIM/100MEDIA/[?p=1&v=1]

	p=1, list out the photo，*.JPG
	v=1, list out the video, *.MOV

Response

	{
		"code":0,
		"desc":"",
		"files":[
			"ABCD0001.MOV", "ABCD0002.MOV"
		]
	}

#### Download file
/DCIM/100MEDIA/EYED0001.MOV</br>
Just like the file request in HTTP. Range operation is supported.

#### Delete file
/DCIM/100MEDIA/EYED0001.MOV?act=rm

#### Get video file info
/DCIM/100MEDIA/EYED0001.MOV?act=info

#### Get thumbnail
/DCIM/100MEDIA/EYED0001.MOV?act=thm
The format of thumbnail is BMP or JPEG, please check the MIME type in HTTP response.

### Get ScreenNail
/DCIM/100MEDIA/EYED0001.JPG?act=scr

### Get the file create time
/DCIM/100MEDIA/EYED0001.JPG?act=ct </br>
Response

	{
		"code":0,
		"desc":"",
		"msg":1479463832  # file create time，it's Unix timestamp
	}

### Get file MD5 value
/DCIM/100MEDIA/EYED0001.JPG?act=md5 </br>
Response

	{
		"code":0,
		"desc":"",
		"msg":"e729f0091f62d7e6437b0dbb5088816"  # MD5 value
	}

<a name="mode"> </a>
### Mode operation

Camera have three mode：

- Record
- Capture， // will have subpattern, e.g timelapse
- Playback

#### Query
/ctrl/mode?action=query 

Response

	{
		"code":0
		"desc":""
		"msg":"pb"              <-- pb, pb_ing, pb_paused,
									 cap, cap_tl_ing, cap_tl_idle, cap_idle,cap_burst
									 rec, rec_ing,
									 unknown

	}
	
#### Switch
	/ctrl/mode?action=to_pb, switch to playback mode
	/ctrl/mode?action=to_cap, switch to still mode
	/ctrl/mode?action=to_rec, switch to movie mode
	
Response

    {
        "code":0    <---- 0 is OK, other is NG
        "desc":""
        "msg":""
    }


<a name="still"> </a>
### Still capture
/ctrl/still?action={cap}

	action=cap # capture
	action=single # force to single capture
	action=remain # query the number of remaining capture number
	action=cancel_burst # if camera in burst mode, this interface will stop burst action
	
Response

    {
        "code":0     
        "desc":""
        "msg":"/DCIM/100MEDIA/ABC_0001.JPG"
    }

<a name="recording"> </a>
### Movie
/ctrl/rec?action={start}
 
	action=start
	action=stop
	action=remain
	
Response

    {
        "code":0     
        "desc":""
        "msg":"/DCIM/100MEDIA/ABC_0001.MOV"  
    }

<a name="setting"> </a>
### Setting
#### Qeury
/ctrl/get?k={key}</br>
There are three types for setting: string, option, range <br>
String:

	{
		"code":0,			 	// 0 means OK, otherwise is NG
		"desc":"desc...",		// 
		"key":"ssid",		// key
		"type":2,				// 2 means it's a string type
		"ro":0,					// 0 means changeable, otherwise is read only
		"value":"ABCDEFG",			// value
	}
Option:

	{
		"code":0,			 	// 0 means OK, otherwise is NG
		"desc":"desc...",		// 
		"key":"photosize",		// key
		"type":1,				// 1 means it's a option type.
		"ro":0,					// 0 means changeable, otherwise is read only
		"value":"17M",			// value
		"opts":["12M","8M","5M","3M","XGA","VGA","9M"] // valid options
	}
Range:

	{
		"code":0,			 	// 0 means OK, otherwise is NG
		"desc":"desc...",		// 
		"key":"iso",		// key
		"type":0,				// 0 means it's a range type.
		"ro":0,					// 0 means changeable, otherwise is read only
		"value":50,			// value
		"min":0,
		"max":100,
		"step":1
	}

Valid settings：

	ssid,  // string type，set Wi-Fi SSID
	ap_key， // string type，set Wi-Fi passwd
	movfmt,
	photosize，
	wb, // white balance
	iso,
    sharpness，
	contrast，
	saturation, 
	brightness，// image brightness
	meter_mode, 
	flicker, [auto, 50Hz, 60Hz]
	video_system, // NTSC, PAL
	video_putput, // CVBS out put
	ev, 
	battery，
	lcd, // turn on/off LCD
	rotation, // camera stream rotation
	mag_pos, // 
	focus，// [AF，MF]
	iris， // aperture
	af_mode，// [Normal, Flexible Zone]
	af_area，// query current af area
	mf_drive, // 
	photo_q，// Photo quality [basic, fine, s.fine]
	led, // control camera LED
	beep，// control camera beep
	max_exp， // Max exposure time
	shutter_angle,
	mwb, // manual white balance
	lens_zoom //
	lens_focus_pos
	lens_focus_spd
	shutter_spd, // shutter speed
    caf_range,  
    caf_sens,   // continue sensitivity
	burst_spd,  // burst speed
	lut,  // [sRGB, Z-LOG]
	last_file_name,  //
	camera_rot, // 
	multiple_mode, // [None, Master, slave]
	dewarp,  // Distortion correction
	vignette,  // Vignette correction
	noise_reduction,  // Noise reduction
	tint,
	file_number， 
	lcd_backlight, 
	hdmi_fmt, // HDMI output resolution
	oled， 
	multiple_id, 
	multiple_delay， 
	shoot_mode， // Capture mode, exposure mode，P/A/S/M
	ois_mode， // Lens OIS function
	split_duration， //
	multiple_to， // multiple timeout
	liveview_audio， // liveview with audio
	max_iso， 
	dhcp， // enable camera enable dhcpc
	Fn, // Fn key function
	auto_off, 
    auto_off_lcd, 
	photo_tl_num,  // photo timelapse mode, max capture number
    photo_tl_interval, 
	caf, // continue AF
	hdmi_osd, // turn on/off HDMI OSD
	F2, // F2 key function
	drive_mode, // [Single, Burst shooting, Time lapse, Self-timer]
	grid_display，// display grid in LCD 
	photo_self_interval, s
	focus_area， // Af area
	level_correction，
	WIFI_MODE， //switch wifi mode in ap/sta
	
	We will update the list later.

#### Change setting
We MUST read the setting before we set it, as we need to know the valid range/option first.</br>
/ctrl/set?{key}={value}</br>
/ctrl/set?photosize=9M

    {
    	"code":0                // 0 means OK, otherwise is NG
    	"desc":""
    }

#### Clear setting
/ctrl/set?action=clear

<a name="focus"></a>
### Focus method

	
	/ctrl/set?focus=AF # AF
	/ctrl/set?focus=MF # MF

When using AF, there two method for it.

* Center
* Flexiable

/ctrl/set?af_method={param}
	
	af_method=Normal
	af_method=Selection

<a name="af"> </a>
###AF
Camera focus area can be changed by http interface </br>
/ctrl/af?pos={index}

	pos=index, index is 32 bits int, high 16 bits is for X, low 16 bits is for Y.
	We mark the camera preview into X(Y) (X direction and Y direction) evenly by 16 in each line.
	When you mark a certain point X(Y) as 1, you set the point as your focus area.

    In the 16 x 16 matrix, X(Y),  all of the points you set as focus area should be in a rectangle and should be continuous. 

e.g.</br>
pos = ((0x6<<16) | 0x003c represents that (1,2) on the X direction and (2,3,4,5) on the Y direction consist the focus area (2x4=8 points in total).
	
	 |－－－－|－－－－|－－－－|－－－－|
	| o o o o o o o o o o o o o o o o |
	| o o o o o o o o o o o o o o o o |
 	| o x x o o o o o o o o o o o o o |
 	| o x x o o o o o o o o o o o o o |
 	| o x x o o o o o o o o o o o o o |
 	| o x x o o o o o o o o o o o o o |
 	| o o o o o o o o o o o o o o o o |
 	| o o o o o o o o o o o o o o o o |
 	| o o o o o o o o o o o o o o o o |
 	| o o o o o o o o o o o o o o o o |
 	| o o o o o o o o o o o o o o o o |
 	| o o o o o o o o o o o o o o o o |
 	| o o o o o o o o o o o o o o o o |
 	| o o o o o o o o o o o o o o o o |
 	| o o o o o o o o o o o o o o o o |
 	| o o o o o o o o o o o o o o o o |
 	-----------------------------------

When you are trying to realize this function by cell phone, note the picture ratio is different under video mode and still mode.
   Video is 16:9 and still is 4:3. The size of the area should be adjusted according to the ratio.

### Magnify
It's used for MF assistent. It's not avaiable for recording.

/ctrl/mag?action={enable}[&pos={position}]

	action=enable # enter magnify
	action=disable # exit magnify
	
	pos=position, high 16 bits is for X，low 16 bits is for Y.
	              It's a normalized value. 0－1000 means 0%-100%
	              e.g.
	              (0 << 16) | 0         top-left
	              (500 << 16) | 500     center
	              (1000 << 16) | 1000   bottom-right

### SDCARD
/ctrl/card?action={format}

	action=format, format card, default format FAT32
	action=present，if the card is present
	action=fat32,  format card to FAT32
	action=exfat,  format card to exFAT

<a name="streaming"> </a>
###Streaming
The server address is 10.98.32.1 and port is 9876. It's a TCP based server.<br>
Fetch one frame:<br>
1. send 0x01(1 byte) to server to request one frame.<br>
2. recv data (4 byte payload len + following the frame data)<br>
3. repeat the step1/step2 to get more frames.<br>

Data format in payload:<br>
Each payload of the data is H.264 encoded data. If the fps is 30, you can get 30 H.264 data in one second.

<a name="mag"> </a>

