#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <cmath>
#include <sstream>
#include <iomanip>

// === 1. Целевой Интерфейс (Target) ===
// Интерфейс, который ожидает Клиент (Система управления)
class ITargetSensor {
public:
    virtual ~ITargetSensor() = default;
    // Клиент ожидает температуру в Цельсиях (C)
    virtual double GetTemperatureCelsius() const = 0;
};

// === 2. Адаптируемый Класс (Adaptee) ===
// Устаревший сенсор, интерфейс которого несовместим
class LegacyFahrenheitSensor {
public:
    double GetTemperatureFahrenheit() const {
        // Устаревшая система всегда возвращает температуру в Фаренгейтах
        double tempFahrenheit = 77.0; // Пример: 77°F
        std::cout << "[Adaptee] Legacy sensor returned: " << tempFahrenheit << " °F" << std::endl;
        return tempFahrenheit;
    }
};

// === 3. Адаптер (Adapter) ===
// Класс, который преобразует интерфейс Adaptee к интерфейсу Target
class SensorAdapter : public ITargetSensor {
private:
    // Композиция: Адаптер содержит объект адаптируемого класса
    std::unique_ptr<LegacyFahrenheitSensor> fahrenheitSensor_;

    // Вспомогательный метод преобразования
    double FahrenheitToCelsius(double tempF) const {
        return (tempF - 32.0) * 5.0 / 9.0;
    }

public:
    SensorAdapter() : fahrenheitSensor_(std::make_unique<LegacyFahrenheitSensor>()) {}
    
    // Реализация целевого интерфейса
    double GetTemperatureCelsius() const override {
        // 1. Вызов метода адаптируемого класса
        double tempF = fahrenheitSensor_->GetTemperatureFahrenheit();
        
        // 2. Преобразование данных
        double tempC = FahrenheitToCelsius(tempF);

        // 3. Возврат результата клиенту
        return tempC;
    }
};

// === 4. Клиент (Client) ===
// Использует только Целевой Интерфейс (ITargetSensor)
void ClientCode(const ITargetSensor& sensor) {
    std::cout << "\n[Client] Requesting temperature from control system (expecting °C)..." << std::endl;
    double tempC = sensor.GetTemperatureCelsius();
    
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "[Client] Received temperature: " << tempC << " °C." << std::endl;
    if (tempC > 24.0) {
        std::cout << "[Client] Output: AC required." << std::endl;
    } else {
        std::cout << "[Client] Output: Comfortable temperature." << std::endl;
    }
}