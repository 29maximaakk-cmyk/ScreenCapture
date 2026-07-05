📸 ScreenCapture – Утилита для скриншотов на 7 языках
Мощная кроссплатформенная утилита для создания скриншотов с поддержкой задержки, выбора области, настраиваемого формата и имени файла.
Поддерживает Windows, Linux и macOS.
Реализована на 7 языках программирования с единым интерфейсом командной строки.

🚀 Возможности
Захват всего экрана – сохраняет полный снимок рабочего стола.

Захват области – укажите координаты и размер (-r x,y,w,h).

Задержка – установите таймер перед снимком (-d секунды).

Настраиваемый выходной файл – укажите имя и формат (PNG, JPEG).

Кроссплатформенность – работает на Windows, Linux и macOS.

Цветной вывод – информативные сообщения в терминале.

Минимальные зависимости – использует встроенные или легко устанавливаемые библиотеки.

📖 Использование
Синтаксис (единый для всех версий):

bash
<команда> [опции]
Опции
Опция	Описание
-o, --output <файл>	Имя выходного файла (по умолчанию screenshot_<дата>.png)
-d, --delay <сек>	Задержка перед снимком в секундах (по умолчанию 0)
-r, --region <x,y,w,h>	Область захвата (координаты и размер)
-h, --help	Справка
Примеры
bash
# Сделать скриншот всего экрана
python screenshot.py

# Сделать скриншот с задержкой 5 секунд
python screenshot.py -d 5

# Сохранить в конкретный файл
python screenshot.py -o my_screen.png

# Захватить область (x=100, y=100, ширина=800, высота=600)
python screenshot.py -r 100,100,800,600
🛠 Установка и запуск
Для каждого языка требуются свои зависимости. Подробности см. в соответствующих разделах.

Python
bash
pip install pillow
python screenshot.py [опции]
Go
bash
go get github.com/kbinani/screenshot
go build screenshot.go
./screenshot [опции]
JavaScript (Node.js)
bash
npm install screenshot-desktop
node screenshot.js [опции]
C++
bash
# Для Linux: sudo apt-get install scrot (или gnome-screenshot)
# Для macOS: встроенная утилита screencapture
# Для Windows: встроенный PowerShell
g++ -std=c++11 screenshot.cpp -o screenshot
./screenshot [опции]
C#
bash
csc screenshot.cs
mono screenshot.exe [опции]   # или dotnet run
Требуется .NET Framework 4.5+ или .NET Core.

Ruby
bash
gem install rmagick
ruby screenshot.rb [опции]
Java
bash
javac screenshot.java
java screenshot [опции]
📂 Состав репозитория
Язык	Файл	Статус
Python	screenshot.py	✅
Go	screenshot.go	✅
JavaScript	screenshot.js	✅
C++	screenshot.cpp	✅
C#	screenshot.cs	✅
Ruby	screenshot.rb	✅
Java	screenshot.java	✅
Все версии имеют одинаковый интерфейс и поведение.

🤝 Вклад в проект
Приветствуются улучшения:

Поддержка выбора монитора.

Захват отдельного окна.

Графический интерфейс.

Создавайте Issues и Pull Requests.

📜 Лицензия
MIT License – свободное использование, модификация и распространение.

📂 Исходный код
Первая строка каждого файла – его имя. Скопируйте блок целиком и сохраните в соответствующий файл.

