/******************************/
/* Practica 2a - Cuadrado     */
/******************************/


#include "Aria.h"
int main(int argc, char** argv)
{
  Aria::init(); // Initialize some global data
  ArArgumentParser parser(&argc, argv); // This object parses program options from the command lin
  parser.loadDefaultArguments(); // This object parses program options from the command lin

  // Central object that is an interface to the robot and its integrated
  // devices, and which manages control of the robot by the rest of the program.
  ArRobot robot;

  // Object that connects to the robot or simulator using program options
  ArRobotConnector robotConnector(&parser, &robot);
  // Try connecting to the robot.
  if(!robotConnector.connectRobot(&robot))
  {
    // Error!
    ArLog::log(ArLog::Terse, "Error, could not connect to robot.\n");
    robotConnector.logOptions();
    Aria::exit(1);
  }




 /* Configuración basica del robot */
  
  ArSonarDevice sonarDev; // Used to access and process sonar range data
  robot.addRangeDevice(&sonarDev); // Attach sonarDev to the robot so it gets data from it.
  
  robot.runAsync(true); // Run the ArRobot processing/task cycle thread.
    
  robot.lock();
  robot.enableMotors();
  robot.unlock();
  ArUtil::sleep(1000);
 
 
 /*
  Acción: cuadro 4 metros de lado
  el ángulo de giro determinado con setHeading es absoluto, no relativo a la posición actual
 */
 
  // Avanzar 4m
  ArLog::log(ArLog::Normal, "Avanzar 4m");
  robot.lock();
  robot.move(4000);
  robot.unlock();
  ArUtil::sleep(10000);

  // girar 90º
  ArLog::log(ArLog::Normal, "Girar 90º ...");
  robot.lock();
  robot.setHeading(-90);
  robot.unlock();
  ArUtil::sleep(5000);

  // Avanzar 4m
  ArLog::log(ArLog::Normal, "Avanzar 4m");
  robot.lock();
  robot.move(4000);
  robot.unlock();
  ArUtil::sleep(10000);

  // girar 90º
  ArLog::log(ArLog::Normal, "Girar 90º ...");
  robot.lock();
  robot.setHeading(-180);
  robot.unlock();
  ArUtil::sleep(5000);

  // Avanzar 4m
  ArLog::log(ArLog::Normal, "Avanzar 4m");
  robot.lock();
  robot.move(4000);
  robot.unlock();
  ArUtil::sleep(10000);

  // girar 90º
  ArLog::log(ArLog::Normal, "Girar 90º ...");
  robot.lock();
  robot.setHeading(-270);
  robot.unlock();
  ArUtil::sleep(5000);

  // Avanzar 4m
  ArLog::log(ArLog::Normal, "Avanzar 4m");
  robot.lock();
  robot.move(4000);
  robot.unlock();
  ArUtil::sleep(10000);


  // fin acción
  ArLog::log(ArLog::Normal, "ACCION: Salir.");
  robot.disableMotors();
  Aria::exit(0);
  return 0;

}
