#pragma once
#include <atomic>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>

class Timer {
public:

 // Lance un chronométrage étiqueté ; en général on démarre aussi le thread de suivi ici.
    explicit Timer (std :: string label = "En cours");
    ~Timer();

    void stop();// Demande l'arrêt 

private:
    void run(); // Boucle d'affichage périodique du temps écoulé depuis start_.

    std::string label_;
    std::atomic<bool> running_;
    std::chrono::steady_clock::time_point start_;
    std::thread t_;
};
