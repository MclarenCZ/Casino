#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QItemSelectionModel>
#include <QDebug>
#include <QModelIndex>
#include "login.h"
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QItemSelectionModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int getPlayer_id() const;
    void setPlayer_id(int value);
    enum BetColor {Black, Red, Green};

public slots:
    void Update(int id_player);
    void AddItem(int id_player,int id_item);
    void DeleteItem();
    void SelectInfo(int id);
    void LoadInventory(int id);
    void WriteChanges();
    void ResetCrash();
    int CrashNum();
    void BuyItem(int id_player, int buyvalue, int id_item);



private slots:
    void on_sellButton_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_pushButton_2_clicked();

    void on_B_BetButton_clicked();

    void on_C_Start_clicked();

    void on_C_Stop_clicked();

    void on_C_Next_clicked();

    void on_B1_clicked();

    void on_B2_clicked();

    void on_B150_clicked();

    void on_B151_clicked();

    void on_B30_clicked();

    void on_B40_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlTableModel *model;
    QSqlTableModel *itemListModel;
    QSqlQueryModel *viewmodel;
    int player_id;
    QString player_name;
    int coins;
    int lvl;

    int C_Bet;
    bool CrashPlay = false;
    int C_Num = 0;
    int C_LocalNum=1;
    int C_Multiplier=1;

};

#endif // MAINWINDOW_H
