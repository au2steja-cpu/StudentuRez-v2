#pragma once
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iterator>
#include <list>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "studentas.h"
#include "utils.h"

template <typename C> struct KonteinerioVardas;
template <> struct KonteinerioVardas<std::vector<Studentas>> { static constexpr const char* vardas = "vector"; };
template <> struct KonteinerioVardas<std::list<Studentas>> { static constexpr const char* vardas = "list"; };

namespace detail {
	inline void rasytiAntraste(std::ostream& out) {
		out << std::left << std::setw(plotis) << "Vardas" << "|";
		out << std::setw(utf8Plotis("Pavarde")) << "Pavarde" << "|";
		out << std::setw(plotis) << "Galutinis" << "|\n";
	}

	inline void rasytiStudenta(std::ostream& out, const Studentas& s) {
		out << std::left << std::setw(utf8Plotis(s.vardas)) << s.vardas << "|";
		out << std::setw(utf8Plotis(s.pavarde)) << s.pavarde << "|";
		out << std::right << std::setw(plotis) << s.galutinis << "|\n";
	}

	template <typename C, typename Cmp>
	void rusiuoti(C& c, Cmp cmp) {
		using Kategorija = typename std::iterator_traits<typename C::iterator>::iterator_category;
		if constexpr (std::is_base_of_v<std::random_access_iterator_tag, Kategorija>) {
			std::sort(c.begin(), c.end(), cmp);
		}
		else {
			c.sort(cmp);
		}
	}
}

template <typename C>
void nuskaitytiStudentus(C& grupe, const std::string& filename) {
	std::ifstream ins(filename);
	if (!ins) throw std::runtime_error("Nepavyko atidaryti failo " + filename);

	std::string eilute;
	std::getline(ins, eilute);
	std::istringstream antraste(eilute);
	std::string zodis;
	int stulpeliu = 0;
	while (antraste >> zodis) stulpeliu++;
	const int pazKiekis = std::max(stulpeliu - 3, 0);

	while (std::getline(ins, eilute)) {
		Studentas s;
		std::istringstream duomenys(eilute);
		duomenys >> s.vardas >> s.pavarde;
		s.paz.reserve(pazKiekis);
		for (int i = 0; i < pazKiekis; i++) {
			int num;
			duomenys >> num;
			s.paz.push_back(num);
		}
		duomenys >> s.exam;
		grupe.push_back(std::move(s));
	}
}

template <typename C>
void isvestiStudentus(const C& grupe, const std::string& filename) {
	std::ofstream out(filename);
	out << std::fixed << std::setprecision(2);
	detail::rasytiAntraste(out);
	for (const Studentas& s : grupe) {
		detail::rasytiStudenta(out, s);
	}
}

template <typename C>
void rusiuotiStudentus(C& grupe, Rusiuoti r) {
	switch (r) {
	case Rusiuoti::vardas:
		detail::rusiuoti(grupe, [](const Studentas& a, const Studentas& b) {
			if (a.vardas != b.vardas) return a.vardas < b.vardas;
			if (a.pavarde != b.pavarde) return a.pavarde < b.pavarde;
			return a.galutinis < b.galutinis;
			});
		break;
	case Rusiuoti::pavarde:
		detail::rusiuoti(grupe, [](const Studentas& a, const Studentas& b) {
			if (a.pavarde != b.pavarde) return a.pavarde < b.pavarde;
			if (a.vardas != b.vardas) return a.vardas < b.vardas;
			return a.galutinis < b.galutinis;
			});
		break;
	case Rusiuoti::galutinis:
		detail::rusiuoti(grupe, [](const Studentas& a, const Studentas& b) {
			if (a.galutinis != b.galutinis) return a.galutinis < b.galutinis;
			if (a.pavarde != b.pavarde) return a.pavarde < b.pavarde;
			return a.vardas < b.vardas;
			});
		break;
	}
}

namespace detail {
	template <typename C>
	constexpr bool arAtsitiktinisPrieiga() {
		using Kategorija = typename std::iterator_traits<typename C::iterator>::iterator_category;
		return std::is_base_of_v<std::random_access_iterator_tag, Kategorija>;
	}

	inline bool vargsiukas(const Studentas& st) { return st.galutinis < IslaikymoRiba; }
}

// 1 strategija: studentai kopijuojami i du NAUJUS konteinerius (vargsiukai ir kietiakai),
// bendras konteineris `grupe` lieka nepakites.
template <typename C>
void dalintiStrategija1(const C& grupe, C& vargsiukai, C& kietiakai) {
	std::copy_if(grupe.begin(), grupe.end(), std::back_inserter(vargsiukai),
		[](const Studentas& st) { return detail::vargsiukas(st); });
	std::copy_if(grupe.begin(), grupe.end(), std::back_inserter(kietiakai),
		[](const Studentas& st) { return !detail::vargsiukas(st); });
}

// 2 strategija: sukuriamas tik vienas naujas konteineris (vargsiukai); jie nukopijuojami
// (std::copy_if) ir istrinami is bendro konteinerio. `grupe` po funkcijos lieka tik kietiakai.
template <typename C>
void dalintiStrategija2(C& grupe, C& vargsiukai) {
	std::copy_if(grupe.begin(), grupe.end(), std::back_inserter(vargsiukai),
		[](const Studentas& st) { return detail::vargsiukas(st); });
	if constexpr (detail::arAtsitiktinisPrieiga<C>()) {
		grupe.erase(std::remove_if(grupe.begin(), grupe.end(),
			[](const Studentas& st) { return detail::vargsiukas(st); }), grupe.end());
	}
	else {
		grupe.remove_if([](const Studentas& st) { return detail::vargsiukas(st); });
	}
}

// 3 strategija: optimizuota 2 strategija. Nieko nekopijuojama:
//  - vector: std::stable_partition (islaiko tvarka) + vargsiuku perkelimas (move) ir vienas erase;
//  - list:   mazgai perkeliami std::list::splice (be alokavimo ir be kopijavimo).
template <typename C>
void dalintiStrategija3(C& grupe, C& vargsiukai) {
	if constexpr (detail::arAtsitiktinisPrieiga<C>()) {
		auto riba = std::stable_partition(grupe.begin(), grupe.end(),
			[](const Studentas& st) { return !detail::vargsiukas(st); });
		vargsiukai.reserve(static_cast<size_t>(std::distance(riba, grupe.end())));
		std::move(riba, grupe.end(), std::back_inserter(vargsiukai));
		grupe.erase(riba, grupe.end());
	}
	else {
		for (auto it = grupe.begin(); it != grupe.end();) {
			auto kitas = std::next(it);
			if (detail::vargsiukas(*it)) vargsiukai.splice(vargsiukai.end(), grupe, it);
			it = kitas;
		}
	}
}
