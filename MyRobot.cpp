#include "WPILib.h"

#include "WPILib.h"
#include "EGamepad.h"
#include "EJoystick.h"
#include "ERobotDrive.h"
#include "Autonomous.h"

#include "BasicDefines.h"

/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 
class RobotDemo : public SimpleRobot
{
	LEFT_DRIVE_MOTOR leftFrontDrive, leftRearDrive;
	RIGHT_DRIVE_MOTOR rightFrontDrive, rightRearDrive;
	CENTER_DRIVE_MOTOR leftCenterDrive, rightCenterDrive;
	ERobotDrive myRobot; // robot drive system
	EJoystick stick; // only joystick
	EGamepad gamepad;
	Encoder leftEncoder;
	Encoder rightEncoder;
	AutonomousHelper auton;

public:
	RobotDemo():
		leftFrontDrive(LEFT_FRONT_DRIVE_PWM),
		leftRearDrive(LEFT_REAR_DRIVE_PWM),
		rightFrontDrive(RIGHT_FRONT_DRIVE_PWM),
		rightRearDrive(RIGHT_REAR_DRIVE_PWM),
		leftCenterDrive(CENTER_LEFT_DRIVE_PWM),
		rightCenterDrive(CENTER_RIGHT_DRIVE_PWM),
		myRobot(leftFrontDrive, leftRearDrive, 
				leftCenterDrive, rightCenterDrive, rightFrontDrive, rightRearDrive),	// these must be initialized in the same order
		stick(1),		// as they are declared above.
		gamepad(3),
		leftEncoder(2, 3),
		rightEncoder(4, 5),
		auton(&myRobot, &leftEncoder, &rightEncoder, &gamepad, &stick)
	{
		myRobot.SetExpiration(0.1);
	}

	/**
	 * Drive left & right motors for 2 seconds then stop
	 */
	void Autonomous()
	{
		myRobot.SetSafetyEnabled(false);
		myRobot.Drive(-0.5, 0.0); 	// drive forwards half speed
		Wait(2.0); 				//    for 2 seconds
		myRobot.Drive(0.0, 0.0); 	// stop robot
	}

	/**
	 * Runs the motors with arcade steering. 
	 */
	void OperatorControl()
	{
		myRobot.SetSafetyEnabled(true);
		while (IsOperatorControl())
		{
			myRobot.ArcadeDrive(stick); // drive with arcade style (use right stick)
			Wait(0.005);				// wait for a motor update time
		}
	}
	
	/**
	 * Runs during test mode
	 */
	void Test() {
		auton.Calibrate();
	}
};

START_ROBOT_CLASS(RobotDemo);

