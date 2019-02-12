#include "msgcodec.h"
#include "uart_def.h"
#include <string.h>
#include <crc16.h>

#pragma pack(1)
//ea 02 1a 8f 8f 00 01 00 00 54 00 00 00 42 00 5e 00 2e 00 5c 00 6f 00 6c 00 6a 00 07 00 04 00
typedef struct {
    short current_value;
    int nr_opt;
    short options[MAX_CAMERA_CONFIG_OPTIONS];
} FRAME_PAYLOAD_CHOICE;

typedef struct {
    int current_value;
    int max;
    int min;
    int step;
} FRAME_PAYLOAD_RANGE;

typedef struct {
    unsigned char sof;
    unsigned char opcode;
    unsigned char len;
    unsigned char cmd;
    unsigned char ack;
    unsigned char type;     /* CAMERA_CONFIG_TYPE_CHOICE or CAMERA_CONFIG_TYPE_RANGE */
    unsigned char payload[240];
} FRAME_RESPONSE;


typedef struct {
    unsigned char sof;
    unsigned char opcode;
    unsigned char len;
    unsigned char cmd;
    unsigned short crc16;
} FRAME_COMMON;

typedef struct {
    unsigned char sof;
    unsigned char opcode;
    unsigned char len;
    unsigned char cmd;
    unsigned char value;
    unsigned short crc16;
} FRAME_COMMON_SET;

typedef struct {
    unsigned char sof;
    unsigned char opcode;
    unsigned char len;
    unsigned char cmd;
    unsigned char key;
    unsigned short crc16;
} FRAME_GET_CONFIG;

typedef struct {
    unsigned char sof;
    unsigned char opcode;
    unsigned char len;
    unsigned char cmd;
    unsigned char key;
    unsigned char type;     /* CAMERA_CONFIG_TYPE_CHOICE or CAMERA_CONFIG_TYPE_RANGE */
    unsigned char payload[240];
} FRAME_SET_CONFIG;

#pragma pack()

#define BigLittleSwap16(A)        ((((unsigned short int)(A) & 0xff00) >> 8) | \
                                    (((unsigned short int)(A) & 0x00ff) << 8))


#define BigLittleSwap32(A)        ((((unsigned long int)(A) & 0xff000000) >> 24) | \
                                   (((unsigned long int)(A) & 0x00ff0000) >> 8) | \
                                   (((unsigned long int)(A) & 0x0000ff00) << 8) | \
                                   (((unsigned long int)(A) & 0x000000ff) << 24))

int encode_msg_common(unsigned char * buf, unsigned char cmd, unsigned char crc_enable)
{
    FRAME_COMMON *fr = (FRAME_COMMON *)buf;
    fr->sof = 0xEA;
    fr->opcode = 0x02;
    fr->len = 0x01;
    fr->cmd = cmd;
    if(crc_enable) {
        fr->len += 2;
        fr->crc16 = BigLittleSwap16(crc16(0xffff, buf, 4));
    }
    return fr->len + 3;
}


int encode_msg_common_set(unsigned char * buf, unsigned char cmd, unsigned char value, unsigned char crc_enable)
{
    FRAME_COMMON_SET *fr = (FRAME_COMMON_SET *)buf;
    fr->sof = 0xEA;
    fr->opcode = 0x02;
    fr->len = 0x02;
    fr->cmd = cmd;
    fr->value = value;
    if(crc_enable) {
        fr->len += 2;
        fr->crc16 = BigLittleSwap16(crc16(0xffff, buf, 5));
    }
    return fr->len + 3;
}
int encode_msg_get_mode(unsigned char * buf, int len, unsigned char crc_enable)
{
    return(encode_msg_common(buf, UART_GET_MODE, crc_enable));
}

int encode_msg_get_status(unsigned char * buf, int len, unsigned char crc_enable)
{
    return(encode_msg_common(buf, UART_GET_STATUS, crc_enable));
}

int encode_msg_start_rec(unsigned char * buf, int len, unsigned char crc_enable)
{
    return(encode_msg_common(buf, UART_START_REC, crc_enable));
}

int encode_msg_stop_rec(unsigned char * buf, int len, unsigned char crc_enable)
{
    return(encode_msg_common(buf, UART_STOP_REC, crc_enable));
}

int encode_msg_format(unsigned char * buf, int len, unsigned char crc_enable)
{
    return(encode_msg_common(buf, UART_FORMAT_CARD, crc_enable));
}

int encode_msg_clear_setting(unsigned char * buf, int len, unsigned char crc_enable)
{
    return(encode_msg_common(buf, UART_CLEAR_SETTING, crc_enable));
}

int encode_msg_crc_enable(unsigned char * buf, int len)
{
    return(encode_msg_common_set(buf, UART_CRC_VALID_CONFIG, 1, 0));
}

int encode_msg_crc_disable(unsigned char * buf, int len)
{
    return(encode_msg_common_set(buf, UART_CRC_VALID_CONFIG, 0, 1));
}

