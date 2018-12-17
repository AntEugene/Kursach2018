import socket
import hashlib
#from queue import PriorityQueue
#import GUI

class Socket_Client():
    #PORT = 18785
    #HOST = '37.21.190.140'
    #HOST = '192.168.7.224'
    #LOGIN = 'Enot'
    #LOGIN = 'Furious_Fox'
    #PASSWORD = 'qwerty'
    #PASSWORD = '12345'

    def Connect_server():
        PORT = 18785
        HOST = '192.168.7.224'
        global clientsock
        clientsock = socket.socket()
        try:
            clientsock.connect((HOST,PORT))
        except Exception:
            print ('Connection error')

    def Data_recv():
        queue = PriorityQueue()
        try:
            while True:
                data = clientsock.recv(1024).decode('ascii')
                print(data + ' recv')
                if not data:
                    break
                queue.put(data)
                buffer = queue.queue[0]
                if buffer[-1] == '>' and buffer[-2] == '<':
                    break
        except socket.error as e:
            print (e)
        return queue

    def Send_to_server(send_me):
        clientsock.send(send_me.encode('ascii'))

    def REG_user(input_str_login, input_str_pass):
        Socket_Client.Send_to_server('REG ' + hashlib.sha1(input_str_pass.encode('utf-8')).hexdigest() + input_str_login+'<>')

    def LOGIN_user(input_str_login, input_str_pass):
        Socket_Client.Send_to_server('LOGIN ' + hashlib.sha1(input_str_pass.encode('utf-8')).hexdigest() + input_str_login+'<>')

    def Write_in_file(Login_user):
        client_file = open('{}'.format(Login_user), 'w')
        data = PriorityQueue()
        while data.queue[0] != 'EOF':
            data = Socket_client.Data_recv()
            client_file.write(data.get_novait())

        
        
def pyTest(input):
    return input
