#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <map>

// Предварительное объявление классов
class Observer;
class ElevatorModel;
class IElevatorState;

// --- 0. Паттерн Наблюдатель (Observer) - Основа связи View-Model ---

class Observer {
public:
    virtual ~Observer() = default;
    // Метод, вызываемый Моделью при изменении данных
    virtual void update() = 0;
};

class Observable {
protected:
    std::vector<Observer*> observers_;
public:
    void AddObserver(Observer *observer) {
        observers_.push_back(observer);
        std::cout << "[Observable] Observer subscribed." << std::endl;
    }
    void NotifyUpdate() {
        std::cout << "\n[Model] Notifying all Views about data change..." << std::endl;
        for (Observer* obs : observers_) {
            obs->update();
        }
    }
    // Удаление наблюдателя опущено для краткости ЛР
};

// --- 1. Интерфейс Состояния (State Interface) ---

class IElevatorState {
public:
    virtual ~IElevatorState() = default;
    virtual std::string GetName() const = 0;
    
    // Операции (триггеры), которые могут быть вызваны Контроллером
    virtual void Call(ElevatorModel* model, int floor) = 0;
    virtual void Load(ElevatorModel* model) = 0;
    virtual void Unload(ElevatorModel* model) = 0;
    virtual void RestorePower(ElevatorModel* model) = 0;
    virtual void Emergency(ElevatorModel* model) = 0;
};

// --- Вспомогательные функции для получения экземпляров состояний (Singleton) ---
// Используются для управления жизненным циклом состояний
IElevatorState* GetStandingState();
IElevatorState* GetMovingState();
IElevatorState* GetOverloadedState();
IElevatorState* GetNoPowerState();
IElevatorState* GetMalfunctionState();


// --- 2. Модель (Model) - Контекст для Состояния и Observable ---

class ElevatorModel : public Observable {
private:
    std::unique_ptr<IElevatorState> state_;
    int currentFloor_ = 1;
    bool isOverloaded_ = false;
    
public:
    // Конструктор инициализирует начальное состояние
    ElevatorModel(std::unique_ptr<IElevatorState> initialState) : currentFloor_(1) {
        this->state_ = std::move(initialState);
        std::cout << "Model initialized. Current Floor: " << currentFloor_ << ", State: " << state_->GetName() << std::endl;
    }

    // Метод для смены состояния (вызывается из Concrete States)
    void ChangeState(std::unique_ptr<IElevatorState> newState) {
        std::cout << "Context: Changing state from " << state_->GetName() << " to " << newState->GetName() << std::endl;
        state_ = std::move(newState);
        // Ключевой момент MVC: оповещаем Представления после изменения Модели
        NotifyUpdate();
    }

    // --- Геттеры для View ---
    std::string GetCurrentStateName() const { return state_->GetName(); }
    int GetFloor() const { return currentFloor_; }
    bool IsOverloaded() const { return isOverloaded_; }

    // --- Сеттеры для State/Controller ---
    void SetFloor(int floor) { currentFloor_ = floor; }
    void SetOverloaded(bool isOverloaded) { 
        if (isOverloaded_ != isOverloaded) {
            isOverloaded_ = isOverloaded; 
            // Оповещаем, если меняется только флаг (без смены состояния)
            NotifyUpdate();
        }
    }

    // --- Операции (триггеры, вызываемые Контроллером) ---
    void Call(int floor) { state_->Call(this, floor); }
    void Load() { state_->Load(this); }
    void Unload() { state_->Unload(this); }
    void RestorePower() { state_->RestorePower(this); }
    void Emergency() { state_->Emergency(this); }
};

// --- 3. Конкретные Состояния (Concrete States) - Используют ElevatorModel* как контекст ---

