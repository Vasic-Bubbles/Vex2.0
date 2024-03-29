// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// L1                   motor         10              
// L2                   motor         1               
// R2                   motor         11              
// R1                   motor         20              
// Controller1          controller                    
// Intakes              motor         5               
// StartDirect          vision        18              
// Vision_sens          vision        16              
// Launch               motor         15              
// ---- END VEXCODE CONFIGURED DEVICES ----
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Castillo                                                  */
/*    Created:      Thu Oct 12 2023                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/



#include "vex.h"

using namespace vex;




#include "vex.h"

using namespace vex;



// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!

  L1.resetPosition();
  L2.resetPosition();
  R1.resetPosition();
  R2.resetPosition();

  L1.stop();
  L2.stop();
  R1.stop();
  R2.stop();

  
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/




void leftpower(double percet) {
  // Left spin until stopped

  L1.spin(forward, percet, pct) ;
  L2.spin(forward, percet, pct) ;

}

void rightpower(double percet) {
  // Right spin until stopped

  R1.spin(forward, percet, pct) ;
  R2.spin(forward, percet, pct) ;

}

void fullpower(double leftpow, double rightpow) {
  // Full spin until stopped

  R1.spin(forward, rightpow, pct) ;
  R2.spin(forward, rightpow, pct) ;

  L1.spin(forward, leftpow, pct) ;
  L2.spin(forward, leftpow, pct) ;
}



int hgreen = 0; // Height of Green Object for distance measurement
int wgreen = 0; // Width of Green Object for distance measurement
int adjust_turn = 0; // Will turn a specified constant
double CenterFOV = 157; // Center of pixels
int CenterGreen = 0;
int Targetarea = 53172; // Area which robot should stop at

int Realarea = 0;

double powe = 0;
double Bcons = -0.4623 ; 
double Acons = 2804;
double distance_temp = 0;
double distance_p = 0; //Distance conversion from area of pixels into distance in cm
double target_distance = 10;
double adjust_kp = 0;


// ----------------------------------------------------------------------------------------------------------------------------------
// PID Class

class PID //This class will contain all information and calculations of my PIDs
{
  private:

  double last_error = 0 ;
  double Kp ;
  double Ki ;
  double Kd ;
  double Integral = 0;


  public: 
  
  PID(double kp, double ki, double kd) : last_error(0), Kp(kp), Ki(ki), Kd(kd), Integral(0) {} //Constructor

  double Regular_PID(double Target, double Current) { //Regular PID calculations, if special will use another function
    double error = Target - Current ;
  
    double Proportional = error * Kp ;
    Integral = Integral + (error * Ki) ;
    double Derivative = Kd * (error - last_error) ;


    last_error = error ;

    double Control_PID = Proportional + Integral + Derivative ;

    return Control_PID ;
  }

  class Distance_Control // I will probably not use this class again, but its so general that I just have to make it like this
  {

    private:

    double ConstantA ;
    double ConstantB ;
    double (*func)(double, double, double);


    
    public:

    Distance_Control(double(*func)(double, double, double), double ConsA, double ConsB) : ConstantA(ConsA), ConstantB(ConsB) {} //I dont even know why I am making this, it isn't even necessary

    double Distance_Mul(double Current_Pos) {
    
      double Constant_Mul = func(ConstantA, ConstantB, Current_Pos) ;

      return Constant_Mul ;
    }

  };

};

//PID CLASS 
// --------------------------------------------------------------------------------------------------------



// -------------------------------------------------------------------------------------------------------------------
// Math Functions

double Linear(double slope, double offset, double var) {
  double Linear_val = (var*slope) + offset;

  return Linear_val ;
}

double Power(double mul_A, double pow_b, double var) {
    double Power_val = pow(var, pow_b) * mul_A ;

    return Power_val ;
}


// Might add more later on 
// -----------------------------------------------------------------------------------------------------------------------------



// -------------------------------------------------------------------------------------------------------------------------
// Vision Sensor Adjust

void Visionadjust() {

  

  StartDirect.takeSnapshot(StartDirect__REDT);
  hgreen = StartDirect.objects[0].height;
  wgreen = StartDirect.objects[0].width;
  Realarea = hgreen * wgreen ; // Used to calculate our distance from the object

  CenterGreen = StartDirect.objects[0].centerX;

  powe = 100 ; // Full power which will be subtracted based on  the adjust

  distance_temp = pow(Realarea, Bcons) ;
  distance_p = distance_temp*Acons ; // Distance in centimeters from area Realarea


  PID Vision_Values(0.01, 0.01, 0.02) ; // Kp, Ki, Kd values

  PID::Distance_Control Regulator(Power, 33.06, -0.9204) ; //Use logger pro to get according function

  // So this is a basic PID, the problem is we have to mix it with our distance target
  // I am thinking of changing Kp Ki and Kd depending on our distance to our object
  //Or better yet, lets just multiply the adjust depending on the distance, the closer it is the stronger the adjust is


  // Also if we stop seeing the object we will just stop and then I will programm a back up plan
  // I do not know what will be the back up yet though, I am thinking of using the black bars

  if (StartDirect.objects[0].exists) {
    if (distance_p > target_distance) {
      
      double adjust = Vision_Values.Regular_PID(CenterFOV, CenterGreen) ;
      double mul_dis = Regulator.Distance_Mul(distance_p);
      
     // Takes the adjust and adds it to B (since its negative), subtracts to C (Its positive)
      //The mul_dis multiplies a constant that should increase the adjust the closer the robot it to the object

      L1.spin(reverse,powe + (adjust*mul_dis),pct);
      L2.spin(reverse,powe + (adjust*mul_dis),pct);
      R1.spin(forward,powe - (adjust*mul_dis),pct);
      R2.spin(forward,powe - (adjust*mul_dis),pct);

    }
  }

}
// Vision Sensor Adjust
// -------------------------------------------------------------------------------------------------------------------------------------------------------



void forwardstime(double times) {
  L1.spin(reverse,100,pct);
  L2.spin(reverse,100,pct);
  R1.spin(forward,100,pct);
  R2.spin(forward,100,pct);

  wait(times, msec);

  L1.stop();
  L2.stop();
  R1.stop();
  R2.stop();
}

void backwardstime(double times) {
  L1.spin(reverse,50,pct);
  L2.spin(reverse,50,pct);
  R1.spin(reverse,50,pct);
  R2.spin(reverse,50,pct);

  wait(times, msec);

  L1.stop();
  L2.stop();
  R1.stop();
  R2.stop();
}

void leftime(double times) {
  L1.spin(reverse,50,pct);
  L2.spin(reverse,50,pct);
  R1.spin(forward,50,pct);
  R2.spin(forward,50,pct);

  wait(times, msec);

  L1.stop();
  L2.stop();
  R1.stop();
  R2.stop();
}

void rightime(double times) {
  L1.spin(forward,50,pct);
  L2.spin(forward,50,pct);
  R1.spin(reverse,50,pct);
  R2.spin(reverse,50,pct);

  wait(times, msec);

  L1.stop();
  L2.stop();
  R1.stop();
  R2.stop();
}

void forwards(double degr) {

  // Maybe set time out
  L1.setVelocity(100, pct);
  R1.setVelocity(100, pct);
  L2.setVelocity(100, pct);
  R2.setVelocity(100, pct);
  L2.spinToPosition(degr, degrees, false);
  R1.spinToPosition(degr, degrees, false);
  L2.spinToPosition(degr, degrees, false);
  R2.spinToPosition(degr, degrees, false);
  //Spin to position maybe?
}

void turnle(double degr) {

  // Maybe set time out
  L1.setVelocity(100, pct);
  R1.setVelocity(100, pct);
  L2.setVelocity(100, pct);
  R2.setVelocity(100, pct);
  L2.spinToPosition(degr, degrees, false);
  R1.spinToPosition(degr, degrees, false);
  L2.spinToPosition(degr, degrees, false);
  R2.spinToPosition(degr, degrees, false);
  //Spin to position maybe?
}

void rect(double degr, double spedr) {

  L1.resetPosition();
  L2.resetPosition();
  R1.resetPosition();
  R2.resetPosition();


  R1.spinTo(degr, degrees, spedr, rpm, false);
  R2.spinTo(degr, degrees, spedr, rpm, false);
  L1.spinTo(-degr, degrees, spedr, rpm, false);
  L2.spinTo(-degr, degrees, spedr, rpm, true);

  L1.stop();
  L2.stop();
  R1.stop();
  R2.stop();

}

void lefr(double degr, double spedl, double spedr) {
  L1.resetPosition();
  L2.resetPosition();
  R1.resetPosition();
  R2.resetPosition();


  R1.spinTo(degr, degrees, spedr, rpm, false);
  L1.spinTo(degr, degrees, spedl, rpm, false);
  R2.spinTo(degr, degrees, spedr, rpm, false);
  L2.spinTo(degr, degrees, spedl, rpm, true);

  L1.stop();
  L2.stop();
  R1.stop();
  R2.stop();

}

// Hey Joao all of these functions operate on time sooooo
// It would be great if you could make a function that operates on degrees
// I haven't had a lot of time to do it since I have been working on vision sensor
// Also these autonomous is pretty useless it just goes forward and backwards

void autonomous(void) {

  L1.resetPosition();
  L2.resetPosition();
  R1.resetPosition();
  R2.resetPosition();


  rect(2000, 290);

  lefr(390, 250, 250);

  forwardstime(4000);

  rect(-1800, 290);
  rect(300, 250);
  lefr(780, 250, 250);

}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop

  //pos2 will be set degrees before launching
  Launch.resetPosition();

  while (1) {
    float ch3=-1*Controller1.Axis3.value();
    float ch2=Controller1.Axis2.value();

    //If moving forward is more than 10 then advance if less then do nothing,
    //I do not understand the axis of the robot very well, have to investigate better
    if(fabs(ch3)<10)
    {
      ch3=0;
    }
    if(fabs(ch2)<10)
    {
      ch2=0;
    }


    

    L1.spin(forward,ch3,pct);
    L2.spin(forward,ch3,pct);
    R1.spin(forward,ch2,pct);
    R2.spin(forward,ch2,pct);

    if (Controller1.ButtonL1.pressing()){
      Intakes.spin(reverse, -100, pct);
    } else if (Controller1.ButtonL2.pressing()){
      Intakes.stop();
    }

    
    if (Controller1.ButtonR2.pressing()){
      Intakes.stop();
      Launch.spin(forward, -120, rpm);
      Lunch.spin(forward, -120, rpm);
    } 
    else if (Controller1.ButtonA.pressing()){
      Launch.stop();
      Launch.resetPosition();
      Launch.setVelocity(0, rpm);
      Lunch.stop();
      Lunch.resetPosition();
      Lunch.setVelocity(0, rpm);
    } else if (Controller1.ButtonR1.pressing()) {
      Launch.resetPosition();
      Launch.spinToPosition(-1500, degrees, -120, rpm);
      Launch.setVelocity(0, rpm);
      Lunch.resetPosition();
      Lunch.spinToPosition(-1500, degrees, -120, rpm);
      Lunch.setVelocity(0, rpm);
    }

    

    // ch2 and ch2 are percentage/power defined by analog stick

    
    // sudo code variable x degrees position, label current position, 
    // press button up arrow set degrees for launch mode


    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {

    wait(100, msec);
  }
}


//The conliftic merge just failed, that is good to know as there is no risk of deleting something wrong and doing a costly mistake, right?

