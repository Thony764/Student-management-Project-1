#include "login.h"
#include "ui_login.h"

login::login(QWidget *parent)
    : QDialog(parent), ui(new Ui::login)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());                                    // Esto evita que la ventana se pueda redimensionar
    this->setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint); // Esto permite que la ventana se pueda mover

    login::signIn_mode();

    ui->le_gmail->setPlaceholderText(QString("Gmail"));
    ui->le_username->setPlaceholderText(QString("Username"));
    ui->le_password->setPlaceholderText(QString("Password"));

    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(20);
    shadowEffect->setXOffset(5);
    shadowEffect->setYOffset(5);
    shadowEffect->setColor(QColor(0, 0, 0, 150)); // 150 para el color negro semitransparente

    ui->bt_login_sumbit_out->setGraphicsEffect(shadowEffect);

    // Limitar el QLineEdit le_password para aceptar solo números
    QIntValidator *validator = new QIntValidator(this);
    ui->le_password->setValidator(validator);

    // Limitar el QLineEdit le_username para no aceptar espacios en blanco
    QRegularExpressionValidator *usernameValidator = new QRegularExpressionValidator(QRegularExpression("[^\\s]+"), this);
    ui->le_username->setValidator(usernameValidator);

    // Limitar el QLineEdit le_gmail para no aceptar espacios en blanco
    QRegularExpressionValidator *gmailValidator = new QRegularExpressionValidator(QRegularExpression("[^\\s]+"), this);
    ui->le_gmail->setValidator(gmailValidator);
}

login::~login()
{
    delete ui;
}

void login::on_bt_eye_clicked()
{
    if (eye_status)
    {
        ui->le_password->setEchoMode(QLineEdit::Normal);        // Mostrar contraseña
        ui->bt_eye->setIcon(QIcon(":/img/icon_light/eye.svg")); // Cambiar ícono a ojo abierto
        eye_status = false;
    }
    else
    {
        ui->le_password->setEchoMode(QLineEdit::Password);          // Ocultar contraseña
        ui->bt_eye->setIcon(QIcon(":/img/icon_light/eye-off.svg")); // Cambiar ícono a ojo cerrado
        eye_status = true;
    }
}

void login::on_bt_eye_toggled(bool checked)
{
    if (checked)
    {
        ui->le_password->setEchoMode(QLineEdit::Normal);        // Mostrar contraseña
        ui->bt_eye->setIcon(QIcon(":/img/icon_light/eye.svg")); // Cambiar ícono a ojo abierto
    }
    else
    {
        ui->le_password->setEchoMode(QLineEdit::Password);          // Ocultar contraseña
        ui->bt_eye->setIcon(QIcon(":/img/icon_light/eye-off.svg")); // Cambiar ícono a ojo cerrado
    }
}

void login::sign_in()
{
    // Verificar que los campos no estén vacíos
    if (ui->le_username->text().isEmpty() || ui->le_password->text().isEmpty())
    {
        QMessageBox::warning(this, "Error", "Por favor ingrese nombre de usuario y contraseña.");
        return;
    }

    // Verificar si el usuario existe
    QSqlQuery query;
    query.prepare("SELECT id_user FROM user WHERE username = ? AND password = ?");
    query.addBindValue(ui->le_username->text());
    query.addBindValue(ui->le_password->text());
    if (!query.exec())
    {
        QMessageBox::critical(this, "Error", "Error al verificar el usuario.");
        return;
    }

    if (query.next())
    {
        QMessageBox::information(this, "Éxito", "Inicio de sesión exitoso.");

        int userId = query.value(0).toInt();

        // Obtener una referencia al MainWindow existente
        MainWindow *mainWindow = qobject_cast<MainWindow *>(parent());
        if (mainWindow)
        {
            mainWindow->loadUser(userId);
        }
        else
        {
            qDebug() << "No se pudo obtener una referencia válida a MainWindow.";
        }

        if (ui->chb_rememberme->isChecked())
        {
            query.prepare("UPDATE rememberme_status SET id_rememberme = ?");
            query.addBindValue(userId);
        }
        else
        {
            query.prepare("UPDATE rememberme_status SET id_rememberme = 0");
        }

        if (!query.exec())
        {
            qDebug() << "Error al actualizar el estado de remember me";
        }

        login::close();
    }
    else
    {
        QMessageBox::warning(this, "Error", "Usurio o contraseña son incorrectos.");
    }
}

void login::sign_up()
{
    // Verificar que los campos no estén vacíos
    if (ui->le_username->text().isEmpty() || ui->le_password->text().isEmpty() || ui->le_gmail->text().isEmpty())
    {
        QMessageBox::warning(this, "Error", "Por favor ingrese nombre de usuario, contraseña y correo electrónico.");
        return;
    }

    // Verificar si el usuario ya existe
    QSqlQuery query;
    query.prepare("SELECT id_user FROM user WHERE username = ?");
    query.addBindValue(ui->le_username->text());
    if (!query.exec())
    {
        QMessageBox::critical(this, "Error", "Error al verificar el usuario.");
        return;
    }

    if (query.next())
    {
        QMessageBox::warning(this, "Error", "El usuario ya existe.");
        return;
    }

    // Insertar el nuevo usuario
    query.prepare("INSERT INTO user (username, password, gmail) VALUES (?, ?, ?)");
    QString gmail = ui->le_gmail->text() + "@gmail.com";
    query.addBindValue(ui->le_username->text());
    query.addBindValue(ui->le_password->text());
    query.addBindValue(gmail);
    if (!query.exec())
    {
        QMessageBox::critical(this, "Error", "Error al insertar el usuario.");
        return;
    }

    QMessageBox::information(this, "Éxito", "Usuario insertado correctamente.");
    login::signIn_mode();
}

