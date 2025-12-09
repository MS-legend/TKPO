#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <random>
#include <chrono>

// === 1. Абстрактный Продукт (Product) ===
// Интерфейс для всех фигур.
class Фигура {
protected:
    int размерКлеток;
    std::string форма;
public:
    Фигура(int размер, const std::string& ф) : размерКлеток(размер), форма(ф) {}
    virtual ~Фигура() = default;
    
    // Общий интерфейс для манипуляции абстрактными объектами
    virtual std::string получитьТип() const = 0;
    virtual void отрисовать() const = 0;
};

// === 2. Конкретные Продукты (Concrete Products) ===
// Конкретные фигуры, которые создает фабрика.
class IОбразнаяФигура : public Фигура {
public:
    IОбразнаяФигура() : Фигура(4, "I-образная") {}
    std::string получитьТип() const override { return "Обычная I-фигура (4 клетки)"; }
    void отрисовать() const override {
        std::cout << "  Отрисовка: [ ][ ][ ][ ] " << std::endl;
    }
};

class JОбразнаяФигура : public Фигура {
public:
    JОбразнаяФигура() : Фигура(4, "J-образная") {}
    std::string получитьТип() const override { return "Обычная J-фигура (4 клетки)"; }
    void отрисовать() const override {
        std::cout << "  Отрисовка: [ ]\n  [ ][ ][ ] " << std::endl;
    }
};

class СуперФигура : public Фигура {
public:
    // Супер-фигура с большим числом клеток (6 клеток)
    СуперФигура() : Фигура(6, "Супер-фигура") {}
    std::string получитьТип() const override { return "Супер-фигура (6 клеток)"; }
    void отрисовать() const override {
        std::cout << "  Отрисовка: [ ][ ][ ]\n  [ ][ ][ ] " << std::endl;
    }
};

// === 3. Абстрактный Создатель (Creator) ===
// Объявляет Фабричный Метод.
class ФабрикаФигур {
public:
    virtual ~ФабрикаФигур() = default;
    
    // ФАБРИЧНЫЙ МЕТОД: возвращает абстрактный продукт. 
    virtual std::unique_ptr<Фигура> создатьФигуру() const = 0;

    // Вспомогательный метод: работает с абстракцией, 
    // не зная, какой конкретно продукт будет создан.
    void операцияПолученияФигуры() const {
        std::unique_ptr<Фигура> фигура = this->создатьФигуру();
        std::cout << "Фабрика успешно создала: " << фигура->получитьТип() << std::endl;
        фигура->отрисовать();
    }
};

// === 4. Конкретные Создатели (Concrete Creators) ===

class ОбычнаяФабрика : public ФабрикаФигур {
public:
    // Переопределение Фабричного метода: реализует логику случайного выбора
    std::unique_ptr<Фигура> создатьФигуру() const override {
        // Случайный выбор из конечного набора обычных фигур
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, 1);
        
        if (distrib(gen) == 0) {
            return std::make_unique<IОбразнаяФигура>();
        } else {
            return std::make_unique<JОбразнаяФигура>();
        }
    }
};

class СуперФабрика : public ФабрикаФигур {
public:
    // Переопределение Фабричного метода: всегда возвращает Супер-фигуру
    std::unique_ptr<Фигура> создатьФигуру() const override {
        return std::make_unique<СуперФигура>();
    }
};