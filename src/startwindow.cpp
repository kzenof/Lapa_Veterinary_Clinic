#include "startwindow.h"
#include "loginwindow.h"
#include "clientwindow.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

StartWindow::StartWindow(QWidget *parent)
    : QWidget(parent)
    , users(demoUsers())
{
    setWindowTitle(QStringLiteral("Ветеринарная клиника «Лапа»"));
    setFixedSize(420, 360);
    setStyleSheet(QStringLiteral("background:#eef3f1;"));

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(40, 36, 40, 36);
    auto *title = new QLabel(QStringLiteral("Ветеринарная клиника «Лапа»"));
    title->setStyleSheet(QStringLiteral("color:#153e3b; font-size:16px; font-weight:600;"));
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);
    layout->addSpacing(16);

    auto *clientBtn = new QPushButton(QStringLiteral("Я клиент\nвход и регистрация"));
    clientBtn->setMinimumHeight(72);
    clientBtn->setStyleSheet(QStringLiteral("background:#2a9d8f; color:#fff; border:none; font-weight:600;"));
    auto *staffBtn = new QPushButton(QStringLiteral("Я сотрудник\nрегистратура / врач / админ"));
    staffBtn->setMinimumHeight(72);
    staffBtn->setStyleSheet(QStringLiteral("background:#153e3b; color:#fff; border:none; font-weight:600;"));
    layout->addWidget(clientBtn);
    layout->addWidget(staffBtn);

    connect(clientBtn, &QPushButton::clicked, this, [this]() {
        auto *w = new ClientAuthWindow(users);
        w->show();
    });
    connect(staffBtn, &QPushButton::clicked, this, [this]() {
        auto *w = new LoginWindow(users);
        w->show();
    });
}
