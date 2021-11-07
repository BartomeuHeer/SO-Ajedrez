DROP DATABASE if exists ajedrez_db;

CREATE DATABASE IF NOT EXISTS ajedrez_db;

USE ajedrez_db;

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

	FOREIGN KEY (ganador) REFERENCES jugador(id),

	PRIMARY KEY (id)

	);

	

CREATE TABLE IF NOT EXISTS resultado (

	idJ INT NOT NULL,

	idP INT NOT NULL,

	puntos INT NOT NULL,

	FOREIGN KEY (idJ) REFERENCES jugador(id),

	FOREIGN KEY (idP) REFERENCES partidas(id)

	);

	

INSERT INTO jugador VALUES 

	(NULL,"barto","heer","salva","tolo","admin1",23),

	(NULL,"marc","cuadrado","batlle","marc","admin2",20),

	(NULL,"omar","fallouk","ferrer","omar","admin3",20);

	

INSERT INTO partidas VALUES 

	(NULL,'2021-03-21 10:03:21','01:03:21',1),

	(NULL,'2019-07-04 21:44:21','00:23:21',3),

	(NULL,'2020-12-11 16:02:22','00:46:21',3),

	(NULL,'2015-06-23 14:25:54','00:28:21',2),

	(NULL,'2021-04-05 23:12:05','01:11:21',1),

	(NULL,'2017-10-09 08:35:13','01:47:21',2);

	

INSERT INTO resultado VALUES

	(1,1,33),

	(2,1,21),

	(1,2,25),

	(3,2,44),

	(3,3,35),

	(2,3,32),

	(1,4,21),

	(2,4,28),

	(3,5,31),

	(1,5,33),

	(2,6,39),

	(3,6,27);

	