#include "loginwindow.h"
#include "adminwindow.h"
#include "registrarwindow.h"
#include "clientwindow.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

LoginWindow::LoginWindow(QList<UserAccount> &usersList, QWidget *parent)
    : QWidget(parent)
    , users(usersList)
{
    setWindowTitle(QStringLiteral("Ветеринарная клиника «Лапа» — вход"));
    setFixedSize(420, 430);
    setStyleSheet(QStringLiteral("background:#eef3f1;"));

    auto *card = new QWidget(this);
    card->setStyleSheet(QStringLiteral("background:#ffffff;"));
    auto *layout = new QVBoxLayout(card);
    layout->setContentsMargins(32, 28, 32, 28);
    layout->setSpacing(8);

    auto *title = new QLabel(QStringLiteral("Ветеринарная клиника «Лапа»"));
    title->setStyleSheet(QStringLiteral("color:#153e3b; font-size:16px; font-weight:600;"));
    auto *subtitle = new QLabel(QStringLiteral("Вход в систему"));
    subtitle->setStyleSheet(QStringLiteral("color:#5b6e6b;"));

    loginEdit = new QLineEdit;
    passwordEdit = new QLineEdit;
    passwordEdit->setEchoMode(QLineEdit::Password);

    auto *enterBtn = new QPushButton(QStringLiteral("Войти"));
    enterBtn->setStyleSheet(QStringLiteral(
        "QPushButton { background:#2a9d8f; color:#fff; padding:8px; border:none; font-weight:600; }"
        "QPushButton:hover { background:#238b7e; }"));

    auto *hint = new QLabel(
        QStringLiteral("Тестовые пароли\n"
                       "anton / admin123          — системный администратор\n"
                       "volkov_doc / doctor123    — врач\n"
                       "ivanova_reg / 12345       — регистратор"));
    hint->setStyleSheet(QStringLiteral("background:#f4faf8; color:#3d5552; padding:10px; font-family:Consolas;"));

    layout->addWidget(title);
    layout->addWidget(subtitle);
    layout->addSpacing(8);
    layout->addWidget(new QLabel(QStringLiteral("Логин")));
    layout->addWidget(loginEdit);
    layout->addWidget(new QLabel(QStringLiteral("Пароль")));
    layout->addWidget(passwordEdit);
    layout->addWidget(enterBtn);
    layout->addWidget(hint);

    auto *root = new QVBoxLayout(this);
    root->addWidget(card);

    connect(enterBtn, &QPushButton::clicked, this, &LoginWindow::tryLogin);
    connect(passwordEdit, &QLineEdit::returnPressed, this, &LoginWindow::tryLogin);
}

void LoginWindow::tryLogin()
{
    const QString login = loginEdit->text().trimmed();
    const QString password = passwordEdit->text();

    for (UserAccount &user : users) {
        if (user.login != login || user.password != password)
            continue;
        if (user.blocked) {
            QMessageBox::warning(this, QStringLiteral("Вход"),
                                 QStringLiteral("Учётная запись заблокирована."));
            return;
        }
        user.online = true;
        hide();
        if (user.role == UserRole::Admin) {
            auto *admin = new AdminWindow(users, user);
            admin->show();
        } else if (user.role == UserRole::Client) {
            QMessageBox::information(this, QStringLiteral("Вход"),
                                     QStringLiteral("Клиенты входят через отдельное окно «Я клиент»."));
            return;
        } else {
            auto *work = new RegistrarWindow(user);
            work->show();
        }
        return;
    }

    QMessageBox::critical(this, QStringLiteral("Ошибка входа"),
                          QStringLiteral("Неверный логин или пароль."));
}
