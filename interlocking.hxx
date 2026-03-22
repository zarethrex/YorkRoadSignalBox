#ifndef INTERLOCKING_HXX
#define INTERLOCKING_HXX

#include <QPair>
#include <QSoundEffect>
#include <QObject>
#include <memory>

#include "signal.hxx"
#include "points.hxx"
#include "framelever.hxx"
#include "leverframe.hxx"
#include "blocksection.hxx"

namespace YRB
{
    typedef QMap<int, LeverState> interlock_coordinate;
    typedef QMap<LeverState, interlock_coordinate> lever_logic;
    typedef QMap<int, lever_logic> interlock_logic;
    typedef QMap<int, QPair<FrameLever*, Points*>> points_connection;
    typedef QPair<Signal*, SignalState> lever_active_signal_state;
    typedef QMap<int, QPair<FrameLever*, lever_active_signal_state>> signal_connection;

    class InterLocking : public QObject
    {
        Q_OBJECT
        private:
            LeverFrame* lever_frame_;
            interlock_logic _logic;
            QMap<QString, BlockSection*> _block_sections;
            QMap<int, Signal*> _signals;
            Points* _points;
            points_connection _point_lever_connections;
            signal_connection _signal_lever_connections;

            LeverState reverse(LeverState state) const
            {
                return FrameLever().reverse(state);
            }

            void _setup_block_sections();
            void _create_block_sections();
            void _create_logic_table();
            void _add_points();
            void _add_signals();
            void _connect_levers();
            void _perform_action(const int& i);
            void _connect(const int& id, HomeLever* lever, Signal* signal, SignalState aspect = SignalState::Off);
            void _connect(const int& id, PointsLever* lever, Points* points);

        public:
            InterLocking(LeverFrame* lever_frame);
            BlockSection* getBlockSection(const QString& id) const {return _block_sections[id];}
            void setOccupied(QString block, bool is_occupied=true);
            bool Query(const int& id);
            void update(const int& id);
            Points* getPoints() const {return _points;}
        signals:
            void broadcastSignal(int id, YRB::SignalState state);
            void broadcastPoints(YRB::PointsState state);
        public slots:
            void signalAspectUpdate(int id, YRB::SignalState state) {
                emit broadcastSignal(id, state);
            }
            void pointsUpdate(YRB::PointsState state) {
                qDebug() << "Updating points";
                emit broadcastPoints(state);
            }
            void linkBlocks(QString block_1_id, QString block_2_id);

    };
};
#endif // INTERLOCKING_HXX
