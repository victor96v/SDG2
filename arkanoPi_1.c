
#include "arkanoPi_1.h"

//Pines
#define pinColumna0 14
#define pinColumna1 17
#define pinColumna2 18
#define pinColumna3 22
#define pinFila0 0
#define pinFila1 1
#define pinFila2 2
#define pinFila3 3
#define pinFila4 4
#define pinFila5 7
#define pinFila6 23
#define BOTON_DER 20
#define BOTON_IZQ 21
#define NFILA 7
#define NCOLUMNA 10

// Definimos las matrices de pines de filas y columnas que iran a la salida de la placa
int fila[7]={pinFila0, pinFila1, pinFila2,pinFila3, pinFila4, pinFila5, pinFila6};
int columna [4] = {pinColumna0, pinColumna1, pinColumna2, pinColumna3};
int botonDerecho = BOTON_DER;
int botonIzquierdo = BOTON_IZQ;

static tmr_t* timer_pelota;
static tmr_t* timer_refresco;
#define TIMEOUT 200
//Definimos los posibles estados del juego
 enum fsm_state{

	WAIT_START= 1,
	WAIT_PUSH = 0,
	WAIT_WAIT = 2,
	WAIT_END= 3};

static volatile tipo_juego juego;
// Definicion de los estados
//enum fsm_state {
//  WAIT_START=0,
//  WAIT_PUSH=1,
//  WAIT_END=2,
//  };

volatile int flags = 0;

// espera hasta la próxima activación del reloj
void delay_until (unsigned int next) {
	unsigned int now = millis();

	if (next > now) {
		delay (next - now);
    }
}

// FUNCIONES DE ENTRADA O COMPROBACIÓN DE LA MAQUINA DE ESTADOS
// Comprobamos que se haya pulsado una tecla
void pelota_isr (union sigval sigval) {
	piLock (FLAGS_KEY);
	flags |= FLAG_PELOTA;
	piUnlock (FLAGS_KEY);

	tmr_startms(timer_pelota,CLK_PELOTA);
}
void botonderecho_isr (void) {
//	int pau = flags & FLAG_RAQUETA_IZQUIERDA;
//		if(pau != 0){
//			flags |= FLAG_PAUSE;
//			return;
//		}

	int debounceTime =0;
		if (millis() < debounceTime){
			debounceTime = millis() + DEBOUNCE_TIME;
		}
		piLock (FLAGS_KEY);
			flags |= FLAG_RAQUETA_DERECHA;
			piUnlock (FLAGS_KEY);
			piLock (FLAGS_KEY);
							flags |= FLAG_TECLA;
							piUnlock (FLAGS_KEY);
	while (digitalRead (botonDerecho)== HIGH){
		delay(1);
	}
	debounceTime = millis() + DEBOUNCE_TIME;

}
void botonizquierdo_isr (void) {
	/*int pau = flags & FLAG_RAQUETA_DERECHA;
	if(pau != 0){
				flags |= FLAG_PAUSE;
				return;
			}
*/
		int debounceTime =0;
				if (millis() < debounceTime){
					debounceTime = millis() + DEBOUNCE_TIME;
				}
				piLock (FLAGS_KEY);
						flags |= FLAG_RAQUETA_IZQUIERDA;
						piUnlock (FLAGS_KEY);
						piLock (FLAGS_KEY);
										flags |= FLAG_TECLA;
										piUnlock (FLAGS_KEY);

		while (digitalRead (botonIzquierdo)== HIGH){
			delay(1);
		}
		debounceTime = millis() + DEBOUNCE_TIME;
	}

