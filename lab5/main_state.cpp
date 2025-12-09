#include "ElevatorSystem.h"

void RunElevatorScenario(Elevator& elevator) {
    std::cout << "\n--- SCENARIO START ---" << std::endl;

    // 1. Лифт стоит, вызываем на другой этаж
    std::cout << "\n[Step 1] Calling elevator to floor 5." << std::endl;
    elevator.Call(5); // Standing -> Moving
    
    // 2. Имитация прибытия и остановки
    std::cout << "\n[Step 2] Elevator arrives and stops. (Manual state change for scenario)" << std::endl;
    elevator.ChangeState(std::unique_ptr<IElevatorState>(GetStandingState()));
    
    // 3. Загрузка пассажиров (имитируем перегрузку)
    std::cout << "\n[Step 3] Loading passengers (causing overload)." << std::endl;
    elevator.Load(); // Standing -> Overloaded
    
    // 4. Попытка вызвать лифт в перегруженном состоянии
    std::cout << "\n[Step 4] Calling elevator while Overloaded." << std::endl;
    elevator.Call(10); // Remains Overloaded
    
    // 5. Разгрузка (устранение перегрузки)
    std::cout << "\n[Step 5] Unloading (resolving overload)." << std::endl;
    elevator.Unload(); // Overloaded -> Standing
    
    // 6. Имитация потери питания
    std::cout << "\n[Step 6] Power outage simulation (Manual state change for scenario)." << std::endl;
    elevator.ChangeState(std::unique_ptr<IElevatorState>(GetNoPowerState()));
    
    // 7. Восстановление питания
    std::cout << "\n[Step 7] Restoring power." << std::endl;
    elevator.RestorePower(); // NoPower -> Standing
    
    // 8. Аварийная ситуация
    std::cout << "\n[Step 8] Emergency triggered." << std::endl;
    elevator.Emergency(); // Standing -> Malfunction
    
    // 9. Попытка вызвать лифт в состоянии аварии
    std::cout << "\n[Step 9] Calling elevator while Malfunction." << std::endl;
    elevator.Call(1); // Remains Malfunction

    std::cout << "\n--- SCENARIO END ---" << std::endl;
}

int main() {
    std::cout << "--- Elevator System (State Pattern) ---" << std::endl;

    // Инициализация лифта в состоянии "Standing"
    // ИСПРАВЛЕНО: Заменены () на {} для предотвращения Most Vexing Parse
    Elevator elevator{std::unique_ptr<IElevatorState>(GetStandingState())}; 
    
    RunElevatorScenario(elevator);

    return 0;
}