int encode_msg_get_camera_config(unsigned char * buf, int len, int key, unsigned char crc_enable)
{
    FRAME_GET_CONFIG *fr = (FRAME_GET_CONFIG *)buf;
    fr->sof = 0xEA;
    fr->opcode = 0x02;
    fr->len = 0x02;
    fr->cmd = UART_GET_CONFIG;
    fr->key = key;
    if(crc_enable) {
        fr->len += 2;
        fr->crc16 = BigLittleSwap16(crc16(0xffff, buf, 5));
    }
    return fr->len + 3;
}

int encode_msg_set_camera_config(unsigned char * buf, int len, struct camera_config * cfg, unsigned char crc_enable)
{
    FRAME_SET_CONFIG *fr = (FRAME_SET_CONFIG *)buf;
    fr->sof = 0xEA;
    fr->opcode = 0x02;
    //fr->len = 0x02;
    fr->cmd = UART_SET_CONFIG;
    fr->key = cfg->key;
    fr->type = cfg->type;

    if(fr->type == CAMERA_CONFIG_TYPE_CHOICE) {
        *(short *)fr->payload = BigLittleSwap16(cfg->u.choice.current_value);
        fr->len = 5;
    } else if(fr->type == CAMERA_CONFIG_TYPE_RANGE) {
        *(int *)fr->payload = BigLittleSwap32(cfg->u.range.current_value);
        fr->len = 7;
    } else if(fr->type == CAMERA_CONFIG_TYPE_STRING) {
        int len = strlen(cfg->u.string);
        memcpy(fr->payload, cfg->u.string, len);
        fr->len = 3+ len;
    }
    if(crc_enable) {
        fr->len += 2;
        unsigned short crc = crc16(0xffff, buf, fr->len + 3 -2);
        buf[fr->len + 3 - 2] = (crc >> 8) & 0xff;
        buf[fr->len + 3 - 1] = crc & 0xff;
    }

    return fr->len + 3;
}


bool checkCrc16(unsigned char *buf, unsigned char len)
{
    unsigned short crc16_sum, crc16_src;
    if((buf == NULL)||(len < 3))
        return false;

    crc16_sum = crc16(0xffff, buf, len-2);
    crc16_src = (buf[len-2] << 8) + buf[len-1];

    return (crc16_src == crc16_sum) ? true: false;
}

int decode_msg_camera_config(unsigned char * buf, int len, struct camera_config * cfg, unsigned char crc_enable)
{
    if(crc_enable) {
        if(false == checkCrc16(buf, len)) {
            return -1;
        }
    }
    // TODO: check if OK/NG
    FRAME_RESPONSE *res = (FRAME_RESPONSE *)buf;
    //cfg->key = res->cmd & 0x7f;
    cfg->type = res->type & 0x7f;
    cfg->ro =  (res->type & 0x80) ? 1 : 0;
    if(cfg->type == CAMERA_CONFIG_TYPE_CHOICE) {
        FRAME_PAYLOAD_CHOICE *ch;
        ch = (FRAME_PAYLOAD_CHOICE *) res->payload;
        cfg->u.choice.nr_opt = BigLittleSwap32(ch->nr_opt);
        if(cfg->u.choice.nr_opt > MAX_CAMERA_CONFIG_OPTIONS) {
            return -1;
        }
        cfg->u.choice.current_value = BigLittleSwap16(ch->current_value);
        memcpy(cfg->u.choice.options, ch->options, res->len - 5);

        for(int i=0; i<cfg->u.choice.nr_opt; i++){
            cfg->u.choice.options[i] = BigLittleSwap16(cfg->u.choice.options[i]);
        }
    }else if (cfg->type == CAMERA_CONFIG_TYPE_RANGE) {
        FRAME_PAYLOAD_RANGE *ra;
        ra = (FRAME_PAYLOAD_RANGE *) res->payload;
        cfg->u.range.current_value =  BigLittleSwap32(ra->current_value);
        cfg->u.range.min =  BigLittleSwap32(ra->min);
        cfg->u.range.max =  BigLittleSwap32(ra->max);
        cfg->u.range.step =  BigLittleSwap32(ra->step);
    }else if(cfg->type == CAMERA_CONFIG_TYPE_STRING) {
        memcpy(cfg->u.string, res->payload, strlen((const char*)res->payload)+1);
    }

    if(res->ack == 0)
        return 0;
    else
        return -1;
}

int decode_msg_common_response(unsigned char * buf, int len, unsigned char crc_enable)
{
    if(crc_enable) {
        if(false == checkCrc16(buf, len)) {
            return -1;
        }
    }
    FRAME_RESPONSE *res = (FRAME_RESPONSE *)buf;
    if(res->ack == 0)
        return 0;
    else
        return -1;
}

int decode_msg_query_response(unsigned char * buf, int len, unsigned char *value, unsigned char crc_enable)
{
    if(crc_enable) {
        if(false == checkCrc16(buf, len)) {
            return -1;
        }
    }
    FRAME_RESPONSE *res = (FRAME_RESPONSE *)buf;
    *value = res->type;
    if(res->ack == 0)
        return 0;
    else
        return -1;
}
