#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTcpServer>
#include <QTcpSocket>
#include <QLabel>   // Para QLabel
#include <QTimer>   // Para QTimer
#include <QVBoxLayout> // Para QVBoxLayout si no está incluido

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

protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // MAINWINDOW_H
