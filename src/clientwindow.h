#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QWidget>
#include "userdata.h"

class QLineEdit;
class QTabWidget;

// Отдельное окно клиента: вкладки «Вход» и «Регистрация», затем личный кабинет.
class ClientAuthWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ClientAuthWindow(QList<UserAccount> &users, QWidget *parent = nullptr);

private slots:
    void tryLogin();
    void tryRegister();

private:
    QList<UserAccount> &users;
    QLineEdit *loginEdit;
    QLineEdit *passwordEdit;
    QLineEdit *regName;
    QLineEdit *regPhone;
    QLineEdit *regEmail;
    QLineEdit *regLogin;
    QLineEdit *regPass;
    QLineEdit *regPass2;
};

class ClientCabinetWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ClientCabinetWindow(const UserAccount &user, QWidget *parent = nullptr);
};

#endif // CLIENTWINDOW_H
