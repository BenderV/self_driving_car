# Self Driving Car

Mini self-driving car with raspberry pi board.
Read [the report on this project](https://olympe.quip.com/oDncAFFJdbRC)!

### Objectives 
- Lines detections
- Traffics signs detections
- Obstacles avoidance
- Neural Network algorithm
- Remote control
- Build a complete system to develop and analyse the autonomous driving.
  - Remote "live" monitoring
  - Open Dataset for detections

### Hardware

* 2 wheels platforms (with one caster)
* 1 raspberry PI with a camera and a usb wifi dongle
* 1 motors I2C card: https://www.robot-electronics.co.uk/htm/md25tech.htm
* 5 I2C Ultra sonic range sensors: https://www.robot-electronics.co.uk/htm/srf08tech.html

Note that these pieces are quite expensive for a personal project (~100$ each piece). With some works, I believe it's possible to replace them with low cost materials.

### Code organisation

![car2 1](https://cloud.githubusercontent.com/assets/2799516/17330216/3f220cd6-58c6-11e6-8dec-6ceca7c1be3b.png)

### Code Structure

* `ai/` - the code for all the intelligent system. 
    * `preprocessing.py`  - computer vision pre-processing
    * `detection.py` - lines/signs detections with haar cascade & hough lines
    * `deepq.py`  - reinforcement learning using neural networks
* `robot/` - the code relative to the robot
    * `cloud/` - computer/server that remotely control or monitor the robot
        * `remote.py` - live-stream of car sensing with detections & remote control with socket & curse
    * `raspberry/` - all the raspberry code, control the robot, communicate with the remote server and use ai library to perform automatic action.
        * `camera.py` - camera thread to have efficient record
        * `controls.py` - remote controls with socket
        * `i2c.py` - function to use I2C (get sonars input, motors functions)
        * `livestream.py` - thread to send data with socket
        * `drive.py` - car class & implement different driver (human, ai, logic based). 

The decision for this structure is that the “AI” code is independent from the robot. This allow to use it both in real time on the robot or offline on the servers. This allow for more to train more effectively neural networks and to also create a live stream of what the cars see and detect.

### Code Logic

![car 1](https://cloud.githubusercontent.com/assets/2799516/17330245/5f2b0104-58c6-11e6-9ed5-230cebf8efe6.png)

In the end, the software can be viewed as a simple loop which performs a number of tasks. The first is to fetch the last sensors informations. This task can be time consuming, so it's suggested to use parallel computing and cache system.
The second task is to live-stream the car informations. It's purely optional and intended to ease development process.
The third task is to get the driver actions. It can be a human driver or an autonomous “AI” driver. In the case of an autonomous driver, it's important to have enough hardware to support real time decision. 
Finally, the driver's actions are pass by a constraint module. It's in this module where logic constraint are apply (for example, to avoid collision with obstacles). It's also in this module where the car can inform that an action wasn't apply for X reasons. (for example, in the case of a broken vehicle).
Finally, the data capture by the cars, the driver's decision and the result of the constraint module are all register in a in-board database. The database will regularly push records to the central server.

### Todo

- Replace hardware with cheaper component (and more powerful?)
- Add kinetic position
- Upgrade detections algorithms
  - Create or found open dataset for each
  - Create a scoring system for each
- Train thee reinforcement learning algorithm

