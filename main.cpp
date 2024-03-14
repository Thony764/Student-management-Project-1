#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    a.setStyle("fusion");
    w.show();
    // __FILE__ contiene la ruta completa al archivo fuente actual
    QString currentFile = __FILE__;

    // Crear un objeto QFileInfo con la ruta del archivo fuente
    QFileInfo fileInfo(currentFile);

    // Extraer la ruta del directorio del archivo fuente
    QString sourceDirectory = fileInfo.absolutePath();

    qDebug() << "Data base path:" << sourceDirectory;

    return a.exec();
}
