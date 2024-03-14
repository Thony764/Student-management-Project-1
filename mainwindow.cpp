#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connOpen(); // abrir base de datos

    // Simular el click del botón bt_0 con efecto
    ui->rb_todo_alu->setChecked(true);
    ui->bt_0->animateClick();

    // Simular el click del botón bt_save_alu con efecto
    ui->bt_save_alu->animateClick();
    ui->le_search_alu->setEnabled(false);
    ui->cb_sortby_alu->setEnabled(false);
    ui->bt_clean_tb_alu->setEnabled(false);

    // Conectar la señal clicked de todos los botones dentro de frame_lateral
    QList<QPushButton *> botones = ui->frame_left->findChildren<QPushButton *>();
    for (QPushButton *btn : botones)
    {
        connect(btn, &QPushButton::clicked, this, [=]()
                { actualizarEstiloBoton(btn); });

        // Establecer la política de tamaño mínimo
        btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        btn->setMinimumHeight(40);
    }

    QList<QPushButton *> botones_alu = ui->frame_opc_alu->findChildren<QPushButton *>();
    for (QPushButton *btn_alu : botones_alu)
    {
        connect(btn_alu, &QPushButton::clicked, this, [=]()
                { actualizarEstiloBoton_alu(btn_alu); });
    }

    ui->le_dni_alu_update->setPlaceholderText(QString("Inserte un DNI"));
    ui->le_dni_alu_delete->setPlaceholderText(QString("Inserte un DNI"));

    // validar line edits regsiter
    setIntValidator(ui->le_dni_alu_save);
    setIntValidator(ui->le_dni_alu_update);
    setIntValidator(ui->le_dni_alu_delete);
    setIntValidator(ui->le_dni_apo_save);
    setIntValidator(ui->le_dni_apo_update);
    setIntValidator(ui->le_dni_apo1_delete);

    // cargar los ajuestes
    MainWindow::setSettings();

    // Obtener el valor de id_rememberme de la tabla rememberme_status
    QSqlQuery query("SELECT id_rememberme FROM rememberme_status");
    if (query.exec() && query.next())
    {
        int userId = query.value(0).toInt();
        loadUser(userId);
    }
    else
    {
        // Mostrar el último error si la consulta falla
        qDebug() << "Error en la consulta SQL:" << query.lastError().text();
    }

    setShadow(ui->frame_a);
    setShadow(ui->frame_b);
    setShadow(ui->frame_c);
    setShadow(ui->bt_perfil);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setIntValidator(QLineEdit *lineEdit)
{
    QIntValidator *validator = new QIntValidator(0, 999999999, lineEdit);
    lineEdit->setValidator(validator);
}

void MainWindow::setShadow(QWidget *widget)
{
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(20);
    shadowEffect->setXOffset(5);
    shadowEffect->setYOffset(5);
    shadowEffect->setColor(QColor(0, 0, 0, 150)); // 150 para el color negro semitransparente

    widget->setGraphicsEffect(shadowEffect);
}

void MainWindow::actualizarEstiloBoton(QPushButton *botonPresionado)
{
    // Limpiar el estilo de todos los botones dentro de frame_lateral
    QList<QPushButton *> botones = ui->frame_left->findChildren<QPushButton *>();

    QString estilo = "QPushButton { }";
    for (QPushButton *btn : botones)
    {
        btn->setStyleSheet(estilo);
    }

    // Aplicar el estilo al botón presionado
    estilo = "QPushButton { "
             "border-left: 3px solid #026ec1;"
             "background-color: #0e0d14"
             " }"
             "QPushButton:hover {"
             "background-color: #0e0d14;"
             "}";
    botonPresionado->setStyleSheet(estilo);
}

// contar catidad de alumnos y calcular ganacia totoal
void MainWindow::setSettings()
{
    QSqlQuery query;
    query.prepare("SELECT * FROM settings");

    if (query.exec())
    {
        if (query.next())
        {
            precio = query.value(0).toFloat();
            vacantes = query.value(1).toInt();
            auto_refresh_tb = query.value(2).toInt();
            edit_tb_mode = query.value(3).toInt();
            hide_DB_status_bar = query.value(4).toInt();
        }
    }
    else
    {
        qDebug() << "Error al ejecutar la consulta:" << query.lastError().text();
    }

    // ESTADISTICAS

    // num registrados
    query.prepare("SELECT COUNT(*) FROM alumno");
    if (query.exec() && query.next())
    {
        registrados = query.value(0).toInt();
        ui->lb_count_alu->setText(QString::number(registrados));
    }
    else
    {
        qDebug() << "Error al obtener el número de registros de alumnos";
        return;
    }

    // ganacias total
    float gananciaTotal = registrados * precio;
    int vacantes_Total = vacantes - registrados;

    // Mostrar la ganancia total en un QLabel
    ui->lb_ganancia_total->setText(QString::number(gananciaTotal));
    // vacantes
    ui->lb_vacantes->setText(QString::number(vacantes_Total));

    // SETTINGS

    if (auto_refresh_tb == 1)
    {
        MainWindow::on_bt_refresh_alu_clicked();
    }
    else
    {
        MainWindow::on_bt_refresh_alu_clicked();
    }

    if (edit_tb_mode == 1) // editar tabla
    {
        ui->tableView_alu->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
    }
    else // no editar tabla
    {
        ui->tableView_alu->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }

    if (hide_DB_status_bar == 1)
    {
        ui->frame_DB_status_bar->hide();
    }
    else
    {
        ui->frame_DB_status_bar->show();
    }
}

