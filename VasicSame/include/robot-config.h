using namespace vex;

extern brain Brain;

using signature = vision::signature;

// VEXcode devices
extern motor L1;
extern motor L2;
extern motor R2;
extern motor R1;
extern controller Controller1;
extern motor Intakes;
extern signature StartDirect__GREENT;
extern signature StartDirect__REDT;
extern signature StartDirect__BLUET;
extern signature StartDirect__BAR;
extern signature StartDirect__SIG_5;
extern signature StartDirect__SIG_6;
extern signature StartDirect__SIG_7;
extern vision StartDirect;
extern signature Vision_sens__SIG_RED;
extern signature Vision_sens__SIG_2;
extern signature Vision_sens__SIG_3;
extern signature Vision_sens__SIG_4;
extern signature Vision_sens__SIG_5;
extern signature Vision_sens__SIG_6;
extern signature Vision_sens__SIG_7;
extern vision Vision_sens;
extern motor Launch;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );