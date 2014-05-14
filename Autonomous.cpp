#include "Autonomous.h"

#include "EGamepad.h"
#include "EJoystick.h"
#include "ERobotDrive.h"

#include "WPILib.h"

#include <stdio.h>

AutonomousHelper::AutonomousHelper(ERobotDrive *rbt, Encoder *left, Encoder *right, EGamepad *gamepad, EJoystick *joystick) {
  this->rbt = rbt;
  this->left = left;
  this->right = right;

  this->dsLCD = DriverStationLCD::GetInstance();

  this->gamepad = (gamepad == 0 ? gamepad : (new EGamepad(GAMEPAD_DEFAULT_PORT)));
  this->joystick = (joystick == 0 ? joystick : (new EJoystick(JOYSTICK_DEFAULT_PORT)));
  
  FILE *out = fopen(AUTO_CONSTANT_FILE, "r");

  if (out == NULL) {
    this->constant = 0.0;
  }
  else {
    fscanf(out, "%c%c%f", &left_sign, &right_sign, &constant);

    fclose(out);

    this->ResetEncoders();
  }
}

void AutonomousHelper::PrintInstructions() {
  dsLCD->PrintfLine(DriverStationLCD::kUser_Line1, "DS Calibration");
  dsLCD->PrintfLine(DriverStationLCD::kUser_Line3, "Drive 5ft fwd");
  dsLCD->PrintfLine(DriverStationLCD::kUser_Line4, "Press btn 5/x");
}

void AutonomousHelper::ResetEncoders() {
  this->left->SetDistancePerPulse(constant);
  this->right->SetDistancePerPulse(constant);

  if (left_sign == '-') {
    this->left->SetReverseDirection(true);
  }
  if (right_sign == '-') {
    this->right->SetReverseDirection(true);
  }

  this->left->Reset();
  this->right->Reset();
}

void AutonomousHelper::PrintCalibrationFinishedDisplay() {
  dsLCD->Clear();

  this->PrintInstructions();

  dsLCD->PrintfLine(DriverStationLCD::kUser_Line5, "Constant: %f", constant);

  dsLCD->UpdateLCD();
}

void AutonomousHelper::PrintCalibrationDisplay(double distance, char enc) {
  dsLCD->Clear();

  this->PrintInstructions();

  // Information.
  dsLCD->PrintfLine(DriverStationLCD::kUser_Line4, "Enc: %c", enc);
  dsLCD->PrintfLine(DriverStationLCD::kUser_Line5, "Dist: %f", distance);

  dsLCD->UpdateLCD();
}

double d_abs(double b) {
  return b >= 0.0 ? b : -b;
}

int c_abs(int b) {
	return b >= 0 ? b : -b;
}

int c_max(int a, int b) {
  return a > b ? a : b;
}

void AutonomousHelper::Calibrate() {
  char encoder = 'L';
  int left_reading = 0;
  int right_reading = 0;

  this->ResetEncoders();
  
  this->StartEncoders();
  
  this->PrintCalibrationDisplay(left_reading, encoder);

  gamepad->EnableButton(BUTTON_CALIBRATE);

  // Ensure no false positives by making sure state is initialized for the button.
  gamepad->Update();
  gamepad->Update();

  while (gamepad->GetEvent(BUTTON_CALIBRATE) != kEventOpened) {
    rbt->ArcadeDrive(joystick);
    
    left_reading = left->Get();
    right_reading = right->Get();

    this->PrintCalibrationDisplay(c_max(c_abs(left_reading), c_abs(right_reading)), encoder);
    gamepad->Update();
  }

  if (c_abs(left_reading - right_reading) > CALIBRATION_TOLERANCE) {
    dsLCD->PrintfLine(DriverStationLCD::kUser_Line4, "Enc readings don't match!!");
    dsLCD->UpdateLCD();
  }

  /* 
   * To get the constant to use in the future, we divide 
   * the number of feet traveled by the number of encoder
   * pulses. This provides a constant suitable for the 
   * SetDistancePerPulse method of the Encoder class.
   */
  this->constant = (double)NUM_FT / c_max(c_abs(left_reading), c_abs(right_reading));
  this->left_sign = left_reading >= 0 ? '+' : '-';
  this->right_sign = right_reading >= 0 ? '+' : '-';

  this->PrintCalibrationFinishedDisplay();
  dsLCD->UpdateLCD();

  this->ResetEncoders();
  this->StopEncoders();

  FILE *in = fopen(AUTO_CONSTANT_FILE, "w");
  
  fprintf(in, "%c%c%f", left_sign, right_sign, constant);

  fclose(in);
}

void AutonomousHelper::StopEncoders() {
  this->left->Stop();
  this->right->Stop();
}

void AutonomousHelper::StartEncoders() {
  this->left->Start();
  this->right->Start();
}

void AutonomousHelper::SimpleDrive(double ft, float speed) {
  this->ResetEncoders();

  this->StartEncoders();

  double reading = 0;

  double dist = d_abs(ft);

  rbt->Drive(speed, 0.0);

  while (reading < dist) {
    reading = d_abs(left->GetDistance());
  }

  rbt->Drive(0.0, 0.0);

  this->StopEncoders();
}
