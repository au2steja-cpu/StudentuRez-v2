#pragma once
#include <iosfwd>
#include <string>
#include <utility>
#include <vector>

enum class Rusiuoti { vardas, pavarde, galutinis };
enum class Strategija { Pirma = 1, Antra = 2, Trecia = 3 };
constexpr double IslaikymoRiba = 5.0;

// v1.1: struct pakeistas klase. Duomenys privatus, prieiga per get'erius ir set'erius.
class Studentas {
public:
	// --- konstruktoriai, priskyrimo operatoriai, destruktorius (Rule of Five) ---
	Studentas();
	Studentas(std::string vardas, std::string pavarde, std::vector<int> paz, int egzaminas);
	Studentas(const Studentas& kitas);
	Studentas(Studentas&& kitas) noexcept;
	Studentas& operator=(const Studentas& kitas);
	Studentas& operator=(Studentas&& kitas) noexcept;
	~Studentas();

	// --- get'eriai ---
	const std::string& vardas() const { return vardas_; }
	const std::string& pavarde() const { return pavarde_; }
	const std::vector<int>& pazymiai() const { return paz_; }
	int egzaminas() const { return egzaminas_; }
	double galutinis() const { return galutinis_; }

	// --- set'eriai ---
	void setVardas(std::string vardas) { vardas_ = std::move(vardas); }
	void setPavarde(std::string pavarde) { pavarde_ = std::move(pavarde); }
	void setPazymiai(std::vector<int> paz) { paz_ = std::move(paz); }
	void pridetiPazymi(int pazymys) { paz_.push_back(pazymys); }
	void setEgzaminas(int egzaminas) { egzaminas_ = egzaminas; }

	// galutinis = 0.4 * namu darbu vidurkis + 0.6 * egzaminas
	void skaiciuotiGalutini();

	// Isvedimas i srauta (viena lentelės eilutė: vardas | pavarde | galutinis)
	friend std::ostream& operator<<(std::ostream& out, const Studentas& s);

private:
	std::string vardas_;
	std::string pavarde_;
	std::vector<int> paz_;
	int egzaminas_;
	double galutinis_;
};
