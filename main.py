import socket
import hashlib
from queue import PriorityQueue
import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk
#from pgi.repository import Gtk, GObject

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


class GUI(Gtk.Window):

    def __init__(self):
        Gtk.Window.__init__(self, title="Login Window")
        self.set_border_width(10)

        top_box = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=10)
        self.add(top_box)

        self.entry_login = Gtk.Entry()
        self.entry_login.set_text('Login')
        top_box.pack_start(self.entry_login, True, True, 0)
        global Login_buffer

        self.entry_password = Gtk.Entry()
        self.entry_password.set_text('Password')
        top_box.pack_start(self.entry_password, True, True, 0)
        global Password_buffer
        

        bot_box = Gtk.Box(spacing=6)
        top_box.pack_start(bot_box, True, True, 0)
        top_box.grab_focus()

        button_registration = Gtk.Button.new_with_label('Registration')
        button_registration.connect('clicked', self.REG_button_clicked)
        bot_box.pack_start(button_registration, False, False, 0)

        button_login = Gtk.Button.new_with_label('Login')
        button_login.connect('clicked', self.Login_button_clicked)
        bot_box.pack_start(button_login, False, False, 10)

        button_close = Gtk.Button.new_with_mnemonic("_Close")
        button_close.connect("clicked", self.on_close_clicked)
        bot_box.pack_end(button_close, False, False, 0)

    def Error_msg(Error):
        msg = "Error message:" + Error
        dialog = Gtk.MessageDialog(Window, 0, Gtk.MessageType.INFO, Gtk.ButtonsType.OK, msg)
        dialog.run()
        dialog.destroy()

    def on_close_clicked(self, button):
        print("Closing application")
        Gtk.main_quit()

    def Login_button_clicked(self, button):
       Socket_Client.Connect_server()
       Socket_Client.LOGIN_user(self.entry_login.get_text(), self.entry_password.get_text())
    
    def REG_button_clicked(self, button):
        Socket_Client.Connect_server()
        Socket_Client.REG_user(self.entry_login.get_text(), self.entry_password.get_text())
        
        
def pyTest(input):
    return input
