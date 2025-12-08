#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

// === 1. Принцип OCP: Абстракция Назначения ===
// Открыто для расширения (добавления новых типов назначений), закрыто для изменения.
class Appointment {
public:
    virtual ~Appointment() = default;
    virtual std::string getType() const = 0;
    virtual void execute() const = 0; // Логика выполнения
};

// Конкретные классы для расширения (OCP)
class MedicationAppointment : public Appointment {
private:
    std::string medicationName;
public:
    MedicationAppointment(const std::string& name) : medicationName(name) {}
    std::string getType() const override { return "Лекарство"; }
    void execute() const override {
        std::cout << "-> Назначено: Принять лекарство: " << medicationName << std::endl;
    }
};

class ProcedureAppointment : public Appointment {
private:
    std::string procedureName;
public:
    ProcedureAppointment(const std::string& name) : procedureName(name) {}
    std::string getType() const override { return "Процедура"; }
    void execute() const override {
        std::cout << "-> Назначено: Выполнить процедуру: " << procedureName << std::endl;
    }
};

// === 2. Принцип SRP: Класс Пациента ===
// Ответственность: хранение данных пациента и его назначений.
class Patient {
private:
    std::string name;
    std::string condition;
    bool isDischarged = false;
    std::vector<std::unique_ptr<Appointment>> appointments;

public:
    Patient(const std::string& n, const std::string& c) : name(n), condition(c) {}

    const std::string& getName() const { return name; }
    bool getIsDischarged() const { return isDischarged; }
    void discharge() { isDischarged = true; } // Используется DischargeService

    void addAppointment(std::unique_ptr<Appointment> app) {
        if (!isDischarged) {
            appointments.push_back(std::move(app));
            std::cout << "Пациенту " << name << " добавлено назначение: " 
                      << appointments.back()->getType() << "." << std::endl;
        }
    }

    void viewAppointments() const {
        std::cout << "\n--- Назначения для " << name << " ---" << std::endl;
        for (const auto& app : appointments) {
            app->execute();
        }
    }
};

// === 3. Принцип SRP: Класс Врача ===
// Ответственность: назначение лечения.
class Doctor {
private:
    std::string name;
public:
    Doctor(const std::string& n) : name(n) {}

    void makeMedicationAppointment(Patient& p, const std::string& medication) const {
        std::cout << "\nДоктор " << name << " назначает лечение..." << std::endl;
        p.addAppointment(std::make_unique<MedicationAppointment>(medication));
    }

    void makeProcedureAppointment(Patient& p, const std::string& procedure) const {
        std::cout << "Доктор " << name << " назначает лечение..." << std::endl;
        p.addAppointment(std::make_unique<ProcedureAppointment>(procedure));
    }
};

// === 4. Принцип SRP: Сервис Выписки ===
// Ответственность: логика выписки и ее причины.
class DischargeService {
public:
    void dischargePatient(Patient& p, const std::string& reason) const {
        if (!p.getIsDischarged()) {
            p.discharge();
            std::cout << "\n[SERVICE] Пациент " << p.getName() << " выписан из Больницы." << std::endl;
            std::cout << "Причина: " << reason << std::endl;
        } else {
            std::cout << "\n[SERVICE] Пациент " << p.getName() << " уже выписан." << std::endl;
        }
    }
};