#include "audio.hxx"

YRB::Audio::Audio(QWidget* parent)
{
    parent_ = parent;
    lever_sound_->setSource(QUrl::fromLocalFile(":/audio/audio/lever_move_1.wav"));
    lever_fail_->setSource(QUrl::fromLocalFile(":/audio/audio/lever_fail.wav"));
    indicator_->setSource(QUrl::fromLocalFile(":/audio/audio/panel_update.wav"));
    lever_sound_->setLoopCount(1);
    lever_fail_->setLoopCount(1);
    indicator_->setLoopCount(1);
}
