#define _CRT_SECURE_NO_WARNINGS
/*
Quesito 2
Un ente di formazione eroga 10 corsi diversi ciascuno dei quali e identificato
da un codice numerico compreso fra 0 e 9.
In un file di testo sono memorizzati uno per riga gli iscritti ai corsi.
Per ogni iscritto nel file (vedi esempio) sono memorizzati:
•   cognome (una stringa di al massimo 20 caratteri utili senza bianchi)
•   nome (una stringa di al massimo 20 caratteri utili senza bianchi)
•   eta (un intero)
•   codiceCorso (codice numerico)
Una stessa persona non può essere iscritta due volte allo stesso corso ma può essere iscritta a corsi diversi.
Progettare la struttura dati adatta a mantenere in memoria centrale le informazioni
utilizzando strutture e puntatori per le liste;
Scrivere un opportuno main in cui sia previsto un menu di scelta delle operazioni richieste.
Tutti i valori necessari al funzionamento devono essere passati utilizzando parametri,
non è permesso l’uso di variabili globali;
1.  Implementare una funzione che esegua il caricamento in un vettore di liste (una lista per ogni corso) il contenuto del file verificando che non ci siano duplicati sullo stesso corso
2.  Implementare una funzione che restituisce in un vettore di float l’età media degli iscritti a ciascun corso
3.  Implementare una funzione che dato un cognome e nome restituisca la lista dei corsi a cui questa persona è iscritta
4.  Implementare una funzione che stampi tutti gli iscritti ad un corso scelto dall’utente
Esempio dei dati nel file di ingresso
Giuseppe
Verdi
23
1
Alfio
Pappalardo
22
3
*/
#include <stdio.h>
#include <malloc.h>
#include <string.h>
typedef struct a_nodo_corso {
    int codice;
    struct a_nodo_corso* successivo;
} t_nodo_corso, * t_lista_corso;

t_lista_corso corso_lista_aggiungi(t_lista_corso lista_ingresso, int codice_corso) {
    t_nodo_corso* aux = (t_nodo_corso*)malloc(sizeof(t_nodo_corso));
    if (aux == NULL)
        return lista_ingresso;
    aux->codice = codice_corso;
    aux->successivo = lista_ingresso;
    return aux;
}
void corso_stampa(t_lista_corso l) {
    while (l != NULL) {
        printf("corso %d\n", l->codice);
        l = l->successivo;
    }
}
#define CAMPO_MASSIMA_LUNGHEZZA 21
typedef struct s_nodo_persona {
    // Sezione Info
    char cognome[CAMPO_MASSIMA_LUNGHEZZA];
    char nome[CAMPO_MASSIMA_LUNGHEZZA];
    unsigned int eta;
    // Colleamento al prossimo
    struct s_nodo_persona* successivo;
} t_nodo_persona, t_persona;
int persona_confronta(const t_persona* a, const t_persona* b) { //const punt passo perchè pesa meno ed è immodificabile (visita)
    int risultato = strcmp(a->cognome, b->cognome);
    if (risultato != 0)
        return risultato;
    return strcmp(a->nome, b->nome);
}
typedef t_nodo_persona* t_lista_persona;
// Inserire le funzioni primitive
t_lista_persona persona_lista_aggiungi(t_lista_persona lista_ingresso, t_persona nuova_persona) {
    t_nodo_persona* aux = (t_nodo_persona*)malloc(sizeof(t_nodo_persona));
    if (aux == NULL)
        return lista_ingresso;
    *aux = nuova_persona; //aux è definito come aux*, quindi *aux deferenzia -> radice
    aux->successivo = lista_ingresso;
    return aux;
}
t_persona* persona_lista_cerca(t_lista_persona lista_ingresso, t_persona persona) {
    while (lista_ingresso != NULL) {
        if (persona_confronta(lista_ingresso, &persona) == 0) { //passo due puntatori alla funzione confronta
            return lista_ingresso;
        }
        lista_ingresso = lista_ingresso->successivo;
    }
    return NULL;
}
void persona_lista_stampa(t_lista_persona l) {
    while (l != NULL) {
        printf("%s\n%s\n%d\n\n", l->nome, l->cognome, l->eta);
        l = l->successivo;
    }
}
float persona_lista_eta_media(t_lista_persona l) {
    int somma_eta = 0;
    int numero_elementi = 0;
    while (l != NULL) {
        somma_eta += l->eta;
        ++numero_elementi;
        l = l->successivo;
    }
    if (numero_elementi == 0)
        return 0;
    return ((float)somma_eta) / numero_elementi;
}
#define CORSI_NUMERO_MASSIMO 10
typedef t_lista_persona t_corsi[CORSI_NUMERO_MASSIMO]; //t_corsi è un array di C_N_M posti di tipo t_lista_persona.

