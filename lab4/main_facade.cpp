#include "FacadeSystem.h"

int main() {
    std::cout << "--- Tourist Agency System (Facade Pattern) ---" << std::endl;

    // Клиент
    TourismFacade travelAgency;

    // Сценарий 1: Роскошный отдых (Клиент вызывает только один метод Фасада)
    travelAgency.OrganizeTour("Dubai", 5, "All Inclusive");

    // Сценарий 2: Бюджетный отдых
    travelAgency.OrganizeTour("Sochi", 3, "Half Board");
    
    // ВАЖНО: Клиент может обращаться и к подсистемам напрямую, если нужно.
    std::cout << "\n--- Non-standard access (bypassing Facade) ---" << std::endl;
    HotelBookingSystem hotelSystem;
    std::cout << hotelSystem.FindHotel("Moscow", 5) << std::endl;

    return 0;
}