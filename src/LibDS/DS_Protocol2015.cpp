/*
 * Copyright (c) 2015 WinT 3794 <http://wint3794.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "LibDS/DS_Protocol2015.h"

#define CONTROL_DISABLED 0x00 // The robot is disabled
#define CONTROL_NO_COMM  0x02 // Robot has not responded to sent data
#define CONTROL_TELEOP   0x04 // The robot is in TeleOp
#define CONTROL_TEST     0x05 // The robot is in Test Mode
#define CONTROL_AUTO     0x06 // The robot is in Autonomous
#define CONTROL_ESTOP    0x80 // The robot is in Emergeny Stop

#define SECTION_GENERAL  0x01 // Used for the general packet header
#define SECTION_JOYSTICK 0x0C // Used at the start of a joystick definition

#define STATUS_OK        0x10 // Communications are ok
#define STATUS_NULL      0x00 // No communications, ask the robot to respond
#define STATUS_REBOOT    0x18 // Reboot the roboRIO
#define STATUS_KILL_CODE 0x14 // Restart the user code process

#define PROGRAM_DISABLED 0x01 // Program reports that robot is disabled
#define PROGRAM_TELEOP   0x02 // Program reports that robot is in TeleOp
#define PROGRAM_AUTO     0x04 // Program reports that robot is in Autonomous
#define PROGRAM_TEST     0x08 // Program reports that robot is in TeleOp
#define PROGRAM_ROBORIO  0x10 // Unknown
#define PROGRAM_USERCODE 0x20 // Unknown
#define PROGRAM_NO_CODE  0x00 // No user code is loaded

#define ALLIANCE_RED1    0x00 // Red alliance, position 1
#define ALLIANCE_RED2    0x01 // Red alliance, position 2
#define ALLIANCE_RED3    0x02 // Red alliance, position 3
#define ALLIANCE_BLUE1   0x03 // Blue alliance, position 1
#define ALLIANCE_BLUE2   0x04 // Blue alliance, position 2
#define ALLIANCE_BLUE3   0x05 // Blue alliance, position 3

#define PORT_ROBOT       1110 // We send data to this port
#define PORT_CLIENT      1150 // We receive data from this port
#define DOUBLE_TO_CHAR   0x7F // Convert joystick axis value to char [-128, 128]

#define FTP_PCM          "/tmp/frc_versions/PCM-0-versions.ini"
#define FTP_PDP          "/tmp/frc_versions/PDP-0-versions.ini"
#define FTP_LIB          "/tmp/frc_versions/FRC_Lib_Version.ini"

DS_Protocol2015::DS_Protocol2015() {
    connect (&m_manager, SIGNAL (finished              (QNetworkReply*)),
             this,       SLOT   (onDownloadFinished    (QNetworkReply*)));
    connect (this,       SIGNAL (robotAddressChanged   (QString)),
             this,       SLOT   (onRobotAddressChanged (QString)));

    reset();
}

void DS_Protocol2015::reset() {
    /* Reset internal values */
    m_index = 0;
    p_robotCode = false;
    m_status = STATUS_OK;
    p_robotCommunication = false;

    /* Notify other objects */
    emit codeChanged (p_robotCode);
    emit voltageChanged (QString (""));
    emit communicationsChanged (p_robotCommunication);

    /* Disable the robot, avoid bad surprises */
    setControlMode (DS_ControlNoCommunication);

    /* Lookup for the robot IP again */
    QHostInfo::lookupHost (p_robotIp, this,  SLOT (onLookupFinished (QHostInfo)));
}

void DS_Protocol2015::reboot() {
    m_status = STATUS_REBOOT;
}

int DS_Protocol2015::robotPort() {
    return PORT_ROBOT;
}

int DS_Protocol2015::clientPort() {
    return PORT_CLIENT;
}

void DS_Protocol2015::restartCode() {
    m_status = STATUS_KILL_CODE;
}

QString DS_Protocol2015::robotAddress() {
    if (p_robotAddress.isEmpty())
        return QString ("roboRIO-%1.local").arg (p_team);

    return p_robotAddress;
}

