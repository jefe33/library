#include "klienci.h"
#include "ksiazki.h"

struct klienci *wczytajKlientow(FILE *in_stream, int *max) {
    *max = 0;
    struct klienci head;
    head.next = NULL;

    struct klienci *previous = &head;
    struct klienci x;

    while (fread(&x, sizeof(struct klienci), 1, in_stream) == 1) {
        if (x.index > *max) *max = x.index;
        x.next = NULL;

        previous->next = malloc(sizeof *(previous->next));
        *(previous->next) = x;

        previous = previous->next;
    }
    return head.next;
}

void uwolnicKlientow(struct klienci *t) {
    struct klienci *tmp;
    while (t) {
        tmp = t;
        t = t->next;
        free(tmp);
    }
}

void usunKlienta() {
    int max, i = 0, nr;
    printf("Podaj numer klienta ktorego chcesz usunac\n");
    if (scanf("%i", &nr) == 0) return;
    FILE *f = fopen("../pliki/klienci.bin", "rb");
    if (f != NULL) {
        struct klienci *kl = wczytajKlientow(f, &max);
        struct klienci *tmp = kl;
        struct klienci *tmp2 = NULL;
        if (tmp->index == nr) {
            i = 1;
            if (tmp->ilosc > 0) {
                i = 2;
            } else {
                kl = tmp->next;
                free(tmp);
            }
        } else {
            struct klienci *prev = kl;
            tmp = kl;
            while (tmp) {
                if (tmp->index == nr) {
                    i = 1;
                    if (tmp->ilosc > 0) {
                        i = 2;
                    } else {
                        tmp2 = prev->next;
                        prev->next = tmp->next;
                        free(tmp2);
                    }
                    break;
                } else {
                    prev = tmp;
                }
                tmp = tmp->next;
            }
        }
        if (i == 1) {
            tmp = kl;
            fclose(f);
            f = fopen("../pliki/klienci.bin", "wb");
            while (tmp && fwrite(tmp, sizeof(struct klienci), 1, f) == 1) {
                tmp = tmp->next;
            }
        } else if (i == 2) {
            printf("Nie mozna usunac bo klient ma wypozyczone ksiazki");
        }
        uwolnicKlientow(kl);
    }
    fclose(f);
}

int menuEdycjiKlienta() {
    printf("0.Wroc\n1.Zmien nazwisko klienta\n2.Zmien imie klienta\n3.Zmien adres klienta\n"
           "4.Zmien numer telefonu klienta\n");
    int x;
    scanf("%d", &x);
    return x;
}

void edytujKlienta() {
    int max, i = 1, nr, x;
    clear();
    printf("Podaj numer klienta ktorego chcesz edytowac\n");
    if (scanf("%i", &nr) == 0) return;
    FILE *f = fopen("../pliki/klienci.bin", "rb");
    if (f != NULL) {
        struct klienci *kl = wczytajKlientow(f, &max);
        if (nr > max || nr < 1) {
            uwolnicKlientow(kl);
            fclose(f);
            return;
        }
        struct klienci *tmp = kl;
        while (tmp) {
            if (tmp->index == nr) {
                i = 0;
                break;
            }
            tmp = tmp->next;
        }
        if (i == 1) {
            uwolnicKlientow(kl);
            fclose(f);
            return;
        }
        struct klienci *p = tmp->next;
        tmp->next = NULL;
        wyswietlKlienta(tmp);
        tmp->next = p;
        x = menuEdycjiKlienta();
        clear();
        switch (x) {
            case 0:
                break;
            case 1:
                strcpy(tmp->nazwisko, wczytajKsiazka("Podaj nazwisko klienta"));
                break;
            case 2:
                strcpy(tmp->imie, wczytajKsiazka("Podaj imie klienta"));
                break;
            case 3:
                strcpy(tmp->adres, wczytajKsiazka("Podaj adres klienta"));
                break;
            case 4:
                strcpy(tmp->telefon, wczytajKsiazka("Podaj numer telefonu klienta"));
                break;
            default:
                break;
        }
        tmp = kl;
        fclose(f);
        f = fopen("../pliki/klienci.bin", "wb");
        while (tmp && fwrite(tmp, sizeof(struct klienci), 1, f) == 1) {
            tmp = tmp->next;
        }
        uwolnicKlientow(kl);
    }
    fclose(f);
}

