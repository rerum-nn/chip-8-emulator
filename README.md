# Chip-8 Emulator
Эмулятор работы программ, написанных на CHIP-8

# Мотивация
Курс "Архитектура компьютера" направлен на знакомство с ассемблером и внутренним устройством электронной вычислительной техники. Эмуляция работы CHIP-8 была выбрана как попытка воссоздания процессов, происходящих внутри компьютера, но в виртуальной среде.

# Компиляция и запуск
Для успешной компиляции необходима библиотека SFML. После компиляции появляется файл исполняемый chip-8-emulator, правила запуска:
```
./chip-8-emulator /path/to/program.ch8 {-h (int cpu hertz)} {-s (int window scale)}
```

# Известные проблемы
- Проблема с блокированием потока при ожидании ввода пользователя
- Баги в игре tetris и прочих
- Большая связанность Display и Kernel 

# Обязанности на текущий момент
- Rerum_nn: Реализация ядра и памяти
- Zzifi: Реализация перефирии
- Все обязанности условные, естественно возможна передача каких-то обязанностей друг другу

# Roadmap
- ~~Базовая логика работы модели ядра и памяти без инструкций, связанных с отрисовкой~~
- ~~Ввод с клавиатуры~~
- ~~Вывод на экран~~
- ~~Загрузка программы из файла~~
- Устранение недоработок
- Прочие улучшения взаимодействия
- Сделать систему логов
- Написать тесты

# Источники
- [Общая информация по CHIP-8](https://github.com/mattmikolay/chip-8/wiki/Mastering-CHIP%E2%80%908)
- [Больше информации о технической части и описание instruction set](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
- [One more спецификация](https://tonisagrista.com/blog/2021/chip8-spec/)
- [Wikipedia](https://en.wikipedia.org/wiki/CHIP-8)
