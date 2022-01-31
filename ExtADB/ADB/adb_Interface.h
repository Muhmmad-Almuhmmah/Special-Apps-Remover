/***
 * -------------------------------------------------
#
# Project created by QtCreator 2022-02-20T21:31:19
# written By Muhammad Almuhmmah
# https://www.facebook.com/X.Dev.Ye/
#
#-------------------------------------------------
***/

#ifndef GLOBALE_ADB_H
#define GLOBALE_ADB_H

// https://stackoverflow.com/questions/38793887/qt-logging-tool-multithreading-calling-signal-and-slot-with-variable-number-of
#ifdef __cplusplus

#include <QObject>
#include <QMultiMap>
#include <QProcess>
#include <QFuture>
#include <QtConcurrent>
#include <QApplication>

typedef QMultiMap<QString, QString> PropMap;
typedef QMultiMap<QString, QString> adb_Devices;
typedef QMultiMap<QString, QString>::iterator MultiIteritor;

typedef struct{
    QString TheModel,Android,MSLSN,CSC_Code,CarrierID,
    IMEI,IMSI,MEID,MOTOID,
    Baseband,Brand;
}AndroidInfo;

class ADB : public QObject {
    Q_OBJECT
    PropMap props;
    adb_Devices devices;
    AndroidInfo Info;
public:
    QString Serial;
    static ADB *instance();
    QString Shell(const QString &cmd);
    bool LoadDevices();
    bool LoadProps();
    void Reboot(const QString &cmd=QString());
    static QString cmd(const QString &command);
    static QString cmd2(const QString &command,const QString &WorkPw);
    QString Exec(const QString &command);
    QString Exec2(const QString &command,const QString &WorkPw);
    PropMap GetProps();
    adb_Devices getDevices();
    AndroidInfo getInfo();
    void setInfo(const AndroidInfo &info);
    void Clear();
    bool Push(const QString &file, const QString &FileOnDevice);
    bool Pull(const QString &FileOnDevice, const QString &file);
    QString ShellSu(const QString &cmd);
};

#define GLOBALE_ADB_EXPORT extern "C"
#else
#define GLOBALE_ADB_EXPORT
#endif


void SetSerial(const QString &Serial);
QString GetSerial();
AndroidInfo GetAndroidInfo();
void SetAndroidInfo(const AndroidInfo&info);
QString DoShell(const QString &command);
QString DoShell2(const char * format,...);
QString DoShellSu(const QString &command);
QString GetPropValue(const QString &command);
PropMap GetPropsList();
PropMap GetDevicesList();
bool WaitForADBDevice();
bool PushFile(const QString &file,const QString &fileOnDevice);
bool PullFile(const QString &fileOnDevice,const QString &file);
#endif // GLOBALE_ADB_H
