/*
  Dino Game - Arduino Pro Mini + SSD1306 OLED + Single Button
  Wiring:
    OLED SDA -> A4
    OLED SCL -> A5
    OLED VCC -> 5V (or 3.3V depending on your Pro Mini variant)
    OLED GND -> GND
    Button   -> one leg to D2, other leg to GND (uses INPUT_PULLUP)
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BUTTON_PIN 2

// ---------- Dino sprite (16x16) ----------
// Simple T-Rex-like running pose
const unsigned char dinoBitmap[] PROGMEM = {
  0b00000000, 0b00111000,
  0b00000000, 0b01111100,
  0b00000000, 0b01111110,
  0b00000111, 0b11111110,
  0b00001111, 0b11111100,
  0b00011111, 0b11111000,
  0b00111111, 0b11110000,
  0b01111111, 0b11100000,
  0b11111111, 0b11000000,
  0b11111111, 0b10000000,
  0b01111111, 0b00000000,
  0b00111110, 0b00000000,
  0b00111110, 0b00000000,
  0b00110110, 0b00000000,
  0b00100110, 0b00000000,
  0b01100011, 0b00000000
};
const int dinoW = 16;
const int dinoH = 16;

// Ground / dino settings
const int groundY = 54;
int dinoX = 8;
float dinoY = groundY - dinoH;

// Jump physics
bool isJumping = false;
float velocity = 0;
const float gravity = 0.9;
const float jumpStrength = -8.5;

// Obstacle (cactus shape, simple narrow rect with spikes)
int obsX = SCREEN_WIDTH;
const int obsWidth = 8;
const int obsHeight = 16;
int obsSpeed = 3;

// Game state
unsigned long score = 0;
bool gameOver = false;
bool started = false;
unsigned long lastFrame = 0;
const int frameDelay = 40; // ms

// Button debounce
int lastRawState = HIGH;
int stableState = HIGH;
unsigned long lastChangeTime = 0;
const unsigned long debounceDelay = 25;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Wire.begin();
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    // Try alternate address if 0x3C fails
    display.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.display();

  randomSeed(analogRead(A0));

  drawStartScreen();
}

void loop() {
  bool buttonPressed = readButtonPressed();

  if (!started) {
    if (buttonPressed) {
      started = true;
    }
    return;
  }

  if (millis() - lastFrame < frameDelay) return;
  lastFrame = millis();

  if (gameOver) {
    if (buttonPressed) {
      resetGame();
    }
    drawGameOver();
    return;
  }

  // Jump trigger
  if (buttonPressed && !isJumping) {
    isJumping = true;
    velocity = jumpStrength;
  }

  // Jump physics
  if (isJumping) {
    dinoY += velocity;
    velocity += gravity;
    if (dinoY >= groundY - dinoH) {
      dinoY = groundY - dinoH;
      isJumping = false;
      velocity = 0;
    }
  }

  // Move obstacle
  obsX -= obsSpeed;
  if (obsX < -obsWidth) {
    obsX = SCREEN_WIDTH + random(10, 50);
    score++;
    if (obsSpeed < 7 && score % 5 == 0) obsSpeed++;
  }

  // Collision check (slightly shrunk hitbox for fairness)
  if (checkCollision()) {
    gameOver = true;
  }

  drawGame();
}

// Returns true exactly once when button transitions HIGH -> LOW (pressed)
bool readButtonPressed() {
  int raw = digitalRead(BUTTON_PIN);

  if (raw != lastRawState) {
    lastChangeTime = millis();
    lastRawState = raw;
  }

  bool justPressed = false;
  if ((millis() - lastChangeTime) > debounceDelay) {
    if (raw != stableState) {
      stableState = raw;
      if (stableState == LOW) {
        justPressed = true;
      }
    }
  }

  return justPressed;
}

bool checkCollision() {
  int dinoTop = (int)dinoY + 2;       // shrink hitbox by 2px each side
  int dinoBottom = (int)dinoY + dinoH - 2;
  int dinoLeft = dinoX + 2;
  int dinoRight = dinoX + dinoW - 2;

  int obsTop = groundY - obsHeight;
  int obsBottom = groundY;
  int obsLeft = obsX;
  int obsRight = obsX + obsWidth;

  return (dinoRight > obsLeft && dinoLeft < obsRight &&
          dinoBottom > obsTop && dinoTop < obsBottom);
}

void drawGame() {
  display.clearDisplay();

  // Ground line
  display.drawLine(0, groundY, SCREEN_WIDTH, groundY, SSD1306_WHITE);

  // Dino sprite
  display.drawBitmap(dinoX, (int)dinoY, dinoBitmap, dinoW, dinoH, SSD1306_WHITE);

  // Obstacle - cactus shape (vertical bars)
  display.fillRect(obsX, groundY - obsHeight, 3, obsHeight, SSD1306_WHITE);
  display.fillRect(obsX + 4, groundY - obsHeight + 4, 3, obsHeight - 4, SSD1306_WHITE);

  // Score
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(F("Score: "));
  display.print(score);

  display.display();
}

void drawGameOver() {
  display.clearDisplay();
  display.setTextSize(1);

  display.setCursor(28, 18);
  display.print(F("GAME OVER"));

  display.setCursor(20, 34);
  display.print(F("Score: "));
  display.print(score);

  display.setCursor(6, 50);
  display.print(F("Press to restart"));

  display.display();
}

void drawStartScreen() {
  display.clearDisplay();
  display.setTextSize(1);

  display.setCursor(20, 20);
  display.print(F("DINO GAME"));

  display.setCursor(10, 40);
  display.print(F("Press button to start"));

  display.display();
}

void resetGame() {
  score = 0;
  obsSpeed = 3;
  obsX = SCREEN_WIDTH;
  dinoY = groundY - dinoH;
  isJumping = false;
  velocity = 0;
  gameOver = false;
}
