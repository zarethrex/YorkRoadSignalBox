#include "leverframe.hxx"

YRB::LeverFrame::LeverFrame(QWidget* parent)
{
    _parent = parent;

    for(const auto& [id, type] : YRB::levers.toStdMap()) {
        switch (type) {
            case LeverType::Spare:
                _levers[id] = new YRB::SpareLever(id, _parent);
                break;
            case LeverType::Signal:
                _levers[id] = new YRB::HomeLever(id, _parent);
                break;
            case LeverType::PointLock:
                _levers[id] = new YRB::HomeLever(id, _parent);
                break;
            default:
                _levers[id] = new YRB::PointsLever(id, _parent);
        };
        connect(_levers[id], &YRB::FrameLever::leverUpdate, this, &YRB::LeverFrame::frameLeverUpdate);
    }

}

void YRB::LeverFrame::update()
{
    qDebug() << "Running Frame Update...";

    for(const auto& tc : track_circuits_) {
        tc->update();
    }
}
