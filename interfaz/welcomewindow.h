#ifndef WELCOMEWINDOW_H
#define WELCOMEWINDOW_H

#include <QWidget>
#include <QTcpServer>
#include <QList>
#include <QTcpSocket>
#include <QTimer>
#include <QPushButton>
#include <QLabel>
#include <QMap> // Para manejar usuarios y saldos
#include <QString>
#include <QPropertyAnimation>
#include "qledlabel.h" // Indicador LED
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

#define PORTsERVER  10234
#define TARJETACODIGO "03 A5 C6 12 72" // Código esperado

class WelcomeWindow : public QWidget {
    Q_OBJECT

public:
    explicit WelcomeWindow(QWidget *parent = nullptr);
    void updateClientBalance(const QString &clientName, double updatedBalance);
    ~WelcomeWindow();

signals:
    void clientVerified(const QString &clientName, double balance); // Señal para cambiar a MainWindow

private slots:
    void onNewConnection(); // Manejar nuevas conexiones TCP
    void onReadyRead();     // Manejar datos recibidos
    void onStateChanged(QAbstractSocket::SocketState state);

private:
    QTcpServer server;
    QTcpSocket *socket;
    QLedLabel *ledStatus;         // Indicador LED para el estado de la tarjeta
    QTimer *blinkTimer;           // Temporizador para animación del LED
    QLabel *dynamicMessage;       // Mensaje dinámico
    QLabel *connectionStatus;     // Estado de conexión
    QPropertyAnimation *logoAnimation; // Animación para el logo
    QList<QTcpSocket*> sockets;   // Lista de sockets para manejar múltiples conexiones
    QLabel *clientCountLabel;     // Mostrar la cantidad de clientes conectados

    bool validarChecksum(const QString &trama);

    // Base de datos de clientes
    QMap<QString, double> clientsDatabase; // clave: clientName, valor: balance
    void initializeDatabase();            // Inicializa la base de datos

    // Métodos para manejar el archivo JSON
    void loadDatabaseFromJson();           // Cargar base de datos desde JSON
    void saveDatabaseToJson();             // Guardar base de datos en JSON
};

#endif // WELCOMEWINDOW_H
