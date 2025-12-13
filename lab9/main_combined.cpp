#include "CompositeChainMemento.h"

// Демонстрация Компоновщика и Цепочки
void TestCompositeAndChain(Directory& dir) {
    // 1. Создание древовидной структуры (Composite)
    
    // Создаем корневой каталог
    ContactGroup* root = dir.GetRoot(); 
    
    // Создаем подгруппы
    ContactGroup* salesGroup = new ContactGroup("Sales Dept");
    ContactGroup* managementGroup = new ContactGroup("Management");
    
    // Создаем контакты (Leafs)
    Contact* ivanov = new PhysicalContact("Ivanov P.I.", "89011234567", "Moscow");
    Contact* alphaLLC = new LegalContact("Alpha LLC", "88002000000", "St. Petersburg");
    Contact* zuev = new PhysicalContact("Zuev A.A.", "89039876543", "Kazan");
    Contact* tooShort = new PhysicalContact("A", "", "Invalid Address");
    
    // Строим иерархию
    salesGroup->Add(ivanov);
    salesGroup->Add(alphaLLC);
    root->Add(salesGroup);
    managementGroup->Add(zuev);
    root->Add(managementGroup);
    root->Add(tooShort); // Контакт с ошибкой для теста CoR
    
    std::cout << "\n==============================================" << std::endl;
    std::cout << "--- I. Composite Pattern (Directory Tree) ---" << std::endl;
    std::cout << "==============================================" << std::endl;
    dir.Display();

    // 2. Создание Цепочки обязанностей (Chain of Responsibility)
    std::cout << "\n==============================================" << std::endl;
    std::cout << "--- II. Chain of Responsibility (Validation) ---" << std::endl;
    std::cout << "==============================================" << std::endl;

    IValidator* nameChecker = new NameValidator();
    IValidator* phoneChecker = new PhoneValidator();

    // Строим цепочку: Имя -> Телефон
    nameChecker->SetNext(phoneChecker);

    std::vector<ContactComponent*> contactsToValidate = {ivanov, alphaLLC, tooShort, salesGroup};
    
    for (auto contact : contactsToValidate) {
        std::cout << "\n[VALIDATE] Contact: " << contact->GetName() << std::endl;
        std::string result = nameChecker->Handle(contact);
        std::cout << "Result: " << result << std::endl;
    }
    
    delete nameChecker;
    delete phoneChecker;
}

// Демонстрация Снимка
void TestMemento(Directory& dir) {
    std::cout << "\n==============================================" << std::endl;
    std::cout << "--- III. Memento Pattern (Save & Restore) ---" << std::endl;
    std::cout << "==============================================" << std::endl;

    History history(&dir);
    
    // 1. Сохраняем начальное состояние
    history.Backup();
    
    // 2. Имитация изменения (например, меняем телефон у контакта, который находится глубоко в дереве)
    std::cout << "\n[Action] Changing state: Modifying Alpha LLC's phone number." << std::endl;
    // Поиск контакта (в реальной жизни нужен итератор)
    ContactGroup* salesGroup = dynamic_cast<ContactGroup*>(dir.GetRoot()->GetChildren().front());
    if (salesGroup) {
        Contact* alphaLLC = dynamic_cast<Contact*>(salesGroup->GetChildren().back());
        if (alphaLLC) {
            alphaLLC->SetPhone("999-999-9999");
        }
    }
    dir.Display();
    
    // 3. Сохраняем измененное состояние
    history.Backup();

    // 4. Имитация дальнейших изменений
    std::cout << "\n[Action] Changing state again: Adding a new Group." << std::endl;
    dir.GetRoot()->Add(new ContactGroup("New Temp Group"));
    dir.Display();

    // 5. Откат (Undo)
    std::cout << "\n[UNDO 1] Reverting to previous state (after phone change)..." << std::endl;
    history.Undo();
    dir.Display(); 
    
    // 6. Откат ко второму снимку (начальному)
    std::cout << "\n[UNDO 2] Reverting to initial state (full restore)..." << std::endl;
    history.Undo();
    dir.Display(); 
}

int main() {
    std::cout << "--- LR9: Composite, Chain of Responsibility, Memento ---" << std::endl;

    Directory mainDirectory("Global Contact Registry");
    
    // Тестирование Компоновщика и Цепочки
    TestCompositeAndChain(mainDirectory);

    // Тестирование Снимка
    TestMemento(mainDirectory);

    std::cout << "\n--- Program finished. Calling destructors... ---" << std::endl;
    return 0;
}