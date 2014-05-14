#ifndef AUTONOMOUS_H_
#define AUTONOMOUS_H_

#include "WPILib.h"
#include "EGamepad.h"

#define BUTTON_CALIBRATE 1
// TODO: Look at controllers to see what the front left and right triggers are
#define BUTTON_SETLEFT 7
#define BUTTON_SETRIGHT 8

#define GAMEPAD_DEFAULT_PORT 3
/* Right joystick on most of our robots */
#define JOYSTICK_DEFAULT_PORT 1

#define NUM_FT 5
#define CALIBRATION_TOLERANCE 100

#define AUTO_CONSTANT_FILE "auto_const"

/* class MoveStep { */
/* public: */
/*   MoveStep(); */
/*   virtual ~MoveStep(); */

/* private: */
/*   int left_ft; */
/*   int right_ft; */
  
/* }; */

class Autonomous {
public:
  Autonomous(RobotDrive *rbt, Encoder *left, Encoder *right,
	     EGamepad *gamepad=0, EJoystick *joystick=0);

  // Runs a standard calibration routine.
  void Calibrate();

  /* Drives forwards or backwards a given number of feet based on the calibration data.
   * This will fail if calibration has not occurred.
   * 
   * param ft: Number of feet to drive forward.
   */
  void SimpleDrive(double ft, float speed=1.0);

private:
  void PrintCalibrationDisplay(double distance, char enc);
  void PrintCalibrationFinishedDisplay(double constant);
  void PrintInstructions();
  void ResetEncoders();
  void StartEncoders();
  void StopEncoders();

  RobotDrive *m_rbt;
  Encoder *left, *right;
  char left_sign, right_sign;
  DriverStationLCD *dsLCD;
  EGamepad *gamepad;
  EJoystick *joystick;
  double constant;
};

#endif // AUTONOMOUS_H_
