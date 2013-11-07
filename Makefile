OBJS = Robot.o Ev3.o Xg1300lGyro.o Odometry.o InputKeys.o Keyboard.o MathFunctions.o
CC = arm-none-linux-gnueabi-g++
CFLAGS = -g -Wall -static 
TARGET = main

all: $(TARGET)
$(TARGET): $(OBJS) main.cpp
	$(CC) $(CFLAGS) $(OBJS) main.cpp -o $@ 
Robot.o: Robot.cpp Robot.h
	$(CC) $(CFLAGS) -c Robot.cpp 
Ev3.o: Ev3.cpp Ev3.h Robot.o MathFunctions.o
	$(CC) $(CFLAGS) -c Ev3.cpp
Xg1300lGyro.o: Xg1300lGyro.cpp Xg1300lGyro.h Ev3.o MathFunctions.o
	$(CC) $(CFLAGS) -c Xg1300lGyro.cpp
Odometry.o: Odometry.cpp Odometry.h MathFunctions.o
	$(CC) $(CFLAGS) -c Odometry.cpp
InputKeys.o: InputKeys.cpp InputKeys.h
	$(CC) $(CFLAGS) -c InputKeys.cpp
Keyboard.o: Keyboard.cpp Keyboard.h InputKeys.o
	$(CC) $(CFLAGS) -c Keyboard.cpp
MathFunctions.o: MathFunctions.cpp MathFunctions.h
	$(CC) $(CFLAGS) -c MathFunctions.cpp
clean:
	\rm *.o $(TARGET) 
