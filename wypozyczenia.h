struct wypozyczenia {
    int numer, index_ksiazki, index_klienta;
    struct wypozyczenia *next;
};

struct wypozyczenia *wczytajWypozyczenia(FILE *in_stream, int *max) {
    *max = 0;
    struct wypozyczenia head;
    head.next = NULL; // code only uses the `next` field of head

    struct wypozyczenia *previous = &head;
    struct wypozyczenia x;

    // While another record was successfully read ...
    while (fread(&x, sizeof(struct wypozyczenia), 1, in_stream) == 1) {
        // Fill the next field
        if (x.numer > *max) *max = x.numer;
        x.next = NULL;

        // Allocate space and copy
        previous->next = malloc(sizeof *(previous->next));
        //assert(previous->next);
        *(previous->next) = x;

        // Advance to the next
        previous = previous->next;
    }
    return head.next;
}

void uwolnicWyporzyczenia(struct wypozyczenia *w) {
    struct wypozyczenia *tmp;
    while (w) {
        tmp = w;
        w = w->next;
        free(tmp);
    }
    free(w);
}

void usunWypozyczenie() {
    int max, i = 0, nr, nr_klienta;
    printf("Podaj numer wypozyczenia ktore chcesz usunac\n");
    scanf("%i", &nr);
    FILE *f = fopen("../pliki/wypozyczenia.bin", "rb");
    if (f != NULL) {
        fseek(f, 0, SEEK_END);
        if (ftell(f) != 0) {
            rewind(f);
            struct wypozyczenia *wyp = wczytajWypozyczenia(f, &max);
            struct wypozyczenia *tmp = wyp;
            if (tmp->numer == nr) {
                nr_klienta = tmp->index_klienta;
                i = 1;
                wyp = tmp->next;
            } else {
                struct wypozyczenia *prev = wyp;
                tmp = wyp;
                while (tmp) {
                    if (tmp->numer == nr) {
                        nr_klienta = tmp->index_klienta;
                        prev->next = tmp->next;
                        i = 1;
                        break;
                    } else {
                        prev = tmp;
                    }
                    tmp = tmp->next;
                }
            }
            if (i) {
                FILE *f2 = fopen("../pliki/klienci.bin", "rb");
                if (f2) {
                    struct klienci *klient = wczytajKlientow(f2, &max);
                    struct klienci *temp = klient;
                    while (temp) {
                        if (temp->index == nr_klienta) {
                            temp->ilosc -= 1;
                            break;
                        }
                        temp = temp->next;
                    }
                    fclose(f2);
                    f2 = fopen("../pliki/klienci.bin", "wb");
                    temp = klient;
                    while (temp && fwrite(temp, sizeof(struct klienci), 1, f2) == 1) {
                        temp = temp->next;
                    }
                    fclose(f2);
                }
                tmp = wyp;
                fclose(f);
                f = fopen("../pliki/wypozyczenia.bin", "wb");
                while (tmp && fwrite(tmp, sizeof(struct wypozyczenia), 1, f) == 1) {
                    tmp = tmp->next;
                }
            }
            uwolnicWyporzyczenia(wyp);
        }
    }
    fclose(f);
}

void wyswietlWypozyczenia() {
    FILE *f = fopen("../pliki/wypozyczenia.bin", "rb");
    int max;
    if (f != NULL) {
        struct wypozyczenia *wyp = wczytajWypozyczenia(f, &max);
        struct wypozyczenia *tmp = wyp;
        if (tmp != NULL) {
            printf("---------------------------------------------\n");
            printf("| numer |  index ksiazki  |  index klienta  |\n");
            printf("---------------------------------------------\n");
            while (tmp) {
                printf("| %-5i | ", tmp->numer);
                printf("%-15i | ", tmp->index_ksiazki);
                printf("%-15i | ", tmp->index_klienta);
                printf("\n");
                tmp = tmp->next;
            }
        } else printf("Brak wypozyczen\n");
        uwolnicWyporzyczenia(wyp);
    } else printf("Brak wypozyczen\n");
    fclose(f);
}

