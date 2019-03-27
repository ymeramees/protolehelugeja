#ifndef SCORINGMACHINECONNECTION_H
#define SCORINGMACHINECONNECTION_H

#include <QObject>
#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include "../../protokollitaja/src/lask.h"

class ScoringMachineConnection : public QObject
{
    Q_OBJECT
public:
    explicit ScoringMachineConnection(QObject *parent = 0);
    ~ScoringMachineConnection();
    void setScoringMachineType(int machineType);
    void setTargetType(int targetType);

    enum ScoringMachineType {RMIII, RMIV};
    enum TargetType {AirRifle, AirPistol, SmallboreRifle};

private:
    bool m_connected;   // Shows if scoring machine is in connected stage
    bool m_machineChoiceInProgress;
    int m_scoringMachineType;
    int m_sendingStage;   // For RM-IV, showing in which stage sending currently is:
    // 0 - initial/ACK received (end), 1 -ENQ sent, 2 - STX received, 3 - text sent, 4 - text received, need to reply ACK
    int shotNo; // Current shot number in series
    int m_targetType = -1;

    QString m_portName;

    QByteArray m_dataToSend;

    QSerialPort m_serialPort;

    QTimer m_readTimer;   // Delay between reads from serial port
    QTimer m_sendTimer; // Delay before sending, so that RMIV has time to react

signals:
    void dataSent(QByteArray data);
    void shotRead(QString shotInfo);
    void connectionStatusChanged(QString status);

public slots:
    void closeConnection();
    int CRC(QByteArray const *s) const;
    void connectToMachine();
    Lask extractRMIIIShot(QString shotInfo);
    Lask extractRMIVShot(QString);
    void sendToMachine();
    void sendToMachine(QString);

private slots:
    void connectToRMIII();
    void connectToRMIV();
    void readFromMachine();
    void readFromRMIII();
    void readFromRMIV();
};

#endif // SCORINGMACHINECONNECTION_H
