#include "mainwindow.h"

MainWindow::MainWindow(QTcpSocket *socket, QTcpServer *server, const QString &clientName, double initialBalance, QWidget *parent)
    : QWidget(parent), clientSocket(socket), clientServer(server), clientBalance(initialBalance), clientName(clientName) {
    if (!clientSocket || clientSocket->state() != QAbstractSocket::ConnectedState) {
        qDebug() << "Error: Socket no válido o no conectado.";
        QMessageBox::critical(this, "Error de conexión", "El socket no está conectado.");
        return;
    }
    if (!clientServer || !clientServer->isListening()) {
        qDebug() << "Error: Servidor no válido o no escuchando.";
        QMessageBox::critical(this, "Error del servidor", "El servidor no está activo.");
        return;
    }
    qDebug() << "Socket y servidor recibidos correctamente.";

    QVBoxLayout *layout = new QVBoxLayout(this);

    // Información del cliente
    clientNameLabel = new QLabel(QString("Cliente: %1").arg(clientName), this);
    clientNameLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #0047AB; text-align: center;");
    clientNameLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(clientNameLabel);

    // Configuración del GIF como fondo
    QLabel *backgroundLabel = new QLabel(this); // QLabel para mostrar el GIF
    QMovie *movie = new QMovie("C:/Users/SinstuTror/Desktop/Interfaz/Interfaz Con Base/interfaz/Lavarropas");

    // Configurar el QLabel para cubrir la ventana completa
    backgroundLabel->setMovie(movie);
    backgroundLabel->setScaledContents(true); // Ajusta el GIF al tamaño del QLabel
    movie->start(); // Inicia la animación del GIF

    // Coloca el QLabel al fondo de la ventana
    backgroundLabel->lower(); // Envía el QLabel al fondo
    backgroundLabel->setGeometry(0, 0, this->width(), this->height()); // Ocupa toda la ventana

    // Agrega los otros widgets encima del fondo
    QLabel *titleLabel = new QLabel("<h1 style='color:#1F4E79;'>Cliente: " + clientName + "</h1>", this);
    layout->addWidget(titleLabel, 0, Qt::AlignCenter);

    balanceLabel = new QLabel(this);
    updateBalance(); // Actualiza el texto y color del saldo
    balanceLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(balanceLabel);

    // Estado del sistema
    statusLabel = new QLabel("Seleccione un modo de lavado:", this);
    statusLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #333;");
    layout->addWidget(statusLabel);

    // Cargar el historial al iniciar
    loadHistoryFromJson();

    // Mensaje de espera de confirmación
    waitingLabel = new QLabel("Esperando conexión con el lavarropas...", this);
    waitingLabel->setAlignment(Qt::AlignCenter);
    waitingLabel->setStyleSheet("font-size: 16px; color: #FFA500; font-weight: bold;");
    waitingLabel->hide(); // Ocultar por defecto
    layout->addWidget(waitingLabel);

    // Botones para seleccionar el tipo de lavado
    QHBoxLayout *buttonsLayout = new QHBoxLayout;

    quickButton = new QPushButton("Lavado Rápido ($2.00)", this);
    quickButton->setIcon(QIcon(":/icons/quick.png"));
    connect(quickButton, &QPushButton::clicked, this, &MainWindow::startWashingQuick);
    buttonsLayout->addWidget(quickButton);

    normalButton = new QPushButton("Lavado Normal ($3.00)", this);
    normalButton->setIcon(QIcon(":/icons/normal.png"));
    connect(normalButton, &QPushButton::clicked, this, &MainWindow::startWashingNormal);
    buttonsLayout->addWidget(normalButton);

    centrifugeButton = new QPushButton("Centrifugado ($1.50)", this);
    centrifugeButton->setIcon(QIcon(":/icons/centrifuge.png"));
    connect(centrifugeButton, &QPushButton::clicked, this, &MainWindow::startWashingCentrifuge);
    buttonsLayout->addWidget(centrifugeButton);

    layout->addLayout(buttonsLayout);

    // Configuración del reproductor de música
    audioPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    audioPlayer->setAudioOutput(audioOutput);
    audioPlayer->setSource(QUrl::fromLocalFile("C:/Users/Frave/Desktop/interfaz/sonido.mp3"));
    audioOutput->setVolume(0.5);
    audioPlayer->setLoops(QMediaPlayer::Infinite);

    // Barra de progreso
    progressBar = new QProgressBar(this);
    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    progressBar->setStyleSheet("QProgressBar { border: 2px solid grey; border-radius: 5px; text-align: center; } "
                               "QProgressBar::chunk { background-color: violet; width: 20px; }");
    layout->addWidget(progressBar);

    timeRemainingLabel = new QLabel("Tiempo restante: 0 segundos", this);
    timeRemainingLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(timeRemainingLabel);

    // Mensaje de agradecimiento
    thankYouLabel = new QLabel("Gracias por confiar en nosotros.", this);
    thankYouLabel->setAlignment(Qt::AlignCenter);
    thankYouLabel->setStyleSheet("font-size: 16px; color: #1E90FF;");
    thankYouLabel->hide(); // Inicialmente oculto
    layout->addWidget(thankYouLabel);

    // Botón para detener el lavado
    stopButton = new QPushButton("Detener Lavado", this);
    stopButton->setEnabled(false);
    stopButton->setStyleSheet("background-color: red; color: white; font-size: 16px;");
    connect(stopButton, &QPushButton::clicked, this, &MainWindow::stopWashing);
    layout->addWidget(stopButton);

    // Botón para mostrar el historial
    showHistoryButton = new QPushButton("Ver Historial de Lavados", this);
    connect(showHistoryButton, &QPushButton::clicked, this, &MainWindow::showWashHistory);
    layout->addWidget(showHistoryButton);

    // Botón de ayuda
    helpButton = new QPushButton("Ayuda", this);
    connect(helpButton, &QPushButton::clicked, this, &MainWindow::showHelp);
    helpButton->setStyleSheet("font-size: 14px; background-color: #0078D7; color: white; border-radius: 5px;");
    layout->addWidget(helpButton);

    // Botón para finalizar sesión
    endSessionButton = new QPushButton("Finalizar Sesión", this);
    connect(endSessionButton, &QPushButton::clicked, this, &MainWindow::endSession);
    endSessionButton->setStyleSheet("font-size: 14px; background-color: #FF4500; color: white; border-radius: 5px;");
    layout->addWidget(endSessionButton);

    // Temporizador para actualizar el progreso
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateProgress);

    setLayout(layout);
}

