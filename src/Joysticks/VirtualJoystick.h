/*
 * Copyright (c) 2015-2016 WinT 3794 <http://wint3794.org>
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

#ifndef _QDS_VIRTUAL_JOYSTICK_H
#define _QDS_VIRTUAL_JOYSTICK_H

#include <QWidget>
#include <QKeyEvent>
#include <QApplication>

#include "Global/Global.h"
#include "JoysticksCommon.h"

/**
 * This class implements the code necessary to capture
 * keyboard events and interpret them in such a way that
 * it allows us to control the robot easily.
 *
 * This class also implements the keyboard shortcuts of
 * the application, such as pressing <space> to E-stop the robot.
 */
class VirtualJoystick : public QObject {
    Q_OBJECT

  public:
    explicit VirtualJoystick();

    float axisRange() const;
    bool joystickEnabled() const;
    QDS_InputDevice* joystick();

  public slots:
    void setJoystickID (int id);
    void setAxisRange (float range);
    void setJoystickEnabled (bool enabled);

  signals:
    void enabledChanged();
    void axisEvent (QDS_AxisEvent event);
    void buttonEvent (QDS_ButtonEvent event);

  private slots:
    void readAxes (int key, bool pressed);
    void readButtons (int key, bool pressed);
    void readShortcuts (int key, bool pressed);
    void processKeyEvent (QKeyEvent* event, bool pressed);

  protected:
    bool eventFilter (QObject* object, QEvent* event);

  private:
    float m_axisRange;
    bool m_joystickEnabled;
    QDS_InputDevice m_joystick;
};

#endif
