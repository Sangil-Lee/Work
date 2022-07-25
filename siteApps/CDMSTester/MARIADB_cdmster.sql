create table result_t (
		ridx bigint unsigned NOT NULL AUTO_INCREMENT PRIMARY KEY, 
		tdate datetime, 
		result tinyint unsigned, 
		rvalue double, 
		sidx bigint unsigned, 
		sernum bigint unsigned, 
		foreign key(sidx) references scenario_t(sidx), 
		foreign key(sernum) references module_t(sernum) 
		)ENGINE=INNODB;

create table scenario_t (
		sidx bigint unsigned NOT NULL AUTO_INCREMENT, 
		scenario varchar(1024), 
		modinfo varchar(255), 
		outmod tinyint unsigned, 
		inmod tinyint unsigned, 
		evalue double, 
		PRIMARY KEY(sidx) 
		)ENGINE=INNODB;


create table module_t (
		sernum bigint unsigned NOT NULL, 
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
		grp     varchar(50),
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

