struct klienci {
    int index;
    char nazwisko[MAX], imie[MAX], adres[MAX], telefon[MAX];
    struct klienci *next;
};

struct klienci *wczytajKlientow(FILE *in_stream, int *max);

void uwolnicKlientow(struct klienci *t);

void usunKlienta();

char *wczytajKlienta(char *s);

struct klienci *sortowaniePrzezWstawianieKL(struct klienci *l);

void wyswietlKlientow(struct klienci *k);

void wyswietlWedlugLiczby();

void wyswietlWedlugNazwiska();

struct klienci *wstawPosortowanych(struct klienci *l, struct klienci *ks);

struct klienci *utworzKlienta();

void dodajKlienta(int);

struct klienci *losowyKlient();

struct klienci *wczytajKlientow(FILE *in_stream, int *max) {
    *max = 0;
    struct klienci head;
    head.next = NULL; // code only uses the `next` field of head

    struct klienci *previous = &head;
    struct klienci x;

    // While another record was successfully read ...
    while (fread(&x, sizeof(struct klienci), 1, in_stream) == 1) {
        // Fill the next field
        if (x.index > *max) *max = x.index;
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

void uwolnicKlientow(struct klienci *t) {
    struct klienci *tmp;
    while (t) {
        tmp = t;
        t = t->next;
        free(tmp);
    }
    free(t);
}

void usunKlienta() {
    int max, i = 1, nr;
    printf("Podaj numer klienta ktorego chcesz usunac\n");
    scanf("%i", &nr);
    FILE *f = fopen("../pliki/klienci.bin", "rb");
    if (f != NULL) {
        struct klienci *kl = wczytajKlientow(f, &max);
        struct klienci *tmp = kl;
        while (tmp && i) {
            if (tmp->index == nr && i) {
                kl = tmp->next;
            } else {
                i = 0;
            }
            tmp = tmp->next;
        }
        struct klienci *prev = kl;
        tmp = kl;
        while (tmp) {
            if (tmp->index == nr) {
                prev->next = tmp->next;
            } else {
                prev = tmp;
            }
            tmp = tmp->next;
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
    /*struct klienci *sorted = NULL;
    struct klienci *tmp = l;
    struct klienci *tmp2;
    struct klienci *t = NULL;
    while (tmp) {
        t = tmp->next;
        if (!sorted || strcasecmp(tmp->tytul, sorted->tytul) <= 0) {
            tmp->next = sorted;
            sorted = tmp;
        } else {
            tmp2 = sorted;
            while (tmp2->next && strcasecmp(tmp2->next->tytul, tmp->tytul) < 0) {
                tmp2 = tmp2->next;
            }
            tmp->next = tmp2->next;
            tmp2->next = tmp;
        }
        tmp = t;
    }
    return sorted;*/
}

void wyswietlKlientow(struct klienci *k) {
    printf("------------------------------------------------------------------------------------------------------------------------");
    printf("| nr |    imie    |    nazwisko     |                  adres                   |     telefon     | ilosc wypozyczonych |");
    printf("------------------------------------------------------------------------------------------------------------------------");
    while (k) {
        printf("| %-3i| ", k->index);
        printf("%-10s | ", k->imie);
        printf("%-15s | ", k->nazwisko);
        printf("%-40s | ", k->adres);
        printf("%-15s |", k->telefon);
        printf("\n");
        k = k->next;
    }
}

void wyswietlWedlugLiczby() {
    /* FILE *f = fopen("../pliki/klienci.bin", "rb");
     int max, i;
     if (f != NULL) {
         struct klienci *kl = wczytajKlientow(f, &max);
         kl = sortowaniePrzezWstawianieKL(kl);
         struct klienci *tmp = kl;
         if (tmp != NULL) {
             wyswietlKlientow(tmp);
             uwolnicKlientow(kl);
         } else printf("Brak kliazek\n");
     } else printf("Brak kliazek\n");
     fclose(f);*/
}

void wyswietlWedlugNazwiska() {
    FILE *f = fopen("../pliki/klienci.bin", "rb");
    int max, i;
    if (f != NULL) {
        struct klienci *kl = wczytajKlientow(f, &max);
        struct klienci *tmp = kl;
        if (tmp != NULL) {
            wyswietlKlientow(tmp);
            uwolnicKlientow(kl);
        } else printf("Brak klientow\n");
    } else printf("Brak klientow\n");
    fclose(f);
}

struct klienci *wstawPosortowanych(struct klienci *l, struct klienci *kl) {
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
    return klient;
}

void dodajKlienta(int n) {
    char *Pklient = "../pliki/klienci.bin";
    struct klienci *k1;
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
            k1->index = max + 1;
            k2 = wstawPosortowanych(k2, k1);
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
            k1->index = 1;
            fwrite(k1, sizeof(struct klienci), 1, f);
            free(k1);
        }
    } else {
        fclose(f);
        f = fopen(Pklient, "wb");
        k1->index = 1;
        fwrite(k1, sizeof(struct klienci), 1, f);
        free(k1);
    }
    fclose(f);
}

struct klienci *losowyKlient() {
    srand(time(0));
    struct klienci *klient = malloc(sizeof(struct klienci));
    char imie[][MAX] = {"Jan", "Andrzej", "Maria", "Bartosz", "Karol", "Karolina", "Anna", "Pawel", "Tomasz",
                        "Donald", "Katarzyna", "Gustaw", "Artut"};
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
    return klient;
}