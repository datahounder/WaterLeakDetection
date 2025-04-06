// Псевдокод поступления воды каждые 3–5 секунд с объёмом 0.2, 0.5 или 1.0 литра
// Платформа: ESP32

#include <Arduino.h>

// Настройки
const int alertPin = 2;                // пин для индикации тревоги
const unsigned long intervalMin = 3000; // 3 секунды
const unsigned long intervalMax = 5000; // 5 секунды
const float volumes[] = {0.2, 0.5, 1.0};
const int volumeCount = sizeof(volumes) / sizeof(volumes[0]);
const float leakThreshold = 3.0;       // литров в минуту
const unsigned long windowSize = 60000; // 60 секунд (минута) в миллисекундах

// структура для отметок времени
struct WaterEvent {
  unsigned long timestamp;
  float volume;
};

#define MAX_EVENTS 100
WaterEvent events[MAX_EVENTS]; //Все события будут сохраняться в массив с отметками времени (timestamps)
int eventCount = 0;

unsigned long lastPourTime = 0;
bool alertActive = false;

void setup() {
  Serial.begin(115200);
  pinMode(alertPin, OUTPUT);
  digitalWrite(alertPin, LOW);
  randomSeed(analogRead(0));
  Serial.println("Умный счетчик воды запущен!");
}

void loop() {
  unsigned long now = millis();

  //  периодически генерируется случайное событие набора воды
  if (now - lastPourTime >= random(intervalMin, intervalMax)) {
    float volume = volumes[random(0, volumeCount)];
    addEvent(now, volume);
    Serial.print("Текущий расход воды: ");
    Serial.print(volume);
    Serial.println(" л");
    lastPourTime = now;
  }

  // Старые события удаляются при каждом витке цикла
  removeOldEvents(now);

  // Подсчёт общего объёма воды за последние 60 секунд
  float totalVolume = getTotalVolume();

  if (totalVolume > leakThreshold) {
    // Включение тревоги (пин и сообщение в UART), если расход превышает 3.0L за минуту
    if (!alertActive) {
      alertActive = true;
      digitalWrite(alertPin, HIGH);
      Serial.println("Внимание: превышен порог"); 
    }
  } else {
    //Отключение тревоги, если расход нормализовался
    if (alertActive) {
      alertActive = false;
      digitalWrite(alertPin, LOW);
      Serial.println("Расход нормализовался");
    }
  }

  delay(100);
}

void addEvent(unsigned long timestamp, float volume) {
  if (eventCount < MAX_EVENTS) {
    events[eventCount++] = {timestamp, volume};
  } else {
    // сдвиг влево
    for (int i = 1; i < MAX_EVENTS; i++) {
      events[i - 1] = events[i];
    }
    events[MAX_EVENTS - 1] = {timestamp, volume};
  }
}

void removeOldEvents(unsigned long now) {
  int shift = 0;
  for (int i = 0; i < eventCount; i++) {
    if (now - events[i].timestamp > windowSize) {
      shift++;
    } else {
      break;
    }
  }
  if (shift > 0) {
    for (int i = shift; i < eventCount; i++) {
      events[i - shift] = events[i];
    }
    eventCount -= shift;
  }
}

float getTotalVolume() {
  float total = 0.0;
  for (int i = 0; i < eventCount; i++) {
    total += events[i].volume;
  }
  return total;
}
