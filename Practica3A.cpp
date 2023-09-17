
/**********************************/
/* Practica 3a - Evitar obstaculos*/
/**********************************/

#include "Aria.h"

int main(int argc, char** argv)
{

  /* Configuracion inicial */
  Aria::init();
  ArArgumentParser parser(&argc, argv); 
  parser.loadDefaultArguments();

  ArRobot robot; //Objeto robot
  ArSick laser; //Objeto laser

  // Robot conector
  ArRobotConnector robotConnector(&parser, &robot);
  if(!robotConnector.connectRobot(&robot))
  {
    ArLog::log(ArLog::Terse, "Error, no es posible conectar con el robot.\n"); // Error!
    robotConnector.logOptions();
    Aria::exit(1);
  }

  // Laser conector
  ArLaserConnector laserConnector(&parser, &robot, &robotConnector);
  laser.runAsync();
  if (!laserConnector.connectLaser(&laser))
  {
  ArLog::log(ArLog::Terse, "Error, no es posible conectar con el laser.\n"); // Error!
  exit(1);
  }

  // Conexion laser
  robot.addRangeDevice(&laser);

  robot.runAsync(true); //Inicio hilo de ejecucion
  ArUtil::sleep(500); // Espera para permitir inicializacion de todos los dispositivos
  
  /* Definicion variables flujo de control del comportamiento del robot */
  
  bool control = true;
  double rango = 0; //distancia al obstaculo
  int giro = 0; //angulo de giro

  // Activar efectores de movimiento
  robot.lock();
  robot.enableMotors();
  robot.unlock();

  // Flujo de control del comportamiento del robot */  

  while (control)
 {
	rango = laser.currentReadingPolar(-45,45); //lectura rango laser

	if (rango > 1000)//si rango superior a 1m iniciamos movimiento
	{
		robot.lock();
  		robot.setVel(200);
  		robot.unlock();

	} else//si rango inferior a 1m iniciamos maniobra evasiva
	{
	  	printf ("\n");
	  	printf("\rDistancia al obstaculo inferior a 1 metro: %10.4f ", rango/1000.0);
	  	printf("\nInicio maniobra evasiva: ");
		
		// Detenemos el robot
	  	robot.lock();
	  	robot.stop();
	  	robot.unlock();

	  	// Selección sentido de giro
	  	srand((unsigned) time(NULL));
		int random = (rand() %2)+1;


	  while (rango <= 1000)//mientras el rango es inferior a 1m vamos girando 90º en le mismo sentido
	  {
	  	if (random == 1)//sentido de giro +
	  	{
			giro = giro + 90;
	 		ArLog::log(ArLog::Normal, "\nGirar +90º ...");
			robot.lock();
			robot.setHeading(giro);
			robot.unlock();
			ArUtil::sleep(4000);		
	  	} 

	 	 if (random == 2)//sentido de giro -
	  	{
	  		giro = giro - 90;
	 		ArLog::log(ArLog::Normal, "\nGirar -90º ...");
			robot.lock();
			robot.setHeading(giro);
			robot.unlock();
			ArUtil::sleep(4000);
	  	}

	  	rango = laser.currentReadingPolar(-45,45);//lectura rango laser
	  }

	ArLog::log(ArLog::Normal, "Fin maniobra evasiva: ");//si rango mayor a 1m termina la maniobra evasiva y 
		
	}
  }
 
  // Fin de ejecucion
  Aria::exit(0);
  return 0;
}

