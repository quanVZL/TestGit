#include "ShareMemoryClient.hpp"
#include <QTime>
#include <QtEndian>

ShareMemoryClient::ShareMemoryClient(QObject *parent) : QObject(parent) {
    m_shareMemory = std::unique_ptr<QSharedMemory>(new QSharedMemory);
    m_shareMemory->setKey("ShareData");
    m_shareMemory->create(200);
    m_writeTimer = startTimer(200);
}

void ShareMemoryClient::timerEvent(QTimerEvent *event) {
    if (event->timerId() == m_writeTimer) {
        dataChanged();

        killTimer(m_writeTimer);
    }
}

void ShareMemoryClient::setDefaultFrame(DataFrameInfo &settingInfo) {
    memset(&settingInfo, 0, SETTING_INFO_LENGTH);
    settingInfo.speed = mSpeed;
    settingInfo.rpm = mRpm;
    settingInfo.odo = mOdo;
    settingInfo.avgSpeed = mAvgSpeed;
    settingInfo.warnLightInfo.lightA = mLightA;
    settingInfo.warnLightInfo.lightB = mLightB;
    settingInfo.warnLightInfo.lightC = mLightC;
    settingInfo.warnLightInfo.lightD = mLightD;
    settingInfo.warnLightInfo.lightE = mLightE;
    settingInfo.reset = mReset;
}

void ShareMemoryClient::dataChanged() {
    DataFrameInfo specSettingInfo;

    mSpeed += rand() % 2;
    mRpm += rand() %  50;
    mRpm = mRpm % 8001;
    mOdo += rand() % 100;
    mAvgSpeed = 40;

    mLightA = rand() % 4;
    mLightB = !mLightB;
    mLightC = !mLightC;
    mLightD = rand() % 8;
    mLightE = !mLightE;

    mReset = rand() % 256;

    setDefaultFrame(specSettingInfo);

    writeShareData(QByteArray((char *)&specSettingInfo, SETTING_INFO_LENGTH));
}

void ShareMemoryClient::writeShareData(const QByteArray &byte) {
//    if (m_shareMemory->isAttached()) {
//        m_shareMemory->detach();
//    }

    qDebug() << QTime::currentTime().toString("yy-mm-dd hh:mm:ss.zzz") << mSpeed << ", rpm: " << mRpm
             << ", odo: " << mOdo << ", avg: " << mAvgSpeed << ", lightA: " << mLightA << ", lightB: " << mLightB
             << ", lightC: " << mLightC << ", lightD: " << mLightD << ", LightE: " << mLightE << ", reset: " << mReset;

    qWarning() << "byte: " << byte.toHex();

    QByteArray q1 = (QByteArray)qFromBigEndian(byte.data());

    QByteArray asd;
    asd = byte.toHex();
    asd = asd.left(16);
    bool ok;
    quint64 qqq = asd.toULongLong(&ok, 16);

    qWarning() << "qqq: " << qqq;


    QBuffer buffer;
    buffer.open(QBuffer::ReadWrite);
    QDataStream in(&buffer);
    in << byte;

//    if (!m_shareMemory->create(200)) {
//        qWarning() << "Create share memory failed, Error: " << m_shareMemory->errorString();
//    }

    m_shareMemory->lock();
    char *to = (char*)m_shareMemory->data();
    const char *from = buffer.data().data();
    memcpy(to, from, m_shareMemory->size());
    m_shareMemory->unlock();
}
