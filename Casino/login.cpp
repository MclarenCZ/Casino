#include "login.h"
#include "ui_login.h"


Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("../Casino/casino.db");
    db.open();
    if(db.isOpen()){
        ui->status->setText("Pripojeno");

    }
    else
    {
        ui->status->setText("Nepripojeno");
    }
}

Login::~Login()
{
    delete ui;
}

void Login::accept()
{
    //QDialog::accept();
    if(ui->name->text().isEmpty()){
        QMessageBox::warning(this, "Error!", "Please fill your name!!");
        ui->name->setFocus();
        return;
    }
    if(ui->pswd->text().isEmpty()){
        QMessageBox::warning(this, "Error!", "Please fill your pswd!!");
        ui->name->setFocus();
        return;
    }

    if(check(ui->name->text(),ui->pswd->text())){
        QDialog::accept();
    }
    else{
        QMessageBox::warning(this, "Error!", "Wrong Pswd!!");
        return;
    }

}

bool Login::check(QString name,QString pswd)
{
    QSqlQuery dotaz;
    dotaz.prepare("SELECT * FROM players where name=:name");
    dotaz.bindValue(":name", QString(name));
    dotaz.exec();
    while(dotaz.next())
    {
       QString pswdsql = dotaz.value("pswd").toString();
       int idlocal = dotaz.value("id").toInt();
       if(pswdsql==pswd){
           setId(idlocal);
           return true;

       }
       else{
           setId(idlocal);
           return false;
       }
    }
}

int Login::getId() const
{
    return id;
}

void Login::setId(int value)
{
    id = value;
}
