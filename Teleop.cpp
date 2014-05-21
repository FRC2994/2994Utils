#include "Teleop.h"

// Team includes
#include "ERobotDrive.h"
#include "EJoystick.h"

// Library introductions
#include "WPILib.h"

TeleopHelper::TeleopHelper(ERobotDrive *rbt, EJoystick *left, EJoystick *right) {
  Initialize(rbt, left, right);
}

TeleopHelper::TeleopHelper(ERobotDrive *rbt, int left_port, int right_port) {
  Initialize(rbt, left_port > 0 ? new EJoystick(left_port) : 0, right_port > 0 ? new EJoystick(right_port) : 0);
}

void TeleopHelper::SetShifters(DoubleSolenoid *shifters) {
  this->shifters = shifters;
}

void TeleopHelper::SetLeft(EJoystick *left) {
  this->left = left;
}

void TeleopHelper::SetLeft(int port) {
  this->left = new EJoystick(port);
}

void TeleopHelper::SetRight(EJoystick *right) {
  this->right = right;
}

void TeleopHelper::SetRight(int port) {
  this->right = new EJoystick(port);
}

void TeleopHelper::SetJoysticks(EJoystick *left, EJoystick *right) {
  this->left = left;
  this->right = right;
}

void TeleopHelper::SetJoysticks(int left_port, int right_port) {
  this->left = new EJoystick(left_port);
  this->right = new EJoystick(right_port);
}

void TeleopHelper::SetLeftHanded(bool left_handed) {
  this->left_handed = left_handed;
}

void TeleopHelper::ArcadeDrive() {
  EJoystick *drive_stick = right;
  if (left_handed) {
    drive_stick = left;
  }

  if(kEventOpened == drive_stick.GetEvent(BUTTON_SHIFT))
  {
    // Shift into high gear.
    shifters.Set(DoubleSolenoid::kForward);
  }

  else if(kEventClosed == drive_stick.GetEvent(BUTTON_SHIFT))
  {
    // Shift into low gear.
    shifters.Set(DoubleSolenoid::kReverse);
  }

  rbt->ArcadeDrive(drive_stick);
}

void TeleopHelper::Initialize(ERobotDrive *rbt, EJoystick *left, EJoystick *right) {
  this->rbt = rbt;

  if (left) {
    this->left = left;
  }

  if (right) {
    this->right = right;
  }
}
