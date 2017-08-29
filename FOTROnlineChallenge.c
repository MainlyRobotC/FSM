#pragma config(Sensor, port3,  color,          sensorVexIQ_ColorGrayscale)
#pragma config(Sensor, port4,  gyro,           sensorVexIQ_Gyro)
#pragma config(Sensor, port5,  scanner,        sensorVexIQ_Color12Color)
#pragma config(Motor,  motor1,           ,             tmotorVexIQ, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motor6,           ,             tmotorVexIQ, PIDControl, reversed, driveRight, encoder)
#pragma config(Motor,  motor10,         arm,           tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor11,         claw,          tmotorVexIQ, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//IMPORTANT!!! YOU DON'T NEED TO INTAKE, JUST DRIVE INTO A RING AND IT WILL INTAKE
// YOU DON'T NEED TO SCORE THE RING JUST POSITION THE ARM NEAR THE POST AND IT WILL SCORE
// YOU CAN JUST DRIVE INTO THE BONUS TRAY AND IT WILL KNOW THE SWITCH OVE RNO MATTER WHAT

enum State
{
	turn1,
	moveback1,
	liftBar1,
	turn2,
	moveForward1,
	turn3,
	moveForward2,
	turn4,
	moveForward3,
	liftBar2,
	turn5,
	moveback2,
	lowerBar1,
	moveForward4,
	moveback3
};

State currentState;
int count = 0;

void moveArm(State nextState, int power, int degrees)
{
	motor[arm] = power;
	if(abs(nMotorEncoder[arm]) >= degrees)
	{
		nMotorEncoder[arm] = 0;
		motor[arm] = 0;
		currentState = nextstate;
	}
}

void moveLine(int lines, int power, State nextState)
{
	motor[motor1] = power;
	motor[motor6] = power;
	if(getColorGrayscale(color) <= 50)
	{
		count++;
	}
	if(count >= lines)
	{
		motor[motor1] = 0;
		motor[motor6] = 0;
		currentState = nextState;
		count = 0;
	}
}

void lowerScore(State nextState)
{
	motor[arm] = 50;
	if(abs(nMotorEncoder[arm]) >= 200)
	{
		motor[arm] = 0;
		nMotorEncoder[arm] = 0;
		currentState = nextState;
	}
}

void upperScore(State nextState)
{
	motor[arm] = 50;
	if(abs(nMotorEncoder[arm]) >= 400)
	{
		currentState = nextState;
		nMotorEncoder[arm] = 0;
	}
}

void degrees(State nextState, int degrees, int powerl, int powerr)
{
	motor[motor1] = powerl;
	motor[motor6] = powerr;
	if(abs(nMotorEncoder[motor1]) >= degrees)
	{
		nMotorEncoder[motor1] = 0;
		motor[motor1] = 0;
		motor[motor6] = 0;
		currentState = nextState;
	}
}

void turn(State nextState, int direction, int power, int turnAngle) // left turn = 0, right turn  = 1
{

	if(direction == 1)
	{
		motor[motor1] = power;
		motor[motor6] = -1*power;
	}
	else
	{
		motor[motor1] = -1*power;
		motor[motor6] = power;
	}
	if(abs(SensorValue[gyro]) >= turnAngle)
	{
		motor[motor1] = 0;
		motor[motor6] = 0;
		currentState = nextState;
		resetGyro(gyro);
		nMotorEncoder[motor1] = 0;
	}
}

task main()
{
	setColorMode(color, colorTypeGrayscale_Reflected);
	currentState = turn1;
	nMotorEncoder[arm] = 0;
	nMotorEncoder[motor1] = 0;
	while(true)
	{
		switch(currentState)
		{
		case turn1:
			turn(moveback1, 1, 50, 90);
			break;

		case moveback1:
			degrees(liftBar1, 40, 50, 50);
			break;

		case liftBar1:
			lowerScore(turn2);
			break;

		case turn2:
			turn(moveForward1, 0, 50, 45);
			break;

		case moveForward1:
			degrees(turn3, 250, 50, 50);
			break;

		case turn3:
			turn(moveForward2, 0, 50, 90);
			break;

		case moveForward2:
			degrees(turn4, 1250, 75,75);
			break;

		case turn4:
			turn(moveForward3, 1, 50, 45);
			break;

		case moveForward3:
			degrees(liftBar2, 200, 50, 50);
			break;

		case liftBar2:
			moveArm(turn5, 50, 100);
			break;

		case turn5:
			turn(moveBack2, 0, 50, 90);
			break;

		case moveback2:
			degrees(lowerBar1, 200, -50, -50);
			break;

		case lowerBar1:
			moveArm(moveForward4, -50, 200);
			break;

		case moveForward4:
			degrees(moveback3, 175, 50, 50);
			break;
		}
	}
}
