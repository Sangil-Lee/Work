create table oxygen(number bigint not null auto_increment, 
		exec_date datetime, 
		degree smallint, 
		olm_esq_volt01 double, 
		olm_esq_volt02 double, 
		olm_esq_volt03 double, 
		olm_esq_volt04 double, 
		olm_esq_volt05 double, 
		olm_esq_volt06 double,
		olm_esq_volt07 double,
		olm_esq_volt08 double,
		olm_esq_volt09 double,
		olm_esq_volt10 double,
		olm_esq_volt11 double,
		olm_esq_volt12 double,
		olm_esq_volt13 double,
		olm_mq_gauss01 double,
		olm_mq_gauss02 double,
		olm_mq_gauss03 double,
		olm_mq_gauss04 double,
		olm_mq_gauss05 double,
		olm_bcav_ampl01 mediumint,
		olm_bcav_phase01 mediumint,
		olm_bcav_amp02 mediumint,
		olm_qwrcav_phase01 mediumint,
		result_data varchar(128),
		primary key(number));

create table bismuth(number bigint not null auto_increment, 
		exec_date datetime, 
		degree smallint, 
		olm_esq_volt01 double, 
		olm_esq_volt02 double, 
		olm_esq_volt03 double, 
		olm_esq_volt04 double, 
		olm_esq_volt05 double, 
		olm_esq_volt06 double,
		olm_esq_volt07 double,
		olm_esq_volt08 double,
		olm_esq_volt09 double,
		olm_esq_volt10 double,
		olm_esq_volt11 double,
		olm_esq_volt12 double,
		olm_esq_volt13 double,
		olm_mq_gauss01 double,
		olm_mq_gauss02 double,
		olm_mq_gauss03 double,
		olm_mq_gauss04 double,
		olm_mq_gauss05 double,
		olm_bcav_ampl01 mediumint,
		olm_bcav_phase01 mediumint,
		olm_bcav_amp02 mediumint,
		olm_qwrcav_phase01 mediumint,
		result_data varchar(128),
		primary key(number));

create table t_machine_parameters(
		olm_esq_volt01 double, 
		olm_esq_volt02 double, 
		olm_esq_volt03 double, 
		olm_esq_volt04 double, 
		olm_esq_volt05 double, 
		olm_esq_volt06 double,
		olm_esq_volt07 double,
		olm_esq_volt08 double,
		olm_esq_volt09 double,
		olm_esq_volt10 double,
		olm_esq_volt11 double,
		olm_esq_volt12 double,
		olm_esq_volt13 double,
		olm_mq_gauss01 double,
		olm_mq_gauss02 double,
		olm_mq_gauss03 double,
		olm_mq_gauss04 double,
		olm_mq_gauss05 double,
		olm_bcav_ampl01 mediumint,
		olm_bcav_phase01 mediumint,
		olm_bcav_amp02 mediumint,
		olm_qwrcav_phase01 mediumint);

	delimiter //
create trigger t_machine_oxygen before insert on oxygen for each row begin update t_machine_parameters set olm_esq_volt01=new.olm_esq_volt01, 
olm_esq_volt02=new.olm_esq_volt02, 
olm_esq_volt03=new.olm_esq_volt03, 
olm_esq_volt04=new.olm_esq_volt04, 
olm_esq_volt05=new.olm_esq_volt05, 
olm_esq_volt06=new.olm_esq_volt06, 
olm_esq_volt07=new.olm_esq_volt07, 
olm_esq_volt08=new.olm_esq_volt08, 
olm_esq_volt09=new.olm_esq_volt09, 
olm_esq_volt10=new.olm_esq_volt10, 
olm_esq_volt11=new.olm_esq_volt11, 
olm_esq_volt12=new.olm_esq_volt12, 
olm_esq_volt13=new.olm_esq_volt13, 
olm_mq_gauss01=new.olm_mq_gauss01, 
olm_mq_gauss02=new.olm_mq_gauss02, 
olm_mq_gauss03=new.olm_mq_gauss03, 
olm_mq_gauss04=new.olm_mq_gauss04, 
olm_mq_gauss05=new.olm_mq_gauss05, 
olm_bcav_ampl01=new.olm_bcav_ampl01, 
olm_bcav_phase01=new.olm_bcav_phase01, 
olm_bcav_amp02=new.olm_bcav_amp02, 
olm_qwrcav_phase01=new.olm_qwrcav_phase01); end //
	delimiter ;

