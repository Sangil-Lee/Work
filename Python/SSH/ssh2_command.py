from __future__ import print_function

import os
import socket

from ssh2.session import Session

host = 'ctrluser'
user = os.getlogin()

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect(('10.1.61.113', 22))

session = Session()
session.handshake(sock)
#session.agent_auth(user)
session.userauth_password('ctrluser', 'qwer1234')

channel = session.open_session()
#channel.execute('echo me; exit 2')
channel.execute('/sbin/ifconfig')
size, data = channel.read()
while size > 0:
        print(data)
        size, data = channel.read()
channel.close()
print("Exit status: %s" % channel.get_exit_status())
