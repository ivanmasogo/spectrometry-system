# Spectra-GUI-2.0
GUI for As7265x

Did not add support for uart in my PCB because originally the project would run with a raspberry( or similiar SBC with linux).
The advantage of using QT is that the application can also run on Windows if AS7265x_uart libraries are provided, for now, only runs with debug purposes.
In the folder ImportantFiles/compilationType.h coment or discomment the unnused OS line.
Also this gui can plot data from any kind of sensor, just add your libraries and change the lines where the AS7265x sensor is called.
If windows is needed and you have uart support in your pcb, feel free to implement your libraries into this solution.