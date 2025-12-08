#include "HospitalSystem.h"
#include <cassert>

// Функция для тестирования (SRP: отдельная ответственность)
void runTests() {
    std::cout << "\n*** НАЧАЛО ТЕСТИРОВАНИЯ ***" << std::endl;
    
    // Тест 1: Назначение и выполнение
    Patient p1("Иванов", "Грипп");
    Doctor d1("Доктор Смирнов");
    
    d1.makeMedicationAppointment(p1, "Антибиотик-500");
    d1.makeProcedureAppointment(p1, "Ингаляция");
    
    p1.viewAppointments();
    assert(!p1.getIsDischarged());

    // Тест 2: Выписка
    DischargeService ds;
    ds.dischargePatient(p1, "По окончании лечения");
    assert(p1.getIsDischarged());

    // Тест 3: Проверка OCP (добавление нового типа назначения)
    // Если бы мы добавили класс OperationAppointment, 
    // существующий код Patient не пришлось бы менять.
    
    std::cout << "*** ТЕСТИРОВАНИЕ ЗАВЕРШЕНО УСПЕШНО ***" << std::endl;
}

int main() {
    std::cout << "--- Система Больница (Вариант 3) ---" << std::endl;
    
    // Инициализация
    Patient vasily("Василий Петров", "Травма ноги");
    Doctor anna("Анна Соколова");
    DischargeService ds;

    // Сценарий 1: Назначение
    anna.makeProcedureAppointment(vasily, "Рентген");
    anna.makeMedicationAppointment(vasily, "Обезболивающее");

    // Сценарий 2: Выполнение назначений (например, медсестрой)
    vasily.viewAppointments();
    
    // Сценарий 3: Выписка 
    ds.dischargePatient(vasily, "Лечение завершено, состояние удовлетворительное");

    runTests();
    
    return 0;
}