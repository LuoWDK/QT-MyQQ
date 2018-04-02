#include "qq_drawer.h"
#include <QGroupBox>
#include <QVBoxLayout>

Drawer::Drawer(QWidget *parent, Qt::WindowFlags f)
    : QToolBox(parent, f)
{
    setWindowTitle(tr("MyQQ 2018"));
    setWindowIcon(QPixmap(":/images/qq-icon.png"));

    tool_btn_1_ = new QToolButton;
    tool_btn_1_->setText(tr("Bat"));
    tool_btn_1_->setIcon(QPixmap(":/images/Bat-icon.png"));
//    tool_btn_1_->setIconSize(QPixmap(":/images/Bat-icon.png").size());
    tool_btn_1_->setAutoRaise(true);
    tool_btn_1_->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(tool_btn_1_, SIGNAL(clicked()), this, SLOT(ShowChatWidget1()));

    tool_btn_2_ = new QToolButton;
    tool_btn_2_->setText(tr("Capt. Spaulding"));
    tool_btn_2_->setIcon(QPixmap(":/images/Capt-Spaulding-icon.png"));
//    tool_btn_2_->setIconSize(QPixmap(":/images/Capt-Spaulding-icon.png").size());
    tool_btn_2_->setAutoRaise(true);
    tool_btn_2_->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(tool_btn_2_, SIGNAL(clicked()), this, SLOT(ShowChatWidget2()));

    tool_btn_3_ = new QToolButton;
    tool_btn_3_->setText(tr("Casper"));
    tool_btn_3_->setIcon(QPixmap(":/images/Casper-icon.png"));
//    tool_btn_3_->setIconSize(QPixmap(":/images/Casper-icon.png").size());
    tool_btn_3_->setAutoRaise(true);
    tool_btn_3_->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(tool_btn_3_, SIGNAL(clicked()), this, SLOT(ShowChatWidget3()));

    tool_btn_4_ = new QToolButton;
    tool_btn_4_->setText(tr("Chuckie"));
    tool_btn_4_->setIcon(QPixmap(":/images/Chuckie-icon.png"));
//    tool_btn_4_->setIconSize(QPixmap(":/images/Chuckie-icon.png").size());
    tool_btn_4_->setAutoRaise(true);
    tool_btn_4_->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(tool_btn_4_, SIGNAL(clicked()), this, SLOT(ShowChatWidget4()));

    tool_btn_5_ = new QToolButton;
    tool_btn_5_->setText(tr("Dave"));
    tool_btn_5_->setIcon(QPixmap(":/images/Dave-icon.png"));
//    tool_btn_5_->setIconSize(QPixmap(":/images/Dave-icon.png").size());
    tool_btn_5_->setAutoRaise(true);
    tool_btn_5_->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(tool_btn_5_, SIGNAL(clicked()), this, SLOT(ShowChatWidget5()));

    tool_btn_6_ = new QToolButton;
    tool_btn_6_->setText(tr("Devil"));
    tool_btn_6_->setIcon(QPixmap(":/images/Devil-icon.png"));
//    tool_btn_6_->setIconSize(QPixmap(":/images/Devil-icon.png").size());
    tool_btn_6_->setAutoRaise(true);
    tool_btn_6_->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(tool_btn_6_, SIGNAL(clicked()), this, SLOT(ShowChatWidget6()));

    tool_btn_7_ = new QToolButton;
    tool_btn_7_->setText(tr("Diablo"));
    tool_btn_7_->setIcon(QPixmap(":/images/Diablo-icon.png"));
//    tool_btn_7_->setIconSize(QPixmap(":/images/Diablo-icon.png").size());
    tool_btn_7_->setAutoRaise(true);
    tool_btn_7_->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(tool_btn_7_, SIGNAL(clicked()), this, SLOT(ShowChatWidget7()));

    tool_btn_8_ = new QToolButton;
    tool_btn_8_->setText(tr("Dracula"));
    tool_btn_8_->setIcon(QPixmap(":/images/Dracula-icon.png"));
//    tool_btn_8_->setIconSize(QPixmap(":/images/Dracula-icon.png").size());
    tool_btn_8_->setAutoRaise(true);
    tool_btn_8_->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(tool_btn_8_, SIGNAL(clicked()), this, SLOT(ShowChatWidget8()));

    tool_btn_9_ = new QToolButton;
    tool_btn_9_->setText(tr("Frankenstein"));
    tool_btn_9_->setIcon(QPixmap(":/images/Frankenstein-icon.png"));
//    tool_btn_9_->setIconSize(QPixmap(":/images/Frankenstein-icon.png").size());
    tool_btn_9_->setAutoRaise(true);
    tool_btn_9_->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(tool_btn_9_, SIGNAL(clicked()), this, SLOT(ShowChatWidget9()));

    QGroupBox *group_box = new QGroupBox;
    QVBoxLayout *box_layout = new QVBoxLayout(group_box);

    box_layout->setMargin(20);;
    box_layout->setAlignment(Qt::AlignLeft);
    box_layout->addWidget(tool_btn_1_);
    box_layout->addWidget(tool_btn_2_);
    box_layout->addWidget(tool_btn_3_);
    box_layout->addWidget(tool_btn_4_);
    box_layout->addWidget(tool_btn_5_);
    box_layout->addWidget(tool_btn_6_);
    box_layout->addWidget(tool_btn_7_);
    box_layout->addWidget(tool_btn_8_);
    box_layout->addWidget(tool_btn_9_);
    box_layout->addStretch();
    this->addItem( (QWidget *)group_box, tr("Group members"));
}

