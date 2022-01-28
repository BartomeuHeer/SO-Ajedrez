DROP DATABASE if exists M7_DB;

CREATE DATABASE IF NOT EXISTS M7_DB;

USE M7_DB;

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

	PRIMARY KEY (id)

	);

	

CREATE TABLE IF NOT EXISTS resultado (

	idJ INT NOT NULL,

	idP INT NOT NULL,

	puntuacion INT,

	resultado INT,

	FOREIGN KEY (idJ) REFERENCES jugador(id),

	FOREIGN KEY (idP) REFERENCES partidas(id)

	);

	

INSERT INTO jugador VALUES 

	(NULL,"barto","heer","salva","tolo","admin1",23),

	(NULL,"marc","cuadrado","batlle","marc","admin2",20),

	(NULL,"omar","fallouk","ferrer","omar","admin3",20);

	

INSERT INTO partidas VALUES 

	(NULL,'2021-03-21 10:03:21'),

	(NULL,'2019-07-04 21:44:21'),

	(NULL,'2020-12-11 16:02:22'),

	(NULL,'2015-06-23 14:25:54'),

	(NULL,'2021-04-05 23:12:05'),

	(NULL,'2017-10-09 08:35:13');





	