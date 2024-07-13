#ifndef GLOBAL_PARAMS_HXX
#define GLOBAL_PARAMS_HXX

#include <QMap>

namespace YRB
{
    enum class SignalState
    {
        On,
        Caution,
        Off
    };

    enum class LeverState
    {
        Off,
        Mid,
        On
    };


    enum class PointsState
    {
        Normal,
        Reverse
    };

    enum class TrackCircuitStatus
    {
        Clear,
        Occupied
    };

    enum class LeverType
    {
        Signal,
        Points,
        PointLock,
        Spare
    };

    const QList<QString> block_ids{"A", "B", "C", "D", "E1", "E2", "F", "G"};
    const QMap<int, LeverType> levers{
        {1, LeverType::Spare},
        {2, LeverType::Signal},
        {3, LeverType::Signal},
        {4, LeverType::Signal},
        {5, LeverType::Spare},
        {6, LeverType::Points},
        {7, LeverType::PointLock},
        {8, LeverType::Spare},
        {9, LeverType::Spare},
        {10, LeverType::Spare},
        {11, LeverType::Spare}
    };
    const QList<int> signal_ids{2, 3, 4};
    const QList<int> points_ids{6};

};

#endif // GLOBAL_PARAMS_HXX
