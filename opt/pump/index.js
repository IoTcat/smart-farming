const mqtt = require('mqtt');
const client  = mqtt.connect('mqtts://mqtt.yimian.xyz:30032');

var mcu = {
	name: 'waterSys',
	syncId: 4,
	status: 0,
	data:{
		pump0: 0,
		pump1: 1,
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

  	client.publish('qos/' + mcu.name, message.toString());

  	if(parseInt(Number(message) / 100) <= mcu.syncId){
  		snsrSimulate();
  	}
  }

  if(topic.toString() == 'ctl/' + mcu.name + '/status'){
  	mcu.status = message;
  }

  if(topic.toString() == 'ctl/' + mcu.name + '/pump0'){
  	mcu.data.pump0 = message;
  }

  if(topic.toString() == 'ctl/' + mcu.name + '/pump1'){
  	mcu.data.pump1 = message;
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



	sendSnsrData();
};

//setInterval(()=>{snsrSimulate();client.publish('qos/' + mcu.name , '-1')}, 10000);


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