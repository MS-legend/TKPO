#include "AdapterSystem.h"

int main() {
    std::cout << "--- Smart Home System (Adapter Pattern) ---" << std::endl;

    // 1. Создаем Адаптер
    SensorAdapter adapter;
    
    // 2. Клиентский код работает с Адаптером через Целевой Интерфейс
    ClientCode(adapter);
    
    return 0;
}