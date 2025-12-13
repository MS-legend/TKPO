#include "ContactDecorator.h"

// Функция для демонстрации цепочки декораторов
void TestDecorator(const std::string& title, const Contact& contact) {
    std::cout << "\n=================================================" << std::endl;
    std::cout << title << std::endl;
    std::cout << "-------------------------------------------------" << std::endl;
    
    // 1. Вывод информации через метод Display
    contact.Display();
    
    // 2. Вывод полной информации через метод ToString
    std::cout << "\nFull String Output:\n" << contact.ToString() << std::endl;
}

int main() {
    std::cout << "--- LR8: Decorator Pattern (Contact Directory) ---" << std::endl;

    // 1. Исходный Конкретный Компонент (Юридическое лицо)
    std::unique_ptr<Contact> legalContact = 
        std::make_unique<LegalContact>("Tech Corp LLC", "555-1000", "New York, Wall St", "LLC");

    TestDecorator("SCENARIO A: Base Legal Contact (Initial)", *legalContact);
    
    // 2. ДЕКОРИРОВАНИЕ (Цепочка Декораторов)
    
    // Исходный Компонент (Физическое лицо)
    std::unique_ptr<Contact> physicalContact = 
        std::make_unique<PhysicalContact>("Ivanov P.I.", "8-901-123-4567", "Moscow, Tverskaya", "ivanov@mail.ru");

    // Шаг 1: Декорирование статусом верификации
    std::unique_ptr<Contact> verifiedContact = 
        std::make_unique<VerificationDecorator>(std::move(physicalContact), "Admin Verified");
        
    // Шаг 2: Дополнительное декорирование приоритетом
    // Объект verifiedContact (который сам является декоратором)
    // оборачивается новым декоратором PriorityDecorator
    std::unique_ptr<Contact> fullyDecoratedContact = 
        std::make_unique<PriorityDecorator>(std::move(verifiedContact), "Critical");
    
    TestDecorator("SCENARIO B: Fully Decorated Physical Contact (Chain)", *fullyDecoratedContact);

    std::cout << "\n--- Program finished. Calling destructors... ---" << std::endl;
    return 0;
}