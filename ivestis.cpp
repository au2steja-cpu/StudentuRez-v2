#include "ivestis.h"

#include <iomanip>
#include <iostream>
#include <string>

#include "utils.h"

namespace {
	Studentas ivestiStudenta() {
		std::string vardas, pavarde;
		std::vector<int> paz;
		std::cout << "\n--- Naujas studentas ---\n";
		std::cout << "Vardas: ";
		std::getline(std::cin >> std::ws, vardas);
		std::cout << "Pavarde: ";
		std::getline(std::cin >> std::ws, pavarde);
		const int n = ivestiSkaiciu("Namu darbu kiekis (0-20): ", 0, 20);
		for (int i = 0; i < n; i++) {
			paz.push_back(ivestiSkaiciu("Namu darbo pazymys " + std::to_string(i + 1) + " (1-10): ", 1, 10));
		}
		const int egzaminas = ivestiSkaiciu("Egzamino pazymys (1-10): ", 1, 10);
		Studentas s(std::move(vardas), std::move(pavarde), std::move(paz), egzaminas);
		s.skaiciuotiGalutini();
		return s;
	}

	template <typename C>
	void spausdintiSuAdresais(const std::string& pavadinimas, const C& grupe) {
		std::cout << "\n=== " << pavadinimas << " (elementu: " << grupe.size() << ") ===\n";
		std::cout << std::left << std::setw(utf8Plotis("Vardas")) << "Vardas"
			<< std::setw(utf8Plotis("Pavarde")) << "Pavarde"
			<< std::setw(12) << "Galutinis" << "Adresas\n";
		std::cout << std::fixed << std::setprecision(2);
		for (const Studentas& s : grupe) {
			std::cout << std::left << std::setw(utf8Plotis(s.vardas())) << s.vardas()
				<< std::setw(utf8Plotis(s.pavarde())) << s.pavarde()
				<< std::setw(12) << s.galutinis()
				<< static_cast<const void*>(&s) << "\n";
		}
	}
}

void rankinisIvedimas() {
	std::vector<Studentas> vec;
	std::list<Studentas> lst;

	do {
		Studentas s = ivestiStudenta();
		vec.push_back(s);
		lst.push_back(std::move(s));
	} while (taipArNe("Ar ivesti dar viena studenta? [y/n] "));

	spausdintiSuAdresais("std::vector", vec);
	spausdintiSuAdresais("std::list", lst);
	std::cout << "\nPastaba: std::vector elementai guli vienas salia kito (adresu skirtumas = sizeof(Studentas) = "
		<< sizeof(Studentas) << " B), o std::list elementai issklaidyti atmintyje (kiekvienas mazgas "
		<< "alokuojamas atskirai).\n";
}