int CompruebaTeclaPulsada (fsm_t* this) {
	int result;

	piLock (FLAGS_KEY);
	result = (flags & FLAG_TECLA);
	piUnlock (FLAGS_KEY);

	return result;
}
// Comprobamos que esta usandose la pelota
int CompruebaTeclaPelota (fsm_t* this) {
	int result;

	piLock (FLAGS_KEY);
	result = (flags & FLAG_PELOTA);
	piUnlock (FLAGS_KEY);

	return result;
}
// Comprobamos que esta moviendose la raqueta a la derecha
int CompruebaTeclaRaquetaDerecha (fsm_t* this) {
	int result;

	piLock (FLAGS_KEY);
	result = (flags & FLAG_RAQUETA_DERECHA) && (!(flags & FLAG_RAQUETA_IZQUIERDA));
	piUnlock (FLAGS_KEY);

	return result;
}
int CompruebaPausa (fsm_t* this) {
	int result;
	piLock (FLAGS_KEY);
	result = (flags & FLAG_PAUSE);
	piUnlock (FLAGS_KEY);
	return result;
}
// Comprobamos que esta moviendose la raqueta a la izquierda
int CompruebaTeclaRaquetaIzquierda (fsm_t* this) {
	int result;

	piLock (FLAGS_KEY);

	result = (flags & FLAG_RAQUETA_IZQUIERDA) && (!(flags & FLAG_RAQUETA_DERECHA));

	piUnlock (FLAGS_KEY);

	return result;
}

// Comprobamos que el juego ha terminado
int CompruebaFinalJuego (fsm_t* this) {
	int result;

	piLock (FLAGS_KEY);
	result = (flags & FLAG_FINAL_JUEGO);
	piUnlock (FLAGS_KEY);

	return result;
}

//------------------------------------------------------
// FUNCIONES DE ACCION
//------------------------------------------------------

// void InicializaJuego (void): funcion encargada de llevar a cabo
// la oportuna inicialización de toda variable o estructura de datos
// que resulte necesaria para el desarrollo del juego.
void InicializaJuego (fsm_t* fsm) {
	InicializaArkanoPi((tipo_arkanoPi*)(&(juego.arkanoPi)));
	piLock (FLAGS_KEY);
	flags &= ~FLAG_TECLA;
	piUnlock (FLAGS_KEY);
	//juego.teclaPulsada='p';
	piLock(STD_IO_BUFFER_KEY);
	//ActualizaPantalla((tipo_arkanoPi*)(&(juego.arkanoPi)));
	//PintaPantallaPorTerminal((tipo_pantalla*)(&(juego.arkanoPi.pantalla)));
	piUnlock(STD_IO_BUFFER_KEY);
	timer_pelota = tmr_new (pelota_isr);
	tmr_startms(timer_pelota,CLK_PELOTA);
	delay(1000);
}

// void MueveRaquetaIzquierda (void): funcion encargada de ejecutar
// el movimiento hacia la izquierda contemplado para la raqueta.
// Debe garantizar la viabilidad del mismo mediante la comprobación
// de que la nueva posición correspondiente a la raqueta no suponga
// que ésta rebase o exceda los límites definidos para el área de juego
// (i.e. al menos uno de los leds que componen la raqueta debe permanecer
// visible durante todo el transcurso de la partida)

void MueveRaquetaIzquierda (fsm_t* fsm) {
		//Activamos el flag de la raqueta moviendose a la izquierda
	    piLock (FLAGS_KEY);
		flags &= ~FLAG_RAQUETA_IZQUIERDA;
		piUnlock (FLAGS_KEY);
		//Comprobamos que el limite izquierdo de la pared no se supera
		//Es decir, que al menos haya un led iluminado
		if(juego.arkanoPi.raqueta.x > -2)
				juego.arkanoPi.raqueta.x=(juego.arkanoPi.raqueta.x)-1;
		//Actualizamos la situacion de los bits de la pantalla
		ActualizaPantalla((tipo_arkanoPi*)(&(juego.arkanoPi)));
		piLock(STD_IO_BUFFER_KEY);
		//Visualizamos la pantalla
		//PintaPantallaPorTerminal((tipo_pantalla*)(&(juego.arkanoPi.pantalla)));
		piUnlock(STD_IO_BUFFER_KEY);

}

// void MueveRaquetaDerecha (void): función similar a la anterior
// encargada del movimiento hacia la derecha.
void MueveRaquetaDerecha (fsm_t* fsm) {
		//Activamos el flag de la raqueta moviendose a la izquierda
		piLock (FLAGS_KEY);
		flags &= ~FLAG_RAQUETA_DERECHA;
		piUnlock (FLAGS_KEY);
		//Comprobamos que el limite derecho de la pared no se supera
		//Es decir, que al menos haya un led iluminado
		if(juego.arkanoPi.raqueta.x < MATRIZ_ANCHO-1)
				juego.arkanoPi.raqueta.x=(juego.arkanoPi.raqueta.x)+1;
		piLock(STD_IO_BUFFER_KEY);
		//Actualizamos la situacion de los bits de la pantalla
		ActualizaPantalla((tipo_arkanoPi*)(&(juego.arkanoPi)));
		//Visualizamos la pantalla
		//PintaPantallaPorTerminal((tipo_pantalla*)(&(juego.arkanoPi.pantalla)));
		piUnlock(STD_IO_BUFFER_KEY);

}

