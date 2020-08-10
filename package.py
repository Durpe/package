# -*- coding: utf-8 -*-
from tkinter import Tk,Button
from tkinter import messagebox
from tkinter import filedialog
from os import getcwd,listdir,path
from shutil import copy



class Window(object):
    def __init__(self):
        self.application_window = Tk()
        self.application_window.geometry("400x150+500+400")

        self.application_window.title("请选择软件位置")
        self.software_box()

    def software_box(self):
        self.btn1=Button(self.application_window, text='test',width=10,command=lambda :self.copy_qt(name="test"))
        self.btn1.grid(row=1, column=1, padx=55, pady=5, ipadx=10, ipady=5)
        self.btn2=Button(self.application_window, text='test',width=10, command=lambda :self.copy_qt(name="test"))
        self.btn2.grid(row=2, column=1, padx=55, pady=5, ipadx=10, ipady=5)
        self.btn3=Button(self.application_window, text='test',width=10, command=lambda :self.copy_qt(name="test"))
        self.btn3.grid(row=3, column=1, padx=55, pady=5, ipadx=10, ipady=5)

        self.btn4 = Button(self.application_window, text='test',width=10,command=lambda :self.copy_api(name="test"))
        self.btn4.grid(row=1, column=2, padx=10, pady=5,ipadx=10, ipady=5)
        self.btn5 = Button(self.application_window, text='test',width=10,command=lambda :self.copy_api(name="test"))
        self.btn5.grid(row=2, column=2, padx=10, pady=5,ipadx=10, ipady=5)
        self.btn6 = Button(self.application_window, text='test',width=10,command=lambda :self.copy_api(name="test"))
        self.btn6.grid(row=3, column=2, padx=10, pady=5,ipadx=10, ipady=5)
        self.application_window.mainloop()


    # 复制文件
    def copy_qt(self,name):
        # 选择路径
        address = filedialog.askdirectory(parent=self.application_window, initialdir="C:", title="请选择%s的路径" %name)
        file_name = ["", "", "", "", ""]
        #folder_name = ["platforms", "imageformats", "plugins"]
        try:
            if address != '':
                for each in file_name:
                    if not path.exists(address + r"/" + each):
                        copy(getcwd()+"/QT/" + each, address)
                    else:
                        messagebox.showinfo("信息", "%s文件已存在" % each)

                # for each in folder_name:
                #     if not os.path.exists(txt_box.get() + r"/" + each):
                #         shutil.copytree(os.getcwd()+"/QT/" + each, txt_box.get() + "/" + each)
                #     else:
                #         tk.messagebox.showinfo("信息", "%s文件已存在" % each)
                messagebox.showinfo('提示', '文件复制完成')
            else:
                return
        except:
            messagebox.showerror('错误', '无效的路径')
    # 复制api
    def copy_api(self,name):
        address = filedialog.askdirectory(parent=self.application_window, initialdir="C:", title="请选择%s的路径" % name)
        file_path = getcwd()+r'/apilibs'
        try:
            file_name = listdir(file_path)
            if address != '':
                for each in file_name:
                    if not path.exists(address + r"/" + each):
                        copy(getcwd()+r"/apilibs/" + each, address)
                    else:
                        messagebox.showinfo("信息", "%s文件已存在" % each)

                messagebox.showinfo('提示', '文件复制完成')
            else:
                return
        except FileNotFoundError:
            if address != '':
                messagebox.showerror('错误','找不到指定的文件')
            else:
                return
        except:
            messagebox.showerror('错误', '无效的路径')

if __name__ == '__main__':
    window = Window()
