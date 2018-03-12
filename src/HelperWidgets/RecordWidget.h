/*
 * Copyright 2016 Adam Berlinger
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#ifndef _RECORD_WIDGET_H_
#define _RECORD_WIDGET_H_

#include <QtGui>
#include <QtSerialPort/QtSerialPort>
#include <QCheckBox>
#include <QTabWidget>
#include "ExtendedPlot.h"
#include "Protocol.h"
#include "RangeControl.h"

class RecordWidget : public QWidget {
    Q_OBJECT
protected:
    QBoxLayout* mainLayout;
    QBoxLayout* controlLayout;
    QWidget* sidePanel;
    QPushButton* startButton;
    QPushButton* stopButton;
    QPushButton* clearButton;

    ExtendedPlot* plot;
    QVector<double> dataX,dataY;
    double x;
    double min,max;
    double recordWidth;
    bool running;
    bool hideOnClose;

    QLabel* noiseLevelLabel;
    QPushButton* noiseLevelButton;

    virtual void closeEvent(QCloseEvent *event);
private slots:
    void startRecording();
    void stopRecording();
    void clearRecord();
    void computeNoise();
public slots:
    void record(float value);
    void record(float value, float time);
public:
    RecordWidget(QString caption, QString yAxisLabel,bool hideOnClose, double recordWidth = 5.0);
    virtual ~RecordWidget();
};

class MultiRecordWidget;

class MultiRecordWidgetChannel : public QObject {
    Q_OBJECT
public:
    QVector<double> dataX,dataY;
    QCPGraph *graph;
    QAction* toggle;
    MultiRecordWidget* parent;
    int slot;
    MultiRecordWidgetChannel(MultiRecordWidget* parent, ExtendedPlot* plot,
        int slot, bool visibility);
    ~MultiRecordWidgetChannel();
public slots:
    void toggleVisibility(bool value);
};

class MultiRecordWidget : public QWidget {
    Q_OBJECT
protected:
    QBoxLayout* mainLayout;
    QBoxLayout* controlLayout;
    QWidget* sidePanel;
    QPushButton* startButton;
    QPushButton* stopButton;
    QPushButton* clearButton;
    QHash<int,MultiRecordWidgetChannel*> channels;
    QElapsedTimer timer;
    QMenu* channelMenu;

    ExtendedPlot* plot;
    double min,max;
    double recordWidth;
    bool running;
    bool recordActive;
    bool hideOnClose;
    bool minMaxInitialized;

    double recordTime;
    uint32_t hideChannelMask;
    uint32_t fixedChannels;

    QLabel* noiseLevelLabel;
    QPushButton* noiseLevelButton;

    virtual void closeEvent(QCloseEvent *event);
private slots:
    void startRecording();
    void stopRecording();
    void clearRecord();
    void computeNoise();
public slots:
    void recordSimple(float value);
    void recordPrepare();
    void recordPrepare(float time);
    void record(float value, int slot);
    void recordSubmit();
public:
    MultiRecordWidget(QString caption, QString yAxisLabel,bool hideOnClose, double recordWidth = 5.0, uint32_t fixedChannels = 0);
    virtual ~MultiRecordWidget();
    void setChannelVisibility(int slot,bool value);
};

#endif /*  _RECORD_WIDGET_H_ */
