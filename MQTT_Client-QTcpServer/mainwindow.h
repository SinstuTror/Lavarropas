#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTcpServer>
#include <QTcpSocket>
#include <QLabel>   // Para QLabel
#include <QTimer>   // Para QTimer
#include <QVBoxLayout> // Para QVBoxLayout si no está incluido
#include <QPushButton>
#include <QProgressBar>
#include <QDialog>
#include <QTimer>


#define PORTsERVER 10234

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_LedGreen_clicked();
    void on_pushButton_LedRed_clicked();
    void on_pushButton_LedBlue_clicked();
    void onNewConnection();
    void onReadyRead();
    QString generateChecksum(const QString &data);
    void onStateChanged(QAbstractSocket::SocketState state);
    void MessageReceived(const QString &msg);
    void onLogoutClicked(); // Función para finalizar sesión
    void on_paradaButton_clicked();

private:
    Ui::MainWindow *ui;
    QTcpServer server;
    QList<QTcpSocket*> sockets;
    QLabel *statusLabel; // Etiqueta para el texto "En espera"
    QLabel *ledLabel;
    bool ledState;
    QTimer *ledTimer;
    QVBoxLayout *mainLayout;  // Layout principal de la ventana
    QLabel *backgroundLabel;
    QLabel *connectionStatusLabel; // Nueva etiqueta para el estado de conexión
    QLabel *rfidInstructionLabel;  // Nueva etiqueta para la instrucción de pasar la tarjeta
    QTimer *rfidAnimationTimer;    // Temporizador para la animación de los puntos suspensivos
    int dotCount;                  // Contador de puntos para la animación
    bool isClientConnected; // Variable para rastrear si el cliente está conectado
    QPushButton *logoutButton; // Botón para finalizar sesión

    QLabel *titleLabel;          // Etiqueta para el título
    QLabel *userGreetingLabel;   // Mensaje de bienvenida con el nombre del usuario
    QLabel *userBalanceLabel;    // Saldo disponible del usuario
    int userBalance;             // Saldo del usuario (por ahora estático)

    void updateBalanceDisplay();

    QDialog *loadingDialog;        // Ventana emergente de carga
    QProgressBar *progressBar;     // Barra de progreso en la ventana emergente
    QTimer *loadingTimer;          // Temporizador para simular carga

    void showLoadingDialog();      // Método para mostrar la ventana de carga
    void updateProgressBar();      // Método para actualizar la barra de progreso
    void closeLoadingDialog();     // Método para cerrar la ventana de carga

    void showLogoutMessage();

    QDialog *processingDialog;     // Ventana emergente de proceso de lavado
    QProgressBar *processingBar;   // Barra de progreso para el proceso
    QTimer *processingTimer;       // Temporizador para simular progreso
    QLabel *processingLabel;       // Etiqueta con mensaje de espera

    void showProcessingDialog();   // Método para mostrar la ventana de proceso
    void updateProcessingBar();    // Método para actualizar la barra de progreso
    void closeProcessingDialog();  // Método para cerrar la ventana de proceso

    QPushButton *emergencyStopButton;  // Botón de parada de emergencia dentro de la ventana de procesamiento
    QTimer *dotsTimer;  // Temporizador para los puntos suspensivos dinámicos
    int dotsCount;  // Contador de puntos




protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // MAINWINDOW_H
