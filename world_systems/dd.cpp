#include "coordinate.hpp"
#include "cmath"

DD::DD() {
    deg = 0;
}

DD::DD(float degrees) {
    deg = degrees;
}

// DMS DD::dms() {
//     int degrees = int(deg);
//     float fptr;
//     float minDec = std::modf(deg, &fptr) * 60;
//     int minutes = int(minDec);
//     float seconds = std::modf(minDec, &fptr) * 60;
//     return DMS(degrees, minutes, seconds);
// }

// DMS DD::dms() {
//     float fDeg;
//     float minDec = std::modf(deg, &fDeg) * 60;
//     int degrees = int(fDeg);
//     float fMin;
//     float seconds = std::modf(minDec, &fMin) * 60;
//     int minutes = int(fMin);
//     return DMS(degrees, minutes, seconds);
// }

float DD::toFloat() {
    return deg;
}

std::string DD::repr() {
    return std::to_string(deg);
}

