#include "adminwindow.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QLineEdit>
#include <QComboBox>
#include <QStackedWidget>
#include <QMessageBox>
#include <QFrame>
#include <QColor>
#include <QAbstractItemView>
#include <utility>

AdminWindow::AdminWindow(QList<UserAccount> usersList,
                         const UserAccount &current,
                         QWidget *parent)
    : QWidget(parent)
    , users(std::move(usersList))
    , currentUser(current)
{
    setWindowTitle(QStringLiteral("CORE_ADMIN_v1"));
    resize(1180, 640);
    setStyleSheet(QStringLiteral("background:#ffffff;"));

    auto *root = new QHBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // Тёмное меню администратора
    auto *side = new QWidget;
    side->setFixedWidth(188);
    side->setStyleSheet(QStringLiteral("background:#2b2b2b;"));
    auto *sideLayout = new QVBoxLayout(side);
    sideLayout->setContentsMargins(0, 0, 0, 12);
    sideLayout->setSpacing(0);

    auto *brand = new QLabel(QStringLiteral("CORE_ADMIN_v1"));
    brand->setStyleSheet(QStringLiteral("color:#ffffff; font-weight:600; padding:16px;"));
    sideLayout->addWidget(brand);

    auto makeNav = [this, sideLayout](const QString &title, void (AdminWindow::*slot)()) {
        auto *btn = new QPushButton(title);
        btn->setFlat(true);
        btn->setStyleSheet(QStringLiteral(
            "QPushButton { color:#e8e8e8; text-align:left; padding:10px 18px; border:none; }"
            "QPushButton:hover { background:#3f3f3f; }"));
        connect(btn, &QPushButton::clicked, this, slot);
        sideLayout->addWidget(btn);
    };
    makeNav(QStringLiteral("Пользователи"), &AdminWindow::showUsersPage);
    makeNav(QStringLiteral("Логи системы"), &AdminWindow::showLogsPage);
    makeNav(QStringLiteral("Бэкап БД"), &AdminWindow::showBackupPage);
    makeNav(QStringLiteral("Настройки сети"), &AdminWindow::showNetworkPage);
    sideLayout->addStretch();

    pages = new QStackedWidget;

    // ---- страница «Управление персоналом» ----
    auto *usersPage = new QWidget;
    auto *usersRoot = new QHBoxLayout(usersPage);
    usersRoot->setContentsMargins(0, 0, 0, 0);

    auto *center = new QWidget;
    auto *centerLayout = new QVBoxLayout(center);
    auto *caption = new QLabel(QStringLiteral("Управление персоналом"));
    caption->setStyleSheet(QStringLiteral("font-size:13px; font-weight:600; padding:16px 18px 8px 18px;"));
    usersTable = new QTableWidget(0, 4);
    usersTable->setHorizontalHeaderLabels({
        QStringLiteral("ID"), QStringLiteral("Логин"),
        QStringLiteral("Роль"), QStringLiteral("Статус")
    });
    usersTable->verticalHeader()->setVisible(false);
    usersTable->horizontalHeader()->setStretchLastSection(true);
    usersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    usersTable->setSelectionMode(QAbstractItemView::SingleSelection);
    usersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(usersTable, &QTableWidget::itemSelectionChanged,
            this, &AdminWindow::onUserSelected);
    centerLayout->addWidget(caption);
    centerLayout->addWidget(usersTable, 1);

    // Правая карточка «Данные работника»
    auto *card = new QFrame;
    card->setFixedWidth(280);
    card->setStyleSheet(QStringLiteral("background:#ffffff; border-left:1px solid #e0e0e0;"));
    auto *form = new QVBoxLayout(card);
    form->setContentsMargins(22, 22, 22, 22);
    auto *cardTitle = new QLabel(QStringLiteral("Данные работника"));
    cardTitle->setStyleSheet(QStringLiteral("font-size:12px; font-weight:600;"));
    cardTitle->setAlignment(Qt::AlignHCenter);

    loginEdit = new QLineEdit;
    passwordEdit = new QLineEdit;
    passwordEdit->setEchoMode(QLineEdit::Password);
    roleBox = new QComboBox;
    roleBox->addItems({QStringLiteral("Админ"), QStringLiteral("Врач"), QStringLiteral("Регистратор")});

    auto *saveBtn = new QPushButton(QStringLiteral("СОХРАНИТЬ ИЗМЕНЕНИЯ"));
    saveBtn->setStyleSheet(QStringLiteral(
        "QPushButton { background:#3a3a3a; color:#fff; padding:10px; border:none; font-weight:600; }"));
    auto *blockBtn = new QPushButton(QStringLiteral("ЗАБЛОКИРОВАТЬ"));
    blockBtn->setStyleSheet(QStringLiteral(
        "QPushButton { background:#c0392b; color:#fff; padding:10px; border:none; font-weight:600; }"));

    connect(saveBtn, &QPushButton::clicked, this, &AdminWindow::saveUserChanges);
    connect(blockBtn, &QPushButton::clicked, this, &AdminWindow::blockUser);

    form->addWidget(cardTitle);
    form->addSpacing(12);
    form->addWidget(new QLabel(QStringLiteral("Логин")));
    form->addWidget(loginEdit);
    form->addWidget(new QLabel(QStringLiteral("Новый пароль")));
    form->addWidget(passwordEdit);
    form->addWidget(new QLabel(QStringLiteral("Уровень доступа (Роль)")));
    form->addWidget(roleBox);
    form->addSpacing(10);
    form->addWidget(saveBtn);
    form->addWidget(blockBtn);
    form->addStretch();

    usersRoot->addWidget(center, 1);
    usersRoot->addWidget(card);
    pages->addWidget(usersPage);

    auto makeSimplePage = [](const QString &title, const QString &text) {
        auto *page = new QWidget;
        auto *l = new QVBoxLayout(page);
        l->setContentsMargins(24, 24, 24, 24);
        auto *t = new QLabel(title);
        t->setStyleSheet(QStringLiteral("font-size:13px; font-weight:600;"));
        auto *d = new QLabel(text);
        d->setWordWrap(true);
        l->addWidget(t);
        l->addWidget(d);
        l->addStretch();
        return page;
    };
    pages->addWidget(makeSimplePage(
        QStringLiteral("Логи системы"),
        QStringLiteral("Журнал событий: вход/выход пользователей, смена ролей, блокировка учётных записей.")));
    pages->addWidget(makeSimplePage(
        QStringLiteral("Бэкап БД"),
        QStringLiteral("Создание резервной копии файла базы данных clinic.db.")));
    pages->addWidget(makeSimplePage(
        QStringLiteral("Настройки сети"),
        QStringLiteral("Параметры подключения: адрес сервера, порт, таймаут.")));

    root->addWidget(side);
    root->addWidget(pages, 1);

    fillUsersTable();
    if (usersTable->rowCount() > 0)
        usersTable->selectRow(0);
}

