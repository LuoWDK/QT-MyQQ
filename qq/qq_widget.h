#ifndef QQ_WIDGET_H
#define QQ_WIDGET_H

#include <QWidget>
#include <QUdpSocket>

class Server;

namespace Ui {
class Widget;
}
enum MsgType{Msg, UsrEnter, UsrLeft, FileName, Refuse};

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent, QString user_name);
    ~Widget();

protected:
    void UserEnter(QString user_name, QString ip_addr);
    void UserLeft(QString user_name, QString time);
    void SendMsg(MsgType type, QString srv_addr = "");
    QString GetIp();
    QString GetUser();
    QString GetMsg();
    void HasPendingFile(QString user_name, QString srv_addr, QString client_addr, QString file_name);

private:
    Ui::Widget *ui;
    QUdpSocket *udp_socket_;
    quint16 port_;
    QString u_name_;
    QString file_name_;
    Server *srv_;

private slots:
    void ProcessPendingDatagrams();
    void on_send_btn__clicked();
    void GetFileName(QString);
    void on_send_file_btn__clicked();
};

#endif // QQ_WIDGET_H
