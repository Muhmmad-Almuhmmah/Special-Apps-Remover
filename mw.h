/***
 * -------------------------------------------------
#
# Project created by QtCreator 2022-02-20T21:31:19
# written By Muhammad Almuhmmah
# https://www.facebook.com/X.Dev.Ye/
#
#-------------------------------------------------
***/

#ifndef MW_H
#define MW_H

#include <QMainWindow>
#include <QTime>
#include <QSerialPort>
#include <QLabel>
#include <QProgressBar>
QT_BEGIN_NAMESPACE
namespace Ui { class MW; }
QT_END_NAMESPACE

#define HDR_MSG QString("<a href=\"https://github.com/Muhmmad-Almuhmmah/\" target=\"_blank\">GitHup</a> <a href=\"https://www.facebook.com/X.Dev.Ye\" target=\"_blank\">Facebook</a>\t|\t%1")
#define GetPercentge(value,total) (double(value)/total)*100
#define SpanColor(Text,Color) QString("<span style=\"color:%2;\"><b>%1</b></span><br>").arg(Text,Color)
#define SpanColor2(Text,Color) QString("<br><span style=\"color:%2;\"><b>%1</b></span>").arg(Text,Color)

class MW : public QMainWindow
{
    Q_OBJECT
    QLabel label;
    QProgressBar pBar;
public:
    MW(QWidget *parent = nullptr);
    ~MW();
    void Wait(int time);
    void SetEnable(bool enable);
    void Append(const QString &text);
    void Insert(const QString &text, bool success=1);
    void InsertLine2(const QString &text,bool success=1);
    void InsertLine(const QString &text, const QString &result);    
    bool WaitForADBDevice();
    bool RemoveApps();
    QStringList LoadListApps(QStringList &ids);
    QStringList ListPackages(const QString &packageCarrier);
    bool RemovePackage(const QString &App, const QString &Package);
    void MoveCursorToEnd();
private slots:
    void on_btnRemove_clicked();

private:
    Ui::MW *ui;
};
#endif // MW_H
