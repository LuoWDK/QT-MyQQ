#ifndef QQ_WIDGET_H
#define QQ_WIDGET_H

#include <QWidget>
#include <QUdpSocket>
#include <QTextCharFormat>

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
    bool SaveFile(const QString &file_name);
    void closeEvent(QCloseEvent *);

private:
    Ui::Widget *ui;
    QUdpSocket *udp_socket_;
    quint16 port_;
    QString u_name_;
    QString file_name_;
    Server *srv_;
    QColor color_;

private slots:
    void ProcessPendingDatagrams();
    void on_send_btn__clicked();
    void GetFileName(QString);
    void on_send_file_btn__clicked();
    void on_font_cbx__currentFontChanged(const QFont &f);
    void on_size_cbx__currentIndexChanged(const QString &arg1);
    void on_bold_btn__clicked(bool checked);
    void on_italic_btn__clicked(bool checked);
    void on_under_btn__clicked(bool checked);
    void on_color_btn__clicked();
    void CurrentFormatChanged(const QTextCharFormat &fmt);
    void on_save_file_btn__clicked();
    void on_clear_btn__clicked();
    void on_exit_btn__clicked();
};

#endif // QQ_WIDGET_H
