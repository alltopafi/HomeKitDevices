var Temperature = 0.0;

// MQTT Setup
var mqtt = require('mqtt');
console.log("Connecting to MQTT broker...");
var mqtt = require('mqtt');
var options = {
port: 1883,
host: '192.168.1.140',
clientId: 'Home_TemperatureSensor'
};
var client = mqtt.connect(options);
console.log("Home Temperature Sensor Connected to MQTT broker");
client.subscribe('home/temperature');
client.on('message', function(topic, message) {
          console.log(parseFloat(message));
          Temperature = parseFloat(message);
          });

var Accessory = require('../').Accessory;
var Service = require('../').Service;
var Characteristic = require('../').Characteristic;
var uuid = require('../').uuid;

// here's a fake temperature sensor device that we'll expose to HomeKit
var HOME_TEMP_SENSOR = {
    
getTemperature: function() {
    console.log("Getting the current temperature!");
    return parseFloat(Temperature);
},
randomizeTemperature: function() {
    // randomize temperature to a value between 0 and 100
    HOME_TEMP_SENSOR.currentTemperature = parseFloat(Temperature);;
}
}


// Generate a consistent UUID for our Temperature Sensor Accessory that will remain the same
// even when restarting our server. We use the `uuid.generate` helper function to create
// a deterministic UUID based on an arbitrary "namespace" and the string "temperature-sensor".
var sensorUUID = uuid.generate('hap-nodejs:accessories:nursery-temperature-sensor');

// This is the Accessory that we'll return to HAP-NodeJS that represents our fake lock.
var sensor = exports.accessory = new Accessory('Nursery Temperature Sensor', sensorUUID);

// Add properties for publishing (in case we're using Core.js and not BridgedCore.js)
sensor.username = "C3:5D:3A:AE:5E:1A";
sensor.pincode = "031-45-154";

// Add the actual TemperatureSensor Service.
// We can see the complete list of Services and Characteristics in `lib/gen/HomeKitTypes.js`
sensor
.addService(Service.TemperatureSensor, "Home")
.getCharacteristic(Characteristic.CurrentTemperature)
.on('get', function(callback) {
    
    // return our current value
    callback(null, HOME_TEMP_SENSOR.getTemperature());
    });

// randomize our temperature reading every 3 seconds
setInterval(function() {
            
            HOME_TEMP_SENSOR.randomizeTemperature();
            
            // update the characteristic value so interested iOS devices can get notified
            sensor
            .getService(Service.TemperatureSensor)
            .setCharacteristic(Characteristic.CurrentTemperature, HOME_TEMP_SENSOR.currentTemperature);
            
            }, 3000);
