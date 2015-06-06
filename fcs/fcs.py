#!/usr/bin/python

import Tkinter


MODE_DISABLED = 0
MODE_AUTO =     1
MODE_TELEOP =   2
MODE_TEST =     3


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

        self.mode = Tkinter.IntVar(MODE_DISABLED)

        self.autoSelector = Tkinter.Radiobutton(
                self,
                text="Autonomous",
                variable=self.mode,
                value=MODE_AUTO
                )
        self.autoSelector.pack(side="top")

        self.teleopSelector = Tkinter.Radiobutton(
                self,
                text="Tele-operated",
                variable=self.mode,
                value=MODE_TELEOP
                )
        self.teleopSelector.pack(side="top")

        self.testSelector = Tkinter.Radiobutton(
                self,
                text="Test",
                variable=self.mode,
                value=MODE_TEST
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

        self.QUIT = Tkinter.Button(
                self,
                text="QUIT",
                fg="red",
                command=root.destroy
                )
        self.QUIT.pack(side="bottom")


    def enable(self):

        self.enableButton["state"] = Tkinter.DISABLED
        self.disableButton["state"] = Tkinter.NORMAL

        self.autoSelector["state"] =    Tkinter.DISABLED
        self.teleopSelector["state"] =  Tkinter.DISABLED
        self.testSelector["state"] =    Tkinter.DISABLED

        self.printMode()


    def disable(self):

        self.enableButton["state"] = Tkinter.NORMAL
        self.disableButton["state"] = Tkinter.DISABLED

        self.autoSelector["state"] =    Tkinter.NORMAL
        self.teleopSelector["state"] =  Tkinter.NORMAL
        self.testSelector["state"] =    Tkinter.NORMAL


    def printMode(self):

        modeStrings = {
                MODE_DISABLED :     "disabled",
                MODE_AUTO :         "autonomous",
                MODE_TELEOP :       "teloperated",
                MODE_TEST :         "test"
                }
        print("The current mode is %s" % modeStrings[self.mode.get()])


root = Tkinter.Tk()
gui = GUI(master=root)
gui.mainloop()
