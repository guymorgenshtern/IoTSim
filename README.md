# IoTSim
SYSC4001 Assignment 1
#README

Internet of Things Simulation:
Designed and Implemented in Ottawa, Ontario by Guy Morgenshtern

About:
The system simulates how an internet of things would function. It's composed of sensor, actuators, a controller, and a cloud. The sensors constantly read values from the "environment" and once connected to a Controller, begin sending it those values every second. The Controller is responsible for reading the sensor values, determining if they are above a set threshold, and once the treshold is surpassed, send a message to both Accuator (emulates a trigger) and the Cloud (holds information about what is going on in the system).

Design choices:
- Sensor continuously increment values from when they are initialized to when the user closes the process (even after a threshold is reached). This is to emulate how a sensor would be used in real life.
- All processes that rely on another to write to a pipe in order to read data wait a specified amount of time for that device to connect. Once connected, they start normal behaviour
- Sensor continues incrementing by one after reaching the threshold value. In the real world the sensor value would be read from the environment and would change accordingly. In this simulation there is no pipe between Actuator and Sensor so currently there is no way for a Sensor to know if an Actuator was triggered

How to run the system:
- Run script provided in project folder to compile each file and open terminals (./script)
- *IMPORTANT Run each device in this order, immediately after one another
	- Cloud
	- Actuator
	- Controller
	- Sensor #1
	- Sensor #2
- Specific initialization steps are defined for each device below

Overview:

Cloud:
- HOW TO RUN: ./Cloud
- This device needs no additional parameters
- Establishes connection to Controller (read)
- Once ran, the Cloud waits for a specified amount of time for Controller to connect
- Reads message from Controller that says an Actuator has been triggered

Actuator:
- HOW TO RUN: ./Actuator "DEVICE_NAME"
- Establishes connection to Controller (read)
- Once ran, the Actuator waits for a specified amount of time for Controller to connect

Controller:
- HOW TO RUN: ./Controller
- This device needs no additional parameters
- Establishes connection to Sensor (read), Accuator (write), and Cloud (write)
- Once ran, the Controller waits for a specified amount of time for a Sensor to connect
- Once one sensor connects up to 4 more sensors can also be connected (for a total of 5)
- Reads sensor values and once a device's specified threshold is reached, the Controller sends a message to Accuator (through the child process) and a message to Cloud (through the parent process)

Sensor:
- HOW TO RUN: ./Sensor "DEVICE_NAME" "THRESHOLD"
- Initializes at a random value
- Begins "reading" values from environment (in this case, incrementing value by one)
- Once connected to Controller, begins sending updated value to Controller ever second

