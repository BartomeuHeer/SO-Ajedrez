CREATE DATABASE IF NOT EXISTS ajerez_db;



CREATE TABLE IF NOT EXISTS jugador (

	id INT NOT NULL AUTO_INCREMENT,

	nombre VARCHAR(20) NOT NULL,

	apellido1 VARCHAR(20) NOT NULL,

	apellido2 VARCHAR(20),
	user_name VARCHAR(20) NOT NULL,
	password VARCHAR(20) NOT NULL UNIQUE,

	edat INT NOT NULL,

	PRIMARY KEY (id)

	);

	

CREATE TABLE IF NOT EXISTS partidas (

	id INT NOT NULL AUTO_INCREMENT,

	fecha DATETIME NOT NULL,

	duracion TIME NOT NULL,

	ganador INT NOT NULL,

	PRIMARY KEY (id),

	FOREIGN KEY (ganador) REFERENCES jugador(id)

	);

	

CREATE TABLE IF NOT EXISTS resultado (

	idJ INT NOT NULL,

	idP INT NOT NULL,

	puntos INT NOT NULL,

	FOREIGN KEY (idJ) REFERENCES jugador(id),

	FOREIGN KEY (idP) REFERENCES partidas(id)

	);

	

INSERT INTO jugador VALUES 

	(0,"barto","heer","salva","barto","admin1",23),

	(1,"marc","cuadrado","batlle","marc","admin2",20),

	(2,"omar","fallouk","ferrer","omar","admin3",20);

	

INSERT INTO partidas VALUES 

	(0,'2021-03-21 10:03:21','01:03:21',0),

	(1,'2019-07-04 21:44:21','00:23:21',2),

	(2,'2020-12-11 16:02:22','00:46:21',2),

	(3,'2015-06-23 14:25:54','00:28:21',1),

	(4,'2021-04-05 23:12:05','01:11:21',0),

	(5,'2017-10-09 08:35:13','01:47:21',1);

	

INSERT INTO resultado VALUES

	(0,0,33),

	(1,0,21),

	(0,1,25),

	(2,1,44),

	(2,2,35),

	(1,2,32),

	(0,3,21),

	(1,3,28),

	(2,4,31),

	(0,4,33),

	(1,5,39),

	(2,5,27);

	