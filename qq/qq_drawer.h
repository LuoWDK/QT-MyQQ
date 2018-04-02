#ifndef QQ_DRAWER_H
#define QQ_DRAWER_H

#include <QWidget>
#include <QToolBox>
#include <QToolButton>
#include <qq_widget.h>

class Drawer : public QToolBox
{
    Q_OBJECT

public:
    Drawer(QWidget *parent = 0, Qt::WindowFlags f = 0);

private slots:
    void ShowChatWidget1();
    void ShowChatWidget2();
    void ShowChatWidget3();
    void ShowChatWidget4();
    void ShowChatWidget5();
    void ShowChatWidget6();
    void ShowChatWidget7();
    void ShowChatWidget8();
    void ShowChatWidget9();

private:
    QToolButton *tool_btn_1_;
    QToolButton *tool_btn_2_;
    QToolButton *tool_btn_3_;
    QToolButton *tool_btn_4_;
    QToolButton *tool_btn_5_;
    QToolButton *tool_btn_6_;
    QToolButton *tool_btn_7_;
    QToolButton *tool_btn_8_;
    QToolButton *tool_btn_9_;

    Widget *chat_widget_1_;
    Widget *chat_widget_2_;
    Widget *chat_widget_3_;
    Widget *chat_widget_4_;
    Widget *chat_widget_5_;
    Widget *chat_widget_6_;
    Widget *chat_widget_7_;
    Widget *chat_widget_8_;
    Widget *chat_widget_9_;

};

#endif // QQ_DRAWER_H