void MainWindow::startWashingQuick() {
    startWashing("Lavado Rápido", 20.0, 2.0);
}

void MainWindow::startWashingNormal() {
    startWashing("Lavado Normal", 40.0, 3.0);
}

void MainWindow::startWashingCentrifuge() {
    startWashing("Centrifugado", 15.0, 1.5);
}

void MainWindow::startWashing(const QString &mode, double time, double cost) {
    if (clientBalance < cost) {
        QMessageBox::warning(this, "Saldo insuficiente", "No tienes suficiente saldo para este lavado.");
        return;
    }

    // Actualizar saldo
    clientBalance -= cost;
    updateBalance();

    // Determinar el código de lavado
    QString modeCode;
    if (mode == "Lavado Rápido") modeCode = "Q";
    else if (mode == "Lavado Normal") modeCode = "N";
    else if (mode == "Centrifugado") modeCode = "C";

    // Enviar comando al LPC845
    sendCommandToLPC845(modeCode);

    // Esperar confirmación del LPC845
    if (!waitForConfirmation(clientSocket)) {
        QMessageBox::critical(this, "Error de confirmación",
                              "No se pudo iniciar el lavado. Por favor, reintente o contacte al personal.");
        qDebug() << "Error: No se recibió confirmación del LPC845.";
        return;
    }

    // Iniciar transacción en la base de datos
    if (!QSqlDatabase::database().transaction()) {
        QMessageBox::critical(this, "Error", "No se pudo iniciar la transacción.");
        qDebug() << "Error al iniciar transacción.";
        return;
    }

    // Actualizar saldo del cliente
    clientBalance -= cost;
    updateBalance();

    // Registrar el lavado en el historial
    QString timestamp = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
    QString historyEntry = QString("[%1] %2 - $%3").arg(timestamp).arg(mode).arg(cost, 0, 'f', 2);
    washHistory.append(historyEntry);

    // Guardar el historial en JSON
    saveHistoryToJson();

    // Configuración del lavado
    washingTime = time;
    timeLeft = washingTime;

    progressBar->setValue(0);
    statusLabel->setText("Modo seleccionado: " + mode);
    timer->start(1000);

    // Mostrar mensaje de agradecimiento
    thankYouLabel->show();

    // Deshabilitar botones
    quickButton->setEnabled(false);
    normalButton->setEnabled(false);
    centrifugeButton->setEnabled(false);
    stopButton->setEnabled(true);

    // Configuración de música
    audioPlayer->setLoops(QMediaPlayer::Infinite);
    audioPlayer->play();
}

