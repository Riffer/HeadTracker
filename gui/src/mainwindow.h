#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QTimer>
#include <QDebug>
#include <QMessageBox>
#include <QScrollBar>
#include <QSerialPortInfo>
#include <QFileDialog>
#include <QSettings>
#include <QCloseEvent>
#include <QQueue>
#include <QTextEdit>
#include "trackersettings.h"
#include "firmware.h"
#include "calibratebno.h"
#include "calibrateble.h"
#include "diagnosticdisplay.h"
#include "boardnano33ble.h"
#include "boardbno055.h"

const int MAX_LOG_LENGTH=6000; // How many bytes to keep of log data in the gui
const QString version="0.90"; // Current Version Number
const QString fwversion="09"; // Which suffix on firmware file to use from GITHUB
const QStringList firmwares={"BNO055","NANO33BLE"}; // Allowable hardware types

const int IMHERETIME=8000; // milliseconds before sending another I'm Here Message to keep communication open
const int MAX_TX_FAULTS=8; // Number of times to try re-sending data
const int TX_FAULT_PAUSE=750; // milliseconds wait before trying another send
const int ACKNAK_TIMEOUT=500; // milliseconds without an ack/nak is a fault

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private:    
    Ui::MainWindow *ui;
    QList<BoardType*> boards;
    QSerialPort *serialcon;
    TrackerSettings trkset;
    QByteArray serialData;
    QTimer rxledtimer;
    QTimer txledtimer;
    QTimer requestTimer;
    QTimer connectTimer;
    QTimer saveToRAMTimer;
    QTimer requestParamsTimer;
    bool waitingOnParameters;
    int boardRequestIndex;

    QString logd;
    Firmware *firmwareUploader;
    DiagnosticDisplay *diagnostic;
    QMessageBox *msgbox;
    QTextEdit *serialDebug;

    BoardNano33BLE *nano33ble;
    BoardBNO055 *bno055;
    BoardType *currentboard;

    void parseSerialData();
    void sendSerialData(QByteArray data);
    bool checkSaved();

protected:
    void keyPressEvent(QKeyEvent *event);

private slots:
    void addToLog(QString log, int ll=0);
    void findSerialPorts();
    void serialConnect();
    void serialDisconnect();
    void serialError(QSerialPort::SerialPortError);
    void connectTimeout();
    void requestTimeout();
    void saveToRAMTimeout();
    void requestParamsTimeout();
    void updateFromUI();
    void updateToUI();
    void offOrientChanged(float,float,float);
    void ppmOutChanged(int,int,int);
    void serialReadReady();
    void manualSend();
    void storeToNVM();
    void storeToRAM();
    void resetCenter();
    void rxledtimeout();
    void txledtimeout();
    void saveSettings();
    void loadSettings();
    void uploadFirmwareClick();
    void startCalibration();
    void startData();
    void showDiagsClicked();
    void showSerialDiagClicked();

    // Board Connections
    void paramSendStart();
    void paramSendComplete();
    void paramSendFailure(int);
    void paramReceiveStart();
    void paramReceiveComplete();
    void paramReceiveFailure(int);
    void calibrationSuccess();
    void calibrationFailure();
    void serialTxReady();
    void needsCalibration();    
    void boardDiscovered(BoardType *);
    void statusMessage(QString,int timeout=0);
};
#endif // MAINWINDOW_H
