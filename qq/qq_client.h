#ifndef QQ_CLIENT_H
#define QQ_CLIENT_H

#include <QDialog>
#include <QHostAddress>
#include <QFile>
#include <QTime>
#include <QTcpSocket>

namespace Ui {
class Client;
}

class Client : public QDialog
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = 0);
    ~Client();
    void SetHostAddr(QHostAddress addr);
    void SetFileName(QString name);

protected:
    void closeEvent(QCloseEvent *);

private:
    Ui::Client *ui;
    QTcpSocket *tClient_;
    quint16 block_size_;
    QHostAddress host_addr_;
    qint16 tport_;
    qint64 total_bytes_;
    qint64 bytes_received_;
    qint64 file_name_size_;
    QString file_name_;
    QFile *loc_file_;
    QByteArray in_block_;
    QTime time_;

private slots:
    void NewConn();
    void ReadMsg();
    void DisplayErr(QAbstractSocket::SocketError);
    void on_cancel_btn__clicked();
    void on_close_btn__clicked();
};

#endif // QQ_CLIENT_H
