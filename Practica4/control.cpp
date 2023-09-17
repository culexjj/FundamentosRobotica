#include "control.h"
#include <math.h>


Control::Control(ArRobot *robot) : robot(robot)
{
    init();
}


// IMPORTANTE
// We must "lock" the ArRobot object
// before calling its methods, and "unlock" when done, to prevent conflicts
// with the background thread started by the call to robot.runAsync() above.
// See the section on threading in the manual for more about this.
// Make sure you unlock before any sleep() call or any other code that will
// take some time; if the robot remains locked during that time, then
// ArRobot's background thread will be blocked and unable to communicate with
// the robot, call tasks, etc.

void Control::init()
{

    robot->lock();
    // Activar motores
    robot->enableMotors();

    // Establecer laser y sonar,
    // si no está alguno será = 0
    laser = robot->findLaser(1); //indica el numero del laser
    sonar = (ArSonarDevice*)robot->findRangeDevice("sonar"); //usamos casting (ArSonarDevice*)

    // MobileSim no devuelve correctamente este valor si no se establece antes
    robot->setTransVelMax(600);
    robot->unlock();

    // TODO Inicialización de parámetros del control
    // Si se accede a los métodos del robot o del laser habrá que hacer
    // el lock oportuno

    //Inicializar variables
    valorMax = 10.0; //lectura maxima del laser (limitada por el tamaño del plano)
    valorMin = 0.0; //lectura minima del laser
    motorIzq = 0.0; //salida motor izquierdo
    motorDcho = 0.0; //salida motor derecho

    for (int i=0; i<=5;i++) {
        laserArray[i] = 0.0; //Array de lecturas laser
    }
}


void Control::execute()
{
    // Bucle de control tipo reactivo
    while(true) {
        input();
        proccess();
        output();
        ArUtil::sleep(100); //misma frecuencia  con el bucle del robot
    }
}

void Control::input()
{

    if(laser) {
        laser->lockDevice();
        // TODO lectura del laser, hay que hacerle lock
        laserArray[0] = laser->currentReadingPolar(54,90); //lectura sector 1
        laserArray[1] = laser->currentReadingPolar(18,53); //lectura sector 2
        laserArray[2] = laser->currentReadingPolar(-18,17); //lectura sector 3
        laserArray[3] = laser->currentReadingPolar(-54,-19); //lectura sector 4
        laserArray[4] = laser->currentReadingPolar(-90,-55); //lectura sector 5
        laser->unlockDevice();
    }

    if(sonar) {
        // TODO Lectura del sonar, no hay lock
        sonar->getCurrentBufferAsVector();
    }

    // TODO Normalizar entradas de 0 a 1
    double temp = 0.0;
    for (int i=0; i<=5;i++) {
        temp =(1.0 - ((laserArray[i]/1000.0) / valorMax));
        laserArray[i] = temp;
    }
}

void Control::proccess()
{
    // TODO Calcular salidas a partir de entradas y otros posibles parámetros (pesos, reglas, etc)
    // TODO Normalizar salidas entre -1 y 1

    //matriz de pesos
    double matriz [5][2] = {{0.019,0.2128},{-0.2165,0.13145},{-0.575,-0.5625},{0.1252,-0.21025},{0.2128,0.019}};

    double velMax = robot->getTransVelMax(); //Velocidad max

    //Calcular salida motor derecho, normarlizar y calcular  la velocidad de la rueda
    motorDcho = ((laserArray[0]*matriz[0][0]) + (laserArray[1]*matriz[1][0]) + (laserArray[2]*matriz[2][0]) + (laserArray[3]*matriz[3][0]) + (laserArray[4]*matriz[4][0])) + 0.5;
    motorDcho = tanh (motorDcho);
    velDcho = motorDcho * velMax;

    //Calcular salida motor izquierdo, normarlizar y calcular  la velocidad de la rueda
    motorIzq = ((laserArray[0]*matriz[0][1]) + (laserArray[1]*matriz[1][1]) + (laserArray[2]*matriz[2][1]) + (laserArray[3]*matriz[3][1]) + (laserArray[4]*matriz[4][1])) + 0.5;
    motorIzq = tanh (motorIzq);
    velIzq = motorIzq * velMax;

    //Debugging
    printf("\r Lectura : %3.2f %3.2f %3.2f %3.2f %3.2f VelI:%3.2f VelD:%3.2f", laserArray[0], laserArray[1], laserArray[2],laserArray[3], laserArray[4], velIzq, velDcho);


}

void Control::output()
{
    // TODO Pasar salidas a velocidades de ruedas o velocidades de traslación y rotación
   robot->lock();
   robot->setVel2(velIzq, velDcho);
   robot->unlock();
}


