#include "ShareMemoryServer.hpp"
#include "unistd.h"

ShareMemoryServer::ShareMemoryServer(QObject *parent) : QObject(parent) {
    m_shareMemory = std::unique_ptr<QSharedMemory>(new QSharedMemory);
    m_shareMemory->setKey("ShareData");

    m_readTimer = startTimer(200);
}

void ShareMemoryServer::timerEvent(QTimerEvent *event) {
    if (event->timerId() == m_readTimer) {
        readData();
    }
}

void ShareMemoryServer::readData() {
    int i = 10;
    while(i > 0) {
        if (m_shareMemory->attach()) {
            break;
        } else {
            i--;
            qWarning() << QTime::currentTime().toString("yy-mm-dd hh:mm:ss.zzz") <<
                          "Unable to attach to share memory segment, " << m_shareMemory->error();
        }
        usleep(10 * 1000);
    }

    if (i <= 0) return;

    QBuffer buffer;
    QByteArray receivedData;
    QDataStream in(&buffer);

    m_shareMemory->lock();
    buffer.setData((char*)m_shareMemory->constData(), 200);
    buffer.open(QBuffer::ReadOnly);
    in >> receivedData;
    qWarning() << "recv: "<<  QTime::currentTime().toString("yy-mm-dd hh:mm:ss.zzz") << receivedData.toHex();
    m_shareMemory->unlock();
    m_shareMemory->detach();

    updateStatus(receivedData);
}

void ShareMemoryServer::updateStatus(QByteArray data) {
    mCarStatus = (DataFrame *)((uint8_t *)data.data());

    qWarning() << mCarStatus->speed << " "<< mCarStatus->rpm << " "<< mCarStatus->odo << " "<< mCarStatus->avgSpeed <<
                  ", lightA: " << mCarStatus->warnLightInfo.lightA << ", lightB: " << mCarStatus->warnLightInfo.lightB <<
                  ", lightC: " << mCarStatus->warnLightInfo.lightC << ", lightD: " << mCarStatus->warnLightInfo.lightD <<
                  ", lightE: " << mCarStatus->warnLightInfo.lightE << ", reset: " << mCarStatus->reset;
    this->test();
}

void ShareMemoryServer::test() {
    DataFrame carInfo;
    memcpy(&carInfo, mCarStatus, SETTING_INFO_LENGTH);

    qWarning() << "---------------- " << carInfo.speed << carInfo.rpm << carInfo.odo << carInfo.avgSpeed <<
                  carInfo.warnLightInfo.lightA << carInfo.warnLightInfo.lightB << carInfo.warnLightInfo.lightC <<
                  carInfo.warnLightInfo.lightD << carInfo.warnLightInfo.lightE << carInfo.reset;
}
