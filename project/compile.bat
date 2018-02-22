@echo off
echo.
g++ src/main.cpp -Wall -Iinclude -Llib -lmingw32 -lSDL2main -lSDL2 -lglad -lopengl32 -o bin/main
start bin/main.exe