#ifndef COMMON_HEADER_H
#define COMMON_HEADER_H

enum AgvState
{
    NONE_STATE,
    INIT_STATE,
    SERVO_STATE,
    CURRENT_STATE,
    VELOCITY_STATE,
    POSITION_STATE,
    OFF_STATE
};

enum ShowType
{
    ZERO,
    SIGNAL,
    CURRENT1,
    CURRENT2,
    CURRENT3,
    CURRENT4,
    CURRENT5,
    CURRENT6,
    CURRENT7,
    CURRENT8,
    VELOCITY1,
    VELOCITY2,
    VELOCITY3,
    VELOCITY4,
    VELOCITY5,
    VELOCITY6,
    VELOCITY7,
    VELOCITY8,
    POSITION1,
    POSITION2,
    POSITION3,
    POSITION4,
    POSITION5,
    POSITION6,
    POSITION7,
    POSITION8
};

enum CurveType
{
    NONE_TYPE,
    SIN_TYPE,
    SQUARE_TYPE
};


#endif // COMMON_HEADER_H
