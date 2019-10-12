#ifndef MSGCODEC_H
#define MSGCODEC_H

#include "uart_def.h"

#define MAX_CAMERA_CONFIG_OPTIONS       (64)
#define MAX_CAMERA_CONFIG_STRING_LEN MAX_CAMERA_CONFIG_OPTIONS

/* predefined choice */
struct camera_config_choice {
    short current_value;
    unsigned int nr_opt;
    short options[MAX_CAMERA_CONFIG_OPTIONS];
};
/* range value */
struct camera_config_range {
    int current_value;
    int min;
    int max;
    int step;
};
struct camera_config {
    unsigned int key;
    unsigned char type;     /* CAMERA_CONFIG_TYPE_CHOICE or CAMERA_CONFIG_TYPE_RANGE */
    unsigned char ro;       // ro == 1 -> readonly
    union {
        struct camera_config_choice choice;
        struct camera_config_range range;
        char string[MAX_CAMERA_CONFIG_STRING_LEN];
    } u;
};

int encode_msg_get_mode(unsigned char * buf, int len, unsigned char crc_enable);
int encode_msg_get_status(unsigned char * buf, int len, unsigned char crc_enable);
int encode_msg_start_rec(unsigned char * buf, int len, unsigned char crc_enable);
int encode_msg_stop_rec(unsigned char * buf, int len, unsigned char crc_enable);
int encode_msg_format(unsigned char * buf, int len, unsigned char crc_enable);
int encode_msg_clear_setting(unsigned char * buf, int len, unsigned char crc_enable);
int encode_msg_get_camera_config(unsigned char * buf, int len, int key, unsigned char crc_enable);
int encode_msg_set_camera_config(unsigned char * buf, int len, struct camera_config * cfg, unsigned char crc_enable);
int encode_msg_crc_enable(unsigned char * buf, int len);
int encode_msg_crc_disable(unsigned char * buf, int len);
// decode
int decode_msg_camera_config(unsigned char * buf, int len, struct camera_config * cfg, unsigned char crc_enable);
int decode_msg_common_response(unsigned char * buf, int len, unsigned char crc_enable);
int decode_msg_query_response(unsigned char * buf, int len, unsigned char *value, unsigned char crc_enable);


#endif // MSGCODEC_H

