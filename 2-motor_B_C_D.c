
// Example 2
// 0. Slow movement
// 1. Smooth turn RIGHT/LEFT
// 2. Smooth BACK-LEFT/RIGHT turn

#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>

#define IN1_PIN 1
#define IN2_PIN 4
#define IN3_PIN 5
#define IN4_PIN 6

#define TRIG_PIN 28
#define ECHO_PIN 29

#define MAX_SPEED 100
#define MIN_SPEED 0

void initUltrasonic();
int getDistance();

void initDCMotor();
void goForward();
void goBackward();
void smoothLeft();
void smoothRight();
void stopDCMotor();
void slow();
void smoothBackLeft();

int dist;

int main(void)
{
	if (wiringPiSetup() == -1)
		return 0;

	initUltrasonic();
	initDCMotor();

	while (1)
	{
		// smoothRight();
		// delay(600);

		// smoothLeft();
		// delay(800);

		dist = getDistance();
		// break;

		if (dist <= 25)
		{
			stopDCMotor();
			printf("STOP: distance is less than 25cm\n");
			delay(500);

			smoothRight();
			delay(620);

			stopDCMotor();
			delay(500);

			break;
		}
		else
		{
			goForward();
		}
	}

	return 0;
}

void initUltrasonic()
{
	pinMode(TRIG_PIN, OUTPUT);
	pinMode(ECHO_PIN, INPUT);
}

int getDistance()
{
	int start_time = 0, end_time = 0;
	float distance = 0;

	digitalWrite(TRIG_PIN, LOW);
	delay(500);
	digitalWrite(TRIG_PIN, HIGH);
	delayMicroseconds(10);
	digitalWrite(TRIG_PIN, LOW);

	while (digitalRead(ECHO_PIN) == 0)
		;
	start_time = micros();

	while (digitalRead(ECHO_PIN) == 1)
		;
	end_time = micros();

	distance = (end_time - start_time) / 29. / 2.;

	return (int)distance;
}

void initDCMotor()
{
	pinMode(IN1_PIN, SOFT_PWM_OUTPUT);
	pinMode(IN2_PIN, SOFT_PWM_OUTPUT);
	pinMode(IN3_PIN, SOFT_PWM_OUTPUT);
	pinMode(IN4_PIN, SOFT_PWM_OUTPUT);

	softPwmCreate(IN1_PIN, MIN_SPEED, MAX_SPEED);
	softPwmCreate(IN2_PIN, MIN_SPEED, MAX_SPEED);
	softPwmCreate(IN3_PIN, MIN_SPEED, MAX_SPEED);
	softPwmCreate(IN4_PIN, MIN_SPEED, MAX_SPEED);
}

void slow()
{
	softPwmWrite(IN1_PIN, 30);
	softPwmWrite(IN2_PIN, MIN_SPEED);
	softPwmWrite(IN3_PIN, 30);
	softPwmWrite(IN4_PIN, MIN_SPEED);
}

void goForward()
{
	softPwmWrite(IN1_PIN, 77);
	softPwmWrite(IN2_PIN, MIN_SPEED);
	softPwmWrite(IN3_PIN, 100);
	softPwmWrite(IN4_PIN, MIN_SPEED);
}

void goBackward()
{
	softPwmWrite(IN1_PIN, MIN_SPEED);
	softPwmWrite(IN2_PIN, MAX_SPEED);
	softPwmWrite(IN3_PIN, MIN_SPEED);
	softPwmWrite(IN4_PIN, MAX_SPEED);
}

void smoothLeft()
{
	softPwmWrite(IN1_PIN, MAX_SPEED / 8);
	softPwmWrite(IN2_PIN, MIN_SPEED);
	softPwmWrite(IN3_PIN, MAX_SPEED);
	softPwmWrite(IN4_PIN, MIN_SPEED);
}

void smoothRight()
{
	softPwmWrite(IN1_PIN, MAX_SPEED);
	softPwmWrite(IN2_PIN, MIN_SPEED);
	softPwmWrite(IN3_PIN, MAX_SPEED / 8);
	softPwmWrite(IN4_PIN, MIN_SPEED);
}

void smoothBackLeft()
{
	softPwmWrite(IN1_PIN, MIN_SPEED);
	softPwmWrite(IN2_PIN, MAX_SPEED / 8);
	softPwmWrite(IN3_PIN, MIN_SPEED);
	softPwmWrite(IN4_PIN, MAX_SPEED);
}

void smoothBackRight()
{
	softPwmWrite(IN1_PIN, MIN_SPEED);
	softPwmWrite(IN2_PIN, MAX_SPEED);
	softPwmWrite(IN3_PIN, MIN_SPEED);
	softPwmWrite(IN4_PIN, MAX_SPEED / 8);
}

void stopDCMotor()
{
	softPwmWrite(IN1_PIN, MIN_SPEED);
	softPwmWrite(IN2_PIN, MIN_SPEED);
	softPwmWrite(IN3_PIN, MIN_SPEED);
	softPwmWrite(IN4_PIN, MIN_SPEED);
	// printf("Stop\n");
}

// gcc 2-motor_C_D.c -o motorcd -lwiringPi
//  ./motorcd