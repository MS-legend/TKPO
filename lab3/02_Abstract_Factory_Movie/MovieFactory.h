#pragma once
#include <iostream>
#include <string>
#include <memory>

// === 1. Абстрактные Продукты (Abstract Products) ===

class ЗвуковаяДорожка {
public:
    virtual ~ЗвуковаяДорожка() = default;
    virtual std::string получитьЯзык() const = 0;
    virtual std::string играть() const = 0;
};

class ФайлСубтитров {
public:
    virtual ~ФайлСубтитров() = default;
    virtual std::string получитьЯзык() const = 0;
    // Метод для демонстрации взаимодействия с другим продуктом из семейства
    virtual std::string проверитьСоответствие(const ЗвуковаяДорожка& дорожка) const = 0;
};

// === 2. Конкретные Продукты (Concrete Products) ===
// Продукты для Русского семейства
class РусскаяДорожка : public ЗвуковаяДорожка {
public:
    std::string получитьЯзык() const override { return "Русский"; }
    std::string играть() const override { return "Воспроизведение: [Русская Звуковая Дорожка]"; }
};

class РусскиеСубтитры : public ФайлСубтитров {
public:
    std::string получитьЯзык() const override { return "Русский"; }
    std::string проверитьСоответствие(const ЗвуковаяДорожка& дорожка) const override {
        if (дорожка.получитьЯзык() == "Русский") {
            return "Субтитры (Русский) соответствуют аудиодорожке.";
        } else {
            return "Ошибка: Субтитры (Русский) не соответствуют аудиодорожке (" + дорожка.получитьЯзык() + ").";
        }
    }
};

// Продукты для Английского семейства
class АнглийскаяДорожка : public ЗвуковаяДорожка {
public:
    std::string получитьЯзык() const override { return "Английский"; }
    std::string играть() const override { return "Playing: [English Audio Track]"; }
};

class АнглийскиеСубтитры : public ФайлСубтитров {
public:
    std::string получитьЯзык() const override { return "Английский"; }
    std::string проверитьСоответствие(const ЗвуковаяДорожка& дорожка) const override {
        if (дорожка.получитьЯзык() == "Английский") {
            return "Subtitles (English) match audio track.";
        } else {
            return "Error: Subtitles (English) do not match audio track (" + дорожка.получитьЯзык() + ").";
        }
    }
};

// === 3. Абстрактная Фабрика (Abstract Factory) ===
class АбстрактнаяФабрикаЯзыка {
public:
    virtual ~АбстрактнаяФабрикаЯзыка() = default;
    // Методы для создания продуктов из одного семейства
    virtual std::unique_ptr<ЗвуковаяДорожка> создатьЗвуковуюДорожку() const = 0;
    virtual std::unique_ptr<ФайлСубтитров> создатьФайлСубтитров() const = 0;
};

// === 4. Конкретные Фабрики (Concrete Factories) ===
class ФабрикаРусскогоЯзыка : public АбстрактнаяФабрикаЯзыка {
public:
    std::unique_ptr<ЗвуковаяДорожка> создатьЗвуковуюДорожку() const override {
        return std::make_unique<РусскаяДорожка>();
    }
    std::unique_ptr<ФайлСубтитров> создатьФайлСубтитров() const override {
        return std::make_unique<РусскиеСубтитры>();
    }
};

class ФабрикаАнглийскогоЯзыка : public АбстрактнаяФабрикаЯзыка {
public:
    std::unique_ptr<ЗвуковаяДорожка> создатьЗвуковуюДорожку() const override {
        return std::make_unique<АнглийскаяДорожка>();
    }
    std::unique_ptr<ФайлСубтитров> создатьФайлСубтитров() const override {
        return std::make_unique<АнглийскиеСубтитры>();
    }
};

// === 5. Клиентский код (Client) ===
void клиентскийКод(const АбстрактнаяФабрикаЯзыка& фабрика) {
    std::cout << "\nКлиент: Запрашиваем фильм на новом языке..." << std::endl;
    
    // Создаем целое семейство взаимосвязанных продуктов
    auto дорожка = фабрика.создатьЗвуковуюДорожку();
    auto субтитры = фабрика.создатьФайлСубтитров();
    
    std::cout << дорожка->играть() << std::endl;
    std::cout << "Проверка соответствия: " << субтитры->проверитьСоответствие(*дорожка) << std::endl;
}