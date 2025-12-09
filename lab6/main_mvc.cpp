#include "ElevatorMVC.h"

// Сценарий для демонстрации MVC
void RunMVCDemo(ElevatorController& controller) {
    std::cout << "\n==============================================" << std::endl;
    std::cout << "--- SCENARIO 1: Basic Movement and Overload ---" << std::endl;
    std::cout << "==============================================" << std::endl;

    // 1. Пользователь вызывает лифт (Standing -> Moving)
    controller.HandleCallButton(5);
    // -> View1 & View2 обновляются: State=Moving, Floor=5
    
    // 2. Имитация прибытия и остановки (Moving -> Standing)
    controller.SimulateArrival();
    // -> View1 & View2 обновляются: State=Standing, Floor=5

    // 3. Загрузка пассажиров (Standing -> Overloaded)
    controller.HandleLoadEvent();
    // -> View1 & View2 обновляются: State=Overloaded, Overload Alarm=ACTIVE
    
    // 4. Попытка вызвать лифт в перегруженном состоянии (No Change)
    controller.HandleCallButton(10);
    
    // 5. Разгрузка (Overloaded -> Standing)
    controller.HandleUnloadEvent();
    // -> View1 & View2 обновляются: State=Standing, Overload Alarm=OFF
    
    std::cout << "\n=================================================" << std::endl;
    std::cout << "--- SCENARIO 2: Power Loss and Emergency Stop ---" << std::endl;
    std::cout << "=================================================" << std::endl;
    
    // 6. Имитация потери питания (Standing -> NoPower)
    controller.HandlePowerLoss();
    // -> View1 & View2 обновляются: State=NoPower
    
    // 7. Восстановление питания (NoPower -> Standing)
    controller.HandlePowerRestore();
    // -> View1 & View2 обновляются: State=Standing
    
    // 8. Аварийная ситуация (Standing -> Malfunction)
    controller.HandleEmergency();
    // -> View1 & View2 обновляются: State=Malfunction
}

int main() {
    std::cout << "--- LR6: MVC Pattern (Elevator System) ---" << std::endl;

    // 1. Создание МОДЕЛИ (Model), инициализация в Standing
    // Используем {} для чистой инициализации
    ElevatorModel elevator{std::unique_ptr<IElevatorState>(GetStandingState())};
    
    // 2. Создание ПРЕДСТАВЛЕНИЙ (View), подписка на Модель происходит в конструкторе
    ConsoleView1 view1(&elevator);
    ConsoleView2 view2(&elevator);
    
    // 3. Создание КОНТРОЛЛЕРА (Controller)
    ElevatorController controller(&elevator);
    
    RunMVCDemo(controller);

    return 0;
}