void Drawer::ShowChatWidget1()
{
    chat_widget_1_ = new Widget(0, tool_btn_1_->text());
    chat_widget_1_->setWindowTitle(tool_btn_1_->text());
    chat_widget_1_->setWindowIcon(tool_btn_1_->icon());
    chat_widget_1_->show();
}

void Drawer::ShowChatWidget2()
{
    chat_widget_2_ = new Widget(0, tool_btn_2_->text());
    chat_widget_2_->setWindowTitle(tool_btn_2_->text());
    chat_widget_2_->setWindowIcon(tool_btn_2_->icon());
    chat_widget_2_->show();
}

void Drawer::ShowChatWidget3()
{
    chat_widget_3_ = new Widget(0, tool_btn_3_->text());
    chat_widget_3_->setWindowTitle(tool_btn_3_->text());
    chat_widget_3_->setWindowIcon(tool_btn_3_->icon());
    chat_widget_3_->show();
}

void Drawer::ShowChatWidget4()
{
    chat_widget_4_ = new Widget(0, tool_btn_4_->text());
    chat_widget_4_->setWindowTitle(tool_btn_4_->text());
    chat_widget_4_->setWindowIcon(tool_btn_4_->icon());
    chat_widget_4_->show();
}

void Drawer::ShowChatWidget5()
{
    chat_widget_5_ = new Widget(0, tool_btn_5_->text());
    chat_widget_5_->setWindowTitle(tool_btn_5_->text());
    chat_widget_5_->setWindowIcon(tool_btn_5_->icon());
    chat_widget_5_->show();
}

void Drawer::ShowChatWidget6()
{
    chat_widget_6_ = new Widget(0, tool_btn_6_->text());
    chat_widget_6_->setWindowTitle(tool_btn_6_->text());
    chat_widget_6_->setWindowIcon(tool_btn_6_->icon());
    chat_widget_6_->show();
}

void Drawer::ShowChatWidget7()
{
    chat_widget_7_ = new Widget(0, tool_btn_7_->text());
    chat_widget_7_->setWindowTitle(tool_btn_7_->text());
    chat_widget_7_->setWindowIcon(tool_btn_7_->icon());
    chat_widget_7_->show();
}

void Drawer::ShowChatWidget8()
{
    chat_widget_8_ = new Widget(0, tool_btn_8_->text());
    chat_widget_8_->setWindowTitle(tool_btn_8_->text());
    chat_widget_8_->setWindowIcon(tool_btn_8_->icon());
    chat_widget_8_->show();
}

void Drawer::ShowChatWidget9()
{
    chat_widget_9_ = new Widget(0, tool_btn_9_->text());
    chat_widget_9_->setWindowTitle(tool_btn_9_->text());
    chat_widget_9_->setWindowIcon(tool_btn_9_->icon());
    chat_widget_9_->show();
}
