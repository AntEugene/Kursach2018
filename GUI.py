from tkinter import *

class GUI:
    global root
    root = Tk()
    #screen_width = root.winfo_screenwidth()
    #screen_height = root.winfo_screenheight()
    root.title('GMINE')
    #root.resizable(False, False)
    root.geometry('600x600')
    root.protocol('WM_DELETE_WINDOW', Window_deleted)

    def SetTsk_name(tsk_name):
        global tsk_name_global
        tsk_name_global = tsk_name

    def SetTsk_name(tsk_string):
        global tsk_string_global
        tsk_string_global = tsk_string


    def Task(tsk_name_global, tsk_string, completion_percentage):
        def Show_tsk():
            tsk_window = Tk()
            tsk_window.title(string=tsk_name_global)
            tsk_label = Label(tsk_window, text=tsk_string)
        button = Button(root, text=tsk_string, width=600, height=5, bg='white', fg='black', command=Show_tsk)
        button.pack()

    def Show_text(text_in):
        label = Label(root, text = text_in, relief=RAISED, font="Arial 14", justify = LEFT)
        label.place(relx=0, rely=0)

    def Window_deleted():
        print (u'Окно закрыто')
        root.quit()

    def Show_Window():
        root.mainloop()