
## Controller send request to camera frame format (HEX, Big-Endian):

<table border="1" cellspacing="0">
    <tr>
        <th>sof  </th>
        <th>opcode  </th>
        <th>len </th>
        <th>payload </th>
   </tr>
   <tr>
        <td>EA  </td>
        <td>02  </td>
        <td>1 byte </td>
        <td>len bytes </td>
   </tr>
</table>

### Common request

| command            | example (HEX) | remark |
| :---               | :---                                  | :---             |
| switch to rec      | EA 02 01 02                           | cmd=02 |
| switch to still    | EA 02 01 03                           | cmd=03 |
| switch to pb       | EA 02 01 04                           | cmd=04 |
| start rec          | EA 02 01 05                           | cmd=05 |
| stop rec           | EA 02 01 06                           | cmd=06 |
| capture            | EA 02 01 07                           | cmd=07 |
| focus              | EA 02 01 09                           | cmd=09 |
| get config         | EA 02 03 2E key[15:0]                 | cmd=2E |
| set config (choise)| EA 02 06 2D key[15:0] 01 value[31:0]  | cmd=2D |
| set config (range) | EA 02 08 2D key[15:0] 02 value[31:0]  | cmd=2D |
| get battery        | EA 02 01 12                           | cmd=12 |
| format card        | EA 02 01 18                           | cmd=18 |
| shut down          | EA 02 01 25                           | cmd=25 |
| async msg enable   | EA 02 02 2A 01                        | cmd=2A |
| async msg disable  | EA 02 02 2A 00                        | cmd=2A |
| config value up    | EA 02 03 2B key[15:0]                 | cmd=2B |
| config value down  | EA 02 03 2C key[15:0]                 | cmd=2C |


### More references (config value up/down)
| command | example(HEX) |
| :---        | :---            |
| ISO -            | EA 02 03 2B 00 03 |
| ISO +            | EA 02 03 2C 00 03 |
| shutter speed  - | EA 02 03 2B 00 64 |
| shutter speed  + | EA 02 03 2C 00 64 |
| iris/aperture -  | EA 02 03 2B 00 15 |
| iris/aperture +  | EA 02 03 2C 00 15 |


### emulate key (cmd=1, payload=key status)

| command | example(HEX)     |
| :---    | :---             |
| F1 press      |  EA 02 02 01 00 |
| F1 release    |  EA 02 02 01 05 |
| OK press     |  EA 02 02 01 01 |
| OK release   |  EA 02 02 01 06 |
| Menu press     |  EA 02 02 01 15 |
| Menu release   |  EA 02 02 01 16 |
| UP press      |  EA 02 02 01 02 |
| UP release    |  EA 02 02 01 07 |
| DOWN press    |  EA 02 02 01 03 |
| DOWN release  |  EA 02 02 01 08 |


## Camera response to controller frame format(HEX):

<table border="1" cellspacing="0">
    <tr>
        <th>sof  </th>
        <th>opcode  </th>
        <th>len </th>
        <th>ack </th>
        <th>result + < payload > </th>
   </tr>
   <tr>
        <td>EA (1 byte)  </td>
        <td>02 (1 byte) </td>
        <td>1 byte </td>
        <td>80 | cmd (1 byte) </td>
        <td>len - 1 bytes </td>
   </tr>
</table>

### Common response

| result  | example(HEX) |
| :---    | :---             |
| OK      | EA 02 02 83 00   |
| NG      | EA 02 02 83 FF   |


### Response of get config type=choice

<table border="1" cellspacing="0">
    <tr>
        <td>EA  </td>
        <td>02  </td>
        <td>3B </td>
        <td>AE </td>
        <td>00 </td>
        <td>01 </td>
        <td>00 26 </td>
        <td>00 00 00 19 </td>
        <td>00 00 </td>
        <td>00 0F </td>
        <td>... </td>
        <td>00 26 </td>
   </tr>
   <tr>
        <td>sof  </td>
        <td>opcode  </td>
        <td>len </td>
        <td>80 | 2E </td>
        <td>OK </td>
        <td>type </td>
        <td>current value </td>
        <td>choice number </td>
        <td>choice 0 </td>
        <td>choice 1 </td>
        <td>... </td>
        <td>choice n </td>
   </tr>
</table>
* type[bit 7] read only flag, if type[bit 7]=1, read only.



### Response of get config type=range
<table border="1" cellspacing="0">
    <tr>
        <td>EA  </td>
        <td>02  </td>
        <td>13 </td>
        <td>AE </td>
        <td>00 </td>
        <td>82 </td>
        <td>00 00 00 32 </td>
        <td>00 00 00 64 </td>
        <td>00 00 00 00 </td>
        <td>00 00 00 01 </td>
   </tr>
   <tr>
        <td>sof  </td>
        <td>opcode  </td>
        <td>len </td>
        <td>80 | 2E </td>
        <td>OK </td>
        <td>type </td>
        <td>current value </td>
        <td>max value </td>
        <td>min value</td>
        <td>step</td>
   </tr>
</table>
* type[bit 7] read only flag, if type[bit 7]=1, read only.


## Camera notify event to controller frame format(HEX):

<table border="1" cellspacing="0">
    <tr>
        <th>sof  </th>
        <th>opcode  </th>
        <th>len </th>
        <th>cmd </th>
        <th>type </th>
        <th>what </th>
        <th>ext1 </th>
        <th>ext2 </th>
   </tr>
   <tr>
        <td>EA (1 byte)  </td>
        <td>02 (1 byte) </td>
        <td>11 </td>
        <td>29 </td>
        <td>4 byte </td>
        <td>4 bytes </td>
        <td>4 bytes </td>
        <td>4 bytes </td>
   </tr>
</table>
** if type = 0, event, type = 1, msg.




### For example:

<table border="1" cellspacing="0">
    <tr>
        <td>EA  </td>
        <td>02  </td>
        <td>11 </td>
        <td>29 </td>
        <td>00 00 00 00</td>
        <td>06 00 00 06 </td>
        <td>00 00 00 01 </td>
        <td>00 00 00 0C </td>
   </tr>
   <tr>
        <td>sof  </td>
        <td>opcode  </td>
        <td>len </td>
        <td>cmd </td>
        <td>type </td>
        <td>what </td>
        <td>ext1 </td>
        <td>ext2</td>
   </tr>
</table>

