# QDriverStation

[![Build Status](https://travis-ci.org/WinT-3794/QDriverStation.svg?branch=master)](https://travis-ci.org/WinT-3794/QDriverStation)
[![License](https://img.shields.io/github/license/wint-3794/qdriverstation.svg)](https://github.com/WinT-3794/QDriverStation/blob/master/LICENSE)
[![BitCoin donate button](https://img.shields.io/badge/bitcoin-donate-yellow.svg)](https://blockchain.info/address/1K85yLxjuqUmhkjP839R7C23XFhSxrefMx "Donate once-off to this project using BitCoin")

### Introduction

QDriverStation is an open-source, cross-platform alternative to the FRC Driver Station.

The application looks and behaves almost identically to the FRC Driver Station, we may release some tutorials in the future. If you are already familiar with the FRC Driver Station, and like our project, we would welcome any contributions regarding the documentation and/or tutorials for new users.

Joystick input is achieved with SDL 2.0, which is a cross-platform library for C/C++.

The actual code that moves and manages a FRC robot is found in a [separate repository](https://github.com/WinT-3794/LibDS), which you can use in your own Qt projects or change it to support older (and future) communication protocols. Please check the [progress](https://github.com/WinT-3794/LibDS#progress) section for more info.


### Screenshots (desktop)

![image](doc/screenshots/Mac.png)

![image](doc/screenshots/Ubuntu.png)

![image](doc/screenshots/Windows.png)

![image](doc/screenshots/Settings.png)

### Wiki

We are also in the process of creating a [wiki](https://github.com/WinT-3794/QDriverStation/wiki) for this project. The idea behind this is for programmers and users alike to understand how our application and the FRC Communication protocol work.

To visit the wiki, please [click here](https://github.com/WinT-3794/QDriverStation/wiki).

### Supported platforms

#### Destktop version
- [x] Microsoft Windows (tested 7, 8.1 and 10)
- [x] Mac OS X (tested Yosemite and El Capitan)
- [x] Linux (tested Ubuntu 14.04)

#### Mobile version (also works on desktop systems)
- [x] Android (tested on 4.2.2)
- [x] iOS (tested on iOS Simulator 8, works on jailbroken devices)
- [ ] Windows Phone (will be tested soon)

### Progress

- [x] Joystick input
- [x] Deploy configurations
- [ ] Mobile user interface
- [x] Desktop user interface
- [ ] Communication protocol (check [this page](https://github.com/WinT-3794/LibDS#progress) for more info)

### Folder and File structure

**Folders:**

- <code>doc</code>: contains Doxygen configuration files
- <code>etc</code>: contains various files used for deployment of the application and resources
- <code>lib</code>: contains various libraries used by the application, including the [**LibDS**](https://github.com/WinT-3794/LibDS) library.
- <code>src</code>: contains the C++ source code of the application

### Contributing

You can contribute to the project in many ways. For example:

#### Designers

You can create an awesome and distinctive icon for our project. If you are familiar with web design, you could also help us out by creating the website of the QDriverStation.

#### Developers

If you know a bit of programming, you can contribute directly to the project. If you are new to Git, [check this](https://help.github.com/) page.

#### Media

Spread the word about our application to other FRC teams around the community.

#### Give feedback

Try out the application and let us know what you think and what we can improve.

### Compilation

#### Requirements

The only requirement to compile the application is to have [Qt](http://www.qt.io/download-open-source/) installed in your system. The desktop application will compile with both Qt4 and Qt5, while the mobile application will require you to have at least Qt 5.4 installed.

- If you are using Linux, make sure that you have installed the following packages:
    - libsdl2-dev
    
The project already contains the compiled SDL libraries for Windows and Mac.

#### Compiling the application

Once you have Qt installed, open *Desktop.pro* in Qt Creator and click the "Run" button.

Alternatively, you can also use the following commands:
- qmake
- make
- **Optional:** sudo make install

### Install notes

Once you finish installing the software, you can launch it and begin driving your robot (NOTE: THIS IS NOT POSSIBLE YET, HOPEFULLY WE WILL MAKE IT WORK SOON). Just be sure to input your team number and to verify that the joysticks are working correctly. 

Notes:
- Mac users will be prompted to download an additional driver for Xbox 360 controllers to work.
- Windows users will be prompted to download Bonjour to get mDNS working, this is not necesary if you have the official FRC Driver Station already installed or if you use a static IP.

If your team uses a custom IP address for your roboRIO (or mDNS is not working, try 10.XX.YY.26), navigate to the 'Preferences' tab and click the 'Advanced Settings...' button to configure the driver station.

### Credits

This application was created by FRC team 3794 "WinT" from Metepec, Mexico. We sincerely hope that you enjoy our application and we would love some feedback from your team about it.