char *wczytajKlienta(char *s) {
    char *z;
    do {
        printf("%s\n", s);
        z = wczytajString();
    } while (is_empty(z));
    return z;
}

struct klienci *sortowaniePrzezWstawianieKL(struct klienci *l) {
    if (l == NULL || l->next == NULL) {
        return l;
    }
    struct klienci *sorted = NULL;
    struct klienci *tmp = l;
    struct klienci *tmp2;
    struct klienci *t = NULL;
    while (tmp) {
        t = tmp->next;
        if (!sorted || tmp->ilosc > sorted->ilosc) {
            tmp->next = sorted;
            sorted = tmp;
        } else {
            tmp2 = sorted;
            while (tmp2->next && tmp2->next->ilosc > tmp->ilosc) {
                tmp2 = tmp2->next;
            }
            tmp->next = tmp2->next;
            tmp2->next = tmp;
        }
        tmp = t;
    }
    return sorted;
}

void wyswietlKlienta(struct klienci *k) {
    printf("0.Wroc\n1.Wyswietl wszystkich klientow\n2.Wyswietl klientow wedlug poczatku nazwiska\n");
    int x;
    char s[MAX];
    if (scanf("%i", &x) == 0) return;
    if (x == 0) return;
    else if (x == 2) {
        clear();
        printf("Podaj poczatek nazwiska\n");
        strcpy(s, wczytajString());
    }
    printf("-----------------------------------------------------------------------------------------------------------------------\n");
    printf(" nr    imie          nazwisko                          adres                                telefon         ilosc_wypo \n");
    printf("-----------------------------------------------------------------------------------------------------------------------\n");
    while (k) {
        if ((x == 2 && strstr(k->nazwisko, s) == k->nazwisko) || x == 1) {
            printf(" %-3i  ", k->index);
            printf(" %-12s ", k->imie);
            printf("%-17s  ", k->nazwisko);
            printf("%-50s  ", k->adres);
            printf("%-16s  ", k->telefon);
            printf("   %-3i", k->ilosc);
            printf("\n");
        }
        k = k->next;
    }
}

void wyswietlKlientow(int n) {
    FILE *f = fopen("../pliki/klienci.bin", "rb");
    int max;
    if (f != NULL) {
        struct klienci *kl = wczytajKlientow(f, &max);
        if (n) {
            kl = sortowaniePrzezWstawianieKL(kl);
        }
        struct klienci *tmp = kl;
        if (tmp != NULL) {
            wyswietlKlienta(tmp);
            uwolnicKlientow(kl);
        } else printf("Brak Klientow\n");
        fclose(f);
    } else printf("Brak Klientow\n");
}


struct klienci *wstawPosortowanychKlientow(struct klienci *l, struct klienci *kl) {
    if (strcasecmp(l->nazwisko, kl->nazwisko) > 0) {
        kl->next = l;
        l = kl;
        return l;
    }
    struct klienci *tmp = l;
    while (tmp) {
        if (tmp->next == NULL) {
            tmp->next = kl;
            break;
        } else {
            if (tmp->next && strcasecmp(tmp->next->nazwisko, kl->nazwisko) > 0) {
                kl->next = tmp->next;
                tmp->next = kl;
                break;
            }
        }
        tmp = tmp->next;
    }
    return l;
}

struct klienci *utworzKlienta() {
    clear();
    struct klienci *klient = malloc(sizeof(struct klienci));
    strcpy(klient->nazwisko, wczytajKlienta("Podaj nazwisko klienta"));
    strcpy(klient->imie, wczytajKlienta("Podaj imie klienta"));
    strcpy(klient->adres, wczytajKlienta("Podaj adres klienta"));
    strcpy(klient->telefon, wczytajKlienta("Podaj numer telefonu klienta"));
    klient->index = 0;
    klient->ilosc = 0;
    return klient;
}

