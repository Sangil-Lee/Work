import paramiko

hostname = '10.1.61.113'
port = 22
username = 'ctrluser'
password = 'qwer1234'

if __name__ == "__main__":
        paramiko.util.log_to_file('paramiko.log')
        s = paramiko.SSHClient()
        s.load_system_host_keys()
        s.connect(hostname, port, username, password)
        stdin, stdout, stderr = s.exec_command('/sbin/ifconfig')
        print(stdout.read())
        s.close()
