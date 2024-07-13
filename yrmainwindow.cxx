#include "yrmainwindow.hxx"
#include "ui_yrmainwindow.h"

YRMainWindow::YRMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::YRMainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("York Road Signal Frame");
    const YRB::Scaler* scaler_ = new YRB::Scaler;
    this->setFixedSize(scaler_->screen_width(), scaler_->screen_height());
    sim_timer_->setInterval(1500);
    sim_panel_->show();
    signal_34_->show();
    signal_2_->show();

    for(int i{1}; i < 12; ++i)
    {
        lever_frame_buttons_[i] = new QPushButton(this);
        connect(lever_frame_buttons_[i], &QPushButton::clicked, [this, i](){lever_action_(i);});
        lever_frame_buttons_[i]->move(scaler_->scale_width(32+(i-1)*45), 0.72*scaler_->screen_height());
        lever_frame_buttons_[i]->setFixedSize(scaler_->scale_width(20), scaler_->scale_height(80));
        lever_frame_buttons_[i]->setStyleSheet("QPushButton { background-color: transparent; border: 0px }");
    }
    lever_frame_->update();
    connect(sim_timer_, &QTimer::timeout, this, &YRMainWindow::move_service);
    connect(this, &YRMainWindow::sendBlockUpdate, graphics_, &YRB::Graphics::updateBlockGraphic);
    connect(sim_panel_->runButton(), &QPushButton::clicked, this, &YRMainWindow::run_service);
    connect(lever_frame_, &YRB::LeverFrame::frameUpdate, graphics_, &YRB::Graphics::updateLeverGraphic);
    connect(lever_frame_, &YRB::LeverFrame::panelUpdate, audio_, &YRB::Audio::playIndicator);
    connect(lever_frame_, &YRB::LeverFrame::leverFailed, audio_, &YRB::Audio::playFail);
    connect(lever_frame_, &YRB::LeverFrame::leverMove, audio_, &YRB::Audio::playLever);
    connect(interlocking_, &YRB::InterLocking::broadcastSignal, graphics_, &YRB::Graphics::updateSignalGraphic);
    connect(interlocking_, &YRB::InterLocking::broadcastSignal, signal_34_, &Signal34::setSignal);
    connect(interlocking_, &YRB::InterLocking::broadcastPoints, graphics_, &YRB::Graphics::updatePointsGraphic);
    connect(interlocking_, &YRB::InterLocking::broadcastPoints, signal_34_, &Signal34::setFeatherIndicator);
    connect(interlocking_, &YRB::InterLocking::broadcastSignal, signal_2_, &Signal2::setSignal);
    connect(interlocking_, &YRB::InterLocking::broadcastSignal, lever_frame_, &YRB::LeverFrame::panelUpdate);
    connect(interlocking_, &YRB::InterLocking::broadcastPoints, lever_frame_, &YRB::LeverFrame::panelUpdate);
    connect(signal_34_, &Signal34::atDanger, signal_2_, &Signal2::update34Status);
}

YRMainWindow::~YRMainWindow()
{
    delete ui;
}

void YRMainWindow::lever_action_(const int &i)
{

    if(!interlocking_->Query(i))
    {
        qDebug() << "Invalid Move";
        return;
    }
    lever_frame_->update();
}
