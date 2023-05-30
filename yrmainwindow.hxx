#ifndef YRMAINWINDOW_HXX
#define YRMAINWINDOW_HXX

#include <QMainWindow>
#include "leverframe.hxx"
#include "interlocking.hxx"
#include "graphics.hxx"
#include "audio.hxx"
#include "signal_34.hxx"
#include "signal_2.hxx"

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
    ~YRMainWindow();

private:
    Ui::YRMainWindow *ui;
    char service_position_{'\0'};
    bool simulation_running_{false};
    void lever_action_(const int& i);
    void run_service_();
    QMap<int, QPushButton*> lever_frame_buttons_;
public slots:
    void _move_service()
    {
       interlocking_->getBlockSection(service_position_)->setOccupied(true);

       // Check that the signal of the neighbouring block is clear
       if(interlocking_->getBlockSection(service_position_)->getNeighbour()->getBlockSignal()) {
            if(interlocking_->getBlockSection(service_position_)->getNeighbour()->getBlockSignal()->getState() == YRB::SignalState::On) return;
       }
       service_position_ = interlocking_->getBlockSection(service_position_)->getNeighbour()->id();
    }
};
#endif // YRMAINWINDOW_HXX
