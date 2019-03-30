import MySQLdb
import datetime

def main():
	print("Hello, MyFunc!")
	conn = MySQLdb.connect(host="127.0.0.1",user="root",passwd="qwer1234",db="online_model")
	cur = conn.cursor()
	try:
		query = """INSERT INTO set_parameters (in_data,setvalue1,setvalue2,setvalue3,setvalue4,setvalue5,setvalue6) VALUES (%s,%s,%s,%s,%s,%s,%s)"""
		print query
#cur.execute(query, ('2017-02-02 10:00:10',2.1,2.2,2.3,2.4,2.5,2.6))
		now = datetime.datetime.now()
		now.strftime('%Y-%m-%d %H:%M:%S')
		cur.execute(query, (now,14.1,14.2,14.3,14.4,14.5,14.6))
		conn.commit()
	except:
		print("SQL-Execution Error!!")
		conn.rollback()
	
	print("DB, Closed!!!")
	conn.close()


if __name__=="__main__":
	main()
