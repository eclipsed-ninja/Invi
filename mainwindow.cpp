

/*
 * (C) Ashish Saxena <ashish@reak.in>
 * (C) 2016 REAK INFOTECH LLP
 *
 * The LICENSE file included with the project would govern the use policy for this code,
 * In case of missing LICENSE file the code will be treated as an Intellectual Property of the creator mentioned above,
 * All rights related to distribution, modifcation, reselling, use for commercial or private use of this code is terminated.
 *
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include "dialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    updateData();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateData()
{
    ui->addproductCategory->clear();
    populateCategory();
    ui->addproductStorage->clear();
    populateStorage();
    ui->sellproductName->clear();
    populatesellName();
    ui->sellproductCount->setMinimum(1);
    ui->paymentAlert->clear();
    ui->addproductAlert->clear();
     ui->settingsAlert->clear();
     populateSettings();
//    ui->paymentclientName->clear();
//    getClients();
}

int MainWindow::populateCategory()
{
    QString path = "main.db";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");//not dbConnection
    db.setDatabaseName(path);
    db.open();
    QSqlQuery query;
    query.exec("select * from productcategory");
    query.first();
    QString init_val = query.value(0).toString();
    ui->addproductCategory->addItem(init_val);
    while(query.next())
    {
        init_val = query.value(0).toString();
        ui->addproductCategory->addItem(init_val);
    }
    db.close();
    return 0;
}

int MainWindow::populateStorage()
{
    QString path = "main.db";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");//not dbConnection
    db.setDatabaseName(path);
    db.open();
    QSqlQuery query;
    query.exec("select * from productstorage");
    query.first();
    QString init_val = query.value(0).toString();
    ui->addproductStorage->addItem(init_val);
    while(query.next())
    {
        init_val = query.value(0).toString();
        ui->addproductStorage->addItem(init_val);
    }
    db.close();
    return 0;
}

int MainWindow::addInventory(QString productName, QString productCategory, QString productCount, QString productStorage)
{
    QString path = "main.db";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");//not dbConnection
    db.setDatabaseName(path);
    db.open();
    QSqlQuery query;
    query.prepare("select * from inventory where Name=:name");
    query.bindValue(":name",productName);
    query.exec();
    if(query.first())
    {
        int dbcount = query.value(2).toInt();
        int pcount = productCount.toInt();
        pcount = pcount+dbcount;
        query.prepare("update inventory set Count=:count where Name=:name");
        query.bindValue(":count",pcount);
        query.bindValue(":name",productName);
        query.exec();
    }
    else
    {
    // Run an if check, see if the name is already there then change stock amount.
    query.prepare("insert into inventory values(:name,:category,:count,:storage)");
    query.bindValue(":name",productName);
    query.bindValue(":category",productCategory);
    query.bindValue(":count",productCount);
    query.bindValue(":storage",productStorage);
    query.exec();
    }
    db.close();
    return 0;
}

// Create new SecDialog Class from new - Qt -> Form
// SecDialog secdialog;
// secdialog.setModal(true);
// secdialog.exec();
// Remeber to CleanAll -> Run qmake

void MainWindow::on_addtoinventoryButton_clicked()
{
    QString productName = ui->addproductName->text();
    QString productCategory = ui->addproductCategory->currentText();
    QString productCount = ui->addproductCount->text();
    int pcount = productCount.toInt();
    QString productStorage = ui->addproductStorage->currentText();
    QString cname = ui->addproductcustName->text();
    QString cadd = ui->addproductcustAdd->text();
    bool payment = ui->addproductPaymentis->isChecked();
    int cost = ui->addproductCost->text().toInt();
    cost = cost * -1;
    QString pcost = QString::number(cost);
    prepareInvoice(productName, productCategory, pcount, cname, cadd, pcost, payment);
    int status = addInventory(productName, productCategory, productCount, productStorage);
    updateData();
    if (!status)
        ui->addproductAlert->setText("Product has been successfully added");
    else
        ui->addproductAlert->setText("Product has not been added, Please try again !");
}

void MainWindow::populatesellName()
{
    QString path = "main.db";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");//not dbConnection
    db.setDatabaseName(path);
    db.open();
    QSqlQuery query;
    query.exec("select Name from inventory where count>0");
    query.first();
    QString init_val = query.value(0).toString();
    ui->sellproductName->addItem(init_val);
    while(query.next())
    {
        init_val = query.value(0).toString();
        ui->sellproductName->addItem(init_val);
    }
    on_sellproductName_activated();
}

void MainWindow::on_sellproductButton_clicked()
{

    QString pname = ui->sellproductName->currentText();
    QString pcat = ui->sellproductCategory->text();
    int pcount = ui->sellproductCount->value();
    QString cname = ui->sellproductCustName->text();
    QString cadd = ui->sellproductCustAddress->text();
    QString pamount = ui->sellproductCost->text();
    bool payment = ui->sellpaymentCheck->isChecked();
    changeInventory(pname, pcount);
    prepareInvoice(pname, pcat, pcount, cname, cadd, pamount, payment);
    updateData();
    ui->sellproductAlert->setText("Item Sold, Invoice filename is : ");
}

void MainWindow::prepareInvoice(QString pname, QString pcat, int pcount, QString cname, QString cadd, QString pamount, bool payment)
{
    //Save an HTML File in MyDocuments, Add in a database aswell.
    QString path = "main.db";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");//not dbConnection
    db.setDatabaseName(path);
    db.open();
    QSqlQuery query;
    query.prepare("insert into invoice (Name, Category, Count, Merchant, Address, Amount, HasPaid) values(:pname, :pcat, :pcount, :cname, :cadd, :pamount, :payment)");
    query.bindValue(":pname", pname);
    query.bindValue(":pcat", pcat);
    query.bindValue(":pcount", pcount);
    query.bindValue(":cname", cname);
    query.bindValue(":cadd", cadd);
    query.bindValue(":pamount", pamount);
    query.bindValue(":payment", payment);
    query.exec();
    db.close();
}

int MainWindow::changeInventory(QString pname, int sellcount)
{
    QString path = "main.db";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");//not dbConnection
    db.setDatabaseName(path);
    db.open();
    QSqlQuery query;
    query.prepare("select Count from inventory where Name = :name limit 1");
    query.bindValue(":name", pname);
    query.exec();
    query.first();
    int pcount = query.value(0).toInt();
    qDebug()<<pcount;
        pcount = pcount - sellcount;
        qDebug()<<pcount;
        query.prepare("update inventory set Count = :count where Name = :name");
        query.bindValue(":count",pcount);
        query.bindValue(":name",pname);
        query.exec();
        db.close();
        return 0;

}

void MainWindow::on_reportsearchButton_clicked()
{
    QString name = ui->reportsearchText->text();
    Dialog newdialog;
    newdialog.setModal(true);
    newdialog.populateSearch(name);
    newdialog.exec();
}

void MainWindow::on_sellproductName_activated()
{
    QString currentText = ui->sellproductName->currentText();

    QString path = "main.db";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");//not dbConnection
    db.setDatabaseName(path);
    db.open();
    QSqlQuery query;
    query.prepare("select Category, Count from inventory where Name = :name");
    query.bindValue(":name",currentText);
    query.exec();
    query.first();
    QString init_val = query.value(0).toString();
    ui->sellproductCategory->setText(init_val);
    ui->sellproductCount->setMaximum(query.value(1).toInt());
    db.close();
}

void MainWindow::getClients()
{
    QString path = "main.db";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");//not dbConnection
    db.setDatabaseName(path);
    db.open();
    QSqlQuery query;
    query.prepare("select distinct Name from invoice");
    query.exec();
    query.first();
    QString var = query.value(0).toString();
//    ui->paymentclientName->addItem(var);
    while (query.next())
    {
        var = query.value(0).toString();
//        ui->paymentclientName->addItem(var);
    }
}

void MainWindow::on_paymentsAdd_clicked()
{
    QString invoiceNumber = ui->paymentinvoiceNumber->text();
    qDebug()<<invoiceNumber;
    QString path = "main.db";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");//not dbConnection
    db.setDatabaseName(path);
    db.open();
    QSqlQuery query;
    query.prepare("update invoice set HasPaid=1 where id = :id");
    query.bindValue(":id",invoiceNumber);
    query.exec();
    updateData();
    ui->paymentAlert->setText("Payment has been successfully added!");
}

void MainWindow::on_settingsSave_clicked()
{
    QString cname = ui->settingsCompanyName->text();
    QString cadd = ui->settingsCompanyAddress->text();
    QString cphone = ui->settingsCompanyPhone->text();
    QString tax = ui->settingsPercent->text();
    QString path = "main.db";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");//not dbConnection
    db.setDatabaseName(path);
    db.open();
    QSqlQuery query;
    query.prepare("update settings set name=:name, address=:address, phone=:phone, tax=:tax where id='1'");
    query.bindValue(":name",cname);
    query.bindValue(":address",cadd);
    query.bindValue(":phone",cphone);
    query.bindValue(":tax", tax);
    query.exec();
    QString pgroup = ui->settingsProductGroup->text();
    if(pgroup!="")
    {
    query.prepare("insert into productcategory values(:group)");
    query.bindValue(":group", pgroup);
    query.exec();
    updateData();
    ui->settingsAlert->setText("Settings have been saved");
    }
}

void MainWindow::populateSettings()
{
    QString path = "main.db";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");//not dbConnection
    db.setDatabaseName(path);
    db.open();
    QSqlQuery query;
    query.exec("select * from settings where id='1'");
    query.first();
    ui->settingsCompanyName->setText(query.value(1).toString());
    ui->settingsCompanyAddress->setText(query.value(2).toString());
    ui->settingsCompanyPhone->setText(query.value(3).toString());
    ui->settingsPercent->setText(query.value(0).toString());
}

void MainWindow::on_reportsshowpendingPayments_clicked()
{
    QString name = ui->reportsearchText->text();
    Dialog newdialog;
    newdialog.setModal(true);
    newdialog.populateInvoice();
    newdialog.exec();
}

