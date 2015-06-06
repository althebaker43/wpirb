#!/usr/bin/python

import Tkinter
import SocketServer
import threading


class RobotMode:

    MODE_UNKNOWN =  -1
    MODE_DISABLED = 0
    MODE_AUTO =     1
    MODE_TELEOP =   2
    MODE_TEST =     3

    def __init__(self):

        self.currentValue = RobotMode.MODE_DISABLED
        self.lock = threading.Lock()


    def get(self):

        value = RobotMode.MODE_UNKNOWN

        if (self.lock.acquire() == True):
            value = self.currentValue
            self.lock.release()

        return value

    
    def set(self, newValue):

        if (self.lock.acquire() == True):
            self.currentValue = newValue
            self.lock.release()
            return True
        else:
            return False


robotMode = RobotMode()


class GUI(Tkinter.Frame):

    def __init__(
            self,
            master = None
            ):

        Tkinter.Frame.__init__(self, master)
        self.pack()
        self.createWidgets()

        self.autoSelector.select()


    def createWidgets(self):

        self.mode = Tkinter.IntVar(RobotMode.MODE_DISABLED)

        self.autoSelector = Tkinter.Radiobutton(
                self,
                text="Autonomous",
                variable=self.mode,
                value=RobotMode.MODE_AUTO
                )
        self.autoSelector.pack(side="top")

        self.teleopSelector = Tkinter.Radiobutton(
                self,
                text="Tele-operated",
                variable=self.mode,
                value=RobotMode.MODE_TELEOP
                )
        self.teleopSelector.pack(side="top")

        self.testSelector = Tkinter.Radiobutton(
                self,
                text="Test",
                variable=self.mode,
                value=RobotMode.MODE_TEST
                )
        self.testSelector.pack(side="top")

        self.enableButton = Tkinter.Button(
                self,
                text="Enable",
                command=self.enable
                )
        self.enableButton.pack(side="top")

        self.disableButton = Tkinter.Button(
                self,
                text="Disable",
                command=self.disable,
                state=Tkinter.DISABLED
                )
        self.disableButton.pack(side="top")


    def enable(self):

        self.enableButton["state"] = Tkinter.DISABLED
        self.disableButton["state"] = Tkinter.NORMAL

        self.autoSelector["state"] =    Tkinter.DISABLED
        self.teleopSelector["state"] =  Tkinter.DISABLED
        self.testSelector["state"] =    Tkinter.DISABLED

        if (robotMode.set(self.mode.get()) == False):
            print("GUI: Could not set global mode.")


    def disable(self):

        self.enableButton["state"] = Tkinter.NORMAL
        self.disableButton["state"] = Tkinter.DISABLED

        self.autoSelector["state"] =    Tkinter.NORMAL
        self.teleopSelector["state"] =  Tkinter.NORMAL
        self.testSelector["state"] =    Tkinter.NORMAL

        if (robotMode.set(RobotMode.MODE_DISABLED) == False):
            print("GUI: Could not set global mode.")


    def printMode(self):

        modeStrings = {
                RobotMode.MODE_DISABLED :     "disabled",
                RobotMode.MODE_AUTO :         "autonomous",
                RobotMode.MODE_TELEOP :       "teloperated",
                RobotMode.MODE_TEST :         "test"
                }
        print("GUI: The current mode is %s" % modeStrings[self.mode.get()])


class ModeRequestHandler(SocketServer.BaseRequestHandler):

    def handle(self):

        isConnected = True

        while (isConnected):

            request = self.request.recv(4096)
            if (len(request) == 0):
                isConnected = False
            else:
                mode = robotMode.get()
                self.request.sendall("%d" % mode)


def runGUI():

    root = Tkinter.Tk()
    gui = GUI(master=root)
    gui.mainloop()


def runServer():

    server = SocketServer.TCPServer(
            ("localhost", 9999),
            ModeRequestHandler
            )
    server.serve_forever()


if (__name__ == "__main__"):

    guiThread = threading.Thread(target=runGUI)
    serverThread = threading.Thread(target=runServer)

    serverThread.start()
    guiThread.start()
