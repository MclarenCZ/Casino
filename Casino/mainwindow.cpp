#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlQuery>
#include <QItemSelectionModel>
#include <QDebug>
#include <QModelIndex>
#include "login.h"
#include <QTimer>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("../Casino/casino.db");
    db.open();
    Login d;
    if(d.exec() == QDialog::Accepted){
        int id = d.getId();
        setPlayer_id(id);
        SelectInfo(id);
        Update(getPlayer_id());

    }
    else
    {
        QTimer::singleShot(0,this,SLOT(close()));
    }

    model = new QSqlTableModel(this);
    model->setTable("invall");
    model->select();
    //model->setHeaderData(1, Qt::Horizontal, "Jméno");
    //model->setHeaderData(2, Qt::Horizontal, "Věk");


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Update(int id_player)
{
    viewmodel = new QSqlQueryModel(this);
    QString invdotaz = "select zaznam_id,player_id,item_id,inventoryall.name,inventoryall.value,item_type.name as type from inventoryall join item_type on inventoryall.type_id == item_type.id and player_id == " + QString::number(id_player);
    //qDebug()<< invdotaz;
    viewmodel->setQuery(invdotaz);
    viewmodel->setHeaderData(3, Qt::Horizontal, "Name");
    viewmodel->setHeaderData(4, Qt::Horizontal, "Value");
    viewmodel->setHeaderData(5, Qt::Horizontal, "Type");

    ui->invModel->setModel(viewmodel);
    ui->invModel->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->invModel->hideColumn(0);
    ui->invModel->hideColumn(1);
    ui->invModel->hideColumn(2);
    //ui->invModel->hideColumn(5);

    ui->Money->setText("Coins:" + QString::number(coins));
    ui->Lvl->setText("Lvl:"+QString::number(lvl)+" ");
    ui->Name->setText(player_name);

    itemListModel = new QSqlTableModel(this);
    itemListModel->setTable("items");
    itemListModel->select();
    ui->ItemList->setModel(itemListModel);
    ui->B_Bet->setMaximum(this->coins);
    ui->C_Bet->setMaximum(this->coins);

}

void MainWindow::AddItem(int id_player, int id_item)
{
    QSqlQuery dotazI;
    dotazI.prepare("INSERT INTO player_inventory VALUES(NULL, :player,:item)");
    dotazI.bindValue(":player", id_player);
    dotazI.bindValue(":item", id_item);
    dotazI.exec();
}

void MainWindow::DeleteItem()
{
    QItemSelectionModel *selection = ui->invModel->selectionModel();
    if(selection->selectedRows().isEmpty()){
        return;
        //qDebug()<<"nevybrano";
    }
    QString sqlS = "select * from inventoryall where player_id="+ QString::number(this->player_id)+ " and zaznam_id IN (";
    for(int i = 0; i<selection->selectedRows().count(); ++i)
    {
        QModelIndex index = selection->selectedRows().at(i);
        sqlS+= QString::number(viewmodel->data(index).toInt());
        if(i+1 != selection->selectedRows().count())
        {
            sqlS+=", ";
        }
    }
    sqlS+=")";
    //qDebug()<<sqlS;
    QSqlQuery dotazS;
    dotazS.prepare(sqlS);
    dotazS.exec();
    int itemvalue = 0;
    while(dotazS.next())
    {
        itemvalue = itemvalue + dotazS.value("value").toInt();
    }
    coins+=itemvalue;
    //qDebug()<<coins;

    QString sql = "DELETE FROM player_inventory WHERE zaznam_id IN (";
    for(int i = 0; i<selection->selectedRows().count(); ++i)
    {
        QModelIndex index = selection->selectedRows().at(i);
        sql+= QString::number(viewmodel->data(index).toInt());
        if(i+1 != selection->selectedRows().count())
        {
            sql+=", ";
        }
    }
    sql+=")";
    QSqlQuery dotaz;
    //qDebug() << sql;
    dotaz.prepare(sql);
    dotaz.exec();

    //viewmodel->select();
    lvl+=1;
    Update(getPlayer_id());
    WriteChanges();
}

void MainWindow::SelectInfo(int id)
{
    QSqlQuery dotaz;
    dotaz.prepare("SELECT * FROM players where id=:id");
    dotaz.bindValue(":id", id);
    dotaz.exec();
    while(dotaz.next())
    {
        player_name = dotaz.value("name").toString();
        coins = dotaz.value("coins").toInt();
        lvl = dotaz.value("lvl").toInt();
    }
}

void MainWindow::LoadInventory(int id)
{

}

void MainWindow::WriteChanges()
{
    QSqlQuery dotazR;
    QString sqlR = "UPDATE players SET coins = "+QString::number(coins)+" ,lvl = "+QString::number(lvl)+ " where id="+QString::number(getPlayer_id());
    //qDebug()<<sqlR;
    dotazR.prepare(sqlR);
    dotazR.exec();
}

void MainWindow::ResetCrash()
{
    ui->C_bar->setValue(0);
    ui->C_YourBet->setText("Your bet:0");
    ui->C_WinBet->setText("You can take:0");
    ui->C_Multipler->setText("Multiplier:0");
    C_Bet=0;
    C_LocalNum=1;
    C_Multiplier=1;
    C_Num=0;
    CrashPlay=false;
}

int MainWindow::CrashNum()
{
    int jedna = qrand() % ((100 + 1) - 1) + 1;
    int dva = qrand() % ((100 + 1) - 1) + 1;
    int tri = qrand() % ((100 + 1) - 1) + 1;
    int ct = qrand() % ((100 + 1) - 1) + 1;
    int pet = qrand() % ((100 + 1) - 1) + 1;
    int sest = qrand() % ((100 + 1) - 1) + 1;
    int sedm = qrand() % ((100 + 1) - 1) + 1;
    int osm = qrand() % ((100 + 1) - 1) + 1;
    int dev = qrand() % ((100 + 1) - 1) + 1;
    int des = qrand() % ((100 + 1) - 1) + 1;
    if(jedna <70){
        if(dva<65){
            if(tri<60){
                if(ct<55){
                    if(pet<50){
                        if(sest<50){
                            if(sedm<50){
                                if(osm<50){
                                    if(dev<50){
                                        if(des<50){
                                            return 10;
                                        }
                                        else{
                                            return 10;
                                        }
                                    }
                                    else{
                                        return 9;
                                    }
                                }
                                else{
                                    return 8;
                                }
                            }
                            else{
                                return 7;
                            }
                        }
                        else{
                            return 6;
                        }
                    }
                    else{
                        return 5;
                    }
                }
                else{
                    return 4;
                }
            }
            else{
                return 3;
            }
        }
        else{
            return 2;
        }
    }
    else{
        return 1;
    }
}

void MainWindow::BuyItem(int id_player, int buyvalue, int id_item)
{
   if(buyvalue>coins){
       QMessageBox::warning(this,"Error","You dont have enough money!");
       return;
   }
   coins-=buyvalue;
   AddItem(id_player,id_item);
   WriteChanges();
   Update(getPlayer_id());



}

void MainWindow::on_sellButton_clicked()
{
   DeleteItem();
}

int MainWindow::getPlayer_id() const
{
    return player_id;
}

void MainWindow::setPlayer_id(int value)
{
    player_id = value;
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    Update(getPlayer_id());
    if(index==1){
        ui->B_Bet->setMaximum(this->coins);
        ui->C_Bet->setMaximum(this->coins);
    }


}

void MainWindow::on_pushButton_2_clicked()
{
    AddItem(getPlayer_id(),2);
    Update(getPlayer_id());
    WriteChanges();
}

