#pragma once
#include <iostream>
#include <string>

#include "failai.h"
#include "konteineriai.h"
#include "results.h"
#include "timer.h"

template <typename C>
Rezultatai testuotiFaila(int dydis, int kartai, Rusiuoti rusiuoti, Strategija strategija) {
	const std::string vardas = KonteinerioVardas<C>::vardas;
	const int str = static_cast<int>(strategija);
	Rezultatai rez;
	rez.konteineris = vardas;
	rez.strategija = str;
	rez.dydis = dydis;

	const std::string pavadinimas = failoPavadinimas("kursiokai", dydis);
	std::cout << "[" << vardas << ", " << str << " strategija] Apdorojamas failas " << pavadinimas << "\n";

	for (int j = 0; j < kartai; j++) {
		std::cout << "  " << j + 1 << "-oji iteracija\n";
		Timer apdorojimas;
		Timer t;

		C grupe;
		nuskaitytiStudentus(grupe, pavadinimas);
		rez.nuskaitymai.push_back(t.elapsed());

		t.reset();
		for (Studentas& st : grupe) st.skaiciuotiGalutini();
		rez.galutiniai.push_back(t.elapsed());

		t.reset();
		rusiuotiStudentus(grupe, rusiuoti);
		rez.rusiavimai.push_back(t.elapsed());

		t.reset();
		C vargsiukai, kietiakai;
		const C* kietiakuSaltinis = &grupe;
		switch (strategija) {
		case Strategija::Pirma:
			dalintiStrategija1(grupe, vargsiukai, kietiakai);
			kietiakuSaltinis = &kietiakai;
			break;
		case Strategija::Antra:
			dalintiStrategija2(grupe, vargsiukai);
			break;
		case Strategija::Trecia:
			dalintiStrategija3(grupe, vargsiukai);
			break;
		}
		rez.dalijimai.push_back(t.elapsed());

		t.reset();
		isvestiStudentus(vargsiukai, failoPavadinimas("nuskriaustukai_" + vardas + "_", dydis));
		rez.isvedimaiNuskriaustuku.push_back(t.elapsed());

		t.reset();
		isvestiStudentus(*kietiakuSaltinis, failoPavadinimas("kietukai_" + vardas + "_", dydis));
		rez.isvedimaiKietuku.push_back(t.elapsed());

		rez.bendri.push_back(apdorojimas.elapsed());
		std::cout << "    nuskaitymas " << rez.nuskaitymai.back()
			<< " s | rusiavimas " << rez.rusiavimai.back()
			<< " s | dalijimas " << rez.dalijimai.back()
			<< " s | viso " << rez.bendri.back() << " s\n";
	}
	return rez;
}
