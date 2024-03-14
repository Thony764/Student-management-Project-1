#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QMessageBox>
#include <QtSql>
#include <QDoubleValidator>
#include <QIntValidator>
#include "mainwindow.h"

namespace Ui
{
    class settings;
}

class settings : public QDialog
{
    Q_OBJECT
public:
    explicit settings(QWidget *parent = 0);
    ~settings();
    void setCheckStatus();

private slots:
    void on_btx_settings_accepted();
    void on_btx_settings_rejected();
    void on_le_vacantes_editingFinished();

private:
    Ui::settings *ui;
};

#endif // SETTINGS_H
