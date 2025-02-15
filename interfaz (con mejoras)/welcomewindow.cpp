#include "welcomewindow.h"
#include "qledlabel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QMessageBox>
#include <QPalette>
#include <QPixmap>
#include <QDebug>



WelcomeWindow::WelcomeWindow(QWidget *parent) : QWidget(parent), socket(nullptr) {
    setWindowTitle("Bienvenido al Centro de Lavado");
    setFixedSize(480, 480);

    // Configuración de la imagen de fondo
    QPalette palette;
    QPixmap background("C:/Users/Frave/Desktop/interfaz/logo.jpg"); // Cambiar por la ruta adecuada
    palette.setBrush(QPalette::Window, background.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    setPalette(palette);

    // Layout principal
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Encabezado
    QLabel *headerLabel = new QLabel("Sistema de Lavado Inteligente", this);
    headerLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: white; margin-top: 10px;");
    headerLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(headerLabel);

    // Imagen central
    QLabel *imageLabel = new QLabel(this);
    QPixmap image(":/images/logo.jpg"); // Cambiar a la ruta correcta
    imageLabel->setPixmap(image.scaled(300, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imageLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(imageLabel);

    // Animación del logo
    logoAnimation = new QPropertyAnimation(imageLabel, "geometry");
    logoAnimation->setDuration(2000);
    logoAnimation->setStartValue(QRect(imageLabel->x() - 10, imageLabel->y() - 10, imageLabel->width() + 20, imageLabel->height() + 20));
    logoAnimation->setEndValue(imageLabel->geometry());
    logoAnimation->setEasingCurve(QEasingCurve::InOutQuad);
    logoAnimation->setLoopCount(-1);
    logoAnimation->start();

    // Espaciador flexible entre la imagen y la comprobación
    mainLayout->addStretch();

    // Cargar la base de datos desde el archivo JSON
    loadDatabaseFromJson();

    // Si la base de datos está vacía, inicializar valores básicos
    if (clientsDatabase.isEmpty()) {
        initializeDatabase();
        saveDatabaseToJson(); // Guardar los datos iniciales
    }

// Configuración del servidor TCP
connect(&server, &QTcpServer::newConnection, this, &WelcomeWindow::onNewConnection);

// Manejo de errores del servidor
connect(&server, &QTcpServer::acceptError, this, [this]() {
    qDebug() << "Error del servidor TCP:" << server.errorString();
    QMessageBox::critical(this, "Error del Servidor", "Ocurrió un error en la conexión: " + server.errorString());
});

if (!server.listen(QHostAddress::Any, PORTsERVER)) {
    QMessageBox::critical(this, "Error", "No se pudo iniciar el servidor TCP.");
    qDebug() << "Error al iniciar el servidor TCP:" << server.errorString();
}

//Conteo Clientes
clientCountLabel = new QLabel("Clientes conectados: 0", this);
clientCountLabel->setStyleSheet("font-size: 16px; color: white;");
clientCountLabel->setAlignment(Qt::AlignRight);
mainLayout->addWidget(clientCountLabel);

// Contenedor para texto y LED
QHBoxLayout *bottomLayout = new QHBoxLayout();

QLabel *welcomeLabel = new QLabel("Por favor, pase su tarjeta RFID:", this);
welcomeLabel->setStyleSheet("font-family: 'Times New Roman'; font-size: 20px; font-weight: bold; color: white;");
welcomeLabel->setAlignment(Qt::AlignLeft);

ledStatus = new QLedLabel(this);
ledStatus->setState(QLedLabel::LED_GRAY);
ledStatus->setFixedSize(30, 30); // Tamaño del LED

bottomLayout->addWidget(welcomeLabel);
bottomLayout->addWidget(ledStatus);

// Botón de ayuda
QPushButton *helpButton = new QPushButton("Ayuda", this);
helpButton->setStyleSheet("font-size: 14px; color: white; background-color: #0078D7; padding: 5px 10px; border-radius: 5px; border: 2px solid white;");
helpButton->setIcon(QIcon(":/icons/help.png")); // Icono opcional
connect(helpButton, &QPushButton::clicked, this, [this]() {
    // Crear una nueva ventana emergente para mostrar la ayuda
    QWidget *helpWindow = new QWidget();
    helpWindow->setWindowTitle("Ayuda - Sistema de Lavado");
    helpWindow->setFixedSize(400, 300);

    QVBoxLayout *layout = new QVBoxLayout(helpWindow);

    QLabel *infoLabel = new QLabel("¿Necesitas ayuda?", helpWindow);
    infoLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: black;");
    infoLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(infoLabel);

    QLabel *detailsLabel = new QLabel(
        "1. Asegúrate de que tu tarjeta RFID esté disponible.\n"
        "2. Si tienes problemas, verifica la conexión del lector.\n"
        "3. Para más información, contacta al soporte técnico.",
        helpWindow);
    detailsLabel->setStyleSheet("font-size: 14px; color: black;");
    detailsLabel->setWordWrap(true);
    layout->addWidget(detailsLabel);

    QPushButton *testConnectionButton = new QPushButton("Probar conexión al lector", helpWindow);
    testConnectionButton->setStyleSheet("font-size: 14px; color: white; background-color: green; padding: 5px 10px; border-radius: 5px;");
    layout->addWidget(testConnectionButton);

    // Botón para cerrar la ventana de ayuda
    QPushButton *closeButton = new QPushButton("Cerrar", helpWindow);
    closeButton->setStyleSheet("font-size: 14px; color: white; background-color: red; padding: 5px 10px; border-radius: 5px;");
    layout->addWidget(closeButton);

    connect(closeButton, &QPushButton::clicked, helpWindow, &QWidget::close);

    // Funcionalidad para probar la conexión
    connect(testConnectionButton, &QPushButton::clicked, this, [this]() {
        if (server.isListening()) {
            QMessageBox::information(nullptr, "Prueba de conexión", "El lector RFID está conectado correctamente.");
        } else {
            QMessageBox::critical(nullptr, "Prueba de conexión", "El lector RFID no está conectado. Verifica los cables o la configuración.");
        }
    });

    helpWindow->setLayout(layout);
    helpWindow->show();
});

bottomLayout->addStretch();
bottomLayout->addWidget(helpButton);
mainLayout->addLayout(bottomLayout);

// Mensaje dinámico
dynamicMessage = new QLabel("Esperando tarjeta RFID...", this);
dynamicMessage->setStyleSheet("font-size: 16px; color: white; font-style: italic;");
dynamicMessage->setAlignment(Qt::AlignCenter);
mainLayout->addWidget(dynamicMessage);

QTimer *messageTimer = new QTimer(this);
connect(messageTimer, &QTimer::timeout, this, [this]() {
    static bool toggle = false;
    dynamicMessage->setText(toggle ? "Por favor, pase su tarjeta RFID..." : "Esperando tarjeta RFID...");
    toggle = !toggle;
});
messageTimer->start(2000);

// Estado de conexión
connectionStatus = new QLabel("Estado: Conectando...", this);
connectionStatus->setStyleSheet("font-size: 14px; color: lightgreen;");
connectionStatus->setAlignment(Qt::AlignRight);
mainLayout->addWidget(connectionStatus);

connect(&server, &QTcpServer::newConnection, this, [this]() {
    connectionStatus->setText("Estado: Conectado");
});
connect(&server, &QTcpServer::acceptError, this, [this]() {
    connectionStatus->setText("Estado: Error en la conexión");
});

setLayout(mainLayout);

// Animación del LED
blinkTimer = new QTimer(this);
connect(blinkTimer, &QTimer::timeout, this, [this]() {
    static bool on = false;
    ledStatus->setState(on ? QLedLabel::LED_GRAY : QLedLabel::LED_BLUE_ON);
    on = !on;
});
blinkTimer->start(1000);

// Configuración del servidor TCP
connect(&server, &QTcpServer::newConnection, this, &WelcomeWindow::onNewConnection);

if (!server.listen(QHostAddress::Any, PORTsERVER)) {
    QMessageBox::critical(this, "Error", "No se pudo iniciar el servidor TCP.");
    qDebug() << "Error al iniciar el servidor TCP:" << server.errorString();
}
}

QTcpServer* WelcomeWindow::getServer() {
    return &server; // Devuelve un puntero al servidor
}


// Cargar base de datos desde un archivo JSON
void WelcomeWindow::loadDatabaseFromJson() {
    QFile file("clients_database.json");
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Base de Datos", "No se pudo cargar la base de datos. Se utilizará una base vacía.");
        qDebug() << "No se pudo abrir el archivo JSON.";
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
    QJsonObject jsonObj = jsonDoc.object();

    for (const QString &key : jsonObj.keys()) {
        clientsDatabase.insert(key, jsonObj.value(key).toDouble());
    }

    qDebug() << "Base de datos cargada desde JSON:" << clientsDatabase;
}

// Guardar base de datos en un archivo JSON
void WelcomeWindow::saveDatabaseToJson() {
    QFile file("clients_database.json");
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "No se pudo abrir el archivo JSON para guardar.";
        return;
    }

    QJsonObject jsonObj;
    for (auto it = clientsDatabase.begin(); it != clientsDatabase.end(); ++it) {
        jsonObj.insert(it.key(), it.value());
    }

    QJsonDocument jsonDoc(jsonObj);
    file.write(jsonDoc.toJson());
    file.close();

    qDebug() << "Base de datos guardada en JSON.";
}

void WelcomeWindow::initializeDatabase() {
    // Aquí defines a los clientes iniciales y sus saldos
    clientsDatabase.insert("Juan", 10.00); // Ejemplo: Cliente "Juan" con $10.00
    clientsDatabase.insert("Ana", 20.00);
    clientsDatabase.insert("Carlos", 15.50);
}

void WelcomeWindow::onNewConnection() {
    QTcpSocket *clientSocket = server.nextPendingConnection();
    connect(clientSocket, &QTcpSocket::readyRead, this, &WelcomeWindow::onReadyRead);
    connect(clientSocket, &QTcpSocket::stateChanged, this, &WelcomeWindow::onStateChanged);
    sockets.append(clientSocket);
    qDebug() << "Nueva conexión establecida.";
    clientCountLabel->setText("Clientes conectados: " + QString::number(sockets.size()));


    QTimer *timeoutTimer = new QTimer(this);
    timeoutTimer->setSingleShot(true);
    connect(timeoutTimer, &QTimer::timeout, this, [this, clientSocket]() {
        if (clientSocket->state() == QAbstractSocket::ConnectedState) {
            clientSocket->disconnectFromHost();
            qDebug() << "Cliente desconectado por inactividad.";
        }
    });
    timeoutTimer->start(600000); // 60 segundos de inactividad
}


void WelcomeWindow::onReadyRead() {
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    QByteArray data = socket->readAll();
    QString msg = QString::fromUtf8(data).trimmed();

    qDebug() << "Mensaje recibido de cliente:" << msg;

    // Trama esperada para comparación
    const QString expectedCardCode = "03 A5 C6 12 72";
    const QString fran = "08 49 01 3B 7B";

    if (msg == expectedCardCode || msg == fran) {
        // Cliente válido
        double balance = clientsDatabase.value(expectedCardCode, 10.0); // Recuperar saldo, predeterminado en 0.0 si no existe
        QString clientName = expectedCardCode; // Usa el código de tarjeta como nombre

        qDebug() << "Cliente válido:" << clientName << "con saldo:" << balance;

        // Actualizar LED y mensaje dinámico
        blinkTimer->stop();
        ledStatus->setState(QLedLabel::LED_GREEN_ON);
        dynamicMessage->setText(QString("Acceso concedido. Bienvenido, %1.").arg(clientName));

        // Emitir señal para abrir MainWindow
        activeSocket = socket; // Guardar el socket activo
        emit clientVerified(clientName, balance); // Emitir la señal hacia MainWindow

        // Responder al cliente con un mensaje de éxito
        QString respuesta = ">L,B,1,<\n";
        socket->write(respuesta.toUtf8());

        // Guardar la base de datos actualizada
        saveDatabaseToJson();
    } else {
        // Cliente no válido o trama incorrecta
        qDebug() << "Código inválido o no reconocido:" << msg;

        // Encender el LED rojo y actualizar el mensaje dinámico
        ledStatus->setState(QLedLabel::LED_RED_ON);
        dynamicMessage->setText("Acceso denegado. Inténtelo nuevamente.");

        // Responder al cliente con un mensaje de error
        QString respuesta = ">L,B,0,<\n";
        socket->write(respuesta.toUtf8());
    }

    // Reiniciar el parpadeo del LED
    blinkTimer->start(1000);
}
bool WelcomeWindow::validarChecksum(const QString &trama) {
    char chk = 0;
    for (int i = 1; i < trama.size() - 2; i++) {
        chk ^= trama[i].toLatin1();
    }
    return (chk == trama[trama.size() - 2].toLatin1());
}

void WelcomeWindow::onStateChanged(QAbstractSocket::SocketState state) {
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (state == QAbstractSocket::UnconnectedState && socket) {
        sockets.removeAll(socket);
        socket->deleteLater();
        qDebug() << "Conexión finalizada.";
        clientCountLabel->setText("Clientes conectados: " + QString::number(sockets.size()));
    }
}

void WelcomeWindow::updateClientBalance(const QString &clientName, double updatedBalance) {
    if (clientsDatabase.contains(clientName)) {
        clientsDatabase[clientName] = updatedBalance; // Actualizar saldo
        saveDatabaseToJson(); // Guardar los cambios en el archivo JSON
        qDebug() << "Saldo actualizado para" << clientName << ":" << updatedBalance;
    } else {
        qDebug() << "Cliente no encontrado:" << clientName;
    }
}


WelcomeWindow::~WelcomeWindow() {
    /*server.close();

    for (QTcpSocket *socket : sockets) {
        if (socket == activeSocket) {
            // No limpiar el socket en uso por MainWindow
            continue;
        }

        if (socket->state() == QAbstractSocket::ConnectedState) {
            socket->disconnectFromHost();
            if (socket->state() != QAbstractSocket::UnconnectedState) {
                socket->waitForDisconnected();
            }
        }
        socket->deleteLater();
    }

    sockets.clear();
    qDebug() << "Destructor de WelcomeWindow ejecutado correctamente.";*/
}
