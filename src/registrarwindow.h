#ifndef REGISTRARWINDOW_H
#define REGISTRARWINDOW_H

#include <QWidget>
#include "userdata.h"

// Главное окно регистратора / врача: навигация и рабочая область «Сегодня».
class RegistrarWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RegistrarWindow(const UserAccount &user, QWidget *parent = nullptr);
};

#endif // REGISTRARWINDOW_H
