# WaterLeakDetection
Умный счётчик воды
Платформа: ESP32
Фреймворк: Arduino IDE

Что реализовано
Имитация набора воды каждые 3–5 секунд с объёмом 0.2, 0.5 или 1.0 литра
Подсчёт общего объёма воды за последние 60 секунд
Включение тревоги (Пин и сообщение в UART), если расход превышает 3.0L за минуту
Отключение тревоги, если расход нормализовался

Как работает
В loop() периодически генерируется случайное событие набора воды
Все события сохраняются в массив с отметками времени
Старые события удаляются при каждом проходе цикла
Подсчитывается суммарный объём воды за последние 60 секунд
Выводятся сообщения об превышении и последующей нормализации 

Как протестировать
Подключите ESP32, залейте скетч через Arduino IDE (на старых версиях необходимо скачать драйвер)
Откройте Terminal 1.9b, Putty или в самом Arduino IDE монитор порта (Serial Monitor), установите скорость общения 115200, parity (четность) - None, Data bits - 8 бит
Если все работает, в терминале будут отображаться сообщения

