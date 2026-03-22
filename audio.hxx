#ifndef AUDIO_HXX
#define AUDIO_HXX

#include <QObject>
#include <QThread>
#include <QSoundEffect>

namespace YRB
{
    class Audio : public QObject
    {
        Q_OBJECT
    private:
        QWidget* parent_ = nullptr;
        QSoundEffect* lever_sound_ = new QSoundEffect(this);
        QSoundEffect* lever_fail_ = new QSoundEffect(this);
        QSoundEffect* indicator_ = new QSoundEffect(this);
    public:
        Audio(QWidget* parent);
    public slots:
        void playLever() {
            lever_sound_ -> play();
        }
        void playFail() {
            lever_fail_ -> play();
        }
        void playIndicator() {
            QThread::msleep(100);
            indicator_ -> play();
        }
    };
};

#endif // AUDIO_HXX
