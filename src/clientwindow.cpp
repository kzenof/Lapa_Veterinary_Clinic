#include "clientwindow.h"

#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QTableWidget>
#include <QHeaderView>
#include <QComboBox>

ClientAuthWindow::ClientAuthWindow(QList<UserAccount> &usersList, QWidget *parent)
    : QWidget(parent)
    , users(usersList)
{
    setWindowTitle(QStringLiteral("Личный кабинет — клиника «Лапа»"));
    setFixedSize(440, 520);
    setStyleSheet(QStringLiteral("background:#eef3f1;"));

    auto *root = new QVBoxLayout(this);
    auto *title = new QLabel(QStringLiteral("Личный кабинет владельца"));
    title->setStyleSheet(QStringLiteral("color:#153e3b; font-size:16px; font-weight:600;"));
    root->addWidget(title);

    auto *tabs = new QTabWidget;
    auto *loginTab = new QWidget;
    auto *regTab = new QWidget;
    loginTab->setStyleSheet(QStringLiteral("background:#ffffff;"));
    regTab->setStyleSheet(QStringLiteral("background:#ffffff;"));

    auto *ll = new QVBoxLayout(loginTab);
    loginEdit = new QLineEdit;
    passwordEdit = new QLineEdit;
    passwordEdit->setEchoMode(QLineEdit::Password);
    ll->addWidget(new QLabel(QStringLiteral("Логин")));
    ll->addWidget(loginEdit);
    ll->addWidget(new QLabel(QStringLiteral("Пароль")));
    ll->addWidget(passwordEdit);
    auto *enter = new QPushButton(QStringLiteral("Войти"));
    enter->setStyleSheet(QStringLiteral("background:#2a9d8f; color:#fff; padding:8px; border:none;"));
    ll->addWidget(enter);
    ll->addWidget(new QLabel(QStringLiteral("Тест: client / 12345")));
    ll->addStretch();
    connect(enter, &QPushButton::clicked, this, &ClientAuthWindow::tryLogin);

    auto *rl = new QVBoxLayout(regTab);
    regName = new QLineEdit;
    regPhone = new QLineEdit;
    regEmail = new QLineEdit;
    regLogin = new QLineEdit;
    regPass = new QLineEdit;
    regPass2 = new QLineEdit;
    regPass->setEchoMode(QLineEdit::Password);
    regPass2->setEchoMode(QLineEdit::Password);
    rl->addWidget(new QLabel(QStringLiteral("ФИО *")));
    rl->addWidget(regName);
    rl->addWidget(new QLabel(QStringLiteral("Телефон *")));
    rl->addWidget(regPhone);
    rl->addWidget(new QLabel(QStringLiteral("Email")));
    rl->addWidget(regEmail);
    rl->addWidget(new QLabel(QStringLiteral("Логин *")));
    rl->addWidget(regLogin);
    rl->addWidget(new QLabel(QStringLiteral("Пароль *")));
    rl->addWidget(regPass);
    rl->addWidget(new QLabel(QStringLiteral("Подтверждение пароля *")));
    rl->addWidget(regPass2);
    auto *regBtn = new QPushButton(QStringLiteral("Зарегистрироваться"));
    regBtn->setStyleSheet(QStringLiteral("background:#2a9d8f; color:#fff; padding:8px; border:none;"));
    rl->addWidget(regBtn);
    connect(regBtn, &QPushButton::clicked, this, &ClientAuthWindow::tryRegister);

    tabs->addTab(loginTab, QStringLiteral("Вход"));
    tabs->addTab(regTab, QStringLiteral("Регистрация"));
    root->addWidget(tabs);
}

void ClientAuthWindow::tryLogin()
{
    const QString login = loginEdit->text().trimmed();
    const QString password = passwordEdit->text();
    for (UserAccount &user : users) {
        if (user.login != login || user.password != password)
            continue;
        if (user.role != UserRole::Client) {
            QMessageBox::warning(this, QStringLiteral("Вход"),
                                 QStringLiteral("Это окно только для клиентов."));
            return;
        }
        hide();
        auto *cabinet = new ClientCabinetWindow(user);
        cabinet->show();
        return;
    }
    QMessageBox::critical(this, QStringLiteral("Вход"),
                          QStringLiteral("Неверный логин или пароль клиента."));
}

