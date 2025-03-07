/*
  ==============================================================================

    FMSignal.cpp
    Created: 7 Mar 2025 9:44:09am
    Author:  Eli Faulkner

  ==============================================================================
*/

#include "FMSignal.h"

void FMSignal::setFunction(FMSignalFunction function) {
    _function = function;
}

float FMSignal::eval(float x) {
    switch(_function) {
        case FMSignalFunction::sin:
            return std::sin(x);
        case FMSignalFunction::noise:
            return _random.nextDouble()*2-1;
        case FMSignalFunction::square:
            return x < 0 ? -1 : 1;
        default:
            return 0;
    }
}
