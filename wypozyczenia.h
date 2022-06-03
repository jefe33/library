#ifndef WYPOZYCZENIA_H_   /* Include guard */
#define WYPOZYCZENIA_H_
#include "main.h"
#include "ksiazki.h"
#include "klienci.h"

struct wypozyczenia {
    int numer, index_ksiazki, index_klienta;
    struct wypozyczenia *next;
};

void dodajWypozyczenie();
void czysczenie(FILE *f, FILE *f2, struct ksiazki *ks, struct klienci *kl);
void wyswietlWypozyczenia();
void usunWypozyczenie();
void uwolnicWyporzyczenia(struct wypozyczenia *w);
struct wypozyczenia *wczytajWypozyczenia(FILE *in_stream, int *max);

#endif