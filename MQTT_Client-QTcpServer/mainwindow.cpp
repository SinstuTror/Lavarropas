#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>
#include <QRandomGenerator>

using namespace Qt::StringLiterals;

int flag_habilitacion = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Sistema de Lavado Inteligente");

    isClientConnected = false;  // Inicialmente, no hay cliente conectado

    // Crear el widget central
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Configurar el QLabel para la imagen de fondo
    backgroundLabel = new QLabel(this);
    backgroundLabel->setPixmap(QPixmap("C:/Users/SinstuTror/Desktop/logo/logo.jpg")
                                   .scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    backgroundLabel->setScaledContents(true);
    backgroundLabel->lower();
    backgroundLabel->setGeometry(0, 0, width(), height());

    // Configurar el título del sistema
    titleLabel = new QLabel("Sistema de Lavado Inteligente", this);
    titleLabel->setStyleSheet(
        "font-size: 26px; "  // Tamaño más grande
        "font-weight: bold; " // Negrita
        "color: white; " // Texto en blanco
        "background-color: transparent; " // Fondo transparente
        "padding: 10px;" // Espaciado interno
        );
    titleLabel->setAlignment(Qt::AlignCenter);

    // Crear el mensaje de bienvenida del usuario (inicialmente oculto)
    userGreetingLabel = new QLabel("", this);
    userGreetingLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: white;");
    userGreetingLabel->setAlignment(Qt::AlignCenter);
    userGreetingLabel->setVisible(false);

    // Crear la etiqueta del saldo del usuario (inicialmente oculta)
    userBalanceLabel = new QLabel("", this);
    userBalanceLabel->setStyleSheet("font-size: 18px; color: white;");
    userBalanceLabel->setAlignment(Qt::AlignCenter);
    userBalanceLabel->setVisible(false);



    // **Aplicar sombra al texto**
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect();
    shadowEffect->setBlurRadius(10);  // Radio de desenfoque de la sombra
    shadowEffect->setColor(Qt::black); // Color negro para la sombra
    shadowEffect->setOffset(2, 2);  // Desplazamiento de la sombra

    titleLabel->setGraphicsEffect(shadowEffect);

    // Layout superior con título
    QVBoxLayout *headerLayout = new QVBoxLayout();
    headerLayout->addWidget(titleLabel, 0, Qt::AlignTop | Qt::AlignCenter);

    // Agregar las etiquetas al layout superior
    headerLayout->addWidget(titleLabel, 0, Qt::AlignCenter);
    headerLayout->addWidget(userGreetingLabel, 0, Qt::AlignCenter);
    headerLayout->addWidget(userBalanceLabel, 0, Qt::AlignCenter);

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
        // Declarar la ventana y el layout antes del if-else
        QWidget *helpWindow = new QWidget();
        helpWindow->setWindowTitle("Ayuda - Sistema de Lavado");
        helpWindow->setFixedSize(400, 300);

        QVBoxLayout *layout = new QVBoxLayout(helpWindow);

        if (!isClientConnected) {
            // Si el cliente NO está conectado, mostrar la ayuda estándar
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

            // Conectar funcionalidad del botón de prueba de conexión
            connect(testConnectionButton, &QPushButton::clicked, this, [this]() {
                if (server.isListening()) {
                    QMessageBox::information(this, "Prueba de conexión", "El lector RFID está conectado correctamente.");
                } else {
                    QMessageBox::critical(this, "Prueba de conexión", "El lector RFID no está conectado. Verifica los cables o la configuración.");
                }
            });

        } else {
            // Si el cliente ESTÁ conectado, mostrar la nueva ayuda
            QLabel *detailsLabel = new QLabel(
                "1. Verifique que tiene saldo suficiente.\n"
                "2. Seleccione un modo de lavado.\n"
                "3. Puede detener el lavado en cualquier momento.\n"
                "4. Si tiene dudas, contacte con el personal.",
                helpWindow);
            detailsLabel->setStyleSheet("font-size: 14px; color: black;");
            detailsLabel->setWordWrap(true);
            layout->addWidget(detailsLabel);
        }

        // Botón para cerrar la ventana
        QPushButton *closeButton = new QPushButton("Cerrar", helpWindow);
        closeButton->setStyleSheet("font-size: 14px; color: white; background-color: red; padding: 5px 10px; border-radius: 5px;");
        layout->addWidget(closeButton);
        connect(closeButton, &QPushButton::clicked, helpWindow, &QWidget::close);

        // Aplicar el layout a la ventana y mostrarla
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

    // Crear el botón de "Finalizar Sesión" (Inicialmente oculto)
    logoutButton = new QPushButton("Finalizar Sesión", this);
    logoutButton->setStyleSheet(
        "font-size: 14px; color: white; background-color: red; padding: 5px 15px; "
        "border-radius: 8px; border: 2px solid white; font-weight: bold;");
    logoutButton->setVisible(false);

    // Conectar el botón de "Finalizar Sesión" a su funcionalidad
    connect(logoutButton, &QPushButton::clicked, this, &MainWindow::onLogoutClicked);

    // Agregar el botón de "Finalizar Sesión" al layout donde estaba el estado
    QHBoxLayout *logoutLayout = new QHBoxLayout();
    logoutLayout->addStretch();
    logoutLayout->addWidget(logoutButton);
    logoutLayout->setAlignment(Qt::AlignBottom | Qt::AlignRight);

    // Insertar el layout en la parte inferior derecha
    mainLayout->addLayout(logoutLayout);


    ui->pushButton_LedGreen->setText("Lavado Normal ($10)");
    ui->pushButton_LedRed->setText("Lavado Rápido ($15)");
    ui->pushButton_LedBlue->setText("Centrifugado ($12)");

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

        int cost = 10; // Costo del lavado normal

        if (userBalance < cost) {
            QMessageBox::warning(this, "Saldo insuficiente",
                                 "No tienes saldo suficiente para realizar este lavado.\n"
                                 "Por favor, recarga tu saldo o elige una opción más económica.");
            return; // Evita continuar si el saldo es insuficiente
        }

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirmar Lavado",
                                      "Este lavado costará $" + QString::number(cost) +
                                          ".\n¿Desea continuar?",
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            showLoadingDialog();
            userBalance -= cost; // Descontar saldo
            updateBalanceDisplay(); // Actualizar el saldo en pantalla



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
}

