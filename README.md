# Console Manager
## Чтобы заработал код нужно
1. Склонировать репрезеторий библиотек vcpkg используя консоль cmd: ```git clone https://github.com/microsoft/vcpkg.git```
2. Перейти в папку скаченой фигни: ```cd vcpkg```
3. Запустить скрипт начальной загрузки: ```bootstrap-vcpkg.bat```
4. Заинстолить конкретную библиотеку: ```vcpkg install nlohmann-json```
5. Скачать с GitHub или клонировать мой репрезеторий в Visual Studio (это один из вариантов "Начала работы" справа при запуске VS)
6. В открытом проекте в верхней панели перейти в "Вид" -> "Терминал"
7. Добавить vcpkg в path: ```$env:VCPKG_ROOT = "C:\path\to\vcpkg"``` ```$env:PATH = "$env:VCPKG_ROOT;$env:PATH"```
8. Прописать: ```vcpkg integrate install``` (Для этого нужны права админа)
