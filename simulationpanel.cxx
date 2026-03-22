#include "simulationpanel.hxx"

SimulationPanel::SimulationPanel(QWidget *parent)
    : QMainWindow{parent}
{
    this->setGeometry(1.02*YRB::SCALER.screen_width(), 0.82*YRB::SCALER.screen_height(), 0.2*YRB::SCALER.screen_width(), 0.1*YRB::SCALER.screen_height());
    this->setWindowTitle("Simulations");
    run_button_->move(0.3*this->width(), 0.1*this->height());
    run_button_->setText("Run");
    run_button_->show();
}
