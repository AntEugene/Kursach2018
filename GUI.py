import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk

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
        

#Определение разрешения экрана
#screen = self.get_screen()
# #mg = screen.get_monitor_geometry(0)
#print ("monitor {}: {} x {}".format(0,mg.width,mg.height))
# #self.set_default_size(mg.width//2, mg.height//2)

#label = Gtk.Label(label='Login:', angle=25, halign=Gtk.Align.END)
#grid.add(label)
