# Smart Farming @surf

## Real-time Display Page
[https://smartfarm.yimian.xyz](https://smartfarm.yimian.xyz)
## MQTT

### MQTT Server
 - +`mqtt://mqtt.yimian.xyz:30031`: mqtt port with no SSL. (Normally Closed)
 - +`mqtts://mqtt.yimian.xyz:30032`: mqtt port with SSL. (Recommended)
 
### MQTT Subject Structure
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



### MQTT Action for Devices

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
qos/sync | subscribe | receive the qos package | 218 (random key)
qos/node0 | publish | send back the qos package | 218 (return the recived key, or return -1)


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
qos/sync | subscribe | receive the qos package | 148 (random key)
qos/node1 | publish | send back the qos package | 148 (return the recived key, or return -1)


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
qos/sync | subscribe | receive the qos package | 148 (random key)
qos/station | publish | send back the qos package | 148 (return the recived key, or return -1)


**waterSys**    

Subject | Action | Description | Demo Content
----|-------|------|------
res/waterSys/status | publish | Node0 status | 0 (ok)
res/waterSys/pump0 | publish | pump0 status | 1 (off)
res/waterSys/pump1 | publish | pump1 status | 0 (on)
ctl/waterSys/status | subscribe | run MCU at certain status | 0 (on)
ctl/waterSys/pump0 | subscribe | change pump0 status | 0 (on)
ctl/waterSys/pump1 | subscribe | change pump1 status | 1 (off)
qos/sync | subscribe | receive the qos package | 148 (random key)
qos/waterSys | publish | send back the qos package | 148 (return the recived key, or return -1)


### Client Code Examples

**Fake Station Code with NodeJS**

```js
const mqtt = require('mqtt');
const client  = mqtt.connect('mqtts://mqtt.yimian.xyz:30032');

var mcu = {
	name: 'station',
	syncId: 1,
	status: 0,
	data:{
		batteryLevel: 100,
		light: 23,
		temperature: 23,
		humidity: 45.8,
		rainfall: 22,
		CO: 11,
		NH3: 89,
		airPressure: 22,
	},
};


client.on('connect', function () {
  client.subscribe('ctl/' + mcu.name + '/#');
  client.subscribe('qos/sync');
  console.log('Connected!');
})



client.on('message', function (topic, message) {

  console.log(topic.toString() + ': ' +  message.toString());
  if(topic.toString() == 'qos/sync'){
  	// sync respond, for calculating delay purpose
  	client.publish('qos/' + mcu.name, message.toString());
  	// match syncId to send sensor message
  	if(parseInt(Number(message) / 100) <= mcu.syncId){
  		snsrSimulate();
  	}
  }

  if(topic.toString() == 'ctl/' + mcu.name + '/status'){
  	mcu.status = message;
  }

  if(topic.toString() == 'ctl/' + mcu.name + '/waterSwitch'){
  	mcu.data.waterSwitch = message;
  }

})


// collect and send sensor data
var sendSnsrData = function(){
	client.publish('res/' + mcu.name + '/status', String(mcu.status));
	for(i in mcu.data){
		client.publish('res/' + mcu.name + '/' + i, String(mcu.data[i]));
	};
}

// simulation
var snsrSimulate = function() {
	var status = code2status(mcu.status);
	if(status.status == 'on'){

		mcu.data.temperature = 100 * Math.random();
		mcu.data.humidity = 100 * Math.random();
		mcu.data.light = 100 * Math.random();
		mcu.data.CO = 100 * Math.random();;
		mcu.data.NH3 = 100 * Math.random();
		mcu.data.airPressure = 100 * Math.random();
		
		if(status.battery == 'consuming' && mcu.data.batteryLevel > 0 && Math.random() < .1){
			mcu.data.batteryLevel --;
		}

		if(status.battery == 'charging' && mcu.data.batteryLevel < 100 && Math.random() < .6){
			mcu.data.batteryLevel ++;
		}
		
	}


	sendSnsrData();
};

//setInterval(snsrSimulate, 50000);


var code2status = function(statusCode){
	var o = {
		status: 'on',
		mode: 'normal',
		battery: 'consuming',
		pump0: 'ok',
		waterSwitch: 'ok',
		air: 'ok',
		pump1: 'ok',
		temperature: 'ok',
		rainfall: 'ok',
		humidity: 'ok',
		light: 'ok',
		CO: 'ok',
		NH3: 'ok',
	};
	if(statusCode % 2) o.status = 'off';
	if(statusCode % 4 >= 2) o.mode = 'sleep';
	if(statusCode % 8 >= 4){
		o.battery = 'charging';
		o.pump0 = 'error';
	}
	if(statusCode % 16 >= 8){
		o.waterSwitch = 'error';
		o.air = 'error';
		o.pump1 = 'error';
	}
	if(statusCode % 32 >= 16){
		o.temperature = 'error';
		o.rainfall = 'error';
	}
	if(statusCode % 64 >= 32){
		o.humidity = 'error';
		o.light = 'error';
	}
	if(statusCode % 128 >= 64){
		o.CO = 'error';
	}
	if(statusCode % 256 >= 128){
		o.NH3 = 'error';
	}

	return o;

}


var status2code = function(status){
	var o = {
		status: 'on',
		mode: 'normal',
		battery: 'consuming',
		pump0: 'ok',
		waterSwitch: 'ok',
		air: 'ok',
		pump1: 'ok',
		temperature: 'ok',
		rainfall: 'ok',
		humidity: 'ok',
		light: 'ok',
		CO: 'ok',
		NH3: 'ok',
	};

	Object.assign(o, status);

	return (o.status != 'on') * 1 + (o.mode != 'normal') * 2 + (o.battery != 'consuming') * 4
			+ (o.pump0 != 'ok') * 4 + (o.waterSwitch != 'ok') * 8 + (o.air != 'ok') * 8
			+ (o.pump1 != 'ok') * 8 + (o.temperature != 'ok') * 16 + (o.rainfall != 'ok') * 16
			+ (o.humidity != 'ok') * 32 + (o.light != 'ok') * 32 + (o.CO != 'ok') * 64
			+ (o.NH3 != 'ok') *128;
}
```

## API - HTTP

### HTTP Server
 - +`http://smartfarm.yimian.xyz/api`: Http API url (unsafe, will be redirected to https)
 - +`https://smartfarm.yimian.xyz/api`: Https API url (Recommended)
 
### /get

**Parameters**    

Param | Type | Description | Priority | Demo
:--:|:--:|:--:|:--:|:--:
**type** | string | target tag <station/waterSys/node> | 1 | station
sid | number | soil number <0/1>| 2 | 0
num | number | num of last items to get (default: 1) | 3, 5(default) | 15
timestart | timestamp | search start time (exclude num) | 4 | 1562036854
timeend | timestamp | search end time (exclude num) | 4 | 1562036966

**Return Parameters**    

Param | Type | Description | Demo
:--:|:--:|:--:|:--:
**code** | number | operation status code | 200 
**msg** | string | status message | "Found 2 items!!"
data | array | the objects of items | {}

**Data Content - station**     

name|type|description
--|:--:|:--:
light|Number|Data of luminosity，范围为1-65535，精度为0.01
humidity|number|Data of humidity，范围为1-100，精度为0.008
temperature|number|Data of temperature，范围为（-）40-85，精度为0.01
rainfall|number|Data of rainfall，范围为0-750，精度为1
co|number|Data of CO concentration，范围为10-1000，精度为1
nh3|number|Data of NH3 concentration，范围为10-1000，精度为1
airpressure|number|Data of air pressure，范围为300-1100，精度为0.18
batterylevel|number|Left energy of whether station battery，范围为0-100，精度为1
timestamp|number|timestamp
datetime|string|datetime format of the timestamp
status|number| device status code
qos| number | delay (ms)

**Data Content - node**     

name|type|description
--|:--:|:--:
tid|number|item id (unique)
id|number|soil id (sid)
humidity|number|Data of soil humidity，范围为0-100，精度为1
temperature|number|Data of soil temperature，范围为（-）55-180，精度为0.6
~~healthCondition~~|~~boolean~~|~~False:Healthy<br>True:Sick~~
waterswitch|number|1:Not watering<br>0:Watering
batterylevel|number|Left energy of field battery，范围为0-100，精度为1
timestamp|number|timestamp
datetime|string|datetime format of the timestamp
status|number| device status code
qos| number | delay (ms)

**Data Content - waterSys**     

name|type|description
--|:--:|:--:
timestamp|number|timestamp
datetime|string|datetime format of the timestamp
status|number| device status code
qos| number | delay (ms)
pump0 | number | pump status <0:watering/1:close>
pump1 | number | pump status <0:watering/1:close>

**Example - get last station data**    
 - +`url`: [https://smartfarm.yimian.xyz/api/get?type=station](https://smartfarm.yimian.xyz/api/get?type=station)
 - +`Return Value`:
 ```js
 {
  "code": 200,
  "msg": "Found 1 items!!",
  "data": [
    {
      "timestamp": 1562040864,
      "status": 0,
      "qos": 42,
      "batterylevel": 95,
      "light": 32.75,
      "temperature": 94.83,
      "humidity": 21.151,
      "rainfall": 22,
      "co": 57,
      "nh3": 7,
      "airpressure": 65.28,
      "datetime": "2019-07-02 12:14:24"
    }
  ]
}
 
 ```


**Example - get last 3 node1 data**    
 - +`url`: [https://smartfarm.yimian.xyz/api/get?type=node&sid=1&num=5](https://smartfarm.yimian.xyz/api/get?type=node&sid=1&num=5)
 - +`Return Value`:
 ```js
{
  "code": 200,
  "msg": "Found 2 items!!",
  "data": [
    {
      "tid": 16115,
      "timestamp": 1562041014,
      "id": 1,
      "status": 0,
      "qos": 41,
      "batterylevel": 92,
      "waterswitch": 1,
      "temperature": 79,
      "humidity": 97.1,
      "datetime": "2019-07-02 12:16:54"
    },
    {
      "tid": 16113,
      "timestamp": 1562040964,
      "id": 1,
      "status": 0,
      "qos": 40,
      "batterylevel": 92,
      "waterswitch": 1,
      "temperature": 0,
      "humidity": 90.9,
      "datetime": "2019-07-02 12:16:04"
    }
  ]
}
 
 ```

**Example - get waterSys data between timestamps**    
 - +`url`: [https://smartfarm.yimian.xyz/api/get?type=waterSys&timestart=1562041304&timeend=1562041380](https://smartfarm.yimian.xyz/api/get?type=waterSys&timestart=1562041304&timeend=1562041380)
 - +`Return Value`:
 ```js
{
  "code": 200,
  "msg": "Found 2 items!!",
  "data": [
    {
      "timestamp": 1562041314,
      "status": 0,
      "qos": 41,
      "pump0": 0,
      "pump1": 0,
      "datetime": "2019-07-02 12:21:54"
    },
    {
      "timestamp": 1562041364,
      "status": 0,
      "qos": 44,
      "pump0": 1,
      "pump1": 1,
      "datetime": "2019-07-02 12:22:44"
    }
  ]
}
 
 ```
 ### /set

**Parameters**    

Param | Type | Description | Priority | Demo
:--:|:--:|:--:|:--:|:--:
**status** | number | command code | 1 | 0
type | string | select device to change status <br/>if node, a sid is required | 2 | 4 
sid | number |  if type not defined, this will control the waterswitch<br/>otherwise, this will control the device status | 3 | 0
pid | number | pump id <0/1>| 4 | 0


**Return Parameters**    

Param | Type | Description | Demo
:--:|:--:|:--:|:--:
**code** | number | operation status code | 200 
**msg** | string | status message | "Command published successfully!!"


**Example - open node0 waterSwitch**    
 - +`url`: [https://smartfarm.yimian.xyz/api/set?sid=0&status=0](https://smartfarm.yimian.xyz/api/set?sid=0&status=0)
 - +`Return Value`:
 ```js
{
  "code": 200,
  "msg": "Command published successfully!!"
}
 
 ```
 
**Example - charging node1**    
 - +`url`: [https://smartfarm.yimian.xyz/api/set?type=node&sid=1&status=4](https://smartfarm.yimian.xyz/api/set?type=node&sid=1&status=4)
 - +`Return Value`:
 ```js
{
  "code": 200,
  "msg": "Command published successfully!!"
}
 
 ```
 
 **Example - close pump1**    
 - +`url`: [https://smartfarm.yimian.xyz/api/set?pid=1&status=1](https://smartfarm.yimian.xyz/api/set?pid=1&status=1)
 - +`Return Value`:
 ```js
{
  "code": 200,
  "msg": "Command published successfully!!"
}
 
 ```
 ### /refresh
 - Collect data from all devices instantly.
 - inherit all methods from the `/get`

**Return Parameters**    

Param | Type | Description | Demo
:--:|:--:|:--:|:--:
**code** | number | operation status code | 220 
**msg** | string | status message | "Refresh command published successfully!!"


**Example - refresh**    
 - +`url`: [https://smartfarm.yimian.xyz/api/refresh](https://smartfarm.yimian.xyz/api/refresh)
 - +`Return Value`:
 ```js
{
 "code": 220,
 "msg": "Refresh command published successfully!!",
 "data": []
}
 
 ```
 
 ## API - WebSocket
 
### WebSocket Server
 - +`ws://smartfarm.yimian.xyz/ws`: Websocket API url (unsafe, will be redirected to wss)
 - +`wss://smartfarm.yimian.xyz/ws`: Websocket SSL API url (Recommended)
 
### Returned Message
 - when there is a new data, websocket will send it to client

**Data Content**     
 - Sensor data types please reffer the API-HTTP section
 - Other data types
 
name|type|description
--|:--:|:--:
type|string|tag of the sender
BeginTime|timestamp|time recived the first data
EndTime|timestamp|time recived the last data
status|number| device status code
qos| number | delay (ms) 

### Examples

**node**    
```js
{
 "type":"node1",
 "data":{
  "BeginTime":"1562044414358",
  "status":"4",
  "batteryLevel":"100",
  "waterSwitch":"1",
  "temperature":"80.56740131637494",
  "humidity":"80.92740959881388",
  "qos":"42",
  "EndTime":"1562044414360"
 }
}
```

**station**
```js
{
 "type":"station",
 "data":{
  "BeginTime":"1562044464361",
  "status":"1",
  "batteryLevel":"90",
  "light":"11.782830029799985",
  "temperature":"62.00925162058966",
  "humidity":"92.4788961450547",
  "rainfall":"22",
  "CO":"65.13567362711616",
  "NH3":"51.85498279340408",
  "airPressure":"66.305667708908",
  "qos":"45",
  "EndTime":"1562044464365"
 }
}
```

**waterSys**   
```js
{
 "type":"waterSys",
 "data":{
  "BeginTime":"1562044614363",
  "status":"0",
  "pump0":"1",
  "pump1":"0",
  "EndTime":"1562044614364",
  "qos":"42"
 }
}
```

## Json Format via LoRa

### From Station

```js
{
      "status": 0,
      "qos": 42,
      "batterylevel": 95,
      "light": 32.75,
      "temperature": 94.83,
      "humidity": 21.151,
      "rainfall": 22,
      "co": 57,
      "nh3": 7,
      "airpressure": 65.28
}
```

### From Node

```js
{
    "id": 1,   //node id, for node0 is 0, node1 is 1
    "status": 4,
    "qos": 42,
    "batteryLevel": 100,
    "waterSwitch": 1,
    "temperature": 79,
    "humidity": 97.1
 }

```

### To Station

```js
{
    "qos": 43,
    "status": 0
}
```

### To Node

```js
{
    "qos": 43,
    "waterSwitch": 0,
    "status": 0
}
```
