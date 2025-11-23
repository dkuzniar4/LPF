/*
  ==============================================================================

    LPF.h
    Created: 22 Nov 2025 11:58:01pm
    Author:  dkuzn

  ==============================================================================
*/

#pragma once

class LPF
{
public:
    LPF();

    void init(float fs);
    void setFreq(float fc);
    void setQ(float Q);
    float process(float input);

private:
    void calcCoeffs();

    float fs;
    float fc;
    float Q;

    float b0;
    float b1;
    float b2;
    float a0;
    float a1;
    float a2;

    float x1;
    float x2;
    float y1;
    float y2;
};
