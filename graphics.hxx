#ifndef GRAPHICS_HXX
#define GRAPHICS_HXX

#include <QSvgWidget>
#include <QMap>
#include "global_params.hxx"
#include "scaling.hxx"

namespace YRB {
    class Graphics : public QObject
    {
    QWidget* _parent = nullptr;
    QSvgWidget* _frame_svg = nullptr;
    QMap<int, QMap<SignalState, QSvgWidget*>> _map_indicator_svgs;
    QMap<int, QMap<LeverState, QSvgWidget*>> _lever_svgs;
    QMap<char, QMap<bool, QSvgWidget*>> _block_svgs;
    QMap<PointsState, QMap<SignalState, QSvgWidget*>> _point_ind_svgs;
    Scaler* _scaler = new Scaler;
    void _place_signal_at(const int id, const int x, const int y);
    void _place_lever_at(const int id, const int x, const int y);
    void _set_lever_size_at(const int id, const int x, const int y);
    void _set_signal_size_at(const int id, const int x, const int y);
    void _set_point_indicator_size(const YRB::PointsState state, const int x, const int y);
    void _set_point_indicator_position(const YRB::PointsState state, const int x, const int y);
    void _set_block_position(const char block, const int x, const int y);
    void _set_block_size(const char block, const int x, const int y);
    public:
        Graphics(QWidget* parent);
    public slots:
        void updateSignalGraphic(int id, YRB::SignalState state) {
            _map_indicator_svgs[id][YRB::SignalState::Off]->hide();
            _map_indicator_svgs[id][YRB::SignalState::On]->hide();
            if(state == YRB::SignalState::Caution && !_map_indicator_svgs[id].contains(YRB::SignalState::Caution)) {
                state = YRB::SignalState::On;
            }
            _map_indicator_svgs[id][state]->show();
        }
        void updateLeverGraphic(int id, YRB::LeverState state) {
            _lever_svgs[id][YRB::LeverState::On]->hide();
            _lever_svgs[id][YRB::LeverState::Mid]->hide();
            _lever_svgs[id][YRB::LeverState::Off]->hide();
            _lever_svgs[id][state]->show();
        }
        void updatePointsGraphic(YRB::PointsState state) {
            if(state == YRB::PointsState::Normal) {
                _point_ind_svgs[YRB::PointsState::Normal][YRB::SignalState::On]->show();
                _point_ind_svgs[YRB::PointsState::Normal][YRB::SignalState::Off]->hide();
                _point_ind_svgs[YRB::PointsState::Reverse][YRB::SignalState::On]->hide();
                _point_ind_svgs[YRB::PointsState::Reverse][YRB::SignalState::Off]->show();
            }
            else {
                _point_ind_svgs[YRB::PointsState::Normal][YRB::SignalState::On]->hide();
                _point_ind_svgs[YRB::PointsState::Normal][YRB::SignalState::Off]->show();
                _point_ind_svgs[YRB::PointsState::Reverse][YRB::SignalState::On]->show();
                _point_ind_svgs[YRB::PointsState::Reverse][YRB::SignalState::Off]->hide();
            }
        }
        void updateBlockGraphic(char block, bool occupied)
        {
           if(!_block_svgs[block][false]) {
               return;
           }
           _block_svgs[block][false]->hide();
           _block_svgs[block][true]->hide();
           _block_svgs[block][occupied]->show();
        }
    };
};
#endif // GRAPHICS_HXX
