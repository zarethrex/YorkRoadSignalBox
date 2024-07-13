#ifndef BLOCKSECTION_HXX
#define BLOCKSECTION_HXX

#include "global_params.hxx"
#include "signal.hxx"
#include "points.hxx"
#include "trackcircuit.hxx"

namespace YRB
{
    class BlockSection : public QObject
    {
        Q_OBJECT
        private:
            const QString _id = "";
            Signal* _block_signal = nullptr;
            QList<TrackCircuit*> track_circuits_ = {};
            Points* _points = nullptr;
            SignalState _required_state = SignalState::Off;
            QList<PointsState> _required_points_state = {};
            BlockSection* _next = nullptr;
            BlockSection* _prev = nullptr;
            const QList<Signal*> _counter_signals = {}; // Signals in opposing direction
        public:
            BlockSection(){}
            BlockSection(const QString id) : _id(id) {}
            void addTrackCircuits(const QList<TrackCircuit*>& circuits) {
                track_circuits_.append(circuits);
            }
            void setRequiredState(SignalState state) {_required_state = state;}
            void setRequiredPointsState(PointsState state) {_required_points_state = {state};}
            void setRequiredPointsState(QList<PointsState> states) {_required_points_state = states;}
            void setBlockSignal(Signal* signal) {
                _block_signal = signal;
                if(signal) signal->addBlock(_id);
            }
            void setBlockPoints(Points* points) {_points = points;}
            void setOccupied(bool is_occupied)
            {
                if(_block_signal) emit blockStatusChanged(_id, is_occupied);
            };
            TrackCircuitStatus getState();
            BlockSection* getNext() {return _next;}
            BlockSection* getPrevious() {return _prev;}
            SignalState getRequiredState() const {return _required_state;}
            Signal* getBlockSignal() const {return _block_signal;}
            Points* getPoints() const {return _points;}
            QString id() const {return _id;}
            void setNeighbour(BlockSection* other){
                _next = other;
                qDebug() << "Set Neighbour for " << id() << " to " << other->id();
                if(_block_signal) _block_signal->has_neighbour = true;
                _next->setPrevious(this);
            }
            void setPrevious(BlockSection* other){
                _prev = other;
            }
       signals:
            void blockStatusChanged(QString, bool);

    };
};

#endif // BLOCKSECTION_HXX
