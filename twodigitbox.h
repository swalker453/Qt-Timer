#ifndef TWODIGITBOX_H
#define TWODIGITBOX_H

#endif // TWODIGITBOX_H

#include <QSpinBox>

class TwoDigitSpinBox : public QSpinBox
{
public:
    using QSpinBox::QSpinBox;

protected:

    QString textFromValue(int value) const override
    {
        return QString("%1").arg(value, 2, 10, QChar('0'));
    }

};