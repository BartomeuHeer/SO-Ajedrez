#include <mysql.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <pthread.h>

MYSQL *conn;
MYSQL_RES *resultado;
MYSQL_ROW row;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct{
	int id;
	char nombre[50];
	char apellido1[50];
	char apellido2[50];
	char user_name[50];
	char password[50];
	int edat;
	int sock;
}Jugador;

typedef struct{
	int num;
	Jugador jugadores[100];
}ListaJugadores;

typedef struct{
	int socket;
	char nombre[50];
}Conectado;

typedef struct{
	int num;
	Conectado conectados[100];
}ListaConectados;

typedef struct{
	char jugador1[50];
	char jugador2[50];
	int socket1;
	int socket2;
}Equipo;

typedef struct{
	//Equipo equipos[2];
	Conectado jug[4];
	int id;
}Partida;

 typedef struct{
	 Partida partidas[100];
	 int num;
 }ListaPartidas;
 
ListaJugadores listaJug;
ListaConectados listaConect;
ListaPartidas listaPart;
//listaConect.num=0;

int conexion_db (){
	conn = mysql_init(NULL);
	if (conn==NULL) {
		printf ("Error al crear la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	//inicializar la conexion
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "ajedrez_db",0, NULL, 0);
	if (conn==NULL) {
		printf ("Error al inicializar la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	return 0;
}

 void *AtenderCliente(void *soc_ket){
	int socket = *(int *) soc_ket;
	//int *s;
	//int socket;
	//s = (int *) soc_ket;
	//socket = *s;
	
	 char peticion[512];
	 char respuesta[512];
	 char consulta[250];
	 char peticion_num[512];
	 int err;
	
	
	 int terminar =0;
	
	 // Entramos en un bucle para atender todas las peticiones de este cliente
	 //hasta que se desconecte
	 while (terminar ==0)
	 {
		 // Ahora recibimos la petici?n
		int ret=read(socket,peticion_num, sizeof(peticion_num));
		 printf ("Recibido\n");
		 
		 // Tenemos que a?adirle la marca de fin de string 
		 // para que no escriba lo que hay despues en el buffer
		 peticion_num[ret]='\0';
		 
		 
		 printf ("Peticion: %s\n",peticion_num);
		 // vamos a ver que quieren
		 int codigo = atoi(strtok( peticion_num, "/"));
		 //strcpy(peticion,strtok(NULL,"\0"));
		 // Ya tenemos el c?digo de la petici?n
		 switch (codigo)
		 {
		 case 1:	
			strcpy(peticion,strtok(NULL,"\0"));
			sprintf(respuesta,"1|%d", iniciar_sesion(peticion));
			pthread_mutex_lock( &mutex ); //No me interrumpas ahora
			err = ponConectados(row[1],socket);
			//int k = 0;
			pthread_mutex_unlock( &mutex); //ya puedes interrumpirme
			if(err == -1)
				sprintf(respuesta,"1|%d",-2);
			 // Enviamos respuesta
			write (socket,respuesta, strlen(respuesta));
			notificarConectados();
			break;
		 case 2:
			 /*pthread_mutex_lock( &mutex ); //No me interrumpas ahora
			 contador = contador +1;
			 pthread_mutex_unlock( &mutex); //ya puedes interrumpirme*/
			 break;
		 case 3:
			 
			 if(primera_consulta(respuesta) == 0)
				 // Enviamos respuesta
				 write (socket,respuesta, strlen(respuesta));
				notificarConectados();
			 break;
		 case 4:
			 if(segunda_consulta(respuesta) == 0)
				 // Enviamos respuesta
				 write (socket,respuesta, strlen(respuesta));
			     notificarConectados();
			 break;
		 case 5:
			 if(tercera_consulta(respuesta) == 0)
				 // Enviamos respuesta
				 write (socket,respuesta, strlen(respuesta));
			 break;
		 case 6:
			 strcpy(peticion,strtok(NULL,"\0"));
			 
			 
			 else{
				 
				 //sprintf(respuesta,"%"7);
				// write(err, respuesta,strlen(respuesta));
			 }
			 break;
		 default:
			 pthread_mutex_lock( &mutex ); //No me interrumpas ahora
			 int err = quitaConectados(socket);
			 pthread_mutex_unlock( &mutex); //ya puedes interrumpirme
			 notificarConectados();
			 terminar = 1;
			 break;
		 }
	 }
	 // Se acabo el servicio para este cliente
	 close(socket);
	 pthread_exit(0);
 }
 
 int invitacion(char nombres[],int socket){
	 ListaConectados conAux;
	 char nombre1[50];
	 char nombre2[50];
	 char nombre3[50];
	 strcpy(nombre1,strtok(nombres,',');
	 strcpy(nombre2,strtok(NULL,',');
	 strcpy(nombre3,strtok(NULL,'\0');
	 if(listaPart.num == 100)
		 return -1;
	 else{
		 int i = 0;
		 for(i; i<3;i++ ){
			 int err = dameSockCon(peticion);
			 if(err == -1) {
				 strcpy(respuesta,-1);
				 write(socket,respuesta,strlen(respuesta)); 
			 }
			 else{
				 
			 }
		
		 }
	 }
 }
 
 void notificarConectados(){
	 char notificacion[512];
	dameConectados(notificacion);
	int i;
	for(i = 0;i<listaConect.num;i++){
		write (listaConect.conectados[i].socket,notificacion, strlen(notificacion));
	}
 }
 
int iniciar_sesion(char peticion[],int socket) 
{
	char username[35];
	char pass[35];
	char consulta[256];
	//char p = strtok(peticion, "/");
	strcpy(username,strtok(peticion, "/"));
	//p = strtok(NULL, "/");
	strcpy(pass,strtok(NULL, "/"));
	sprintf(consulta,"SELECT * FROM jugador WHERE user_name='%s' AND password='%s'",username,pass);
	int err=mysql_query (conn,consulta);
	if (err != 0) {
		printf ("Error al consultar datos de la base %u %s\n",mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	resultado = mysql_store_result (conn);  
	
	row = mysql_fetch_row (resultado); 
	
	if (row == NULL) {
		printf ("No se han obtenido datos en la consulta\n");
		return -1;
	}
	else {
		printf ("Consulta correcta\n");
		return 0;
	}
}

int registrarse(){

}

int ponConectados(char nombre[50], int socket){
	if(listaConect.num == 100)
		return -1;
	else {
		strcpy(listaConect.conectados[listaConect.num].nombre,nombre);
		listaConect.conectados[listaConect.num].socket = socket;
		listaConect.num++;
		return 0;
	}
}

int dameNombreCon(int soc, char nombre[]){
	int i;
	for(i = 0; i < listaConect.num; i++){
		if(listaConect.conectados[i].socket == soc){
			strcpy(nombre, listaConect.conectados[i].nombre);
			return 0;
		}	
	}
	return -1;
}

int dameSockCon(char nombre[]){
	int i;
	for(i = 0; i < listaConect.num; i++){
		if(strcmp(listaConect.conectados[i].nombre,nombre) == 0)
			return listaConect.conectados[i].socket;
	}
	return -1;
}

int quitaConectados(int socket){
	int i =0;
	while(i < listaConect.num){
		if(listaConect.conectados[i].socket==socket){
			for(i;i < listaConect.num-1;i++){
				strcpy(listaConect.conectados[i].nombre,listaConect.conectados[i+1].nombre);
				listaConect.conectados[i].socket = listaConect.conectados[i+1].socket;
			}
			listaConect.conectados[i].nombre[0] = '\0';
			listaConect.conectados[i].socket = 0;
			return 0;
		}
		i++;
	}
	return -1;
}

void dameConectados(char notificacion[]){
	int i = 0;
	
	sprintf(notificacion,"6|%d/",listaConect.num);
	//printf("%d\n",listaConect.num);
	while(i < listaConect.num){
		printf("%s\n",listaConect.conectados[i].nombre);
		sprintf(notificacion,"%s%s/",notificacion,listaConect.conectados[i].nombre);
		i++;
	}
	notificacion[strlen(notificacion)-1]= NULL;
	//printf("%s\n",notificacion);
}

int primera_consulta(char respuesta[]){
	respuesta[0] = '\0';
	int err=mysql_query (conn,"select distinct jugador.nombre, jugador.apellido1, jugador.apellido2 from (jugador,partidas,resultado) where partidas.duracion < '01:00:00' AND partidas.ganador = jugador.id AND partidas.id = resultado.idP AND resultado.idJ = jugador.id");
	if (err!=0) { 
		printf ("Error al consultar datos de la base %u %s\n",mysql_errno(conn), mysql_error(conn)); 
		sprintf(respuesta, "%d",-1);
	} 
	resultado = mysql_store_result (conn);
	row = mysql_fetch_row (resultado);
	
	if (row == NULL){
		printf ("No se han obtenido datos en la consulta\n");
		sprintf(respuesta, "3|%d",-2);
	}
	else {
		int cont = 0;
		char res[200];
		while(row != NULL){
			sprintf(res,"%s%s,%s,%s/",res,row[0],row[1],row[2]);
			row = mysql_fetch_row (resultado);
			cont++;
		}
		respuesta[strlen(respuesta)-1] = '\0';
		sprintf(respuesta,"3|%d/%s",cont,res);
	}
	return 0;
}
int segunda_consulta(char respuesta[]){
	int err=mysql_query (conn, "select jugador.nombre, jugador.apellido1, jugador.apellido2 from (jugador,partidas,resultado) where jugador.id = resultado.idJ and resultado.puntos = (select MAX(resultado.puntos) from (resultado)) and jugador.id = partidas.ganador");
	if (err!=0) { 	
		printf ("Error al consultar datos de la base %u %s\n",mysql_errno(conn), mysql_error(conn)); 
		return -1; 
	} 
	resultado = mysql_store_result (conn);
	row = mysql_fetch_row (resultado);
	if (row == NULL){
		printf ("No se han obtenido datos en la consulta\n");
		sprintf(respuesta, "4|%d",-2);
	}
	else {
		int cont = 0;
		char res[200];
		while(row != NULL){
			sprintf(res,"%s,%s,%s,%s/",res,row[0],row[1],row[2]);
			row = mysql_fetch_row (resultado);
		}
		respuesta[strlen(respuesta)-1] = "\0";
		sprintf(respuesta,"4|%d/%s",cont,res);
	}
	return 0;
}
int tercera_consulta(char respuesta[]){
	int err=mysql_query (conn,"select distinct jugador.nombre, jugador.apellido1, jugador.apellido2 from (jugador,partidas,resultado) where partidas.ganador = jugador.id AND  jugador.edat > = 18");
	if (err!=0) { 
		printf ("Error al consultar datos de la base %u %s\n",mysql_errno(conn), mysql_error(conn)); 
		sprintf(respuesta, "%d",-1);
	} 
	resultado = mysql_store_result (conn);
	row = mysql_fetch_row (resultado);
	if (row == NULL){
		printf ("No se han obtenido datos en la consulta\n");
		sprintf(respuesta, "%d",-2);
	}
	else {
		int cont = 0;
		char res[200];
		while(row != NULL){
			sprintf(res,"%s%s,%s,%s/",res,row[0],row[1],row[2]);
			row = mysql_fetch_row (resultado);
		}
		respuesta[strlen(respuesta)-1] = NULL;
		sprintf(respuesta,"4|%d/%s",cont,res);
	}
	return 0;
}

int main(int argc, char **argv)
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
	serv_adr.sin_port = htons(9060);
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
		printf ("Error al bind");
	
	if (listen(sock_listen, 3) < 0)
		printf("Error en el Listen");
	conexion_db();
	pthread_t thread[50];
	int c = 0;
	// Bucle infinito
	for (;;){  
		printf ("Escuchando\n");
		printf ("%d\n",c);
		sock_conn = accept(sock_listen, NULL, NULL);
		printf ("He recibido conexion\n");
		//sock_conn es el socket que usaremos para este cliente
		// Ahora recibimos la petici?n
		pthread_create(&thread[c],NULL,AtenderCliente,&sock_conn);
		c++;
	}
}
