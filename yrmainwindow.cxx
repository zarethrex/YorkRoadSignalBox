#include "yrmainwindow.hxx"
#include "ui_yrmainwindow.h"

YRMainWindow::YRMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::YRMainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("York Road Signal Frame");
    this->setFixedSize(YRB::SCALER.screen_width(), YRB::SCALER.screen_height());
    sim_timer_->setInterval(1500);
    sim_panel_->show();
    signal_34_->show();
    signal_2_->show();

    for(int i{1}; i < 12; ++i)
    {
        lever_frame_buttons_[i] = new QPushButton(this);
        connect(lever_frame_buttons_[i], &QPushButton::clicked, [this, i](){lever_action_(i);});
        lever_frame_buttons_[i]->move(YRB::SCALER.scale_width(32+(i-1)*45), 0.72*YRB::SCALER.screen_height());
        lever_frame_buttons_[i]->setFixedSize(YRB::SCALER.scale_width(20), YRB::SCALER.scale_height(80));
        lever_frame_buttons_[i]->setStyleSheet("QPushButton { background-color: transparent; border: 0px }");
    }
    lever_frame_->update();
    connect(sim_timer_, &QTimer::timeout, this, &YRMainWindow::move_service);
    connect(this, &YRMainWindow::sendBlockUpdate, graphics_, &YRB::Graphics::updateBlockGraphic);
    connect(sim_panel_->runButton(), &QPushButton::clicked, this, &YRMainWindow::run_service);
    connect(lever_frame_, &YRB::LeverFrame::frameUpdate, graphics_, &YRB::Graphics::updateLeverGraphic);
    connect(lever_frame_, &YRB::LeverFrame::leverFailed, audio_, &YRB::Audio::playFail);
    connect(lever_frame_, &YRB::LeverFrame::leverMove, audio_, &YRB::Audio::playLever);
    connect(interlocking_, &YRB::InterLocking::broadcastSignal, graphics_, &YRB::Graphics::updateSignalGraphic);
    connect(interlocking_, &YRB::InterLocking::broadcastSignal, signal_34_, &Signal34::setSignal);
    connect(interlocking_, &YRB::InterLocking::broadcastPoints, graphics_, &YRB::Graphics::updatePointsGraphic);
    connect(interlocking_->getPoints(), &YRB::Points::pointsStateChanged, audio_, &YRB::Audio::playIndicator);
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

void YRMainWindow::move_service() {
    const QString end_block_ = service_position_[service_position_.size() - 1];
    qDebug() << "Service Position: " << service_position_;

    // Needed to prevent parallel junction blocks switching each other off
    interlocking_->getBlockSection("E1")->setOccupied(service_position_.contains("E1"));
    interlocking_->getBlockSection("E2")->setOccupied(service_position_.contains("E2"));

    emit sendBlockUpdate('E', service_position_.contains("E1") || service_position_.contains("E2"));

    for(const QString& block : YRB::block_ids) {
        const char graphic_block_ = block.toStdString()[0];
        if(graphic_block_ == 'E') continue;

        interlocking_->getBlockSection(block)->setOccupied(service_position_.contains(block));
        emit sendBlockUpdate(graphic_block_, service_position_.contains(block));
    }

    if(service_position_.size() != 1) {
        service_position_ = {end_block_};
        return;
    }

    YRB::BlockSection* next_block_ = interlocking_->getBlockSection(end_block_)->getNext();

    // Check that the next signal is clear
    if(next_block_ && next_block_->getBlockSignal()) {

        const YRB::Signal* current_signal = interlocking_->getBlockSection(end_block_)->getBlockSignal();
        const int current_signal_id = (current_signal) ? current_signal->id() : 0;
        const YRB::Signal* next_signal = interlocking_->getBlockSection(end_block_)->getNext()->getBlockSignal();
        const int next_signal_id = (next_signal) ? next_signal->id() : -1;

        // If the next section uses the same signal continue
        // Else check the next signal state
        const YRB::SignalState next_state_ = interlocking_->getBlockSection(end_block_)->getNext()->getBlockSignal()->getState();
        if(current_signal_id != next_signal_id && next_state_ == YRB::SignalState::On) {
            qDebug() << "Service held";
            service_held_ = true;
            return;
        }
    }

    const QString new_pos_ = interlocking_->getBlockSection(end_block_)->getNext()->id();
    if(!simulation_running_) {
        sim_timer_->stop();
        return;
    }
    if(new_pos_ == "0") {
        simulation_running_ = false;
        qDebug() << "Simulation complete";
        sim_panel_->setComplete();
    }

    service_position_.push_back(new_pos_);
}

void YRMainWindow::run_service()  {
    if(simulation_running_) {
        qDebug() << "Simulation already running";
        return;
    }
    simulation_running_ = true;
    qDebug() << "Starting timer";
    sim_panel_->setRun();
    sim_timer_->start();
}