int dodajKlienta(int n) {
    char *Pklient = "../pliki/klienci.bin";
    struct klienci *k1;
    int ind;
    if (n) {
        k1 = losowyKlient();
    } else {
        k1 = utworzKlienta();
    }
    FILE *f = fopen(Pklient, "rb");
    if (f) {
        fseek(f, 0, SEEK_END);
        if (ftell(f) != 0) {
            rewind(f);
            int max;
            struct klienci *k2 = wczytajKlientow(f, &max);
            ind = max + 1;
            k1->index = ind;
            k2 = wstawPosortowanychKlientow(k2, k1);
            fclose(f);
            f = fopen(Pklient, "wb");
            struct klienci *tmp = k2;
            while (tmp && fwrite(tmp, sizeof(struct klienci), 1, f) == 1) {
                tmp = tmp->next;
            }
            uwolnicKlientow(k2);
        } else {
            fclose(f);
            f = fopen(Pklient, "wb");
            ind = 1;
            k1->index = ind;
            fwrite(k1, sizeof(struct klienci), 1, f);
            free(k1);
        }
    } else {
        fclose(f);
        f = fopen(Pklient, "wb");
        ind = 1;
        k1->index = ind;
        fwrite(k1, sizeof(struct klienci), 1, f);
        free(k1);
    }
    fclose(f);
    return ind;
}

struct klienci *losowyKlient() {
    srand(time(0));
    struct klienci *klient = malloc(sizeof(struct klienci));
    char imie[][MAX] = {"Jan", "Andrzej", "Maria", "Bartosz", "Karol", "Karolina", "Anna", "Pawel", "Tomasz",
                        "Donald", "Katarzyna", "Gustaw", "Artur"};
    char nazwisko[][MAX] = {"Nowak", "Wojcik", "Kowalczyk", "Wozniak", "Mazur", "Krawczyk", "Kaczmarek", "Zajac",
                            "Stepien", "Wrobel", "Wieczorek", "Krol"};
    char adres[][MAX] = {"ul. Moldawska 75, 61-614 Poznan", "ul. Ciemaly Jozefa 143, 40-862 Katowice",
                         "ul. Biala 114, 60-804 Poznan", "ul. Bukietowa 10, 43-300 Bielsko-Biala",
                         "Pl. Wolnosci 112, 61-738 Poznan", "ul. Sokolow Bydgoskich 30, 85-153 Bydgoszcz",
                         "ul. Grzegorzecka 54, 31-559 Krakow", "ul. Kolejarzy 140, 43-602 Jaworzno",
                         "ul. Opatowska 142, 61-353 Poznan", "ul. Kolberga Oskara 12, 02-650 Warszawa",
                         "ul. Bobrowiecka 52, 00-728 Warszawa", "ul. Niestachowska 80, 60-669 Poznan",
                         "ul. Plywacka 48, 94-127 Lodz", "ul. Pocztowa 44, 40-002 Katowice",
                         "ul. Wolnosci 79, 08-330 Kosow Lacki"};
    char telefon[][MAX] = {"+48 72 470 75 42", "+48 53 716 49 59", "+48-605-5569-36", "+48-795-5544-93",
                           "+48-695-5595-15", "+48-455-5524-68", "+48-885-5539-14", "+48 88 421 38 59",
                           "+48 53 938 15 41", "+48 78 927 78 21", "+48 69 306 21 15"};
    strcpy(klient->nazwisko, nazwisko[rand() % 12]);
    strcpy(klient->imie, imie[rand() % 13]);
    strcpy(klient->adres, adres[rand() % 15]);
    strcpy(klient->telefon, telefon[rand() % 11]);
    klient->next = NULL;
    klient->index = 0;
    klient->ilosc = 0;
    return klient;
}