void MainWindow::on_B_BetButton_clicked()
{
    int result = qrand() % ((100 + 1) - 0) + 0;
    //int result = 50;
    BetColor resultcolor;
    BetColor playerbet;
    int bet = ui->B_Bet->value();
    if(bet<=0){
        QMessageBox::warning(this, "Error", "Please give your bet");
        return;
    }
    coins= coins-bet;
    if(ui->B_Black->isChecked()){
        playerbet = Black;
    }
    if(ui->B_Red->isChecked()){
        playerbet = Red;
    }
    if(ui->B_Green->isChecked()){
        playerbet = Green;
    }
    if(result<49){
        resultcolor = Black;
     //QMessageBox::warning(this, "Error!", "Please fill your name!!");
    }
    if(result>51){
       resultcolor = Red;
    }
    if(result== 49 || result == 50 || result == 51){
       resultcolor = Green;
    }
    if(playerbet == resultcolor){
        if(resultcolor== Green){
           bet = bet*8;
           QMessageBox::warning(this, "Result!", "JACKPOOT OMG YOU WIN "+ QString::number(bet) + " AND NEW POTION!!");
           coins+=bet;
           AddItem(getPlayer_id(),30);
        }
        else{
           bet = bet + bet;
           QMessageBox::warning(this, "Result!", "YOU WIN "+ QString::number(bet) + "!!");
           coins+=bet;
        }
    }
    else{
      QMessageBox::warning(this, "Result!", "FUCK OFF!!");
    }
    //qDebug() << result;
    //qDebug() << resultcolor;
    //qDebug() << coins;
    lvl+=1;
    WriteChanges();
    Update(getPlayer_id());

}

void MainWindow::on_C_Start_clicked()
{
    if(CrashPlay){
         QMessageBox::warning(this, "Error", "You are playing now");
    }
    else{
        this->C_Bet = ui->C_Bet->value();
        coins-=C_Bet;

        ui->C_bar->setValue(C_LocalNum*10);
        ui->C_YourBet->setText("Your bet:"+QString::number(C_Bet));
        ui->C_WinBet->setText("You can take:"+QString::number(C_Bet*C_Multiplier));
        ui->C_Multipler->setText("Multiplier:"+QString::number(C_Multiplier));

        CrashPlay = true;
        //C_Num = qrand() % ((11 + 1) - 1) + 1;
        C_Num =CrashNum();
        WriteChanges();
        Update(getPlayer_id());
        //qDebug()<<"Cnumber:"<<C_Num;
    }
}

void MainWindow::on_C_Stop_clicked()
{
    if(CrashPlay){

        coins+=C_Bet*C_Multiplier;

        ResetCrash();
        WriteChanges();
        Update(getPlayer_id());
    }
    else{
       QMessageBox::warning(this, "Error", "Game is not active");
    }
}

void MainWindow::on_C_Next_clicked()
{
    //qDebug()<<C_LocalNum;
    //qDebug()<<C_Num;
    if(CrashPlay){
        if(C_LocalNum == 10){
           QMessageBox::warning(this, "Result", "JACKPOOT");
           coins+=C_Bet*C_Multiplier;
           AddItem(getPlayer_id(),100);
           ResetCrash();
           WriteChanges();
           Update(getPlayer_id());
           return;
        }
        if(C_LocalNum == C_Num){
          QMessageBox::warning(this, "Result", "OH NOOOOOO");
          ResetCrash();

        }
        else{
            C_Multiplier= C_Multiplier*2;
            qDebug()<<C_Multiplier;
            C_LocalNum+=1;

            ui->C_bar->setValue(C_LocalNum*10);
            ui->C_YourBet->setText("Your bet:"+QString::number(C_Bet));
            ui->C_WinBet->setText("You can take:"+QString::number(C_Bet*C_Multiplier));
            ui->C_Multipler->setText("Multiplier:"+QString::number(C_Multiplier));
        }



        WriteChanges();
        Update(getPlayer_id());
    }
    else{
       QMessageBox::warning(this, "Error", "Game is not active");
    }
}

void MainWindow::on_B1_clicked()
{
    BuyItem(getPlayer_id(),2,1);
}

void MainWindow::on_B2_clicked()
{
    BuyItem(getPlayer_id(),7,2);
}

void MainWindow::on_B150_clicked()
{
    BuyItem(getPlayer_id(),100,150);
}

void MainWindow::on_B151_clicked()
{
    BuyItem(getPlayer_id(),200,151);
}

void MainWindow::on_B30_clicked()
{
    BuyItem(getPlayer_id(),15,30);
}

void MainWindow::on_B40_clicked()
{
    BuyItem(getPlayer_id(),7,40);
}
