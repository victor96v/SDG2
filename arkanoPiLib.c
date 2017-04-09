#include "arkanoPiLib.h"
#include <stdlib.h>
#include <time.h>

int ladrillos_basico[MATRIZ_ANCHO][MATRIZ_ALTO] = {
		{1,1,0,0,0,0,0},
		{1,1,0,0,0,0,0},
		{1,1,0,0,0,0,0},
		{1,1,0,0,0,0,0},
		{1,1,0,0,0,0,0},
		{1,1,0,0,0,0,0},
		{1,1,0,0,0,0,0},
		{1,1,0,0,0,0,0},
		{1,1,0,0,0,0,0},
		{1,1,0,0,0,0,0},

};
/*int pantalla_crema[MATRIZ_ANCHO][MATRIZ_ALTO] = {
				{0,1,1,1,0,0,0},
				{0,0,0,0,1,0,0},
				{0,0,0,0,0,1,0},
				{0,0,0,0,1,0,0},
				{0,1,1,1,0,0,0},
				{0,1,1,1,1,1,0},
				{0,1,0,0,0,1,0},
				{0,1,0,1,0,1,0},
				{0,1,0,1,0,1,0},
				{0,1,0,1,1,1,0},


};*/
tipo_pantalla pantalla={.matriz={
						{0,1,1,1,0,0,0},
						{0,0,0,0,1,0,0},
						{0,0,0,0,0,1,0},
						{0,0,0,0,1,0,0},
						{0,1,1,1,0,0,0},
						{0,1,1,1,1,1,0},
						{0,1,0,0,0,1,0},
						{0,1,0,1,0,1,0},
						{0,1,0,1,0,1,0},
						{0,1,0,1,1,1,0},}

};
tipo_pantalla pantallaFinal = {.matriz ={
						{0,0,0,0,0,0,0},
						{0,1,1,0,0,0,0},
						{0,1,1,0,0,1,0},
						{0,0,0,0,1,0,0},
						{0,0,0,1,0,0,0},
						{0,0,0,1,0,0,0},
						{0,0,0,0,1,0,0},
						{0,1,1,0,0,1,0},
						{0,1,1,0,0,0,0},
						{0,0,0,0,0,0,0},}
};


//------------------------------------------------------
// FUNCIONES DE INICIALIZACION / RESET
//------------------------------------------------------

void ReseteaMatriz(tipo_pantalla *p_pantalla) {
	int i, j = 0;

	for(i=0;i<MATRIZ_ANCHO;i++) {
		for(j=0;j<MATRIZ_ALTO;j++) {
			p_pantalla->matriz[i][j] = 0;
		}
	}
}

void ReseteaLadrillos(tipo_pantalla *p_ladrillos) {
	int i, j = 0;

	for(i=0;i<MATRIZ_ANCHO;i++) {
		for(j=0;j<MATRIZ_ALTO;j++) {
			p_ladrillos->matriz[i][j] = ladrillos_basico[i][j];
		}
	}
}

void ReseteaPelota(tipo_pelota *p_pelota) {
	// Pelota inicialmente en el centro de la pantalla
	p_pelota->x = MATRIZ_ANCHO/2 - 1;
	p_pelota->y = MATRIZ_ALTO/2 - 1;

	// Trayectoria inicial
	p_pelota->yv = 1;
	srand(time(NULL));
	p_pelota->xv = rand() % 3 - 1;// La trayectoria inicial de la pelota es aleatoria
	printf( "%d" ,p_pelota->xv);
}

void ReseteaRaqueta(tipo_raqueta *p_raqueta) {
	// Raqueta inicialmente en el centro de la pantalla
	p_raqueta->x = MATRIZ_ANCHO/2 - p_raqueta->ancho/2;
	p_raqueta->y = MATRIZ_ALTO - 1;
	p_raqueta->ancho = RAQUETA_ANCHO;
	p_raqueta->alto = RAQUETA_ALTO;
}

