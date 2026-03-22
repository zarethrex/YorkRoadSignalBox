#ifndef SIGNAL_HXX
#define SIGNAL_HXX

#include <QObject>
#include <QTimer>
#include <QDebug>

#include <memory>

#include "framelever.hxx"
#include "global_params.hxx"

namespace YRB
{
    class BlockSection;

    class Signal : public QObject
    {
        Q_OBJECT
        private:
            const int _id{-1};
            bool protected_block_occupied{false};
            const int _n_aspects{3};
            SignalState signal_aspect_{SignalState::On};
            HomeLever* control_lever;
            QMap<QString, bool> _block_states{};
        public:
            Signal() {}
            Signal(const int& id, int n_aspects=3) : _id(id), _n_aspects(n_aspects) {};
            SignalState getState() const {return signal_aspect_;};
            void addBlock(QString block_id) {
                _block_states.insert(block_id, false);
            }
            bool has_neighbour = false;
            int id() const {return _id;}
            void setLever(HomeLever* lever) {
                control_lever = lever;
            }
            void tryClear(bool is_lever_action) {
                YRB::SignalState state = YRB::SignalState::Off;
                if(is_lever_action) {
                    qDebug() << "Attempting signal clear by lever action for Signal " << _id;
                    state = SignalState::Off;
                    if(has_neighbour) state = (protected_block_occupied) ? SignalState::Off : ((_n_aspects == 3) ? SignalState::Caution : SignalState::On);
                    if(state != SignalState::Off) {
                        qDebug() << "State change blocked by track circuit occupancy";
                    }
                }
                else if(control_lever) {
                    qDebug() << "Automatic signal state change, attempting switch to clear " << _id;
                    state = (control_lever->getState() == LeverState::Off) ? SignalState::On : SignalState::Off;
                    if(state != SignalState::Off) {
                        qDebug() << "Signal held at On by lever position: " << ((control_lever->getState() == LeverState::Off) ? "Off" : "On");
                    }
                }
                if(signal_aspect_ == state) return;
                signal_aspect_ = state;
                QTimer::singleShot(1000, this, [this]{signalAspectChanged(_id, signal_aspect_);});

            }
            void setOn()
            {
                signal_aspect_ = SignalState::On;
                QTimer::singleShot(1000, this, [this]{signalAspectChanged(_id, signal_aspect_);});
            }
        signals:
            void signalAspectChanged(int id, YRB::SignalState state);
        public slots:
            void signalUpdateFromBlock(QString block, bool is_occupied)
            {
                if(!_block_states.contains(block)) return;
                qDebug() << "BLOCK=" << block << " " << " SIGNAL=" << _id << " OCCUPIED=" << is_occupied;
                _block_states[block] = is_occupied;
                if(is_occupied && signal_aspect_ != SignalState::On) {
                    setOn();
                    return;
                }
                if(std::all_of(_block_states.begin(), _block_states.end(), [](const auto& pair) {return !pair;}) && signal_aspect_ != SignalState::Off) {
                    tryClear(false);
                }
            }
    };

    class ShuntSignal : public Signal
    {
        public:
            ShuntSignal(const int& id) : Signal(id) {}
    };
};

#endif // SIGNAL_HXX
