#include <QCoreApplication>
#include <QQuickView>
#include "ShareMemoryClient.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ShareMemoryClient client;

    qDebug() << "`";

    return a.exec();
}
