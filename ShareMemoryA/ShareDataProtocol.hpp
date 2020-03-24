#ifndef SHAREDATAPROTOCOL_H
#define SHAREDATAPROTOCOL_H

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
}__attribute__((packed)) DataFrameInfo;

#define SETTING_INFO_LENGTH (const int)(sizeof(DataFrameInfo))

#endif // SHAREDATAPROTOCOL_H
