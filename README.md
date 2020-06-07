# LQtUtils
This is a module containing a few tools that I typically use in Qt apps.
## lqtutils_prop.h
Contains a few useful macros to synthetize Qt props. For instance:
```
class Fraction : public QObject
{
    Q_OBJECT
    L_RW_PROP(double, numerator, setNumerator, 5)
    L_RW_PROP(double, denominator, setDenominator, 9)
public:
    Fraction(QObject* parent = nullptr) : QObject(parent) {}
};
```
instead of:
```
class Fraction : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double numerator READ numerator WRITE setNumerator NOTIFY numeratorChanged)
    Q_PROPERTY(double denominator READ denominator WRITE setDenominator NOTIFY denominatorChanged)
public:
    Fraction(QObject* parent = nullptr) :
        QObject(parent),
        m_numerator(5),
        m_denominator(9)
        {}

    double numerator() const {
        return m_numerator;
    }

    double denominator() const {
        return m_denominator;
    }

public slots:
    void setNumerator(double numerator) {
        if (m_numerator == numerator)
            return;
        m_numerator = numerator;
        emit numeratorChanged(numerator);
    }

    void setDenominator(double denominator) {
        if (m_denominator == denominator)
            return;
        m_denominator = denominator;
        emit denominatorChanged(denominator);
    }

signals:
    void numeratorChanged(double numerator);
    void denominatorChanged(double denominator);

private:
    double m_numerator;
    double m_denominator;
};
```
When the QObject subclass is not supposed to have a particular behavior, the two macros L_BEGIN_CLASS and L_END_CLASS can speed up the declaration even more:
```
L_BEGIN_CLASS(Fraction)
L_RW_PROP(double, numerator, setNumerator, 5)
L_RW_PROP(double, denominator, setDenominator, 9)
L_END_CLASS
```
The L_RW_PROP and L_RO_PROP macros are overloaded, and can therefore be cassed with three or four params. The last param is used if you want to init the prop to some specific value automatically.
## lqtutils_settings.h
Contains a few tools that can be used to speed up writing simple settings to a file. Settings will still use QSettings and are therefore fully compatible. The macros are simply shortcuts to synthetise code. I only used this for creating ini files, but should work for other formats. An example:
```
L_DECLARE_SETTINGS(LSettingsTest, new QSettings("settings.ini", QSettings::IniFormat))
L_DEFINE_VALUE(QString, string1, QString("string1"), toString)
L_DEFINE_VALUE(QSize, size, QSize(100, 100), toSize)
L_DEFINE_VALUE(double, temperature, -1, toDouble)
L_DEFINE_VALUE(QByteArray, image, QByteArray(), toByteArray)
L_END_CLASS

L_DECLARE_SETTINGS(LSettingsTestSec1, new QSettings("settings.ini", QSettings::IniFormat), "SECTION_1")
L_DEFINE_VALUE(QString, string2, QString("string2"), toString)
L_END_CLASS
```
This will provide an interface to a "strong type" settings file containing a string, a QSize value, a double, a jpg image and another string, in a specific section of the ini file. Each class is reentrant like QSettings and can be instantiated in multiple threads.
Each class also provides a unique notifier: the notifier can be used to receive notifications when any thread in the code changes the settings, and can also be used in bindings in QML code. For an example, refer to LQtUtilsQuick:
```
Window {
    visible: true
    x: settings.appX
    y: settings.appY
    width: settings.appWidth
    height: settings.appHeight
    title: qsTr("Hello World")

    Connections {
        target: settings
        onAppWidthChanged:
            console.log("App width saved:", settings.appWidth)
        onAppHeightChanged:
            console.log("App width saved:", settings.appHeight)
    }

    Binding { target: settings; property: "appWidth"; value: width }
    Binding { target: settings; property: "appHeight"; value: height }
    Binding { target: settings; property: "appX"; value: x }
    Binding { target: settings; property: "appY"; value: y }
}
```