QString DS_Protocol2015::radioAddress() {
    if (p_radioAddress.isEmpty())
        return DS_GetStaticIp (p_team, 1);

    return p_radioAddress;
}

void DS_Protocol2015::downloadRobotInformation() {
    QString host = "ftp://" + robotAddress();
    m_manager.get (QNetworkRequest (host + FTP_LIB));
    m_manager.get (QNetworkRequest (host + FTP_PCM));
    m_manager.get (QNetworkRequest (host + FTP_PDP));
}

QByteArray DS_Protocol2015::generateClientPacket() {
    /* Generate the ping data */
    QByteArray data;
    DS_PingData ping;
    m_index = ping.generatePingData (m_index);

    /* Add ping data */
    data.append (ping.byte1);
    data.append (ping.byte2);

    /* Add the section header */
    data.append (SECTION_GENERAL);

    /* Add the desired control mode, robot status and alliance data */
    data.append (getControlCode (controlMode()));
    data.append (p_robotCommunication ? m_status : (char) STATUS_NULL);
    data.append (getAllianceCode (alliance()));

    /* Add joystick input information if the robot is in TeleOp */
    if (controlMode() == DS_ControlTeleOp)
        data.append (generateJoystickData());

    return data;
}

QByteArray DS_Protocol2015::generateJoystickData() {
    QByteArray data;

    for (int i = 0; i < p_joysticks->count(); ++i) {
        int numAxes = p_joysticks->at (i)->numAxes;
        int numButtons = p_joysticks->at (i)->numButtons;
        int numPovHats = p_joysticks->at (i)->numPovHats;

        data.append (getJoystickSize (p_joysticks->at (i)) + 1);
        data.append (SECTION_JOYSTICK);

        /* Add axis data */
        if (numAxes > 0) {
            data.append ((uint8_t) numAxes);
            for (int axis = 0; axis < numAxes; ++axis)
                data.append (p_joysticks->at (i)->axes [axis] * DOUBLE_TO_CHAR);
        }

        /* Add button data */
        if (numButtons > 0) {
            int extraBit = (numButtons % 8 == 0) ? 0 : 1;
            int byteCount = (numButtons / 8) + extraBit;

            QBitArray buttons ((uint8_t) numButtons + extraBit);
            for (int button = 0; button < numButtons; ++button)
                buttons [button] = p_joysticks->at (i)->buttons [button];

            buttons.setBit (buttons.count() - 1, byteCount * 8);

            data.append ((uint8_t) p_joysticks->at (i)->numButtons);
            data.append (bitsToBytes (buttons));
        }

        /* Add hat/pov data */
        if (numPovHats > 0) {
            data.append ((uint8_t) numPovHats);
            for (int hat = 0; hat < numPovHats; ++hat) {
                int value = p_joysticks->at (i)->povHats [hat];
                if (value <= 0) value = -1;

                data.append ((uint8_t) value);
                data.append ((uint8_t) value);
            }
        }
    }

    return data;
}

void DS_Protocol2015::readRobotData (QByteArray data) {
    /* Do not try to read an invalid robot packet */
    if (data.isEmpty() || data.length() < 8)
        return;

    /* Update the status of the communications and download robot information */
    if (!p_robotCommunication) {
        p_robotCommunication = true;
        emit communicationsChanged (true);

        downloadRobotInformation();
        setControlMode (DS_ControlDisabled);
    }

    /* Get robot voltage */
    QString major = QString::number (data.at (5));
    QString minor = QString::number (data.at (6));
    minor = minor.replace ("-", "");

    /* Get robot code */
    bool code = data.at (4) != PROGRAM_NO_CODE;
    if (p_robotCode != code) {
        p_robotCode = code;
        emit codeChanged (code);
    }

    /* Restart the watchdog to avoid reseting the class */
    resetWatchdog();

    /* Send new values extracted from the packet to other objects */
    emit packetReceived();
    emit voltageChanged (QString ("%1.%2").arg (major, minor));
}

