#pragma once
#include <iostream>
#include <string>
#include <memory>

class Elevator; // Предварительное объявление класса Контекста

// === 1. Интерфейс Состояния (State Interface) ===
// Определяет общий интерфейс для всех состояний лифта
class IElevatorState {
public:
    virtual ~IElevatorState() = default;
    virtual std::string GetName() const = 0;
    
    // Операции (триггеры), которые могут быть вызваны
    virtual void Call(Elevator* elevator, int floor) = 0;
    virtual void Load(Elevator* elevator) = 0;
    virtual void Unload(Elevator* elevator) = 0;
    virtual void RestorePower(Elevator* elevator) = 0;
    virtual void Emergency(Elevator* elevator) = 0;
};

// === 2. Класс Контекст (Context / Конечный автомат) ===
// Лифт – объект, поведение которого меняется в зависимости от state_
class Elevator {
private:
    std::unique_ptr<IElevatorState> state_;
    int currentFloor_;
    bool isOverloaded_ = false;

public:
    Elevator(std::unique_ptr<IElevatorState> initialState) : currentFloor_(1) {
        // Установка начального состояния
        this->state_ = std::move(initialState);
        std::cout << "Elevator initialized. Current State: " << state_->GetName() << std::endl;
    }

    // Метод для смены состояния
    void ChangeState(std::unique_ptr<IElevatorState> newState) {
        std::cout << "Context: Changing state from " << state_->GetName() << " to " << newState->GetName() << std::endl;
        state_ = std::move(newState);
    }

    // Методы, делегирующие выполнение текущему состоянию
    void Call(int floor) { state_->Call(this, floor); }
    void Load() { state_->Load(this); }
    void Unload() { state_->Unload(this); }
    void RestorePower() { state_->RestorePower(this); }
    void Emergency() { state_->Emergency(this); }
    
    // Вспомогательные методы
    void SetFloor(int floor) { currentFloor_ = floor; }
    int GetFloor() const { return currentFloor_; }
    void SetOverloaded(bool isOverloaded) { isOverloaded_ = isOverloaded; }
    bool IsOverloaded() const { return isOverloaded_; }
    std::string GetCurrentStateName() const { return state_->GetName(); }
};

// --- Вспомогательные функции для получения экземпляров состояний (Singleton) ---
// Используются для управления жизненным циклом состояний
IElevatorState* GetStandingState();
IElevatorState* GetMovingState();
IElevatorState* GetOverloadedState();
IElevatorState* GetNoPowerState();
IElevatorState* GetMalfunctionState();


// === 3. Конкретные Состояния (Concrete States) ===

// 3.1. StandingState (Стоянка)
class StandingState : public IElevatorState {
public:
    std::string GetName() const override { return "Standing"; }
    
    void Call(Elevator* elevator, int floor) override {
        if (floor == elevator->GetFloor()) {
            std::cout << "Standing: Already on floor " << floor << "." << std::endl;
            return;
        }
        if (elevator->IsOverloaded()) {
            std::cout << "Standing: Cannot move, elevator is overloaded." << std::endl;
            // Переход в состояние Перегружен
            elevator->ChangeState(std::unique_ptr<IElevatorState>(GetOverloadedState()));
            return;
        }
        
        std::cout << "Standing: Moving from floor " << elevator->GetFloor() << " to " << floor << "." << std::endl;
        // Переход в состояние Движение
        elevator->ChangeState(std::unique_ptr<IElevatorState>(GetMovingState()));
        elevator->SetFloor(floor);
    }

    void Load(Elevator* elevator) override {
        std::cout << "Standing: Loading passengers..." << std::endl;
        // Имитация перегрузки при загрузке
        elevator->SetOverloaded(true);
        // Переход в состояние Перегружен
        elevator->ChangeState(std::unique_ptr<IElevatorState>(GetOverloadedState()));
    }
    
    void Unload(Elevator* elevator) override {
        std::cout << "Standing: Unloading completed. Doors closed." << std::endl;
    }
    
    void RestorePower(Elevator* elevator) override {
        std::cout << "Standing: Power is already OK. No action." << std::endl;
    }
    
    void Emergency(Elevator* elevator) override {
        // Переход в состояние Авария
        elevator->ChangeState(std::unique_ptr<IElevatorState>(GetMalfunctionState()));
    }
};


