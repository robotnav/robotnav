OBJS = Robot.o Ev3.o Xg1300lGyro.o LegoGyro.o Odometry.o Control.o InputKeys.o Keyboard.o IrRemote.o Buttons.o MathFunctions.o
CC = arm-none-linux-gnueabi-g++
CFLAGS = -Wall -static
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
LegoGyro.o: LegoGyro.cpp LegoGyro.h Ev3.o MathFunctions.o
	$(CC) $(CFLAGS) -c LegoGyro.cpp
Odometry.o: Odometry.cpp Odometry.h MathFunctions.o
	$(CC) $(CFLAGS) -c Odometry.cpp
Control.o: Control.cpp Control.h MathFunctions.o Odometry.o
	$(CC) $(CFLAGS) -c Control.cpp
InputKeys.o: InputKeys.cpp InputKeys.h
	$(CC) $(CFLAGS) -c InputKeys.cpp
Keyboard.o: Keyboard.cpp Keyboard.h InputKeys.o
	$(CC) $(CFLAGS) -c Keyboard.cpp
Buttons.o: Buttons.cpp Buttons.h InputKeys.o
	$(CC) $(CFLAGS) -c Buttons.cpp
IrRemote.o: IrRemote.cpp IrRemote.h InputKeys.o
	$(CC) $(CFLAGS) -c IrRemote.cpp
MathFunctions.o: MathFunctions.cpp MathFunctions.h
	$(CC) $(CFLAGS) -c MathFunctions.cpp
clean:
	\rm *.o $(TARGET) 