void dodajWypozyczenie() {
    int nr, ind, max, i = 1, x;
    printf("0. Wroc\n");
    printf("1. Dodaj wypozyczenie oraz dodaj klienta\n");
    printf("2. Dodaj wypozyczenie z istniejacym juz w bazie klientem\n");
    scanf("%i", &x);
    if (x == 1) {
        ind = dodajKlienta(0);
    } else if (x == 2) {
        printf("Podaj numer klienta\n");
        scanf("%i", &ind);
    } else {
        return;
    }
    printf("Podaj numer ksiazki\n");
    scanf("%i", &nr);
    FILE *f1 = fopen("../pliki/ksiazki.bin", "rb");
    FILE *f2 = fopen("../pliki/klienci.bin", "rb");
    if (f1 && f2) {
        struct ksiazki *ks = wczytajKsiazki(f1, &max);
        if (nr > max || nr < 1) {
            fclose(f1);
            return;
        }
        struct klienci *kl = wczytajKlientow(f2, &max);
        if (ind > max || ind < 1) {
            fclose(f2);
            return;
        }
        struct ksiazki *tmp1 = ks;
        while (tmp1) {
            if (tmp1->nr_katalogowy == nr) {
                i = 0;
                break;
            }
            tmp1 = tmp1->next;
        }
        if (i == 1) {
            uwolnicKsiazki(ks);
            uwolnicKlientow(kl);
            fclose(f1);
            fclose(f2);
            return;
        }
        i = 1;
        struct klienci *tmp2 = kl;
        while (tmp2) {
            if (tmp2->index == ind) {
                tmp2->ilosc += 1;
                i = 0;
                break;
            }
            tmp2 = tmp2->next;
        }
        if (i == 1) {
            uwolnicKsiazki(ks);
            uwolnicKlientow(kl);
            fclose(f1);
            fclose(f2);
            return;
        }
        fclose(f2);
        f2 = fopen("../pliki/klienci.bin", "wb");
        tmp2 = kl;
        while (tmp2 && fwrite(tmp2, sizeof(struct klienci), 1, f2) == 1) {
            tmp2 = tmp2->next;
        }
        FILE *wyp = fopen("../pliki/wypozyczenia.bin", "rb");
        struct wypozyczenia *wypozyczenie = malloc(sizeof(struct wypozyczenia));
        wypozyczenie->index_klienta = ind;
        wypozyczenie->index_ksiazki = nr;
        wypozyczenie->next = NULL;
        if (wyp) {
            fseek(wyp, 0, SEEK_END);
            if (ftell(wyp) != 0) {
                rewind(wyp);
                struct wypozyczenia *w1 = wczytajWypozyczenia(wyp, &max);
                wypozyczenie->numer = max + 1;
                struct wypozyczenia *tmp = w1;
                if (tmp->next == NULL) {
                    tmp->next = wypozyczenie;
                } else {
                    while (tmp) {
                        if (tmp->next == NULL) {
                            tmp->next = wypozyczenie;
                            break;
                        }
                        tmp = tmp->next;
                    }
                }
                tmp = w1;
                fclose(wyp);
                wyp = fopen("../pliki/wypozyczenia.bin", "wb");
                while (tmp && fwrite(tmp, sizeof(struct wypozyczenia), 1, wyp) == 1) {
                    tmp = tmp->next;
                }
                uwolnicWyporzyczenia(w1);
            } else {
                fclose(wyp);
                wyp = fopen("../pliki/wypozyczenia.bin", "wb");
                wypozyczenie->numer = 1;
                fwrite(wypozyczenie, sizeof(struct wypozyczenia), 1, wyp);
                free(wypozyczenie);
            }
        } else {
            fclose(wyp);
            wyp = fopen("../pliki/wypozyczenia.bin", "wb");
            wypozyczenie->numer = 1;
            fwrite(wypozyczenie, sizeof(struct wypozyczenia), 1, wyp);
            free(wypozyczenie);
        }
        uwolnicKsiazki(ks);
        uwolnicKlientow(kl);
        fclose(wyp);
    }
    fclose(f1);
    fclose(f2);
}