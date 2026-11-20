#pragma once
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <random>

#include "nustatymai.h"
#include "studentas.h"
#include "utils.h"
#include "failai.h"

namespace pagalba {
    void rasytiAntraste(std::ostream& out, Statistika statistika);
    void rasytiStudenta(std::ostream& out, const Studentas& st, Statistika statistika);
    template <typename Konteineris>
    void rasytiGrupe(std::ostream& out, Konteineris& grupe, Statistika statistika) {
        rasytiAntraste(out, statistika);
        for (Studentas& st : grupe) {
            rasytiStudenta(out, st, statistika);
        }
    }
}
int ivestiSk(const std::string& klausimas, int nuo, int iki);

bool taipArNe(const std::string& klausimas);
bool iSkaiciu(const std::string& tekstas, int& rezultatas);
template <typename Konteineris>
void ivestiRanka(bool zinomas_sk, Statistika statistika, Rezimas rezimas, std::ostream& output, Rusiuoti rusiuoti, Tvarka tvarka) {
    const auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937 gen(static_cast<std::mt19937::result_type>(seed));
    std::uniform_int_distribution<int> pazymiu_dist(Min_paz, Max_paz);
    Konteineris grupe;
    const int studkiekis = ivestiSk("Iveskite studentu skaiciu: ", 1, Be_ribos);
    for (int i = 0; i < studkiekis; i++) {
        Studentas st;

        std::cout << "Iveskite varda: ";
        std::cin >> st.vardas;

        std::cout << "Iveskite pavarde: ";
        std::cin >> st.pavarde;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (zinomas_sk) {
            const int kiekis = ivestiSk("Iveskite namu darbu skaiciu: ", 0, Be_ribos);
            if (rezimas == Rezimas::Ranka) {
                for (int j = 1; j <= kiekis; j++) {
                    st.paz.push_back(ivestiSk(
                        "Iveskite " + std::to_string(j) + "-aji pazymi: ", Min_paz, Max_paz));
                    std::cout << '\n';
                }
            }
            else {
                std::cout << "Suvesti atsitiktiniai pazymiai: ";
                for (int j = 0; j < kiekis; j++) {
                    const int sk = pazymiu_dist(gen);
                    st.paz.push_back(sk);
                    std::cout << sk << " ";
                }
                std::cout << '\n';
            }
        }
        else {
            std::string eilute;
            while (true) {
                std::cout << "Iveskite " << st.paz.size() + 1 << "-aji pazymi (Enter, kad uzbaigti): ";
                if (!std::getline(std::cin, eilute) || eilute.empty()) break;
                int pazymys;
                if (!iSkaiciu(eilute, pazymys)) {
                    std::cout << "Ivesta netinkama reiksme (ne skaicius arba per didelis skaicius)!\n";
                }
                else if (pazymys < Min_paz || pazymys > Max_paz) {
                    std::cout << "Pazymys turi buti nuo " << Min_paz << " iki " << Max_paz << "!\n";
                }
                else {
                    st.paz.push_back(pazymys);
                }
            }
        }
        if (rezimas == Rezimas::Generavimas) {
            st.exam = pazymiu_dist(gen);
            std::cout << "Atsitiktinis egzamino rezultatas: " << st.exam << '\n';
        }
        else {
            st.exam = ivestiSk("Iveskite egzamino rezultata: ", Min_paz, Max_paz);
        }

        skaiciuotiGalutini(st);
        grupe.push_back(st);
        std::cout << "Ivesto studento adresas atmintyje: " << &grupe.back() << "\n";
    }
    rusiuotiStudentus(grupe, rusiuoti, tvarka);
    pagalba::rasytiGrupe(output, grupe, statistika);
}
template <typename Konteineris>
void nuskaitytiIsFailo(const std::string& filename, Statistika statistika, std::ostream& output, Rusiuoti rusiuoti, Tvarka tvarka) {
    Konteineris grupe;
    nuskaitytiStudentus(grupe, filename);
    for (Studentas& st : grupe) {
        skaiciuotiGalutini(st);
    }
    rusiuotiStudentus(grupe, rusiuoti, tvarka);
    pagalba::rasytiGrupe(output, grupe, statistika);
}

int nustatymai(const std::string& klausimas, const std::string& pasirinkimai);