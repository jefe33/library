#ifndef KSIAZKI_H_   /* Include guard */
#define KSIAZKI_H_
#include "main.h"

struct ksiazki {
    unsigned int nr_isbn[13];
    int nr_katalogowy, dostepnosc, data_wydania;
    char autor_nazwisko[MAX], autor_imie[MAX], tytul[MAX], kategoria[MAX], wydawnictwo[MAX];
    struct ksiazki *next;
};

int is_empty(char *s);

void uwolnicKsiazki(struct ksiazki *t);

char *wczytajString();

char *wczytajKsiazka(char *s);

int is_positive_number(char *s, int len);

void wyswietlK(struct ksiazki *k);

void wyswietlKsiazki(int n);

int menuEdycjiKsiazki();

void edytujKsiazke();

struct ksiazki *utworzKsiazke();

struct ksiazki *sortowaniePrzezWstawianie(struct ksiazki *l, int);

struct ksiazki *wstawPosortowane(struct ksiazki *l, struct ksiazki *ks);

struct ksiazki *losowaKsiazka();

void dodajKsiazke(int);

struct ksiazki *wczytajKsiazki(FILE *in_stream, int *max);

void clear();

#endif