char DS_Protocol2015::getControlCode (DS_ControlMode mode) {
    switch (mode) {
    case DS_ControlTest:
        return CONTROL_TEST;
        break;
    case DS_ControlTeleOp:
        return CONTROL_TELEOP;
        break;
    case DS_ControlDisabled:
        return CONTROL_DISABLED;
        break;
    case DS_ControlAutonomous:
        return CONTROL_AUTO;
        break;
    case DS_ControlEmergencyStop:
        return CONTROL_ESTOP;
    case DS_ControlNoCommunication:
        return CONTROL_NO_COMM;
        break;
    }

    return CONTROL_DISABLED;
}

DS_ControlMode DS_Protocol2015::getControlMode (char byte) {
    switch (byte) {
    case CONTROL_DISABLED:
        return DS_ControlDisabled;
        break;
    case CONTROL_TELEOP:
        return DS_ControlTeleOp;
        break;
    case CONTROL_TEST:
        return DS_ControlTest;
        break;
    case CONTROL_AUTO:
        return DS_ControlAutonomous;
    case CONTROL_NO_COMM:
        return DS_ControlNoCommunication;
        break;
    }

    return DS_ControlEmergencyStop;
}

char DS_Protocol2015::getAllianceCode (DS_Alliance alliance) {
    switch (alliance) {
    case DS_AllianceRed1:
        return ALLIANCE_RED1;
        break;
    case DS_AllianceRed2:
        return ALLIANCE_RED2;
        break;
    case DS_AllianceRed3:
        return ALLIANCE_RED3;
        break;
    case DS_AllianceBlue1:
        return ALLIANCE_BLUE1;
        break;
    case DS_AllianceBlue2:
        return ALLIANCE_BLUE2;
        break;
    case DS_AllianceBlue3:
        return ALLIANCE_BLUE3;
        break;
    }

    return ALLIANCE_RED1;
}

void DS_Protocol2015::onRobotAddressChanged (QString address) {
    Q_UNUSED (address);
    p_robotIp = p_robotAddress.isEmpty() ?
                QString ("roboRIO-%1.local").arg (p_team) : p_robotAddress;
}

void DS_Protocol2015::onLookupFinished (QHostInfo info) {
    m_isLooking = false;

    if (!info.addresses().isEmpty())
        setRobotAddress (info.addresses().first().toString());
}

int DS_Protocol2015::getJoystickSize (DS_Joystick* joystick) {
    return  5
            + (joystick->numAxes > 0 ? joystick->numAxes : 0)
            + (joystick->numButtons / 8)
            + (joystick->numButtons % 8 == 0 ? 0 : 1)
            + (joystick->numPovHats > 0 ? joystick->numPovHats * 2 : 0);
}

void DS_Protocol2015::onDownloadFinished (QNetworkReply* reply) {
    /* Get URL and downloaded data */
    QString url = reply->url().toString();
    QString data = QString::fromUtf8 (reply->readAll());

    /* Data or URL is invalid */
    if (data.isEmpty() || url.isEmpty())
        return;

    /* This is the PCM information file */
    else if (url.contains (FTP_PCM, Qt::CaseInsensitive)) {
        QString version;
        QString key = "currentVersion";

        version.append (data.at (data.indexOf (key) + key.length() + 1));
        version.append (data.at (data.indexOf (key) + key.length() + 2));
        version.append (data.at (data.indexOf (key) + key.length() + 3));
        version.append (data.at (data.indexOf (key) + key.length() + 4));

        emit pcmVersionChanged (version);
    }

    /* This is the PDP information file */
    else if (url.contains (FTP_PDP, Qt::CaseInsensitive)) {
        QString version;
        QString key = "currentVersion";

        version.append (data.at (data.indexOf (key) + key.length() + 1));
        version.append (data.at (data.indexOf (key) + key.length() + 2));
        version.append (data.at (data.indexOf (key) + key.length() + 3));
        version.append (data.at (data.indexOf (key) + key.length() + 4));

        emit pdpVersionChanged (version);
    }

    /* This is the library version file */
    else if (url.contains (FTP_LIB, Qt::CaseInsensitive))
        emit libVersionChanged (data);
}