insert into oxygen(exec_date, degree, 
olm_esq_volt01, 
olm_esq_volt02, 
olm_esq_volt03, 
olm_esq_volt04, 
olm_esq_volt05, 
olm_esq_volt06,
olm_esq_volt07,
olm_esq_volt08,
olm_esq_volt09,
olm_esq_volt10,
olm_esq_volt11,
olm_esq_volt12,
olm_esq_volt13,
olm_mq_gauss01,
olm_mq_gauss02,
olm_mq_gauss03,
olm_mq_gauss04,
olm_mq_gauss05,
olm_bcav_ampl01,
olm_bcav_phase01,
olm_bcav_amp02,
olm_qwrcav_phase01,
result_data) values (curdate(),50,1.1,2.1,3.1,4.1,5.1,6.1,7.1,8.1,9.1,.1.1,11.1,12.1,13.1,1.1,2.1,3.1,4.1,5.1,11,180,-80,-90,'/home/ctrluser/data/');

insert into t_machine_parameters(olm_esq_volt01, 
olm_esq_volt02, 
olm_esq_volt03, 
olm_esq_volt04, 
olm_esq_volt05, 
olm_esq_volt06,
olm_esq_volt07,
olm_esq_volt08,
olm_esq_volt09,
olm_esq_volt10,
olm_esq_volt11,
olm_esq_volt12,
olm_esq_volt13,
olm_mq_gauss01,
olm_mq_gauss02,
olm_mq_gauss03,
olm_mq_gauss04,
olm_mq_gauss05,
olm_bcav_ampl01,
olm_bcav_phase01,
olm_bcav_amp02,
olm_qwrcav_phase01) values (50, 1.0, 2.0, 3.0 4.0, 5.0, 6.0 7.0, 8.0, 9.0 10.0, 11.0, 12.0, 13.0, 1.0, 2.0, 3.0 4.0, 5.0, 10, 270, -80, -90);
		    

	mysql> select * from set_parameters;
	+--------+---------------------+-----------+-----------+-----------+-----------+-----------+-----------+
	| number | in_data             | setvalue1 | setvalue2 | setvalue3 | setvalue4 | setvalue5 | setvalue6 |
	+--------+---------------------+-----------+-----------+-----------+-----------+-----------+-----------+
	|      1 | 2017-02-02 00:00:00 |       1.1 |       2.2 |       3.3 |       4.4 |       5.5 |       6.6 |
	|      2 | 2017-02-02 00:00:00 |       2.1 |       3.2 |       4.3 |       5.4 |       6.5 |       7.6 |
	+--------+---------------------+-----------+-----------+-----------+-----------+-----------+-----------+
	2 rows in set (0.00 sec)

	mysql> select * from t_set_parameters;
	Empty set (0.00 sec)

	mysql> insert into t_set_parameters(in_data, setvalue1, setvalue2, setvalue3, setvalue4, setvalue5, setvalue6) values(curdate(),2.1,3.2,4.3,5.4,6.5,7.6);
	Query OK, 1 row affected (0.03 sec)

	mysql> select * from t_set_parameters;
	+---------------------+-----------+-----------+-----------+-----------+-----------+-----------+
	| in_data             | setvalue1 | setvalue2 | setvalue3 | setvalue4 | setvalue5 | setvalue6 |
	+---------------------+-----------+-----------+-----------+-----------+-----------+-----------+
	| 2017-02-02 00:00:00 |       2.1 |       3.2 |       4.3 |       5.4 |       6.5 |       7.6 |
	+---------------------+-----------+-----------+-----------+-----------+-----------+-----------+
	1 row in set (0.00 sec)

	mysql> insert into set_parameters(in_data, setvalue1, setvalue2, setvalue3, setvalue4, setvalue5, setvalue6) values(curdate(),2.2,3.3,4.4,5.5,6.6,7.7);
	Query OK, 1 row affected (0.04 sec)

	mysql> select * from t_set_parameters;
	+---------------------+-----------+-----------+-----------+-----------+-----------+-----------+
	| in_data             | setvalue1 | setvalue2 | setvalue3 | setvalue4 | setvalue5 | setvalue6 |
	+---------------------+-----------+-----------+-----------+-----------+-----------+-----------+
	| 2017-02-02 00:00:00 |       2.2 |       3.3 |       4.4 |       5.5 |       6.6 |       7.7 |
	+---------------------+-----------+-----------+-----------+-----------+-----------+-----------+
	1 row in set (0.00 sec)

	mysql> select * from set_parameters;
	+--------+---------------------+-----------+-----------+-----------+-----------+-----------+-----------+
	| number | in_data             | setvalue1 | setvalue2 | setvalue3 | setvalue4 | setvalue5 | setvalue6 |
	+--------+---------------------+-----------+-----------+-----------+-----------+-----------+-----------+
	|      1 | 2017-02-02 00:00:00 |       1.1 |       2.2 |       3.3 |       4.4 |       5.5 |       6.6 |
	|      2 | 2017-02-02 00:00:00 |       2.1 |       3.2 |       4.3 |       5.4 |       6.5 |       7.6 |
	|      3 | 2017-02-02 00:00:00 |       2.2 |       3.3 |       4.4 |       5.5 |       6.6 |       7.7 |
	+--------+---------------------+-----------+-----------+-----------+-----------+-----------+-----------+
	3 rows in set (0.00 sec)

	mysql> show databases;
	+--------------------+
	| Database           |
	+--------------------+
	| information_schema |
	| archappl           |
	| mysql              |
	| online_model       |
	| performance_schema |
	+--------------------+
	5 rows in set (0.00 sec)

	mysql> show tables;
	+------------------------+
	| Tables_in_online_model |
	+------------------------+
	| set_parameters         |
	| t_set_parameters       |
	+------------------------+
	2 rows in set (0.00 sec)

	mysql> 
	mysql> 
	mysql> insert into set_parameters(in_data, setvalue1, setvalue2, setvalue3, setvalue4, setvalue5, setvalue6) values(curdate(),1.2,2.3,3.4,4.5,5.6,6.7);
	Query OK, 1 row affected (0.02 sec)

	mysql> desc set_parameters;
	+-----------+--------------+------+-----+---------+----------------+
	| Field     | Type         | Null | Key | Default | Extra          |
	+-----------+--------------+------+-----+---------+----------------+
	| number    | mediumint(9) | NO   | PRI | NULL    | auto_increment |
	| in_data   | datetime     | YES  |     | NULL    |                |
	| setvalue1 | double       | YES  |     | NULL    |                |
	| setvalue2 | double       | YES  |     | NULL    |                |
	| setvalue3 | double       | YES  |     | NULL    |                |
	| setvalue4 | double       | YES  |     | NULL    |                |
	| setvalue5 | double       | YES  |     | NULL    |                |
	| setvalue6 | double       | YES  |     | NULL    |                |
	+-----------+--------------+------+-----+---------+----------------+
	8 rows in set (0.00 sec)

	mysql> 
	mysql> select * from set_parameters;
	+--------+---------------------+-----------+-----------+-----------+-----------+-----------+-----------+
	| number | in_data             | setvalue1 | setvalue2 | setvalue3 | setvalue4 | setvalue5 | setvalue6 |
	+--------+---------------------+-----------+-----------+-----------+-----------+-----------+-----------+
	|      1 | 2017-02-02 00:00:00 |       1.1 |       2.2 |       3.3 |       4.4 |       5.5 |       6.6 |
	|      2 | 2017-02-02 00:00:00 |       2.1 |       3.2 |       4.3 |       5.4 |       6.5 |       7.6 |
	|      3 | 2017-02-02 00:00:00 |       2.2 |       3.3 |       4.4 |       5.5 |       6.6 |       7.7 |
	|      4 | 2017-02-02 00:00:00 |       1.2 |       2.3 |       3.4 |       4.5 |       5.6 |       6.7 |
	|      5 | 2017-02-02 00:00:00 |       0.1 |       0.2 |       0.3 |       0.4 |       0.5 |       0.6 |
	|      6 | 2017-02-02 10:00:00 |       1.1 |       1.2 |       1.3 |       1.4 |       1.5 |       1.6 |
	+--------+---------------------+-----------+-----------+-----------+-----------+-----------+-----------+
	6 rows in set (0.00 sec)

	mysql> select * from set_parameters;
	+--------+---------------------+-----------+-----------+-----------+-----------+-----------+-----------+
	| number | in_data             | setvalue1 | setvalue2 | setvalue3 | setvalue4 | setvalue5 | setvalue6 |
	+--------+---------------------+-----------+-----------+-----------+-----------+-----------+-----------+
	|      1 | 2017-02-02 00:00:00 |       1.1 |       2.2 |       3.3 |       4.4 |       5.5 |       6.6 |
	|      2 | 2017-02-02 00:00:00 |       2.1 |       3.2 |       4.3 |       5.4 |       6.5 |       7.6 |
	|      3 | 2017-02-02 00:00:00 |       2.2 |       3.3 |       4.4 |       5.5 |       6.6 |       7.7 |
	|      4 | 2017-02-02 00:00:00 |       1.2 |       2.3 |       3.4 |       4.5 |       5.6 |       6.7 |
	|      5 | 2017-02-02 00:00:00 |       0.1 |       0.2 |       0.3 |       0.4 |       0.5 |       0.6 |
	|      6 | 2017-02-02 10:00:00 |       1.1 |       1.2 |       1.3 |       1.4 |       1.5 |       1.6 |
	|      7 | 2017-02-02 10:00:10 |       2.1 |       2.2 |       2.3 |       2.4 |       2.5 |       2.6 |
	+--------+---------------------+-----------+-----------+-----------+-----------+-----------+-----------+
