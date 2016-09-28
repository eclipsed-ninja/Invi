

/*
 * (C) Ashish Saxena <ashish@reak.in>
 * (C) 2016 REAK INFOTECH LLP
 *
 * The LICENSE file included with the project would govern the use policy for this code,
 * In case of missing LICENSE file the code will be treated as an Intellectual Property of the creator mentioned above,
 * All rights related to distribution, modifcation, reselling, use for commercial or private use of this code is terminated.
 *
 */
#include "dialog.h"
#include "ui_dialog.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::populateInvoice()
{
    QString path = "main.db";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");//not dbConnection
    db.setDatabaseName(path);
    db.open();
    QSqlQuery query;
    query.prepare("select id, Name,Category,Count,Merchant,Address,Amount from invoice where HasPaid='0'");
    query.exec();
    QSqlQueryModel *modal = new QSqlQueryModel();
    modal->setQuery(query);
    ui->tableView->setModel(modal);
}

void Dialog::populateSearch(QString Search)
{
    QString path = "main.db";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");//not dbConnection
    db.setDatabaseName(path);
    db.open();
    QSqlQuery query;
    query.prepare("select Name,Category,Count,Storage from inventory where Name=:name OR Category=:category");
    query.bindValue(":name",Search);
    query.bindValue(":category",Search);
    query.exec();
    QSqlQueryModel *modal = new QSqlQueryModel();
    modal->setQuery(query);
    ui->tableView->setModel(modal);
}


