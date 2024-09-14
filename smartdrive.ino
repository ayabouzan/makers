// Define motor driver pins connected to Arduino
#define IN1 7    // Input 1 for Motor A
#define IN2 6    // Input 2 for Motor A
#define IN3 5    // Input 1 for Motor B
#define IN4 4    // Input 2 for Motor B

// Define ultrasonic sensor pins
#define TRIG_FRONT 8
#define ECHO_FRONT 9
#define TRIG_LEFT 10
#define ECHO_LEFT 11
#define TRIG_RIGHT 12
#define ECHO_RIGHT 13

// Define obstacle detection distance thresholds (in cm)
#define MAP_DETECT_DISTANCE 10
#define OBSTACLE_DETECT_DISTANCE 5

// Define initial position and direction
int xPos = 0, yPos = 0; // Initial coordinates
int direction = 0;      // 0: x+, 1: y+, 2: x-, 3: y-

// Setup function
void setup() {
  // Set all motor driver pins as output
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Set ultrasonic sensor pins as output/input
  pinMode(TRIG_FRONT, OUTPUT);
  pinMode(ECHO_FRONT, INPUT);
  pinMode(TRIG_LEFT, OUTPUT);
  pinMode(ECHO_LEFT, INPUT);
  pinMode(TRIG_RIGHT, OUTPUT);
  pinMode(ECHO_RIGHT, INPUT);

  // Start serial communication for HC-05 Bluetooth module
  Serial.begin(9600); // Set baud rate to match HC-05's default (9600)
}

// Loop function
void loop() {
  // Measure distances from the three ultrasonic sensors
  int distanceFront = getDistance(TRIG_FRONT, ECHO_FRONT);
  int distanceLeft = getDistance(TRIG_LEFT, ECHO_LEFT);
  int distanceRight = getDistance(TRIG_RIGHT, ECHO_RIGHT);

  // Obstacle avoidance and mapping logic
  if (distanceFront > OBSTACLE_DETECT_DISTANCE) {
    if (distanceFront <= MAP_DETECT_DISTANCE) {
      // Send position and distance to Python for mapping via Bluetooth
      sendPositionAndDistance(distanceFront, distanceLeft, distanceRight);
    }
    // No close obstacle ahead, move forward
    moveForward();
    updatePosition();
  } else {
    // Obstacle detected at 5 cm, decide whether to turn left or right
    if (distanceLeft > distanceRight) {
      turnLeft();
    } else {
      turnRight();
    }
    // Send updated position after turning
    sendPositionAndDistance(distanceFront, distanceLeft, distanceRight);
  }

  delay(100); // Small delay to stabilize sensor readings
}

// Function to measure distance using ultrasonic sensor
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

// Function to move the car forward slowly
void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(1500); // Longer delay for slower movement
}

// Function to turn the car left
void turnLeft() {
  direction = (direction + 1) % 4;  // Update direction (left turn)
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(1000); // Adjust delay for turning angle
}

// Function to turn the car right
void turnRight() {
  direction = (direction + 3) % 4;  // Update direction (right turn)
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(1000); // Adjust delay for turning angle
}

// Function to stop the car
void stopCar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// Function to update the car's position based on the direction
void updatePosition() {
  if (direction == 0) {
    xPos += 1;  // Moving in x+
  } else if (direction == 1) {
    yPos += 1;  // Moving in y+
  } else if (direction == 2) {
    xPos -= 1;  // Moving in x-
  } else if (direction == 3) {
    yPos -= 1;  // Moving in y-
  }
}

// Function to send position and distance data via Bluetooth (HC-05)
void sendPositionAndDistance(int frontDistance, int leftDistance, int rightDistance) {
  Serial.print("Position: ");
  Serial.print("(");
  Serial.print(xPos);
  Serial.print(", ");
  Serial.print(yPos);
  Serial.print("), ");
  Serial.print("Direction: ");
  if (direction == 0) Serial.print("x+");
  else if (direction == 1) Serial.print("y+");
  else if (direction == 2) Serial.print("x-");
  else if (direction == 3) Serial.print("y-");
  Serial.print(", Distances - Front: ");
  Serial.print(frontDistance);
  Serial.print(" cm, Left: ");
  Serial.print(leftDistance);
  Serial.print(" cm, Right: ");
  Serial.println(rightDistance);
}
