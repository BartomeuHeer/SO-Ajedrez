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
	int socket;
	char nombre[50];
	int posicion;
	int resultado;
	int numForm;
}Jugador;

typedef struct{
	int num;
	Jugador conectados[100];
}ListaConectados;

typedef struct{
	Jugador jugadores[4];
	char fecha[20];
	int estado;
	int jugadoresListos;
	int numFinal;
	int numJugadores;
	int numAceptados;
}Partida;

ListaConectados listaConect;
Partida partidas[100];
int listos = 0;

int conexion_db ()
	//estableix la connexiￃﾳ amb la base de dades que, en aquest cas, s'anomena 'ajedrez_db'
{
	conn = mysql_init(NULL);
	if (conn==NULL) {
		printf ("Error al crear la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	//inicializar la conexion
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "M7_DB",0, NULL, 0);
	if (conn==NULL) {
		printf ("Error al inicializar la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	return 0;
}

 void *AtenderCliente(void *soc_ket)
	 //es crea un bucle infinit on, mentre no es tanqui la connexiￃﾳ, la funciￃﾳ agafarￃﾠ el codi que hi haurￃﾠ
	 //al principi de cada peticiￃﾳ i, depￃﾨn de quin sigui aquest, serￃﾠ una o una altre. Per exemple, si es vol
	 //iniciar sessiￃﾳ, que el codi ￃﾩs 1, el client enviarￃﾠ "1/Marc/admin1",  amb l'usuari i la contrassenya corresponents.
	 //tambￃﾩ s'inclou l'exclusiￃﾳ mￃﾺltiple (mutex lock i unlock)
 {
	int socket = *(int *) soc_ket;	
	 char peticion[512];
	 char respuesta[512];
	 char consulta[250];
	 char peticion_num[512];
	 int err, numPart;
	
	
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
			 //case 1: inici de sessio. Rebem del client: nom_usuari/contrasenya.
			 //primer cridam a iniciar sesio. Si tot ha anat afegim l'usuari a l'estructura de conectats.
			 //Al final enviam notificacio a tots els usuaris de la conexio d'aquest.
		 case 1:	
			strcpy(peticion,strtok(NULL,"\0"));
			err = iniciar_sesion(peticion,socket,respuesta);
			if(err == 0){
				pthread_mutex_lock( &mutex ); //No me interrumpas ahora
				err = ponConectados(row[1],socket);
				pthread_mutex_unlock( &mutex); //ya puedes interrumpirme
				if(err == -1)
					sprintf(respuesta,"1|%d",-3);
				else{
					notificarConectados(socket);
				}
			}
			write(socket,respuesta, strlen(respuesta));
			break;
			//case 2: registre. Rebem del client: nom/cognom1/cognom2/nom_usuari/contrasenya/edat.
			//feim el registre i la notificacio.
		 case 2:
			 strcpy(peticion,strtok(NULL,"\0"));
			 sprintf(respuesta, "2|%d", registrarse(peticion, socket));
			 write (socket,respuesta, strlen(respuesta));
			 notificarConectados();
			 break;
			 //case 3: primera consulta. Rebem del client: "".
			 //Cridam a la funcio primera consulta inotificar conectats.
		 case 3:
			 primera_consulta(respuesta,socket);
			 write (socket,respuesta, strlen(respuesta));
			 notificarConectados(socket);
			 break;
			 //case 4: segona consulta. Rebem del client: "nom1/nom2/nom3" son de 1 a 3 noms de jugadors.
			 //Cridam a la funcio segona consulta amb la peticio. Notofocacio de conectats.
		 case 4:
			 segunda_consulta(respuesta,socket,peticion);
			 write (socket,respuesta, strlen(respuesta));
			 notificarConectados(socket);
			 break;
		 case 5:
			 //case 5: tercera consulta. Rebem del client: "data1/data2".
			 //Cridam a la funcio tercera consulta amb la peticio. Notofocacio de conectats.
			 tercera_consulta(respuesta,socket,peticion);
			 write (socket,respuesta, strlen(respuesta));
			 notificarConectados(socket);
			 break;
			 //Case 6: convidacio a una partida. 
		 case 6:
			 strcpy(peticion,strtok(NULL,"\0"));
			 err = invitacion(peticion,socket);
			 if(err == -1){
				strcpy(respuesta,"7|0/0");
				write(socket,respuesta,strlen(respuesta));
			 }
			 break;
			 //Inici de la partida
		 case 7:
			 strcpy(peticion,strtok(NULL,"\0")); 
			 inicioPart(peticion,socket);
			 break;
			 //Assignam el num del formulari al client a la partida que s'esta jugant.
		 case 8:
			 numPart = atoi(strtok(NULL,"/"));
			 int nf = atoi(strtok(NULL,"\0"));
			 for(int i = 0; i < partidas[numPart].numJugadores;i++ ){
				 if(partidas[numPart].jugadores[i].socket == socket)
					 partidas[numPart].jugadores[i].numForm = nf;
			 }
			 break;
			 //Enviam el missatge que ha enviat el client a tots els jugadors de la partida.
		 case 9:
			 numPart = atoi(strtok(NULL,"/"));
			 char nombre[50];
			 char chat[250];
			 strcpy(chat,strtok(NULL,"\0"));
			 dameNombreCon(socket,nombre);
			 for(int i = 0; i < partidas[numPart].numJugadores;i++ ){
				 sprintf(respuesta, "9|%d/%s: %s",partidas[numPart].jugadores[i].numForm,nombre,chat);
				 write(partidas[numPart].jugadores[i].socket,respuesta,strlen(respuesta));
			 }
			 break;
			 //Assignam a un jugador de la partida com a jugador principal, que sera el que fagi l'enviament de les coordenades dels obstacles.
		 case 10:
			 numPart = atoi(strtok(NULL,"\0"));
			 partidas[numPart].jugadoresListos++;
			 if(partidas[numPart].jugadoresListos == partidas[numPart].numJugadores){
				 for(int i = 0; i < partidas[numPart].numJugadores;i++){
					if(i == 0)
						sprintf(respuesta,"10|%d/1",partidas[numPart].jugadores[i].numForm);
					else
						sprintf(respuesta,"10|%d/0",partidas[numPart].jugadores[i].numForm);
					 write(partidas[numPart].jugadores[i].socket,respuesta,strlen(respuesta));
				 }
			 }
			 break;
			 //Els ervidor reb del jugador principal dos numeros, un que sera el numero que li assignam a un obstacle i l'altre la coordenada y. ex: 5/345 (obstacle 5, posicio y 345.
			 //Auesta es envaida a tots els clients.
		 case 11: 
			 numPart = atoi(strtok(NULL,"/"));
			 int numObstaculo = atoi(strtok(NULL,"/"));
			 int posObstaculo = atoi(strtok(NULL,"\0"));
			 for(int i = 0; i < partidas[numPart].numJugadores;i++ ){
				 sprintf(respuesta, "11|%d/%d/%d",partidas[numPart].jugadores[i].numForm,numObstaculo,posObstaculo);
				 write(partidas[numPart].jugadores[i].socket,respuesta,strlen(respuesta));
			 }
			 break;
			 //rebem els punts del cleint al final de la partida o quan s'ha quedat sense vides. Cridam a la funcio finalPartida();
		 case 12:
			 numPart = atoi(strtok(NULL,"/"));
			 int puntos = atoi(strtok(NULL,"/"));
			 finalPartida(numPart,puntos,socket);
			 break;
			 //El client principal de la partida envia la data en que s'ha comena￧at la partida. 
		 case 13: 
			 numPart = atoi(strtok(NULL,"/"));
			 strcpy(partidas[numPart].fecha,strtok(NULL,"/"));
			 break;
			 //El cleint principal envia la peticio per inserir a la base de dades la partida que s'ha acabat.
		 case 14: 
			 numPart = atoi(strtok(NULL,"/"));
			 insertarPartida(numPart);
			 break;
			 //Donam de baixa del sistema al cleint, i el llevam de conectats.
		 case 15: 
			 err = darBaja(socket);
			 if(err = -1)
				 write(socket,"13|-1",strlen("13|-1"));
			 else{
				 write(socket,"13|0",strlen("13|0"));
				 pthread_mutex_lock( &mutex ); //No me interrumpas ahora
				 int err = quitaConectados(socket);
				 pthread_mutex_unlock( &mutex); //ya puedes interrumpirme
				 notificarConectados(socket);
			 }
			 break;
			 //desconexio del client.
		 case 0:
			 pthread_mutex_lock( &mutex ); //No me interrumpas ahora
			 int err = quitaConectados(socket);
			 pthread_mutex_unlock( &mutex); //ya puedes interrumpirme
			 notificarConectados(socket);
			 terminar = 1;
			 break;
				 
		}
	}
	 close(socket);
	 pthread_exit(0);
 }
 
 int darBaja(int socket)
 //Amb dameNombreCon cercam el nom del client a partir del socket que rebem per aprametre i el borram d ela base de dades.
 {
	 char consulta[100];
	 char nombre[50];
	 dameNombreCon(socket,nombre);
	 sprintf(consulta,"delete from resultado,jugador where resultado.idJ=jugador.id and jugador.nombre='%s'",nombre);
	 int err=mysql_query (conn,consulta);
	 if (err != 0) {
		 printf ("Error al borrar resultado %u %s\n",mysql_errno(conn), mysql_error(conn));
		return -1;
	 }
	 sprintf(consulta,"delete from jugador where jugador.nombre='%s'",nombre);
	 err=mysql_query (conn,consulta);
	 if (err != 0) {
		 printf ("Error al borrar jugador %u %s\n",mysql_errno(conn), mysql_error(conn));
		 return -1;
	 }
	 return 0;
 }
 
 void insertarPartida(int numPart)
 //Rebem per parametre un numeor de partida i feim la insercio en la base de dades de la partidfa, i dels resultats.
 //Per els resultats necessitam la partida i els id de cada jugador d'aquesta partida. Al final assignam aquesta partida de la llista ambe stat de 0, que es pot borrar.
 {
	 int idJugador[partidas[numPart].numJugadores];
	 int idPartida;
	 char consulta[100];
	 
	 sprintf(consulta,"INSERT INTO partidas Values(NULL,'%s')",partidas[numPart].fecha);
	 int err=mysql_query (conn,consulta);
	 if (err != 0) {
		 printf ("Error al insertar nueva partida %u %s\n",mysql_errno(conn), mysql_error(conn));
		 exit(1);
	 }
	 strcpy(consulta,"SELECT MAX(id) FROM partidas");
	 printf("select part: %s\n",consulta);
	 err=mysql_query (conn,consulta);
	 if (err != 0) {
		 printf ("Error al consultar datos de la base %u %s\n",mysql_errno(conn), mysql_error(conn));
		 exit(1);
	 }
	 resultado = mysql_store_result (conn);  
	 row = mysql_fetch_row (resultado); 
	 idPartida = atoi(row[0]);
	
	 for(int i = 0; i < partidas[numPart].numJugadores;i++){
		 sprintf(consulta,"SELECT id FROM jugador WHERE nombre='%s'",partidas[numPart].jugadores[i].nombre);
		 printf("select jugs: %s\n",consulta);
		 err=mysql_query (conn,consulta);
		 if (err != 0) {
			 printf ("Error al consultar datos de la base %u %s\n",mysql_errno(conn), mysql_error(conn));
			 exit(1);
		 }
		 resultado = mysql_store_result (conn);  
		 row = mysql_fetch_row (resultado); 
		 sprintf(consulta, "INSERT INTO resultado Values(%d,%d,%d,%d)",atoi(row[0]),idPartida,partidas[numPart].jugadores[i].resultado,partidas[numPart].jugadores[i].posicion);
		 printf("insert results: %s\n",consulta);
		 err=mysql_query (conn,consulta);
		 if (err != 0) {
			 printf ("Error al insertar resultados %u %s\n",mysql_errno(conn), mysql_error(conn));
			 exit(1);
		 }
	 }
	 partidas[numPart].estado = 0;
	 partidas[numPart].fecha[0] = '\0';
	 partidas[numPart].jugadoresListos = 0;
	 partidas[numPart].numAceptados = 0;
	 partidas[numPart].numFinal = 0;
	 partidas[numPart].numJugadores = 0;
 }
 
 void finalPartida(int numPart, int puntos,int socket)
 //Rebem una partida, el socket del client i els punts que ha fet al final de la partida. Li assignam aquest punts a l'estructura i restam 1 a numAcceptados de l aprtida,
	 //que al prinipi es igual a total de jugadors. Si numAceptados arriba a 0, que vol dir que tots els jugadors han inserit els seus pubnts, cridam a la funcio resultados() amb el num de partida.
 {
	 printf("numAcep antes: %d\n",partidas[numPart].numAceptados);
	 for(int i = 0; i < partidas[numPart].numJugadores;i++){
		 if(partidas[numPart].jugadores[i].socket == socket){
			 partidas[numPart].jugadores[i].resultado = puntos;
			 partidas[numPart].numFinal--;
			 printf("numAcep despues: %d\n",partidas[numPart].numFinal);
			 break;
		 }
	 }
	 if(partidas[numPart].numFinal == 0){
		 char fin[100];
		 strcpy(fin,"14|");
		 for(int j = 0; j < partidas[numPart].numJugadores;j++){
			 sprintf(fin,"%s%d",fin,partidas[numPart].jugadores[j].numForm);
			 write(partidas[numPart].jugadores[j].socket,fin,strlen(fin));
		 }
		 resultados(numPart);
	 }
		 
 }
 
 void resultados(int numP)
 //Comprovam les posicions que han quedat els jugadors de una partida.
 {
	int aux1[partidas[numP].numJugadores];
	int aux2[partidas[numP].numJugadores];
	printf("aux\n");
	for(int i = 0; i < partidas[numP].numJugadores; i++){
		for(int j = 0; j < partidas[numP].numJugadores; j++){
			printf("auxiliar[j]: %d\n",aux1[j]);
			if((partidas[numP].jugadores[i].resultado + 1) >= aux1[j]){
				printf("aux 1 %d\n",aux1[j]);
				int z = partidas[numP].numJugadores-1;
				for(z; z > j; z--){
					aux1[z] = aux1[z-1];
					aux2[z] = aux2[z-1];
				}
				aux1[j] = partidas[numP].jugadores[i].resultado;
				aux2[j] = partidas[numP].jugadores[i].socket;
				break;
			}
		}
	}
	colocarPos(aux2,numP);
 }
 void colocarPos(int pos[], int numP)
	 //a partir del vector que le sposicions de la funcio resultados, inserim les posicions per cvada jugador  a l'estructura.
 {
	 char mensaje[100] = "";
	 printf("mensaje1: %s\n",mensaje);
	 for(int i = 0; i < partidas[numP].numJugadores; i++){
		 for(int j = 0; j < partidas[numP].numJugadores; j++){
			 if(pos[i] == partidas[numP].jugadores[j].socket){
				 
				 partidas[numP].jugadores[j].posicion = i + 1;
				 sprintf(mensaje,"%s%s,%d,%d/",mensaje,partidas[numP].jugadores[j].nombre,partidas[numP].jugadores[j].posicion,partidas[numP].jugadores[j].resultado);
				 printf("mensaje2: %s\n",mensaje);
			 }
		 }
	 }
	 mensaje[strlen(mensaje)-1] = NULL;
	 char res[100];
	 printf("men: %d\n",partidas[numP].numJugadores);
	 for(int i = 0; i < partidas[numP].numJugadores; i++){
		 printf("men: %s\n",mensaje);
		 sprintf(res,"12|%d-%s",partidas[numP].jugadores[i].numForm,mensaje);
		 printf("res: %s\n",res);
		 write( partidas[numP].jugadores[i].socket,res,strlen(res));
	 }
	 insertarPartida(numP);
 }
 
 int invitacion(char nombres[],int socket)
	 //Per parametres rebem els noms dels jughadors que ha seleccionat el client per convidar. Cercam una partida de l'estructura amb estat 0 (no comen￧ada) i enviam una convidacio a cada un d'ells.
 {
	 char n1[50];
	 char n2[50];
	 char respuesta[100];
	 char r[100];
	 int numJug = atoi(strcpy(r,strtok(nombres,"/"))) + 1;
	dameNombreCon(socket,n1);
	 for(int i = 0; i < 100;i++){
		if(partidas[i].estado == 0){
			strcpy(partidas[i].jugadores[0].nombre,n1);
			partidas[i].jugadores[0].socket=socket;
			partidas[i].numJugadores = numJug;
			strcpy(n2,strtok(NULL,","));
			for(int j = 1; j<partidas[i].numJugadores; j++ ){
				partidas[i].jugadores[j].socket=dameSockCon(n2);
				strcpy(partidas[i].jugadores[j].nombre,n2);
				sprintf(respuesta,"7|%d/%s",i,n1);
				write(partidas[i].jugadores[j].socket,respuesta, strlen(respuesta));
				if(j<partidas[i].numJugadores - 1)
					strcpy(n2,strtok(NULL,","));
			}
			partidas[i].estado = 1;
			partidas[i].numAceptados = 1;
			return 0;	
		}
	 }
	 return -1;
 }
 
 void inicioPart(char pet[],int socket)
 //A la peticio rebem: resposta-partida|nomJugador. Resposta es 1 si el clietn ha aceeptat la partida; 0 si el jugador ha rebutjat la aprtida.
	 //Per cada client que envia la peticio amb 1, sumam un a numAceptados de la partida. Is ariba a numAceptados=num jugadors totals de la partida convidats,
	 //enviam a tots els clients la resposta de que el joc pot comen￧ar
 {
	 char respuesta[100];
	 int res = atoi(strtok(pet,"-"));
	 int partida = atoi(strtok(NULL,"|"));
	 if(res == 1){
		 partidas[partida].estado = 0;
		 sprintf(respuesta, "8|1/%d",partida);
		 for(int i = 0; i < 4; i++){
			 if(partidas[partida].jugadores[i].socket != socket)
				 write(partidas[partida].jugadores[i].socket,respuesta,strlen(respuesta));
		 }
	 }
	 else {
		 partidas[partida].numAceptados++;
		 if(partidas[partida].numAceptados == partidas[partida].numJugadores){
			 partidas[partida].numFinal = partidas[partida].numJugadores;
			 sprintf(respuesta, "8|0/%d/%d/",partida,partidas[partida].numJugadores);
			 for(int i = 0; i < partidas[partida].numJugadores; i++)
				 sprintf(respuesta,"%s%s/",respuesta,partidas[partida].jugadores[i].nombre);
			 respuesta[strlen(respuesta)] = "\0";
			 for(int j = 0; j < partidas[partida].numJugadores; j++ )
				 write(partidas[partida].jugadores[j].socket,respuesta,strlen(respuesta));
		 }
     }
 }
 
 void notificarConectados()
	 //A cada usuari de la llista connectats mostra els que estan connectats
 {
	char notificacion[512];
	for(int i = 0;i<listaConect.num;i++){
		dameConectados(notificacion,listaConect.conectados[i].socket);
		printf("not: %s\n", notificacion);
		write (listaConect.conectados[i].socket,notificacion, strlen(notificacion));
	}
 }
 
int iniciar_sesion(char peticion[],int socket,char respuesta[]) 
	//Registra nom d'usuari i contrass"2/" + tbName.Text + "/" + tbLN1.Text + "/" + tbLN2.Text + "/" + tbUserNameR.Text + "/" + tbPassR.Text + "/" + tbAge.Textenya, i si consten a la base de dades,la cosnulta fa connectar-te al servidor.
	//La solicitud la farￃﾠ enviant el codi corresponent, l'1, seguit del nom i de la contraseenya (1/tolo/admin1).
{
	respuesta[0]='\0';
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
		exit(1);
	}
	resultado = mysql_store_result (conn);  
	row = mysql_fetch_row (resultado);
	if (row == NULL) {
		printf ("No se han obtenido datos en la consulta\n");
		sprintf(respuesta, "1|%d",-1);
		return -1;
	}
	else {
		printf ("Consulta correcta\n");
		sprintf(respuesta, "1|%d",0);
	}
	printf ("Consulta jjjjdhdh\n");
	return 0;
	
}

