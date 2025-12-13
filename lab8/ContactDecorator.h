#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

// --- 1. Интерфейс Компонента (Component) ---
// Базовый класс Contact выступает в роли Component (определяет основной интерфейс)
class Contact {
protected:
    std::string name_;
    std::string phone_;
    std::string address_;

public:
    Contact(const std::string& name, const std::string& phone, const std::string& address)
        : name_(name), phone_(phone), address_(address) {}
    
    virtual ~Contact() = default;
    
    // Основная операция, которую будем декорировать
    virtual std::string ToString() const {
        return "Name: " + name_ + ", Phone: " + phone_ + ", Address: " + address_;
    }
    
    // Дополнительный метод для демонстрации вывода
    virtual void Display() const {
        std::cout << "  [Base Contact] " << ToString() << std::endl;
    }
    
    const std::string& GetName() const { return name_; }
};

// --- 2. Конкретные Компоненты (Concrete Components) - Наследники из ЛР7 ---

class LegalContact : public Contact {
private:
    std::string legal_form_; 

public:
    LegalContact(const std::string& name, const std::string& phone, const std::string& address, const std::string& legalForm)
        : Contact(name, phone, address), legal_form_(legalForm) {}

    std::string ToString() const override {
        return Contact::ToString() + ", Type: Legal (" + legal_form_ + ")";
    }
    
    void Display() const override {
        std::cout << "  [Legal Entity] " << ToString() << std::endl;
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
    
    void Display() const override {
        std::cout << "  [Physical Person] " << ToString() << std::endl;
    }
};

// --- 3. Базовый Декоратор (Base Decorator) ---

class ContactDecorator : public Contact {
protected:
    // Ссылка на декорируемый объект (The Component/Concrete Component)
    std::unique_ptr<Contact> wrappee_; 

public:
    // Декоратор принимает Компонент через конструктор
    ContactDecorator(std::unique_ptr<Contact> component)
        : Contact(component->GetName(), "", ""), wrappee_(std::move(component)) {}

    // Декоратор имеет тот же интерфейс и делегирует вызов
    std::string ToString() const override {
        return wrappee_->ToString(); 
    }
    
    void Display() const override {
        wrappee_->Display();
    }
};

// --- 4. Конкретные Декораторы (Concrete Decorators) ---

// Декоратор: Добавление приоритета (функциональность ПОСЛЕ базовой)
class PriorityDecorator : public ContactDecorator {
private:
    std::string priority_level_;

public:
    PriorityDecorator(std::unique_ptr<Contact> component, const std::string& priority)
        : ContactDecorator(std::move(component)), priority_level_(priority) {}

    std::string ToString() const override {
        // Вызов базового компонента + добавление новой функциональности
        return ContactDecorator::ToString() + " | [Priority: " + priority_level_ + "]";
    }
    
    void Display() const override {
        // Добавление функциональности (вывод в консоль)
        ContactDecorator::Display();
        std::cout << "  >> Added Feature: Priority Level set to " << priority_level_ << std::endl;
    }
};

// Декоратор: Добавление статуса верификации (функциональность ПЕРЕД базовой)
class VerificationDecorator : public ContactDecorator {
private:
    std::string status_;

public:
    VerificationDecorator(std::unique_ptr<Contact> component, const std::string& status)
        : ContactDecorator(std::move(component)), status_(status) {}

    std::string ToString() const override {
        // Добавление нашей функциональности + вызов базового компонента
        return "[Verified: " + status_ + "] | " + ContactDecorator::ToString();
    }
    
    void Display() const override {
        std::cout << "  >> Added Feature: Verification Status: " << status_ << std::endl;
        ContactDecorator::Display();
    }
};