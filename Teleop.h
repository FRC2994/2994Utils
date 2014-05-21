#ifndef TELEOP_H_
#define TELEOP_H_

#include "ERobotDrive.h"
#include "EJoystick.h"
#include "WPILib.h"

#define BUTTON_SHIFT 7

class TeleopHelper {
public:
  TeleopHelper(ERobotDrive *rbt, EJoystick *left=0, EJoystick *right=0);

  TeleopHelper(ERobotDrive *rbt, int left_port, int right_port=0);

  void SetShifters(DoubleSolenoid *shifters);

  void SetLeft(EJoystick *left);

  void SetLeft(int port);

  void SetRight(EJoystick *right);

  void SetRight(int port);

  void SetJoysticks(EJoystick *left, EJoystick *right);

  void SetJoysticks(int left_port, int right_port);

  void SetLeftHanded(bool left_handed);

  void ArcadeDrive();

private:
  void Initialize(ERobotDrive *rbt, EJoystick *left, EJoystick *right);

  ERobotDrive *rbt;
  EJoystick *left;
  EJoystick *right;
  DoubleSolenoid *shifters;
  bool left_handed;
};

#endif // TELEOP_H_
