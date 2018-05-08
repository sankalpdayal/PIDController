# CarND-Controls-PID
Self-Driving Car Engineer Nanodegree Program

---


## Project Introduction
The aim of this project is to find ideal values of a PID controller to navigate a car in a simulator. For a successful completion, the car should not go off track and should be able to complete at least a lap. The input to the controller is 
only the cross track error and the output is the steering for the car.

#### Submission
This repository contains all the code needed to complete the PID controller project

## Running the Code
This project involves the Term 2 Simulator which can be downloaded [here](https://github.com/udacity/self-driving-car-sim/releases). 
My setup is done using windows with docker installed. The remaining setup was done as a linux system.The main protcol is that main.cpp uses for uWebSocketIO in communicating with the simulator.
1. Compile: `cmake .. && make`
2. Run it: `./pid`. 

Tips for setting up the environment can be found [here](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/0949fca6-b379-42af-a919-ee50aa304e6a/lessons/f758c44c-5e40-4e01-93b5-1a82aa4e044f/concepts/23d376c7-0195-4276-bdf0-e02f1f3c665d)

## Concept
The idea is to first implement a PID controller based on the cross track error. Then to find the coefficients for the control values, start with all zeros and progress with each run in direction of good values. This progression towards good values is done using the Twiddle method.
The idea of good values depends on following criterion

1. The duration for which the car remained on track.
2. The average cross track error while the car was on track.
3. The speed of the car.

There is a hierarchy of importance for these criterion. In the start when the car is not able to even stay on track, the car first maximizes the duration.
Once a good enough duration is achieved, then the car has learnt to stay on track, so next step is to minimize the average cross track error. Once the average 
cross track error has also become low, next step is to maximize the speed. Hence the error function is built in a hierarchial manner.


## Implementation
The implementation can be divided in 4 broad categories; Impelementation of the PID controller,Twiddle algorithm, Run Error Computation and Error Comparison. 

### PID Controller
PID contoller is implemented as separate class. At start of a run, PID class object is first initialized with the gains for PID. During the run, for every timestep, cross track error is used to update the PID errors and then using the updated PID errors steering angle is calculated.
The methods for PID controller is inside PID.h and PID.cpp. 

### Twiddle algorithm
Twiddle is implemented as a seprate class. PID is a variable of Twiddle class. Twiddle is first initialized by reading the last stored state from a text file. This loads the last best values for PID gains. Using these, PID is initalized.
During the run, at every time stamp twiddle accumulates error. Also it checks if the run is to be terminated. In case the run is terminated, depending on the values of error for current run and last best run, it decides how the state of 
Twiddle needs to be updated. If gains are updated then it stores the latest state. Then Twiddle intializes PID with the current gains, resets run error to zero and starts a new run. The twiddle algorithm state has information about current
values of gains, current value of change in gains, index of parameter that is being tested for and index of the condition in which currently algorithm is. 

### Run Error Computation. 
Error update is implemented as a method inside the twiddle class. Error function tracks 5 parameters

1. Duration of run.
2. Duration for which the car remained on track. This is duration when CTE was less than accepted CTE and speed was more than minimum speed.
3. Sum of absoulte cross track error while the car was on track, to be used to get average cross track error.
4. Sum of speed of car while the car was on track, to be used to get average speed.
5. Duration for which car was static. This is duration for which car was less than minimum speed.

Error update function is also responsible for deciding if the run needs to be terminated. This can happen if either duration as gone beyond target duration or if car has been stationary for a prolonged time. 

### Error Comparison
This comparison is done only when a run terminates and hence the error from current run has to be checked against the last best. The implementation follows the hierarchy of importance. First it checks if duration on track is more. If yes, the current run is better. If duration is same or equal to max duration then
it checks for the next criterion which is average CTE. If this CTE is it is below a certain threshold. If not but it is still better than previous best avg CTE, if yes, the current run is better. If this CTE is below a certain threshold and previous best was also below this threshold, then it checks for average speed.
If average speed is more then the current run is better.
 
## Code Modifications
Following modifications were done in the starter code.
1. `PID.h`
	1. Added variables to store pid error values and last CTE and boolean `if_init` to track initialization.
2. `PID.cpp`
	1. Implementated initialization by setting the gains as the input values obtained during init and initializing error values to zero.
	2. Implemented update error function that takes CTE as input ans updates PID errors and last CTE.
	3. Implemented total error function that used the last PID error values and gains to estimate the steering value.
3. `Twiddle.h`
	1. Created a new header file for Twiddle class.
	2. Created variables for PID, values of gains, change in gains, iterations, error values, best error values, target duration and indexes to store which parameter is being updated and what condition is being tested.
	3. Function definitions for Init, Update error, update gains, restting error, checking error against last best and reading and writing parameters.
4. `Twiddle.cpp`
	1. Created a new cpp file for Twiddle class methods.
	2. Implementated intialization where it first tries to read state of algorithm from a file and if unsuccessful then it intializes the algorithm with gains as 0 and change in gains as 1.
	3. Implemented Update Run Error function to track variables to be used in error computation and also checks if the run needs to reset.
	4. Implemented reset run error function that resets all the error variables to 0.
	5. Implemented error check function that performs the error comparison between last best and current using the hierarchy method.
	6. Implemented update function that updates the state of algorithm depending on the output of the error comparison.
	7. Implemented methods to read and write state of algorithm in a text file.
5. `main.cpp`
	1. Removed inclusion of `PID.h` and included `Twiddle.cpp`
	2. Implemented initialization of Twiddle and PID variable of twiddle class object.
	3. Updated address in `h.onMessage` funtion to `twiddle` from `pid`.
	4. Added call to methods to update error of PID and run error for twiddle.
	5. Added call to methods to get steering value from PID.
	6. Implemented method to check if the run needs to reset and if yes, added calls to methods to update gains, reset run error of twiddle, initialization of PID, and sending reset command to simulator.
6. `CMakeLists.txt`
	1. To include Twiddle.cpp during compilation, updated `set()` to `set(sources src/PID.cpp src/main.cpp src/Twiddle.cpp)`.

## Logging
The state of the twiddle algorithm can be defined by the following values

1. Gains for PID
2. Change in gains of PID
3. Iteration
4. Index of gain currently being updated.
5. Index of condition currently being tested.
6. Best error values for duration on track, average CTE and average speed.
7. Target duration.

All these values are stored in a text file `Debug.txt` which are read whenever next time Twiddle intializes and appeded whenever Twiddle algorithm finds a run better than previous best.  

## Evolution of PID Parameters with change of Error Function
In the start of twiddle algorithm for very first time, the car finds it very difficult to stay on track. The error function is kept to maximise the on track duration.
Slowly the car starts learning to stay on track as the parameters get updated.
Whenever the car is able to achieve target duration, the target duration is doubled. This happens till target duration is large enough to cover at least one lap. At this stage the car is 
able to completed lap without getting off track for a long time. Hence target duration is not increased further and the error function now is changed to reduce average CTE. Once the 
average CTE also becomes low enough then the emphasis is done on maximizing speed. Hence then the error function is changed to maximize speed.

## Results and Observations
After running the twiddle algorithm for considerably long time, the twiddle had updated parameters that enabled the car to remain on track very effectively. Following video shows how the car navigated with the final chosen parameters. 

[![Results of PID Run](http://img.youtube.com/vi/hpGvvLTV8gA/0.jpg)](http://www.youtube.com/watch?v=hpGvvLTV8gA)

It can be observed that the car was able to remain on track thoughout the run.

## Scope of improvement
It can observed that car has learnt to remain at low speed by wiggling slowly. In this way car is easily able to stay on track and hence is able to satisfy the error condition. But it is not a 
natural way of driving. Hence a better way to implemented the error function can be thought of. Other idea is to use the vehicle's parameters like turning radius and the expected trajectory of 
coming path in deciding the steering value.
