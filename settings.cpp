#include "settings.h"
#include "ui_settings.h"

settings::settings(QWidget *parent) : QDialog(parent),ui(new Ui::settings)
{
    ui->setupUi(this);

    this->setFixedSize(this->size());                                    // Esto evita que la ventana se pueda redimensionar
    this->setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint); // Esto permite que la ventana se pueda mover

    // Crear un QDoubleValidator para aceptar números enteros o decimales
    QDoubleValidator *validator = new QDoubleValidator(0, 9999999, 2, ui->le_precio); // Rango de 0 a 9999999, con 2 decimales
    validator->setNotation(QDoubleValidator::StandardNotation);                       // Establecer la notación estándar

    QIntValidator *intValidator = new QIntValidator(this);
    intValidator->setBottom(0); // Establece el valor mínimo permitido
    ui->le_vacantes->setValidator(intValidator);

    settings::setCheckStatus();
}
settings::~settings()
{
    delete ui;
}

void settings::on_btx_settings_accepted()
{
    if (ui->le_vacantes->text().isEmpty() || ui->le_precio->text().isEmpty())
    {
        QMessageBox::critical(this, "Error", "Campos vacios.");
        return;
    }
    // Preparar la consulta SQL y ejecutarla
    QSqlQuery query;
    query.prepare("UPDATE settings SET precio_matricula = ?, num_vacantes = ?, auto_refresh_tb = ?, edit_tb_mode = ?, hide_DB_status_bar = ?");
    query.addBindValue(ui->le_precio->text().toFloat());
    query.addBindValue(ui->le_vacantes->text().toInt());
    query.addBindValue(ui->chexb_auto_refresh_tb->isChecked() ? 1 : 0);
    query.addBindValue(ui->chxb_edit_tb_mode->isChecked() ? 1 : 0);
    query.addBindValue(ui->chxb_hide_DB_status_bar->isChecked() ? 1 : 0);

    if (!query.exec())
    {
        qDebug() << "Error al ejecutar la consulta:" << query.lastError().text();
        QMessageBox::critical(this, "Error", "Error al actualizar la configuración.");
        return;
    }

    MainWindow *settingsptr = qobject_cast<MainWindow *>(parent()); // Obtener el puntero a la MainWindow

    // Actualizar la configuración en la MainWindow
    settingsptr->setSettings();

    // Cerrar la ventana de configuración
    close();
}

void settings::on_btx_settings_rejected()
{
    close();
}

void settings::setCheckStatus()
{

    QString precio;
    QString vacantes;
    int auto_refresh_tb;
    int edit_tb_mode;
    int hide_DB_status_bar;

    QSqlQuery query;
    query.prepare("SELECT * FROM settings");

    if (query.exec())
    {
        if (query.next())
        {
            precio = query.value(0).toString();
            vacantes = query.value(1).toString();
            auto_refresh_tb = query.value(2).toInt();
            edit_tb_mode = query.value(3).toInt();
            hide_DB_status_bar = query.value(4).toInt();
        }
    }
    else
    {
        qDebug() << "Error al ejecutar la consulta:" << query.lastError().text();
    }

    ui->le_precio->setText(precio);
    ui->le_vacantes->setText(vacantes);

    if (auto_refresh_tb == 1)
    {
        ui->chexb_auto_refresh_tb->setChecked(true); // se marca
    }
    else
    {
        ui->chexb_auto_refresh_tb->setChecked(false); // se desmarca
    }

    if (edit_tb_mode == 1)
    {
        ui->chxb_edit_tb_mode->setChecked(true); // se marca
    }
    else
    {
        ui->chxb_edit_tb_mode->setChecked(false); // se desmarca
    }

    if (hide_DB_status_bar == 1)
    {
        ui->chxb_hide_DB_status_bar->setChecked(true); // se marca
    }
    else
    {
        ui->chxb_hide_DB_status_bar->setChecked(false); // se desmarca
    }
}

void settings::on_le_vacantes_editingFinished()
{
    MainWindow *settingsptr = qobject_cast<MainWindow *>(parent()); // Obtener el puntero a la MainWindow
    if (ui->le_vacantes->text().toInt() < settingsptr->registrados)
    {
        QMessageBox::critical(this, "Error", "El numero de regsitrados actuales supera la cifra.");
        ui->le_vacantes->setText(QString::number(settingsptr->vacantes));
    }
}
