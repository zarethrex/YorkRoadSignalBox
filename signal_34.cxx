#include "signal_34.hxx"

Signal34::Signal34(QWidget *parent)
    : QMainWindow{parent}
{
    this->setGeometry(1.02*YRB::SCALER.screen_width(), 0.03*YRB::SCALER.screen_height(), 0.2*YRB::SCALER.screen_width(), 0.3*YRB::SCALER.screen_height());
    this->setWindowTitle("YR.3/4");
    signal_34_svg_->setGeometry(0, 0, this->width(), this->height());
    signal_34_svg_->show();
    feather_svgs_[YRB::PointsState::Normal] = new QSvgWidget(":/svgs/media/Feather_off.svg", this);
    feather_svgs_[YRB::PointsState::Reverse] = new QSvgWidget(":/svgs/media/Feather_on.svg", this);

    stop_svgs_[YRB::SignalState::Off] = new QSvgWidget(":/svgs/media/Sig34_Off.svg", this);
    stop_svgs_[YRB::SignalState::On] = new QSvgWidget(":/svgs/media/Sig34_RedOn.svg", this);
    clear_svgs_[YRB::SignalState::On] = new QSvgWidget(":/svgs/media/Sig34_Off.svg", this);
    clear_svgs_[YRB::SignalState::Off] = new QSvgWidget(":/svgs/media/Sig34_GreenOn.svg", this);

    subsid_stop_svgs_[YRB::SignalState::On] = new QSvgWidget(":/svgs/media/Sig34Subsid_RedOn.svg", this);
    subsid_stop_svgs_[YRB::SignalState::Off] = new QSvgWidget(":/svgs/media/Sig34Subsid_Off.svg", this);
    subsid_clear_svgs_[YRB::SignalState::On] = new QSvgWidget(":/svgs/media/Sig34Subsid_Off.svg", this);
    subsid_clear_svgs_[YRB::SignalState::Off] = new QSvgWidget(":/svgs/media/Sig34Subsid_GreenOn.svg", this);

    set_stop_sig_size_(0.043*YRB::SCALER.screen_width(), 0.043*YRB::SCALER.screen_width());
    set_stop_sig_position_(0.106*YRB::SCALER.screen_width(), 0.1965*YRB::SCALER.screen_height());

    set_clear_sig_size_(0.043*YRB::SCALER.screen_width(), 0.043*YRB::SCALER.screen_width());
    set_clear_sig_position_(0.106*YRB::SCALER.screen_width(), 0.1465*YRB::SCALER.screen_height());

    set_clear_subsid_position_(0.145*YRB::SCALER.screen_width(), 0.167*YRB::SCALER.screen_width());
    set_clear_subsid_size_(0.02*YRB::SCALER.screen_width(), 0.02*YRB::SCALER.screen_height());

    set_stop_subsid_position_(0.145*YRB::SCALER.screen_width(), 0.215*YRB::SCALER.screen_width());
    set_stop_subsid_size_(0.02*YRB::SCALER.screen_width(), 0.02*YRB::SCALER.screen_height());

    set_feather_size_(0.102*YRB::SCALER.screen_width(), 0.102*YRB::SCALER.screen_width());
    set_feather_position_(0.052*YRB::SCALER.screen_width(), 0.023*YRB::SCALER.screen_height());
    feather_svgs_[YRB::PointsState::Normal]->show();
    feather_svgs_[YRB::PointsState::Reverse]->hide();
    setAspect(YRB::SignalState::On);
}

void Signal34::set_feather_position_(const int x, const int y)
{
    for(QSvgWidget* feather_status : feather_svgs_) {
        feather_status->move(x, y);
    }
}

void Signal34::set_feather_size_(const int x, const int y)
{
    for(QSvgWidget* feather_status : feather_svgs_) {
        feather_status->setFixedSize(x, y);
    }
}

void Signal34::set_clear_sig_position_(const int x, const int y)
{
    for(QSvgWidget* sig_aspect : clear_svgs_) {
        sig_aspect->move(x, y);
    }
}

void Signal34::set_clear_sig_size_(const int x, const int y)
{
    for(QSvgWidget* sig_aspect : clear_svgs_) {
        sig_aspect->setFixedSize(x, y);
    }
}

void Signal34::set_stop_sig_position_(const int x, const int y)
{
    for(QSvgWidget* sig_aspect : stop_svgs_) {
        sig_aspect->move(x, y);
    }
}

void Signal34::set_stop_sig_size_(const int x, const int y)
{
    for(QSvgWidget* sig_aspect : stop_svgs_) {
        sig_aspect->setFixedSize(x, y);
    }
}

void Signal34::set_clear_subsid_position_(const int x, const int y)
{
    for(QSvgWidget* sig_aspect : subsid_clear_svgs_) {
        sig_aspect->move(x, y);
    }
}

void Signal34::set_clear_subsid_size_(const int x, const int y)
{
    for(QSvgWidget* sig_aspect : subsid_clear_svgs_) {
        sig_aspect->setFixedSize(x, y);
    }
}

void Signal34::set_stop_subsid_position_(const int x, const int y)
{
    for(QSvgWidget* sig_aspect : subsid_stop_svgs_) {
        sig_aspect->move(x, y);
    }
}

void Signal34::set_stop_subsid_size_(const int x, const int y)
{
    for(QSvgWidget* sig_aspect : subsid_stop_svgs_) {
        sig_aspect->setFixedSize(x, y);
    }
}
