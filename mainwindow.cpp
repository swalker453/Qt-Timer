#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QTimer>
#include <QGraphicsOpacityEffect>
#include <QUrl>

#include <QDesktopServices>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/images/images/ico2.ico"));
    now_second = begin_second = ui->secondSpinBox->value() + ui->minuteSpinBox->value()*60 + ui->hourSpinBox->value()*60*60;
    alart_sound = 0;
    started = 0;

    bg_image = new QLabel(ui->centralwidget);
    bg_image->setPixmap(QPixmap(":/images/images/bg.png"));
    bg_image->setScaledContents(true);
    bg_image->setFixedSize(400,192);
    bg_image->setStyleSheet("background:transparent;");

    bg_image2 = new ClickableLabel(ui->centralwidget);
    bg_image2->setPixmap(QPixmap(":/images/images/banner.png"));
    bg_image2->setScaledContents(true);
    bg_image2->setFixedSize(200,40);
    bg_image2->setStyleSheet("background:transparent;");

    connect(bg_image2, &ClickableLabel::clicked, this, []() {
        QDesktopServices::openUrl(QUrl("https://swalker.sakura.ne.jp/"));
    });

    auto *opacity = new QGraphicsOpacityEffect(bg_image);
    opacity->setOpacity(0.5);   // 0.0 = 完全透明, 1.0 = 不透明
    bg_image->setGraphicsEffect(opacity);

    bg_image->lower();
    bg_image2->lower();

#ifdef Q_OS_ANDROID
    ui->hourSpinBox->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->minuteSpinBox->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->secondSpinBox->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
#else
    ui->hourSpinBox->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui->minuteSpinBox->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui->secondSpinBox->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    createTrayIcon();
