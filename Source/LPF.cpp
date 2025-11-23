/*
  ==============================================================================

    LPF.cpp
    Created: 22 Nov 2025 11:58:01pm
    Author:  dkuzn

  ==============================================================================
*/

#include "LPF.h"
#include <cmath>

#ifndef M_PI
namespace
{
    const double M_PI = std::acos(-1.0);
}
#endif


LPF::LPF() : b0(0), b1(0), b2(0), a0(0), a1(0), a2(0), x1(0), x2(0), y1(0), y2(0), fs(44100.0f)
{

}

void LPF::init(float fs)
{
    this->fs = fs;
}

void LPF::setFreq(float fc)
{
    this->fc = fc;

    calcCoeffs();
}

void LPF::setQ(float Q)
{
    this->Q = Q;

    calcCoeffs();
}

void LPF::calcCoeffs()
{
    float K = tan(M_PI * fc / fs);

    b0 = K * K;
    b1 = 2 * K * K;
    b2 = K * K;

    a0 = 1 + (K / Q) + (K * K);
    a1 = (2 * K * K) - 2;
    a2 = 1 - (K / Q) + (K * K);

    b0 /= a0;
    b1 /= a0;
    b2 /= a0;
    a1 /= a0;
    a2 /= a0;
}

float LPF::process(float input)
{
    float y = b0 * input + b1 * x1 + b2 * x2 - a1 * y1 - a2 * y2;

    x2 = x1;
    x1 = input;
    y2 = y1;
    y1 = y;

    return y;
}
