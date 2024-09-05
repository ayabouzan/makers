// Define the motor driver pins connected to the Arduino
#define IN1 7    // Input 1 for Motor A
#define IN2 6    // Input 2 for Motor A
#define IN3 5    // Input 1 for Motor B
#define IN4 4    // Input 2 for Motor B

// Define the ultrasonic sensor pins
#define TRIG_FRONT 8
#define ECHO_FRONT 9
#define TRIG_LEFT 10
#define ECHO_LEFT 11
#define TRIG_RIGHT 12
#define ECHO_RIGHT 13

// Define the obstacle detection distance threshold (in cm)
#define OBSTACLE_DISTANCE 20

// Setup function
void setup() {
  // Set all motor driver pins as output
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Set all ultrasonic sensor pins as output/input
  pinMode(TRIG_FRONT, OUTPUT);
  pinMode(ECHO_FRONT, INPUT);
  pinMode(TRIG_LEFT, OUTPUT);
  pinMode(ECHO_LEFT, INPUT);
  pinMode(TRIG_RIGHT, OUTPUT);
  pinMode(ECHO_RIGHT, INPUT);

  // Start serial communication for debugging
  Serial.begin(9600);
}

// Loop function
void loop() {
  // Measure distances from the three ultrasonic sensors
  int distanceFront = getDistance(TRIG_FRONT, ECHO_FRONT);
  int distanceLeft = getDistance(TRIG_LEFT, ECHO_LEFT);
  int distanceRight = getDistance(TRIG_RIGHT, ECHO_RIGHT);

  // Print distances for debugging
  Serial.print("Front: ");
  Serial.print(distanceFront);
  Serial.print(" cm, Left: ");
  Serial.print(distanceLeft);
  Serial.print(" cm, Right: ");
  Serial.println(distanceRight);

  // Obstacle avoidance logic
  if (distanceFront > OBSTACLE_DISTANCE) {
    // No obstacle ahead, move forward
    moveForward();
  } else {
    // Obstacle detected ahead, decide to turn left or right
    if (distanceLeft > distanceRight) {
      turnLeft();
    } else {
      turnRight();
    }
  }

  delay(100); // Small delay to stabilize sensor readings
}

// Function to measure distance using the ultrasonic sensor
int getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2; // Convert time to distance in cm
  return distance;
}

// Function to move the car forward
void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// Function to turn the car left
void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(500); // Adjust delay for turning angle
}

// Function to turn the car right
void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(500); // Adjust delay for turning angle
}

// Function to stop the car
void stopCar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}