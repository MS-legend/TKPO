#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <ctime>
#include <functional> // ИСПРАВЛЕНИЕ: Добавлено для std::function

// --- I. Паттерн Компоновщик (Composite) ---

// 1.1. Базовый Компонент (Component)
class ContactComponent {
protected:
    std::string name_;

public:
    ContactComponent(const std::string& name) : name_(name) {}
    virtual ~ContactComponent() = default;

    virtual void Display(int depth = 0) const = 0;
    virtual std::string GetName() const { return name_; }

    // Методы для Composite
    virtual void Add(ContactComponent* component) {}
    virtual void Remove(ContactComponent* component) {}

    // Метод для Chain of Responsibility (Chain CoR)
    virtual std::string GetDataForValidation() const = 0;
};

// 1.2. Лист (Leaf) - Контакты из ЛР7
class Contact : public ContactComponent {
protected:
    std::string phone_;
    std::string address_;

public:
    Contact(const std::string& name, const std::string& phone, const std::string& address)
        : ContactComponent(name), phone_(phone), address_(address) {}
    
    // Имплементация для Display
    void Display(int depth) const override {
        std::cout << std::string(depth, ' ') << "- " << name_ 
                  << " (Phone: " << phone_ << ")" << std::endl;
    }
    
    // Имплементация для Chain CoR
    std::string GetDataForValidation() const override {
        return name_ + "|" + phone_ + "|" + address_;
    }
    
    const std::string& GetPhone() const { return phone_; }
    void SetPhone(const std::string& newPhone) { phone_ = newPhone; }
};

class PhysicalContact : public Contact {
public:
    PhysicalContact(const std::string& name, const std::string& phone, const std::string& address)
        : Contact(name, phone, address) {}
};

class LegalContact : public Contact {
public:
    LegalContact(const std::string& name, const std::string& phone, const std::string& address)
        : Contact(name, phone, address) {}
};

// 1.3. Контейнер (Composite) - Группа контактов
class ContactGroup : public ContactComponent {
private:
    std::vector<ContactComponent*> children_;

public:
    ContactGroup(const std::string& name) : ContactComponent(name) {}
    
    ~ContactGroup() override {
        for (auto& child : children_) {
            delete child;
        }
    }

    void Add(ContactComponent* component) override {
        children_.push_back(component);
    }

    void Remove(ContactComponent* component) override {
        children_.erase(std::remove(children_.begin(), children_.end(), component), children_.end());
    }

    void Display(int depth) const override {
        std::cout << std::string(depth, ' ') << "+ " << name_ << " (Group)" << std::endl;
        for (const auto& component : children_) {
            component->Display(depth + 2);
        }
    }
    
    // В случае Composite, просто возвращаем имя (или комбинируем данные)
    std::string GetDataForValidation() const override { return name_; }

    const std::vector<ContactComponent*>& GetChildren() const { return children_; }
};

// --- II. Паттерн Цепочка Обязанностей (Chain of Responsibility) ---

// 2.1. Интерфейс Обработчика
class IValidator {
public:
    virtual ~IValidator() = default;
    // Устанавливает следующий обработчик в цепочке
    virtual IValidator* SetNext(IValidator* handler) = 0;
    // Обрабатывает запрос. Возвращает статус или nullptr
    virtual std::string Handle(ContactComponent* contact) = 0;
};

// 2.2. Базовый Обработчик
class BaseValidator : public IValidator {
private:
    IValidator* nextHandler_ = nullptr;

public:
    IValidator* SetNext(IValidator* handler) override {
        this->nextHandler_ = handler;
        return handler;
    }

    std::string Handle(ContactComponent* contact) override {
        if (this->nextHandler_) {
            return this->nextHandler_->Handle(contact);
        }
        return "[SUCCESS] All checks passed."; // Конец цепочки
    }
};