class StandingState : public IElevatorState {
public:
    std::string GetName() const override { return "Standing"; }
    void Call(ElevatorModel* model, int floor) override {
        if (floor == model->GetFloor()) {
            std::cout << "Standing: Already on floor " << floor << "." << std::endl;
            return;
        }
        if (model->IsOverloaded()) {
            std::cout << "Standing: Cannot move, elevator is overloaded." << std::endl;
            model->ChangeState(std::unique_ptr<IElevatorState>(GetOverloadedState()));
            return;
        }
        std::cout << "Standing: Moving from floor " << model->GetFloor() << " to " << floor << "." << std::endl;
        model->ChangeState(std::unique_ptr<IElevatorState>(GetMovingState()));
        model->SetFloor(floor);
    }
    void Load(ElevatorModel* model) override {
        std::cout << "Standing: Loading passengers... (Simulating overload)" << std::endl;
        model->SetOverloaded(true);
        model->ChangeState(std::unique_ptr<IElevatorState>(GetOverloadedState()));
    }
    void Unload(ElevatorModel* model) override {
        std::cout << "Standing: Unloading completed. Doors closed." << std::endl;
    }
    void RestorePower(ElevatorModel* model) override { /* No op */ }
    void Emergency(ElevatorModel* model) override {
        model->ChangeState(std::unique_ptr<IElevatorState>(GetMalfunctionState()));
    }
};

class MovingState : public IElevatorState {
public:
    std::string GetName() const override { return "Moving"; }
    void Call(ElevatorModel* model, int floor) override {
        std::cout << "Moving: Target floor changed to " << floor << ". Continuing movement." << std::endl;
        model->SetFloor(floor);
        model->NotifyUpdate(); // Обновление View, так как изменилась цель движения
    }
    void Load(ElevatorModel* model) override { std::cout << "Moving: Cannot load while moving." << std::endl; }
    void Unload(ElevatorModel* model) override { 
        std::cout << "Moving: Reached floor. Stopping." << std::endl;
        model->ChangeState(std::unique_ptr<IElevatorState>(GetStandingState()));
    }
    void RestorePower(ElevatorModel* model) override { /* No op */ }
    void Emergency(ElevatorModel* model) override {
        model->ChangeState(std::unique_ptr<IElevatorState>(GetMalfunctionState()));
    }
};

class OverloadedState : public IElevatorState {
public:
    std::string GetName() const override { return "Overloaded"; }
    void Call(ElevatorModel* model, int floor) override {
        std::cout << "Overloaded: Cannot move until unloaded! Alarm active." << std::endl;
    }
    void Load(ElevatorModel* model) override {
        std::cout << "Overloaded: Cannot load more weight. Alarm active." << std::endl;
    }
    void Unload(ElevatorModel* model) override {
        // После разгрузки (если перегрузка устранена), переходим в Standing
        model->SetOverloaded(false); // Сброс флага и оповещение
        model->ChangeState(std::unique_ptr<IElevatorState>(GetStandingState()));
        std::cout << "Overloaded: Weight reduced. State restored to Standing." << std::endl;
    }
    void RestorePower(ElevatorModel* model) override { /* No op */ }
    void Emergency(ElevatorModel* model) override {
        model->ChangeState(std::unique_ptr<IElevatorState>(GetMalfunctionState()));
    }
};

class NoPowerState : public IElevatorState {
public:
    std::string GetName() const override { return "NoPower"; }
    void Call(ElevatorModel* model, int floor) override {
        std::cout << "NoPower: Cannot move. Waiting for power restoration." << std::endl;
    }
    void Load(ElevatorModel* model) override { /* No op */ }
    void Unload(ElevatorModel* model) override { /* No op */ }
    void RestorePower(ElevatorModel* model) override {
        model->ChangeState(std::unique_ptr<IElevatorState>(GetStandingState()));
        std::cout << "NoPower: Power restored! Elevator is now Standing." << std::endl;
    }
    void Emergency(ElevatorModel* model) override { /* No op */ }
};

