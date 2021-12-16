#include <mysql.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <pthread.h>

MYSQL* conn;
MYSQL_RES* resultado;
MYSQL_ROW row;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


typedef struct {
	int socket;
	char nombre[50];
	char tiempo[10];
	int resultado;
}Jugador;

typedef struct {
	int num;
	Jugador conectados[100];
}ListaConectados;

typedef struct {
	Jugador jugadores[4];
	int estado;
	int numJugadores;
	int numAceptados;
}Partida;


//ListaJugadores listaJug;
ListaConectados listaConect;
Partida partidas[100];
int listos = 0;

int conexion_db()
//estableix la connexió amb la base de dades que, en aquest cas, s'anomena 'ajedrez_db'
{
	conn = mysql_init(NULL);
	if (conn == NULL) {
		printf("Error al crear la conexion: %u %s\n",
			mysql_errno(conn), mysql_error(conn));
		exit(1);
	}
	//inicializar la conexion
	conn = mysql_real_connect(conn, "localhost", "root", "mysql", "M7_JOC_DB", 0, NULL, 0);
	if (conn == NULL) {
		printf("Error al inicializar la conexion: %u %s\n",
			mysql_errno(conn), mysql_error(conn));
		exit(1);
	}
	return 0;
}

void* AtenderCliente(void* soc_ket)
//es crea un bucle infinit on, mentre no es tanqui la connexió, la funció agafarà el codi que hi haurà
//al principi de cada petició i, depèn de quin sigui aquest, serà una o una altre. Per exemple, si es vol
//iniciar sessió, que el codi és 1, el client enviarà "1/Marc/admin1",  amb l'usuari i la contrassenya corresponents.
//també s'inclou l'exclusió múltiple (mutex lock i unlock)
{
	int socket = *(int*)soc_ket;
	char peticion[512];
	char respuesta[512];
	char consulta[250];
	char peticion_num[512];
	int err;


	int terminar = 0;

	// Entramos en un bucle para atender todas las peticiones de este cliente
	//hasta que se desconecte
	while (terminar == 0)
	{
		// Ahora recibimos la petici?n
		int ret = read(socket, peticion_num, sizeof(peticion_num));
		printf("Recibido\n");

		// Tenemos que a?adirle la marca de fin de string 
		// para que no escriba lo que hay despues en el buffer
		peticion_num[ret] = '\0';


		printf("Peticion: %s\n", peticion_num);
		// vamos a ver que quieren
		int codigo = atoi(strtok(peticion_num, "/"));
		//strcpy(peticion,strtok(NULL,"\0"));
		// Ya tenemos el c?digo de la petici?n
		switch (codigo)
		{
		case 1:
			strcpy(peticion, strtok(NULL, "\0"));
			sprintf(respuesta, "1|%d", iniciar_sesion(peticion));
			pthread_mutex_lock(&mutex); //No me interrumpas ahora
			err = ponConectados(row[1], socket);
			//int k = 0;
			pthread_mutex_unlock(&mutex); //ya puedes interrumpirme
			if (err == -1)
				sprintf(respuesta, "1|%d", -2);
			// Enviamos respuesta
			write(socket, respuesta, strlen(respuesta));
			notificarConectados(socket);
			break;
		case 2:
			strcpy(peticion, strtok(NULL, "\0"));
			sprintf(respuesta, "2|%d", registrarse(peticion, socket));

			// Enviamos respuesta
			write(socket, respuesta, strlen(respuesta));
			notificarConectados();
			break;
		case 3:

			if (primera_consulta(respuesta) == 0)
				// Enviamos respuesta
				write(socket, respuesta, strlen(respuesta));
			notificarConectados(socket);
			break;
		case 4:
			if (segunda_consulta(respuesta) == 0)
				// Enviamos respuesta
				write(socket, respuesta, strlen(respuesta));
			notificarConectados(socket);
			break;
		case 5:
			if (tercera_consulta(respuesta) == 0)
				// Enviamos respuesta
				write(socket, respuesta, strlen(respuesta));
			notificarConectados(socket);
			break;
		case 6:
			strcpy(peticion, strtok(NULL, "\0"));
			err = invitacion(peticion, socket);
			if (err == -1) {
				strcpy(respuesta, "7|0/0");
				write(socket, respuesta, strlen(respuesta));
			}
			break;
		case 7:
			strcpy(peticion, strtok(NULL, "\0")); //res-par|nomJug
			inicioPart(peticion, socket);
			break;
		case 9:
			printf("hola");
			int np;
			np = atoi(strtok(NULL, "/"));
			char mensaje[250];
			dameNombreCon(socket, mensaje);
			sprintf(mensaje, "9|%s: %s", mensaje, strtok(NULL, "\0"));
			for (int i = 0; i < partidas[np].numJugadores; i++)
				write(partidas[np].jugadores[i].socket, mensaje, strlen(mensaje));
			break;
		default:
			pthread_mutex_lock(&mutex); //No me interrumpas ahora
			int err = quitaConectados(socket);
			pthread_mutex_unlock(&mutex); //ya puedes interrumpirme
			notificarConectados(socket);
			terminar = 1;
			break;
		}
	}
	// Se acabo el servicio para este cliente
	close(socket);
	pthread_exit(0);
}

