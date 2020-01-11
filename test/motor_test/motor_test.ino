/*
 * Motor controller test
*/
#include <ros.h>
#include <std_srvs/SetBool.h>

// Door motor encoders parameters
#define ENC_D1_A 2
#define ENC_D1_B 3
#define ENC_D2_A 18
#define ENC_D2_B 4
#define ENC_D3_A 19
#define ENC_D3_B 5
#define ENC_D4_A 20
#define ENC_D4_B 6
#define ENC_D5_A 21
#define ENC_D5_B 7

// Door pins
#define D1_OPEN 22
#define D1_CLOSE 23
#define D2_OPEN 24
#define D2_CLOSE 25
#define D3_OPEN 26
#define D3_CLOSE 27
#define D4_OPEN 28
#define D4_CLOSE 29
#define D5_OPEN 30
#define D5_CLOSE 31

ros::NodeHandle nh;
void door1_cb(const std_srvs::SetBool::Request & req, std_srvs::SetBool::Response & res);
void door2_cb(const std_srvs::SetBool::Request & req, std_srvs::SetBool::Response & res);
void door3_cb(const std_srvs::SetBool::Request & req, std_srvs::SetBool::Response & res);
void door4_cb(const std_srvs::SetBool::Request & req, std_srvs::SetBool::Response & res);
void door5_cb(const std_srvs::SetBool::Request & req, std_srvs::SetBool::Response & res);
ros::ServiceServer<std_srvs::SetBool::Request, std_srvs::SetBool::Response> server1("door1", &door1_cb);
ros::ServiceServer<std_srvs::SetBool::Request, std_srvs::SetBool::Response> server2("door2", &door2_cb);
ros::ServiceServer<std_srvs::SetBool::Request, std_srvs::SetBool::Response> server3("door3", &door3_cb);
ros::ServiceServer<std_srvs::SetBool::Request, std_srvs::SetBool::Response> server4("door4", &door4_cb);
ros::ServiceServer<std_srvs::SetBool::Request, std_srvs::SetBool::Response> server5("door5", &door5_cb);

volatile long d1_count = 0, d2_count = 0, d3_count = 0, d4_count = 0, d5_count = 0;
//float N = 823.1; // Gear box total pulse per revolution (PPR)
int N = 800;

void door1_cb(const std_srvs::SetBool::Request & req, std_srvs::SetBool::Response & res)
{
  long start = 0;
  // Open door
  if (req.data)
  {
    start = d1_count;
    digitalWrite(D1_OPEN, HIGH);
    while(abs(d1_count - start) < N) {};
    digitalWrite(D1_OPEN, LOW);
    res.message = "Door 1 closed!";
  } else // close door
  {
    start = d1_count;
    digitalWrite(D1_CLOSE, HIGH);
    while(abs(d1_count - start) < N) {};
    digitalWrite(D1_CLOSE, LOW);
    res.message = "Door 1 open";
  }
  res.success = true;
}

void door2_cb(const std_srvs::SetBool::Request & req, std_srvs::SetBool::Response & res)
{
  long start = 0;
  // Open door
  if (req.data)
  {
    start = d2_count;
    digitalWrite(D2_OPEN, LOW);
    while(abs(d2_count - start) < N) {};
    res.message = "Door 2 closed!";
  } else // close door
  {
    start = d2_count;
    digitalWrite(D2_CLOSE, LOW);
    while(abs(d2_count - start) < N) {};
    res.message = "Door 2 open";
  }
  res.success = true;
}

void door3_cb(const std_srvs::SetBool::Request & req, std_srvs::SetBool::Response & res)
{
  long start = 0;
  // Open door
  if (req.data)
  {
    start = d3_count;
    digitalWrite(D3_OPEN, LOW);
    while(abs(d3_count - start) < N) {};
    res.message = "Door 3 closed!";
  } else // close door
  {
    start = d3_count;
    digitalWrite(D3_CLOSE, LOW);
    while(abs(d3_count - start) < N) {};
    res.message = "Door 3 open";
  }
  res.success = true;
}

void door4_cb(const std_srvs::SetBool::Request & req, std_srvs::SetBool::Response & res)
{
  long start = 0;
  // Open door
  if (req.data)
  {
    start = d4_count;
    digitalWrite(D4_OPEN, LOW);
    while(abs(d4_count - start) < N) {};
    res.message = "Door 4 closed!";
  } else // close door
  {
    start = d4_count;
    digitalWrite(D4_CLOSE, LOW);
    while(abs(d4_count - start) < N) {};
    res.message = "Door 4 open";
  }
  res.success = true;
}