void PausaJuego(fsm_t* fsm){
	tmr_stop(timer_pelota);
	piLock (FLAGS_KEY);
	flags &= ~FLAG_PAUSE;
	piUnlock (FLAGS_KEY);
	piLock (FLAGS_KEY);
	flags &= ~FLAG_RAQUETA_DERECHA;
	piUnlock (FLAGS_KEY);
	piLock (FLAGS_KEY);
	flags &= ~FLAG_RAQUETA_IZQUIERDA;
	piUnlock (FLAGS_KEY);
printf("pausa");
}
void ReanudaJuego(fsm_t* fsm){

	tmr_startms(timer_pelota,CLK_PELOTA);
	piLock (FLAGS_KEY);
			flags &= ~FLAG_RAQUETA_DERECHA;
			piUnlock (FLAGS_KEY);
			piLock (FLAGS_KEY);
				flags &= ~FLAG_RAQUETA_IZQUIERDA;
				piUnlock (FLAGS_KEY);
				printf("reanuda");
}


// void MovimientoPelota (void): función encargada de actualizar la
// posición de la pelota conforme a la trayectoria definida para ésta.
// Para ello deberá identificar los posibles rebotes de la pelota para,
// en ese caso, modificar su correspondiente trayectoria (los rebotes
// detectados contra alguno de los ladrillos implicarán adicionalmente
// la eliminación del ladrillo). Del mismo modo, deberá también
// identificar las situaciones en las que se dé por finalizada la partida:
// bien porque el jugador no consiga devolver la pelota, y por tanto ésta
// rebase el límite inferior del área de juego, bien porque se agoten
// los ladrillos visibles en el área de juego.


// este metodo me dice si mi pelota va a rebotar o no en la siguiente posicion

int posicionSiguienteX(void){
	int posicionX= juego.arkanoPi.pelota.xv + juego.arkanoPi.pelota.x;
	return posicionX;
}
int posicionSiguienteY(void){
	int posicionY= juego.arkanoPi.pelota.yv + juego.arkanoPi.pelota.y;
	return posicionY;
}