void login::sign_out()
{
    // Obtener una referencia al MainWindow existente
    MainWindow *mainWindow = qobject_cast<MainWindow *>(parent());

    if (ui->chb_rememberme->isChecked())
    {

        // Verificar que los campos no estén vacíos
        if (ui->le_password->text().isEmpty())
        {
            QMessageBox::warning(this, "Error", "Por favor ingrese contraseña.");
            return;
        }

        // Eliminar el usuario
        QSqlQuery deleteQuery;
        deleteQuery.prepare("DELETE FROM user WHERE username = ? AND password = ?");
        deleteQuery.addBindValue(ui->le_username->text());
        deleteQuery.addBindValue(ui->le_password->text());
        if (!deleteQuery.exec())
        {
            QMessageBox::critical(this, "Error", "Error al eliminar el usuario.");
            return;
        }
    }

    // Actualizar el estado de rememberme_status
    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE rememberme_status SET id_rememberme = 0");
    if (!updateQuery.exec())
    {
        qDebug() << "Error al actualizar el estado de remember me";
        return;
    }

    // Cargar usuario
    int userId = 0;
    mainWindow->loadUser(userId);
    mainWindow->loginStatus = false;

    // Cerrar ventana de login
    close();
}

void login::on_bt_login_sumbit_out_clicked()
{
    if (singInUpOut == 1) // Sing In
    {
        login::sign_in();
    }
    else if (singInUpOut == 2) // Sing Up
    {
        login::sign_up();
    }
    else if (singInUpOut == 3) // Sing Out
    {
        login::sign_out();
    }
}

void login::signIn_mode()
{
    singInUpOut = 1;
    ui->lb_signInUp->setText("Sing In");
    ui->frame_gamil->hide();
    ui->chb_rememberme->show();
    ui->chb_rememberme->setText("remember me");

    ui->bt_signUp->show();
    ui->bt_login_sumbit_out->setText("Login");
    ui->label_msg->setText("Not an admin?");

    ui->bt_login_sumbit_out->setStyleSheet("QPushButton{};");
    QString estilo;
    estilo = "QPushButton { "
             "font: 10pt 'Montserrat'; "
             "font-weight: bold; "
             "color: #f2f2f2; "
             "background-color: rgb(5, 128, 217); "
             "border-radius: 15px; "
             "}";
    ui->bt_login_sumbit_out->setStyleSheet(estilo);
}

void login::signUp_mode()
{
    singInUpOut = 2;
    ui->lb_signInUp->setText("Sing Up");
    ui->frame_gamil->show();
    ui->chb_rememberme->hide();
    ui->bt_signUp->hide();
    ui->bt_login_sumbit_out->setText("Sumbit");
    ui->label_msg->setText("¡Welcome!");

    ui->bt_login_sumbit_out->setStyleSheet("QPushButton{};");

    QString estilo;
    estilo = "QPushButton { "
             "font: 10pt 'Montserrat'; "
             "font-weight: bold; "
             "color: #f2f2f2; "
             "background-color: rgb(5, 128, 217); "
             "border-radius: 15px; "
             "}";
    ui->bt_login_sumbit_out->setStyleSheet(estilo);
}

void login::logOut_mode(QString userName)
{
    singInUpOut = 3;

    QSqlQuery query;
    query.prepare("SELECT gmail, username FROM user WHERE username = ?");
    query.addBindValue(userName);
    if (!query.exec() || !query.next())
    {
        QMessageBox::critical(this, "Error", "Error al cargar los datos del usuario.");
        return;
    }

    QString gmail = query.value(0).toString();
    QString username = query.value(1).toString();

    // Mostrar los datos en los QLabel lb_username y lb_gmail
    ui->le_gmail->setText(gmail);
    ui->le_username->setText(username);
    ui->lb_signInUp->setText("Sing Out");
    ui->frame_gamil->show();
    ui->frame_password->hide();
    ui->lb_gamil_com->hide();
    ui->chb_rememberme->show();
    ui->chb_rememberme->setText("Also delete account");
    ui->bt_signUp->hide();
    ui->bt_login_sumbit_out->setText("Log out");
    ui->label_msg->setText("Good bye!");

    ui->bt_login_sumbit_out->setStyleSheet("QPushButton{};");

    QString estilo;
    estilo = "QPushButton { "
             "font: 10pt 'Montserrat'; "
             "font-weight: bold; "
             "color: #f2f2f2; "
             "background-color: #dd4747; "
             "border-radius: 15px; "
             "}";
    ui->bt_login_sumbit_out->setStyleSheet(estilo);

    foreach (QWidget *widget, ui->frame_name_gmail->findChildren<QWidget *>())
    {
        if (widget != ui->le_password)
        {
            widget->setEnabled(false); // Desactivar el widget
            widget->setStyleSheet("QLineEdit { "
                                  "color: #dedede; "
                                  "border-bottom: 1px solid #666666;"
                                  "background-color: transparent;"
                                  "}");
        }
    }
}

void login::on_bt_signUp_clicked()
{
    login::signUp_mode();
}

// para si quiere elimar tambien la cuenta de usuario o adimin
void login::on_chb_rememberme_clicked(bool checked)
{
    if (ui->lb_gamil_com->isHidden())
    {
        if (checked)
        {
            ui->frame_password->show();
        }
        else
        {
            ui->frame_password->hide();
        }
    }
}
