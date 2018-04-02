#ifndef QQ_SERVER_H
#define QQ_SERVER_H

#include <QDialog>
#include <QTime>
#include <QFile>
#include <QTcpServer>
#include <QTcpSocket>

namespace Ui {
class Server;
}

class Server : public QDialog
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = 0);
    ~Server();
    void InitSrv();
    void Refused();

protected:
    void closeEvent(QCloseEvent *);

private:
    Ui::Server *ui;
    qint16 tPort_;
    QTcpServer *tSrv_;
    QString file_name_;
    QString the_file_name_;
    QFile *loc_file_;
    qint64 total_bytes_;
    qint64 bytes_written_;
    qint64 bytes_tobe_write_;
    qint64 pay_load_size_;
    QByteArray out_block_;
    QTcpSocket *client_conn_;
    QTime time_;

private slots:
    void SendMsg();
    void UdpClientProgress(qint64 num_bytes);
    void on_open_btn__clicked();
    void on_send_btn__clicked();

    void on_close_btn__clicked();

signals:
    void SendFileName(QString file_name_);
};

#endif // QQ_SERVER_H
