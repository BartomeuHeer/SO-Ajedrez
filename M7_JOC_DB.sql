DROP DATABASE if exists M7_JOC_DB;

CREATE DATABASE IF NOT EXISTS M7_JOC_DB;

USE M7_JOC_DB;

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

	duracion TIME NOT NULL,

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

	

INSERT INTO resultado VALUES

	(1,1,'00:03:21',1),

	(2,1,'00:05:21',2),

	(1,2,'00:06:33',3),

	(3,2,'00:04:11',2),

	(2,2,'00:02:45',1),

	(2,3,'00:03:21',1),

	(1,3,'00:06:23',2),

	(3,3,'00:09:44',3),

	(1,4,'00:10:01',2),

	(2,4,'00:08:57',1),

	(3,5,'00:11:24',2),

	(1,5,'00:01:54',1),

	(2,6,'00:05:21',1),

	(3,6,'00:12:51',2);



	
	