class MalfunctionState : public IElevatorState {
public:
    std::string GetName() const override { return "Malfunction"; }
    void Call(ElevatorModel* model, int floor) override {
        std::cout << "Malfunction: Emergency stop! Waiting for maintenance." << std::endl;
    }
    void Load(ElevatorModel* model) override { /* No op */ }
    void Unload(ElevatorModel* model) override { /* No op */ }
    void RestorePower(ElevatorModel* model) override { /* No op */ }
    void Emergency(ElevatorModel* model) override { /* No op */ }
};

// --- Реализация Singleton для состояний ---
IElevatorState* GetStandingState() { static StandingState instance; return &instance; }
IElevatorState* GetMovingState() { static MovingState instance; return &instance; }
IElevatorState* GetOverloadedState() { static OverloadedState instance; return &instance; }
IElevatorState* GetNoPowerState() { static NoPowerState instance; return &instance; }
IElevatorState* GetMalfunctionState() { static MalfunctionState instance; return &instance; }


// --- 4. Представление (View) ---

class AbstractElevatorView : public Observer {
protected:
    ElevatorModel* model_;
public:
    AbstractElevatorView(ElevatorModel* model) : model_(model) {
        this->model_->AddObserver(this);
    }
    virtual void print() = 0;
};

// 4.1. Конкретное Представление 1 (Стандартный вывод)
class ConsoleView1 : public AbstractElevatorView {
public:
    ConsoleView1(ElevatorModel* model) : AbstractElevatorView(model) {}
    
    void update() override {
        std::cout << "\n--- VIEW 1 UPDATE (Standard) ---" << std::endl;
        print();
    }
    
    void print() override {
        std::cout << "  [STATUS] Floor: " << model_->GetFloor() 
                  << ", State: " << model_->GetCurrentStateName() << std::endl;
    }
};

// 4.2. Конкретное Представление 2 (Детализированный вывод)
class ConsoleView2 : public AbstractElevatorView {
public:
    ConsoleView2(ElevatorModel* model) : AbstractElevatorView(model) {}
    
    void update() override {
        std::cout << "\n--- VIEW 2 UPDATE (Detailed) ---" << std::endl;
        print();
    }
    
    void print() override {
        std::cout << "  [LIFT INFO] Current Floor: " << model_->GetFloor() << std::endl;
        std::cout << "  [LIFT INFO] Operational State: " << model_->GetCurrentStateName() << std::endl;
        std::cout << "  [LIFT INFO] Overload Alarm: " << (model_->IsOverloaded() ? "ACTIVE" : "OFF") << std::endl;
    }
};

// --- 5. Контроллер (Controller) ---

class ElevatorController {
private:
    ElevatorModel* model_;
public:
    ElevatorController(ElevatorModel* model) : model_(model) {}

    void HandleCallButton(int floor) {
        std::cout << "\n[Controller] User called lift to floor: " << floor << std::endl;
        model_->Call(floor);
    }
    
    void HandleLoadEvent() {
        std::cout << "\n[Controller] Load event registered (simulating extra weight)." << std::endl;
        model_->Load();
    }
    
    void HandleUnloadEvent() {
        std::cout << "\n[Controller] Unload event triggered (reducing weight)." << std::endl;
        model_->Unload();
    }
    
    void HandleEmergency() {
        std::cout << "\n[Controller] Emergency button pressed." << std::endl;
        model_->Emergency();
    }
    
    void HandlePowerLoss() {
        std::cout << "\n[Controller] Power loss event detected." << std::endl;
        model_->ChangeState(std::unique_ptr<IElevatorState>(GetNoPowerState()));
    }
    
    void HandlePowerRestore() {
        std::cout << "\n[Controller] Power restored event detected." << std::endl;
        model_->RestorePower();
    }
    
    void SimulateArrival() {
        // Для завершения движения
        std::cout << "\n[Controller] Simulation: Lift arrived at target floor." << std::endl;
        model_->Unload(); // Переводит из Moving в Standing
    }
};