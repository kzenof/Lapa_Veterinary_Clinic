#ifndef USERDATA_H
#define USERDATA_H

#include <QString>
#include <QList>

// Роли доступа информационной системы ветеринарной клиники
enum class UserRole {
    Admin,
    Doctor,
    Registrar,
    Client
};

struct UserAccount {
    int id;
    QString login;
    QString password;
    QString displayName;
    UserRole role;
    bool online;
    bool blocked;
};

inline QString roleToString(UserRole role)
{
    switch (role) {
    case UserRole::Admin:     return QStringLiteral("Админ");
    case UserRole::Doctor:    return QStringLiteral("Врач");
    case UserRole::Registrar: return QStringLiteral("Регистратор");
    case UserRole::Client:    return QStringLiteral("Клиент");
    }
    return QStringLiteral("—");
}

inline UserRole roleFromString(const QString &text)
{
    if (text == QStringLiteral("Админ")) return UserRole::Admin;
    if (text == QStringLiteral("Врач"))  return UserRole::Doctor;
    return UserRole::Registrar;
}

// Демонстрационные учётные записи (для отчёта / тестового стенда)
inline QList<UserAccount> demoUsers()
{
    return {
        {1, "anton",       "admin123",  "Антон",        UserRole::Admin,     true,  false},
        {2, "volkov_doc",  "doctor123", "Волков А.В.",  UserRole::Doctor,    false, false},
        {3, "ivanova_reg", "12345",     "Петрова М.И.", UserRole::Registrar, false, false},
        {4, "client",      "12345",     "Смирнова Елена", UserRole::Client,  false, false},
    };
}

#endif // USERDATA_H
