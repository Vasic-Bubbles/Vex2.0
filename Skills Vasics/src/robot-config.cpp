#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor L1 = motor(PORT1, ratio6_1, false);
motor L2 = motor(PORT5, ratio6_1, false);
motor R2 = motor(PORT6, ratio6_1, false);
motor R1 = motor(PORT10, ratio6_1, false);
controller Controller1 = controller(primary);
motor Intakes = motor(PORT5, ratio18_1, false);
/*vex-vision-config:begin*/
signature StartDirect__GREENT = signature (1, -5513, -3941, -4728, -4637, -2519, -3578, 2.5, 0);
signature StartDirect__REDT = signature (2, 9445, 12455, 10950, -1937, -1305, -1621, 2.5, 0);
signature StartDirect__BLUET = signature (3, -3769, -2783, -3276, 8551, 12031, 10290, 2.5, 0);
signature StartDirect__BAR = signature (4, 0, 0, 0, 0, 0, 0, 3, 0);
vision StartDirect = vision (PORT18, 50, StartDirect__GREENT, StartDirect__REDT, StartDirect__BLUET, StartDirect__BAR);
/*vex-vision-config:end*/
/*vex-vision-config:begin*/
signature Vision_sens__SIG_RED = signature (1, 9857, 11281, 10569, -1611, -963, -1287, 2.5, 0);
vision Vision_sens = vision (PORT16, 50, Vision_sens__SIG_RED);
/*vex-vision-config:end*/
motor Launch = motor(PORT3, ratio18_1, false);
motor Lunch = motor(PORT12, ratio18_1, false);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;
// define variables used for controlling motors based on controller inputs
bool Controller1RightShoulderControlMotorsStopped = true;

// define a task that will handle monitoring inputs from Controller1

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
 