int invitacion(char nombres[], int socket)
//Per parametres rebem 
{
	char n1[50];
	char n2[50];
	char respuesta[100];
	char r[100];
	int numJug = atoi(strcpy(r, strtok(nombres, "/"))) + 1;
	dameNombreCon(socket, n1);
	for (int i = 0; i < 100; i++) {
		if (partidas[i].estado == 0) {
			strcpy(partidas[i].jugadores[0].nombre, n1);
			partidas[i].jugadores[0].socket = socket;
			partidas[i].numJugadores = numJug;
			printf("Num jug 1: %d\n", partidas[i].numJugadores);
			strcpy(n2, strtok(NULL, ","));
			for (int j = 1; j < partidas[i].numJugadores; j++) {
				partidas[i].jugadores[j].socket = dameSockCon(n2);
				strcpy(partidas[i].jugadores[j].nombre, n2);
				sprintf(respuesta, "7|%d/%s", i, n1);
				write(partidas[i].jugadores[j].socket, respuesta, strlen(respuesta));
				if (j < partidas[i].numJugadores - 1)
					strcpy(n2, strtok(NULL, ","));
			}
			printf("Num jug 2: %d\n", partidas[i].numJugadores);
			partidas[i].estado = 1;
			partidas[i].numAceptados = 1;
			return 0;
		}
	}
	return -1;
}

void inicioPart(char pet[], int socket) {
	char respuesta[100];
	int res = atoi(strtok(pet, "-"));
	int partida = atoi(strtok(NULL, "|"));
	// char jugador[100]; strtok(NULL,"\0");
	if (res == 1) {
		partidas[partida].estado = 0;
		sprintf(respuesta, "8|1/%d", partida);
		for (int i = 0; i < 4; i++) {
			if (partidas[partida].jugadores[i].socket != socket)
				write(partidas[partida].jugadores[i].socket, respuesta, strlen(respuesta));
		}
	}
	else {
		partidas[partida].numAceptados++;
		if (partidas[partida].numAceptados == partidas[partida].numJugadores) {
			sprintf(respuesta, "8|0/%d/%d/", partida, partidas[partida].numJugadores);
			for (int i = 0; i < partidas[partida].numJugadores; i++)
				sprintf(respuesta, "%s%s/", respuesta, partidas[partida].jugadores[i].nombre);
			respuesta[strlen(respuesta)] = "\0";
			for (int j = 0; j < partidas[partida].numJugadores; j++)
				write(partidas[partida].jugadores[j].socket, respuesta, strlen(respuesta));
		}
	}
}

void notificarConectados()
//A cada usuari de la llista connectats mostra els que estan connectats
{
	char notificacion[512];
	int i;
	for (i = 0; i < listaConect.num; i++) {
		dameConectados(notificacion, listaConect.conectados[i].socket);
		write(listaConect.conectados[i].socket, notificacion, strlen(notificacion));
	}
}

