#ifndef SIGNAL_HXX
#define SIGNAL_HXX

#include <QObject>
#include <QTimer>
#include <QDebug>
#include "global_params.hxx"

namespace YRB
{
    class BlockSection;

    class Signal : public QObject
    {
        Q_OBJECT
        private:
            const int _id = -1;
            bool protected_block_occupied = false;
            const int _n_aspects = 3;
            SignalState signal_aspect_ = SignalState::On;
            LeverState control_position_ = LeverState::Off;
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
            void tryClear(bool from_track_circuit) {
                YRB::SignalState state = YRB::SignalState::Off;
                if(!from_track_circuit) {
                    control_position_ = LeverState::On;
                    state = SignalState::Off;
                    if(has_neighbour) state = (protected_block_occupied) ? SignalState::Off : ((_n_aspects == 3) ? SignalState::Caution : SignalState::On);
                }
                else {
                    state = (control_position_ == LeverState::Off) ? SignalState::On : SignalState::Off;
                }
                signal_aspect_ = state;
                QTimer::singleShot(1000, this, [this]{signalAspectChanged(_id, signal_aspect_);});

            }
            void setOn(bool from_track_circuit)
            {
                signal_aspect_ = SignalState::On;
                if(!from_track_circuit) {
                    control_position_ = LeverState::Off;
                }
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
                    setOn(true);
                    return;
                }
                if(std::all_of(_block_states.begin(), _block_states.end(), [](const auto& pair) {return !pair;}) && signal_aspect_ != SignalState::Off) {
                    tryClear(true);
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
