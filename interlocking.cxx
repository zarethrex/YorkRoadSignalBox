#include "interlocking.hxx"

YRB::InterLocking::InterLocking(YRB::LeverFrame* lever_frame)
{
    lever_frame_ = lever_frame;

    // Qt is not happy without having at least one template entry

    _signal_lever_connections = {{1,{new FrameLever, {new Signal, YRB::SignalState::Off}}}};
    _point_lever_connections = {{6, {new FrameLever, new Points}}};

    _setup_block_sections();
    _add_points();
    _add_signals();
    _connect_levers();
    _create_logic_table();

}

void YRB::InterLocking::_create_logic_table()
{

    _logic[4] = {{YRB::LeverState::On,
                    {{6, YRB::LeverState::Off}, {3, YRB::LeverState::Off}}},
                 {YRB::LeverState::Off, {}}
                };
    _logic[3] = {{YRB::LeverState::On,
                  {{6, YRB::LeverState::On}, {4, YRB::LeverState::Off}}},
                 {YRB::LeverState::Off, {}}
                };
    _logic[6] = {{YRB::LeverState::On,
                  {{4, YRB::LeverState::Off}, {3, YRB::LeverState::Off}}},
                 {YRB::LeverState::Off,
                  {{4, YRB::LeverState::Off}, {3, YRB::LeverState::Off}}},
                };
}

void YRB::InterLocking::update(const int& i)
{
    if(!_logic.keys().contains(i)) return;    // Safety for dev purposes in case all levers not set up yet

    lever_logic _temp = _logic[i];

    interlock_coordinate _coords = _temp[reverse(lever_frame_->operator[](i)->getState())];

    for(auto lever : _coords.keys())
    {
        bool toLock = lever_frame_->operator[](lever)->getState() != _coords[lever] && lever_frame_->operator[](lever)->getState() != YRB::LeverState::Mid;

        if(toLock) qDebug() << "Lever "<< i << " Locked by Lever " << lever << " !";
        lever_frame_->operator[](i)->Lock(toLock);
        if(toLock) return;
    }

    qDebug() << "Interlocking for Lever " << i;

    // As lever move successful, lock all levers which have this lever as a requirement and the state is not met

    for(auto lever : _coords.keys())
    {
        lever_logic _lever_log = _logic[lever];
        if(_lever_log[reverse(lever_frame_->operator[](lever)->getState())].contains(i))
        {
            // Checks that not only the lever is present in other lever lock definition, but attempted move is not a valid
            if(_lever_log[reverse(lever_frame_->operator[](lever)->getState())][i] != reverse(lever_frame_->operator[](i)->getState()))
            {
                qDebug() << "Locking Lever " << lever;
                lever_frame_->operator[](lever)->Lock(true);
            }
            else
            {
                qDebug() << "Releasing Lever " << lever;
                lever_frame_->operator[](lever)->Lock(false);
            }
        }
    }

}

void YRB::InterLocking::_perform_action(const int& i)
{
     qDebug() << "Lever state: " << ((lever_frame_->operator[](i)->getState() == YRB::LeverState::Off) ? "Off" : "On");
    if(_signal_lever_connections.contains(i))
    {
        lever_active_signal_state state = _signal_lever_connections[i].second;

        if(reverse(lever_frame_->operator[](i)->getState()) == YRB::LeverState::On)
        {
            qDebug() << "Setting Signal " << state.first->id() << " to On";
            state.first->setOn(true);
        }
        else
        {
            qDebug() << "Clearing Signal " << state.first->id() << " to Off: Aspect " << int(state.first->getState());
            state.first->tryClear(state.second);
        }
    }
    else if(_point_lever_connections.contains(i))
    {
        if(lever_frame_->operator[](i)->getState() == YRB::LeverState::On)
        {
            qDebug() << "Setting Points to Reverse";
            _point_lever_connections[i].second->setState(YRB::PointsState::Reverse);
            linkBlocks('E', 'G');
        }
        else {
            qDebug() << "Setting Points to Normal";
            _point_lever_connections[i].second->setState(YRB::PointsState::Normal);
            linkBlocks('E', 'F');
        }
    }
}