//------------------------------------------------------
// FUNCIONES DE VISUALIZACION (ACTUALIZACION DEL OBJETO PANTALLA QUE LUEGO USARA EL DISPLAY)
//------------------------------------------------------

// void PintaMensajeInicialPantalla (...): metodo encargado de aprovechar
// el display para presentar un mensaje de bienvenida al usuario

/*void RellenaPantallaInicial(tipo_pantalla *p_pantalla_inicial){
	int i, j = 0;

		for(i=0;i<MATRIZ_ANCHO;i++) {
			for(j=0;j<MATRIZ_ALTO;j++) {
				p_pantalla_inicial->matriz[i][j] = pantalla_crema[i][j];
			}
		}
	}*/

void PintaMensajeInicialPantalla (tipo_pantalla *p_pantalla, tipo_pantalla *p_pantalla_inicial) {
	// A completar por el alumno...
	//p_pantalla_inicial = &matriz_basica;
	//RellenaPantallaInicial (p_pantalla_inicial);
	int i,j;
	//Rellenamos la pantalla con la pantalla inicial
		for(i=0;i<MATRIZ_ANCHO;i++) {
			for(j=0;j<MATRIZ_ALTO;j++) {
				p_pantalla->matriz[i][j] = p_pantalla_inicial->matriz[i][j];
			}
	    }
		int filas, columnas = 0;
			for(columnas=0;columnas<=MATRIZ_ALTO;columnas++) {
				printf("\n");
					for(filas=0;filas<MATRIZ_ANCHO;filas++) {
						printf("%d",p_pantalla->matriz[filas][columnas]);
					}
			    }

	}




// void PintaPantallaPorTerminal (...): metodo encargado de mostrar
// el contenido o la ocupaciÃ³n de la matriz de leds en la ventana de
// terminal o consola. Este mÃ©todo sera fundamental para facilitar
// la labor de depuraciÃ³n de errores (por ejemplo, en la programaciÃ³n
// de los diferentes movimientos tanto de la raqueta como de la pelota).
void PintaPantallaPorTerminal(tipo_pantalla *p_pantalla) {
	// A completar por el alumno...
	int filas, columnas = 0;
	for(columnas=0;columnas<=MATRIZ_ALTO;columnas++) {
		printf("\n");
			for(filas=0;filas<MATRIZ_ANCHO;filas++) {
				printf("%d",p_pantalla->matriz[filas][columnas]);
			}
	    }
}

// void PintaLadrillos(...): funcion encargada de â€œpintarâ€� los ladrillos
// en sus correspondientes posiciones dentro del Ã¡rea de juego
void PintaLadrillos(tipo_pantalla *p_ladrillos, tipo_pantalla *p_pantalla) {
	int i, j = 0;

	for(i=0;i<MATRIZ_ANCHO;i++) {
		for(j=0;j<MATRIZ_ALTO;j++) {
			p_pantalla->matriz[i][j] = p_ladrillos->matriz[i][j];
		}
    }
}

// void PintaRaqueta(...): funcion encargada de â€œpintarâ€� la raqueta
// en su posicion correspondiente dentro del Ã¡rea de juego
void PintaRaqueta(tipo_raqueta *p_raqueta, tipo_pantalla *p_pantalla) {
	int i, j = 0;

	for(i=0;i<RAQUETA_ANCHO;i++) {
		for(j=0;j<RAQUETA_ALTO;j++) {
			if (( (p_raqueta->x+i >= 0) && (p_raqueta->x+i < MATRIZ_ANCHO) ) &&
					( (p_raqueta->y+j >= 0) && (p_raqueta->y+j < MATRIZ_ALTO) ))
				p_pantalla->matriz[p_raqueta->x+i][p_raqueta->y+j] = 1;
		}
	}
}

