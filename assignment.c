#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>

#define IN1_PIN 1
#define IN2_PIN 4
#define IN3_PIN 5
#define IN4_PIN 6

#define TRIG_PIN 28
#define ECHO_PIN 29

#define LEFT_IR_PIN 26
#define RIGHT_IR_PIN 27

#define MAX_SPEED 100
#define MIN_SPEED 0

void initUltrasonic();
int getDistance();

void initIR();

void initDCMotor();
void goForward();
void smoothLeft();
void smoothRight();
void stopDCMotor();

int dist;

int main(void)
{
    if (wiringPiSetup() == -1)
        return 0;

    int LValue, RValue;
    initIR();

    initUltrasonic();
    initDCMotor();

    LValue = digitalRead(LEFT_IR_PIN);
    RValue = digitalRead(RIGHT_IR_PIN);

    int numOfCycles = 0;
    int isFinished = 0;

    while (isFinished == 0)
    {
        // break;
        dist = getDistance();

        LValue = digitalRead(LEFT_IR_PIN);
        RValue = digitalRead(RIGHT_IR_PIN);

        if (dist <= 28)
        {
            numOfCycles++;

            stopDCMotor();
            delay(500);

            while (LValue == 1)
            {
                smoothRight();
                LValue = digitalRead(LEFT_IR_PIN);
            }

            stopDCMotor();
            delay(500);
            LValue = digitalRead(LEFT_IR_PIN);

            while (LValue == 0)
            {
                goForward();
                LValue = digitalRead(LEFT_IR_PIN);
            }
            stopDCMotor();
            delay(500);

            // break;
            switch (numOfCycles)
            {
            case 1:
                goForward();
                delay(500);

                stopDCMotor();
                delay(500);

                smoothLeft();
                delay(600);

                stopDCMotor();
                delay(500);
                break;

            case 2:
            case 3:
            case 4:
                smoothLeft();
                delay(625);

                stopDCMotor();
                delay(500);

                goForward();
                delay(1950);

                stopDCMotor();
                delay(500);

                smoothLeft();
                delay(650);

                stopDCMotor();
                delay(500);
                break;

            case 5:
                goForward();
                delay(2050);

                stopDCMotor();
                delay(500);
                isFinished = 1;
                break;

            default:
                stopDCMotor();
                delay(5000);
                break;
            }
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
    softPwmWrite(IN1_PIN, 55);
    softPwmWrite(IN2_PIN, MIN_SPEED);
    softPwmWrite(IN3_PIN, 70);
    softPwmWrite(IN4_PIN, MIN_SPEED);
}