bool YRB::InterLocking::Query(const int& id)
{
    update(id);

    qDebug() << "Moving Lever " << id;

    if(lever_frame_->operator[](id)->isLocked())
    {
        lever_frame_->moveLever(id, YRB::LeverState::Mid, id == _points->id());
        return false;
    }
    else
    {
        lever_frame_->moveLever(id, YRB::LeverState::Off, id == _points->id());
    }

    _perform_action(id);

    return true;
}

void YRB::InterLocking::linkBlocks(char block_1_id, char block_2_id)
{
    if(_block_sections.contains(block_1_id) && _block_sections.contains(block_2_id))
    {
        _block_sections[block_1_id]->setNeighbour(_block_sections[block_2_id]);
    }
}

void YRB::InterLocking::setOccupied(char block, bool is_occupied)
{
    _block_sections[block]->setOccupied(is_occupied);
}

void YRB::InterLocking::_connect(const int& id, YRB::HomeLever* lever, YRB::Signal* signal, YRB::SignalState aspect)
{
    _signal_lever_connections[id] = {};
    _signal_lever_connections[id] = {lever, {signal, aspect}};
    connect(signal, &YRB::Signal::signalAspectChanged, this, &YRB::InterLocking::signalAspectUpdate);
}

void YRB::InterLocking::_connect(const int& id, YRB::PointsLever* lever, YRB::Points* points)
{
    _point_lever_connections[id] = {lever, points};
    connect(points, &YRB::Points::pointsStateChanged, this, &YRB::InterLocking::pointsUpdate);
}

void YRB::InterLocking::_setup_block_sections()
{
    QMap<QString, TrackCircuit*> _lf_tc = lever_frame_->getTrackCircuits();

    for(const char& alpha : QList<char>({'A', 'B', 'C', 'D', 'E', 'F', 'G'})) {
        _block_sections[alpha] = new YRB::BlockSection(alpha);
    }
    _block_sections['C']->setBlockSignal(_signals[2]);
    connect(_block_sections['C'], &YRB::BlockSection::blockStatusChanged, _signals[2], &YRB::Signal::signalUpdateFromBlock);
    _block_sections['F']->setBlockSignal(_signals[3]);
    connect(_block_sections['F'], &YRB::BlockSection::blockStatusChanged, _signals[3], &YRB::Signal::signalUpdateFromBlock);
    _block_sections['E']->setBlockSignal(_signals[4]);
    connect(_block_sections['E'], &YRB::BlockSection::blockStatusChanged, _signals[4], &YRB::Signal::signalUpdateFromBlock);
    linkBlocks('A', 'B');
    linkBlocks('B', 'C');
    linkBlocks('C', 'D');
    linkBlocks('D', 'E');
    linkBlocks('E', 'F');
}

void YRB::InterLocking::_add_points()
{
    _points = new YRB::Points(6);

    _block_sections['E']->setBlockPoints(_points);
    _block_sections['E']->setRequiredPointsState(PointsState::Normal);
    _block_sections['F']->setBlockPoints(_points);
    _block_sections['F']->setRequiredPointsState(PointsState::Reverse);
}

void YRB::InterLocking::_add_signals()
{
    _signals[2] = new YRB::Signal(2);
    _signals[3] = new YRB::Signal(3);
    _signals[4] = new YRB::Signal(4);

    _block_sections['C']->setBlockSignal(_signals[2]);
    _block_sections['C']->setRequiredState(YRB::SignalState::Off);

    _block_sections['E']->setBlockSignal(_signals[4]);
    _block_sections['E']->setRequiredState(YRB::SignalState::Off);

    _block_sections['F']->setBlockSignal(_signals[3]);
    _block_sections['F']->setRequiredState(YRB::SignalState::Off);
}

void YRB::InterLocking::_connect_levers()
{
    for(const int& sig_id : QList<int>({2, 3, 4})) {
        _connect(sig_id, (HomeLever*)lever_frame_->operator[](sig_id), _signals[sig_id], YRB::SignalState::Off);
    }

    _connect(6, (PointsLever*)lever_frame_->operator[](6), _points);

}