#endif

    alarmSound = new QSoundEffect(this);
    alarmSound->setSource(QUrl("qrc:/images/images/alarm01.wav"));
    alarmSound->setLoopCount(QSoundEffect::Infinite);
    alarmSound->setVolume(1.0);

    label_second_set();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::update);
    timer->start(1000);

    QTimer::singleShot(0, this, [this]() {
        bg_update();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::label_second_set(){
    QString hr_str, min_str, sec_str;
    hr_str = QString("%1").arg(begin_second/60/60,2,10,QChar('0'));
    min_str = QString("%1").arg(begin_second/60%60,2,10,QChar('0'));
    sec_str = QString("%1").arg(begin_second%60,2,10,QChar('0'));

    ui->begin_label->setText(hr_str + " : " + min_str + " : " + sec_str);

    hr_str = QString("%1").arg(now_second/60/60,2,10,QChar('0'));
    min_str = QString("%1").arg(now_second/60%60,2,10,QChar('0'));
    sec_str = QString("%1").arg(now_second%60,2,10,QChar('0'));

    if(alart_sound == 1){
        ui->now_label->setStyleSheet("background-color:#bbb; color: red;");
    }
    else if(started == 1){
        ui->now_label->setStyleSheet("background-color:#bbb; color: blue;");
    }else{
        ui->now_label->setStyleSheet("background-color:#bbb; color: #333;");
    }
    ui->now_label->setText(hr_str + " : " + min_str + " : " + sec_str);

    int val = (int)((double)now_second*100/(double)begin_second+0.5);
    if(val <0)val = 0;
    else if(val>100) val =100;
    ui->progressBar->setValue(val);
}

void MainWindow::on_action_X_triggered()
{
    QApplication::quit();
}


void MainWindow::on_action_V_triggered()
{
    QMessageBox::about(
        this,
        "バージョン情報",
        "<h2>隅田川散歩人企画 謹製タイマー</h2>"
        "<p>Version 1.0.0</p>"
        "<p>座りっぱなしは身体に良くないらしいです</p>"
        "<p><i>© 2026 oikawhr</i></p>"
        );
}


void MainWindow::on_pushButton_clicked()
{
    now_second = begin_second = ui->secondSpinBox->value() + ui->minuteSpinBox->value()*60 + ui->hourSpinBox->value()*60*60;
    alart_sound = 0;
    started =0;
    timer->start(1000);
    label_second_set();
}


void MainWindow::on_pushButton_2_clicked()
{
    started = 1;
    if(now_second ==0){
            now_second = begin_second;
    }
    //timer->start(1000);
    label_second_set();
}


void MainWindow::on_pushButton_3_clicked()
{
    started = 0;
    timer->start(1000);
    alart_sound=0;
    label_second_set();
}


void MainWindow::on_pushButton_4_clicked()
{
    started = 1;
    timer->start(1000);
    alart_sound=0;
    now_second = begin_second;
    label_second_set();
}

void MainWindow::update(){
    if(started){
        if(now_second>0){
            now_second--;
            if(now_second ==0){
                alart_sound=1;
                label_second_set();
                alarmSound->play();
                QMessageBox::warning(
                    this,
                    "タイマーが作動しました！！",
                    "椅子から立ち上がって屈伸運動をしましょう！！"
                    );
                alarmSound->stop();
            }
        }
    }
    label_second_set();

#ifdef Q_OS_ANDROID
    // Android 用
#else
    QString hr_str, min_str, sec_str;
    hr_str = QString("%1").arg(now_second/60/60,2,10,QChar('0'));
    min_str = QString("%1").arg(now_second/60%60,2,10,QChar('0'));
    sec_str = QString("%1").arg(now_second%60,2,10,QChar('0'));

    QString hr_str2, min_str2, sec_str2;
    hr_str2 = QString("%1").arg(begin_second/60/60,2,10,QChar('0'));
    min_str2 = QString("%1").arg(begin_second/60%60,2,10,QChar('0'));
    sec_str2 = QString("%1").arg(begin_second%60,2,10,QChar('0'));
    trayIcon->setToolTip("隅田川散歩人企画 謹製タイマー\n" + hr_str + " : " + min_str + " : " + sec_str + " / " + hr_str2 + " : " + min_str2 + " : " + sec_str2);
#endif
}


#ifdef Q_OS_ANDROID
#else
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (reallyQuit) {
        event->accept();
        return;
    }

    hide();
    trayIcon->show();
    event->ignore();

}

void MainWindow::createTrayIcon()
{
    trayMenu = new QMenu(this);

    QAction *showAction = new QAction("開く", this);
    QAction *verAction = new QAction("バージョン情報", this);
    QAction *quitAction = new QAction("終了", this);

    trayMenu->addAction(showAction);
    trayMenu->addSeparator();
    trayMenu->addAction(verAction);
    trayMenu->addSeparator();
    trayMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/images/images/ico2.ico"));
    trayIcon->setToolTip("隅田川散歩人企画 謹製タイマー");

    connect(showAction, &QAction::triggered, this, [this]() {
        show();
        raise();
        trayIcon->hide();
        activateWindow();
    });

    connect(verAction, &QAction::triggered, this, &MainWindow::on_action_V_triggered);

    connect(quitAction, &QAction::triggered, this, [this]() {
        reallyQuit = true;
        QApplication::quit();
    });

    connect(trayIcon, &QSystemTrayIcon::activated,
            this, [this](QSystemTrayIcon::ActivationReason reason) {
                if (reason == QSystemTrayIcon::Context) {
                    trayMenu->popup(QCursor::pos());
                }
                else if (reason == QSystemTrayIcon::DoubleClick) {
                    show();
                    raise();
                    trayIcon->hide();
                    activateWindow();
                }
            });

}
#endif


void MainWindow::bg_update(){
    if(!bg_image || !bg_image2){
        return;
    }

    bg_image->move(8,ui->line->pos().y()-200+3);
    bg_image->lower();

    bg_image2->move(ui->centralwidget->width()-210,ui->line->pos().y()-40);
    bg_image2->lower();
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    bg_update();
}