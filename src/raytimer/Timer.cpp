#include "Timer.hpp"
#include <utility> 

// Constructeur : initialise le libellé, démarre le chronométrage et lance le thread qui appelle run()
Timer::Timer (std :: string label): 
         label_(std::move(label)),
         running_(true),
         start_(std ::chrono::steady_clock::now()),
         t_([this]{ run(); }) {}




// run() : boucle de fond. Chaque seconde, met à jour la ligne d'état (spinner + secondes écoulées)
void Timer::run() {
    using namespace std::chrono;
    const char spin[4] = {'|','/','-','\\'};
    size_t i = 0;
    while (running_) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        const auto s = duration_cast<seconds>(steady_clock::now() - start_).count();
        std::cout << "\r" << label_ << " " << spin[i++ % 4]
                  << "  Temps ecoule : " << s << " s" << std::flush;
    }
}


// Destructeur : garantit l'arrêt propre du thread et l'affichage du temps total via stop()
Timer::~Timer() { stop(); }


// stop() :Demande l'arrêt (running_=false), rejoint le thread et affiche la durée totale
void Timer::stop() {
    if (running_.exchange(false)) {
        if (t_.joinable()) t_.join();
        using namespace std::chrono;
        const auto ms = duration_cast<milliseconds>(steady_clock::now() - start_).count();
        std::cout << "\n" << label_ << " termine en "
                  << std::fixed << std::setprecision(3) << (ms / 1000.0) << " s\n";
    }
}

