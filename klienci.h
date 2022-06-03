#ifndef KLIENCI_H_   /* Include guard */
#define KLIENCI_H_
#include "main.h"

struct klienci {
    int index, ilosc;
    char nazwisko[MAX], imie[MAX], adres[MAX], telefon[MAX];
    struct klienci *next;
};

struct klienci *wczytajKlientow(FILE *in_stream, int *max);

void uwolnicKlientow(struct klienci *t);

void usunKlienta();

char *wczytajKlienta(char *s);

struct klienci *sortowaniePrzezWstawianieKL(struct klienci *l);

void wyswietlKlienta(struct klienci *k);

struct klienci *wstawPosortowanychKlientow(struct klienci *, struct klienci *);

struct klienci *utworzKlienta();

int dodajKlienta(int);

struct klienci *losowyKlient();

void wyswietlKlientow(int n);

void edytujKlienta();

#endif