/******************************/
/* Practica 2b - Circulo      */
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
  Acción: circulo 2 metros radio & 12,56 metros de perímetro
  Con la siguiente configuración, el robot realiza la acción  

  Forward speed: 0,36 metros por segundo
  Rotational speed: -10 grados por segundo
  Tiempo accion: 37 seconds
     
  Nota: No se tiene en cuenta el tiempo de aceleración/desaceleración  
  Por esta razón ajustamos el tiempo de funcionamiento a 37sg
  */
  
  ArLog::log(ArLog::Normal, "ACCION: realizar circulo 2m de radio ");
  robot.lock();
  robot.setRotVel(-10);
  robot.move(12560);
  robot.setVel(360);
  robot.unlock();
  ArUtil::sleep(37000);

  // fin acción
  ArLog::log(ArLog::Normal, "ACCION: Salir.");
  robot.disableMotors();
  Aria::exit(0);
  return 0;

}