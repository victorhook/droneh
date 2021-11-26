## Packet structure
| --- | --- | --- |
| 0-3 | 4-7 | 8-15 |
| Protocol | Version | Type |
| 16-* |
| Payload |


### Protocol
What protocol is in use, eg:
- Drone
- Car
- Airplane

### Version
Version of the protocol used.

### Type
A packet can have any of the following types:
0. Raw motor thrust
1. Setpoint attitude
2. Logging - download available
3. Logging - download available - ACK

## Paylods

#### Raw motor thrust
**TX:**

| 0-15 | 16-31 | 32-47 | 48-63 |
|  M1  |  M2  |  M3  |  M4  |

#### Setpoint attitude
**TX:**

| 0-15 | 16-31 | 32-47 | 48-63 |
|  Roll  |  Pitch  |  Yaw  |  Thrust  |

####
**TX:**
EMPTY

**RX:**
Client responds X packets *download available - ACK*,
each containing a single log parameter available to log.
To indicate that there's no more parameters available, client sends a
*download available - ACK* without any payload.

| Byte 0 | Byte 1-11 |
| Param type | Param name |


## Logging

