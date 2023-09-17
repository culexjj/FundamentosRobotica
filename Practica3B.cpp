
/********************************/
/* Practica 3B - Seguir paredes */
/********************************/

#include "Aria.h"


/* Declarion metodos */
int SentidoGiro();



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

  // Conexion laser Y sonar
  robot.addRangeDevice(&laser);

  robot.runAsync(true); //Inicio hilo de ejecucion
  ArUtil::sleep(500); // Espera para permitir inicializacion de todos los dispositivos
  
  /* Definicion variables flujo de control del comportamiento del robot */
  
  bool control = true;
  bool paredDCHA = false;
  bool paredIZQ = false;
  double rango = 0; //distancia al obstaculo frontal
  double rangoIZQ = 0; //distancia al obstaculo izq.
  double rangoDCHA = 0; //distancia al obstaculo dcho
  int giro = 0; //angulo de giro
  int umbral = /*a completar*/; //umbral error permitido 

  // Activar efectores de movimiento
  robot.lock();
  robot.enableMotors();
  robot.unlock();

  // Selección sentido de giro (solo el primero giro)
  int aux = SentidoGiro(); 



  // Flujo de control del comportamiento del robot */  
  
  while (control)
 {
	rango = laser.currentReadingPolar(/*a completar*/); //lectura rango laser Frontal
	rangoIZQ = laser.currentReadingPolar(/*a completar*/); //lectura rango laser IZQ
	rangoDCHA = laser.currentReadingPolar(/*a completar*/); //lectura rango laser DCHA


	if (rango > 1000)//si rango superior a 1m iniciamos movimiento
	{
		robot.lock();
  		robot.setVel(/*a completar*/);
  		robot.unlock();
		
		if (paredDCHA) //Pared detectada en el lado dcho del robot
		{
			/* Control desviacion segun umbral permitido */
			if (rangoDCHA  > /*a completar*/ + umbral)
			{				
				giro = giro - 5;//ajuste

				robot.lock();
				robot.setHeading(giro);
				robot.unlock();
				
				robot.lock();
  				robot.setVel(100);
  				robot.unlock();
				ArUtil::sleep(1000);
			}
   			if (rangoDCHA < /*a completar*/ - umbral)
			{
				giro = giro + 5;//ajuste

				robot.lock();
				robot.setHeading(giro);
				robot.unlock();
				
				robot.lock();
  				robot.setVel(100);
  				robot.unlock();
				ArUtil::sleep(1000);
			}
		}

		if (paredIZQ) //Pared detectada en el lado dcho del robot
		{
			/* Control desviacion segun umbral permitido */	
			if (rangoIZQ  < /*a completar*/ - umbral)
			{				
				giro = giro - 5;//ajuste

				robot.lock();
				robot.setHeading(giro);
				robot.unlock();

				robot.lock();
  				robot.setVel(100);
  				robot.unlock();
				ArUtil::sleep(1000);
			}
   			if (rangoIZQ > /*a completar*/ + umbral)
			{
				giro = giro + 5;//ajuste

				robot.lock();
				robot.setHeading(giro);
				robot.unlock();

				robot.lock();
  				robot.setVel(100);
  				robot.unlock();
				ArUtil::sleep(1000);
			}
			
		}

	} else//si rango inferior a 1m iniciamos maniobra evasiva
	{
	  	printf ("\n");
	  	printf("\rDistancia al obstaculo inferior a 1 metro: %10.4f ", rango/1000.0);
	  	printf("\nInicio maniobra evasiva: ");
		
		// Detenemos el robot
	  	robot.lock();
	  	robot.stop();
	  	robot.unlock();

	  while (rango <= /*a completar*/)//mientras el rango es inferior a 1m vamos girando 90º en le mismo sentido
	  {
	  	if (aux == 1)//sentido de giro +
	  	{
			giro = giro + 90;// girar +90º
	 		ArLog::log(ArLog::Normal, "\nGirar +90º ...");
			robot.lock();
			robot.setHeading(giro);
			robot.unlock();
			ArUtil::sleep(4000);
			paredIZQ = false;			
			paredDCHA = true;//Pared detectada en el lado dcho del robot
	  	} 

	 	 if (aux == 2)//sentido de giro -
	  	{
	  		giro = giro - 90;// girar -90º
	 		ArLog::log(ArLog::Normal, "\nGirar -90º ...");
			robot.lock();
			robot.setHeading(giro);
			robot.unlock();
			ArUtil::sleep(4000);
			paredIZQ = true;//Pared detectada en el lado izq del robot
			paredDCHA = false;
	  	}

	  	rango = laser.currentReadingPolar(/*a completar*/);//lectura rango laser
		
	  }

	ArLog::log(ArLog::Normal, "Fin maniobra evasiva: ");//si rango mayor a 1m termina la maniobra evasiva 
	
	}
  }


  // salir
  Aria::exit(0);
  return 0;

}



/* Definicion de metodos */
  int SentidoGiro(){// Selección sentido de giro
   
  srand((unsigned) time(NULL));
  int valor = (rand() %2)+1;
  
  return valor;
}


