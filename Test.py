<<<<<<< HEAD
import socket
import time
from GUI import *

PORT = 18785
HOST = '192.168.7.224'
LOGIN = 'LOGIN SOVOK_VECHEN'

def Connect_server(host, port):
    try:
        clientsock.connect((host, port))
    except socket.error as e:
        print (e)

def Data_recv():
    buff = []
    try:
        data = clientsock.recv(1024).decode('ascii')
    except socket.error as e:
        print (e)
    else:
        buff.append(buff)
    return buff

def Send_to_server(send_me):
    clientsock.send(send_me.encode('utf-8'))
    #print(send_me.encode('utf-8'))

#def String_parsing():



clientsock = socket.socket()
data = Data_recv()
Connect_server(HOST, PORT)
data = Data_recv()
Send_to_server(LOGIN)
data = Data_recv()
Window = GUI()
data = Data_recv()
GUI.Show_text(data)
data = Data_recv()
GUI.Task(data,data,data)
data = Data_recv()
#GUI.Task('tsk_name', 'tsk_data', 'completion_percentage')
GUI.Show_Window()
data = Data_recv()
del Window
data = Data_recv()
clientsock.close()
=======
def Test(input):
	if type(input) is not int:
		raise TypeError("Type of num argument should be int")
	if input == 1:
		return input
	elif input == 0:
		return input
	else:
		output = -1
		return output

if __name__ == "__main__":
	print(Test("IO"))
>>>>>>> 543cc7f46113735aafd5e4249e66bfa1e48d3534
