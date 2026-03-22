#ifndef SIGNAL34_HXX
#define SIGNAL34_HXX

#include <QMainWindow>
#include <QObject>
#include <QSvgWidget>
#include <QMap>
#include <QThread>
#include <QTimer>
#include "scaling.hxx"
#include "global_params.hxx"
#include <memory>

class Signal34 : public QMainWindow
{
    Q_OBJECT
private:
    QSvgWidget* signal_34_svg_ = new QSvgWidget(":/svgs/media/Signal34.svg", this);
    QMap<YRB::PointsState, QSvgWidget*> feather_svgs_;
    QMap<YRB::SignalState, QSvgWidget*> stop_svgs_;
    QMap<YRB::SignalState, QSvgWidget*> clear_svgs_;
    QMap<YRB::SignalState, QSvgWidget*> subsid_stop_svgs_;
    QMap<YRB::SignalState, QSvgWidget*> subsid_clear_svgs_;
    YRB::SignalState signal_state_ = YRB::SignalState::On;
    YRB::PointsState points_state_ = YRB::PointsState::Normal;
    void set_feather_position_(const int x, const int y);
    void set_feather_size_(const int x, const int y);
    void set_clear_sig_position_(const int x, const int y);
    void set_clear_sig_size_(const int x, const int y);
    void set_stop_sig_position_(const int x, const int y);
    void set_stop_sig_size_(const int x, const int y);
    void set_clear_subsid_position_(const int x, const int y);
    void set_clear_subsid_size_(const int x, const int y);
    void set_stop_subsid_position_(const int x, const int y);
    void set_stop_subsid_size_(const int x, const int y);
    void set_signal_aspect(YRB::SignalState state);
public:
    explicit Signal34(QWidget *parent = nullptr);
signals:
    void atDanger(bool danger);
public slots:
    void setFeatherIndicator(YRB::PointsState state) {
        points_state_ = state;
        feather_svgs_[YRB::PointsState::Normal]->hide();
        feather_svgs_[YRB::PointsState::Reverse]->hide();
        if(signal_state_ != YRB::SignalState::Off) {
            feather_svgs_[YRB::PointsState::Normal]->show();
            return;
        }
        feather_svgs_[points_state_]->show();
    }

    void setIntermediate() {
        clear_svgs_[YRB::SignalState::Off]->show();
        stop_svgs_[YRB::SignalState::On]->show();
        subsid_stop_svgs_[YRB::SignalState::On]->show();
        subsid_clear_svgs_[YRB::SignalState::Off]->show();
    }

    void setSignal(int id, YRB::SignalState state) {
        if(signal_state_ == state) return;
        if(id != 3 && id != 4) return;
        signal_state_ = state;
        emit setFeatherIndicator(points_state_);
        QTimer::singleShot(500, this, [this](){setIntermediate();});
        QTimer::singleShot(1500, this, [this, state](){setAspect(state);});
        emit atDanger(signal_state_ == YRB::SignalState::On);
    }

    void setAspect(YRB::SignalState state) {
        clear_svgs_[YRB::SignalState::On]->hide();
        clear_svgs_[YRB::SignalState::Off]->hide();
        stop_svgs_[YRB::SignalState::On]->hide();
        stop_svgs_[YRB::SignalState::Off]->hide();
        subsid_stop_svgs_[YRB::SignalState::On]->hide();
        subsid_stop_svgs_[YRB::SignalState::Off]->hide();
        subsid_clear_svgs_[YRB::SignalState::On]->hide();
        subsid_clear_svgs_[YRB::SignalState::Off]->hide();
        if(clear_svgs_[state]) clear_svgs_[state]->show();
        if(stop_svgs_[state]) stop_svgs_[state]->show();
        if(subsid_clear_svgs_[state]) subsid_clear_svgs_[state]->show();
        if(subsid_stop_svgs_[state]) subsid_stop_svgs_[state]->show();
    }
};

#endif // SIGNAL34_HXX
