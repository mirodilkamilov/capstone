#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>

// DC Motor pins
#define IN1_PIN 1
#define IN2_PIN 4
#define IN3_PIN 5
#define IN4_PIN 6

#define MAX_SPEED 100
#define MIN_SPEED 0

// Ultrasonic pins
#define TRIG_PIN 28
#define ECHO_PIN 29

// IR sensor pins
#define LEFT_IR_PIN 26
#define RIGHT_IR_PIN 27
#define LEFT_TRACER_PIN 10
#define RIGHT_TRACER_PIN 11
void initUltrasonic();
int getDistance();

void initIR();

void initDCMotor();
void goForward();
void smoothLeft();
void smoothRight();
void stopDCMotor();
void avoidObstacleFromRight();

int dist;

int main(void)
{
    if (wiringPiSetup() == -1)
        return 0;

    initIR();

    initUltrasonic();
    initDCMotor();
    initLineTacer();

    int leftTracer;
    int rightTracer;

    int numOfBoxes = 0;
    int isFinished = 0;

    while (isFinished == 0)
    {
        // break;
        leftTracer = digitalRead(LEFT_TRACER_PIN);
        rightTracer = digitalRead(RIGHT_TRACER_PIN);

        while (leftTracer == 1 && rightTracer == 1)
        {
            leftTracer = digitalRead(LEFT_TRACER_PIN);
            rightTracer = digitalRead(RIGHT_TRACER_PIN);
            goForward();
        }
        stopDCMotor();
        delay(100);
        leftTracer = digitalRead(LEFT_TRACER_PIN);

        while (leftTracer == 0)
        {
            smoothRight();
            leftTracer = digitalRead(LEFT_TRACER_PIN);
        }
        stopDCMotor();
        delay(100);
        rightTracer = digitalRead(RIGHT_TRACER_PIN);
        while (rightTracer == 0)
        {
            smoothLeft();
            rightTracer = digitalRead(RIGHT_TRACER_PIN);
        }
        stopDCMotor();
        delay(100);
        leftTracer = digitalRead(LEFT_TRACER_PIN);
        rightTracer = digitalRead(RIGHT_TRACER_PIN);
        if (leftTracer == 0 && rightTracer == 0)
        {
            stopDCMotor();
            delay(100);
            isFinished = 1;
        }
    }

    return 0;
}

/**
 * Avoid obstacle from right:
 * 1. Turns left (until it is paralel to obstacle)
 * 2. Goes forward (until passes by obstacle)
 */
void avoidObstacleFromRight()
{
    int LValue, RValue;

    LValue = digitalRead(LEFT_IR_PIN);
    RValue = digitalRead(RIGHT_IR_PIN);

    // Turn right until IR sensor detect the obstacle from Left
    while (LValue == 1)
    {
        smoothRight();
        LValue = digitalRead(LEFT_IR_PIN);
    }

    stopDCMotor();
    delay(500);
    LValue = digitalRead(LEFT_IR_PIN);

    // Go forward until IR sensor stops detecting the obstacle from Left
    while (LValue == 0)
    {
        goForward();
        LValue = digitalRead(LEFT_IR_PIN);
    }

    stopDCMotor();
    delay(500);
}

void initUltrasonic()
{
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}
void initLineTacer()
{
    pinMode(LEFT_TRACER_PIN, INPUT);
    pinMode(RIGHT_TRACER_PIN, INPUT);
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

void initIR()
{
    pinMode(LEFT_IR_PIN, INPUT);
    pinMode(RIGHT_IR_PIN, INPUT);
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

void smoothRight()
{
    softPwmWrite(IN1_PIN, MAX_SPEED);
    softPwmWrite(IN2_PIN, MIN_SPEED);
    softPwmWrite(IN3_PIN, MAX_SPEED / 8);
    softPwmWrite(IN4_PIN, MIN_SPEED);
}

void smoothLeft()
{
    softPwmWrite(IN1_PIN, MAX_SPEED / 8);
    softPwmWrite(IN2_PIN, MIN_SPEED);
    softPwmWrite(IN3_PIN, MAX_SPEED);
    softPwmWrite(IN4_PIN, MIN_SPEED);
}

void stopDCMotor()
{
    softPwmWrite(IN1_PIN, MIN_SPEED);
    softPwmWrite(IN2_PIN, MIN_SPEED);
    softPwmWrite(IN3_PIN, MIN_SPEED);
    softPwmWrite(IN4_PIN, MIN_SPEED);
    // printf("Stop\n");
}

void goForward()
{
    softPwmWrite(IN1_PIN, 65);
    softPwmWrite(IN2_PIN, MIN_SPEED);
    softPwmWrite(IN3_PIN, 80);
    softPwmWrite(IN4_PIN, MIN_SPEED);
}