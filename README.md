# selfdrivutt
Self-Driving software for an Raspberry-based autonomous mini-car.

### Objectives 
- Lines detections
- Traffics signs detections
- Obstacles avoidance
- Neural Network algorithm
- Perfect remote control
- Build a complete system to develop and analyse the autonomous driving.
  - Remote "live" monitoring
  - Open Dataset for detections

### Hardware
- The motors I2C card:
https://www.robot-electronics.co.uk/htm/md25tech.htm

- The I2C sensors: 
https://www.robot-electronics.co.uk/htm/srf08tech.html

Theses pieces are really expensive. The next step is too rebuild the car from low-price tech. (Cause I only had the robot for less than 2 weeks =/)

### Structure
- `ai/` - the code for all the intelligent system. lines/signs detection, neural networks
- `robot/` - the code relative to the robot
  - `cloud/` - really bad name for the computer/server that remotely control or monitor the robot
  - `raspberry/` - all the raspberry code, control the robot, communicate with the remote server and use ai librairy to perform automatic action.

### TODO

As stated in the "hardware" sections, the next step is to rebuild the robot using low cost items. Then, an effort will be made to install and use the software.