char getEstructura(void){
	char caso = '0';
	//ZonaDeGameOver
	if ( juego.arkanoPi.pelota.y == MATRIZ_ALTO - 1){
		caso = 'z' ;
	}
	//ZonaParedLateral
	else if (((juego.arkanoPi.pelota.x == 0) && (juego.arkanoPi.pelota.xv == -1)) || ((juego.arkanoPi.pelota.x == (MATRIZ_ANCHO - 1)) && (juego.arkanoPi.pelota.xv == 1))){
		caso = 'p';
		}
	//ZonaRaqueta
	else if (posicionSiguienteY() == MATRIZ_ALTO - 1) {
		//Posicion izquierda raqueta
		if(posicionSiguienteX() == juego.arkanoPi.raqueta.x){
					caso = 'i';
		}
		//Posicion centro raqueta
		else if(posicionSiguienteX() == juego.arkanoPi.raqueta.x+1){
					caso = 'c';
						}
		//Posicion derecha raqueta
		else if(posicionSiguienteX() == juego.arkanoPi.raqueta.x+2){
					caso = 'd';
						}
		/*else {
			caso = "FinPartida";
		}*/
	}
	//ZonaParedDerecha
	/*if (juego.arkanoPi.pelota.x == (MATRIZ_ANCHO - 1) && (juego.arkanoPi.pelota.xv)){
		caso = "ParedDerecha";
	}*/
	//ZonaParedSuperior
	else if ((juego.arkanoPi.pelota.y == 0) && (juego.arkanoPi.pelota.yv == -1)){
			caso = 's';
	}
	//ZonaLadrillos
	else if ( juego.arkanoPi.pelota.y <=2 && juego.arkanoPi.pelota.y >= 0) {
		if	(juego.arkanoPi.ladrillos.matriz[posicionSiguienteX()][posicionSiguienteY()] == 1){
			caso = 'l';
		}
	}
	return caso;
}
void MovimientoPelota (fsm_t* fsm) {
	//if (((juego.arkanoPi.pelota.xv)+ || juego.arkanoPi.pelota.yv) == 1)
	// A completar por el alumno...
	piLock (FLAGS_KEY);
	flags &= ~FLAG_PELOTA;
	piUnlock (FLAGS_KEY);


	piLock(STD_IO_BUFFER_KEY);
	char caso = getEstructura();
	switch(caso){
	//No hay rebote
	case '0':
		break;
	//Rebote con Pared Superior
	case 's':
		juego.arkanoPi.pelota.yv = -juego.arkanoPi.pelota.yv;
		caso = getEstructura();
		if (caso == 'l'){
			// Eliminamos ladrillo
			juego.arkanoPi.ladrillos.matriz[posicionSiguienteX()][posicionSiguienteY()] = 0;
			    // Cambio de trayectoria
					juego.arkanoPi.pelota.xv = -juego.arkanoPi.pelota.xv;
					break;
		}
		break;
	//Rebote con pared lateral
	case 'p':
		juego.arkanoPi.pelota.xv = -juego.arkanoPi.pelota.xv;
		caso = getEstructura();
		switch(caso) {
		case 'l':
			// Eliminamos ladrillo
			juego.arkanoPi.ladrillos.matriz[posicionSiguienteX()][posicionSiguienteY()] = 0;
			// Cambio de trayectoria
			juego.arkanoPi.pelota.yv = -juego.arkanoPi.pelota.yv;
			break;
		//ParedSuperior
		case 's':
			juego.arkanoPi.pelota.yv = -juego.arkanoPi.pelota.yv;
			break;
		//ZonaRaquetaIzquierda
		case 'i':
			juego.arkanoPi.pelota.xv = -1;
			juego.arkanoPi.pelota.yv = -1;
			break;
		//ZonaRaquetaCentral
		case 'c':
			juego.arkanoPi.pelota.xv = 0;
			juego.arkanoPi.pelota.yv = -1;
			break;
		//ZonaRaquetaDerecha
		case 'd':
			juego.arkanoPi.pelota.xv = 1;
			juego.arkanoPi.pelota.yv = -1;

		}
		break;
	//Rebote con ladrillos
	case 'l':
		// Eliminamos ladrillo
		juego.arkanoPi.ladrillos.matriz[posicionSiguienteX()][posicionSiguienteY()] = 0;
		// Cambio de trayectoria
		juego.arkanoPi.pelota.yv = -juego.arkanoPi.pelota.yv;
		break;


	//Rebote con raqueta: parte izquierda
	case 'i':
		juego.arkanoPi.pelota.xv = -1;
		juego.arkanoPi.pelota.yv = -1;
		caso = getEstructura();
		if ( caso == 'p') {
						juego.arkanoPi.pelota.xv = - juego.arkanoPi.pelota.xv;
					}
		break;
	//Rebote con raqueta: parte central
	case 'c':
			juego.arkanoPi.pelota.xv = 0;
			juego.arkanoPi.pelota.yv = -1;
			caso = getEstructura();
				if ( caso == 'p') {
				juego.arkanoPi.pelota.xv = - juego.arkanoPi.pelota.xv;
				}
			break;
	//Rebote con raqueta: parte derecha
	case 'd':
			juego.arkanoPi.pelota.xv = 1;
			juego.arkanoPi.pelota.yv = -1;
			caso = getEstructura();
			if ( caso == 'p') {
				juego.arkanoPi.pelota.xv = - juego.arkanoPi.pelota.xv;
			}
			break;

	//GameOver
	case 'z':
		piLock (FLAGS_KEY);
		flags |= FLAG_FINAL_JUEGO;

		piUnlock (FLAGS_KEY);
		//FinalJuego(fsm);
		break;
	default:
		break;
	}
	// Actualizamos la posicion de la pelota
	juego.arkanoPi.pelota.x = posicionSiguienteX();
	juego.arkanoPi.pelota.y = posicionSiguienteY();
	// Actualizamos los bits de la matriz
	ActualizaPantalla((tipo_arkanoPi*)(&(juego.arkanoPi)));
	// Visualizamos los bits en la pantalla de leds
	//
	//PintaPantallaPorTerminal((tipo_pantalla*)(&(juego.arkanoPi.pantalla)));
	piUnlock(STD_IO_BUFFER_KEY);


}

