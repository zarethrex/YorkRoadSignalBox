#ifndef LEVERFRAME_HXX
#define LEVERFRAME_HXX

#include <stdlib.h>

#include <QMap>
#include <QSvgWidget>
#include <QDebug>
#include <QGuiApplication>
#include <QScreen>
#include <QSoundEffect>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QThread>

#include "scaling.hxx"
#include "framelever.hxx"
#include "trackcircuit.hxx"
#include "global_params.hxx"

namespace YRB
{
    class LeverFrame : public QObject
    {
        Q_OBJECT
        private:
            QMap<int, FrameLever*> _levers;
            QMap<QString, TrackCircuit*> track_circuits_;
            QWidget* _parent = nullptr;
        public:
            LeverFrame(QWidget* parent);
            FrameLever* operator[](const int& i)
            {
                return _levers[i];
            }
            QList<int> levers() const {return _levers.keys();}
            void update();
            void moveLever(const int& i, LeverState lever_state, bool points_move)
            {
                if(_levers[i]->getState() == LeverState::Mid) emit leverFailed();
                else if(lever_state != LeverState::Mid) emit leverMove();
                else emit leverFailed();
                _levers[i]->moveLever(lever_state, points_move);
            }
            QMap<QString, TrackCircuit*> getTrackCircuits() {
                return track_circuits_;
            }
        signals:
            void frameUpdate(int id, YRB::LeverState state);
            void leverFailed();
            void leverMove();
            void panelUpdate();
            void panelChanged();
        public slots:
            void frameLeverUpdate(int id, YRB::LeverState state) {
                emit frameUpdate(id, state);
            }
            void panelIndicatorUpdate() {
                QTimer::singleShot(1000, this, &LeverFrame::panelUpdate);
            }
    };
};
#endif // LEVERFRAME_HXX
