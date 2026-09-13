#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QWidget>
#include "userdata.h"

class StartWindow : public QWidget
{
    Q_OBJECT
public:
    explicit StartWindow(QWidget *parent = nullptr);

private:
    QList<UserAccount> users;
};

#endif // STARTWINDOW_H
