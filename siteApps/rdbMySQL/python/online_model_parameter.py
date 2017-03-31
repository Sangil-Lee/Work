import MySQLdb
import datetime

def main():
	conn = MySQLdb.connect(host="127.0.0.1",user="root",passwd="qwer1234",db="online_model")
	cur = conn.cursor()
	try:
		query = """INSERT INTO oxygen (exec_date, degree, olm_esq_volt01, olm_esq_volt02, olm_esq_volt03, olm_esq_volt04, olm_esq_volt05, olm_esq_volt06, olm_esq_volt07, olm_esq_volt08, olm_esq_volt09, olm_esq_volt10, olm_esq_volt11, olm_esq_volt12, olm_esq_volt13, olm_mq_gauss01, olm_mq_gauss02, olm_mq_gauss03, olm_mq_gauss04, olm_mq_gauss05, olm_bcav_ampl01, olm_bcav_phase01, olm_bcav_amp02, olm_qwrcav_phase01, result_data) 
		VALUES (%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s)"""
		print query
		now = datetime.datetime.now()
		now.strftime('%Y-%m-%d %H:%M:%S')
		cur.execute(query, (now, 50,1.6,2.6,3.6,4.6,5.6,6.6,7.6,8.6,9.6,10.6,11.6,12.6,13.6,1.6,2.6,3.6,4.6,5.6,20,-270,-80,180,'/home/ctrluser/data1/'))
		conn.commit()
	except:
		print("SQL-Execution Error!!")
		conn.rollback()
	
	print("DB, Closed!!!")
	conn.close()


if __name__=="__main__":
	main()
