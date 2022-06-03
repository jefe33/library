#ifndef INDEX_H_   /* Include guard */
#define INDEX_H_

#include "main.h"
#include "ksiazki.h"
#include "klienci.h"
#include "wypozyczenia.h"

struct klient {
    int telefon;
    char *imie, *nazwisko, *adres;
    struct ksiazki *wypozyczone;
};

void usunKsiazke();

void usunAutora();

void usunRok();

void wyswietlMenu();

void wyswietlMenuKsiazek();

void wyswietlMenuKlientow();

void wyswietlMenuWypozyczen();

#endif