void MainWindow::actualizarEstiloBoton_alu(QPushButton *botonPresionado_alu)
{
    // Limpiar el estilo de todos los botones dentro de frame_lateral
    QList<QPushButton *> botones_alu = ui->frame_opc_alu->findChildren<QPushButton *>();

    QString estilo_alu = "QPushButton { }";
    for (QPushButton *btn : botones_alu)
    {
        btn->setStyleSheet(estilo_alu);
    }

    // Aplicar el estilo al botón presionado
    estilo_alu = "QPushButton { "
                 "border-bottom: 2px solid #026ec1;"
                 "color: #f2f2f2;"

                 /*"background-color: #262630;"*/
                 "}";

    botonPresionado_alu->setStyleSheet(estilo_alu);
}

void MainWindow::on_btn_menu_clicked()
{
    if (FrameLateralCollapsed)
    {
        // Expandir el frame lateral
        ui->frame_lateral->setMaximumWidth(expandedWidth);
        ui->btn_menu->setIcon(QIcon(":/img/icon_light/menu.svg"));

        FrameLateralCollapsed = false;
    }
    else
    {
        // Colapsar el frame lateral
        ui->frame_lateral->setMaximumWidth(collapsedWidth);
        ui->btn_menu->setIcon(QIcon(":/img/icon_light/chevron-left.svg"));

        FrameLateralCollapsed = true;
    }
}

void MainWindow::on_bt_0_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_bt_1_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_bt_2_clicked()
{
    if (!checkAdminForAction())
    {
        return; // Cerrar la función
    }

    int res;
    settings pd(this);
    res = pd.exec();
    if (res == QDialog::Rejected)
        return;
}

void MainWindow::on_bt_3_clicked()
{

    // Mostrar el cuadro de diálogo "About Qt"
    QMessageBox::aboutQt(this, "About Qt");
}

// DATABASE
bool MainWindow::connOpen()
{
    myDB = QSqlDatabase::addDatabase("QSQLITE");

    // __FILE__ contiene la ruta completa al archivo fuente actual
    QString currentFile = __FILE__;

    // Crear un objeto QFileInfo con la ruta del archivo fuente
    QFileInfo fileInfo(currentFile);

    // Extraer la ruta del directorio del archivo fuente
    QString sourceDirectory = fileInfo.absolutePath();

    QString DB_path = sourceDirectory + "/EscuelaDB.db";

    myDB.setDatabaseName(DB_path);

    if (!myDB.open())
    {
        qDebug() << "Failed to open the database";
        ui->lb_DB_status_bar->setText("[!] Database does not exist");
        return false;
    }
    else
    {
        qDebug() << "Connected to data base.";
        ui->lb_DB_status_bar->setText("[+] Connected to Database");
        return true;
    }
}

void MainWindow::connClose()
{
    myDB.close();
    myDB.removeDatabase(QSqlDatabase::defaultConnection);
}

