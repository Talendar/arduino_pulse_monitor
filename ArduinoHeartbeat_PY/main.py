"""
	Programa auxiliar para o leitor de batimentos cardíacos.
"""

import serial
import logging
import tkinter as tk
from PIL import ImageTk, Image


# CONFIG
port_name = "/dev/ttyUSB0"  # nome da porta USB ao qual o arduino está conectado
serial_speed = 115200


# SCRIPT
try:
    connection = serial.Serial(port_name, serial_speed)  # conectando na porta serial
    print("Successfully connected to port %s at speed %d." % (port_name, serial_speed))
	
    # UI
    # ROOT WINDOW
    root = tk.Tk()
    root.geometry("220x150+150+300")
    root.config(background="#ff8c8c")
    root.overrideredirect(1)

    # TITLE
    tk.Label(root, text="<Pulse Sensor Helper>", font=("bitstream charter", 14, "bold"), background="#ff8c8c").place(x=2, y=5)

    # SPINBOX
    img = ImageTk.PhotoImage(Image.open("./data/heart_icon.png").resize((15, 15)))
    tk.Label(root, image=img, background="#ff8c8c").place(x=8, y=56)
    tk.Label(root, text="Threshold:", font=("bitstream charter", 14, "bold"), background="#ff8c8c").place(x=30, y=50)

    input_box = tk.Spinbox(root, from_=0, to=1023, font=("bitstream charter", 14))
    input_box.place(x=135, y=50, width=65, height=30)
    input_box.delete(0, "end")
    input_box.insert(0, "510")

    # BUTTONS
    tk.Button(root, text="QUIT", font=("bitstream charter", 11, "bold"),
              command=root.destroy).place(x=19, y=100, height=25, width=75)  # QUIT
    tk.Button(root, text="UPDATE", font=("bitstream charter", 11, "bold"),
              command=(lambda: connection.write(input_box.get().encode()))).place(x=113, y=100, height=25)  # UPDATE: pega o resultado da UI e envia para a o arduino pela porta serial

    # SHOW WINDOW
    root.mainloop()

except Exception as ex:
    logging.exception("Failed to connect.")
