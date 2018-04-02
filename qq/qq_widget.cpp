#include "qq_widget.h"
#include "ui_qq_widget.h"
#include <QHostInfo>
#include <QMessageBox>
#include <QScrollBar>
#include <QDateTime>
#include <QNetworkInterface>
#include <QProcess>
#include <QFileDialog>
#include <QColorDialog>
#include "qq_server.h"
#include "qq_client.h"

Widget::Widget(QWidget *parent, QString user_name) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    u_name_ = user_name;
    udp_socket_ = new QUdpSocket(this);
    port_ = 23232;
    udp_socket_->bind(port_, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    connect(udp_socket_, SIGNAL(readyRead()), this, SLOT(ProcessPendingDatagrams()));
    SendMsg(UsrEnter);
    srv_ = new Server(this);
    connect(srv_, SIGNAL(SendFileName(QString)), this, SLOT(GetFileName(QString)));
    connect(ui->msg_txt_edit_, SIGNAL(currentCharFormatChanged(QTextCharFormat)), this, SLOT(CurrentFormatChanged(QTextCharFormat)));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::SendMsg(MsgType type, QString srv_addr)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    QString address = GetIp();
    out << type << GetUser();
    switch (type)
    {
    case Msg:
        if (ui->msg_txt_edit_->toPlainText() == "")
        {
            QMessageBox::warning(0, tr("Warning"), tr("Message must not be empty!"), QMessageBox::Ok);
            return;
        }
        out << address << GetMsg();
        ui->msg_txt_browser_->verticalScrollBar()->setValue(ui->msg_txt_browser_->verticalScrollBar()->maximum());
        break;
    case UsrEnter:
        out << address;
        break;
    case UsrLeft:
        break;
    case FileName:
    {
        int row = ui->usr_tbl_widget->currentRow();
        QString client_addr = ui->usr_tbl_widget->item(row, 1)->text();
        out << address << client_addr << file_name_;
        break;
    }
    case Refuse:
        out << srv_addr;
        break;
    }
    udp_socket_->writeDatagram(data, data.length(), QHostAddress::Broadcast, port_);
}

void Widget::ProcessPendingDatagrams()
{
    while (udp_socket_->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udp_socket_->pendingDatagramSize());
        udp_socket_->readDatagram(datagram.data(), datagram.size());
        QDataStream in(&datagram, QIODevice::ReadOnly);
        int msg_type;
        in >> msg_type;
        QString user_name, ip_address, msg;
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        switch (msg_type)
        {
        case Msg:
            in >> user_name >> ip_address >> msg;
            ui->msg_txt_browser_->setTextColor(Qt::blue);
            ui->msg_txt_browser_->setCurrentFont(QFont("Times New Roman", 12));
            ui->msg_txt_browser_->append("[ " + user_name + " ] " + time);
            ui->msg_txt_browser_->append(msg);
            break;
        case UsrEnter:
            in >> user_name >> ip_address;
            UserEnter(user_name, ip_address);
            break;
        case UsrLeft:
            in >> user_name;
            UserLeft(user_name, time);
            break;
        case FileName:
        {
            in >> user_name >> ip_address;
            QString client_addr, file_name;
            in >> client_addr >> file_name;
            HasPendingFile(user_name, ip_address, client_addr, file_name);
            break;
        }
        case Refuse:
            in >> user_name;
            QString srv_addr;
            in >> srv_addr;
            QString ip_addr = GetIp();
            if (ip_addr == srv_addr)
            {
                srv_->Refused();
            }
            break;
        }
    }
}

void Widget::UserEnter(QString user_name, QString ip_addr)
{
    bool isEmpty = ui->usr_tbl_widget->findItems(user_name, Qt::MatchExactly).isEmpty();
    if (isEmpty)
    {
        QTableWidgetItem *user = new QTableWidgetItem(user_name);
        QTableWidgetItem *ip = new QTableWidgetItem(ip_addr);
        ui->usr_tbl_widget->insertRow(0);
        ui->usr_tbl_widget->setItem(0, 0, user);
        ui->usr_tbl_widget->setItem(0, 1, ip);
        ui->msg_txt_browser_->setTextColor(Qt::gray);
        ui->msg_txt_browser_->setCurrentFont(QFont("Times New Roman", 10));;
        ui->msg_txt_browser_->append(tr("%1 is online").arg(user_name));
        ui->usr_num_lbl_->setText(tr("Online number: %1").arg(ui->usr_tbl_widget->rowCount()));
        SendMsg(UsrEnter);
    }
}

void Widget::UserLeft(QString user_name, QString time)
{
    int row_num = ui->usr_tbl_widget->findItems(user_name, Qt::MatchExactly).first()->row();
    ui->usr_tbl_widget->removeRow(row_num);
    ui->msg_txt_browser_->setTextColor(Qt::gray);
    ui->msg_txt_browser_->setCurrentFont(QFont("Times New Roman", 10));;
    ui->msg_txt_browser_->append(tr("%1 lfeft at %2!").arg(user_name).arg(time));
    ui->usr_num_lbl_->setText(tr("Online number: %1").arg(ui->usr_tbl_widget->rowCount()));
}

