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

#ifndef _LIB_DS_LOG_WINDOW_H
#define _LIB_DS_LOG_WINDOW_H

#include <QDialog>
#include "DS_Global.h"

namespace Ui {
class DS_LogWindow;
}

/**
 * \class DS_LogWindow
 *
 * Implements a simple window that displays the Driver Station log data
 * in a graph.
 *
 * The class is able to save the logs in the local storage and organize them
 * by date and time automatically.
 */
class LIB_DS_DECL DS_LogWindow : public QDialog {
    Q_OBJECT

  public:
    explicit DS_LogWindow ();
    ~DS_LogWindow();

  public slots:
    void addVoltageData (QString voltage);
    void setGraphPalette (QPalette* palette);

  private:
    Ui::DS_LogWindow* ui;
};

#endif
