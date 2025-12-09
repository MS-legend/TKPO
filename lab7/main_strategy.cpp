#include "Directory.h"

void TestFunctionality() {
    // 1. Создание объектов Модели
    Contact* contact1 = new PhysicalContact("Ivanov P.I.", "8-901-123-4567", "Moscow, Tverskaya", "ivanov@mail.ru");
    Contact* contact2 = new LegalContact("Alpha LLC", "8-800-200-0000", "St. Petersburg, Nevsky", "LLC");
    Contact* contact3 = new PhysicalContact("Zuev A.A.", "8-903-987-6543", "Kazan, Baumana", "zuev@corp.com");
    
    // 2. Создание Контекста (Справочника)
    Directory mainDirectory("Company Contacts", "Sidorov A.V.");

    // Добавление объектов в массив (массив базового класса Contact*)
    mainDirectory.AddContact(contact1);
    mainDirectory.AddContact(contact2);
    mainDirectory.AddContact(contact3);
    
    std::cout << "\n--- Initial state of the Directory ---" << std::endl;
    mainDirectory.DisplayRecords();

    // 3. Создание Конкретных Стратегий
    SortByNameStrategy sortByName;
    SortByPhoneStrategy sortByPhone;

    // 4. Применение Паттерна Стратегия
    
    // Смена поведения 1: Сортировка по имени
    mainDirectory.SetSortStrategy(&sortByName);
    mainDirectory.SortRecords();
    std::cout << "\n--- Directory after sorting by Name ---" << std::endl;
    mainDirectory.DisplayRecords();
    
    // Смена поведения 2: Сортировка по телефону
    mainDirectory.SetSortStrategy(&sortByPhone);
    mainDirectory.SortRecords();
    std::cout << "\n--- Directory after sorting by Phone ---" << std::endl;
    mainDirectory.DisplayRecords();
    
    // Деструкторы будут вызваны автоматически при выходе mainDirectory из области видимости
}

int main() {
    std::cout << "--- LR7: Strategy Pattern (Contact Directory) ---" << std::endl;
    TestFunctionality();
    std::cout << "\n--- Program finished. Calling destructors... ---" << std::endl;
    return 0;
}