void carica(t_corsi archivio, FILE* f) { //
    t_persona p;
    int codice_corso;
    if (f == NULL)
        return;
    while (fscanf(f, "%s%s%d%d", p.nome, p.cognome, &p.eta, &codice_corso) == 4) {
        if (persona_lista_cerca(archivio[codice_corso], p) == NULL)
            archivio[codice_corso] = persona_lista_aggiungi(archivio[codice_corso], p);
    }
}
void calcola_eta_media(t_corsi archivio, float eta[]) {
    for (int i = 0; i < CORSI_NUMERO_MASSIMO; ++i) {
        eta[i] = persona_lista_eta_media(archivio[i]);
    }
}
t_lista_corso calcola_elenco_corsi(t_corsi archivio, t_persona p) {
    t_lista_corso lista_corsi = NULL;
    for (int i = 0; i < CORSI_NUMERO_MASSIMO; ++i) {
        if (persona_lista_cerca(archivio[i], p) != NULL)
            lista_corsi = corso_lista_aggiungi(lista_corsi, i);
    }
    return lista_corsi;
}
int main(void) {
    char buffer[128];
    char selezione[128];
    FILE* file_ingresso = NULL;
    t_corsi elenco_corsi;
    float vettore_eta[CORSI_NUMERO_MASSIMO] = { 0.0 };
    t_persona persona;
    t_lista_corso elenco_iscritti_corsi = NULL;
    for (int i = 0; i < CORSI_NUMERO_MASSIMO; ++i)
        elenco_corsi[i] = NULL;
    do {
        /*printf("apri: caricamento in un vettore di liste\n");
        printf("eta: età media degli iscritti\n");
        printf("corsi: lista dei corsi ");
        printf("iscritti: iscritti ad un corso \n\n");
        printf("\ncomando: ");*/
        fgets(selezione, 128, stdin);
        if (!strncmp(selezione, "apri", 4)) {
            printf("Inserisci il nome del file: ");
            scanf("%s", buffer);
            file_ingresso = fopen(buffer, "r");
            if (file_ingresso == NULL) {
                printf("impossibile aprire %s\n", buffer);
                break;
            }
            carica(elenco_corsi, file_ingresso);
            fclose(file_ingresso);
            for (int i = 0; i < CORSI_NUMERO_MASSIMO; ++i) {
                printf("corso codice %d\n", i);
                persona_lista_stampa(elenco_corsi[i]);
            }
        }
        else if (!strncmp(selezione, "eta", 3)) {
            calcola_eta_media(elenco_corsi, vettore_eta);
            for (int i = 0; i < CORSI_NUMERO_MASSIMO; ++i)
                printf("codice corso %d -> eta' media %.2f\n", i, vettore_eta[i]);
        }
        else if (!strncmp(selezione, "corsi", 4)) {
            printf("Inserire il cognome");
            scanf("%s", persona.cognome);
            printf("Inserire il nome");
            scanf("%s", persona.nome);
            elenco_iscritti_corsi = calcola_elenco_corsi(elenco_corsi, persona);
            corso_stampa(elenco_iscritti_corsi);
            // cancellare la lista dei corsiper svuotarla
        }
        else if (!strncmp(selezione, "iscritti", 4)) {
            for (int i = 0; i < CORSI_NUMERO_MASSIMO; ++i) {
                printf("corso codice %d\n", i);
                persona_lista_stampa(elenco_corsi[i]);
            }
        }
    } while (strncmp(selezione, "fine", 4) != 0);
}