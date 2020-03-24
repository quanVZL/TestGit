#ifndef SHAREMEMORYSERVER_HPP
#define SHAREMEMORYSERVER_HPP

#include <QObject>
#include <QSharedMemory>
#include <QTimerEvent>
#include <memory>
#include <QBuffer>
#include <QDataStream>
#include <QDebug>
#include "ShareDataProtocol.hpp"
#include <QTime>

class ShareMemoryServer : public QObject
{
    Q_OBJECT
public:
    explicit ShareMemoryServer(QObject *parent = nullptr);

signals:

protected:
    virtual void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;

private:
    void createShare();
    void readData();
    void updateStatus(QByteArray data);

    void test();

private:
    int m_readTimer = 0;
    std::unique_ptr<QSharedMemory> m_shareMemory = nullptr;
    DataFrame *mCarStatus;
};

#endif // SHAREMEMORYSERVER_HPP
