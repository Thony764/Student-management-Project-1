#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QMainWindow>
#include <QtSql>
#include <QDebug>
#include <QFileInfo>
#include <QMessageBox>
#include <QLineEdit>
#include <QSqlTableModel>
#include <QGraphicsColorizeEffect>
#include <QIntValidator>
#include <QKeyEvent>
#include "login.h"
#include "settings.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QSqlDatabase myDB;
    int current_id_alu_delete = -1; // vacio
    int current_id_alu_update = -2; // vacio
    QString revert_alu_dni;
    bool alu_delete_changed = false;
    bool loginStatus = false;

    // estadistica
    int registrados;
    // settings
    float precio;
    int vacantes;
    int auto_refresh_tb;
    int edit_tb_mode;
    int hide_DB_status_bar;
    void connClose();
    bool connOpen();
    void set_table_alu();
    void alu_update_le_off();
    void alu_update_le_on();
    void alu_delete_le_off();
    void delete_load_alumno(QString dni_alu_delete);
    void get_current_id_alu_delete(QString dni_alu);
    void bt_search_alu();
    void setIntValidator(QLineEdit *lineEdit);
    void loadUser(int id_user);
    bool checkAdminForAction();
    void setShadow(QWidget *widget);
    void setSettings();
private slots:
    void on_btn_menu_clicked();
    void actualizarEstiloBoton(QPushButton *botonPresionado);
    void actualizarEstiloBoton_alu(QPushButton *botonPresionado_alu);
    void on_bt_1_clicked();
    void on_bt_0_clicked();
    void on_bt_2_clicked();
    void on_bt_3_clicked();
    void on_bt_refresh_alu_clicked();
    void on_bt_alu_check_save_clicked();
    void on_bt_save_alu_clicked();
    void on_bt_update_alu_clicked();
    void on_bt_delete_alu_clicked();
    void update_load_alumno();
    void on_bt_alu_check_update_clicked();
    void on_bt_alu_check_delete_clicked();
    void on_bt_alu_x_save_clicked();
    void on_bt_alu_x_update_clicked();
    void on_bt_alu_x_delete_clicked();
    void on_bt_clean_tb_alu_clicked();
    void on_cb_sortby_alu_currentIndexChanged(int index);
    void before_alu_edit(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);
    void on_le_dni_alu_delete_editingFinished();
    void on_le_dni_alu_update_editingFinished();
    void on_le_search_alu_editingFinished();
    void on_bt_login_clicked();
    void on_bt_logout_clicked();

private:
    Ui::MainWindow *ui;
    QString originalButton;
    QString borderLeftButton;
    QString removeBorderLeft;
    bool FrameLateralCollapsed = false;
    const int collapsedWidth = 43;
    const int expandedWidth = 16777215;
    QSqlTableModel *mModel;
    QSqlTableModel *mModel2;
    int index_sortby = 0;
};
#endif // MAINWINDOW_H
