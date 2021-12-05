from client.client import Client
from client.backends import SerialBackend
from client import log
import numpy as np
import time

roll = 0
pitch = 0
yaw = 0
_gyro_roll = 0
_gyro_pitch = 0
_gyro_yaw = 0
t0 = time.time()
calibrated = False
calib = 0
alpha = .6

calibration = {
    'acc_x': 0,
    'acc_y': 0,
    'acc_z': 0,
    'gyro_x': 0,
    'gyro_y': 0,
    'gyro_z': 0
}
bias = {}

def calibrate(acc_x, acc_y, acc_z, gyro_x, gyro_y, gyro_z):
    global calib, calibrated, bias
    calibration['acc_x'] += acc_x
    calibration['acc_y'] += acc_y
    calibration['acc_z'] += acc_z - 9.82
    calibration['gyro_x'] += gyro_x
    calibration['gyro_y'] += gyro_y
    calibration['gyro_z'] += gyro_z
    calib += 1

    I = 20
    if calib == I:
        for p in calibration:
            calibration[p] /= I
        calibrated = True
        bias = calibration.copy()
        print(bias)

def update(acc_x, acc_y, acc_z, gyro_x, gyro_y, gyro_z):
    global t0, roll, pitch, yaw, _gyro_roll, _gyro_pitch, _gyro_yaw, alpha

    acc_x -= bias['acc_x']
    acc_y -= bias['acc_y']
    acc_z -= bias['acc_z']
    gyro_x -= bias['gyro_x']
    gyro_y -= bias['gyro_y']
    gyro_z -= bias['gyro_z']

    t1 = time.time()
    dt = t1 - t0

    p, q, r = gyro_x, gyro_y, gyro_z
    dr = p + q*np.sin(roll)*np.tan(pitch) + r*np.cos(roll)*np.tan(pitch)
    dp =     q*np.cos(roll)               - r*np.sin(roll)
    dy =     q*np.sin(roll)/np.cos(pitch) + r*np.cos(roll)/np.cos(pitch)

    #print(f'ax: {round(acc_x, 3)}, ay: {round(acc_y, 3)}, az: {round(acc_z, 3)}')
    #print(f'gx: {round(gyro_x, 3)}, gy: {round(gyro_y, 3)}, gz: {round(gyro_z, 3)}')

    acc_roll = np.arcsin(acc_x / 9.82)
    acc_pitch = np.arctan2(acc_y, acc_z)
    print(f'Acc_roll: {int(acc_roll  * (180 / np.pi))}, Acc_pitch: {int(acc_pitch * (180 / np.pi))}')

    _gyro_roll  = roll + dr * dt
    _gyro_pitch = pitch + dp * dt
    _gyro_yaw   = yaw + dy * dt

    print(f'Groll: {int(_gyro_roll * (180 / np.pi))}, Gpitch: {int(_gyro_pitch * (180 / np.pi))}, Gyaw: {int(_gyro_yaw * (180 / np.pi))}')
    #print(f'dr: {round(dr, 3)}, dp: {round(dp, 3)}, dy: {round(dy, 3)}')

    roll  = (1-alpha)*_gyro_roll + alpha*acc_roll
    pitch = (1-alpha)*_gyro_pitch + alpha*acc_pitch
    yaw   = _gyro_yaw
    t0    = t1

    print(f'Roll: {int(roll * (180 / np.pi))}, Pitch: {int(pitch * (180 / np.pi))}, Yaw: {int(yaw * (180 / np.pi))}')
    #print(f'groll: {round(roll, 3)}, gpitch: {round(pitch, 3)}, gyaw: {round(yaw * (180 / np.pi), 3)}')

def on_new_data(data):
    acc_x = -data['acc_y'].value
    acc_y = -data['acc_x'].value
    acc_z = data['acc_z'].value
    gyro_x = data['gyro_x'].value
    gyro_y = -data['gyro_y'].value
    gyro_z = -data['gyro_z'].value

    global calibrated
    if not calibrated:
        calibrate(acc_x, acc_y, acc_z, gyro_x, gyro_y, gyro_z)
    else:
        update(acc_x, acc_y, acc_z, gyro_x, gyro_y, gyro_z)


def on_new_data2(data):
    data = {name: p.value for name, p in data.items()}
    #print(data)

if __name__ == '__main__':
    client = Client(SerialBackend(115200, '/dev/ttyACM0', 1000))
    client.cb_on_connect.add_callback(lambda msg: print(msg))

    from client import log
    configs = log.get_log_configs()
    configs = {conf.name: conf for conf in configs}
    raw = configs['raw']

    config = log.ActiveLogConfig(
        raw.name,
        raw.params,
        raw.frequency_ms,
        on_new_data=on_new_data2
    )
    client.add_log_config(config)
    client.connect()

    try:
        client.start()
    finally:
        client.disconnect()