// void FinalJuego (void): función encargada de mostrar en la ventana de
// terminal los mensajes necesarios para informar acerca del resultado del juego.
void FinalJuego (fsm_t* fsm) {
	
		piLock (FLAGS_KEY);
		flags &= ~FLAG_FINAL_JUEGO;
		flags &= ~FLAG_TECLA;
		piUnlock (FLAGS_KEY);
		piLock(STD_IO_BUFFER_KEY);
		printf (" \n Game over \n");
		FinalizaArkanoPi((tipo_arkanoPi*)(&(juego.arkanoPi)));
		piUnlock(STD_IO_BUFFER_KEY);
		tmr_stop(timer_pelota);


}

//void ReseteaJuego (void): función encargada de llevar a cabo la
// reinicialización de cuantas variables o estructuras resulten
// necesarias para dar comienzo a una nueva partida.
void ReseteaJuego (fsm_t* fsm) {

		piLock (FLAGS_KEY);
		flags &= ~FLAG_TECLA;
	    piUnlock (FLAGS_KEY);
	    piLock (STD_IO_BUFFER_KEY);
// Pintamos el inicio del juego
		printf("\n");
		printf("***************************\n");
		printf("**********ARKANOPI*********\n");
		printf("***************************\n");
//Pintamos la pantalla
		InicializaArkanoPi((tipo_arkanoPi*)(&(juego.arkanoPi)));
		printf("\nPULSE PARA COMENZAR EL JUEGO...\n");
		printf("i-Mueve la  raqueta a la  izquierda\n");
		printf("o-Mueve la  raqueta a la  derecha\n");
		printf("p-Mueve la pelota\n");

		piUnlock (STD_IO_BUFFER_KEY);

}

//------------------------------------------------------
// FUNCIONES DE TEMPORIZACION
//------------------------------------------------------
void refresco_isr (union sigval sigval){
	static int pcol = 0;
	digitalWrite(columna[0], (pcol & (1 << 0))? HIGH : LOW);
	digitalWrite(columna[1], (pcol & (1 << 1))? HIGH : LOW);
	digitalWrite(columna[2], (pcol & (1 << 2))? HIGH : LOW);
	digitalWrite(columna[3], (pcol & (1 << 3))? HIGH : LOW);
	int i;

	for (i=0; i<NFILA;i++){
		digitalWrite(fila[i], (juego.arkanoPi.pantalla.matriz[pcol][i]+1)%2);
	}
	pcol=(pcol+1)%NCOLUMNA;
	tmr_startms(timer_refresco,CLK_MS);
}


//------------------------------------------------------
// FUNCIONES DE INICIALIZACION
//------------------------------------------------------

// int systemSetup (void): procedimiento de configuracion del sistema.
// Realizará, entra otras, todas las operaciones necesarias para:
// configurar el uso de posibles librerías (e.g. Wiring Pi),
// configurar las interrupciones externas asociadas a los pines GPIO,
// configurar las interrupciones periódicas y sus correspondientes temporizadores,
// crear, si fuese necesario, los threads adicionales que pueda requerir el sistema

int systemSetup (void) {
	int x = 0;

		piLock (STD_IO_BUFFER_KEY);

		// sets up the wiringPi library
		if (wiringPiSetupGpio () < 0) {
			printf ("Unable to setup wiringPi\n");
			piUnlock (STD_IO_BUFFER_KEY);
			return -1;
	    }
		// Lanzamos thread para exploracion del teclado convencional del PC
	//	x = piThreadCreate (thread_explora_teclado);

		if (x != 0) {
			printf ("it didn't start!!!\n");
			piUnlock (STD_IO_BUFFER_KEY);
			return -1;
	    }
		wiringPiSetupGpio(); // con pins de BCM 2835
		pinMode (20, INPUT) ; // declaro pin 0 como salida
		pinMode (21, INPUT) ;
		wiringPiISR (20, INT_EDGE_FALLING,  botonderecho_isr) ;
		wiringPiISR (21, INT_EDGE_FALLING,  botonizquierdo_isr) ;
		int i;
		for (i=0; i<NFILA; i++){
			pinMode (fila[i], OUTPUT);
			}
		piUnlock (STD_IO_BUFFER_KEY);

		return 1;

}

