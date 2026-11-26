# Studentų Rezultatai v1.0

C++ programa studentų rezultatams skaičiuoti ir `std::vector` bei `std::list` konteinerių greičiui palyginti, skaidant studentus į dvi grupes („vargšiukai“ ir „kietiakai“) pagal tris strategijas.

## Turinys

1. [Releasai](#releasai)
2. [Diegimas](#diegimas)
3. [Naudojimas](#naudojimas)
4. [Strategijos](#strategijos)
5. [Testavimo aplinka ir metodika](#testavimo-aplinka-ir-metodika)
6. [Rezultatai](#rezultatai)
7. [Išvados](#išvados)
8. [Projekto struktūra](#projekto-struktūra)

## Releasai

| Versija | Aprašymas |
|---|---|
| v0.1 | Pagrindinė programa: studentų duomenų įvedimas, galutinio balo skaičiavimas (`0.4 * namų darbų vidurkis + 0.6 * egzaminas`). |
| v0.2 | Duomenų skaitymas iš failų, testinių failų generavimas, rūšiavimas, studentų skaidymas į „vargšiukus“ ir „kietiakus“, laiko matavimas. |
| v0.3 | `std::vector` ir `std::list` palyginimas, dvi skaidymo strategijos, rezultatų lentelės į `rezultatai.md`. |
| **v1.0** | Trys skaidymo strategijos abiem konteineriams, optimizuota 3 strategija (STL algoritmai ir `splice`), sutvarkytas `CMakeLists.txt` su instaliavimu, išvalytas kodas, išsamūs rezultatai ir išvados. |

## Diegimas

Reikalinga: C++20 kompiliatorius (MSVC 2022, GCC 10+ arba Clang 12+) ir CMake 3.16+. Programa veikia Windows, Linux ir macOS.

```bash
git clone <jūsų repozitorijos nuoroda>
cd <repozitorijos aplankas>
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
cmake --install build --config Release --prefix ./instaliacija
```

Po instaliavimo programa yra `instaliacija/bin/` aplanke. Diegimo paketą (ZIP) galima sukurti taip:

```bash
cd build
cpack -C Release
```

**Visual Studio:** atidarykite `StudentuRezultatai.slnx`, pasirinkite `Release` ir `x64`, tada `Build > Build Solution`.

## Naudojimas

```bash
StudentuRezultatai [maksimalus failo dydis] [kartų skaičius]
```

- `maksimalus failo dydis` – didžiausias testinio failo studentų skaičius (numatyta `100000`, mažiausia `1000`). Testuojami dydžiai 1000, 10000, 100000, ... iki nurodyto.
- `kartų skaičius` – kiek kartų kartoti kiekvieną testą, rezultatas yra vidurkis (numatyta `3`).

Pavyzdys: `StudentuRezultatai 1000000 3`

Paleidus pasirodo meniu:

- `1` – rankinis įvedimas (rodomi objektų adresai atmintyje `vector` ir `list` atveju);
- `2` – konteinerių testavimas;
- `0` – išeiti.

Pasirinkus `2`:

1. pasirenkamas rūšiavimo kriterijus (vardas, pavardė, galutinis balas);
2. jei testiniai failai `kursiokai1000.txt`, `kursiokai10000.txt`, ... dar neegzistuoja, jie sugeneruojami automatiškai (kitu atveju programa paklausia, ar generuoti iš naujo), todėl **visi testai naudoja tuos pačius failus**;
3. kiekvienam dydžiui, konteineriui (`vector`, `list`) ir strategijai (1–3) testas kartojamas nurodytą kartų skaičių;
4. vidutiniai laikai įrašomi į `rezultatai.md`, o išvesties failai (`nuskriaustukai_*`, `kietukai_*`) – į darbo aplanką.

## Strategijos

Studentas yra „vargšiukas“, jei galutinis balas < 5.0, kitu atveju „kietiakas“.

| Strategija | Aprašymas | `vector` | `list` |
|---|---|---|---|
| 1 | Studentai kopijuojami į **du naujus** konteinerius, bendras konteineris lieka nepakitęs. Daugiausia atminties. | `std::copy_if` du kartus | `std::copy_if` du kartus |
| 2 | Sukuriamas **vienas naujas** konteineris (vargšiukai), jie ištrinami iš bendro. | `copy_if` + `erase(remove_if(...))` | `copy_if` + `list::remove_if` |
| 3 | Optimizuota 2 strategija: nieko nekopijuojama. | `std::stable_partition` + `std::move` + vienas `erase` | `std::list::splice` (mazgai perkeliami be kopijavimo) |

**Algoritmų pasirinkimas.** Iš pateikto sąrašo naudojami: `std::copy_if` (1 ir 2), `std::remove_if` (2 vektoriui), `std::stable_partition` (3 vektoriui). `stable_partition` išlaiko studentų tvarką (jie jau surūšiuoti) ir viena eiga suskirsto vektorių į dvi dalis, todėl nereikia kopijuoti kietiakų. `std::partition` būtų šiek tiek greitesnis, bet suardytų jau surūšiuotą tvarką. `find`, `search`, `transform` šiam uždaviniui netinka, nes nieko neskaido.

## Testavimo aplinka ir metodika

> **PASTABA: pakeiskite šį skyrių savo kompiuterio duomenimis ir savo paleidimo rezultatais.**

- Procesorius: _įrašykite_
- RAM: _įrašykite_
- Diskas (SSD/HDD): _įrašykite_
- Kompiliatorius ir režimas: _pvz. MSVC 2022, Release, /O2_
- Failai sugeneruoti vieną kartą ir naudoti visiems testams; kiekvienas testas kartotas 3 kartus, lentelėse pateikiamas **laikų vidurkis**.
- Matuojama: nuskaitymas, galutinių balų skaičiavimas, rūšiavimas, dalijimas į dvi grupes, išvedimas į failus, bendras laikas.

## Rezultatai

Lentelės žemiau gautos paleidus `StudentuRezultatai 1000000 3` (Linux, g++ 13, `-O2`).

#### std::vector, 1 strategija

| Studentu skaicius | Nuskaitymas | Galutiniu skaiciavimas | Rusiavimas | Dalijimas i 2 grupes | Nuskriaustuku isvedimas | Kietuku isvedimas | Viso |
|---|---|---|---|---|---|---|---|
| 1000 | 0.001 s | 0.000 s | 0.000 s | 0.000 s | 0.000 s | 0.000 s | 0.002 s |
| 10000 | 0.010 s | 0.000 s | 0.002 s | 0.001 s | 0.002 s | 0.002 s | 0.017 s |
| 100000 | 0.118 s | 0.002 s | 0.035 s | 0.024 s | 0.014 s | 0.015 s | 0.207 s |
| 1000000 | 0.899 s | 0.014 s | 0.569 s | 0.266 s | 0.138 s | 0.143 s | 2.029 s |

#### std::list, 1 strategija

| Studentu skaicius | Nuskaitymas | Galutiniu skaiciavimas | Rusiavimas | Dalijimas i 2 grupes | Nuskriaustuku isvedimas | Kietuku isvedimas | Viso |
|---|---|---|---|---|---|---|---|
| 1000 | 0.001 s | 0.000 s | 0.000 s | 0.000 s | 0.000 s | 0.000 s | 0.002 s |
| 10000 | 0.009 s | 0.000 s | 0.002 s | 0.001 s | 0.002 s | 0.002 s | 0.016 s |
| 100000 | 0.133 s | 0.002 s | 0.047 s | 0.024 s | 0.016 s | 0.015 s | 0.236 s |
| 1000000 | 1.089 s | 0.023 s | 0.974 s | 0.487 s | 0.144 s | 0.143 s | 2.861 s |

#### std::vector, 2 strategija

| Studentu skaicius | Nuskaitymas | Galutiniu skaiciavimas | Rusiavimas | Dalijimas i 2 grupes | Nuskriaustuku isvedimas | Kietuku isvedimas | Viso |
|---|---|---|---|---|---|---|---|
| 1000 | 0.001 s | 0.000 s | 0.000 s | 0.000 s | 0.000 s | 0.000 s | 0.002 s |
| 10000 | 0.008 s | 0.000 s | 0.002 s | 0.000 s | 0.002 s | 0.002 s | 0.015 s |
| 100000 | 0.125 s | 0.002 s | 0.032 s | 0.010 s | 0.019 s | 0.016 s | 0.204 s |
| 1000000 | 0.901 s | 0.014 s | 0.608 s | 0.160 s | 0.279 s | 0.230 s | 2.192 s |

#### std::list, 2 strategija

| Studentu skaicius | Nuskaitymas | Galutiniu skaiciavimas | Rusiavimas | Dalijimas i 2 grupes | Nuskriaustuku isvedimas | Kietuku isvedimas | Viso |
|---|---|---|---|---|---|---|---|
| 1000 | 0.001 s | 0.000 s | 0.000 s | 0.000 s | 0.000 s | 0.000 s | 0.002 s |
| 10000 | 0.009 s | 0.000 s | 0.002 s | 0.001 s | 0.002 s | 0.002 s | 0.017 s |
| 100000 | 0.105 s | 0.002 s | 0.045 s | 0.019 s | 0.021 s | 0.021 s | 0.213 s |
| 1000000 | 0.836 s | 0.022 s | 0.943 s | 0.527 s | 0.416 s | 0.285 s | 3.028 s |

#### std::vector, 3 strategija

| Studentu skaicius | Nuskaitymas | Galutiniu skaiciavimas | Rusiavimas | Dalijimas i 2 grupes | Nuskriaustuku isvedimas | Kietuku isvedimas | Viso |
|---|---|---|---|---|---|---|---|
| 1000 | 0.001 s | 0.000 s | 0.000 s | 0.000 s | 0.000 s | 0.000 s | 0.002 s |
| 10000 | 0.009 s | 0.000 s | 0.002 s | 0.000 s | 0.002 s | 0.002 s | 0.015 s |
| 100000 | 0.101 s | 0.001 s | 0.030 s | 0.005 s | 0.015 s | 0.016 s | 0.169 s |
| 1000000 | 0.837 s | 0.016 s | 0.607 s | 0.141 s | 0.198 s | 0.225 s | 2.024 s |

#### std::list, 3 strategija

| Studentu skaicius | Nuskaitymas | Galutiniu skaiciavimas | Rusiavimas | Dalijimas i 2 grupes | Nuskriaustuku isvedimas | Kietuku isvedimas | Viso |
|---|---|---|---|---|---|---|---|
| 1000 | 0.001 s | 0.000 s | 0.000 s | 0.000 s | 0.000 s | 0.000 s | 0.002 s |
| 10000 | 0.009 s | 0.000 s | 0.001 s | 0.000 s | 0.002 s | 0.002 s | 0.015 s |
| 100000 | 0.112 s | 0.002 s | 0.043 s | 0.006 s | 0.017 s | 0.020 s | 0.200 s |
| 1000000 | 0.989 s | 0.023 s | 0.950 s | 0.181 s | 0.303 s | 0.318 s | 2.763 s |

#### Palyginimas: vector vs list, 1 strategija

| Studentu skaicius | Rusiavimas (vector) | Rusiavimas (list) | Dalijimas (vector) | Dalijimas (list) | Viso (vector) | Viso (list) |
|---|---|---|---|---|---|---|
| 1000 | 0.000 s | 0.000 s | 0.000 s | 0.000 s | 0.002 s | 0.002 s |
| 10000 | 0.002 s | 0.002 s | 0.001 s | 0.001 s | 0.017 s | 0.016 s |
| 100000 | 0.035 s | 0.047 s | 0.024 s | 0.024 s | 0.207 s | 0.236 s |
| 1000000 | 0.569 s | 0.974 s | 0.266 s | 0.487 s | 2.029 s | 2.861 s |

#### Palyginimas: vector vs list, 2 strategija

| Studentu skaicius | Rusiavimas (vector) | Rusiavimas (list) | Dalijimas (vector) | Dalijimas (list) | Viso (vector) | Viso (list) |
|---|---|---|---|---|---|---|
| 1000 | 0.000 s | 0.000 s | 0.000 s | 0.000 s | 0.002 s | 0.002 s |
| 10000 | 0.002 s | 0.002 s | 0.000 s | 0.001 s | 0.015 s | 0.017 s |
| 100000 | 0.032 s | 0.045 s | 0.010 s | 0.019 s | 0.204 s | 0.213 s |
| 1000000 | 0.608 s | 0.943 s | 0.160 s | 0.527 s | 2.192 s | 3.028 s |

#### Palyginimas: vector vs list, 3 strategija

| Studentu skaicius | Rusiavimas (vector) | Rusiavimas (list) | Dalijimas (vector) | Dalijimas (list) | Viso (vector) | Viso (list) |
|---|---|---|---|---|---|---|
| 1000 | 0.000 s | 0.000 s | 0.000 s | 0.000 s | 0.002 s | 0.002 s |
| 10000 | 0.002 s | 0.001 s | 0.000 s | 0.000 s | 0.015 s | 0.015 s |
| 100000 | 0.030 s | 0.043 s | 0.005 s | 0.006 s | 0.169 s | 0.200 s |
| 1000000 | 0.607 s | 0.950 s | 0.141 s | 0.181 s | 2.024 s | 2.763 s |

#### Strategiju palyginimas: vector (dalijimo i 2 grupes laikas)

| Studentu skaicius | 1 strategija | 2 strategija | 3 strategija |
|---|---|---|---|
| 1000 | 0.000 s | 0.000 s | 0.000 s |
| 10000 | 0.001 s | 0.000 s | 0.000 s |
| 100000 | 0.024 s | 0.010 s | 0.005 s |
| 1000000 | 0.266 s | 0.160 s | 0.141 s |

#### Strategiju palyginimas: list (dalijimo i 2 grupes laikas)

| Studentu skaicius | 1 strategija | 2 strategija | 3 strategija |
|---|---|---|---|
| 1000 | 0.000 s | 0.000 s | 0.000 s |
| 10000 | 0.001 s | 0.001 s | 0.000 s |
| 100000 | 0.024 s | 0.019 s | 0.006 s |
| 1000000 | 0.487 s | 0.527 s | 0.181 s |


## Išvados

- **Greičiausia strategija – 3.** 1 000 000 studentų dalijimas `vector` atveju: 0.266 s (1 str.) → 0.160 s (2 str.) → 0.141 s (3 str.). `list` atveju: 0.487 s → 0.527 s → 0.181 s. 3 strategija neturi kopijavimo ir atminties kaštų.
- **`list` 2 strategija gali būti lėtesnė už 1.** `list::remove_if` ir kopijavimas vykdomi po vieną mazgą, o kiekvienas mazgas alokuojamas atskirai.
- **Rūšiavimas** `list` konteineryje ~1.6 karto lėtesnis nei `vector` (0.95 s prieš 0.61 s, 1 000 000 įrašų), nes mazgai išsklaidyti atmintyje.
- **Bendras laikas:** `vector` visada greitesnis arba lygus `list` (2.02 s prieš 2.76 s, 3 strategija, 1 000 000). Dideliems failams laiką lemia ne konteineris, o failų skaitymas ir rašymas.
- **Mažiems dydžiams (iki 10 000)** skirtumo tarp konteinerių ir strategijų praktiškai nėra.
- **Rekomendacija:** naudoti `std::vector` ir 3 strategiją (`stable_partition`).

## Projekto struktūra

- `main.cpp` – meniu ir testavimo paleidimas
- `ivestis.cpp/.h` – rankinis įvedimas
- `studentas.cpp/.h` – `Studentas` struktūra ir galutinio balo skaičiavimas
- `failai.cpp/.h` – testinių failų generavimas
- `konteineriai.h` – skaitymas, rašymas, rūšiavimas ir skaidymo strategijos (šablonai)
- `benchmark.h` – vieno failo testavimas pagal konteinerį ir strategiją
- `results.cpp/.h` – lentelių generavimas
- `utils.cpp/.h`, `timer.h` – pagalbinės funkcijos ir laikmatis
- `CMakeLists.txt` – kompiliavimas, instaliavimas, paketavimas
