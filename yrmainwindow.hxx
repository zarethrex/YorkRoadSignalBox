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
    bool service_held_{false};
    void lever_action_(const int& i);
    QMap<int, QPushButton*> lever_frame_buttons_;
signals:
    void sendBlockUpdate(char block, bool state);
public slots:
    void move_service();
    void run_service();
};
#endif // YRMAINWINDOW_HXX
