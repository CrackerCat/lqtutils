/**
 * MIT License
 *
 * Copyright (c) 2021 Luca Carlon
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 **/

#ifndef LQTUTILS_UI_H
#define LQTUTILS_UI_H

#include <QObject>
#include <QDateTime>
#include <QList>
#include <QJSValue>
#include <QtGlobal>
#include <QMetaObject>

#include "lqtutils_freq.h"
#include "lqtutils_prop.h"

QT_FORWARD_DECLARE_CLASS(QQuickWindow);

namespace lqt {

class FrameRateMonitor : public FreqMeter
{
    Q_OBJECT
public:
    FrameRateMonitor(QQuickWindow* w = nullptr, QObject* parent = nullptr);
    Q_INVOKABLE void setWindow(QQuickWindow* w);
};

QMetaObject::Connection enableAutoFrameUpdates(QQuickWindow& w);

class ScreenLock
{
public:
    ScreenLock();
    ~ScreenLock();
    bool isValid() { return m_isValid; }

private:
    void lockScreen(bool lock);

private:
    bool m_isValid;
};

class QmlUtils : public QObject
{
    Q_OBJECT
public:
    QmlUtils(QObject* parent = nullptr) : QObject(parent) {}

    Q_INVOKABLE void singleShot(int msec, QJSValue callback);
    Q_INVOKABLE static double safeAreaBottomInset();
    Q_INVOKABLE static double safeAreaTopInset();
    Q_INVOKABLE static double safeAreaRightInset();
    Q_INVOKABLE static double safeAreaLeftInset();
};

class SystemNotification : public QObject
{
    Q_OBJECT
    L_RW_PROP_AS(QString, appName)
    L_RW_PROP_AS(quint32, replacesId, 0)
    L_RW_PROP_AS(QString, icon)
    L_RW_PROP_AS(QString, title)
    L_RW_PROP_AS(QString, message)
    L_RW_PROP_AS(QStringList, actions)
    L_RW_PROP_AS(QVariantMap, hints)
    L_RW_PROP_AS(qint32, timeout, -1)
public:
    SystemNotification(QObject* parent = nullptr) : QObject(parent) {}

    void send();
};

} // namespace

#endif // LQTUTILS_UI_H
