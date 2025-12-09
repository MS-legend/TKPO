#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <functional> // Для std::function

// --- 1. Базовый класс Модели и Наследники (Contact) ---

class Contact {
protected:
    std::string name_;
    std::string phone_;
    std::string address_;

public:
    Contact(const std::string& name, const std::string& phone, const std::string& address)
        : name_(name), phone_(phone), address_(address) {}
    
    virtual ~Contact() = default;
    
    // Геттеры для доступа к полям (необходимы для стратегий сортировки)
    const std::string& GetName() const { return name_; }
    const std::string& GetPhone() const { return phone_; }
    const std::string& GetAddress() const { return address_; }

    // Виртуальный метод для получения полной информации о контакте
    virtual std::string ToString() const {
        return "Name: " + name_ + ", Phone: " + phone_ + ", Address: " + address_;
    }
};

class LegalContact : public Contact {
private:
    std::string legal_form_; // Например, ООО, АО

public:
    LegalContact(const std::string& name, const std::string& phone, const std::string& address, const std::string& legalForm)
        : Contact(name, phone, address), legal_form_(legalForm) {}

    std::string ToString() const override {
        return Contact::ToString() + ", Type: Legal (" + legal_form_ + ")";
    }
};

class PhysicalContact : public Contact {
private:
    std::string personal_email_;

public:
    PhysicalContact(const std::string& name, const std::string& phone, const std::string& address, const std::string& email)
        : Contact(name, phone, address), personal_email_(email) {}

    std::string ToString() const override {
        return Contact::ToString() + ", Type: Physical, Email: " + personal_email_;
    }
};

// --- 2. Паттерн Стратегия: Интерфейс и Конкретные Стратегии ---

// Интерфейс Стратегии
class ISortStrategy {
public:
    virtual ~ISortStrategy() = default;
    // Метод для выполнения алгоритма сортировки
    virtual void Sort(std::vector<Contact*>& records) const = 0;
    virtual std::string GetName() const = 0;
};

// Конкретная Стратегия A: Сортировка по имени (Name)
class SortByNameStrategy : public ISortStrategy {
public:
    std::string GetName() const override { return "by Name"; }
    void Sort(std::vector<Contact*>& records) const override {
        std::sort(records.begin(), records.end(), 
            [](const Contact* a, const Contact* b) {
                return a->GetName() < b->GetName();
            });
    }
};

// Конкретная Стратегия B: Сортировка по телефону (Phone)
class SortByPhoneStrategy : public ISortStrategy {
public:
    std::string GetName() const override { return "by Phone"; }
    void Sort(std::vector<Contact*>& records) const override {
        std::sort(records.begin(), records.end(), 
            [](const Contact* a, const Contact* b) {
                return a->GetPhone() < b->GetPhone();
            });
    }
};

// --- 3. Контекст (Основной класс) - Directory ---

class Directory {
private:
    std::string title_;
    std::string ownerName_;
    std::vector<Contact*> records_;
    // Поле, хранящее ссылку на экземпляр класса Стратегия
    ISortStrategy* sortStrategy_ = nullptr;

public:
    Directory(const std::string& title, const std::string& ownerName)
        : title_(title), ownerName_(ownerName) {
        std::cout << "[Directory] Created: " << title_ << std::endl;
    }
    
    ~Directory() {
        // Освобождение памяти, занятой элементами массива
        for (auto& record : records_) {
            delete record;
        }
        std::cout << "[Directory] Destroyed: " << title_ << std::endl;
    }

    // Метод для добавления записи
    void AddContact(Contact* record) {
        records_.push_back(record);
        std::cout << "[Directory] Added contact: " << record->GetName() << std::endl;
    }

    // Метод для установки стратегии (динамическая смена поведения)
    void SetSortStrategy(ISortStrategy* strategy) {
        sortStrategy_ = strategy;
        std::cout << "[Directory] Sorting strategy set to: " << strategy->GetName() << std::endl;
    }

    // Метод, делегирующий выполнение стратегии
    void SortRecords() {
        if (!sortStrategy_) {
            std::cout << "[Directory] Error: Sorting strategy is not set. Cannot sort." << std::endl;
            return;
        }
        // Делегируем алгоритм текущей стратегии
        sortStrategy_->Sort(records_);
        std::cout << "[Directory] Records successfully sorted " << sortStrategy_->GetName() << "." << std::endl;
    }
    
    // Вывод записей
    void DisplayRecords() const {
        std::cout << "\n=== Directory: " << title_ << " (Owner: " << ownerName_ << ") ===" << std::endl;
        for (size_t i = 0; i < records_.size(); ++i) {
            std::cout << "[" << i + 1 << "] " << records_[i]->ToString() << std::endl;
        }
        std::cout << "---------------------------------------" << std::endl;
    }
};