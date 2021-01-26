struct wypozyczenia {
    int numer, index_ksiazki, index_klienta;
    struct wypozyczenia *next;
};

struct wypozyczenia *wczytajWypozyczenia(FILE *in_stream, int *max) {
    *max = 0;
    struct wypozyczenia head;
    head.next = NULL;

    struct wypozyczenia *previous = &head;
    struct wypozyczenia x;

    while (fread(&x, sizeof(struct wypozyczenia), 1, in_stream) == 1) {
        if (x.numer > *max) *max = x.numer;
        x.next = NULL;

        previous->next = malloc(sizeof *(previous->next));
        *(previous->next) = x;

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
}

void usunWypozyczenie() {
    int max, i = 0, nr, nr_klienta;
    printf("Podaj numer wypozyczenia ktore chcesz usunac\n");
    if (scanf("%i", &nr) == 0) return;
    FILE *f = fopen("../pliki/wypozyczenia.bin", "rb");
    if (f) {
        fseek(f, 0, SEEK_END);
        if (ftell(f) != 0) {
            rewind(f);
            struct wypozyczenia *wyp = wczytajWypozyczenia(f, &max);
            struct wypozyczenia *tmp = wyp;
            struct wypozyczenia *tmp2 = NULL;
            if (tmp->numer == nr) {
                nr_klienta = tmp->index_klienta;
                i = 1;
                wyp = tmp->next;
                free(tmp);
            } else {
                struct wypozyczenia *prev = wyp;
                tmp = wyp->next;
                while (tmp) {
                    if (tmp->numer == nr) {
                        nr_klienta = tmp->index_klienta;
                        tmp2 = prev->next;
                        prev->next = tmp->next;
                        i = 1;
                        free(tmp2);
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
                FILE *f1 = fopen("../pliki/ksiazki.bin", "rb");
                if (f1) {
                    struct ksiazki *ks = wczytajKsiazki(f1, &max);
                    struct ksiazki *temp = ks;
                    while (temp) {
                        if (temp->nr_katalogowy == nr_klienta) {
                            temp->dostepnosc += 1;
                            break;
                        }
                        temp = temp->next;
                    }
                    fclose(f1);
                    f1 = fopen("../pliki/ksiazki.bin", "wb");
                    temp = ks;
                    while (temp && fwrite(temp, sizeof(struct ksiazki), 1, f1) == 1) {
                        temp = temp->next;
                    }
                    fclose(f1);
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
            printf("------------------------------------------------\n");
            printf("  numer        index ksiazki     index klienta  \n");
            printf("------------------------------------------------\n");
            while (tmp) {
                printf("%5i  ", tmp->numer);
                printf("%15i  ", tmp->index_ksiazki);
                printf("%15i  ", tmp->index_klienta);
                printf("\n");
                tmp = tmp->next;
            }
        } else printf("Brak wypozyczen\n");
        uwolnicWyporzyczenia(wyp);
    } else printf("Brak wypozyczen\n");
    fclose(f);
}

void czysczenie(FILE *f, FILE *f2, struct ksiazki *ks, struct klienci *kl) {
    uwolnicKsiazki(ks);
    uwolnicKlientow(kl);
    fclose(f);
    fclose(f2);
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
        if (scanf("%i", &ind) == 0) return;
    } else {
        return;
    }
    printf("Podaj numer ksiazki\n");
    if (scanf("%i", &nr) == 0) return;
    FILE *f1 = fopen("../pliki/ksiazki.bin", "rb");
    FILE *f2 = fopen("../pliki/klienci.bin", "rb");
    if (f1 && f2) {
        struct ksiazki *ks = wczytajKsiazki(f1, &max);
        if (nr > max || nr < 1) {
            printf("Nie ma ksiazki o takim numerze\n");
            uwolnicKsiazki(ks);
            fclose(f1);
            return;
        }
        struct klienci *kl = wczytajKlientow(f2, &max);
        if (ind > max || ind < 1) {
            printf("Nie ma klienta o takim numerze\n");
            czysczenie(f1, f2, ks, kl);
            return;
        }
        struct ksiazki *tmp1 = ks;
        while (tmp1) {
            if (tmp1->nr_katalogowy == nr && tmp1->dostepnosc > 0) {
                tmp1->dostepnosc -= 1;
                i = 0;
                break;
            }
            tmp1 = tmp1->next;
        }
        if (i == 1) {
            printf("Nie ma ksiazki o takim numerze lub nie jest dostepna\n");
            czysczenie(f1, f2, ks, kl);
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
            printf("Nie ma klienta o takim numerze\n");
            czysczenie(f1, f2, ks, kl);
            return;
        }
        fclose(f2);
        f2 = fopen("../pliki/klienci.bin", "wb");
        tmp2 = kl;
        while (tmp2 && fwrite(tmp2, sizeof(struct klienci), 1, f2) == 1) {
            tmp2 = tmp2->next;
        }
        fclose(f1);
        f1 = fopen("../pliki/ksiazki.bin", "wb");
        tmp1 = ks;
        while (tmp1 && fwrite(tmp1, sizeof(struct ksiazki), 1, f1) == 1) {
            tmp1 = tmp1->next;
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