#ifndef FRAMELEVER_HXX
#define FRAMELEVER_HXX

#include <QMainWindow>
#include <QSvgWidget>
#include <QMap>
#include <QObject>
#include <QDebug>
#include <QThread>
#include <QPushButton>
#include <QScreen>
#include <QGuiApplication>
#include <QSoundEffect>
#include <QTimer>

#include "global_params.hxx"

namespace YRB
{
    class FrameLever : public QObject
    {
        Q_OBJECT
        private:
            QWidget* _parent = nullptr;
            LeverType _type;
            int _coords[2] = {0,0};
            const int _id = -1;
            bool _locked = false;
            const QString name;
            LeverState _current_state = LeverState::Off, _prev_state= LeverState::Off;
        public:
            FrameLever(){}
            FrameLever(const int& id, const QString& lever_name, QWidget* parent, LeverType type) :
                _parent(parent), _type(type), _id(id), name(lever_name)
            {
            }
            LeverState reverse(LeverState state) const
            {
                return (state == LeverState::On) ? LeverState::Off : LeverState::On;
            }
            void moveLever(LeverState state = LeverState::Off, bool points_delay=false);
            void Lock(bool lock_lever) {_locked = lock_lever;}
            bool isLocked() const {return _locked;}
            LeverType getType() const {return _type;}
            LeverState getState() const {return _current_state;}
            int id() const {return _id;}
            void graphics_refresh();
        signals:
            void leverUpdate(int id, YRB::LeverState state);
        public slots:
            void pointsChangedFinished();
    };

    class HomeLever : public FrameLever
    {
        public:
            HomeLever(const int& id, QWidget* parent) : FrameLever(id, QString("Red"), parent, LeverType::Signal) {}
    };

    class PointsLever : public FrameLever
    {
        public:
            PointsLever(const int& id, QWidget* parent) : FrameLever(id, QString("Black"), parent, LeverType::Points){}
    };

    class FacingPointLockLever : public FrameLever
    {
        public:
            FacingPointLockLever(const int& id, QWidget* parent) : FrameLever(id, QString("Blue"), parent, LeverType::PointLock){}
    };

    class SpareLever : public FrameLever
    {
        public:
            SpareLever(const int& id, QWidget* parent) : FrameLever(id, QString("White"), parent, LeverType::Spare){}
    };
};



#endif // FRAMELEVER_HXX
