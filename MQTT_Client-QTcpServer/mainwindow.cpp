#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>


using namespace Qt::StringLiterals;

int flag_habilitacion = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Sistema de Lavado Inteligente");

    // Crear el widget central
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Configurar el QLabel para la imagen de fondo
    backgroundLabel = new QLabel(this);
    backgroundLabel->setPixmap(QPixmap("C:/Users/SinstuTror/Desktop/Interfaz/Interfaz Con Base/interfaz/logo.jpg")
                                   .scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    backgroundLabel->setScaledContents(true);
    backgroundLabel->lower();
    backgroundLabel->setGeometry(0, 0, width(), height());

    // Configurar el título del sistema
    QLabel *titleLabel = new QLabel("Sistema de Lavado Inteligente", centralWidget);
    titleLabel->setStyleSheet(
        "font-size: 26px; "  // Tamaño más grande
        "font-weight: bold; " // Negrita
        "color: white; " // Texto en blanco
        "background-color: transparent; " // Fondo transparente
        "padding: 10px;" // Espaciado interno
        );
    titleLabel->setAlignment(Qt::AlignCenter);

    // **Aplicar sombra al texto**
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect();
    shadowEffect->setBlurRadius(10);  // Radio de desenfoque de la sombra
    shadowEffect->setColor(Qt::black); // Color negro para la sombra
    shadowEffect->setOffset(2, 2);  // Desplazamiento de la sombra

    titleLabel->setGraphicsEffect(shadowEffect);

    // Layout superior con título
    QVBoxLayout *headerLayout = new QVBoxLayout();
    headerLayout->addWidget(titleLabel, 0, Qt::AlignTop | Qt::AlignCenter);

    // Personalizar estilos de los botones
    ui->pushButton_LedGreen->setStyleSheet(
        "QPushButton {"
        "background-color: rgb(85, 170, 85);"
        "color: white;"
        "border-radius: 10px;"
        "padding: 10px;"
        "font-size: 16px;"
        "border: 2px solid transparent;"
        "font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "border: 2px solid rgb(85, 255, 85);"
        "}"
        );
    ui->pushButton_LedRed->setStyleSheet(
        "QPushButton {"
        "background-color: rgb(255, 120, 0);"
        "color: white;"
        "border-radius: 10px;"
        "padding: 10px;"
        "font-size: 16px;"
        "border: 2px solid transparent;"
        "font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "border: 2px solid rgb(255, 180, 0);"
        "}"
        );
    ui->pushButton_LedBlue->setStyleSheet(
        "QPushButton {"
        "background-color: rgb(50, 50, 200);"
        "color: white;"
        "border-radius: 10px;"
        "padding: 10px;"
        "font-size: 16px;"
        "border: 2px solid transparent;"
        "font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "border: 2px solid rgb(100, 100, 255);"
        "}"
        );

    // Personalizar estilo del botón de parada de emergencia
    ui->paradaButton->setStyleSheet(
        "QPushButton {"
        "background-color: rgb(255, 0, 0);"
        "color: white;"
        "border-radius: 5px;"
        "padding: 20px;"
        "font-size: 20px;"
        "font-weight: bold;"
        "border: 3px solid transparent;"
        "}"
        "QPushButton:hover {"
        "background-color: rgb(255, 50, 50);"
        "border: 3px solid rgb(255, 255, 0);"
        "}"
        "QPushButton:pressed {"
        "background-color: rgb(200, 0, 0);"
        "border: 3px solid rgb(255, 150, 150);"
        "}"
        );

    // Crear un nuevo layout horizontal para los botones
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(ui->pushButton_LedGreen);
    buttonLayout->addWidget(ui->pushButton_LedRed);
    buttonLayout->addWidget(ui->pushButton_LedBlue);

    // Layout del botón de parada de emergencia
    QVBoxLayout *emergencyLayout = new QVBoxLayout();
    emergencyLayout->addWidget(ui->paradaButton);

    // Layout inferior con botón de ayuda
    QPushButton *helpButton = new QPushButton("Ayuda", centralWidget);
    helpButton->setStyleSheet(
        "font-size: 16px; color: white; background-color: #0066CC; padding: 8px 15px; "
        "border-radius: 10px; border: 2px solid #FFFFFF; font-family: 'Arial';"
        );

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->setContentsMargins(10, 0, 10, 10);
    bottomLayout->addWidget(helpButton);
    bottomLayout->addStretch();

    // Layout de LED y estado
    statusLabel = new QLabel("En espera", this);
    statusLabel->setStyleSheet("font-size: 20px; color: white;font-weight: bold;");
    statusLabel->setAlignment(Qt::AlignCenter);

    ledLabel = new QLabel(this);
    ledLabel->setFixedSize(20, 20);
    ledLabel->setStyleSheet(
        "background-color: yellow; "
        "border-radius: 20px; "
        "border: 2px solid black;" // Agregar borde al LED
        );

    QVBoxLayout *topRightLayout = new QVBoxLayout();
    topRightLayout->addWidget(statusLabel, 0, Qt::AlignCenter);
    topRightLayout->addWidget(ledLabel, 0, Qt::AlignCenter);
    topRightLayout->addStretch();

    QWidget *topRightWidget = new QWidget(centralWidget);
    topRightWidget->setLayout(topRightLayout);

    QHBoxLayout *headerTopLayout = new QHBoxLayout();
    headerTopLayout->addStretch();
    headerTopLayout->addWidget(topRightWidget);

    // Agregar todos los layouts al `mainLayout`
    mainLayout->addLayout(headerLayout);
    mainLayout->addLayout(headerTopLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(emergencyLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(bottomLayout);

    // Establecer el widget central
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // Conectar el botón de ayuda
    connect(helpButton, &QPushButton::clicked, this, [this]() {
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

        // Botón para probar la conexión al lector
        QPushButton *testConnectionButton = new QPushButton("Probar conexión al lector", helpWindow);
        testConnectionButton->setStyleSheet(
            "font-size: 14px; color: white; background-color: green; padding: 5px 10px; border-radius: 5px;");
        layout->addWidget(testConnectionButton);

        // Botón para cerrar la ventana de ayuda
        QPushButton *closeButton = new QPushButton("Cerrar", helpWindow);
        closeButton->setStyleSheet(
            "font-size: 14px; color: white; background-color: red; padding: 5px 10px; border-radius: 5px;");
        layout->addWidget(closeButton);

        // Conectar el botón para cerrar la ventana
        connect(closeButton, &QPushButton::clicked, helpWindow, &QWidget::close);

        // Funcionalidad del botón de prueba de conexión
        connect(testConnectionButton, &QPushButton::clicked, this, [this]() {
            if (server.isListening()) {
                QMessageBox::information(this, "Prueba de conexión", "El lector RFID está conectado correctamente.");
            } else {
                QMessageBox::critical(this, "Prueba de conexión", "El lector RFID no está conectado. Verifica los cables o la configuración.");
            }
        });

        helpWindow->setLayout(layout);
        helpWindow->show();
    });

    // Inicializar el estado del LED
    ledState = false;

    // Configurar temporizador para la intermitencia del LED
    ledTimer = new QTimer(this);
    connect(ledTimer, &QTimer::timeout, this, [this]() {
        if (ledState) {
            ledLabel->setStyleSheet("background-color: yellow; border-radius: 10px; border: 2px solid black;");
        } else {
            ledLabel->setStyleSheet("background-color: transparent; border-radius: 10px; border: 2px solid black;");
        }
        ledState = !ledState; // Alternar estado
    });
    ledTimer->start(500); // Intermitencia cada 500ms

    // Crear la etiqueta de estado de conexión en la esquina inferior derecha
    connectionStatusLabel = new QLabel("Estado: Conectando...", this);
    connectionStatusLabel->setStyleSheet("font-size: 14px; color: red;");
    connectionStatusLabel->setAlignment(Qt::AlignRight);

    // Layout para la parte inferior
    QHBoxLayout *connectionLayout = new QHBoxLayout();
    connectionLayout->addStretch();  // Empuja el texto hacia la derecha
    connectionLayout->addWidget(connectionStatusLabel, 0, Qt::AlignRight);

    // Envolver en otro layout para que esté en la parte inferior derecha
    QVBoxLayout *bottomRightLayout = new QVBoxLayout();
    bottomRightLayout->addLayout(connectionLayout);
    bottomRightLayout->setAlignment(Qt::AlignBottom | Qt::AlignRight); // Lo alinea abajo a la derecha

    // Agregar el layout a la ventana
    mainLayout->addLayout(bottomRightLayout);


    // Crear la etiqueta de instrucciones para el cliente (debajo del título)
    rfidInstructionLabel = new QLabel("Por favor, pase su tarjeta RFID", this);
    rfidInstructionLabel->setStyleSheet(
        "font-size: 18px; "
        "color: white; "
        "font-weight: bold; "
        "background-color: transparent;"
        );
    rfidInstructionLabel->setAlignment(Qt::AlignCenter);

    // Agregar la etiqueta de RFID debajo del título
    headerLayout->addWidget(rfidInstructionLabel, 0, Qt::AlignCenter);

    // Temporizador para la animación de puntos suspensivos
    dotCount = 0;
    rfidAnimationTimer = new QTimer(this);
    connect(rfidAnimationTimer, &QTimer::timeout, this, [this]() {
        dotCount = (dotCount + 1) % 4;  // Ciclo de 0 a 3 puntos
        QString dots = QString(".").repeated(dotCount);
        rfidInstructionLabel->setText("Por favor, pase su tarjeta RFID" + dots);
    });
    rfidAnimationTimer->start(500);  // Actualizar cada 500 ms

    // Agregar la etiqueta de RFID debajo del título
    mainLayout->addWidget(rfidInstructionLabel, 0, Qt::AlignCenter);

    // Ocultar los botones de lavado y parada de emergencia al inicio
    ui->pushButton_LedGreen->setVisible(false);
    ui->pushButton_LedRed->setVisible(false);
    ui->pushButton_LedBlue->setVisible(false);
    ui->paradaButton->setVisible(false);

    //==================================================================================================================
    // Configurar el servidor TCP
    server.listen(QHostAddress::Any, PORTsERVER);
    // Conectar la señal de nueva conexión
    connect(&server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
    //==================================================================================================================
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_LedGreen_clicked()
{
    if (flag_habilitacion) {
        // Construir la trama a enviar
        QString buffer = ">L,N,";
        char chk = 0;
        // Calcular el checksum mediante XOR de cada carácter
        for (int i = 0; i < buffer.size(); i++) {
            chk ^= buffer.toStdString()[i];
        }
        // Agregar el checksum, delimitador de fin y salto de línea
        buffer.append(chk);
        buffer.append('<');
        buffer.append('\n');

        // Enviar la trama si hay al menos un socket conectado y en estado conectado
        if (!sockets.isEmpty() &&
            sockets.at(0)->state() == QAbstractSocket::ConnectedState) {
            sockets.at(0)->write(buffer.toUtf8());
            sockets.at(0)->flush();
        }

        //ui->label->setText("Esperando confirmacion del Lavado Normal...");
        qDebug() << buffer;
    }
}

void MainWindow::on_pushButton_LedRed_clicked()
{
    if (flag_habilitacion) {
        // Construir la trama a enviar
        QString buffer = ">L,R,";
        char chk = 0;
        // Calcular el checksum mediante XOR
        for (int i = 0; i < buffer.size(); i++) {
            chk ^= buffer.toStdString()[i];
        }
        // Agregar el checksum, delimitador de fin y salto de línea
        buffer.append(chk);
        buffer.append('<');
        buffer.append('\n');

        // Enviar la trama si hay un socket conectado
        if (!sockets.isEmpty() &&
            sockets.at(0)->state() == QAbstractSocket::ConnectedState) {
            sockets.at(0)->write(buffer.toUtf8());
            sockets.at(0)->flush();
        }
        //ui->label->setText("Esperando confirmacion del Rapido...");
        qDebug() << buffer;
    }
}

void MainWindow::on_pushButton_LedBlue_clicked()
{
    if (flag_habilitacion) {
        // Construir la trama a enviar
        QString buffer = ">L,C,";
        char chk = 0;
        // Calcular el checksum mediante XOR
        for (int i = 0; i < buffer.size(); i++) {
            chk ^= buffer.toStdString()[i];
        }
        // Agregar el checksum, delimitador de fin y salto de línea
        buffer.append(chk);
        buffer.append('<');
        buffer.append('\n');

        // Enviar la trama si hay un socket conectado
        if (!sockets.isEmpty() &&
            sockets.at(0)->state() == QAbstractSocket::ConnectedState) {
            sockets.at(0)->write(buffer.toUtf8());
            sockets.at(0)->flush();
        }

        //ui->label->setText("Esperando confirmacion del Centrifugado...");
        qDebug() << buffer;
    }
}

QString Matias = "03 A5 C6 12 72";

QString MainWindow::generateChecksum(const QString &data) {
    char chk = 0;
    for (int i = 1; i < data.size() - 2; i++) { // Excluir el '>' inicial y el '<' final
        chk ^= data[i].toLatin1();
    }
    return QString::number(static_cast<unsigned char>(chk), 16).toUpper();
}

void MainWindow::MessageReceived(const QString &msg)
{
    QString tramaRecibida = msg;
    qDebug() << "Mensaje recibido en el tema:" << msg;
    qDebug() << "Contenido del mensaje:" << tramaRecibida;

    // Comparar el mensaje recibido con el string preestablecido
    if (msg == Matias) {
        flag_habilitacion = 1;


        // Detener animación y ocultar la etiqueta
        rfidAnimationTimer->stop();
        rfidInstructionLabel->setText("Hola Matias");
        rfidInstructionLabel->setStyleSheet(
            "font-size: 20px; "
            "color: lightgreen; "
            "font-weight: bold;"
        );

        // Cambiar LED a verde y detener intermitencia
        ledTimer->stop();
        ledLabel->setStyleSheet("background-color: green; border-radius: 10px; border: 2px solid black;");

        // Mostrar los botones de lavado y parada de emergencia
        ui->pushButton_LedGreen->setVisible(true);
        ui->pushButton_LedRed->setVisible(true);
        ui->pushButton_LedBlue->setVisible(true);
        ui->paradaButton->setVisible(true);
    }
    else if (msg == ">L,A,1<") {
        // ui->label->setText("Lavado Aceptado!");
    }
    else if (msg == ">L,P,1<") {
        // ui->label->setText("Equipo detenido!");
    }
    else { // Tarjeta inválida
        qDebug() << "Persona no reconocida.";

        // Cambiar LED a rojo momentáneamente y volver a intermitencia
        ledTimer->stop();
        ledLabel->setStyleSheet("background-color: red; border-radius: 10px; border: 2px solid black;");

        // Reiniciar intermitencia después de 1 segundo
        QTimer::singleShot(1000, this, [this]() {
            ledState = false; // Restablecer estado inicial
            ledTimer->start(500);
        });

        // Restaurar la animación y la etiqueta después de 1 segundo
        QTimer::singleShot(1000, this, [this]() {
            ledState = false;
            ledTimer->start(500);
            rfidInstructionLabel->setText("Por favor, pase su tarjeta RFID");
            rfidAnimationTimer->start(500); // Volver a iniciar la animación
        });
    }
}

void MainWindow::onNewConnection()
{
    qDebug() << "Cliente conectado\r\n";
    QTcpSocket *clientSocket = server.nextPendingConnection();

    // Conectar señales
    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(clientSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(onStateChanged(QAbstractSocket::SocketState)));

    sockets.push_back(clientSocket);

    // Actualizar estado de conexión a "Conectado"
    connectionStatusLabel->setText("Estado: Conectado");
    connectionStatusLabel->setStyleSheet("font-size: 14px; color: green;");

    // Mostrar mensaje emergente
    QMessageBox::information(this, "Conexión establecida", "Se ha conectado un cliente exitosamente.");
}

void MainWindow::onStateChanged(QAbstractSocket::SocketState state)
{
    qDebug() << "Cambio de estado en el socket\r\n";
    if (state == QAbstractSocket::UnconnectedState) {
        QTcpSocket *s = static_cast<QTcpSocket*>(QObject::sender());
        sockets.removeOne(s);

        if (sockets.isEmpty()) {
            // Volver al estado de "Conectando..."
            connectionStatusLabel->setText("Estado: Conectando...");
            connectionStatusLabel->setStyleSheet("font-size: 14px; color: red;");
        }
    }
}

void MainWindow::onReadyRead()
{
    qDebug() << "onReadyRead\r\n";
    QTcpSocket *s = static_cast<QTcpSocket*>(QObject::sender());
    QByteArray data = s->readAll();
    QString message = data;

    // Enviar el mensaje recibido a todos los sockets conectados
    for (QTcpSocket *socket : sockets) {
        qDebug() << data;
        MessageReceived(message);
        if (socket != s) {
            socket->write(data);
        }
    }
}

void MainWindow::on_paradaButton_clicked()
{
    if (flag_habilitacion) {
        // Construir la trama a enviar
        QString buffer = ">L,P,";
        char chk = 0;
        // Calcular el checksum mediante XOR
        for (int i = 0; i < buffer.size(); i++) {
            chk ^= buffer.toStdString()[i];
        }
        // Agregar el checksum, delimitador de fin y salto de línea
        buffer.append(chk);
        buffer.append('<');
        buffer.append('\n');

        // Enviar la trama si hay un socket conectado
        if (!sockets.isEmpty() &&
            sockets.at(0)->state() == QAbstractSocket::ConnectedState) {
            sockets.at(0)->write(buffer.toUtf8());
            sockets.at(0)->flush();
        }

        //ui->label->setText("Enviando parada de emergencia...");
        qDebug() << buffer;
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event); // Llamar a la implementación base

    // Redimensionar el QLabel para que cubra toda la ventana
    backgroundLabel->setGeometry(0, 0, width(), height());

    // Redimensionar la imagen manteniendo su proporción y expandiéndola si es necesario
    QPixmap background("C:/Users/SinstuTror/Desktop/Interfaz/Interfaz Con Base/interfaz/logo.jpg");
    backgroundLabel->setPixmap(background.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
}




