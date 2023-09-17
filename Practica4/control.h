#ifndef CONTROL_H
#define CONTROL_H

#include <Aria.h>

class Control
{
public:
    Control(ArRobot* robot);
    void execute();

protected:
    virtual void init();
    virtual void input();
    virtual void proccess();
    virtual void output();

protected:
    ArRobot* robot;
    ArLaser* laser;
    ArSonarDevice* sonar;

    double valorMax; //lectura maxima del laser
    double valorMin; //lectura maxima del laser
    double motorIzq; //Motor izquierdo
    double motorDcho; //Motor derecho
    double velIzq; //velocidad rueda izquierda
    double velDcho; //velocidad rueda derecha
    double laserArray [5]; //array lectura laser


};

#endif // CONTROL_H
