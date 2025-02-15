#include <QApplication>
#include "welcomewindow.h"
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Crear la ventana de bienvenida
    WelcomeWindow welcomeWindow;
    MainWindow *mainWindow = nullptr;

    // Conectar la señal de cliente verificado a la creación de MainWindow
    QObject::connect(&welcomeWindow, &WelcomeWindow::clientVerified, [&](const QString &clientName, double balance) {
        // Crear MainWindow después de la verificación
        mainWindow = new MainWindow(clientName, balance);

        // Conectar para regresar a WelcomeWindow al cerrar sesión
        QObject::connect(mainWindow, &MainWindow::sessionEnded, [&]() {
            welcomeWindow.updateClientBalance(clientName, mainWindow->getClientBalance());
            mainWindow->close();
            delete mainWindow;
            mainWindow = nullptr;
            welcomeWindow.show();
        });

        // Ocultar WelcomeWindow y mostrar MainWindow
        welcomeWindow.hide();
        mainWindow->show();
    });

    // Mostrar WelcomeWindow al inicio
    welcomeWindow.show();

    return app.exec();
}