void MainWindow::on_pushButton_LedRed_clicked()
{
    if (flag_habilitacion) {

        int cost = 15; // Costo del lavado rápido

        if (userBalance < cost) {
            QMessageBox::warning(this, "Saldo insuficiente",
                                 "No tienes saldo suficiente para realizar este lavado.\n"
                                 "Por favor, recarga tu saldo o elige una opción más económica.");
            return;
        }

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirmar Lavado",
                                      "Este lavado costará $" + QString::number(cost) +
                                          ".\n¿Desea continuar?",
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            showLoadingDialog();
            userBalance -= cost;
            updateBalanceDisplay();

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
}

void MainWindow::on_pushButton_LedBlue_clicked()
{
    if (flag_habilitacion) {

        int cost = 12; // Costo del centrifugado

        if (userBalance < cost) {
            QMessageBox::warning(this, "Saldo insuficiente",
                                 "No tienes saldo suficiente para realizar este lavado.\n"
                                 "Por favor, recarga tu saldo o elige una opción más económica.");
            return;
        }

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirmar Lavado",
                                      "Este lavado costará $" + QString::number(cost) +
                                          ".\n¿Desea continuar?",
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            showLoadingDialog();
            userBalance -= cost;
            updateBalanceDisplay();

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

        // Marcar que el cliente está conectado
        isClientConnected = true;

        // Detener animación y ocultar la etiqueta
        rfidAnimationTimer->stop();

        // Cambiar LED a verde y detener intermitencia
        ledTimer->stop();
        ledLabel->setStyleSheet("background-color: green; border-radius: 10px; border: 2px solid black;");

        // **Actualizar la leyenda "En espera" a "Conectado" en verde**
        statusLabel->setText("Conectado");
        statusLabel->setStyleSheet("font-size: 20px; color: green; font-weight: bold;");

        // Ocultar el título anterior y mostrar el mensaje de bienvenida
        titleLabel->setVisible(false);
        userGreetingLabel->setText("¡Bienvenido, Matias!");
        userGreetingLabel->setVisible(true);
        rfidInstructionLabel->setVisible(false); // <-- Se oculta la etiqueta


        // Simular un saldo (esto luego vendrá de la base de datos)
        userBalance = 20; // Puedes cambiarlo por cualquier número para probar

        // Determinar color según el saldo
        QString balanceColor;
        if (userBalance > 15) {
            balanceColor = "green";
        } else if (userBalance > 5) {
            balanceColor = "yellow";
        } else {
            balanceColor = "red";
        }

        // Mostrar saldo con color dinámico
        userBalanceLabel->setText("Saldo disponible: $" + QString::number(userBalance));
        userBalanceLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: " + balanceColor + ";");
        userBalanceLabel->setVisible(true);



        // Mostrar los botones de lavado y parada de emergencia
        ui->pushButton_LedGreen->setVisible(true);
        ui->pushButton_LedRed->setVisible(true);
        ui->pushButton_LedBlue->setVisible(true);
        //ui->paradaButton->setVisible(true);

        // Ocultar el estado de conexión y mostrar el botón de finalizar sesión
        connectionStatusLabel->setVisible(false);
        logoutButton->setVisible(true);
    }
    else if (msg == ">L,A,1<") {
        closeLoadingDialog();
        QMessageBox::information(this, "Lavado iniciado",
                                 "El lavado ha comenzado.\n¡Gracias por confiar en nosotros!");

        ui->pushButton_LedGreen->setEnabled(false);
        ui->pushButton_LedRed->setEnabled(false);
        ui->pushButton_LedBlue->setEnabled(false);

        showProcessingDialog(); // Mostrar ventana de espera
        updateBalanceDisplay();
    }
    else if (msg == ">L,P,1<") {
        if (dotsTimer) dotsTimer->stop();
        if (processingTimer) processingTimer->stop();

        // Completar la barra de progreso
        processingBar->setValue(100);
        QMessageBox::information(this, "Lavado detenido",
                                 "El lavado ha sido detenido.");
        // Cerrar la ventana emergente de proceso de lavado
        closeProcessingDialog();
    }
    else if (msg == ">L,F,1<") {
        closeProcessingDialog();
    }
    else { // Tarjeta inválida
        qDebug() << "Mensaje no reconocido.";

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
            isClientConnected = false;
            // Volver al estado de "Conectando..."
            connectionStatusLabel->setText("Estado: Conectando...");
            connectionStatusLabel->setStyleSheet("font-size: 14px; color: red;");

            // **Restaurar el estado de "En espera" en amarillo**
            statusLabel->setText("En espera");
            statusLabel->setStyleSheet("font-size: 20px; color: yellow; font-weight: bold;");
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
        // Mostrar mensaje de detención
        QMessageBox *stopMessage = new QMessageBox(this);
        stopMessage->setWindowTitle("Deteniendo lavado");
        stopMessage->setText("Deteniendo el lavado, aguarde...");
        stopMessage->setIcon(QMessageBox::Warning);
        stopMessage->setStandardButtons(QMessageBox::NoButton); // No agregar botones

        stopMessage->show();

        // Cerrar la ventana automáticamente después de 3 segundos
        QTimer::singleShot(3000, stopMessage, &QMessageBox::accept);


        // Detener la barra de progreso y los puntos suspensivos
        if (processingTimer) processingTimer->stop();
        if (dotsTimer) dotsTimer->stop();
        processingLabel->setText("Deteniendo el lavado...");

        // Cambiar la barra a color rojo y detenerla
        processingBar->setStyleSheet(
            "QProgressBar {"
            "    border: 2px solid white;"
            "    border-radius: 5px;"
            "    background-color: #444;"
            "    height: 20px;"
            "}"
            "QProgressBar::chunk {"
            "    background-color: red;"
            "    width: 20px;"
            "}");
        processingBar->setValue(50); // Se queda en 50% hasta recibir confirmación

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
    QPixmap background("C:/Users/SinstuTror/Desktop/logo/logo.jpg");
    backgroundLabel->setPixmap(background.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
}

void MainWindow::onLogoutClicked()
{
    qDebug() << "Finalizando sesión...";

    showLogoutMessage();
    // Volver al estado de espera
    isClientConnected = false;
    connectionStatusLabel->setText("Estado: Conectado");
    connectionStatusLabel->setStyleSheet("font-size: 14px; color: green;");
    connectionStatusLabel->setVisible(true);

    // Ocultar el botón de "Finalizar Sesión"
    logoutButton->setVisible(false);

    // Restaurar la leyenda de "Por favor, pase su tarjeta RFID"
    rfidInstructionLabel->setText("Por favor, pase su tarjeta RFID");
    rfidInstructionLabel->setStyleSheet(
        "font-size: 18px; "
        "color: white; "
        "font-weight: bold; "
        "background-color: transparent;"
        );
    rfidAnimationTimer->start(500); // Volver a iniciar la animación de los puntos suspensivos

    // Ocultar los botones de lavado y de parada de emergencia nuevamente
    ui->pushButton_LedGreen->setVisible(false);
    ui->pushButton_LedRed->setVisible(false);
    ui->pushButton_LedBlue->setVisible(false);
    ui->paradaButton->setVisible(false);

    // Restaurar el LED intermitente en amarillo
    ledTimer->start(500);
    ledLabel->setStyleSheet("background-color: yellow; border-radius: 10px; border: 2px solid black;");

    // Restaurar el estado de "En espera" en amarillo
    statusLabel->setText("En espera");
    statusLabel->setStyleSheet("font-size: 20px; color: white; font-weight: bold;");

    // Restaurar el título y ocultar mensaje de bienvenida y saldo
    titleLabel->setVisible(true);
    userGreetingLabel->setVisible(false);
    userBalanceLabel->setVisible(false);

}

void MainWindow::updateBalanceDisplay()
{
    QString balanceColor;
    if (userBalance > 15) {
        balanceColor = "green";
    } else if (userBalance > 5) {
        balanceColor = "yellow";
    } else {
        balanceColor = "red";
    }

    userBalanceLabel->setText("Saldo disponible: $" + QString::number(userBalance));
    userBalanceLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: " + balanceColor + ";");
}

void MainWindow::showLoadingDialog()
{
    // Crear la ventana emergente
    loadingDialog = new QDialog(this);
    loadingDialog->setWindowTitle("Conectando con el lavarropas...");
    loadingDialog->setFixedSize(300, 150);

    QVBoxLayout *layout = new QVBoxLayout(loadingDialog);

    QLabel *messageLabel = new QLabel("Estableciendo conexión. Por favor, espere...", loadingDialog);
    messageLabel->setAlignment(Qt::AlignCenter);

    // Crear la barra de progreso
    progressBar = new QProgressBar(loadingDialog);
    progressBar->setRange(0, 100);
    progressBar->setValue(0);

    layout->addWidget(messageLabel);
    layout->addWidget(progressBar);
    loadingDialog->setLayout(layout);

    // Crear temporizador para simular carga
    loadingTimer = new QTimer(this);
    connect(loadingTimer, &QTimer::timeout, this, &MainWindow::updateProgressBar);

    loadingDialog->show();
    loadingTimer->start(500);  // Incrementar cada 500ms
}

void MainWindow::updateProgressBar()
{
    int currentValue = progressBar->value();
    if (currentValue < 80) {
        progressBar->setValue(currentValue + 10);  // Incremento lento inicial
    } else {
        loadingTimer->stop();  // Detener si llega a 80%
    }
}

void MainWindow::closeLoadingDialog()
{
    if (loadingDialog) {
        progressBar->setValue(100); // Completar la barra
        loadingDialog->accept();
        delete loadingDialog;
        loadingDialog = nullptr;
    }
}

void MainWindow::showLogoutMessage()
{
    // Crear un cuadro de mensaje
    QMessageBox *logoutMessage = new QMessageBox(this);
    logoutMessage->setWindowTitle("Sesión Finalizada");
    logoutMessage->setText("Gracias por confiar en nosotros, vuelva pronto.");
    logoutMessage->setIcon(QMessageBox::Information);
    logoutMessage->setStandardButtons(QMessageBox::NoButton); // No agrega botones
    logoutMessage->show();

    // Usar un QTimer para cerrar la ventana automáticamente después de 3 segundos
    QTimer::singleShot(3000, logoutMessage, &QMessageBox::accept);
}

void MainWindow::showProcessingDialog()
{
    processingDialog = new QDialog(this);
    processingDialog->setWindowTitle("Procesando lavado...");
    processingDialog->setFixedSize(350, 200);
    processingDialog->setStyleSheet("background-color: #222; color: white; font-size: 16px;");

    QVBoxLayout *layout = new QVBoxLayout(processingDialog);

    // **Etiqueta con mensaje de espera y puntos suspensivos dinámicos**
    processingLabel = new QLabel("Matias, estamos procesando tu lavado", processingDialog);
    processingLabel->setAlignment(Qt::AlignCenter);
    processingLabel->setStyleSheet("font-size: 18px; font-weight: bold;");

    // **Crear la barra de progreso con diseño más atractivo**
    processingBar = new QProgressBar(processingDialog);
    processingBar->setRange(0, 100);
    processingBar->setValue(0);
    processingBar->setTextVisible(true);
    processingBar->setStyleSheet(
        "QProgressBar {"
        "    border: 2px solid white;"
        "    border-radius: 5px;"
        "    background-color: #444;"
        "    height: 20px;"
        "}"
        "QProgressBar::chunk {"
        "    background-color: red;"
        "    width: 20px;"
        "}");

    // **Botón de parada de emergencia dentro de la ventana emergente**
    emergencyStopButton = new QPushButton("🛑 PARADA DE EMERGENCIA", processingDialog);
    emergencyStopButton->setStyleSheet(
        "font-size: 18px; font-weight: bold; color: white;"
        "background-color: red; border: 3px solid black; padding: 10px;"
        "border-radius: 10px;"
        );
    connect(emergencyStopButton, &QPushButton::clicked, this, &MainWindow::on_paradaButton_clicked);

    // **Agregar los widgets al layout**
    layout->addWidget(processingLabel);
    layout->addWidget(processingBar);
    layout->addWidget(emergencyStopButton);
    layout->setAlignment(Qt::AlignCenter);
    processingDialog->setLayout(layout);

    // **Temporizador para la animación de puntos suspensivos**
    dotsTimer = new QTimer(this);
    dotsCount = 0;
    connect(dotsTimer, &QTimer::timeout, this, [this]() {
        dotsCount = (dotsCount + 1) % 4;
        QString dots = QString(".").repeated(dotsCount);
        processingLabel->setText("Matias, estamos procesando tu lavado" + dots);
    });
    dotsTimer->start(500); // Cada 500ms cambia los puntos

    // **Temporizador para la barra de carga falsa**
    processingTimer = new QTimer(this);
    connect(processingTimer, &QTimer::timeout, this, &MainWindow::updateProcessingBar);

    processingDialog->show();
    processingTimer->start(500);
}


void MainWindow::updateProcessingBar()
{
    int currentValue = processingBar->value();

    if (currentValue < 80) {
        processingBar->setValue(currentValue + QRandomGenerator::global()->bounded(5, 15));  // Avance aleatorio
    } else {
        processingTimer->stop();  // Se detiene en 80% hasta recibir confirmación real
    }
}


void MainWindow::closeProcessingDialog()
{
    if (processingDialog) {
        dotsTimer->stop();
        processingBar->setValue(100); // Completar la barra
        processingDialog->accept();
        delete processingDialog;
        processingDialog = nullptr;

        // Restaurar UI
        userBalanceLabel->setVisible(true);
        ui->pushButton_LedGreen->setEnabled(true);
        ui->pushButton_LedRed->setEnabled(true);
        ui->pushButton_LedBlue->setEnabled(true);
        userGreetingLabel->setText("¡Bienvenido, Matias!");

        QString finalMessage;
        QString finalTitle;

        if (processingBar->value() < 100) {  // Si el lavado fue detenido antes de finalizar
            finalTitle = "Lavado detenido";
            finalMessage = "El lavado ha sido forzado a detenerse por orden del usuario.";
        } else {
            finalTitle = "Lavado finalizado";
            finalMessage = "El lavado ha finalizado correctamente.\n¡Gracias por confiar en nosotros!";
        }

        QMessageBox::information(this, finalTitle, finalMessage);

    }
}



