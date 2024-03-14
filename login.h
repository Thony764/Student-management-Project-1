#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QGraphicsColorizeEffect>
#include <QtSql>
#include <QMessageBox>
#include <QIntValidator>
#include <QRegularExpressionValidator>
#include "mainwindow.h"

namespace Ui
{
    class login;
}

class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();

    int singInUpOut = 1; // Sing In, 2 Sing Up
    void sign_in();
    void sign_up();
    void sign_out();
    void signIn_mode();
    void signUp_mode();
    void logOut_mode(QString userName);

private slots:
    void on_bt_eye_clicked();
    void on_bt_eye_toggled(bool checked);
    void on_bt_login_sumbit_out_clicked();
    void on_bt_signUp_clicked();
    void on_chb_rememberme_clicked(bool checked);

private:
    Ui::login *ui;
    bool eye_status;
};

#endif // LOGIN_H
