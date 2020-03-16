## Lanc support command list

### Normal command

| command | example(HEX) |
| :---        | :---            |
| REC Start/Stop |  18 33 00 00 00 00 00 00 |


### Special command

| command | example(HEX) |
| :---        | :---             |
| zoom tele |  28 00 00 00 00 00 00 00 |
| zoom wide |  28 10 00 00 00 00 00 00 |
| AF/MF |  28 41 00 00 00 00 00 00 |
| focus |  28 43 00 00 00 00 00 00 |
| focus far|  28 45 00 00 00 00 00 00 |
| focus near|  28 47 00 00 00 00 00 00 |
| iris more close|  28 53 00 00 00 00 00 00 |
| iris more open|  28 55 00 00 00 00 00 00 |

### Custom command

#### Custom setConfig

Frame format:

[55] [53] [config key(2Bytes)] [value(4Bytes)]

supported config key reference

| command | example(HEX) |
| :---        | :---            |
| focus near(speed 1) |  55 53 00 21 00 00 00 01 |
| focus near(speed 2) |  55 53 00 21 00 00 00 02 |
| focus near(speed 3) |  55 53 00 21 00 00 00 03 |
| focus far(speed 1) |  55 53 00 21 ff ff ff ff |
| focus far(speed 2) |  55 53 00 21 ff ff ff fe |
| focus far(speed 3) |  55 53 00 21 ff ff ff fd |
| absolute focus     |  55 53 00 34 00 00 00 64 |


#### Custom emulate key

Frame format:

[55] [54] [emulate key]

| command | example(HEX) |   back          |
| :---        | :---            |  :---            |
| Power press | 55 54 0a 00 00 00 00 00 |  short press (1s > x <0.5s)switch to Playback mode |
| Power release | 55 54 0b 00 00 00 00 00 | |
| OK press | 55 54 01 00 00 00 00 00 |  enter/ok/play/pause |
| OK release | 55 54 06 00 00 00 00 00 | |
| Up press | 55 54 02 00 00 00 00 00 | up |
| Up release | 55 54 07 00 00 00 00 00 | |
| Down press | 55 54 03 00 00 00 00 00 | down |
| Down release | 55 54 08 00 00 00 00 00 | |
| Menu press | 55 54 15 00 00 00 00 00 | menu |
| Menu release | 55 54 16 00 00 00 00 00 | |
| Rec press | 55 54 17 00 00 00 00 00 | rec start/sop |
| Rec release | 55 54 18 00 00 00 00 00 | |
| F1 press | 55 54 00 00 00 00 00 00 | |
| F1 release | 55 54 05 00 00 00 00 00 | |
| F2 press | 55 54 0F 00 00 00 00 00 | |
| F2 release | 55 54 10 00 00 00 00 00 | |
| F3 press | 55 54 11 00 00 00 00 00 | |
| F3 release | 55 54 12 00 00 00 00 00 | |
| Fn press | 55 54 13 00 00 00 00 00 | Fn |
| Fn release | 55 54 14 00 00 00 00 00 | |


##### Config value up/down
Frame format:

[55] [54] [up/down (1 byte)] [config key (4 bytes)] [00]

0d : down

0e : up

| command | example(HEX) |
| :---        | :---            |
| ISO - | 55 54 0d 00 00 00 03 00 |
| ISO + | 55 54 0e 00 00 00 03 00 |
| shutter speed  - | 55 54 0d 00 00 00 64 00 |
| shutter speed  + | 55 54 0e 00 00 00 64 00 |
| iris/aperture - | 55 54 0d 00 00 00 15 00 |
| iris/aperture + | 55 54 0e 00 00 00 15 00 |
| eND - | 55 54 0d 00 00 00 b4 00 |
| eND + | 55 54 0e 00 00 00 b4 00 |