void fsm_setup (fsm_t* arkano_fsm) {
	piLock (FLAGS_KEY);
	flags = 0;
	piUnlock (FLAGS_KEY);
	ReseteaJuego(arkano_fsm);
	//InicializaArkanoPi((tipo_arkanoPi*)(&(juego.arkanoPi)));
	piLock(STD_IO_BUFFER_KEY);
	printf("\n Cargando... \n");
	piUnlock(STD_IO_BUFFER_KEY);
}
/*PI_THREAD (thread_explora_teclado) {

int teclaPulsada;

	while(1) {
		//fflush (stdout);
		delay(10); // Wiring Pi function: pauses program execution for at least 10 ms

		piLock (STD_IO_BUFFER_KEY);
		if(kbhit()) {
			teclaPulsada = kbread();
			//printf("Tecla %c\n", teclaPulsada);

			switch(teclaPulsada) {
				case 'i':
					piLock (FLAGS_KEY);
					flags |= FLAG_RAQUETA_IZQUIERDA;
					piUnlock (FLAGS_KEY);
					piLock (FLAGS_KEY);
					flags |= FLAG_TECLA;
					piUnlock (FLAGS_KEY);
					break;

				case 'o':
					piLock (FLAGS_KEY);
					flags |= FLAG_RAQUETA_DERECHA;
					piUnlock (FLAGS_KEY);
					piLock (FLAGS_KEY);
					flags |= FLAG_TECLA;
					piUnlock (FLAGS_KEY);

					break;
				case 'p':
					piLock (FLAGS_KEY);
					flags |= FLAG_PELOTA;
					piUnlock (FLAGS_KEY);
					piLock (FLAGS_KEY);
					flags |= FLAG_TECLA;
					piUnlock (FLAGS_KEY);

					break;
				case 'q':
					exit(0);
					break;
				case 't':


				default:
				printf("INVALID KEY!!!\n");
				fflush(stdout);
					break;
			}
		}

		piUnlock (STD_IO_BUFFER_KEY);
	}
}*/

int main ()
{
	unsigned int next;

		//Creamos el temporizador

			timer_pelota = tmr_new (pelota_isr);
			timer_refresco = tmr_new (refresco_isr);

		// Maquina de estados: lista de transiciones
		// {EstadoOrigen,FunciónDeEntrada,EstadoDestino,FunciónDeSalida}
		fsm_trans_t arkano_tabla[] = {
				{ WAIT_START,CompruebaTeclaPulsada, WAIT_PUSH, InicializaJuego },
				{ WAIT_PUSH,CompruebaTeclaPelota,  WAIT_PUSH, MovimientoPelota },
				{ WAIT_PUSH,CompruebaTeclaRaquetaDerecha,  WAIT_PUSH, MueveRaquetaDerecha },
				{ WAIT_PUSH,CompruebaTeclaRaquetaIzquierda,  WAIT_PUSH, MueveRaquetaIzquierda },
				{ WAIT_PUSH,CompruebaFinalJuego,  WAIT_END, FinalJuego },
				{ WAIT_PUSH,CompruebaPausa,  WAIT_WAIT, PausaJuego },
				{ WAIT_WAIT,CompruebaPausa,  WAIT_PUSH, ReanudaJuego },
				{ WAIT_END,CompruebaTeclaPulsada,  WAIT_START, ReseteaJuego },
				{ -1, NULL, -1, NULL },
		};

		fsm_t* arkano_fsm = fsm_new (WAIT_START, arkano_tabla, NULL);
		// Configuracion e inicializacion del sistema
		systemSetup();
		fsm_setup (arkano_fsm);
		tmr_startms(timer_refresco, CLK_MS); //no son 100		next = millis();
		while (1) {
			fflush(stdout);
			fsm_fire (arkano_fsm);
			next += CLK_MS;
			delay_until (next);
		}
		fsm_destroy (arkano_fsm);
}
























