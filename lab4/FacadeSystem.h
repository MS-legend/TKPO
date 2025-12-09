#pragma once
#include <iostream>
#include <string>
#include <vector>

// === 1. Подсистема A: Бронирование Отелей (Subsystem) ===
class HotelBookingSystem {
public:
    std::string FindHotel(const std::string& city, int stars) const {
        std::string result = "Hotel: Found hotel in " + city;
        if (stars >= 4) {
            result += " (Premium Class, " + std::to_string(stars) + " stars).";
        } else {
            result += " (Standard Class).";
        }
        return result;
    }
    std::string ReserveRoom() const {
        return " -> Hotel: Room reservation confirmed.";
    }
};

// === 2. Подсистема B: Бронирование Авиабилетов (Subsystem) ===
class FlightBookingSystem {
public:
    std::string FindTickets(const std::string& city) const {
        return "Tickets: Found direct flights to " + city + ".";
    }
    std::string BookSeat() const {
        return " -> Tickets: Seat booked.";
    }
};

// === 3. Подсистема C: Выбор Питания (Subsystem) ===
class CateringSystem {
public:
    std::string SelectMealPlan(const std::string& mealPlan) const {
        if (mealPlan == "All Inclusive") {
            return "Catering: Selected 'All Inclusive' plan (Full board).";
        } else {
            return "Catering: Selected '" + mealPlan + "' plan.";
        }
    }
    std::string PayForMealPlan() const {
        return " -> Catering: Payment confirmed.";
    }
};

// === 4. Фасад (Facade) ===
// Предоставляет простой интерфейс для доступа к подсистемам
class TourismFacade {
private:
    // Фасад использует композицию для доступа к подсистемам
    HotelBookingSystem hotelSystem_;
    FlightBookingSystem flightSystem_;
    CateringSystem cateringSystem_;

public:
    // Упрощенный метод для Клиента
    void OrganizeTour(const std::string& city, int stars, const std::string& mealPlan) const {
        std::cout << "\n========================================================" << std::endl;
        std::cout << "[FACADE] Starting tour organization to " << city << std::endl;
        std::cout << "========================================================" << std::endl;

        // 1. Поиск и бронирование билетов
        std::cout << flightSystem_.FindTickets(city) << std::endl;
        std::cout << flightSystem_.BookSeat() << std::endl;

        // 2. Поиск и бронирование отеля
        std::cout << hotelSystem_.FindHotel(city, stars) << std::endl;
        std::cout << hotelSystem_.ReserveRoom() << std::endl;

        // 3. Выбор и оплата питания
        std::cout << cateringSystem_.SelectMealPlan(mealPlan) << std::endl;
        std::cout << cateringSystem_.PayForMealPlan() << std::endl;
        
        std::cout << "\n[FACADE] Tour successfully organized and confirmed." << std::endl;
    }
};