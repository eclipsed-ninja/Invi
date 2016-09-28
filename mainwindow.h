#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_addtoinventoryButton_clicked();

    void on_reportsearchButton_clicked();

    void on_sellproductButton_clicked();

    void on_sellproductName_activated();

    void on_paymentsAdd_clicked();

    void on_settingsSave_clicked();

    void on_reportsshowpendingPayments_clicked();

private:
    Ui::MainWindow *ui;
    int populateCategory();
    int populateStorage();
    int addInventory(QString, QString, QString, QString);
    void populatesellName();
    void updateData();
    int changeInventory(QString pname, int sellcount);
    void prepareInvoice(QString, QString, int, QString, QString, QString, bool payment);
    void getClients();
    void populateSettings();
};

#endif // MAINWINDOW_H
