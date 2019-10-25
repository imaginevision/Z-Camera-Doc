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

[55] [54] [emulate key] <config key(4 Bytes)>

| command | example(HEX) |   back          |
| :---        | :---            |  :---            |
| power press | 55 54 0a 00 00 00 00 00 |  short press (1s > x <0.5s)switch to Playback mode |
| power release | 55 54 0b 00 00 00 00 00 | |
| return press | 55 54 01 00 00 00 00 00 |  enter/ok/play/pause |
| return release | 55 54 06 00 00 00 00 00 | |
| up press | 55 54 02 00 00 00 00 00 | select the prev file |
| up release | 55 54 07 00 00 00 00 00 | |
| down press | 55 54 03 00 00 00 00 00 | select the next file|
| down release | 55 54 08 00 00 00 00 00 | |


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


