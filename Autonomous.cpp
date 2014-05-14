#include "Autonomous.h"

#include "EGamepad.h"
#include "EJoystick.h"

#include "WPILib.h"

#include <stdio.h>

Autonomous::Autonomous(RobotDrive *rbt, Encoder *left, Encoder *right, EGamepad *gamepad) {
  this->rbt = rbt;
  this->left = left;
  this->right = right;

  this->dsLCD = DriverStationLCD::GetInstance();

  this->gamepad = (gamepad == 0 ? gamepad : (new EGamepad(GAMEPAD_DEFAULT_PORT)));
  this->joystick = (joystick == 0 ? joystick : (new EJoystick(JOYSTICK_DEFAULT_PORT)));

  // TODO: Load the calibration data from the cRIO
  Priv_SetWriteFileAllowed(1);
  
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

void Autonomous::PrintInstructions() {
  dsLCD->PrintfLine(DriverStationLCD::kUser_Line1, "DS Calibration");
  dsLCD->PrintfLine(DriverStationLCD::kUser_Line3, "Drive 5ft fwd");
  dsLCD->PrintfLine(DriverStationLCD::kUser_Line4, "Press btn 5/x");
}

void Autonomous::ResetEncoders() {
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

void Autonomous::PrintCalibrationFinishedDisplay(double constant) {
  dsLCD->Clear();

  this->PrintInstructions();

  dsLCD->PrintfLine(DriverStationLCD::kUser_Line5, "Constant: %f", constant);

  dsLCD->UpdateLCD();
}

void Autonomous::PrintCalibrationDisplay(double distance, char enc) {
  dsLCD->Clear();

  this->PrintInstructions();

  // Information.
  dsLCD->PrintfLine(DriverStationLCD::kUser_Line4, "Enc: %c", enc);
  dsLCD->PrintfLine(DriverStationLCD::kUser_Line5, "Dist: %f", distance);

  dsLCD->UpdateLCD();
}

inline void d_abs(double b) {
  return b >= 0.0 ? b : -b;
}

inline void c_abs(int b) {
  return b >= 0 ? b : -b;
}

inline void max(int a, int b) {
  return a > b ? a : b;
}

void Autonomous::Calibrate() {
  char encoder = 'L';
  int left_reading = 0;
  int right_reading = 0;

  this->ResetEncoders();
  
  this->PrintCalibrationDisplay(distance, encoder);

  gamepad->EnableButton(BUTTON_CALIBRATE);

  // Ensure no false positives by making sure state is initialized for the button.
  gamepad->GetEvent(BUTTON_CALIBRATE);
  gamepad->GetEvent(BUTTON_CALIBRATE);

  while (gamepad->GetEvent(BUTTON_CALIBRATE) != kEventOpened) {
    m_rbt->ArcadeDrive(joystick);
    
    left_reading = left->Get();
    right_reading = right->Get();

    this->PrintCalibrationDisplay(reading, encoder);
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
  this->constant = NUM_FT / max(c_abs(left_reading), c_abs(right_reading));
  this->left_sign = left_reading >= 0 ? '+' : '-';
  this->right_sign = right_reading >= 0 ? '+' : '-';

  this->PrintCalibrationFinishedDisplay();
  dsLCD->UpdateLCD();

  this->ResetEncoders();

  FILE *in = fopen(AUTO_CONSTANT_FILE, "w");
  
  fprintf("%c%c%f", left_sign, right_sign, constant);

  fclose(in);
}

void Autonomous::StopEncoders() {
  this->left->Stop();
  this->right->Stop();
}

void Autonomous::StartEncoders() {
  this->left->Start();
  this->right->Start();
}

void Autonomous::SimpleDrive(double ft, float speed) {
  this->ResetEncoders();

  this->StartEncoders();

  int reading = 0;

  double dist = c_abs(ft);
  
  // We use the left encoder for default...
  double enc_dist = d_abs(left->GetDistance());

  rbt->Drive(speed, 0.0);

  while (reading < dist) {
    reading = d_abs(left->GetDistance);
  }

  rbt->Drive(0.0, 0.0);

  this->StopEncoders();
}