void MainWindow::set_table_alu()
{

    mModel = new QSqlTableModel(this);
    mModel->setTable("alumno");

    // Configurar las cabeceras y combinar apellidos en una sola columna
    mModel->setHeaderData(0, Qt::Horizontal, "ID");
    mModel->setHeaderData(1, Qt::Horizontal, "DNI");
    mModel->setHeaderData(2, Qt::Horizontal, "Nombres");
    mModel->setHeaderData(3, Qt::Horizontal, "Apellido Paterno");
    mModel->setHeaderData(4, Qt::Horizontal, "Apellido Materno");
    mModel->setHeaderData(5, Qt::Horizontal, "Edad");
    mModel->setHeaderData(6, Qt::Horizontal, "Grado");
    mModel->setHeaderData(7, Qt::Horizontal, "Apoderado");

    mModel->select();

    ui->tableView_alu->setModel(mModel);

    // Ocultar la columna ID
    // Conexiones para las funciones de validación
    connect(mModel, &QSqlTableModel::dataChanged, this, &MainWindow::before_alu_edit);

    ui->tableView_alu->setColumnHidden(0, true);

    // tabla_2
    //  Crear un nuevo modelo para la tabla 2
    mModel2 = new QSqlTableModel(this);
    mModel2->setTable("alumno");

    mModel2->setHeaderData(0, Qt::Horizontal, "ID");
    mModel2->setHeaderData(1, Qt::Horizontal, "DNI");
    mModel2->setHeaderData(2, Qt::Horizontal, "Nombres");
    mModel2->setHeaderData(3, Qt::Horizontal, "Apellido Paterno");
    mModel2->setHeaderData(4, Qt::Horizontal, "Apellido Materno");
    mModel2->setHeaderData(5, Qt::Horizontal, "Edad");
    mModel2->setHeaderData(6, Qt::Horizontal, "Grado");
    mModel2->setHeaderData(7, Qt::Horizontal, "Apoderado");
    mModel2->select();

    ui->tableView_alu_2->setModel(mModel2);
    ui->tableView_alu_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

bool MainWindow::checkAdminForAction()
{
    if (loginStatus == false)
    {
        QMessageBox::warning(this, "Advertencia", "Necesitas una cuenta de administrador para esta acción.");
        return false; // Indica falta de cuenta de administrador
    }

    return true; // Indica que se tiene una cuenta de administrador
}

void MainWindow::before_alu_edit(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    Q_UNUSED(roles); // Indicar que el parámetro roles no se utiliza intencionalmente

    // Verificar si la edición está ocurriendo en la columna correspondiente a 'dni_alu'
    int dniAluColumnIndex = mModel->fieldIndex("dni_alu");
    if (dniAluColumnIndex != -1 && topLeft.column() == dniAluColumnIndex && bottomRight.column() == dniAluColumnIndex)
    {
        // Obtener el nuevo valor de 'dni_alu'
        QString nuevoDNI = mModel->data(topLeft).toString();

        // Realizar una consulta para verificar si el nuevo DNI ya existe en la tabla 'alumno' (excepto en la fila actual)
        QSqlQuery queryDNI;
        queryDNI.prepare("SELECT COUNT(*) FROM alumno WHERE dni_alu = ? AND id_alu != ?");
        queryDNI.bindValue(0, nuevoDNI);
        queryDNI.bindValue(1, mModel->data(mModel->index(topLeft.row(), mModel->fieldIndex("id_alu"))).toInt());

        if (!queryDNI.exec() || (queryDNI.next() && queryDNI.value(0).toInt() > 0))
        {
            // Si hay un error en la consulta o si el DNI ya está registrado para otro alumno, muestra un mensaje de error y revierte los cambios
            QMessageBox::critical(this, "Error", "El DNI está registrado por otro alumno.");
            mModel->revertRow(topLeft.row());
            return;
        }
    }

    // Verificar si la edición está ocurriendo en la columna correspondiente a 'edad_alu'
    int edadAluColumnIndex = mModel->fieldIndex("edad_alu");
    if (edadAluColumnIndex != -1 && topLeft.column() == edadAluColumnIndex && bottomRight.column() == edadAluColumnIndex)
    {
        // Obtener el nuevo valor de 'edad_alu'
        int nuevaEdad = mModel->data(topLeft).toInt();

        // Verificar si la nueva edad es válida
        if (nuevaEdad > 100 || nuevaEdad < 0) // rango de edades válido
        {
            // Si la edad no está dentro del rango válido, muestra un mensaje de error y revierte los cambios
            QMessageBox::critical(this, "Error", "Edad no válida.");
            mModel->revertRow(topLeft.row());
            return;
        }
    }

    // Verificar si la edición está ocurriendo en la columna correspondiente a 'num_grad2' (grado)
    int gradoColumnIndex = mModel->fieldIndex("num_grad2");
    if (gradoColumnIndex != -1 && topLeft.column() == gradoColumnIndex && bottomRight.column() == gradoColumnIndex)
    {
        // Obtener el nuevo valor de 'num_grad2'
        int nuevoGrado = mModel->data(topLeft).toInt();

        // Realizar una consulta para verificar si el nuevo grado existe en la tabla 'grado'
        QSqlQuery queryGrado;
        queryGrado.prepare("SELECT COUNT(*) FROM grado WHERE num_grad = ?");
        queryGrado.bindValue(0, nuevoGrado);

        if (!queryGrado.exec() || (queryGrado.next() && queryGrado.value(0).toInt() == 0))
        {
            // Si hay un error en la consulta o el grado no existe en la tabla 'grado', muestra un mensaje de error y revierte los cambios
            QMessageBox::critical(this, "Error", "Grado inválido [1-6].");
            mModel->revertRow(topLeft.row());
            return;
        }
    }

    // Verificar si la edición está ocurriendo en la columna correspondiente a 'dni_apo'
    int dniApoColumnIndex = mModel->fieldIndex("dni_apo");
    if (dniApoColumnIndex != -1 && topLeft.column() == dniApoColumnIndex && bottomRight.column() == dniApoColumnIndex)
    {
        // Obtener el nuevo valor de 'dni_apo'
        QString nuevoDNI = mModel->data(topLeft).toString();

        // Realizar una consulta para verificar si el nuevo DNI ya existe en la tabla 'apoderado' (excepto en la fila actual)
        QSqlQuery queryDNI;
        queryDNI.prepare("SELECT COUNT(*) FROM alumno WHERE dni_apo = ?");
        queryDNI.bindValue(0, nuevoDNI);

        if (!queryDNI.exec() || (queryDNI.next() && queryDNI.value(0).toInt() > 0))
        {
            // Si hay un error en la consulta o si el DNI ya está registrado para otro apoderado, muestra un mensaje de error y revierte los cambios
            QMessageBox::critical(this, "Error", "El DNI está registrado por otro apoderado.");
            mModel->revertRow(topLeft.row());
            return;
        }
    }
}

void MainWindow::on_bt_refresh_alu_clicked()
{
    MainWindow::set_table_alu();
    MainWindow::bt_search_alu();

    ui->le_search_alu->setEnabled(true);
    ui->le_search_alu->setPlaceholderText(QString("Buscar..."));

    ui->cb_sortby_alu->setEnabled(true);
    ui->bt_clean_tb_alu->setEnabled(true);
}

void MainWindow::on_bt_alu_check_save_clicked()
{
    if (!checkAdminForAction())
    {
        return; // Cerrar la función
    }

    // Verificar si todos los campos están vacíos
    if (ui->le_dni_alu_save->text().isEmpty() ||
        ui->le_nombre_alu_save->text().isEmpty() ||
        ui->le_apellidoP_alu_save->text().isEmpty() ||
        ui->le_apellidoM_alu_save->text().isEmpty() ||
        ui->le_edad_alu_save->text().isEmpty() ||
        ui->le_dni_apo_save->text().isEmpty() ||
        ui->cb_num_grad2_save->currentText().isEmpty())
    {
        QMessageBox::warning(this, "Advertencia", "Todos los campos son obligatorios.");
        return; // No proceder con el guardado
    }

    // Verificar si el DNI del alumno ya existe en la tabla alumno
    QString dni_alumno = ui->le_dni_alu_save->text();
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM alumno WHERE dni_alu = ?");
    query.addBindValue(dni_alumno);
    if (query.exec() && query.next())
    {
        int count = query.value(0).toInt();
        if (count > 0)
        {
            QMessageBox::warning(this, "Advertencia", "El DNI del alumno ya existe en la tabla alumno.");
            return; // No proceder con el guardado
        }
    }
    else
    {
        QMessageBox::critical(this, "Error", "Error al verificar el DNI del alumno.");
        return; // No proceder con el guardado
    }

    // Si todas las verificaciones son exitosas, proceder con el guardado
    // Realizar la inserción en la tabla alumno
    query.prepare("INSERT INTO alumno (dni_alu, nombre_alu, apellidoP_alu, apellidoM_alu, edad_alu, num_grad2, dni_apo) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(ui->le_dni_alu_save->text());
    query.addBindValue(ui->le_nombre_alu_save->text());
    query.addBindValue(ui->le_apellidoP_alu_save->text());
    query.addBindValue(ui->le_apellidoM_alu_save->text());
    query.addBindValue(ui->le_edad_alu_save->text());
    query.addBindValue(ui->cb_num_grad2_save->currentText().toInt());
    query.addBindValue(ui->le_dni_apo_save->text());

    if (query.exec())
    {
        QMessageBox::information(this, "Éxito", "Datos del alumno guardados exitosamente.");
        // auto refrescar tabla
        if (auto_refresh_tb == 1)
        {
            MainWindow::on_bt_refresh_alu_clicked();
        }
    }
    else
    {
        QMessageBox::critical(this, "Error", "Error al guardar los datos del alumno.");
        qDebug() << "Error SQL:" << query.lastError().text();
    }
}

void MainWindow::on_bt_save_alu_clicked()
{
    ui->stackedWidget_alu->setCurrentIndex(0);
}

void MainWindow::alu_update_le_off()
{

    // Desactivar el QComboBox
    ui->cb_num_grad2_update->setEnabled(false);

    // Recorrer los widgets dentro de frame_regsiter_alu_update
    foreach (QWidget *widget, ui->frame_update_alu_action->findChildren<QWidget *>())
    {
        if (widget != ui->le_dni_alu_update && widget != ui->lb_dni_alu_update)
        {                              // Dejar activo solo el QLineEdit del DNI
            widget->setEnabled(false); // Desactivar el widget
            widget->setStyleSheet("QLineEdit { "
                                  "border-bottom: 1px solid #666666 ;"
                                  "background-color: transparent;"
                                  "}"
                                  "QComboBox {"
                                  "background-color: transparent;}");
        }
    }
}

void MainWindow::alu_delete_le_off()
{

    // Desactivar el QComboBox
    ui->cb_num_grad2_delete->setEnabled(false);

    // Recorrer los widgets dentro de frame_regsiter_alu_update
    foreach (QWidget *widget, ui->frame_delete_alu_action->findChildren<QWidget *>())
    {
        if (widget != ui->le_dni_alu_delete && widget != ui->lb_dni_alu_delete)
        {                              // Dejar activo solo el QLineEdit del DNI
            widget->setEnabled(false); // Desactivar el widget
            widget->setStyleSheet("QLineEdit { "
                                  "color: #a0a0a0; "
                                  "border-bottom: 1px solid #666666;"
                                  "background-color: transparent;"
                                  "}"
                                  "QComboBox {"
                                  "background-color: transparent;}");
        }
    }
}

void MainWindow::alu_update_le_on()
{

    // Activar todos los widgets dentro de frame_regsiter_alu_update
    foreach (QWidget *widget, ui->frame_update_alu_action->findChildren<QWidget *>())
    {
        widget->setEnabled(true); // Activar el widget
        widget->setStyleSheet("QLineEdit { "
                              "border: 0px;"
                              "color: white; /* Texto blanco */"
                              "border-bottom: 1px solid #0580d9; /* Borde inferior azul de 1px */"
                              "background-color: transparent; /* Fondo transparente */"
                              "}"
                              "QLineEdit:focus {"
                              "border-bottom: 1px solid #ffffff; /* Borde inferior blanco cuando el QLineEdit está enfocado */"
                              "}");
    }

    // Activar el QComboBox
    ui->cb_num_grad2_update->setEnabled(true);
}

void MainWindow::on_bt_update_alu_clicked()
{
    ui->stackedWidget_alu->setCurrentIndex(1);
    if (current_id_alu_update == -2)
    {
        MainWindow::alu_update_le_off();
    }
}

void MainWindow::on_bt_delete_alu_clicked()
{

    ui->stackedWidget_alu->setCurrentIndex(2);

    if (alu_delete_changed == true)
    {
        QMessageBox::warning(this, "Advertencia", "Los datos han sido modificados");
        alu_delete_changed = false;
    }

    if (ui->le_dni_alu_delete->text().isEmpty())
    {
        MainWindow::alu_delete_le_off();
    }
}

void MainWindow::update_load_alumno()
{
    QString dni_alumno = ui->le_dni_alu_update->text();

    // Verificar si el DNI del alumno existe en la tabla alumno
    QSqlQuery query;
    query.prepare("SELECT * FROM alumno WHERE dni_alu = ?");
    query.addBindValue(dni_alumno);
    if (!query.exec())
    {
        QMessageBox::critical(this, "Error", "Error al ejecutar la consulta.");
        return; // No proceder con la carga de datos
    }

    if (!query.next())
    {

        if (!ui->le_dni_alu_update->text().isEmpty() && !ui->le_nombre_alu_update->isEnabled())
        {
            QMessageBox::warning(this, "Advertencia", "No se encontraron coicidencias.");
        }
        return; // No proceder con la carga de datos
    }

    // Si el DNI existe, cargar los datos en los LineEdit y ComboBox

    ui->le_nombre_alu_update->setText(query.value(2).toString());
    ui->le_apellidoP_alu_update->setText(query.value(3).toString());
    ui->le_apellidoM_alu_update->setText(query.value(4).toString());
    ui->le_edad_alu_update->setText(query.value(5).toString());
    ui->cb_num_grad2_update->setCurrentText(query.value(6).toString());
    ui->le_dni_apo_update->setText(query.value(7).toString());

    // Guardar el ID del alumno cargado en la variable current_id_alu
    current_id_alu_update = query.value(0).toInt();

    MainWindow::alu_update_le_on();
}

void MainWindow::get_current_id_alu_delete(QString dni_alu)
{
    // get id
    QSqlQuery query;
    query.prepare("SELECT id_alu FROM alumno WHERE dni_alu = ?");
    query.addBindValue(dni_alu);
    if (!query.exec())
    {
        QMessageBox::critical(this, "Error", "Error al ejecutar la consulta.");
        return;
    }

    if (!query.next())
    {
        QMessageBox::warning(this, "Advertencia", "No se encontraron coincidencias.");
        return; // Agregado para salir de la función si no se encontraron resultados
    }

    current_id_alu_delete = query.value(0).toInt();
}

void MainWindow::delete_load_alumno(QString dni_alu_delete)
{

    // Verificar si el DNI del alumno existe en la tabla alumno
    QSqlQuery query;
    query.prepare("SELECT * FROM alumno WHERE dni_alu = ?");
    query.addBindValue(dni_alu_delete);
    if (!query.exec())
    {
        QMessageBox::critical(this, "Error", "Error al ejecutar la consulta.");
        return; // No proceder con la carga de datos
    }

    if (!query.next())
    {

        if (ui->le_dni_alu_delete->text().isEmpty() && current_id_alu_delete == -1)
        {

            ui->le_dni_alu_delete->setText(revert_alu_dni);
            return;
        }
        else
        {
            QMessageBox::warning(this, "Advertencia", "No se encontraron coincidencias.");

            if (current_id_alu_delete != -1)
            {
                ui->le_dni_alu_delete->setText(revert_alu_dni);
            }
            return;
        }
    }

    // Si el DNI existe, cargar los datos en los LineEdit y ComboBox
    ui->le_dni_alu_delete->setText(query.value(1).toString());
    ui->le_nombre_alu_delete->setText(query.value(2).toString());
    ui->le_apellidoP_alu_delete->setText(query.value(3).toString());
    ui->le_apellidoM_alu_delete->setText(query.value(4).toString());
    ui->le_edad_alu_delete->setText(query.value(5).toString());
    ui->cb_num_grad2_delete->setCurrentText(query.value(6).toString());
    ui->le_dni_apo1_delete->setText(query.value(7).toString());

    revert_alu_dni = ui->le_dni_alu_delete->text();

    // get el ID del alumno cargado en la variable current_id_alu_delete
    MainWindow::get_current_id_alu_delete(ui->le_dni_alu_delete->text());
}

void MainWindow::on_bt_alu_check_update_clicked()
{

    if (!checkAdminForAction())
    {
        return; // Cerrar la función
    }

    if (ui->le_dni_alu_update->text().isEmpty())
    {
        QMessageBox::warning(this, "Advertencia", "Por favor ingrese un DNI.");
        return; // No proceder con la eliminación
    }

    // MainWindow::update_load_alumno();

    // Verificar si todos los campos están vacíos
    if (ui->le_dni_alu_update->text().isEmpty() ||
        ui->le_nombre_alu_update->text().isEmpty() ||
        ui->le_apellidoP_alu_update->text().isEmpty() ||
        ui->le_apellidoM_alu_update->text().isEmpty() ||
        ui->le_edad_alu_update->text().isEmpty() ||
        ui->le_dni_apo_update->text().isEmpty() ||
        ui->cb_num_grad2_update->currentText().isEmpty())
    {
        QMessageBox::warning(this, "Advertencia", "Todos los campos son obligatorios.");
        return; // No proceder con la actualización
    }

    // Verificar si el nuevo DNI ya está registrado por otro alumno ademas que sireve para la condicion
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM alumno WHERE dni_alu = ? AND id_alu != ?");
    query.addBindValue(ui->le_dni_alu_update->text());
    query.addBindValue(current_id_alu_update);
    if (!query.exec())
    {
        QMessageBox::critical(this, "Error", "Error al verificar el DNI del alumno.");
        return; // No proceder con la actualización
    }

    if (query.next())
    {
        int count = query.value(0).toInt();
        if (count > 1)
        {
            QMessageBox::warning(this, "Advertencia", "El nuevo DNI ya está registrado por otro alumno.");
            return; // No proceder con la actualización
        }
    }

    // Si todas las verificaciones son exitosas, proceder con la actualización
    query.prepare("UPDATE alumno SET dni_alu = ?, nombre_alu = ?, apellidoP_alu = ?, apellidoM_alu = ?, edad_alu = ?, num_grad2 = ?, dni_apo = ? WHERE id_alu = ?");
    query.addBindValue(ui->le_dni_alu_update->text());
    query.addBindValue(ui->le_nombre_alu_update->text());
    query.addBindValue(ui->le_apellidoP_alu_update->text());
    query.addBindValue(ui->le_apellidoM_alu_update->text());
    query.addBindValue(ui->le_edad_alu_update->text());
    query.addBindValue(ui->cb_num_grad2_update->currentText().toInt());
    query.addBindValue(ui->le_dni_apo_update->text());
    query.addBindValue(current_id_alu_update);

    if (query.exec())
    {
        QMessageBox::information(this, "Éxito", "Datos del alumno actualizados correctamente.");

        if (current_id_alu_update == current_id_alu_delete)
        {
            alu_delete_changed = true;
            MainWindow::delete_load_alumno(ui->le_dni_alu_update->text());
        }
        MainWindow::on_bt_alu_x_update_clicked();
        current_id_alu_update = -2; // reiniciar id_alu
        // auto refrescar tabla
        if (auto_refresh_tb == 1)
        {
            MainWindow::on_bt_refresh_alu_clicked();
        }
    }
    else
    {
        QMessageBox::critical(this, "Error", "Error al actualizar los datos del alumno.");
    }
}

void MainWindow::on_bt_alu_check_delete_clicked()
{
    if (!checkAdminForAction())
    {
        return; // Cerrar la función
    }

    if (registrados > vacantes)
    {
        QMessageBox::critical(this, "Error", "No hay suficientes vacantes.");
        return;
    }

    // Verificar que el DNI no esté vacío
    if (ui->le_dni_alu_delete->text().isEmpty())
    {
        QMessageBox::warning(this, "Advertencia", "Por favor ingrese un DNI.");
        return; // No proceder con la eliminación
    }

    // Obtener el ID del alumno a eliminar
    MainWindow::get_current_id_alu_delete(ui->le_dni_alu_delete->text());

    // Verificar si el alumno está en uso
    if (current_id_alu_update == current_id_alu_delete)
    {
        QMessageBox::critical(this, "Error", "No se puede eliminar el alumno porque está en uso.");
        return;
    }

    // Eliminar el alumno
    QSqlQuery query;
    query.prepare("DELETE FROM alumno WHERE dni_alu = ?");
    query.addBindValue(ui->le_dni_alu_delete->text());
    if (query.exec())
    {
        QMessageBox::information(this, "Éxito", "Alumno eliminado correctamente.");
        MainWindow::on_bt_alu_x_delete_clicked(); // Limpiar los campos después de eliminar
        current_id_alu_delete = -1;
        // auto refrescar tabla
        if (auto_refresh_tb == 1)
        {
            MainWindow::on_bt_refresh_alu_clicked();
        }
    }
    else
    {
        QMessageBox::critical(this, "Error", "Error al eliminar el alumno.");
    }
}

void MainWindow::on_bt_alu_x_save_clicked()
{
    // Limpiar los LineEdit y ComboBox
    ui->le_dni_alu_save->clear();
    ui->le_nombre_alu_save->clear();
    ui->le_apellidoP_alu_save->clear();
    ui->le_apellidoM_alu_save->clear();
    ui->le_edad_alu_save->clear();
    ui->cb_num_grad2_save->setCurrentIndex(0);
    ui->le_dni_apo_save->clear();
}

void MainWindow::on_bt_alu_x_update_clicked()
{
    current_id_alu_update = -2; // reiniciar id_alu
    // Limpiar los LineEdit y ComboBox
    ui->le_dni_alu_update->clear();
    ui->le_nombre_alu_update->clear();
    ui->le_apellidoP_alu_update->clear();
    ui->le_apellidoM_alu_update->clear();
    ui->le_edad_alu_update->clear();
    ui->cb_num_grad2_update->setCurrentIndex(0);
    ui->le_dni_apo_update->clear();

    MainWindow::alu_update_le_off();
}

void MainWindow::on_bt_alu_x_delete_clicked()
{
    // Limpiar los LineEdit y ComboBox
    ui->le_dni_alu_delete->clear();
    ui->le_nombre_alu_delete->clear();
    ui->le_apellidoP_alu_delete->clear();
    ui->le_apellidoM_alu_delete->clear();
    ui->le_edad_alu_delete->clear();
    ui->cb_num_grad2_delete->setCurrentIndex(0);
    ui->le_dni_apo1_delete->clear();

    revert_alu_dni = "";
    current_id_alu_delete = -1;
}

void MainWindow::on_bt_clean_tb_alu_clicked()
{
    if (!checkAdminForAction())
    {
        return; // Cerrar la función
    }

    // Confirmar con el usuario antes de limpiar la tabla
    QMessageBox::StandardButton confirmation;
    confirmation = QMessageBox::question(this, "Confirmación", "¿Está seguro de que desea eliminar todos los registros de la tabla alumno?", QMessageBox::Yes | QMessageBox::No);

    if (confirmation == QMessageBox::Yes)
    {
        // Eliminar todos los registros de la tabla alumno
        QSqlQuery query;
        if (query.exec("DELETE FROM alumno"))
        {
            QMessageBox::information(this, "Éxito", "Se han eliminado todos los registros de la tabla alumno.");
            // auto refrescar tabla
            if (auto_refresh_tb == 1)
            {
                MainWindow::on_bt_refresh_alu_clicked();
            }
        }
        else
        {
            QMessageBox::critical(this, "Error", "Error al limpiar la tabla alumno.");
        }
    }
}
void MainWindow::bt_search_alu()
{
    QString filtro = ui->le_search_alu->text();
    QString campo;

    // Determinar el campo de búsqueda según el radio botón seleccionado
    if (ui->rb_dni_alu->isChecked())
    {
        campo = "dni_alu";
    }
    else if (ui->rb_nombre_alu->isChecked())
    {
        campo = "nombre_alu";
    }
    else if (ui->rb_grado_alu->isChecked())
    {
        campo = "num_grad2";
    }
    else if (ui->rb_edad_alu->isChecked())
    {
        campo = "edad_alu";
    }

    // Construir la consulta de filtro
    QString filtroQuery;
    if (ui->rb_apellidos_alu->isChecked())
    {
        // Si se selecciona el filtro por apellidos, buscar en ambos campos de apellidos
        filtroQuery = QString("(apellidoP_alu LIKE '%%1%' OR apellidoM_alu LIKE '%%1%')").arg(filtro);
    }
    else if (ui->rb_todo_alu->isChecked())
    {
        // Si se selecciona el filtro "Todo", buscar en todos los campos
        filtroQuery = QString("nombre_alu LIKE '%%1%' OR apellidoP_alu LIKE '%%1%' OR apellidoM_alu LIKE '%%1%' OR dni_alu LIKE '%%1%' OR num_grad2 LIKE '%%1%' OR edad_alu LIKE '%%1%'").arg(filtro);
    }
    else if (campo == "edad_alu")
    {
        // Si se selecciona el filtro por edad, buscar cualquier valor
        filtroQuery = "1 = 1"; // Consulta siempre verdadera
    }
    else
    {
        // Para otros campos, utilizar el filtro LIKE
        filtroQuery = QString("%1 LIKE '%%2%'").arg(campo, filtro);
    }

    // Aplicar el filtro y actualizar la vista
    mModel->setFilter(filtroQuery);
    mModel->select();

    MainWindow::on_cb_sortby_alu_currentIndexChanged(index_sortby);
}

void MainWindow::on_cb_sortby_alu_currentIndexChanged(int index)
{
    QString orderByColumn;
    Qt::SortOrder order = Qt::AscendingOrder; // Por defecto, orden ascendente

    switch (index)
    {
    case 0: // Mantener "Ordenar por" sin cambios
        break;
    case 1: // Más reciente (por ID, de forma descendente)
        orderByColumn = "id_alu";
        order = Qt::DescendingOrder;
        break;
    case 2: // Más antiguo (por ID, de forma ascendente)
        orderByColumn = "id_alu";
        order = Qt::AscendingOrder;
        break;
    case 3: // Nombre A-Z
        orderByColumn = "nombre_alu";
        break;
    case 4: // Nombre Z-A
        orderByColumn = "nombre_alu";
        order = Qt::DescendingOrder;
        break;
    case 5: // Apellido Paterno A-Z
        orderByColumn = "apellidoP_alu";
        break;
    case 6: // Apellido Paterno Z-A
        orderByColumn = "apellidoP_alu";
        order = Qt::DescendingOrder;
        break;
    default:
        break;
    }

    // Configurar el orden
    if (!orderByColumn.isEmpty())
    {
        mModel->setSort(mModel->fieldIndex(orderByColumn), order);
        mModel->select();

        index_sortby = index;
    }
}

void MainWindow::on_le_dni_alu_delete_editingFinished()
{
    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(QApplication::focusWidget());
    if (lineEdit)
    {
        MainWindow::delete_load_alumno(lineEdit->text());
    }
}

void MainWindow::on_le_dni_alu_update_editingFinished()
{
    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(QApplication::focusWidget());
    if (lineEdit)
    {
        if (current_id_alu_update == -2)
        {
            MainWindow::update_load_alumno();
        }
    }
}

void MainWindow::on_le_search_alu_editingFinished()
{
    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(QApplication::focusWidget());
    if (lineEdit)
    {
        MainWindow::bt_search_alu();
    }
}

void MainWindow::on_bt_login_clicked()
{
    login *Log = new login(this);
    Log->open();
}

void MainWindow::loadUser(int id_user)
{

    if (id_user == 0)
    {

        ui->lb_username->clear();
        ui->lb_gmail->clear();

        ui->bt_login->show();
        ui->bt_logout->hide();
    }
    else
    {
        QSqlQuery query;
        query.prepare("SELECT username, gmail FROM user WHERE id_user = ?");
        query.addBindValue(id_user);
        if (!query.exec() || !query.next())
        {
            QMessageBox::critical(this, "Error", "Error al cargar los datos del usuario.");
            return;
        }

        loginStatus = true;

        QString username = query.value(0).toString();
        QString gmail = query.value(1).toString();

        // Mostrar los datos en los QLabel lb_username y lb_gmail
        ui->lb_username->setText(username);
        ui->lb_gmail->setText(gmail);
        ui->bt_login->hide();

        ui->bt_logout->show();
    }
}

void MainWindow::on_bt_logout_clicked()
{
    login *Log = new login(this);
    Log->open();

    Log->logOut_mode(ui->lb_username->text());
}
