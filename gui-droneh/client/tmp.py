import struct


data = bytearray([0, 0, 0, 22])
print(struct.unpack('<f', data))
