#ifndef YRMAINWINDOW_HXX
#define YRMAINWINDOW_HXX

#include <QMainWindow>
#include <QTimer>
#include "leverframe.hxx"
#include "interlocking.hxx"
#include "graphics.hxx"
#include "audio.hxx"
#include "signal_34.hxx"
#include "signal_2.hxx"
#include "simulationpanel.hxx"

QT_BEGIN_NAMESPACE
namespace Ui { class YRMainWindow; }
QT_END_NAMESPACE

class YRMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    YRMainWindow(QWidget *parent = nullptr);
    YRB::LeverFrame* lever_frame_ = new YRB::LeverFrame(this);
    YRB::InterLocking* interlocking_ = new YRB::InterLocking(lever_frame_);
    YRB::Graphics* graphics_ = new YRB::Graphics(this);
    YRB::Audio* audio_ = new YRB::Audio(this);
    Signal34* signal_34_ = new Signal34(this);
    Signal2* signal_2_ = new Signal2(this);
    SimulationPanel* sim_panel_ = new SimulationPanel(this);
    QTimer* sim_timer_ = new QTimer(this);
    ~YRMainWindow();

private:
    Ui::YRMainWindow *ui;
    QList<QString> service_position_{"0"};
    bool simulation_running_{false};
    void lever_action_(const int& i);
    QMap<int, QPushButton*> lever_frame_buttons_;
signals:
    void sendBlockUpdate(char block, bool state);
public slots:
    void move_service()
    {
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
    void run_service() {
        if(simulation_running_) {
            qDebug() << "Simulation already running";
            return;
        }
        simulation_running_ = true;
        qDebug() << "Starting timer";
        sim_panel_->setRun();
        sim_timer_->start();
    }

};
#endif // YRMAINWINDOW_HXX
