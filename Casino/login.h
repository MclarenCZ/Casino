#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QSqlQuery>
#include <QMessageBox>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();
    int getId() const;
    void setId(int value);

private slots:
    void accept();
    bool check(QString name, QString pswd);

private:
    Ui::Login *ui;
    int id;
    QSqlDatabase db;
};

#endif // LOGIN_H