QString Widget::GetIp()
{
    QList <QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress addr, list)
    {
        if (addr.protocol() == QAbstractSocket::IPv4Protocol)
        {
            return addr.toString();
        }
    }
    return 0;
}

QString Widget::GetUser()
{
    return u_name_;
}

QString Widget::GetMsg()
{
    QString msg = ui->msg_txt_edit_->toHtml();
    ui->msg_txt_edit_->clear();
    ui->msg_txt_edit_->setFocus();
    return msg;
}

void Widget::on_send_btn__clicked()
{
    SendMsg(Msg);
}

void Widget::GetFileName(QString name)
{
    file_name_ = name;
    SendMsg(FileName);
}

void Widget::on_send_file_btn__clicked()
{
    if (ui->usr_tbl_widget->selectedItems().isEmpty())
    {
        QMessageBox::warning(0, tr("Select user"), tr("Please select target user!"), QMessageBox::Ok);
        return;
    }
    srv_->show();
    srv_->InitSrv();
}

void Widget::HasPendingFile(QString user_name, QString srv_addr, QString client_addr, QString file_name)
{
    QString ip_addr = GetIp();
    if (ip_addr == client_addr)
    {
        int btn = QMessageBox::information(this, tr("Receive file"), tr("File (%2) from %1: receive?").arg(srv_addr).arg(file_name), QMessageBox::Yes, QMessageBox::No);
        if (btn == QMessageBox::Yes)
        {
            QString name = QFileDialog::getSaveFileName(0, tr("Save file"), file_name);
            if (name.isEmpty())
            {
                Client *client = new Client(this);
                client->SetFileName(name);
                client->SetHostAddr(QHostAddress(srv_addr));
                client->show();
            }
        }
        else {
            SendMsg(Refuse, srv_addr);
        }
    }
}

void Client::SetFileName(QString name)
{
    loc_file_ = new QFile(name);
}

void Client::SetHostAddr(QHostAddress addr)
{
    host_addr_ = addr;
    NewConn();
}

void Widget::on_font_cbx__currentFontChanged(const QFont &f)
{
    ui->msg_txt_edit_->setCurrentFont(f);
    ui->msg_txt_edit_->setFocus();
}

void Widget::on_size_cbx__currentIndexChanged(const QString &arg1)
{
    ui->msg_txt_edit_->setFontPointSize(arg1.toDouble());
    ui->msg_txt_edit_->setFocus();
}

void Widget::on_bold_btn__clicked(bool checked)
{
    if (checked)
        ui->msg_txt_edit_->setFontWeight(QFont::Bold);
    else
        ui->msg_txt_edit_->setFontWeight(QFont::Normal);
    ui->msg_txt_edit_->setFocus();
}

void Widget::on_italic_btn__clicked(bool checked)
{
    ui->msg_txt_edit_->setFontItalic(checked);
    ui->msg_txt_edit_->setFocus();
}

void Widget::on_under_btn__clicked(bool checked)
{
    ui->msg_txt_edit_->setFontUnderline(checked);
    ui->msg_txt_edit_->setFocus();
}

void Widget::on_color_btn__clicked()
{
    color_ = QColorDialog::getColor(color_, this);
    if (color_.isValid())
    {
        ui->msg_txt_edit_->setTextColor(color_);
        ui->msg_txt_edit_->setFocus();
    }
}

void Widget::CurrentFormatChanged(const QTextCharFormat &fmt)
{
    ui->font_cbx_->setCurrentFont(fmt.font());
    if (fmt.fontPointSize() < 8)
    {
        ui->size_cbx_->setCurrentIndex(4);
    }
    else
    {
        ui->size_cbx_->setCurrentIndex(ui->size_cbx_->findText(QString::number(fmt.fontPointSize())));
    }
    ui->bold_btn_->setChecked(fmt.font().bold());
    ui->italic_btn_->setChecked(fmt.font().italic());
    ui->under_btn_->setChecked(fmt.font().underline());
    color_ = fmt.foreground().color();
}

void Widget::on_save_file_btn__clicked()
{
    if (ui->msg_txt_browser_->document()->isEmpty())
    {
        QMessageBox::warning(0, tr("Warning"), tr("Chat log is empty."), QMessageBox::Ok);
    }
    else
    {
        QString fname = QFileDialog::getSaveFileName(this, tr("Save chat log"), tr("Chat log"), tr("file(*.txt);;all file(*.*)"));
        if (!fname.isEmpty())
        {
            SaveFile(fname);
        }
    }
}

bool Widget::SaveFile(const QString &file_name)
{
    QFile file(file_name);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Save file"), tr("Can't save file %1:\n %2").arg(file_name).arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
    out << ui->msg_txt_browser_->toPlainText();
    return true;
}

void Widget::on_clear_btn__clicked()
{
    ui->msg_txt_browser_->clear();
}

void Widget::on_exit_btn__clicked()
{
    close();
}

void Widget::closeEvent(QCloseEvent *e)
{
    SendMsg(UsrLeft);
    QWidget::closeEvent(e);
}
