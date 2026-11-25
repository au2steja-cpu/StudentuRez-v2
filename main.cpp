#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <string>
#include <vector>
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

#include "benchmark.h"
#include "failai.h"
#include "ivestis.h"
#include "results.h"
#include "studentas.h"
#include "timer.h"
#include "utils.h"

namespace {
	void testavimas(int maxFailas, int kartai) {
		constexpr int MINFAILAS = 1000;

		std::cout << "Pagal ka rusiuoti isvesties failus?\n"
			<< "[0] - Varda\n[1] - Pavarde\n[2] - Galutini bala\n";
		const auto rusiuoti = static_cast<Rusiuoti>(ivestiSkaiciu("Pasirinkimas: ", 0, 2));

		if (!arFailaiEgzistuoja(MINFAILAS, maxFailas) || taipArNe("Ar generuoti naujus failus? [y/n] ")) {
			Timer visas;
			for (int i = MINFAILAS; i <= maxFailas; i *= 10) {
				Timer t;
				generuotiFaila(i);
				std::cout << "  Generavimas uztruko: " << t.elapsed() << " s\n";
			}
			std::cout << "Visu failu generavimas uztruko: " << visas.elapsed() << " s\n\n";
		}

		std::vector<Rezultatai> visi;
		std::vector<Rezultatai> rezVector[3], rezList[3];
		for (int i = MINFAILAS; i <= maxFailas; i *= 10) {
			for (Strategija str : { Strategija::Pirma, Strategija::Antra, Strategija::Trecia }) {
				const int n = static_cast<int>(str) - 1;
				rezVector[n].push_back(testuotiFaila<std::vector<Studentas>>(i, kartai, rusiuoti, str));
				rezList[n].push_back(testuotiFaila<std::list<Studentas>>(i, kartai, rusiuoti, str));
				visi.push_back(rezVector[n].back());
				visi.push_back(rezList[n].back());
			}
		}

		std::ofstream out("rezultatai.md");
		for (int n = 0; n < 3; n++) {
			spausdintiLentele(out, "std::vector, " + std::to_string(n + 1) + " strategija", rezVector[n]);
			spausdintiLentele(out, "std::list, " + std::to_string(n + 1) + " strategija", rezList[n]);
		}
		spausdintiPalyginima(out, visi);
		std::cout << "\nRezultatu lenteles irasytos i rezultatai.md\n";
	}
}

int main(int argc, char* argv[]) {
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
#endif
	// argumentai: [maksimalus failo dydis] [kiek kartu kartoti kiekviena testa]
	int maxFailas = 100000;
	int kartai = 3;
	try {
		if (argc > 1) maxFailas = std::stoi(argv[1]);
		if (argc > 2) kartai = std::stoi(argv[2]);
	}
	catch (const std::exception&) {
		std::cerr << "Naudojimas: StudentuRezultatai [maksimalus failo dydis >= 1000] [kartu skaicius >= 1]\n";
		return 1;
	}
	if (maxFailas < 1000 || kartai < 1) {
		std::cerr << "Naudojimas: StudentuRezultatai [maksimalus failo dydis >= 1000] [kartu skaicius >= 1]\n";
		return 1;
	}
	std::cout << std::fixed << std::setprecision(3);

	std::cout << "--- Studentu rezultatai v1.0 ---\n"
		<< "[1] - Rankinis ivedimas (rodomi objektu atminties adresai)\n"
		<< "[2] - Konteineriu testavimas (vector vs list, 1-3 strategijos)\n"
		<< "[0] - Iseiti\n";
	switch (ivestiSkaiciu("Pasirinkite programos rezima: ", 0, 2)) {
	case 1: rankinisIvedimas(); break;
	case 2: testavimas(maxFailas, kartai); break;
	default: break;
	}
}