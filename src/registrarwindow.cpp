#include "registrarwindow.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QFrame>

RegistrarWindow::RegistrarWindow(const UserAccount &user, QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(QStringLiteral("Ветеринарная клиника «Лапа»"));
    resize(1180, 740);
    setStyleSheet(QStringLiteral("background:#eef3f1;"));

    auto *root = new QHBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // Левая навигационная панель
    auto *side = new QWidget;
    side->setFixedWidth(210);
    side->setStyleSheet(QStringLiteral("background:#153e3b;"));
    auto *sideLayout = new QVBoxLayout(side);
    sideLayout->setContentsMargins(16, 20, 16, 16);

    auto *brand = new QLabel(QStringLiteral("Лапа"));
    brand->setStyleSheet(QStringLiteral("color:#ffffff; font-size:16px; font-weight:600;"));
    auto *who = new QLabel(user.displayName + "\n" + roleToString(user.role));
    who->setStyleSheet(QStringLiteral("color:#a8d5cf;"));

    sideLayout->addWidget(brand);
    sideLayout->addWidget(who);
    sideLayout->addSpacing(16);

    const QStringList menu{
        QStringLiteral("Сегодня"),
        QStringLiteral("Приёмы"),
        QStringLiteral("Пациенты"),
        QStringLiteral("Клиенты"),
        QStringLiteral("Услуги")
    };
    for (const QString &item : menu) {
        auto *btn = new QPushButton(item);
        btn->setFlat(true);
        btn->setStyleSheet(QStringLiteral(
            "QPushButton { color:#e8f4f2; text-align:left; padding:10px 8px; border:none; }"
            "QPushButton:hover { background:#1f5c58; }"));
        sideLayout->addWidget(btn);
    }
    sideLayout->addStretch();

    auto *logout = new QPushButton(QStringLiteral("Выйти"));
    logout->setFlat(true);
    logout->setStyleSheet(QStringLiteral("color:#f4c7b8; text-align:left; border:none; padding:10px 8px;"));
    connect(logout, &QPushButton::clicked, this, &QWidget::close);
    sideLayout->addWidget(logout);

    // Рабочая область «Сегодня»
    auto *work = new QWidget;
    auto *workLayout = new QVBoxLayout(work);
    workLayout->setContentsMargins(24, 20, 24, 20);

    auto *caption = new QLabel(QStringLiteral("Сегодня"));
    caption->setStyleSheet(QStringLiteral("color:#153e3b; font-size:16px; font-weight:600;"));
    workLayout->addWidget(caption);

    auto *cards = new QHBoxLayout;
    const QList<QPair<QString, QString>> stats{
        {QStringLiteral("2"), QStringLiteral("Пациенты")},
        {QStringLiteral("2"), QStringLiteral("Клиенты")},
        {QStringLiteral("1"), QStringLiteral("Приёмы сегодня")}
    };
    for (const auto &stat : stats) {
        auto *card = new QFrame;
        card->setStyleSheet(QStringLiteral("background:#ffffff; border:1px solid #c9ddd8;"));
        auto *cl = new QVBoxLayout(card);
        auto *num = new QLabel(stat.first);
        num->setStyleSheet(QStringLiteral("color:#153e3b; font-size:22px; font-weight:600;"));
        auto *txt = new QLabel(stat.second);
        txt->setStyleSheet(QStringLiteral("color:#5b6e6b;"));
        cl->addWidget(num);
        cl->addWidget(txt);
        cards->addWidget(card);
    }
    cards->addStretch();
    workLayout->addLayout(cards);

    auto *tableTitle = new QLabel(QStringLiteral("Расписание на сегодня"));
    workLayout->addWidget(tableTitle);

    auto *table = new QTableWidget(1, 6);
    table->setHorizontalHeaderLabels({
        QStringLiteral("Время"), QStringLiteral("Пациент"), QStringLiteral("Владелец"),
        QStringLiteral("Врач"), QStringLiteral("Причина"), QStringLiteral("Статус")
    });
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setStretchLastSection(true);
    table->setItem(0, 0, new QTableWidgetItem(QStringLiteral("11:30")));
    table->setItem(0, 1, new QTableWidgetItem(QStringLiteral("Рекс")));
    table->setItem(0, 2, new QTableWidgetItem(QStringLiteral("Козлов Дмитрий")));
    table->setItem(0, 3, new QTableWidgetItem(QStringLiteral("Иванова А.С.")));
    table->setItem(0, 4, new QTableWidgetItem(QStringLiteral("Хромота, осмотр")));
    table->setItem(0, 5, new QTableWidgetItem(QStringLiteral("Запланирован")));
    workLayout->addWidget(table, 1);

    root->addWidget(side);
    root->addWidget(work, 1);
}
