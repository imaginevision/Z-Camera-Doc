### Content
+ [`/info`](#info)
+ [`session`](#session)
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

<a name="session"> </a>
### Session
/ctrl/session[?action=quit]

/ctrl/* interface is for controlling the camera. Only one client can control the camera at the same time. The client own the session can control the camera, otherwise, HTTP status code is 409.</br>


	action=quit, quick current session
	/ctrl/session withouw arg means heart beat.

** We suggest the App owns the session when it's in foreground**

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

<a name="mode"> </a>
### Mode operation

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
        "code":0
        "desc":""
        "msg":"pb"              <-- one of [pb, cap, rec, unknown]
    }


<a name="still"> </a>
### Still capture
/ctrl/still?action={cap}

	action=cap # capture
	action=single # force to single capture
	action=remain # query the number of remaining capture number
	
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
There are three types for setting: string, option, range
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

	ssid
	ap_key
	movfmt
	photosize
	battery
	ev
	brightness
	saturation
	sharpness
	contrast
	meter_mode
	flicker，[auto, 50Hz, 60Hz]
	iso
	wb
	iris
	af_mode
	focus
	caf
	photo_q
	photo_tl
	burst
	led
	beep
	af_area
	drive_mode
	
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

<a name="mag"> </a>
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

	action=format, format card
	action=present，if the card is present
	

