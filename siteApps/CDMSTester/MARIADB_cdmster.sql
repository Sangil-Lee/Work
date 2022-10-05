#create table scenario_t (
#sidx bigint unsigned NOT NULL AUTO_INCREMENT, 
#scenario varchar(1024), 
#modinfo varchar(255), 
#outmod tinyint unsigned, 
#inmod tinyint unsigned, 
#evalue double, 
#PRIMARY KEY(sidx) 
#)ENGINE=INNODB;

create database cdmstester;

create table scenario_t (
		sidx int unsigned NOT NULL AUTO_INCREMENT, 
		scenario varchar(1024), 
		modinfo varchar(255), 
		PRIMARY KEY(sidx) 
		)ENGINE=INNODB;

/*sernum bigint unsigned NOT NULL,*/
create table module_t (
		sernum varchar(64) NOT NULL, 
		modname varchar(128), 
		modtype tinyint unsigned, 
		location tinyint unsigned, 
		desciption varchar(255), 
		PRIMARY KEY(sernum) 
		)ENGINE=INNODB;

create table user_t (
		seq int unsigned NULL AUTO_INCREMENT PRIMARY KEY, 
		user_id varchar(20), 
		passwd  varchar(50),
		grp     varchar(50)
		)ENGINE=INNODB;

/*sernum bigint unsigned*/
create table result_t (
		ridx bigint unsigned NOT NULL AUTO_INCREMENT PRIMARY KEY, 
		sidx int unsigned, 
		sernum varchar(64), 
		scenario varchar(1024), 
		tdate datetime, 
		rvalue double, 
		result tinyint unsigned, 
		foreign key(sidx) references scenario_t(sidx), 
		foreign key(sernum) references module_t(sernum) 
		)ENGINE=INNODB;



MariaDB [(none)]> show databases;
+--------------------+
| Database           |
+--------------------+
| cdmstester         |
| information_schema |
+--------------------+

MariaDB [cdmstester]> show tables;
+----------------------+
| Tables_in_cdmstester |
+----------------------+
| module_t             |
| result_t             |
| scenario_t           |
| user_t               |
+----------------------+
MariaDB [cdmstester]> desc user_t;
+---------+-------------+------+-----+---------+----------------+
| Field   | Type        | Null | Key | Default | Extra          |
+---------+-------------+------+-----+---------+----------------+
| seq     | int(11)     | NO   | PRI | NULL    | auto_increment |
| user_id | varchar(20) | YES  |     | NULL    |                |
| passwd  | varchar(50) | YES  |     | NULL    |                |
| grp     | varchar(50) | YES  |     | NULL    |                |
+---------+-------------+------+-----+---------+----------------+

MariaDB [cdmstester]> desc module_t;
+------------+---------------------+------+-----+---------+-------+
| Field      | Type                | Null | Key | Default | Extra |
+------------+---------------------+------+-----+---------+-------+
| sernum     | varchar(64)         | NO   | PRI | NULL    |       |
| modname    | varchar(128)        | YES  |     | NULL    |       |
| modtype    | tinyint(3) unsigned | YES  |     | NULL    |       |
| location   | tinyint(3) unsigned | YES  |     | NULL    |       |
| desciption | varchar(255)        | YES  |     | NULL    |       |
+------------+---------------------+------+-----+---------+-------+

MariaDB [cdmstester]> desc result_t;
+--------+---------------------+------+-----+---------+----------------+
| Field  | Type                | Null | Key | Default | Extra          |
+--------+---------------------+------+-----+---------+----------------+
| ridx   | bigint(20) unsigned | NO   | PRI | NULL    | auto_increment |
| tdate  | datetime            | YES  |     | NULL    |                |
| result | tinyint(3) unsigned | YES  |     | NULL    |                |
| rvalue | double              | YES  |     | NULL    |                |
| sidx   | bigint(20) unsigned | YES  | MUL | NULL    |                |
| sernum | varchar(64)         | YES  | MUL | NULL    |                |
+--------+---------------------+------+-----+---------+----------------+
MariaDB [cdmstester]> desc scenario_t;
+----------+---------------------+------+-----+---------+----------------+
| Field    | Type                | Null | Key | Default | Extra          |
+----------+---------------------+------+-----+---------+----------------+
| sidx     | bigint(20) unsigned | NO   | PRI | NULL    | auto_increment |
| scenario | varchar(1024)       | YES  |     | NULL    |                |
| modinfo  | varchar(255)        | YES  |     | NULL    |                |
| outmod   | tinyint(3) unsigned | YES  |     | NULL    |                |
| inmod    | tinyint(3) unsigned | YES  |     | NULL    |                |
| evalue   | double              | YES  |     | NULL    |                |
+----------+---------------------+------+-----+---------+----------------+
MariaDB [cdmstester]> desc result_t;
+--------+---------------------+------+-----+---------+----------------+
| Field  | Type                | Null | Key | Default | Extra          |
+--------+---------------------+------+-----+---------+----------------+
| ridx   | bigint(20) unsigned | NO   | PRI | NULL    | auto_increment |
| tdate  | datetime            | YES  |     | NULL    |                |
| result | tinyint(3) unsigned | YES  |     | NULL    |                |
| rvalue | double              | YES  |     | NULL    |                |
| sidx   | int(10) unsigned    | YES  | MUL | NULL    |                |
| sernum | varchar(64)         | YES  | MUL | NULL    |                |
+--------+---------------------+------+-----+---------+----------------+
6 rows in set (0.001 sec)

MariaDB [cdmstester]> desc scenario_t;
+----------+------------------+------+-----+---------+----------------+
| Field    | Type             | Null | Key | Default | Extra          |
+----------+------------------+------+-----+---------+----------------+
| sidx     | int(10) unsigned | NO   | PRI | NULL    | auto_increment |
| scenario | varchar(1024)    | YES  |     | NULL    |                |
| modinfo  | varchar(255)     | YES  |     | NULL    |                |
+----------+------------------+------+-----+---------+----------------+
3 rows in set (0.001 sec)

