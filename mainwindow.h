#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QSoundEffect>
#include <QLabel>
#include <QResizeEvent>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>

#ifdef Q_OS_ANDROID
    //Android用
#else
#include <QSystemTrayIcon>
#include <QMenu>
#include <QCloseEvent>
#include <QCursor>
#include <QPoint>
#endif

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H
class ClickableLabel : public QLabel
{
    Q_OBJECT

public:
    explicit ClickableLabel(QWidget *parent = nullptr)
        : QLabel(parent)
    {
        setCursor(Qt::PointingHandCursor);
        setMouseTracking(true);
    }

signals:
    void clicked();

protected:
    void mouseReleaseEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton) {
            emit clicked();
        }

        QLabel::mouseReleaseEvent(event);
    }

    void enterEvent(QEnterEvent *event) override
    {
        hover = true;
        update();
        QLabel::enterEvent(event);
    }

    void leaveEvent(QEvent *event) override
    {
        hover = false;
        update();
        QLabel::leaveEvent(event);
    }

    void paintEvent(QPaintEvent *event) override
    {
        QLabel::paintEvent(event);

        if (hover) {
            QPainter painter(this);
            painter.fillRect(rect(), QColor(255, 255, 255, 127));
        }
    }

private:
    bool hover = false;
};

#endif // CLICKABLELABEL_H


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_action_X_triggered();

    void on_action_V_triggered();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void update();

protected:
#ifdef Q_OS_ANDROID
    //Android用
#else
    void closeEvent(QCloseEvent *event) override;
#endif
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::MainWindow *ui;
    void label_second_set();
    void bg_update();

    int begin_second;
    int now_second;
    int alart_sound;
    int started;
    QTimer *timer;
    QSoundEffect *alarmSound;
    QLabel *bg_image = nullptr;
    ClickableLabel *bg_image2 = nullptr;

#ifdef Q_OS_ANDROID
    // Android 用
#else
    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;
    bool reallyQuit = false;
    void createTrayIcon();
#endif
};
#endif // MAINWINDOW_H
