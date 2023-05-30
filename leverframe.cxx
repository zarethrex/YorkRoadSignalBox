#include "leverframe.hxx"

YRB::LeverFrame::LeverFrame(QWidget* parent)
{
    _parent = parent;

    for(int i{1}; i < 12; ++i)
    {
        if(QVector<int>({1, 5, 8, 9, 10, 11}).contains(i)) {
            _levers[i] = new YRB::SpareLever(i, _parent);
        }
        else if(QVector<int>({2, 3, 4}).contains(i)) {
            _levers[i] = new YRB::HomeLever(i, _parent);
        }
        else if(i == 7) {
            _levers[i] = new YRB::FacingPointLockLever(i, _parent);
        }
        else {
            _levers[i] = new YRB::PointsLever(i, _parent);
        }
        connect(_levers[i], &YRB::FrameLever::leverUpdate, this, &YRB::LeverFrame::frameLeverUpdate);
    }

}

void YRB::LeverFrame::update()
{
    qDebug() << "Running Frame Update...";

    for(const auto& tc : track_circuits_) {
        tc->update();
    }
}
