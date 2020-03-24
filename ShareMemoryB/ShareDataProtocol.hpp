#ifndef SHAREDATAPROTOCOL_HPP
#define SHAREDATAPROTOCOL_HPP

#include <stdint.h>

typedef struct {
    quint8 lightA: 2,
        lightB: 1,
        lightC: 1,
        lightD: 3,
        lightE: 1;
}__attribute__((packed)) WarningLightList;

typedef struct {
    quint8 speed;
    quint16 rpm;
    quint32 odo: 24,
        avgSpeed: 8;
    WarningLightList warnLightInfo;
    quint8 reset;
}__attribute__((packed)) DataFrame;

#define SETTING_INFO_LENGTH (const int)(sizeof(DataFrame))

#endif // SHAREDATAPROTOCOL_HPP
