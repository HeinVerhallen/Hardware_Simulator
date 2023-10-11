#ifndef CELLEMULATOR_H
#define CELLEMULATOR_H

#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class CellEmulator
{
public:
    CellEmulator();
    ~CellEmulator();
    
    double getVoltage();
    void setVoltage(double voltage);

    double getCurrent();
    void setCurrent(double current);

private:
    double voltage = 2.5;
    double current = 0.1;
};

#endif