void ClientAuthWindow::tryRegister()
{
    if (regName->text().trimmed().isEmpty() || regPhone->text().trimmed().isEmpty()
        || regLogin->text().trimmed().isEmpty() || regPass->text().isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("Регистрация"),
                             QStringLiteral("Заполните ФИО, телефон, логин и пароль."));
        return;
    }
    if (regPass->text() != regPass2->text()) {
        QMessageBox::warning(this, QStringLiteral("Регистрация"),
                             QStringLiteral("Пароли не совпадают."));
        return;
    }
    for (const UserAccount &u : users) {
        if (u.login == regLogin->text().trimmed()) {
            QMessageBox::warning(this, QStringLiteral("Регистрация"),
                                 QStringLiteral("Такой логин уже занят."));
            return;
        }
    }
    users.append({
        users.size() + 1,
        regLogin->text().trimmed(),
        regPass->text(),
        regName->text().trimmed(),
        UserRole::Client,
        false,
        false
    });
    QMessageBox::information(this, QStringLiteral("Регистрация"),
                             QStringLiteral("Аккаунт создан. Войдите на вкладке «Вход»."));
}

ClientCabinetWindow::ClientCabinetWindow(const UserAccount &user, QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(QStringLiteral("Личный кабинет — «Лапа»"));
    resize(1000, 640);
    auto *root = new QHBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);

    auto *side = new QWidget;
    side->setFixedWidth(210);
    side->setStyleSheet(QStringLiteral("background:#153e3b;"));
    auto *sl = new QVBoxLayout(side);
    auto *brand = new QLabel(QStringLiteral("Лапа"));
    brand->setStyleSheet(QStringLiteral("color:#fff; font-size:16px; font-weight:600;"));
    auto *who = new QLabel(user.displayName + QStringLiteral("\nВладелец (клиент)"));
    who->setStyleSheet(QStringLiteral("color:#a8d5cf;"));
    sl->addWidget(brand);
    sl->addWidget(who);
    sl->addStretch();

    auto *work = new QWidget;
    auto *wl = new QVBoxLayout(work);
    auto *cap = new QLabel(QStringLiteral("Мои питомцы и записи"));
    cap->setStyleSheet(QStringLiteral("color:#153e3b; font-size:16px; font-weight:600;"));
    wl->addWidget(cap);

    auto *pets = new QTableWidget(2, 3);
    pets->setHorizontalHeaderLabels({QStringLiteral("Кличка"), QStringLiteral("Вид"), QStringLiteral("Порода")});
    pets->setItem(0, 0, new QTableWidgetItem(QStringLiteral("Мурка")));
    pets->setItem(0, 1, new QTableWidgetItem(QStringLiteral("Кошка")));
    pets->setItem(0, 2, new QTableWidgetItem(QStringLiteral("Британская")));
    pets->setItem(1, 0, new QTableWidgetItem(QStringLiteral("Барсик")));
    pets->setItem(1, 1, new QTableWidgetItem(QStringLiteral("Кот")));
    pets->setItem(1, 2, new QTableWidgetItem(QStringLiteral("Дворовая")));
    pets->verticalHeader()->setVisible(false);
    pets->horizontalHeader()->setStretchLastSection(true);
    wl->addWidget(pets);

    auto *bookTitle = new QLabel(QStringLiteral("Записаться на приём"));
    wl->addWidget(bookTitle);
    auto *doctor = new QComboBox;
    doctor->addItems({QStringLiteral("Иванова А.С."), QStringLiteral("Сидоров П.Н.")});
    wl->addWidget(doctor);
    auto *book = new QPushButton(QStringLiteral("Записаться на приём"));
    book->setStyleSheet(QStringLiteral("background:#2a9d8f; color:#fff; padding:8px; border:none;"));
    wl->addWidget(book);
    wl->addStretch();

    root->addWidget(side);
    root->addWidget(work, 1);
}
