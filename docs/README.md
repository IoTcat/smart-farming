# Smart Farming @surf



## MQTT Subjects

### Structure
````
|
|---res
|   |---node0
|   |   |---status
|   |   |---batteryLevel
|   |   |---waterSwitch
|   |   |---temperature
|   |   |---humidity
|   |
|   |---node1
|   |   |---status
|   |   |---batteryLevel
|   |   |---waterSwitch
|   |   |---temperature
|   |   |---humidity
|   |
|   |---station
|   |   |---status
|   |   |---batteryLevel
|   |   |---light
|   |   |---temperature
|   |   |---humidity
|   |   |---rainfall
|   |   |---CO
|   |   |---NH3
|   |   |---airPressure
|   |
|   |---waterSys
|   |   |---status
|   |   |---pump0
|   |   |---pump1
|
|
|---ctl
|   |---node0
|   |   |---waterSwitch
|   |   |---status
|   |
|   |---node1
|   |   |---waterSwitch
|   |   |---status
|   |
|   |---station
|   |   |---status
|   |
|   |---waterSys
|   |   |---status
|   |   |---pump0
|   |   |---pump1
|
|
|---qos
|   |---sync
|   |---node0
|   |---node1
|   |---station
|   |---waterSys
|   |---localhost
|
|

````


### Status Code

**Weight Table**    

Status | Weight 
-----|-------
on/off | 0/1
normal mode/sleep mode | 0/2
power consuming/charging | 0/4
pump0 ok/error | 0/4
waterSwitch ok/error | 0/8
temp/hum/airPre(sta) ok/error | 0/8
pump1 ok/error | 0/8
temperature sensor ok/error | 0/16
rainfall ok/error | 0/16
humidity sensor ok/error | 0/32
light sensor ok/error | 0/32
CO sensor ok/error | 0/64
NH3 sensor ok/error | 0/128

**Useful Code**    

Code | Status
----|-------
0 | on / turn on
1 | off / turn off
2 | on with sleep mode
3 | off with sleep mode
4 | on with charging
5 | off with charging

### Subjects Distribution

**Node0**    

Subject | Action | Description | Demo Content
----|-------|------|------
res/node0/status | publish | Node0 status | 0 (ok)
res/node0/batteryLevel | publish | battery left energy level | 89 (0-100)
res/node0/waterSwitch | publish | status of waterSwitch | 1 (off)
res/node0/temperature | publish | temperature measured | 28
res/node0/humidity | publish | humidity measured | 66.6
ctl/node0/waterSwitch | subscribe | control waterSwitch | 0 (turn on)
ctl/node0/status | subscribe | run MCU at certain status | 4 (charging)
qos/sync | subscribe | receive the qos package | 2018 (random key)
qos/node0 | publish | send back the qos package | 2018


**Node1**    

Subject | Action | Description | Demo Content
----|-------|------|------
res/node1/status | publish | Node1 status | 0 (ok)
res/node1/batteryLevel | publish | battery left energy level | 89 (0-100)
res/node1/waterSwitch | publish | status of waterSwitch | 1 (off)
res/node1/temperature | publish | temperature measured | 28
res/node1/humidity | publish | humidity measured | 66.6
ctl/node1/waterSwitch | subscribe | control waterSwitch | 0 (turn on)
ctl/node1/status | subscribe | run MCU at certain status | 4 (charging)
qos/sync | subscribe | receive the qos package | 2018 (random key)
qos/node1 | publish | send back the qos package | 2018


**Station**    

Subject | Action | Description | Demo Content
----|-------|------|------
res/station/status | publish | station status | 0 (ok)
res/station/batteryLevel | publish | battery left energy level | 89 (0-100)
res/station/light | publish | light measured | 558.93
res/station/temperature | publish | temperature measured | 28.32
res/station/humidity | publish | humidity measured | 66.632
res/station/rainfall | publish | rainfall measured | 445 
res/station/CO | publish | CO measured | 189
res/station/NH3 | publish | NH3 measured | 674
res/station/airPressure | publish | airPressure measured | 743.18
ctl/station/status | subscribe | run MCU at certain status | 4 (charging)
qos/sync | subscribe | receive the qos package | 2018 (random key)
qos/station | publish | send back the qos package | 2018


**waterSys**    

Subject | Action | Description | Demo Content
----|-------|------|------
res/waterSys/status | publish | Node0 status | 0 (ok)
res/waterSys/pump0 | publish | pump0 status | 1 (off)
res/waterSys/pump1 | publish | pump1 status | 0 (on)
ctl/waterSys/status | subscribe | run MCU at certain status | 0 (on)
ctl/waterSys/pump0 | subscribe | change pump0 status | 0 (on)
ctl/waterSys/pump1 | subscribe | change pump1 status | 1 (off)
qos/sync | subscribe | receive the qos package | 2018 (random key)
qos/waterSys | publish | send back the qos package | 2018



## API

