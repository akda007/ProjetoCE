#include <Arduino.h>

static uint32_t RED_PIN = 25;
static uint32_t YELLOW_PIN = 33;
static uint32_t GREEN_PIN = 32;

static uint32_t POT_PIN = 35;
static uint32_t BUTTON_PIN = 13;

bool system_paused = false;
bool  night_mode = false;

uint64_t semaphore_delay = 0;

int last_button_pressed = 0;
uint64_t button_start_press = 0;

uint64_t nightmode_delay = 0;
bool night_mode_state = false;

uint64_t ciclos = 0, changes = 0;

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

  pinMode(POT_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLDOWN);
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

  Serial.printf("Mudando para a luz %s\n", get_color_name(current_color));
}



void loop() {
  int pot_input = analogRead(POT_PIN);
  int led_delay = 200 + pot_input * 1800 / 4095;

  int button_pressed = digitalRead(BUTTON_PIN);

  if (last_button_pressed == true && !button_pressed) {

    if (millis() - button_start_press > 2000) {
      night_mode = !night_mode;

      if (night_mode) {
        Serial.println("Nightmode Ligado!");
      } else {
        Serial.println("Nightmode Desligado!");
      }
    } else {
      if (system_paused) {
        Serial.println("Sistema Despausado!!!");
      } else {
        Serial.println("Sistema Pausado!!!");
      }

      system_paused = !system_paused;
    }
    
  }

  if (!last_button_pressed && button_pressed) {
    delay(30);
    button_start_press = millis();
  }

  last_button_pressed = button_pressed;


  if (!night_mode && !system_paused && millis() - semaphore_delay > led_delay) {
    semaphore_delay = millis();

    update_color();

    changes++;

    if (changes % 3 == 0) {
      ciclos++;
    }

    if (changes % 15 == 0) {
      Serial.printf("%i ciclos completos!\n", ciclos);
    }
  }

  if (night_mode && millis() - nightmode_delay > 500) {
    turn_leds_off();

    digitalWrite(YELLOW_PIN, night_mode_state);
    night_mode_state = !night_mode_state;

    nightmode_delay = millis();

    
  }

  // Serial.printf("Potenciometro: %i, delay = %i\n", pot_input, led_delay);
}