void AdminWindow::fillUsersTable()
{
    usersTable->setRowCount(users.size());
    for (int i = 0; i < users.size(); ++i) {
        const UserAccount &u = users[i];
        usersTable->setItem(i, 0, new QTableWidgetItem(QString::number(u.id)));
        usersTable->setItem(i, 1, new QTableWidgetItem(u.login));
        usersTable->setItem(i, 2, new QTableWidgetItem(roleToString(u.role)));
        auto *status = new QTableWidgetItem(u.blocked ? QStringLiteral("Blocked")
                                        : (u.online ? QStringLiteral("Online")
                                                    : QStringLiteral("Offline")));
        status->setForeground(u.online && !u.blocked ? QColor("#2e7d32") : QColor("#888888"));
        usersTable->setItem(i, 3, status);
    }
}

int AdminWindow::selectedRow() const
{
    const auto indexes = usersTable->selectionModel()->selectedRows();
    return indexes.isEmpty() ? -1 : indexes.first().row();
}

void AdminWindow::onUserSelected()
{
    const int row = selectedRow();
    if (row < 0) return;
    const UserAccount &u = users[row];
    loginEdit->setText(u.login);
    passwordEdit->clear();
    roleBox->setCurrentText(roleToString(u.role));
}

void AdminWindow::saveUserChanges()
{
    const int row = selectedRow();
    if (row < 0) {
        QMessageBox::information(this, QStringLiteral("Пользователи"),
                                 QStringLiteral("Выберите сотрудника в таблице."));
        return;
    }
    users[row].login = loginEdit->text().trimmed();
    users[row].role = roleFromString(roleBox->currentText());
    if (!passwordEdit->text().isEmpty())
        users[row].password = passwordEdit->text();
    fillUsersTable();
    usersTable->selectRow(row);
    QMessageBox::information(this, QStringLiteral("Пользователи"),
                             QStringLiteral("Изменения сохранены."));
}

void AdminWindow::blockUser()
{
    const int row = selectedRow();
    if (row < 0) return;
    if (users[row].login == currentUser.login) {
        QMessageBox::warning(this, QStringLiteral("Ошибка"),
                             QStringLiteral("Нельзя заблокировать собственную учётную запись."));
        return;
    }
    users[row].blocked = true;
    users[row].online = false;
    fillUsersTable();
    usersTable->selectRow(row);
}

void AdminWindow::showUsersPage()    { pages->setCurrentIndex(0); }
void AdminWindow::showLogsPage()     { pages->setCurrentIndex(1); }
void AdminWindow::showBackupPage()   { pages->setCurrentIndex(2); }
void AdminWindow::showNetworkPage()  { pages->setCurrentIndex(3); }
