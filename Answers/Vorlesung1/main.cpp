#include <iostream>
#include <omp.h>
#include <random>

using namespace std;

int main() {
  int n = 100000000; // Anzahl der zu erzeugenden Punkte

  int anzahl_threads;
  cout << "Anzahl von Threads: " << endl;
  cin >> anzahl_threads;

  omp_set_num_threads(anzahl_threads); // Anzahl der Threads für OpenMP setzen

  int punkte_pro_thread = n / anzahl_threads; // Ganze Punkte pro Thread
  cout << "Ganze Punkte pro Thread: " << punkte_pro_thread << endl;
  int restliche_punkte = n % anzahl_threads;  // Restliche Punkte
  int counter = 0;

  auto start_time = omp_get_wtime(); // omp_get_wtime() - Zeitmessung
#pragma omp parallel default(none) shared(cout,  counter, n, anzahl_threads, punkte_pro_thread, restliche_punkte) // parallel-region!
    {
        auto thread_id = omp_get_thread_num();

#pragma omp critical
      {
      cout << "Thread ID: " << thread_id << endl;
    }

    unsigned int seed = thread_id;  // jedes Thread hat eigenen Seed
    default_random_engine re{seed}; // Klasse für Zufallszahlengeneratoren
    // {} brace-initialization, Uniform Initializer Syntax
    uniform_real_distribution<double> zero_to_one{0.0, 1.0}; // Klasse aus random für die Erzeugung von gleichmäßig verteilten Gleitkommazahlen
    int  local_counter = 0; // Zähler für Punkte die im Viertelkreis liegen


    int start = thread_id * punkte_pro_thread; // startindex für Punkte die dieser Thread verarbeitet
    int end = (thread_id == anzahl_threads - 1) ? (start + punkte_pro_thread + restliche_punkte) : (start + punkte_pro_thread);
    // compute n points and test if they lie within the first quadrant of a unit circle
    for (int i = start; i < end; ++i) {
        // mit auto - automatisch den Datentyp ableiten
        auto x = zero_to_one(re); // zufällige x-Koordinate (between 0.0 and 1.0)
        auto y = zero_to_one(re); // zufällige y-Koordinate between 0.0 and 1.0
        if (x * x + y * y <= 1.0) { // der Punkt liegt innerhalb des Kreises oder genau auf dem Rand!
            ++ local_counter;
        }

    }

    #pragma omp atomic
    counter += local_counter;
    }
    auto run_time = omp_get_wtime() - start_time;
    auto pi = 4 * (double(counter) / n);

    cout << "pi: " << pi << endl;
    cout << "run_time: " << run_time << " s" << endl;}
