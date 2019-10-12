#include "camera.h"
#include <QtDebug>
#include "msgcodec.h"

Camera::Camera()
    : connected_(false),
      recording_(false),
      crc_enable_(false)
{
}

Camera::~Camera()
{
    disconnect();
}

int Camera::connect(const QString & serialPort)
{
    serial_port_ = new QSerialPort();
    serial_port_->setPortName(serialPort);
    serial_port_->setBaudRate(QSerialPort::Baud115200);
    serial_port_->setDataBits(QSerialPort::Data8);
    serial_port_->setParity(QSerialPort::NoParity);
    serial_port_->setStopBits(QSerialPort::OneStop);
    serial_port_->setFlowControl(QSerialPort::NoFlowControl);

    if (serial_port_->open(QIODevice::ReadWrite)) {
        connected_ = true;
        return 0;
    } else {
        connected_ = false;
        return -1;
    }
}

void Camera::disconnect(void)
{
    if (connected_) {
        serial_port_->close();
    }
    connected_ = false;
}

void Camera::setCrcEnable(bool enable)
{
    crc_enable_ = enable;
}

int Camera::sendRequest(const QByteArray &data)
{
    if (!isConnected()) return -1;
//    qDebug() << ">>> " << data.toHex();
    while(serial_port_->waitForReadyRead(50)) {
    }

    return serial_port_->write(data);
}

int Camera::waitResponse(QByteArray &data)
{
    bool ret = serial_port_->waitForReadyRead(1000);
    if(ret) {
        data.append(serial_port_->readAll());

        while(serial_port_->waitForReadyRead(50)) {
            data.append(serial_port_->readAll());
        }

//        qDebug() << "<<< " << data.toHex();
    } else {
        qDebug() << "<<< no response!!!";
    }

    return data.size();
}

int Camera::sendCommonRequest(unsigned char *s,unsigned int len)
{
    QByteArray res;
    QByteArray a = QByteArray::fromRawData((const char *)s, len);
    sendRequest(a);

    if (waitResponse(res) <= 0) {
        return -1;
    }

    if(0 == decode_msg_common_response((unsigned  char*)res.data(), res.size(), crc_enable_)) {
//        qDebug() << "response OK !!!";
        return 0;
    } else {
//        qDebug() << "response NG !!!";
        return -1;
    }
}

int Camera::sendQueryRequest(unsigned char *s,unsigned int len,unsigned char *value)
{
    QByteArray res;
    QByteArray a = QByteArray::fromRawData((const char *)s, len);
    sendRequest(a);

    if (waitResponse(res) <= 0) {
        return -1;
    }

    if(0 == decode_msg_query_response((unsigned  char*)res.data(), res.size(), value, crc_enable_)) {
//        qDebug() << "response OK !!!";
        return 0;
    } else {
//        qDebug() << "response NG !!!";
        return -1;
    }
}



int Camera::getStatus(void)
{
    unsigned char buf[250];
    unsigned char value;
    unsigned int len = encode_msg_get_status(buf, sizeof(buf), crc_enable_);
    int ack = sendQueryRequest(buf, len, &value);
    if(value == UART_CAMERA_STATUS_RECORDING) {
        recording_ = true;
    } else {
        recording_ = false;
    }
    return ack;
}

int Camera::getMode(void)
{
    unsigned char buf[250];
    unsigned int len = encode_msg_get_mode(buf, sizeof(buf), crc_enable_);
    return(sendCommonRequest(buf, len));
}

int Camera::startRec(void)
{
    unsigned char buf[250];
    unsigned int len = encode_msg_start_rec(buf, sizeof(buf), crc_enable_);
    return(sendCommonRequest(buf, len));
}

int Camera::stopRec(void)
{
    unsigned char buf[250];
    unsigned int len = encode_msg_stop_rec(buf, sizeof(buf), crc_enable_);
    return(sendCommonRequest(buf, len));
}

int Camera::formatSdCard(void)
{
    unsigned char buf[250];
    unsigned int len = encode_msg_format(buf, sizeof(buf), crc_enable_);
    return(sendCommonRequest(buf, len));
}

int Camera::clearSetting(void)
{
    unsigned char buf[250];
    unsigned int len = encode_msg_clear_setting(buf, sizeof(buf), crc_enable_);
    return(sendCommonRequest(buf, len));
}

int Camera::getConfig(struct camera_config * config)
{
    if (!isConnected()) return -1;
    QByteArray res;
    unsigned char buf[250];
    unsigned int len = encode_msg_get_camera_config(buf, sizeof(buf), config->key, crc_enable_);
    QByteArray a = QByteArray::fromRawData((const char *)buf, len);

    sendRequest(a);
    if (waitResponse(res) <= 0) {
        return -1;
    }

    if(0 == decode_msg_camera_config((unsigned  char*)res.data(), res.size(), config, crc_enable_)) {
//        qDebug() << "response OK !!!";
        return 0;
    } else {
//        qDebug() << "response NG !!!";
        return -1;
    }
}

int Camera::setConfig(struct camera_config * config)
{
    if (!isConnected()) return -1;

    QByteArray res;
    unsigned char buf[250];
    unsigned int len = encode_msg_set_camera_config(buf, sizeof(buf), config, crc_enable_);
    QByteArray a = QByteArray::fromRawData((const char *)buf, len);

    sendRequest(a);
    if (waitResponse(res) <= 0) {
        return -1;
    }

    if(decode_msg_common_response((unsigned  char*)res.data(), res.size(), crc_enable_) == 0) {
//        qDebug() << "response OK !!!";
        return 0;
    } else {
//        qDebug() << "response NG !!!";
        return -1;
    }
}


int Camera::enableCRC(void)
{
    unsigned char buf[250];
    unsigned int len = encode_msg_crc_enable(buf, sizeof(buf));
    return(sendCommonRequest(buf, len));
}

int Camera::disableCRC(void)
{
    unsigned char buf[250];
    unsigned int len = encode_msg_crc_disable(buf, sizeof(buf));
    return(sendCommonRequest(buf, len));
}

