#include "studentas.h"

#include <iomanip>
#include <ostream>

#include "utils.h"

// Numatytasis konstruktorius
Studentas::Studentas() : vardas_(), pavarde_(), paz_(), egzaminas_(0), galutinis_(0.0) {}

// Konstruktorius su parametrais
Studentas::Studentas(std::string vardas, std::string pavarde, std::vector<int> paz, int egzaminas)
	: vardas_(std::move(vardas)), pavarde_(std::move(pavarde)), paz_(std::move(paz)),
	egzaminas_(egzaminas), galutinis_(0.0) {
}

// Kopijavimo konstruktorius
Studentas::Studentas(const Studentas& kitas)
	: vardas_(kitas.vardas_), pavarde_(kitas.pavarde_), paz_(kitas.paz_),
	egzaminas_(kitas.egzaminas_), galutinis_(kitas.galutinis_) {
}

// Perkelimo konstruktorius
Studentas::Studentas(Studentas&& kitas) noexcept
	: vardas_(std::move(kitas.vardas_)), pavarde_(std::move(kitas.pavarde_)), paz_(std::move(kitas.paz_)),
	egzaminas_(kitas.egzaminas_), galutinis_(kitas.galutinis_) {
	kitas.egzaminas_ = 0;
	kitas.galutinis_ = 0.0;
}

// Kopijavimo priskyrimo operatorius
Studentas& Studentas::operator=(const Studentas& kitas) {
	if (this != &kitas) {
		vardas_ = kitas.vardas_;
		pavarde_ = kitas.pavarde_;
		paz_ = kitas.paz_;
		egzaminas_ = kitas.egzaminas_;
		galutinis_ = kitas.galutinis_;
	}
	return *this;
}

// Perkelimo priskyrimo operatorius
Studentas& Studentas::operator=(Studentas&& kitas) noexcept {
	if (this != &kitas) {
		vardas_ = std::move(kitas.vardas_);
		pavarde_ = std::move(kitas.pavarde_);
		paz_ = std::move(kitas.paz_);
		egzaminas_ = kitas.egzaminas_;
		galutinis_ = kitas.galutinis_;
		kitas.egzaminas_ = 0;
		kitas.galutinis_ = 0.0;
	}
	return *this;
}

// Destruktorius. Visi nariai (std::string, std::vector) patys atlaisvina atminti (RAII),
// todel rankinio atlaisvinimo nereikia; destruktorius apibreztas aiskiai del Rule of Five.
Studentas::~Studentas() {}

void Studentas::skaiciuotiGalutini() {
	galutinis_ = 0.4 * vidurkis(paz_) + 0.6 * egzaminas_;
}

std::ostream& operator<<(std::ostream& out, const Studentas& s) {
	out << std::left << std::setw(utf8Plotis(s.vardas_)) << s.vardas_ << "|";
	out << std::setw(utf8Plotis(s.pavarde_)) << s.pavarde_ << "|";
	out << std::right << std::setw(plotis) << s.galutinis_ << "|\n";
	return out;
}
