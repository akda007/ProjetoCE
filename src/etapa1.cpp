#include <Arduino.h>

static uint32_t RED_PIN = 25;
static uint32_t YELLOW_PIN = 33;
static uint32_t GREEN_PIN = 32;

enum Color {
  RED,
  YELLOW,
  GREEN
};

Color current_color = RED;

uint32_t colors[] = {RED_PIN, YELLOW_PIN, GREEN_PIN};

void setup() {
  Serial.begin(115200);

  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
}

void turn_leds_off( ){
  digitalWrite(RED_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
}

String get_color_name(Color color) {
  return color == RED ? "Vermelha" : color == YELLOW ? "Amarela" : "Verde";
}

void update_color() {
  current_color = static_cast<Color>((((int)current_color + 1) % 3));

  turn_leds_off();

  digitalWrite(colors[current_color], HIGH);

  Serial.printf("Ligando a luz %s\n", get_color_name(current_color));
}

void loop() {
  sleep(1);
  update_color();
}

