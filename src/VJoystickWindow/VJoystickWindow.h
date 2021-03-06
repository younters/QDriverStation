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

#ifndef _QDS_VJOYSTICK_WINDOW_H
#define _QDS_VJOYSTICK_WINDOW_H

#include <QDialog>

class QLabel;
class QGroupBox;
class QCheckBox;
class QTabWidget;
class QListWidget;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QDoubleSpinBox;

class VJoystickWindow : public QDialog {
    Q_OBJECT

  public:
    explicit VJoystickWindow();
    ~VJoystickWindow();

  private slots:
    void createWidgets();
    void createLayouts();
    void connectSlots();
    void readSettings();

  private:
    QTabWidget* m_tab;
    QGroupBox* m_keysBox;
    QWidget* m_generalTab;

    QListWidget* m_keys;
    QListWidget* m_meanings;

    QLabel* m_axisRangeLabel;
    QDoubleSpinBox* m_axisRange;
    QWidget* m_axisRangeContainer;

    QWidget* m_bottomWidget;
    QPushButton* m_closeButton;
    QCheckBox* m_useKeyboardAsJoystick;

    QVBoxLayout* m_mainLayout;
    QVBoxLayout* m_tabsLayout;
    QHBoxLayout* m_keysLayout;
    QHBoxLayout* m_bottomLayout;
    QVBoxLayout* m_axisRangeLayout;
};

#endif
