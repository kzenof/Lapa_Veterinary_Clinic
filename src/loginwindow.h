#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include "userdata.h"

class QLineEdit;

// Окно авторизации. После успешного входа открывается
// рабочее место регистратора, врача или системного администратора.
class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QList<UserAccount> &users, QWidget *parent = nullptr);

private slots:
    void tryLogin();

private:
    QLineEdit *loginEdit;
    QLineEdit *passwordEdit;
    QList<UserAccount> &users;
};

#endif // LOGINWINDOW_H