int registrarse (char peticion[], int socket) 
	//Reb tots els camps de la taula jugador mensy el id, comprova que el nom d'usuari no existeixi, que si es el cas retorna -1,
	//i insereix una nmova fila a la taula de dades amb els camps que ha rebut del cluent(nom,cognom1,cognom2,edat,nom usuari, contrasenya)
{
	int edad;
	char username[35];
	char pass[35];
	char consulta[256];
	char nombre[35], ap1[35],ap2[35];
	strcpy(nombre,strtok(peticion,"/"));
	strcpy(ap1,strtok(NULL,"/"));
	strcpy(ap2,strtok(NULL,"/"));
	strcpy(username,strtok(NULL,"/"));
	strcpy(pass,strtok(NULL,"/"));
	edad = atoi(strtok(NULL,"/"));
	
	sprintf(consulta,"SELECT*FROM jugador WHERE user_name='%s' AND password='%s'", username, pass);
	int err=mysql_query (conn,consulta);
	if (row == NULL){
		printf ("No se han obtenido datos en la consulta\n");
		return -2;
	}
	resultado = mysql_store_result (conn);  
	row = mysql_fetch_row (resultado);
	if(row==NULL){
		sprintf(consulta, "INSERT INTO jugador (id, nombre,apellido1,apellido2,user_name,password, edat) Values(NULL,'%s','%s','%s','%s','%s',%d)", nombre, ap1, ap2, username, pass, edad);
		int err = mysql_query(conn, consulta);
		if (err != 0){
			printf ("Error en el insert a la base de datos.\n");
			exit (1);
		}
		return 0;
	}
	else 
	   return -1;
}


