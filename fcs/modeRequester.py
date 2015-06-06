#!/usr/bin/python

import socket
import time

MODE_UNKNOWN =  -1
MODE_DISABLED = 0
MODE_AUTO =     1
MODE_TELEOP =   2
MODE_TEST =     3

modeStrings = {
        MODE_UNKNOWN :      "unknown",
        MODE_DISABLED :     "disabled",
        MODE_AUTO :         "autonomous",
        MODE_TELEOP :       "teloperated",
        MODE_TEST :         "test"
        }

reqSock = socket.create_connection(("localhost", 9999), 5)
if (reqSock):

    while (True):

        reqSock.sendall("request")
        responseStr = reqSock.recv(4096)

        try:
            mode = int(responseStr)
            print("Requester: the current mode is %s" % modeStrings[mode])
        except ValueError:
            print("Requester: no mode given")

        time.sleep(1)

