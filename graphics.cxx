#include "graphics.hxx"

YRB::Graphics::Graphics(QWidget* parent)
{
    _parent = parent;
    _frame_svg = new QSvgWidget(QString(":/svgs/media/YRFrame_Flattened.svg"), _parent);
    _frame_svg->setFixedSize(SCALER.screen_width(), SCALER.screen_height());

    _point_ind_svgs = {
        {YRB::PointsState::Normal, {
                 {YRB::SignalState::On, new QSvgWidget(":/svgs/media/PointsIndicatorOn.svg", _parent)},
                 {YRB::SignalState::Off, new QSvgWidget(":/svgs/media/PointsIndicatorOff.svg", _parent)}
            }
        },
        {YRB::PointsState::Reverse, {
             {YRB::SignalState::On, new QSvgWidget(":/svgs/media/PointsIndicatorOn.svg", _parent)},
             {YRB::SignalState::Off, new QSvgWidget(":/svgs/media/PointsIndicatorOff.svg", _parent)}
            }
        }
    };

    _set_point_indicator_position(YRB::PointsState::Normal, SCALER.scale_width(156.5), SCALER.scale_height(212));
    _set_point_indicator_size(YRB::PointsState::Normal, SCALER.scale_width(20), SCALER.scale_height(20));
    _set_point_indicator_position(YRB::PointsState::Reverse, SCALER.scale_width(156.5), SCALER.scale_height(249.25));
    _set_point_indicator_size(YRB::PointsState::Reverse, SCALER.scale_width(20), SCALER.scale_height(20));

    _block_svgs['0'] = {
        {false, nullptr},
        {true, nullptr}
    };

    for(const char& block : QList<char>({'A', 'B', 'C', 'D', 'E', 'F', 'G'})) {
        QString _block{block};
        _block_svgs[block] = {
          {false, new QSvgWidget(":/svgs/media/Block" + _block + "_On.svg", _parent)},
          {true, new QSvgWidget(":/svgs/media/Block" + _block + "_Off.svg", _parent)}
        };
    }
    _set_block_size('A', SCALER.scale_width(60), SCALER.scale_height(10));
    _set_block_position('A', SCALER.scale_width(42.5), SCALER.scale_height(153.4));
    _block_svgs['A'][true]->hide();
    _set_block_size('B', SCALER.scale_width(60), SCALER.scale_height(10));
    _set_block_position('B', SCALER.scale_width(100), SCALER.scale_height(153.4));
    _block_svgs['B'][true]->hide();
    _set_block_size('C', SCALER.scale_width(70), SCALER.scale_height(10));
    _set_block_position('C', SCALER.scale_width(161), SCALER.scale_height(153.4));
    _block_svgs['C'][true]->hide();
    _set_block_size('D', SCALER.scale_width(100), SCALER.scale_height(10));
    _set_block_position('D', SCALER.scale_width(234), SCALER.scale_height(153.4));
    _block_svgs['D'][true]->hide();
    _set_block_size('E', SCALER.scale_width(105), SCALER.scale_height(70));
    _set_block_position('E', SCALER.scale_width(333), SCALER.scale_height(99.8));
    _block_svgs['E'][true]->hide();
    _set_block_size('F', SCALER.scale_width(60), SCALER.scale_height(10));
    _set_block_position('F', SCALER.scale_width(437), SCALER.scale_height(153.4));
    _block_svgs['F'][true]->hide();
    _set_block_size('G', SCALER.scale_width(60), SCALER.scale_height(10));
    _set_block_position('G', SCALER.scale_width(437), SCALER.scale_height(107.3));
    _block_svgs['G'][true]->hide();

    for(const int& sig_id : signal_ids)
    {
        _map_indicator_svgs[sig_id] = {
            {SignalState::Off, new QSvgWidget(":/svgs/media/Sig_Off.svg", _parent)},
            {SignalState::On, new QSvgWidget(":/svgs/media/Sig_On.svg", _parent)},
        };
        _set_signal_size_at(sig_id, SCALER.scale_width(13), SCALER.scale_height(13));
    }

    _place_signal_at(2, SCALER.scale_width(155), SCALER.scale_height(136));
    _place_signal_at(3, SCALER.scale_width(336), SCALER.scale_height(124));
    _place_signal_at(4, SCALER.scale_width(336), SCALER.scale_height(166));

    for(const auto& [id, type] : levers.toStdMap()) {
        QString _lever_colour;
        switch(type) {
            case LeverType::Signal:
                _lever_colour = "Red";
                break;
            case LeverType::Points:
                _lever_colour = "Black";
                break;
            case LeverType::PointLock:
                _lever_colour = "Blue";
                break;
            default:
                _lever_colour = "White";
                break;
        }

        _lever_svgs[id] = {
            {LeverState::Off, new QSvgWidget(QString(":/svgs/media/")+_lever_colour+QString("_LeverBack.svg"), _parent)},
            {LeverState::Mid, new QSvgWidget(QString(":/svgs/media/")+_lever_colour+QString("_LeverMid.svg"), _parent)},
            {LeverState::On, new QSvgWidget(QString(":/svgs/media/")+_lever_colour+QString("_LeverForward.svg"), _parent)}
        };
        _place_lever_at(id, SCALER.scale_width(32+(id-1)*45), 0.72*SCALER.screen_height());
        _set_lever_size_at(id, SCALER.scale_width(25), SCALER.scale_height(100));
        _lever_svgs[id][LeverState::On]->hide();
        _lever_svgs[id][LeverState::Mid]->hide();
        _point_ind_svgs[YRB::PointsState::Normal][YRB::SignalState::On]->show();
        _point_ind_svgs[YRB::PointsState::Normal][YRB::SignalState::Off]->hide();
        _point_ind_svgs[YRB::PointsState::Reverse][YRB::SignalState::On]->hide();
        _point_ind_svgs[YRB::PointsState::Reverse][YRB::SignalState::Off]->show();
    }
}

void YRB::Graphics::_set_point_indicator_position(const YRB::PointsState state, const int x, const int y) {
    for(QSvgWidget* graphic : _point_ind_svgs[state]) {
        graphic->move(x, y);
    }
}

void YRB::Graphics::_set_point_indicator_size(const YRB::PointsState state, const int x, const int y) {
    for(QSvgWidget* graphic : _point_ind_svgs[state]) {
        graphic->setFixedSize(x, y);
    }
}

void YRB::Graphics::_set_block_position(const char block, const int x, const int y) {
    for(QSvgWidget* graphic : _block_svgs[block]) {
        graphic->move(x, y);
    }
}

void YRB::Graphics::_set_block_size(const char block, const int x, const int y) {
    for(QSvgWidget* graphic : _block_svgs[block]) {
        graphic->setFixedSize(x, y);
    }
}

void YRB::Graphics::_place_signal_at(const int id, const int x, const int y) {
    for(QSvgWidget* graphic : _map_indicator_svgs[id]) {
        graphic->move(x, y);
    }
}

void YRB::Graphics::_set_signal_size_at(const int id, const int x, const int y) {
    for(QSvgWidget* graphic : _map_indicator_svgs[id]) {
        graphic->setFixedSize(x, y);
    }
}

void YRB::Graphics::_place_lever_at(const int id, const int x, const int y) {
    for(QSvgWidget* graphic : _lever_svgs[id]) {
        graphic->move(x, y);
    }
}

void YRB::Graphics::_set_lever_size_at(const int id, const int x, const int y) {
    for(QSvgWidget* graphic : _lever_svgs[id]) {
        graphic->setFixedSize(x, y);
    }
}
