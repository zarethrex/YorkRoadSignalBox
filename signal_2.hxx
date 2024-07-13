#ifndef SIGNAL_2_H
#define SIGNAL_2_H

#include <QMainWindow>
#include <QObject>
#include <QSvgWidget>
#include <QMap>
#include <QThread>
#include <QTimer>
#include <QDebug>
#include "scaling.hxx"
#include "global_params.hxx"

class Signal2 : public QMainWindow
{
    Q_OBJECT
private:
    QSvgWidget* signal_2_svg_ = new QSvgWidget(":/svgs/media/Signal2.svg", this);
    QMap<YRB::SignalState, QSvgWidget*> stop_svgs_;
    QMap<YRB::SignalState, QSvgWidget*> clear_svgs_;
    QMap<YRB::SignalState, QSvgWidget*> amber_svgs_;
    QMap<YRB::SignalState, QSvgWidget*> subsid_stop_svgs_;
    QMap<YRB::SignalState, QSvgWidget*> subsid_clear_svgs_;
    QMap<YRB::SignalState, QSvgWidget*> subsid_amber_svgs_;
    YRB::SignalState clear_state_{YRB::SignalState::Caution};
    YRB::SignalState current_state_{YRB::SignalState::On};
    void set_clear_sig_position_(const int x, const int y);
    void set_clear_sig_size_(const int x, const int y);
    void set_caution_sig_position_(const int x, const int y);
    void set_caution_sig_size_(const int x, const int y);
    void set_stop_sig_position_(const int x, const int y);
    void set_stop_sig_size_(const int x, const int y);
    void set_clear_subsid_position_(const int x, const int y);
    void set_clear_subsid_size_(const int x, const int y);
    void set_caution_subsid_position_(const int x, const int y);
    void set_caution_subsid_size_(const int x, const int y);
    void set_stop_subsid_position_(const int x, const int y);
    void set_stop_subsid_size_(const int x, const int y);

    void set_signal_aspect(YRB::SignalState state);
public:
    explicit Signal2(QWidget *parent = nullptr);
public slots:
    void setIntermediate(YRB::SignalState next_state) {
        switch (next_state) {
            case YRB::SignalState::Off:
                clear_svgs_[next_state]->show();
                subsid_clear_svgs_[next_state]->show();
                break;
            case YRB::SignalState::On:
                stop_svgs_[next_state]->show();
                subsid_stop_svgs_[next_state]->show();
                break;
            default:
                amber_svgs_[next_state]->show();
                subsid_amber_svgs_[next_state]->show();
                break;
        }
    }

    void setAspect(YRB::SignalState state) {
        clear_svgs_[YRB::SignalState::On]->hide();
        clear_svgs_[YRB::SignalState::Caution]->hide();
        clear_svgs_[YRB::SignalState::Off]->hide();
        stop_svgs_[YRB::SignalState::On]->hide();
        stop_svgs_[YRB::SignalState::Caution]->hide();
        stop_svgs_[YRB::SignalState::Off]->hide();
        amber_svgs_[YRB::SignalState::On]->hide();
        amber_svgs_[YRB::SignalState::Caution]->hide();
        amber_svgs_[YRB::SignalState::Off]->hide();
        subsid_stop_svgs_[YRB::SignalState::On]->hide();
        subsid_stop_svgs_[YRB::SignalState::Caution]->hide();
        subsid_stop_svgs_[YRB::SignalState::Off]->hide();
        subsid_clear_svgs_[YRB::SignalState::On]->hide();
        subsid_clear_svgs_[YRB::SignalState::Caution]->hide();
        subsid_clear_svgs_[YRB::SignalState::Off]->hide();
        subsid_amber_svgs_[YRB::SignalState::On]->hide();
        subsid_amber_svgs_[YRB::SignalState::Caution]->hide();
        subsid_amber_svgs_[YRB::SignalState::Off]->hide();
        clear_svgs_[state]->show();
        stop_svgs_[state]->show();
        amber_svgs_[state]->show();
        subsid_clear_svgs_[state]->show();
        subsid_stop_svgs_[state]->show();
        subsid_amber_svgs_[state]->show();
    }
    void setSignal(int id, YRB::SignalState state) {
        if(current_state_ == state) return;
        if(id != 2) return;
        state = (state == YRB::SignalState::On) ? state : clear_state_;
        QTimer::singleShot(500, this, [this, state](){setIntermediate(state);});
        QTimer::singleShot(1500, this, [this, state](){setAspect(state);});
        current_state_ = state;
    }

    void update34Status(bool at_danger) {
        clear_state_ = (at_danger) ? YRB::SignalState::Caution : YRB::SignalState::Off;
        YRB::SignalState state = current_state_;
        if(at_danger && current_state_ == YRB::SignalState::Off) state = YRB::SignalState::Caution;
        else if(!at_danger && current_state_ == YRB::SignalState::Caution) state = YRB::SignalState::Off;
        setSignal(2, state);
    }
};

#endif // SIGNAL_2_H