// void PintaPelota(...): funcion encargada de â€œpintarâ€� la pelota
// en su posicion correspondiente dentro del Ã¡rea de juego
void PintaPelota(tipo_pelota *p_pelota, tipo_pantalla *p_pantalla) {
	if( (p_pelota->x >= 0) && (p_pelota->x < MATRIZ_ANCHO) ) {
		if( (p_pelota->y >= 0) && (p_pelota->y < MATRIZ_ALTO) ) {
			p_pantalla->matriz[p_pelota->x][p_pelota->y] = 1;
		}
		else {
			printf("\n\nPROBLEMAS!!!! posicion y=%d de la pelota INVALIDA!!!\n\n", p_pelota->y);
			fflush(stdout);
		}
	}
	else {
		printf("\n\nPROBLEMAS!!!! posicion x=%d de la pelota INVALIDA!!!\n\n", p_pelota->x);

	}
}

// void ActualizaPantalla(...): metodo cuya ejecucion estara ligada a
// cualquiera de los movimientos de la raqueta o de la pelota y que
// sera el encargado de actualizar convenientemente la estructura de datos
// en memoria que representa el Ã¡rea de juego y su correspondiente estado.
void ActualizaPantalla(tipo_arkanoPi* p_arkanoPi) {
    // Borro toda la pantalla
	ReseteaMatriz((tipo_pantalla*)(&(p_arkanoPi->pantalla)));
	//Pintamos ladrillos
	PintaLadrillos((tipo_pantalla*)(&(p_arkanoPi->ladrillos)),(tipo_pantalla*)(&(p_arkanoPi->pantalla)));
	//Pintamos Raqueta
	PintaRaqueta((tipo_raqueta*)(&(p_arkanoPi->raqueta)),(tipo_pantalla*)(&(p_arkanoPi->pantalla)));
	//Pintamos pelota
	PintaPelota((tipo_pelota*)(&(p_arkanoPi->pelota)), (tipo_pantalla*)(&(p_arkanoPi->pantalla)));
	// A completar por el alumno...

}

// void InicializaArkanoPi(...): metodo encargado de la inicializaciÃ³n
// de toda variable o estructura de datos especificamente ligada al
// desarrollo del juego y su visualizacion.
void InicializaArkanoPi(tipo_arkanoPi *p_arkanoPi) {
	ReseteaMatriz((tipo_pantalla*)(&(p_arkanoPi->pantalla)));
	ReseteaLadrillos((tipo_pantalla*)(&(p_arkanoPi->ladrillos)));
	ReseteaPelota((tipo_pelota*)(&(p_arkanoPi->pelota)));
	ReseteaRaqueta((tipo_raqueta*)(&(p_arkanoPi->raqueta)));
	PintaMensajeInicialPantalla ((tipo_pantalla*)(&(p_arkanoPi->pantalla)),(tipo_pantalla*)(&(pantalla)));
	// A completar por el alumno...

}
void FinalizaArkanoPi(tipo_arkanoPi *p_arkanoPi) {
	ReseteaMatriz((tipo_pantalla*)(&(p_arkanoPi->pantalla)));
	ReseteaLadrillos((tipo_pantalla*)(&(p_arkanoPi->ladrillos)));
	ReseteaPelota((tipo_pelota*)(&(p_arkanoPi->pelota)));
	ReseteaRaqueta((tipo_raqueta*)(&(p_arkanoPi->raqueta)));
	PintaMensajeInicialPantalla ((tipo_pantalla*)(&(p_arkanoPi->pantalla)),(tipo_pantalla*)(&(pantallaFinal)));
	// A completar por el alumno...

}

// int CalculaLadrillosRestantes(...): funciÃ³n encargada de evaluar
// el estado de ocupacion del area de juego por los ladrillos y
// devolver el numero de estos
int CalculaLadrillosRestantes(tipo_pantalla *p_ladrillos) {
	int i, j = 0;
	int num_ladrillos_restantes = 0;
	for(i=0;i<LADRILLOS_ANCHO;i++) {
			for(j=0;j<LADRILLOS_ALTO;j++) {
				if(p_ladrillos->matriz[i][j] == 1)
					num_ladrillos_restantes++;
			}
	    }
	// A completar por el alumno...

	return num_ladrillos_restantes;
}
