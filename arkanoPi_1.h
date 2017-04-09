#ifndef _ARKANOPI_H_
#define _ARKANOPI_H_
//Test Github
#include <time.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <wiringPi.h>
//#include <wiringPi.h> // A descomentar en posteriores sesiones
#include "fsm.h"
#include "tmr.h"

#include "kbhit.h" // para poder detectar teclas pulsadas sin bloqueo y leer las teclas pulsadas

#include "arkanoPiLib.h"
#define CLK_MS 2
#define CLK_PELOTA 1500
#define DEBOUNCE_TIME 200
#define FLAG_PELOTA				0x01
#define FLAG_TECLA				0x02
#define FLAG_RAQUETA_IZQUIERDA	0x04
#define FLAG_RAQUETA_DERECHA	0x08
#define FLAG_FINAL_JUEGO		0x10
#define FLAG_PAUSE              0x20
#define FLAGS_KEY 1
#define STD_IO_BUFFER_KEY 2
//FLAGS DEL SISTEMA
/*
typedef enum {

	WAIT_START,
	WAIT_PUSH,
	WAIT_END} tipo_estados_juego;
*/

typedef struct {
	tipo_arkanoPi arkanoPi;
	tmr_t* temporizador;
	//tipo_estados_juego estado;

	char teclaPulsada;

} tipo_juego;

//------------------------------------------------------
// FUNCIONES DE ENTRADA
//------------------------------------------------------
int CompruebaTeclaPelota (fsm_t* this);
int CompruebaTeclaRaquetaDerecha (fsm_t* this);
int CompruebaTeclaRaquetaIzquierda (fsm_t* this);
int CompruebaTeclaPulsada (fsm_t* this);
int CompruebaTeclaFinalJuego(fsm_t* this);
void pelota_isr (union sigval sigval);
void boton_isr (void);
void refresco_isr(union sigval sigval);


//------------------------------------------------------
// FUNCIONES DE SALIDA
//------------------------------------------------------

void InicializaJuego (fsm_t*);
void MueveRaquetaIzquierda (fsm_t*);
void MueveRaquetaDerecha (fsm_t*);
void MovimientoPelota (fsm_t*);
void FinalJuego (fsm_t*);
void ReseteaJuego (fsm_t*);
//------------------------------------------------------
// FUNCIONES AUXILIARES
//------------------------------------------------------
//int hayRebote(void);
int posicionSiguienteX(void);
int posicionSiguienteY(void);
char getEstructura(void);

//------------------------------------------------------
// SUBRUTINAS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------
PI_THREAD (thread_explora_teclado);
//------------------------------------------------------
// FUNCIONES DE INICIALIZACION
//------------------------------------------------------
int systemSetup (void);

#endif /* ARKANOPI_H_ */
