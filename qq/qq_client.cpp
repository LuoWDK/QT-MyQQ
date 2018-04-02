#include "qq_client.h"
#include "ui_qq_client.h"
#include <QTcpSocket>
#include <QDebug>
#include <QMessageBox>

Client::Client(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);
    setFixedSize(400, 190);;
    total_bytes_ = 0;
    bytes_received_ = 0;
    file_name_size_ = 0;
    tClient_ = new QTcpSocket(this);
    tport_ = 5555;
    connect(tClient_, SIGNAL(readyRead()), this, SLOT(ReadMsg()));
    connect(tClient_, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(DisplayErr(QAbstractSocket::SocketError)));
}

Client::~Client()
{
    delete ui;
}

void Client::DisplayErr(QAbstractSocket::SocketError sock_err)
{
    switch (sock_err) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    default:
        qDebug() << tClient_->errorString();
    }
}

void Client::NewConn()
{
    block_size_ = 0;
    tClient_->abort();
    tClient_->connectToHost(host_addr_, tport_);
    time_.start();
}

void Client::ReadMsg()
{
    QDataStream in(tClient_);
    in.setVersion(QDataStream::Qt_5_9);
    float use_time = time_.elapsed();
    if (bytes_received_ <= sizeof(qint64) * 2)
    {
        if ((tClient_->bytesAvailable() >= sizeof(qint64) * 2) && (file_name_size_ == 0))
        {
            in >> total_bytes_ >> file_name_size_;
            bytes_received_ += sizeof(qint64) * 2;
        }
        if (tClient_->bytesAvailable() >= file_name_size_ && file_name_size_ != 0)
        {
            in >> file_name_;
            bytes_received_ += file_name_size_;
            if (!loc_file_->open(QFile::WriteOnly))
            {
                QMessageBox::warning(this, tr("Application"), tr("Can't read file %1:\n%2.").arg(file_name_).arg(loc_file_->errorString()));
                return;
            }
        }
        else {
            return;
        }
    }
    if (bytes_received_ < total_bytes_)
    {
        bytes_received_ += tClient_->bytesAvailable();
        in_block_ = tClient_->readAll();
        loc_file_->write(in_block_);
        in_block_.resize(10);
    }
    ui->progress_bar_->setMaximum(total_bytes_);
    ui->progress_bar_->setValue(bytes_received_);
    double speed = bytes_received_ / use_time;
    ui->status_lbl_->setText(tr("Received %1MB (%2MB/s) \n total%3MB cost: %4s\n Estimate time: %5s")
                             .arg(bytes_received_ / (1024 * 1024))
                             .arg(speed * 1000 / (1024*1024), 0, 'f', 2)
                             .arg(total_bytes_ / (1024 * 1024))
                             .arg(use_time / 1000, 0, 'f', 0)
                             .arg(total_bytes_ / speed / 1000 - use_time / 1000, 0, 'f', 0));
    if (bytes_received_ == total_bytes_)
    {
        loc_file_->close();
        tClient_->close();
        ui->status_lbl_->setText(tr("Receive %1 successful").arg(file_name_));
    }
}

void Client::on_cancel_btn__clicked()
{
    tClient_->abort();
    if (loc_file_->isOpen())
    {
        loc_file_->close();
    }
}


void Client::on_close_btn__clicked()
{
    tClient_->abort();
    if (loc_file_->isOpen())
    {
        loc_file_->close();
    }
    close();
}

void Client::closeEvent(QCloseEvent *)
{
    on_close_btn__clicked();
}