int iniciar_sesion(char peticion[], int socket)
//Registra nom d'usuari i contrassenya, i si consten a la base de dades,la cosnulta fa connectar-te al servidor.
//La solicitud la farà enviant el codi corresponent, l'1, seguit del nom i de la contraseenya (1/tolo/admin1).
{
	char username[35];
	char pass[35];
	char consulta[256];
	//char p = strtok(peticion, "/");
	strcpy(username, strtok(peticion, "/"));
	//p = strtok(NULL, "/");
	strcpy(pass, strtok(NULL, "/"));
	sprintf(consulta, "SELECT * FROM jugador WHERE user_name='%s' AND password='%s'", username, pass);
	int err = mysql_query(conn, consulta);
	if (err != 0) {
		printf("Error al consultar datos de la base %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit(1);
	}
	resultado = mysql_store_result(conn);

	row = mysql_fetch_row(resultado);

	if (row == NULL) {
		printf("No se han obtenido datos en la consulta\n");
		return -1;
	}
	else {
		printf("Consulta correcta\n");
		return 0;
	}
}

int registrarse(char peticion[], int socket)
//funcion que recibe la peticion del boton registrarse y añade los parametros de un jugador a la base de datos
//los parametros son: nombre, apellido1, apellido2, username, password y edad.
{
	int edad;
	char username[35];
	char pass[35];
	char consulta[256];
	char nombre[35], ap1[35], ap2[35];
	printf("consulta, %s\n", peticion);
	strcpy(nombre, strtok(peticion, "/"));
	strcpy(ap1, strtok(NULL, "/"));
	strcpy(ap2, strtok(NULL, "/"));
	strcpy(username, strtok(NULL, "/"));
	strcpy(pass, strtok(NULL, "/"));
	edad = atoi(strtok(NULL, "/"));

	sprintf(consulta, "SELECT*FROM jugador WHERE user_name='%s' AND password='%s'", username, pass);
	int err = mysql_query(conn, consulta);
	if (err != 0) {
		printf("Error al consultar datos de la base %u %s \n", mysql_errno(conn), mysql_error(conn));
		exit(1);
	}
	resultado = mysql_store_result(conn);
	row = mysql_fetch_row(resultado);

	if (row == NULL) {
		sprintf(consulta, "INSERT INTO jugador (id, nombre,apellido1,apellido2,user_name,password, edat) Values(NULL,'%s','%s','%s','%s','%s',%d)", nombre, ap1, ap2, username, pass, edad);
		printf("%s\n", consulta);
		int err = mysql_query(conn, consulta);
		if (err != 0) {
			printf("Error al consultar datos de la base %u %s \n", mysql_errno(conn), mysql_error(conn));
			exit(1);
		}
		return 0;
	}
	else
		return -1;
}


int ponConectados(char nombre[50], int socket)
//Si un usuari es connecta, sempre i quan no s'excedeixi el límit de jugadors, es copia el nom a la llista 
//de connectats juntament amb el seu socket.
{
	if (listaConect.num == 100)
		return -1;
	else {
		strcpy(listaConect.conectados[listaConect.num].nombre, nombre);
		listaConect.conectados[listaConect.num].socket = socket;
		listaConect.num++;
		return 0;
	}
}

int dameNombreCon(int soc, char nombre[])
//Donat un socket dona el nom de l'usuari associat
{
	int i;
	for (i = 0; i < listaConect.num; i++) {
		if (listaConect.conectados[i].socket == soc) {
			strcpy(nombre, listaConect.conectados[i].nombre);
			return 0;
		}
	}
	return -1;
}

int dameSockCon(char nombre[])
//Donat un nom d'uruari, dona el seu socket associat
{
	int i;
	for (i = 0; i < listaConect.num; i++) {
		if (strcmp(listaConect.conectados[i].nombre, nombre) == 0)
			return listaConect.conectados[i].socket;
	}
	return -1;
}

int quitaConectados(int socket)
//Compara el socket el qual es vol desconnectar de la llista i quan el troba l'elimina juntament amb el nom
{
	int i = 0;
	while (i < listaConect.num) {
		if (listaConect.conectados[i].socket == socket) {
			for (i; i < listaConect.num - 1; i++) {
				strcpy(listaConect.conectados[i].nombre, listaConect.conectados[i + 1].nombre);
				listaConect.conectados[i].socket = listaConect.conectados[i + 1].socket;
			}
			listaConect.conectados[i].nombre[0] = '\0';
			listaConect.conectados[i].socket = 0;
			listaConect.num--;
			return 0;
		}
		i++;
	}
	return -1;
}

void dameConectados(char notificacion[], int s)
//Funció que imprimeixbels noms dels connectats de la llistaConnect.
{
	int i = 0;
	char n[50];
	sprintf(notificacion, "6|%d/", listaConect.num - 1);
	while (i < listaConect.num) {
		dameNombreCon(s, n);
		if (strcmp(n, listaConect.conectados[i].nombre) != 0)
			sprintf(notificacion, "%s%s/", notificacion, listaConect.conectados[i].nombre);
		i++;
	}
	notificacion[strlen(notificacion) - 1] = NULL;
}

int primera_consulta(char respuesta[])
//Búsqueda on mostra els noms del jugadors que han guanyar partides de menys d'1 hora
{
	respuesta[0] = '\0';
	int err = mysql_query(conn, "select distinct jugador.nombre, jugador.apellido1, jugador.apellido2 from (jugador,partidas,resultado) where partidas.duracion < '01:00:00' AND partidas.ganador = jugador.id AND partidas.id = resultado.idP AND resultado.idJ = jugador.id");
	if (err != 0) {
		printf("Error al consultar datos de la base %u %s\n", mysql_errno(conn), mysql_error(conn));
		sprintf(respuesta, "%d", -1);
	}
	resultado = mysql_store_result(conn);
	row = mysql_fetch_row(resultado);

	if (row == NULL) {
		printf("No se han obtenido datos en la consulta\n");
		sprintf(respuesta, "3|%d", -2);
	}
	else {
		int cont = 0;
		char res[200];
		while (row != NULL) {
			sprintf(res, "%s%s,%s,%s/", res, row[0], row[1], row[2]);
			row = mysql_fetch_row(resultado);
			cont++;
		}
		respuesta[strlen(respuesta) - 1] = '\0';
		sprintf(respuesta, "3|%d/%s", cont, res);
	}
	return 0;
}
int segunda_consulta(char respuesta[])
//Búsqueda que indica els nonms dels guanyadors amb més punts
{
	int err = mysql_query(conn, "select jugador.nombre, jugador.apellido1, jugador.apellido2 from (jugador,partidas,resultado) where jugador.id = resultado.idJ and resultado.puntos = (select MAX(resultado.puntos) from (resultado)) and jugador.id = partidas.ganador");
	if (err != 0) {
		printf("Error al consultar datos de la base %u %s\n", mysql_errno(conn), mysql_error(conn));
		return -1;
	}
	resultado = mysql_store_result(conn);
	row = mysql_fetch_row(resultado);
	if (row == NULL) {
		printf("No se han obtenido datos en la consulta\n");
		sprintf(respuesta, "4|%d", -2);
	}
	else {
		int cont = 0;
		char res[200];
		while (row != NULL) {
			sprintf(res, "%s,%s,%s,%s/", res, row[0], row[1], row[2]);
			row = mysql_fetch_row(resultado);
		}
		respuesta[strlen(respuesta) - 1] = "\0";
		sprintf(respuesta, "4|%d/%s", cont, res);
	}
	return 0;
}
int tercera_consulta(char respuesta[])
//Búsqueda que mostra els noms dels guanyadors menors de 18 anys
{
	int err = mysql_query(conn, "select distinct jugador.nombre, jugador.apellido1, jugador.apellido2 from (jugador,partidas,resultado) where partidas.ganador = jugador.id AND  jugador.edat > = 18");
	if (err != 0) {
		printf("Error al consultar datos de la base %u %s\n", mysql_errno(conn), mysql_error(conn));
		sprintf(respuesta, "%d", -1);
	}
	resultado = mysql_store_result(conn);
	row = mysql_fetch_row(resultado);
	if (row == NULL) {
		printf("No se han obtenido datos en la consulta\n");
		sprintf(respuesta, "%d", -2);
	}
	else {
		int cont = 0;
		char res[200];
		while (row != NULL) {
			sprintf(res, "%s%s,%s,%s/", res, row[0], row[1], row[2]);
			row = mysql_fetch_row(resultado);
		}
		respuesta[strlen(respuesta) - 1] = NULL;
		sprintf(respuesta, "4|%d/%s", cont, res);
	}
	return 0;
}

int main(int argc, char** argv)
{
	int sock_conn, sock_listen;
	struct sockaddr_in serv_adr;

	// INICIALITZACIONS
	// Obrim el socket
	if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		printf("Error creant socket");
	// Fem el bind al port
	memset(&serv_adr, 0, sizeof(serv_adr));// inicialitza a zero serv_addr
	serv_adr.sin_family = AF_INET;
	// asocia el socket a cualquiera de las IP de la m?quina. 
	//htonl formatea el numero que recibe al formato necesario
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	// establecemos el puerto de escucha
	serv_adr.sin_port = htons(9050);
	if (bind(sock_listen, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) < 0)
		printf("Error al bind");

	if (listen(sock_listen, 3) < 0)
		printf("Error en el Listen");
	conexion_db();
	pthread_t thread[50];
	int c = 0;
	// Bucle infinito
	for (;;) {
		printf("Escuchando\n");
		printf("%d\n", c);
		sock_conn = accept(sock_listen, NULL, NULL);
		printf("He recibido conexion\n");
		//sock_conn es el socket que usaremos para este cliente
		// Ahora recibimos la petici?n
		pthread_create(&thread[c], NULL, AtenderCliente, &sock_conn);
		c++;
	}
}

