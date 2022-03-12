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
void initLineTacer();

void initDCMotor();
void goForward();
void goBackward();
void smoothLeft();
void smoothRight();
void stopDCMotor();
void avoidObstacleFromRight();

int dist;
int LValue, RValue;

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

    int isStopped = 0;
    int isDoneWithBox2 = 0;
    int numOfBoxes = 0;
    int isFinished = 0;

    while (isFinished == 0)
    {
        // break;
        leftTracer = digitalRead(LEFT_TRACER_PIN);
        rightTracer = digitalRead(RIGHT_TRACER_PIN);

        printf("leftTracer: %d rightTracer: %d\n", leftTracer, rightTracer);

        while (leftTracer == 1 && rightTracer == 1)
        {
            dist = getDistance();
            printf("\ndist: %d\n", dist);
            printf("\nnumOfBoxes: %d\n", numOfBoxes);

            if (dist <= 20 && dist != 0)
            {
                printf("numOfBoxes: %d\n", numOfBoxes);
                switch (numOfBoxes)
                {
                case 2:
                    stopDCMotor();
                    delay(100);

                    LValue = digitalRead(LEFT_IR_PIN);

                    // Turn right until IR sensor detect the obstacle from Left
                    while (LValue == 1)
                    {
                        smoothRight();
                        LValue = digitalRead(LEFT_IR_PIN);
                    }
                    stopDCMotor();
                    delay(100);

                    smoothLeft();
                    delay(400);

                    stopDCMotor();
                    delay(100);

                    goBackward();
                    delay(300);

                    stopDCMotor();
                    delay(100);

                    rightTracer = digitalRead(RIGHT_TRACER_PIN);
                    while (rightTracer == 1)
                    {
                        goForward();
                        rightTracer = digitalRead(RIGHT_TRACER_PIN);
                    }
                    stopDCMotor();
                    delay(100);

                    while (rightTracer == 0)
                    {
                        smoothLeft();
                        rightTracer = digitalRead(RIGHT_TRACER_PIN);
                    }
                    stopDCMotor();
                    delay(100);

                    goForward();
                    delay(300);

                    smoothLeft();
                    delay(300);

                    numOfBoxes = 3;
                    printf("In case 2\n");
                    break;

                case 3:
                    printf("In case 3\n");
                    stopDCMotor();
                    delay(1000);
                    isFinished = 1;
                    break;

                default:
                    numOfBoxes = 1;
                    isStopped = 1;
                    printf("If stopped\n");
                    stopDCMotor();
                    delay(1000);
                    break;
                }
            }
            else
            {
                goForward();
            }

            leftTracer = digitalRead(LEFT_TRACER_PIN);
            rightTracer = digitalRead(RIGHT_TRACER_PIN);
            if (isFinished == 1)
            {
                leftTracer = NULL;
                rightTracer = NULL;
            }
        }
        stopDCMotor();
        delay(100);

        leftTracer = digitalRead(LEFT_TRACER_PIN);
        printf("leftTracer: %d\n", leftTracer);

        while (leftTracer == 0)
        {
            smoothRight();
            // TODO: uncomment if you start from the begining
            // if (isStopped == 1 && numOfBoxes == 1)
            // {
            //     numOfBoxes = 2;
            // }
            leftTracer = digitalRead(LEFT_TRACER_PIN);
        }
        stopDCMotor();
        delay(100);

        rightTracer = digitalRead(RIGHT_TRACER_PIN);
        printf("rightTracer: %d\n", rightTracer);

        while (rightTracer == 0)
        {
            smoothLeft();
            if (isStopped == 1 && numOfBoxes == 1)
            {
                numOfBoxes = 2;
            }
            rightTracer = digitalRead(RIGHT_TRACER_PIN);
        }
        stopDCMotor();
        delay(100);

        leftTracer = digitalRead(LEFT_TRACER_PIN);
        rightTracer = digitalRead(RIGHT_TRACER_PIN);

        printf("\nLoop finshed\n");
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
    int loop_start = 0;
    int start_time = 0, end_time = 0;
    float distance = 0;

    digitalWrite(TRIG_PIN, LOW);

    delayMicroseconds(2);
    // delay(10);

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    loop_start = micros();
    while (digitalRead(ECHO_PIN) == 0)
    {
        if (micros() - loop_start >= 1000)
            break;
    }
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
}

void goForward()
{
    softPwmWrite(IN1_PIN, 45);
    softPwmWrite(IN2_PIN, MIN_SPEED);
    softPwmWrite(IN3_PIN, 60);
    softPwmWrite(IN4_PIN, MIN_SPEED);
}

void goBackward()
{
    softPwmWrite(IN1_PIN, MIN_SPEED);
    softPwmWrite(IN2_PIN, MAX_SPEED);
    softPwmWrite(IN3_PIN, MIN_SPEED);
    softPwmWrite(IN4_PIN, MAX_SPEED);
}