void MainWindow::stopWashing() {
    timer->stop();

    // Actualizar barra de progreso y mensaje de estado
    progressBar->setValue(0);
    progressBar->setStyleSheet("QProgressBar { border: 2px solid grey; border-radius: 5px; text-align: center; } "
                               "QProgressBar::chunk { background-color: red; width: 20px; }");

    thankYouLabel->hide();

    quickButton->setEnabled(true);
    normalButton->setEnabled(true);
    centrifugeButton->setEnabled(true);
    stopButton->setEnabled(false);

    // Enviar comando para detener el lavado
    QString stopCode = "S"; // Código para detener el lavado
    sendCommandToLPC845(stopCode); // Llama a la función para enviar el comando

    // Esperar confirmación del LPC
    if (waitForConfirmation(clientSocket)) {
        qDebug() << "El lavado se detuvo correctamente.";
        QMessageBox::information(this, "Lavado detenido", "El lavarropas se detuvo correctamente.");

        // Actualizar barra de progreso y reiniciar la interfaz
        progressBar->setValue(0); // Resetear la barra de progreso
        progressBar->hide(); // Ocultar la barra

        // Actualizar etiquetas y botones
        statusLabel->setText("Lavado detenido.");
        statusLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: red;");
        quickButton->setEnabled(true);
        normalButton->setEnabled(true);
        centrifugeButton->setEnabled(true);
        stopButton->setEnabled(false); // Deshabilitar el botón de detener
        thankYouLabel->hide(); // Ocultar cualquier mensaje de agradecimiento
        statusLabel->setText("Lavado detenido.");
        statusLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: red;");

        // Cálculo del reembolso parcial
        double completedRatio = (washingTime - timeLeft) / washingTime; // Porcentaje completado
        double refundAmount = (1.0 - completedRatio) * lastCost; // Reembolso proporcional
        if (refundAmount > 0) {
            clientBalance += refundAmount;
            updateBalance();
            QMessageBox::information(this, "Reembolso parcial",
                                     QString("Se ha devuelto $%1 a su saldo.").arg(refundAmount, 0, 'f', 2));
        }
    }
    audioPlayer->stop();
    // Resetear estado de la interfaz
    statusLabel->setText("Lavado detenido.");
    statusLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: red;");
}



void MainWindow::updateProgress() {
    timeLeft -= 1.0;
    int progress = static_cast<int>((1.0 - timeLeft / washingTime) * 100.0);
    progressBar->setValue(progress);

    timeRemainingLabel->setText(QString("Tiempo restante: %1 segundos").arg(static_cast<int>(timeLeft)));

    if (timeLeft <= 0) {
        timer->stop();
        stopButton->setEnabled(false);
        quickButton->setEnabled(true);
        normalButton->setEnabled(true);
        centrifugeButton->setEnabled(true);

        thankYouLabel->hide(); // Ocultar mensaje de agradecimiento
        statusLabel->setText("Lavado completado.");
        QMessageBox::information(this, "Lavado completado", "El lavado ha finalizado.");
    }
}

void MainWindow::updateBalance() {
    balanceLabel->setText(QString("Saldo: $%1").arg(clientBalance, 0, 'f', 2));
    if (clientBalance < 2.0) {
        balanceLabel->setStyleSheet("font-size: 16px; color: red;");
    } else if (clientBalance <= 5.0) {
        balanceLabel->setStyleSheet("font-size: 16px; color: orange;");
    } else {
        balanceLabel->setStyleSheet("font-size: 16px; color: green;");
    }
}

void MainWindow::showWashHistory() {
    if (washHistory.isEmpty()) {
        QMessageBox::information(this, "Historial de Lavados", "No hay lavados registrados.");
    } else {
        QString history = washHistory.join("\n");
        QMessageBox::information(this, "Historial de Lavados", history);
    }
}


void MainWindow::showHelp() {
    QMessageBox::information(this, "Ayuda",
                             "1. Verifique que tiene saldo suficiente.\n"
                             "2. Seleccione un modo de lavado.\n"
                             "3. Puede detener el lavado en cualquier momento.\n"
                             "4. Si tiene dudas, contacte con el personal.");
}

void MainWindow::saveHistoryToJson() {
    QFile file("wash_history.json");
    if (file.open(QIODevice::WriteOnly)) {
        QJsonArray historyArray;
        for (const auto &entry : washHistory) {
            historyArray.append(entry);
        }
        QJsonDocument jsonDoc(historyArray);
        file.write(jsonDoc.toJson());
        file.close();
        qDebug() << "Historial guardado correctamente en JSON.";
    } else {
        qDebug() << "No se pudo abrir el archivo para guardar el historial.";
    }
}

double MainWindow::getClientBalance() const {
    return clientBalance;
}

void MainWindow::loadHistoryFromJson() {
    QFile file("wash_history.json");
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        QJsonArray historyArray = jsonDoc.array();

        washHistory.clear();
        for (const auto &entry : historyArray) {
            washHistory.append(entry.toString());
        }

        file.close();
        qDebug() << "Historial cargado desde JSON.";
    } else {
        qDebug() << "No se pudo abrir el archivo de historial. Se iniciará vacío.";
    }
}

void MainWindow::endSession() {
    emit sessionEnded(); // Emitir señal para cerrar sesión
    close();             // Cerrar MainWindow
}

void MainWindow::sendCommandToLPC845(const QString &washCode) {
    if (!clientSocket || clientSocket->state() != QAbstractSocket::ConnectedState) {
        qDebug() << "Error: Socket no conectado.";
        QMessageBox::critical(this, "Error de conexión", "No hay conexión con el sistema.");
        return;
    }

    QString command = QString(">L,%1,1,<").arg(washCode); // Crear el comando
    QString checksum = generateChecksum(command);        // Generar checksum
    QString fullCommand = command.insert(command.size() - 1, checksum); // Insertar checksum

    int maxRetries = 3; // Máximo número de reintentos
    for (int attempt = 1; attempt <= maxRetries; ++attempt) {
        clientSocket->write(fullCommand.toUtf8());
        clientSocket->flush();

        // Mostrar en pantalla el comando enviado
        lastCommandLabel->setText("Intento " + QString::number(attempt) + ": " + fullCommand);
        waitingLabel->setText("Esperando confirmación del lavarropas... Intento " + QString::number(attempt));
        waitingLabel->show();

        qDebug() << "Comando enviado al LPC845 (Intento " << attempt << "):" << fullCommand;

        if (waitForConfirmation(clientSocket)) {
            qDebug() << "Confirmación recibida en el intento " << attempt;
            waitingLabel->hide();
            return; // Salir del bucle al recibir confirmación
        }

        qDebug() << "No se recibió confirmación en el intento " << attempt;
    }

    // Mostrar mensaje de error si no hubo respuesta después de varios intentos
    waitingLabel->setText("Error: No se recibió respuesta tras " + QString::number(maxRetries) + " intentos.");
    waitingLabel->setStyleSheet("font-size: 16px; color: red; font-weight: bold;");
    QMessageBox::critical(this, "Error de confirmación", "El lavarropas no respondió. Por favor, reintente o contacte al personal.");
}

bool MainWindow::waitForConfirmation(QTcpSocket *socket, int timeoutMs) {
    QElapsedTimer timer;
    timer.start();

    waitingLabel->setText("Esperando confirmación del lavarropas...");
    waitingLabel->setStyleSheet("font-size: 16px; color: #FFA500; font-weight: bold;");
    waitingLabel->show(); // Mostrar mensaje de espera

    while (timer.elapsed() < timeoutMs) {
        if (socket->waitForReadyRead(100)) { // Esperar datos
            QByteArray response = socket->readAll();
            QString responseStr = QString::fromUtf8(response).trimmed();

            if (responseStr.contains(">L,A,1<")) { // Verifica si es la confirmación esperada
                qDebug() << "Confirmación recibida del LPC845:" << responseStr;
                waitingLabel->hide(); // Ocultar mensaje de espera
                return true;
            }
        }
        QCoreApplication::processEvents(); // Permitir que la UI siga respondiendo
    }

    // Si el tiempo de espera expira, mostrar un error en pantalla
    qDebug() << "Error: No se recibió confirmación del LPC845.";
    waitingLabel->setText("Error: No se recibió respuesta del lavarropas.");
    waitingLabel->setStyleSheet("font-size: 16px; color: red; font-weight: bold;");
    return false;
}

QString MainWindow::generateChecksum(const QString &data) {
    char chk = 0;
    for (int i = 1; i < data.size() - 2; i++) { // Excluir el '>' inicial y el '<' final
        chk ^= data[i].toLatin1();
    }
    return QString::number(static_cast<unsigned char>(chk), 16).toUpper();
}

MainWindow::~MainWindow() {
    qDebug() << "Destructor de MainWindow llamado.";

    // Detener el temporizador si está activo
    if (timer && timer->isActive()) {
        timer->stop();
    }

    // Detener el reproductor de audio si está activo
    if (audioPlayer && audioPlayer->playbackState() == QMediaPlayer::PlayingState) {
        audioPlayer->stop();
    }

    /*
    // Liberar el socket del cliente
    if (clientSocket) {
        if (clientSocket->state() == QAbstractSocket::ConnectedState) {
            clientSocket->disconnectFromHost();
            if (clientSocket->state() != QAbstractSocket::UnconnectedState) {
                clientSocket->waitForDisconnected();
            }
        }
        clientSocket->deleteLater(); // Liberar memoria asociada
    }

    // Liberar el servidor solo si lo necesitas
    if (clientServer) {
        clientServer->close(); // Cierra cualquier conexión activa
        clientServer->deleteLater();
    }*/

    qDebug() << "Recursos de MainWindow liberados correctamente.";
}
