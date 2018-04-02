#include "qq_server.h"
#include "ui_qq_server.h"
#include <QFile>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QDataStream>

Server::Server(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Server)
{
    ui->setupUi(this);
    setFixedSize(400, 207);
    tPort_ = 5555;
    tSrv_ = new QTcpServer(this);
    connect(tSrv_, SIGNAL(newConnection()), this, SLOT(SendMsg()));
    InitSrv();
}

Server::~Server()
{
    delete ui;
}

void Server::InitSrv()
{
    pay_load_size_ = 64 * 1024;
    total_bytes_ = 0;
    bytes_written_ = 0;
    bytes_tobe_write_ = 0;
    ui->status_label_->setText(tr("Please select a file / folder!"));
    ui->progress_bar_->reset();
    ui->open_btn_->setEnabled(true);
    ui->send_btn_->setEnabled(false);
    tSrv_->close();
}

void Server::SendMsg()
{
    ui->send_btn_->setEnabled(false);
    client_conn_ = tSrv_->nextPendingConnection();
    connect(client_conn_, SIGNAL(bytesWritten(qint64)), this, SLOT(UdpClientProgress(qint64)));
    ui->status_label_->setText(tr("Sending %1!").arg(the_file_name_));
    loc_file_ = new QFile(file_name_);
    if (!loc_file_->open((QFile::ReadOnly)))
    {
        QMessageBox::warning(this, tr("Application"), tr("Can't read file %1:\n%2").arg(file_name_).arg(loc_file_->errorString()));
        return;
    }
    total_bytes_ = loc_file_->size();

    QDataStream send_out(&out_block_, QIODevice::WriteOnly);
    send_out.setVersion(QDataStream::Qt_5_9);
    time_.start();
    QString cur_file = file_name_.right(file_name_.size() - file_name_.lastIndexOf('/') - 1);
    send_out << qint64(0) << qint64(0) << cur_file;
    total_bytes_ += out_block_.size();
    send_out.device()->seek(0);
    send_out << total_bytes_ << qint64((out_block_.size() - sizeof(qint64) * 2));
    bytes_tobe_write_ = total_bytes_ - client_conn_->write(out_block_);
    out_block_.resize(0);
}

void Server::UdpClientProgress(qint64 num_bytes)
{
    qApp->processEvents();
    bytes_written_ += (int)num_bytes;
    if (bytes_tobe_write_ > 0)
    {
        out_block_ = loc_file_->read(qMin(bytes_tobe_write_, pay_load_size_));
        bytes_tobe_write_ -= (int)client_conn_->write(out_block_);
        out_block_.resize(0);
    }
    else
    {
        loc_file_->close();
    }
    ui->progress_bar_->setMaximum(total_bytes_);
    ui->progress_bar_->setValue(bytes_written_);
    float use_time = time_.elapsed();
    double speed = bytes_written_ / use_time;
    ui->status_label_->setText(tr("Sending %1MB (%2MB/s) \ntotal %3MB usaed: %4s\nremaining: %5s")
                               .arg(bytes_written_ / (1024*1024))
                               .arg(speed * 1000 / (1024*1024), 0, 'f', 2)
                               .arg(total_bytes_ / (1024*1024))
                               .arg(use_time / 1000, 0, 'f', 0)
                               .arg(total_bytes_/speed/1000 - use_time/1000, 0, 'f', 0));
    if (bytes_written_ == total_bytes_)
    {
        loc_file_->close();
        tSrv_->close();
        ui->status_label_->setText(tr("Send %1 successful!").arg(the_file_name_));
    }
}

void Server::on_open_btn__clicked()
{
    file_name_ = QFileDialog::getOpenFileName(this);
    if (!file_name_.isEmpty())
    {
        the_file_name_ = file_name_.right(file_name_.size() - file_name_.lastIndexOf('/') - 1);
        ui->status_label_->setText(tr("%1 will be sent.").arg(the_file_name_));
        ui->send_btn_->setEnabled(true);
        ui->open_btn_->setEnabled(false);
    }
}

void Server::on_send_btn__clicked()
{
    if (!tSrv_->listen(QHostAddress::Any, tPort_))
    {
        qDebug() << tSrv_->errorString();
        close();
        return;
    }
    ui->status_label_->setText(tr("Waiting for receiving..."));
    emit SendFileName(the_file_name_);
}

void Server::on_close_btn__clicked()
{
    if (tSrv_->isListening())
    {
        tSrv_->close();
        if (loc_file_->isOpen())
        {
            loc_file_->close();
        }
        client_conn_->abort();
    }
    close();
}

void Server::closeEvent(QCloseEvent *)
{
    on_close_btn__clicked();
}

void Server::Refused()
{
    tSrv_->close();
    ui->status_label_->setText(tr("Receive refused!"));
}
