#ifndef CAMERA_H
#define CAMERA_H

#include <QString>
#include <QSerialPort>
#include <memory>
#include <map>

#include "msgcodec.h"


class Camera
{
public:
    Camera();
    ~Camera();

    int connect(const QString & serialPort);
    void disconnect(void);
    bool isConnected(void) const { return connected_; }
    bool isRecording(void) const { return recording_; }
    bool isCrcEnable(void) const { return crc_enable_; }
    void setCrcEnable(bool enable);

    int getMode(void);
    int getStatus(void);
    int startRec(void);
    int stopRec(void);
    int formatSdCard(void);
    int clearSetting(void);
    int enableCRC(void);
    int disableCRC(void);

    int setConfig(struct camera_config * config);
    int getConfig(struct camera_config * config);

private:
    int sendCommonRequest(unsigned char *s, unsigned int len);
    int sendQueryRequest(unsigned char *s, unsigned int len, unsigned char *value);
    int sendRequest(const QByteArray &dat);
    int waitResponse(QByteArray &dat);

    bool recording_;
    bool connected_;
    bool crc_enable_;
    QSerialPort * serial_port_;

};

#endif // CAMERA_H
