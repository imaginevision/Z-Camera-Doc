
## Free-D Protocol

- only support stream mode
- data package is sent at 30fps
- for Z-Cam PTZ product only. for example: [P2-R1](https://www.z-cam.com/p2-r1/)
- [JS API](http/api.js)

### Start Free-D streaming in one command:
```
/ctrl/freed?enable=1&camera_id={id}&ip={ip}&port={port}
```

### Set camera id:
```
/ctrl/freed?camera_id={id}
 *id:[0~255]
```
### Set target UDP IP address and port
```
/ctrl/freed?ip={ip}&port={port}
 *port: [1~65535]
```
### Enable/Disable Free-d D1 message streaming
```
/ctrl/freed?enable=1
```
```
/ctrl/freed?enable=0
```
Response
```
{"code":0, "desc":"","msg":""}
```
```
{"code":-1,"desc":"","msg":""}
```

### Query Free-D Status:
```
/ctrl/freed
```
```
{
	"FreeD": "udp;stream mode only;",
	"camera_id": 01,
	"ip": "192.168.1.101",
	"port": 5001,
	"enable": 0,
	"transfer_mode": "always"
}
```

### Free-D D1 message format:
```
The D1 message is used for transferring the camera position and orientatdata from the free-d unit.
The message contains 29 bytes as follows.

		<D1>                Message type
        <CA>                Camera ID
        <PH><PM><PL>        Camera Pan Angle   (Yaw)        described in degree
        <TH><TM><TL>        Camera Tilt Angle  (Pitch)      described in degree
        <RH><RM><RL>        Camera Roll Angle  (Roll)       described in degree
        <XH><XM><XL>        Camera X-Position               described in millimeter
        <YH><YM><YL>        Camera Y-Position               described in millimeter
        <HH><HM><HL>        Camera Height (Z-Position)      described in millimeter
        <ZH><ZM><ZL>        Camera Zoom                     0x0000(WIDE) ~ 0x4000(TELE)
        <FH><FM><FL>        Camera Focus                    0xF000(NEAR) ~ 0x1000(FAR) 
        <SH><SL>            Iris and Seq. (16 bits)         FNo and a sequence number
        <CK>                Checksum

Note:
    Camera ID : [0~255]
    Pan Angle : [-175 ~ +175] degree
	Tilt Angle: [-30  ~ +90]  degree
	Roll,X,Y,Z: Always 0
	Zoom:  0x0000(WIDE) ~ 0x4000(TELE)
    Focus: 0xF000(NEAR) ~ 0x1000(FAR) 
    <SH><SL> Iris and Seq:
        Iris: bit 0~11;    F2.8->280->0x118 => 0x1, 0x18
        Seq:  bit 12~15;   mod(frame_seq). seq%16

```
