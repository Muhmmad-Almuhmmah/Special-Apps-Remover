#include "mw.h"
#include "ui_mw.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MW w;
    w.show();
    return a.exec();
}


MW::MW(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MW)
{
    ui->setupUi(this);
    ui->statusbar->addWidget(&pBar);
    ui->statusbar->addWidget(&label);
    label.setText(HDR_MSG.arg(windowTitle()));
    pBar.setStyleSheet("text-align:center;background-color:white;");
    label.show();
    label.setOpenExternalLinks(true);
    ui->statusbar->addWidget(&label);

    InsertLine("Special Apps Remover (U.S.A Android Phones) (ADB ONLY)","Version 1.0");
    InsertLine("Supports deleting the applications","some American companies");
    InsertLine("#","Temporary delete (Without Root)");
    InsertLine("#","permanent delete (NEED Root)");

    InsertLine("companies supported by the first version","4");
    InsertLine("#1","Verzion");
    InsertLine("#2","Sprint");
    InsertLine("#3","AT&T");
    InsertLine("#4","Microsoft");
    InsertLine2("Supported Multi Brands {LG/Samsung/ZTE/ANS...}",0);
    InsertLine2("Only OneClick",0);

}

MW::~MW()
{
    delete ui;
}

void MW::Wait(int time)
{
    QTime dieTime = QTime::currentTime().addMSecs(time);
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

void MW::SetEnable(bool enable)
{
    if(!enable)
        ui->textBrowser->clear();
    ui->btnRemove->setEnabled(enable);
    ui->ch_UseSU->setEnabled(enable);
}
#include <QDebug>

void MW::MoveCursorToEnd()
{
    QTextCursor cursor = ui->textBrowser->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textBrowser->setTextCursor(cursor);
}

void MW::Append(const QString &text){
    ui->textBrowser->insertHtml(QString("<p>%1 ...</p>").arg(text));
}

void MW::Insert(const QString &text,bool success){
    ui->textBrowser->insertHtml(SpanColor(text,(success?"blue":"red")));
    MoveCursorToEnd();
}

void MW::InsertLine2(const QString &text, bool success){
    ui->textBrowser->insertHtml(SpanColor2(text,(success?"blue":"red")));
    MoveCursorToEnd();
}

void MW::InsertLine(const QString &text,const QString &result){
    if(result.isEmpty())
        return;
    Append(text);
    Insert(result);
}
#include "adb_Interface.h"

bool MW::WaitForADBDevice()
{
    ADB::instance()->Clear();
    Append("Wait For ADB Device");
    for(int i=0;i<120;i++){
        if(ADB::instance()->LoadDevices())
            break;
        pBar.setValue(GetPercentge(i,60));
        Wait(500);
    }
    pBar.setValue(100);
    if(!GetDevicesList().count()){
        Insert("FAILED",0);
        return 0;
    }
    Insert("DONE");
    return 1;
}

QStringList MW::ListPackages(const QString &packageCarrier){
    return DoShell("pm list package -s -f "+packageCarrier)
            .remove("package:")
            .remove("\r")
            .split("\n",QString::SkipEmptyParts);
}

bool MW::RemovePackage(const QString &App,const QString &Package){
    Append("Remove "+App);
    QString ret;
    if(ui->ch_UseSU->isChecked())
    {
        ret=DoShellSu("pm uninstall "+Package);
    }else{
        ret=DoShell("pm uninstall -k --user 0 "+Package);
    }
    Wait(300);
    if(!ret.contains("Success"))
    {
        Insert("FAILED",0);
        return 0;
    }
    Insert("DONE");
    return 1;
}
QStringList MW::LoadListApps(QStringList &ids){
    QString strComment;
    QStringList listApps,carrierApps;
    foreach(QString carrier,ids){
        carrierApps=ListPackages(carrier);
        if(carrier=="spr")
        {
            strComment="sprint";
            carrierApps<<ListPackages(strComment);
        }
        if(carrier=="vzw")
        {
            strComment="verizon";
            carrierApps<<ListPackages(strComment);
        }
        if(carrier=="att")
        {
            strComment="AT&T";
        }else{
            strComment=carrier;
        }
        if(carrierApps.isEmpty()){
            continue;
        }
        listApps<<carrierApps;
        Append("Detect "+strComment.toUpper()+" Apps");
        Insert(QString::number(carrierApps.count()));
        Wait(200);
    }
    return listApps;
}

bool MW::RemoveApps()
{
    QStringList listApps,Carriers,tempList;
    QString rel,strComment,package;;
    int apps=0;
    Carriers=QStringList(QStringList()<<"spr"<<"amazon"<<"vzw"<<"microsoft"<<"att");
    listApps=LoadListApps(Carriers);
    if(!listApps.isEmpty() && ui->ch_UseSU->isChecked()){
        InsertLine2("selected Remove Apps By Root!");
    }
    for(int i=0;i<listApps.count();i++)
    {
        strComment=QString(QString(listApps.at(i)).split("=").first()).split("/").last();
        package=QString(listApps.at(i)).split("=").last();
        if(tempList.contains(package))
            continue;
        tempList.append(package);
        if(RemovePackage(strComment,package)){
            apps++;
        }
        pBar.setValue(GetPercentge(i,listApps.count()));
    }
    apps ?  InsertLine2(QString("Operation Successfully Removeing Apps(%1)").arg(apps)):
            InsertLine2("Failed Detect Special Apps On Your Device",0);
    pBar.setValue(100);
    return apps;
}

void MW::on_btnRemove_clicked()
{
    SetEnable(0);
    if(WaitForADBDevice()){
        pBar.setValue(0);
        Append("Read Mobile Info");
        Insert("DONE");
        PropMap prop_map=GetPropsList();
        QString model,SDK,Manufacturer,simType,ChipName,Hardware,ProductName,EncryptionState,Phone_ESN ,DEVICE,PartFRP,Target_Operator,CPU,SKU,PDA,Android,EMMCSIZE,Security_Patch,Brand,Product_locale,SubscriptionMode;
        model = prop_map.value("ro.product.model",prop_map.value("ro.product.name"));
        Manufacturer = prop_map.value("ro.product.manufacturer");
        DEVICE = prop_map.value("ro.product.device");
        CPU = prop_map.value("ro.board.platform");
        SKU = prop_map.value("ro.boot.hardware.sku");
        Android = prop_map.value("ro.build.version.release");
        EMMCSIZE = prop_map.value("storage.mmc.size",prop_map.value("ro.emmc_size",""));
        SDK = prop_map.value("ro.build.version.sdk");
        Security_Patch = prop_map.value("ro.build.version.security_patch");
        Brand = prop_map.value("ro.product.brand");
        Product_locale = prop_map.value("ro.product.locale");
        SubscriptionMode = prop_map.value("ril.subscription.types");
        Target_Operator = prop_map.value("ro.build.target_operator");
        PartFRP = prop_map.value("ro.frp.pst");
        Phone_ESN = prop_map.value("ril.cdma.esn");
        EncryptionState = prop_map.value("ro.crypto.state");
        Hardware = prop_map.value("ro.hardware");

        InsertLine("Product Model",model);
        InsertLine("Brand",Brand);
        InsertLine("Android",Android);
        InsertLine("Manufacturer",Manufacturer);
        InsertLine("EMMC SIZE",EMMCSIZE);
        InsertLine("SKU",SKU);
        InsertLine("Security Patch",Security_Patch);
        InsertLine("SDK Version",SDK);
        InsertLine("Encryption State",EncryptionState);
        InsertLine("Protection FRP",QString(PartFRP.split("/").last()).toUpper());
        InsertLine("Hardware",Hardware);
        InsertLine("Phone ESN",Phone_ESN);
        InsertLine("Platform",CPU);
        InsertLine("Target Operator",Target_Operator);
        InsertLine("Chip Name",ChipName);
        InsertLine("SubscriptionMode",SubscriptionMode);
        InsertLine("Product locale",Product_locale);
        //        if(!Brand.contains("samsung") or !Brand.contains("lg")){
        //            InsertLine("Beta Phone "+model,0);
        //        }
        RemoveApps();
    }
    SetEnable(1);
}