// 3.2. MovingState (Движение)
class MovingState : public IElevatorState {
public:
    std::string GetName() const override { return "Moving"; }

    void Call(Elevator* elevator, int floor) override {
        std::cout << "Moving: Target floor changed to " << floor << ". Continuing movement." << std::endl;
        elevator->SetFloor(floor);
    }

    void Load(Elevator* elevator) override {
        std::cout << "Moving: Cannot load while moving." << std::endl;
    }
    
    void Unload(Elevator* elevator) override {
        std::cout << "Moving: Cannot unload while moving." << std::endl;
    }

    void RestorePower(Elevator* elevator) override {
        std::cout << "Moving: Power is OK. Continuing movement." << std::endl;
    }

    void Emergency(Elevator* elevator) override {
        // Переход в состояние Авария
        elevator->ChangeState(std::unique_ptr<IElevatorState>(GetMalfunctionState()));
    }
};


// 3.3. OverloadedState (Перегружен)
class OverloadedState : public IElevatorState {
public:
    std::string GetName() const override { return "Overloaded"; }

    void Call(Elevator* elevator, int floor) override {
        std::cout << "Overloaded: Cannot move until unloaded! Alarm active." << std::endl;
    }

    void Load(Elevator* elevator) override {
        std::cout << "Overloaded: Cannot load more weight. Alarm active." << std::endl;
    }
    
    void Unload(Elevator* elevator) override {
        // После разгрузки (если перегрузка устранена), переходим в Standing
        elevator->SetOverloaded(false);
        elevator->ChangeState(std::unique_ptr<IElevatorState>(GetStandingState()));
        std::cout << "Overloaded: Weight reduced. State restored to Standing." << std::endl;
    }

    void RestorePower(Elevator* elevator) override {
        std::cout << "Overloaded: Power is OK, but elevator is overloaded." << std::endl;
    }

    void Emergency(Elevator* elevator) override {
        // Переход в состояние Авария
        elevator->ChangeState(std::unique_ptr<IElevatorState>(GetMalfunctionState()));
    }
};


// 3.4. NoPowerState (Нет питания)
class NoPowerState : public IElevatorState {
public:
    std::string GetName() const override { return "NoPower"; }

    void Call(Elevator* elevator, int floor) override {
        std::cout << "NoPower: Cannot move. Waiting for power restoration." << std::endl;
    }

    void Load(Elevator* elevator) override {
        std::cout << "NoPower: Cannot load. Doors are probably locked." << std::endl;
    }
    
    void Unload(Elevator* elevator) override {
        std::cout << "NoPower: Cannot unload. Doors are probably locked." << std::endl;
    }

    void RestorePower(Elevator* elevator) override {
        // Переход в стоячее состояние
        elevator->ChangeState(std::unique_ptr<IElevatorState>(GetStandingState()));
        std::cout << "NoPower: Power restored! Elevator is now Standing." << std::endl;
    }

    void Emergency(Elevator* elevator) override {
        std::cout << "NoPower: Emergency triggered, but power is already out. Stays in NoPower state." << std::endl;
    }
};


// 3.5. MalfunctionState (Авария)
class MalfunctionState : public IElevatorState {
public:
    std::string GetName() const override { return "Malfunction"; }

    void Call(Elevator* elevator, int floor) override {
        std::cout << "Malfunction: Emergency stop! Waiting for maintenance." << std::endl;
    }

    void Load(Elevator* elevator) override {
        std::cout << "Malfunction: Emergency stop! Loading disabled." << std::endl;
    }
    
    void Unload(Elevator* elevator) override {
        std::cout << "Malfunction: Emergency stop! Unloading disabled." << std::endl;
    }

    void RestorePower(Elevator* elevator) override {
        std::cout << "Malfunction: Power is restored, but manual reset is required." << std::endl;
    }

    void Emergency(Elevator* elevator) override {
        std::cout << "Malfunction: Already in emergency state." << std::endl;
    }
};

// --- Реализация Singleton для состояний ---
IElevatorState* GetStandingState() { static StandingState instance; return &instance; }
IElevatorState* GetMovingState() { static MovingState instance; return &instance; }
IElevatorState* GetOverloadedState() { static OverloadedState instance; return &instance; }
IElevatorState* GetNoPowerState() { static NoPowerState instance; return &instance; }
IElevatorState* GetMalfunctionState() { static MalfunctionState instance; return &instance; }