void door5_cb(const std_srvs::SetBool::Request & req, std_srvs::SetBool::Response & res)
{
  long start = 0;
  // Open door
  if (req.data)
  {
    start = d5_count;
    digitalWrite(D5_OPEN, LOW);
    while(abs(d5_count - start) < N) {};
    res.message = "Door 5 closed!";
  } else // close door
  {
    start = d5_count;
    digitalWrite(D5_CLOSE, LOW);
    while(abs(d5_count - start) < N) {};
    res.message = "Door 5 open";
  }
  res.success = true;
}

void d1encoderEvent2x()
{
  if (digitalRead(ENC_D1_B) == 0) {
    if (digitalRead(ENC_D1_A) == 0) {
      // A fell, B is LOW
      d1_count--; // Moving reverse
    } else {
      // A rose, B is LOW
      d1_count++; // Moving reverse
    }
  } else {
    if (digitalRead(ENC_D1_A) == 0) {
      // A fell, B is HIGH
      d1_count++; // Moving reverse
    } else {
      // A rose, B is HIGH
      d1_count--; // Moving forward
    }
  }
}

void d2encoderEvent2x()
{
  if (digitalRead(ENC_D2_B) == 0) {
    if (digitalRead(ENC_D2_A) == 0) {
      d2_count--;
    } else {
      d2_count++;
    }
  } else {
    if (digitalRead(ENC_D2_A) == 0) {
      d2_count++;
    } else {
      d2_count--;
    }
  }
}

void d3encoderEvent2x()
{
  if (digitalRead(ENC_D3_B) == 0) {
    if (digitalRead(ENC_D3_A) == 0) {
      d3_count--;
    } else {
      d3_count++;
    }
  } else {
    if (digitalRead(ENC_D3_A) == 0) {
      d3_count++;
    } else {
      d3_count--;
    }
  }
}

void d4encoderEvent2x()
{
  if (digitalRead(ENC_D4_B) == 0) {
    if (digitalRead(ENC_D4_A) == 0) {
      d4_count--; 
    } else {
      d4_count++; 
    }
  } else {
    if (digitalRead(ENC_D4_A) == 0) {
      d4_count++; 
    } else {
      d4_count--; 
    }
  }
}

void d5encoderEvent2x()
{
  if (digitalRead(ENC_D5_B) == 0) {
    if (digitalRead(ENC_D5_A) == 0) {
      d5_count--;
    } else {
      d5_count++;
    }
  } else {
    if (digitalRead(ENC_D5_A) == 0) {
      d5_count++;
    } else {
      d5_count--;
    }
  }
}

//##################################################

void setup()
{
  // ROS
  nh.initNode();
  nh.advertiseService(server1);
  nh.advertiseService(server2);
  nh.advertiseService(server3);
  nh.advertiseService(server4);
  nh.advertiseService(server5);

  // Pins
  pinMode(ENC_D1_A, INPUT);
  pinMode(ENC_D1_B, INPUT);
  pinMode(ENC_D2_A, INPUT);
  pinMode(ENC_D2_B, INPUT);
  pinMode(ENC_D3_A, INPUT);
  pinMode(ENC_D3_B, INPUT);
  pinMode(ENC_D4_A, INPUT);
  pinMode(ENC_D4_B, INPUT);
  pinMode(ENC_D5_A, INPUT);
  pinMode(ENC_D5_B, INPUT);

  pinMode(D1_OPEN, OUTPUT);
  pinMode(D1_CLOSE, OUTPUT);
  pinMode(D2_OPEN, OUTPUT);
  pinMode(D2_CLOSE, OUTPUT);
  pinMode(D3_OPEN, OUTPUT);
  pinMode(D3_CLOSE, OUTPUT);
  pinMode(D4_OPEN, OUTPUT);
  pinMode(D4_CLOSE, OUTPUT);
  pinMode(D5_OPEN, OUTPUT);
  pinMode(D5_CLOSE, OUTPUT);

  // Init interrupts
  attachInterrupt(digitalPinToInterrupt(ENC_D1_A), d1encoderEvent2x, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_D2_A), d2encoderEvent2x, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_D3_A), d3encoderEvent2x, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_D4_A), d4encoderEvent2x, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_D5_A), d5encoderEvent2x, CHANGE);
  
}

void loop()
{
  nh.spinOnce();
  delay(100);
}
