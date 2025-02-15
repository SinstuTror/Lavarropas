#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QTimer>
#include <QStringList>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMovie>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>

class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(QTcpSocket *socket, QTcpServer *server, const QString &clientName, double initialBalance, QWidget *parent = nullptr);
    double getClientBalance() const; // Declaración del método
    QTcpServer *clientServer; // Servidor compartido desde WelcomeWindow
    ~MainWindow(); // Destructor

signals:
    void sessionEnded(); // Para regresar a la ventana de bienvenida

private slots:
    void startWashingCentrifuge();
    void startWashingNormal();
    void startWashingQuick();
    void stopWashing();
    void updateProgress();
    void showWashHistory();
    void showHelp(); // Ayuda para el cliente
    void endSession(); // Finalizar la sesión

private:
    void startWashing(const QString &mode, double time, double cost);
    void updateBalance();

    void saveHistoryToJson(); // Guarda el historial en un archivo JSON
    void loadHistoryFromJson(); // Carga el historial desde un archivo JSON (opcional, si deseas cargar datos previos)

    QLabel *statusLabel;
    QLabel *clientNameLabel;
    QLabel *balanceLabel;
    QLabel *timeRemainingLabel;
    QLabel *thankYouLabel;
    QLabel *lastCommandLabel;
    QLabel *waitingLabel;
    QProgressBar *progressBar;
    QPushButton *stopButton;
    QPushButton *centrifugeButton;
    QPushButton *normalButton;
    QPushButton *quickButton;
    QPushButton *showHistoryButton;
    QPushButton *helpButton; // Botón de ayuda
    QPushButton *endSessionButton; // Botón para finalizar sesión
    QTimer *timer;

    QList<QString> washHistory; // Historial de lavados

    double clientBalance; // Saldo actual del cliente
    double washingTime;   // Tiempo total del lavado actual
    double timeLeft;      // Tiempo restante del lavado actual
    double lastCost;      // Costo del último lavado
    QString clientName;

    void sendCommandToLPC845(const QString &command); // Envía el código al LPC845
    bool waitForConfirmation(QTcpSocket *socket, int timeoutMs = 5000);
    void handleCommunicationFailure();               // Maneja el error de comunicación
    int retryCount;
    QString generateChecksum(const QString &data);

    // Reproductor de música
    QMediaPlayer *audioPlayer;
    QAudioOutput *audioOutput;

    //Cliente
    QTcpSocket *clientSocket; // Socket compartido desde WelcomeWindow
};

#endif // MAINWINDOW_H
