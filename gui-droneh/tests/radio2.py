from client.client import Client
from client.backends import SerialBackend
from client import log
import numpy as np
import time


def second():
    t0 = time.time()
    
    while 1:
        if not client.is_armed():
            client.arm()
        t1 = time.time() 
        if (t1 - t0) > 1:
            client.set_motor_thrust(1, 1, 1, 1)
            print(client.is_armed())
            t0 = t1


if __name__ == '__main__':
    import threading
    client = Client(SerialBackend(115200, '/dev/ttyACM0', 1000))
    client.cb_on_connect.add_callback(lambda msg: print(msg))
    client.connect()
    threading.Thread(target=second, daemon=True).start()
    try:
        client.start()
    finally:
        client.disconnect()

