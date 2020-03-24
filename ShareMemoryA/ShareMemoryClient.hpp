#ifndef SHAREMEMORYCLIENT_HPP
#define SHAREMEMORYCLIENT_HPP

#include <QObject>
#include <QSharedMemory>
#include <memory>
#include <QTimerEvent>
#include <QBuffer>
#include <QDataStream>
#include <QDebug>

#include "ShareDataProtocol.hpp"

#define SHARE_MEMORY_SIZE       200

class ShareMemoryClient : public QObject
{
    Q_OBJECT
public:
    explicit ShareMemoryClient(QObject *parent = nullptr);

signals:

protected:
    virtual void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;

private:
    void setDefaultFrame(DataFrameInfo &settingInfo);
    void dataChanged();
    void writeShareData(const QByteArray &byte);

private:
    int m_writeTimer = 0;
    std::unique_ptr<QSharedMemory> m_shareMemory = nullptr;
    DataFrameInfo mCarStatus;
    quint8 mSpeed = 0;
    quint16 mRpm = 0;
    quint32 mOdo = 0;
    quint8 mAvgSpeed = 0;
    quint8 mLightA = 0;
    bool mLightB = false;
    bool mLightC = false;
    quint8 mLightD = 0;
    bool mLightE = false;
    quint8 mReset = 0;
};

#endif // SHAREMEMORYCLIENT_HPP
