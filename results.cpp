#include "results.h"

#include <iomanip>

#include "utils.h"

namespace {
	void header(std::ostream& out) {
		out << "| Studentu skaicius | Nuskaitymas | Galutiniu skaiciavimas | Rusiavimas | Dalijimas i 2 grupes "
			<< "| Nuskriaustuku isvedimas | Kietuku isvedimas | Viso |\n";
		out << "|---|---|---|---|---|---|---|---|\n";
	}
	void eilute(std::ostream& out, const Rezultatai& r) {
		out << "| " << r.dydis << " | " << vidurkis(r.nuskaitymai) << " s | "
			<< vidurkis(r.galutiniai) << " s | "
			<< vidurkis(r.rusiavimai) << " s | "
			<< vidurkis(r.dalijimai) << " s | "
			<< vidurkis(r.isvedimaiNuskriaustuku) << " s | "
			<< vidurkis(r.isvedimaiKietuku) << " s | "
			<< vidurkis(r.bendri) << " s |\n";
	}
}

void spausdintiLentele(std::ostream& out, const std::string& pavadinimas, const std::vector<Rezultatai>& visi) {
	out << std::fixed << std::setprecision(3);
	out << "### " << pavadinimas << "\n\n";
	header(out);
	for (const auto& r : visi) eilute(out, r);
	out << "\n";
}

namespace {
	const Rezultatai* rasti(const std::vector<Rezultatai>& visi, const std::string& konteineris, int strategija, int dydis) {
		for (const auto& r : visi)
			if (r.konteineris == konteineris && r.strategija == strategija && r.dydis == dydis) return &r;
		return nullptr;
	}
}

void spausdintiPalyginima(std::ostream& out, const std::vector<Rezultatai>& visi) {
	out << std::fixed << std::setprecision(3);

	// 1) vector vs list kiekvienai strategijai
	for (int str = 1; str <= 3; str++) {
		out << "### Palyginimas: vector vs list, " << str << " strategija\n\n";
		out << "| Studentu skaicius | Rusiavimas (vector) | Rusiavimas (list) | Dalijimas (vector) | Dalijimas (list) "
			<< "| Viso (vector) | Viso (list) |\n";
		out << "|---|---|---|---|---|---|---|\n";
		for (const auto& v : visi) {
			if (v.konteineris != "vector" || v.strategija != str) continue;
			const Rezultatai* l = rasti(visi, "list", str, v.dydis);
			if (!l) continue;
			out << "| " << v.dydis
				<< " | " << vidurkis(v.rusiavimai) << " s | " << vidurkis(l->rusiavimai) << " s"
				<< " | " << vidurkis(v.dalijimai) << " s | " << vidurkis(l->dalijimai) << " s"
				<< " | " << vidurkis(v.bendri) << " s | " << vidurkis(l->bendri) << " s |\n";
		}
		out << "\n";
	}

	// 2) strategiju palyginimas kiekvienam konteineriui (tik dalijimo laikas)
	for (const char* k : { "vector", "list" }) {
		out << "### Strategiju palyginimas: " << k << " (dalijimo i 2 grupes laikas)\n\n";
		out << "| Studentu skaicius | 1 strategija | 2 strategija | 3 strategija |\n|---|---|---|---|\n";
		for (const auto& r : visi) {
			if (r.konteineris != k || r.strategija != 1) continue;
			out << "| " << r.dydis;
			for (int str = 1; str <= 3; str++) {
				const Rezultatai* x = rasti(visi, k, str, r.dydis);
				out << " | " << (x ? vidurkis(x->dalijimai) : 0.0) << " s";
			}
			out << " |\n";
		}
		out << "\n";
	}
}
