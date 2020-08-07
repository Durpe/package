# -*- coding: utf-8 -*-
import tkinter as tk
import tkinter.messagebox
from tkinter import filedialog
from tkinter import *
import os
import shutil


class Window(object):
    def __init__(self):
        self.application_window = tk.Tk()
        self.application_window.geometry("450x150")
        self.application_window.title("请选择软件位置")
        self.software_box()

    def software_box(self):
        self.btn1=tk.Button(self.application_window, text='中望:',width=5,command=lambda :self.get_address(txt_box=self.txt_box1,name="中望")).grid(row=1, column=1, padx=10, pady=5, ipadx=10, ipady=5)
        self.btn1=tk.Button(self.application_window, text='CREO:',width=5, command=lambda :self.get_address(txt_box=self.txt_box2,name="CATIA")).grid(row=2, column=1, padx=10, pady=5, ipadx=10, ipady=5)
        self.btn1=tk.Button(self.application_window, text='CATIA:',width=5, command=lambda :self.get_address(txt_box=self.txt_box3,name="CREO")).grid(row=3, column=1, padx=10, pady=5, ipadx=10, ipady=5)
        self.txt_box1 = tk.Entry(self.application_window, width=30)
        self.txt_box1.grid(row=1, column=2, padx=10, pady=5, ipadx=10, ipady=5)
        self.txt_box2 = tk.Entry(self.application_window, width=30)
        self.txt_box2.grid(row=2, column=2, padx=10, pady=5, ipadx=10, ipady=5)
        self.txt_box3 = tk.Entry(self.application_window, width=30)
        self.txt_box3.grid(row=3, column=2, padx=10, pady=5, ipadx=10, ipady=5)
        self.btn4 = tk.Button(self.application_window, text='确认',command=lambda:self.copy_file(txt_box=self.txt_box1)).grid(row=1, column=3, padx=10, pady=5,ipadx=10, ipady=5)
        self.btn5 = tk.Button(self.application_window, text='确认',command=lambda:self.copy_file(txt_box=self.txt_box2)).grid(row=2, column=3, padx=10, pady=5,ipadx=10, ipady=5)
        self.btn6 = tk.Button(self.application_window, text='确认',command=lambda:self.copy_file(txt_box=self.txt_box3)).grid(row=3, column=3, padx=10, pady=5,ipadx=10, ipady=5)
        self.application_window.mainloop()

    # 选择路径
    def get_address(self,txt_box,name):
        txt_box.delete(0,'end')
        answer = filedialog.askdirectory(parent=self.application_window, initialdir="C:", title="请选择%s的路径" %name)
        txt_box.insert(0,answer)


    # 复制文件
    def copy_file(self,txt_box):
        file_name = ["Qt5Gui.dll", "Qt5Core.dll", "Qt5Sql.dll", "Qt5Svg.dll", "Qt5Widgets.dll"]
        folder_name = ["platforms", "imageformats", "plugins"]
        try:
            if txt_box.get() != '':
                for each in file_name:
                    if not os.path.exists(txt_box.get() + r"/" + each):
                        shutil.copy(os.getcwd()+"/QT/" + each, txt_box.get())
                    else:
                        tk.messagebox.showinfo("信息", "%s文件已存在" % each)

                for each in folder_name:
                    if not os.path.exists(txt_box.get() + r"/" + each):
                        shutil.copytree(os.getcwd()+"/QT/" + each, txt_box.get() + "/" + each)
                    else:
                        tk.messagebox.showinfo("信息", "%s文件已存在" % each)
                tk.messagebox.showinfo('提示', '文件复制完成')
            else:
                return
        except:
            tk.messagebox.showerror('错误', '无效的路径')


if __name__ == '__main__':
    window = Window()