// 2.3. Конкретный Обработчик 1: Проверка имени
class NameValidator : public BaseValidator {
public:
    std::string Handle(ContactComponent* contact) override {
        if (contact->GetName().length() < 3) {
            return "[FAILURE] Name check failed: Name too short.";
        }
        std::cout << "  > NameValidator: Passed." << std::endl;
        return BaseValidator::Handle(contact);
    }
};

// 2.4. Конкретный Обработчик 2: Проверка наличия телефона (только для Leaf)
class PhoneValidator : public BaseValidator {
public:
    std::string Handle(ContactComponent* contact) override {
        // Проверяем, является ли компонент контактом (Leaf)
        Contact* leaf = dynamic_cast<Contact*>(contact);
        if (leaf && leaf->GetPhone().empty()) {
             return "[FAILURE] Phone check failed: Phone number is empty.";
        }
        std::cout << "  > PhoneValidator: Passed." << std::endl;
        return BaseValidator::Handle(contact);
    }
};

// --- III. Паттерн Снимок (Memento) ---

// 3.1. Снимок (Memento)
class Memento {
private:
    std::string state_;
    std::string date_;

public:
    Memento(const std::string& state) : state_(state) {
        // Сохраняем текущее время для отслеживания
        time_t now = time(0);
        date_ = ctime(&now);
    }

    // Доступ к сохраненному состоянию (для Originator)
    const std::string& GetState() const { return state_; }
    
    // Доступ к метаданным (для Caretaker)
    std::string GetName() const { return "Snapshot taken at " + date_; }
};

// 3.2. Создатель (Originator) - Вся структура Справочника
class Directory {
private:
    std::unique_ptr<ContactGroup> root_;
    std::string title_;

public:
    Directory(const std::string& title) : title_(title) {
        root_ = std::make_unique<ContactGroup>("Root Directory");
    }

    ContactGroup* GetRoot() const { return root_.get(); }
    void Display() const { root_->Display(0); }
    const std::string& GetTitle() const { return title_; }

    // Создание Снимка (Сохранение состояния)
    Memento* Save() {
        std::string state;
        // Простая сериализация: список всех имен контактов
        std::function<void(ContactComponent*)> serialize = 
            [&state, &serialize](ContactComponent* comp) {
            state += comp->GetName() + ";";
            if (ContactGroup* group = dynamic_cast<ContactGroup*>(comp)) {
                for (auto& child : group->GetChildren()) {
                    serialize(child);
                }
            }
        };
        serialize(root_.get());
        std::cout << "\n[Memento] Saving state for " << title_ << ". Size: " << state.length() << std::endl;
        return new Memento(state);
    }

    // Восстановление состояния из Снимка
    void Restore(Memento* memento) {
        std::cout << "\n[Memento] Restoring state from snapshot..." << std::endl;
        // В реальной системе здесь будет десериализация
        // Для демонстрации: просто выводим восстановленное состояние
        std::cout << "  > Restored Content (Names): " << memento->GetState() << std::endl;
        // Для простоты: сбрасываем текущее дерево
        root_ = std::make_unique<ContactGroup>("Restored Directory"); 
        std::cout << "[Directory] Restored to a basic state (demonstration)." << std::endl;
    }
};

// 3.3. Хранитель (Caretaker)
class History {
private:
    std::vector<Memento*> mementos_;
    Directory* originator_;

public:
    History(Directory* originator) : originator_(originator) {}
    ~History() {
        for (auto& m : mementos_) {
            delete m;
        }
    }

    void Backup() {
        Memento* m = originator_->Save();
        mementos_.push_back(m);
    }

    void Undo() {
        if (mementos_.empty()) {
            std::cout << "[History] No saved states to restore." << std::endl;
            return;
        }
        Memento* memento = mementos_.back();
        mementos_.pop_back();
        std::cout << "[History] Restoring: " << memento->GetName() << std::endl;
        originator_->Restore(memento);
        delete memento; // Освобождаем память после использования
    }
};