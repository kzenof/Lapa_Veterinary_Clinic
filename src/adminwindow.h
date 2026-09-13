#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QWidget>
#include "userdata.h"

class QTableWidget;
class QLineEdit;
class QComboBox;
class QStackedWidget;

// Окно системного администратора CORE_ADMIN_v1:
// управление персоналом, логи, бэкап БД и параметры сети.
class AdminWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AdminWindow(QList<UserAccount> users,
                         const UserAccount &current,
                         QWidget *parent = nullptr);

private slots:
    void showUsersPage();
    void showLogsPage();
    void showBackupPage();
    void showNetworkPage();
    void onUserSelected();
    void saveUserChanges();
    void blockUser();

private:
    void fillUsersTable();
    int selectedRow() const;

    QList<UserAccount> users;
    UserAccount currentUser;
    QStackedWidget *pages;
    QTableWidget *usersTable;
    QLineEdit *loginEdit;
    QLineEdit *passwordEdit;
    QComboBox *roleBox;
};

#endif // ADMINWINDOW_H
