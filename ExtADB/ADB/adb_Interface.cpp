/***
 * -------------------------------------------------
#
# Project created by QtCreator 2022-02-20T21:31:19
# written By Muhammad Almuhmmah
# https://www.facebook.com/X.Dev.Ye/
#
#-------------------------------------------------
***/

#include "adb_interface.h"
#include <cstdarg>
#include <QDebug>
#include <QThread>
Q_GLOBAL_STATIC(ADB, Adb)

ADB * ADB::instance(){
    return Adb;
}

AndroidInfo ADB::getInfo()
{
    return Info;
}

void ADB::setInfo(const AndroidInfo &info)
{
    Info=info;
}

void ADB::Clear()
{
    props.clear();
    Serial.clear();
    devices.clear();
}

QString ADB::Shell(const QString &cmd)
{
    return Exec(QString("adb -s %1 shell %2").arg(Serial).arg(cmd));
}

QString ADB::ShellSu(const QString &cmd)
{
    return Exec(QString("adb -s %1 shell \"su -c \'%2\'\"").arg(Serial).arg(cmd));
}

bool ADB::Push(const QString &file,const QString &FileOnDevice)
{
    return Exec(QString("adb -s %1 push \"%2\" \"%3\"").arg(Serial).arg(file).arg(FileOnDevice)).contains("KB/s");
}

bool ADB::Pull(const QString &FileOnDevice,const QString &file)
{
    return Exec(QString("adb -s %1 pull \"%2\" \"%3\"").arg(Serial).arg(FileOnDevice).arg(file)).contains("");
}

#include <QMessageBox>
bool ADB::LoadDevices()
{
    QString lst=Exec("adb devices").split("List of devices attached").last();
    //    qDebug() <<"lst"<<lst;
    QStringList opt,opts;
    foreach(QString item,lst.split("\n"))
        if(item.contains("\tdevice")){
            item=item.remove("\tdevice").trimmed();
            Serial=item;
            devices.insert(item,Shell("getprop ro.product.model"));
            qDebug() <<devices;
        }else if(item.contains("unauthorized")){
            QMessageBox::warning(0,"ADB Auth","Please Accept ADB Auth :"+item.remove("unauthorized").remove("\t"));
        }
    return devices.count();
}

void ADB::Reboot(const QString &cmd)
{
    Shell("reboot "+cmd);
}

bool ADB::LoadProps()
{
    QString prps=Shell("getprop");
    QStringList list=prps.remove("\r").split("\n"),propArgs;
    if(list.isEmpty())
    {
        qDebug() <<"Fail to load props";
        return 0;
    }
    QString prop,value;
    foreach(QString item,list)
    {
        propArgs=item.split("]: [");
        if(propArgs.isEmpty())
            propArgs=item.split("] :[");
        if(propArgs.isEmpty())
            propArgs=item.split("] : [");
        if(propArgs.count()!=2)
            continue;
        prop=propArgs.first();
        value=propArgs.last();
        prop=prop.remove("[");
        value=value.remove("]");
        props.insert(prop,value);
        //        qDebug()<<"propArgs"<<propArgs<<props.lastKey();
    }
    return props.count();
}

PropMap ADB::GetProps()
{
    if(props.isEmpty())
        LoadProps();
    return props;
}

adb_Devices ADB::getDevices()
{
    return devices;
}

QString ADB::cmd(const QString &command)
{
    QProcess P2;
    P2.start(command);
    P2.waitForFinished(-1);
    P2.setReadChannel(QProcess::StandardOutput);
    QTextStream reade2(&P2);
    QString line2,line,Out;
    while (reade2.readLineInto(&line2))
        Out.append(line2 +'\n');
    P2.setReadChannel(QProcess::StandardError);
    QTextStream reader(&P2);
    while (reader.readLineInto(&line))
        Out.append(line +'\n');
    P2.close();
    return Out.trimmed();
}

QString ADB::cmd2(const QString &command, const QString &WorkPw)
{
    QProcess P2;
    P2.start(command);
    P2.setWorkingDirectory(WorkPw);
    P2.waitForFinished(-1);
    P2.setReadChannel(QProcess::StandardOutput);
    QTextStream reade2(&P2);
    QString line2,line,Out;
    while (reade2.readLineInto(&line2))
        Out.append(line2 +'\n');
    P2.setReadChannel(QProcess::StandardError);
    QTextStream reader(&P2);
    while (reader.readLineInto(&line))
        Out.append(line +'\n');
    P2.close();
    return Out.trimmed();
}

QString ADB::Exec(const QString &command)
{
    QFuture<QString> RunIt = QtConcurrent::run(cmd,command);
    while (!RunIt.isFinished()) {
        QApplication::processEvents();
    }

    qDebug() <<command<<RunIt.result();
    return RunIt.result();
}

QString ADB::Exec2(const QString &command, const QString &WorkPw)
{
    QFuture<QString> RunIt = QtConcurrent::run(cmd2,command,WorkPw);
    while (!RunIt.isFinished()) {
        QApplication::processEvents();
    }
    qDebug() <<command<<RunIt.result();
    return RunIt.result();
}

QString GetSerial(){
    return Adb->Serial;
}

AndroidInfo GetAndroidInfo(){
    return Adb->getInfo();
}

void SetAndroidInfo(const AndroidInfo&info){
    Adb->setInfo(info);
}

QString DoShell(const QString &command){
    return Adb->Shell(command);
}


QString DoShellSu(const QString &command){
    return Adb->ShellSu(command);
}

QString GetPropValue(const QString &command){
    return Adb->GetProps().find(command).value();
}

PropMap GetPropsList(){
    return Adb->GetProps();
}

QString DoShell2(const char * format,...)
{
    va_list argList;
    va_start(argList, format);
    auto msg = QString::vasprintf(format, argList);
    va_end(argList);
    return DoShell(msg);
}

PropMap GetDevicesList()
{
    return Adb->getDevices();
}

bool PushFile(const QString &file, const QString &fileOnDevice)
{
   return Adb->Push(file,fileOnDevice);
}

bool PullFile(const QString &fileOnDevice, const QString &file)
{
    return Adb->Pull(fileOnDevice,file);
}