int ponConectados(char nombre[50], int socket)
	//Rebem un string del nom del jugador i un nt del socket. Si la llista esta plena, 100 o mes conectats, retorna -1; si no, fica el nom i el socket dins la darrea posicio de la llistaconectats i retorna 0.
{	
	if(listaConect.num == 100)
		return -1;
	else {
		strcpy(listaConect.conectados[listaConect.num].nombre,nombre);
		listaConect.conectados[listaConect.num].socket = socket;
		listaConect.num++;
		return 0;
	}
}

int dameNombreCon(int soc, char nombre[])
	//Donat un socket dona el nom de l'usuari associat dins nombre[]. retorna -1 si no s'ha trobat el socket, 0 si ha anat be.
{
	int i;
	for(i = 0; i < listaConect.num; i++){
		if(listaConect.conectados[i].socket == soc){
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
	for(i = 0; i < listaConect.num; i++){
		if(strcmp(listaConect.conectados[i].nombre,nombre) == 0)
			return listaConect.conectados[i].socket;
	}
	return -1;
}

int quitaConectados(int socket)
	//Rebem un socket, i eliminam el client amb aquest socket de la llista de conectats.
{
	int i =0;
	while(i < listaConect.num){
		if(listaConect.conectados[i].socket==socket){
			for(i;i < listaConect.num-1;i++){
				strcpy(listaConect.conectados[i].nombre,listaConect.conectados[i+1].nombre);
				listaConect.conectados[i].socket = listaConect.conectados[i+1].socket;
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

void dameConectados(char notificacion[],int s)
	//Per parametre passam un string on guardarem els noms desl conectats, i els coket del client.
	//Dins notificacio tendrem "num conectats/nom client conectat 1/nom client conectat 2/...".
{
	int i = 0;
	char n[50];
	sprintf(notificacion,"6|%d/",listaConect.num-1);
	while(i < listaConect.num){
		dameNombreCon(s,n);
		if(strcmp(n,listaConect.conectados[i].nombre)!=0)
			sprintf(notificacion,"%s%s/",notificacion,listaConect.conectados[i].nombre);
		i++;
	}
	notificacion[strlen(notificacion)-1]= NULL;
}

void primera_consulta(char respuesta[],int socket)
	//priemra cerca. parametre respuesta: on guardam la resposta de la cerca.
	//parametre socket: socket del client.
	//La funcio cerca en la base de dades els jugadors que han jugat almenys una aprtida amb el client.
	//Resposta te el valor de -1 si no hi ha cap jugador, o "num jugadors/nom jugador 1/nom jugador 2/...".
{
	respuesta[0] = '\0';
	char consulta[500];
	char nombre[50];
	dameNombreCon(socket, nombre);
	sprintf(consulta,"select distinct jugador.nombre from jugador,partidas,resultado where \t"
			"partidas.id in (select partidas.id from partidas,resultado,jugador where jugador.nombre='%s' and jugador.id=resultado.idJ and resultado.idP=partidas.id)\t"
			" and partidas.id=resultado.idP and resultado.idJ=jugador.id;", nombre);
	int err=mysql_query (conn,consulta);
	if (err!=0) { 
		printf ("Error al consultar datos de la base %u %s\n",mysql_errno(conn), mysql_error(conn)); 
		sprintf(respuesta, "3|%d",-2);
		exit(1);
	} 
	resultado = mysql_store_result (conn);
	row = mysql_fetch_row (resultado);
	if(row == NULL){
		printf ("No hay datos para esta consulta\n");
		sprintf(respuesta, "3|%d",-1);
		return;
	}
	int cont = 0;
	char res[100];
	while(row != NULL){
		if(strcmp(row[0],nombre) != 0){
			sprintf(res,"%s%s/",res,row[0]);
			cont++;
		}
		row = mysql_fetch_row (resultado);
	}
	res[strlen(res)-1] = NULL;
	sprintf(respuesta,"3|%d/%s",cont,res);
	printf("resFinal: %s\n",respuesta);
}

void segunda_consulta(char respuesta[],int socket,char peticion[])
	//segona cerca. parametre respuesta: on guardam la resposta de la cerca.
	//parametre socket: socket del client.
	//parametre peticion: "num jugadorstotals/nom jugador 1/nom jugador 2/nom jugador 3". Tenim un nombre variable entre 1 i 3 jugadors amb els seus noms.
	//La funcio cerca en la base de dades les partides que han jugat el client amb els jugadors de peticion tots junts.
	//Dins respuesta hi haura -1 si no hi ha cap partida que compliexi les condicions, o "num partides/data partida,punts,posicio/..." el nombre de partides totals,
	//i de cada partida la data i els punts i posicio del jugador encada una d'elels.
{
	respuesta[0] = '\0';
	int numJ = atoi(strtok(NULL,"/"));
	printf("numJug: %d\n",numJ);
	char consulta[500];
	char nombre[50];
	printf("res principi: %s\n",respuesta);
	dameNombreCon(socket,nombre);
	strcpy(consulta,"select partidas.fecha,resultado.puntuacion,resultado.resultado from partidas,resultado,jugador where");
	for(int i = 0; i < numJ; i++){
		sprintf(consulta,"%s partidas.id in (select partidas.id from partidas,resultado,jugador where jugador.nombre='%s' and jugador.id=resultado.idJ and resultado.idP=partidas.id) and",consulta,strtok(NULL,"/"));
	}
	sprintf(consulta,"%s jugador.nombre='%s' and jugador.id=resultado.idJ and partidas.id=resultado.idP",consulta,nombre);
	printf("consu: %s\n",consulta);
	int err=mysql_query (conn,consulta);
	if (err!=0) { 	
		printf ("Error al consultar datos de la base %u %s\n",mysql_errno(conn), mysql_error(conn)); 
		exit(1); 
	} 
	resultado = mysql_store_result (conn);
	row = mysql_fetch_row (resultado);
	if(row == NULL){
		printf ("No hay datos para esta consulta\n");
		sprintf(respuesta, "4|%d",-1);
		return;
	}
	int cont = 0;
	char res[100];
	while(row != NULL){
		if(strcmp(row[0],nombre) != 0){
			sprintf(res,"%s%s,%s,%s/",res,row[0],row[1],row[2]);
			cont++;
		}
		row = mysql_fetch_row (resultado);
	}
	res[strlen(res)-1] = NULL;
	sprintf(respuesta,"4|%d/%s",cont,res);
	printf("resFinal: %s\n",respuesta);
}
int tercera_consulta(char respuesta[],int socket,char peticion[])
	//tercera cerca. parametre respuesta: on guardam la resposta de la cerca.
	//parametre socket: socket del client.
	//parametre peticion: "yyy-MM-dd hh:mm:ss/yyy-MM-dd hh:mm:ss". Dues dates amb hores incloses. La primera es mes petita que la segona sermpre.
	//La funcio cerca en labase de dades les dates de les partides, els punts i la posicio del jugador en les partides que s'han jugat entre les dues dates de peticion.
	//Resposta te el valor de -1 si no hi ha cap partida o "nombre de partides/data partida,punts,posicio/...".
{
	
	respuesta[0] = '\0';
	printf("res principi: %s\n",respuesta);
	char fecha1[40];
	strcpy(fecha1,strtok(NULL,"/"));
	char fecha2[40];
	strcpy(fecha2,strtok(NULL,"/"));
	char nombre[50];
	dameNombreCon(socket,nombre);
	char consulta[500];
	sprintf(consulta,"select partidas.fecha,resultado.puntuacion,resultado.resultado from partidas,resultado,jugador where\t" 
			"resultado.idP = partidas.id and resultado.idJ=jugador.id and jugador.nombre='%s' and partidas.fecha between '%s' and '%s'",nombre,fecha1,fecha2);
	printf("consu: %s\n",consulta);
	int err=mysql_query (conn,consulta);
	if (err!=0) { 
		printf ("Error al consultar datos de la base %u %s\n",mysql_errno(conn), mysql_error(conn)); 
		exit(1);
	} 
	resultado = mysql_store_result (conn);
	row = mysql_fetch_row (resultado);
	if(row == NULL){
		printf ("No hay datos para esta consulta\n");
		sprintf(respuesta, "5|%d",-1);
		return;
	}
	int cont = 0;
	char res[100];
	printf("res petita: %s\n",res);
	while(row != NULL){
		if(strcmp(row[0],nombre) != 0){
			sprintf(res,"%s%s,%s,%s/",res,row[0],row[1],row[2]);
			cont++;
		}
		row = mysql_fetch_row (resultado);
	}
	res[strlen(res)-1] = NULL;
	sprintf(respuesta,"5|%d/%s",cont,res);
	printf("resFinal: %s\n",respuesta);
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
	serv_adr.sin_port = htons(9080);
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
		printf ("Error al bind");
	if (listen(sock_listen, 3) < 0)
		printf("Error en el Listen");
	conexion_db();//Resposta segona consulta. Rebem "-1" si no hi ha dades per la consulta.
                                //Si no, rebem "total partides/data partida 1,punts que s'ha fet a la partida,posicio partida/data partida 2,punts que s'ha fet a la partida,posicio partida" 
                                //Obrim el form de la segona cerca i enviam el missatge.
	pthread_t thread[50];
	int c = 0;
	// Bucle infinito
	for (;;){  
		printf ("Escuchando\n");
		sock_conn = accept(sock_listen, NULL, NULL);
		printf ("He recibido conexion\n");
		//sock_conn es el socket que usaremos para este cliente
		// Ahora recibimos la petici?n
		pthread_create(&thread[c],NULL,AtenderCliente,&sock_conn);
		c++;
	}
}

