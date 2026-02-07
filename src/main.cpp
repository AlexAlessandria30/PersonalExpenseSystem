#include <iostream>
#include <string>
#include <iomanip>
#include <mysql/mysql.h>
#include <cstdlib>
#include <limits>

using namespace std;





// configurazione del database di mysql 
const char* DB_HOST = "localhost";
const char* DB_USER = "root";         
const char* DB_PASSWORD = "";          
const char* DB_NAME = "gestione_spese";
const int DB_PORT = 3306;


// configurazione connessione al DB

MYSQL* connectDatabase(){

    MYSQL* conn = mysql_init(NULL);

    if (conn == NULL)
    {
        cerr <<"Errore: mysql_init() fallito\n";
        return NULL;
    }

    if(mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASSWORD, DB_NAME, DB_PORT, NULL, 0) == NULL){

            cerr <<"Errore connessione database: " <<mysql_error(conn) <<"\n";
            mysql_close(conn);
            return NULL;
    }
    
    return conn;

}

// funzione per pulire il buffer di input 

void clearInputBuffer(){

    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

}


//Gestione Categorie

void gestioneCategorie(MYSQL* conn){

    cout << "\n ---  GESTIONE CATEGORIE ---\n";
    cout <<"1. Aggiungi nuova categoria\n";
    cout <<"2. Visualizza categorie esistenti\n";
    cout <<"scegli un'opzione";

    int scelta;
    cin >> scelta;
    clearInputBuffer();

    if(scelta == 1){

        //Gestione aggiunta categoria
        string nomeCategoria;
        cout <<"inserisci il nome della categoria: ";
        getline(cin, nomeCategoria);


        //verifica se il nome della categoria è vuoto 
        if (nomeCategoria.empty())
        {

          cout <<"Errore: il nome della categoria non può essere vuoto.\n";
          return;

        }


        // verifica se la categoria esiste
        string queryCheck = "SELECT id FROM categoria WHERE nome = '" + nomeCategoria + "'";
        if (mysql_query(conn, queryCheck.c_str())) {

            cerr << "Errore query: " << mysql_error(conn) << "\n";
            return;

        }


        MYSQL_RES* result = mysql_store_result(conn);
        if (mysql_num_rows(result) > 0) {

            cout << "Errore: la categoria esiste già.\n";
            mysql_free_result(result);
            return;

        }
        mysql_free_result(result);


        //inserimento  categoria
        string queryInsert = "INSERT INTO categoria (nome) VALUES ('" + nomeCategoria + "')";
        if (mysql_query(conn, queryInsert.c_str())) {

            cerr << "Errore inserimento: " << mysql_error(conn) << "\n";
            return;

        }

        cout << "Categoria inserita correttamente.\n";




        

    }else if( scelta == 2 ){

        // visualizzazione categoria
        string query = "SELECT id, nome FROM categoria ORDER BY nome";
        if (mysql_query(conn, query.c_str())) {

            cerr << "Errore query: " << mysql_error(conn) << "\n";
            return;

        }


         MYSQL_RES* result = mysql_store_result(conn);

        if (result == NULL) {

            cerr << "Errore: " << mysql_error(conn) << "\n";
            return;
        }

        cout << "\n--- CATEGORIE ESISTENTI ---\n";
        cout << left << setw(5) << "ID" << setw(30) << "Nome Categoria" << "\n";
        cout << string(35, '-') << "\n";

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {

            cout << left << setw(5) << row[0] << setw(30) << row[1] << "\n";

        }

        mysql_free_result(result);


    } else {

        cout << "Scelta non valida.\n";

    }

}

//Inserimento Spesa

void inserisciSpesa(MYSQL* conn){
    
    cout<< "\n --- Inserisci Spesa ---\n";

    string data, nomeCategoria, descrizione;
    double importo;


    //richiesta Data
    cout <<" Data (YYY-MM-DD): ";
    getline(cin, data);


    //richiesta importo
    cout <<"Importo: ";
    cin >> importo;
    clearInputBuffer();


    //Validazione importo
    if( importo <= 0){

        cout <<"Errore l'importo deve essere maggiore di zero ";

    }

    //richiesta categoria
    cout << "Nome categoria: ";
    getline(cin, nomeCategoria);


    // richiesta descrizione
    cout << "Descrizione (facoltativa): ";
    getline(cin, descrizione);

    // verifica se essite la categoria e prendi ID
    string queryCategoria = "SELECT id FROM categoria WHERE nome = '" + nomeCategoria + "'";

    if (mysql_query(conn, queryCategoria.c_str())) {

        cerr << "Errore query: " << mysql_error(conn) << "\n";
        return;

    }


    MYSQL_RES* result = mysql_store_result(conn);
      if (mysql_num_rows(result) == 0) {

        cout << "Errore: la categoria non esiste.\n";
        mysql_free_result(result);
        return;

    }


    MYSQL_ROW row = mysql_fetch_row(result);
    string categoriaId = row[0];
    mysql_free_result(result);


    // Inserisci la spesa
    char queryInsert[1024];

    snprintf(queryInsert, sizeof(queryInsert),

             "INSERT INTO spese (data, importo, categoria_id, descrizione) VALUES ('%s', %.2f, %s, '%s')",
             data.c_str(), importo, categoriaId.c_str(), descrizione.c_str());

    if (mysql_query(conn, queryInsert)) {

        cerr << "Errore inserimento: " << mysql_error(conn) << "\n";
        return;

    }

    cout << "Spesa inserita correttamente.\n";


}

// Definizione Budget Mensile

void definisciBudget(MYSQL* conn){

    cout <<"\n--- Definisci Budget Mensile ---\n";

    string mese, nomeCategoria;
    double importoBudget;

    //inserisciil mese 
    cout << "Mese (YYYY-MM-DD): ";
    getline(cin, mese);


    // inserisci la categoria
    cout << "Nome categoria: ";
    getline(cin, nomeCategoria);


    // inserisci importo budget
    cout << "Importo budget: ";
    cin >> importoBudget;
    clearInputBuffer();


    // validazione dell'importo budget
    if (importoBudget <= 0) {
    
        cout << "Errore: il budget deve essere maggiore di zero.\n";
        return;
    
    }


    // verifica se esiste la categoria e prendi ID
    string queryCategoria = "SELECT id FROM categoria WHERE nome = '" + nomeCategoria + "'";
    if (mysql_query(conn, queryCategoria.c_str())) {
    
        cerr << "Errore query: " << mysql_error(conn) << "\n";
        return;
    
    }


    MYSQL_RES* result = mysql_store_result(conn);
    if (mysql_num_rows(result) == 0) {
    
        cout << "Errore: la categoria non esiste.\n";
        mysql_free_result(result);
        return;
    
    }


    MYSQL_ROW row = mysql_fetch_row(result);
    string categoriaId = row[0];
    mysql_free_result(result);


    // verifica se esiste  un budget per questo mese e categoria
    char queryCheck[512];
    snprintf(queryCheck, sizeof(queryCheck),
    
        "SELECT id FROM budget WHERE mese = '%s' AND categoria_id = %s",
        mese.c_str(), categoriaId.c_str());

            if (mysql_query(conn, queryCheck)) {
            cerr << "Errore query: " << mysql_error(conn) << "\n";
            return;

            }

        result = mysql_store_result(conn);
        bool budgetEsiste = (mysql_num_rows(result) > 0);


        if (budgetEsiste) {
        row = mysql_fetch_row(result);
        string budgetId = row[0];
        mysql_free_result(result);

        // update del budget esistente
        char queryUpdate[512];
        snprintf(queryUpdate, sizeof(queryUpdate),
                 "UPDATE budget SET importo_budget = %.2f WHERE id = %s",
                 importoBudget, budgetId.c_str());

        if (mysql_query(conn, queryUpdate)) {
           
            cerr << "Errore aggiornamento: " << mysql_error(conn) << "\n";
            return;
       
        }


    } else {
        mysql_free_result(result);

        // inserimento di un nuovo budget 
        char queryInsert[512];
        snprintf(queryInsert, sizeof(queryInsert),
                 "INSERT INTO budget (mese, categoria_id, importo_budget) VALUES ('%s', %s, %.2f)",
                 mese.c_str(), categoriaId.c_str(), importoBudget);

        if (mysql_query(conn, queryInsert)) {
            cerr << "Errore inserimento: " << mysql_error(conn) << "\n";
            return;
        }
    }

    cout << "Budget mensile salvato correttamente.\n";

}

// Visualizza il report 

// report totsle spese per la categoria
void reportTotalePerCategoria(MYSQL* conn) {
    cout << "\n--- Totale spese per la categoria ---\n";

    string query =
        "SELECT c.nome, COALESCE(SUM(s.importo), 0) as totale "
        "FROM categoria c "
        "LEFT JOIN spese s ON c.id = s.categoria_id "
        "GROUP BY c.id, c.nome "
        "ORDER BY totale DESC";

    if (mysql_query(conn, query.c_str())) {
       
        cerr << "Errore query: " << mysql_error(conn) << "\n";
        return;
    
    }

    MYSQL_RES* result = mysql_store_result(conn);
    if (result == NULL) {
    
        cerr << "Errore: " << mysql_error(conn) << "\n";
        return;
    
    }

    cout << left << setw(30) << "Categoria" << right << setw(15) << "Totale Speso" << "\n";
    cout << string(45, '-') << "\n";

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
       
        cout << left << setw(30) << row[0]
             << right << setw(12) << fixed << setprecision(2) << atof(row[1]) << " EUR\n";
    
            }

    mysql_free_result(result);
}


// Report spese mensile vs Budget 
void reportSpeseVsBudget(MYSQL* conn) {
    cout << "\n--- Spese mensili VS Budget ---\n";

    string query =
        "SELECT "
        "    b.mese, "
        "    c.nome, "
        "    b.importo_budget, "
        "    COALESCE(SUM(s.importo), 0) as totale_speso "
        "FROM budget b "
        "JOIN categoria c ON b.categoria_id = c.id "
        "LEFT JOIN spese s ON c.id = s.categoria_id "
        "    AND DATE_FORMAT(s.data, '%Y-%m') = b.mese "
        "GROUP BY b.mese, c.nome, b.importo_budget "
        "ORDER BY b.mese DESC, c.nome";
    if (mysql_query(conn, query.c_str())) {
      
        cerr << "Errore query: " << mysql_error(conn) << "\n";
        return;
   
    }

    MYSQL_RES* result = mysql_store_result(conn);
    if (result == NULL) {
       
        cerr << "Errore: " << mysql_error(conn) << "\n";
        return;
    
    }

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        double budget = atof(row[2]);
        double speso = atof(row[3]);
        string stato;

        if (speso > budget) {
            stato = "SUPERAMENTO BUDGET";
        } else if (speso >= budget * 0.9) {
            stato = "ATTENZIONE (>90%)";
        } else {
            stato = "OK";
        }

        cout << "\nMese: " << row[0] << "\n";
        cout << "Categoria: " << row[1] << "\n";
        cout << "Budget: " << fixed << setprecision(2) << budget << " EUR\n";
        cout << "Speso: " << fixed << setprecision(2) << speso << " EUR\n";
        cout << "Stato: " << stato << "\n";
        cout << string(45, '-') << "\n";
    }

    mysql_free_result(result);

}


// Report completo delle spese 
void reportElencoSpese(MYSQL* conn) {
    cout << "\n--- Elenco completo spese ---\n";

    string query =
        "SELECT s.data, c.nome, s.importo, s.descrizione "
        "FROM spese s "
        "JOIN categoria c ON s.categoria_id = c.id "
        "ORDER BY s.data DESC";

    if (mysql_query(conn, query.c_str())) {
      
        cerr << "Errore query: " << mysql_error(conn) << "\n";
        return;
   
    }

    MYSQL_RES* result = mysql_store_result(conn);
    if (result == NULL) {
    
        cerr << "Errore: " << mysql_error(conn) << "\n";
        return;
    
    }

    cout << left << setw(12) << "Data"
         << setw(20) << "Categoria"
         << right << setw(12) << "Importo"
         << "  " << left << "Descrizione\n";
    cout << string(70, '-') << "\n";

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        cout << left << setw(12) << row[0]
             << setw(20) << row[1]
             << right << setw(10) << fixed << setprecision(2) << atof(row[2])
             << "  " << left << (row[3] ? row[3] : "") << "\n";
    }

    mysql_free_result(result);
}


// Menu report
void visualizzaReport(MYSQL* conn) {
    int scelta;

    do {
        cout << "\n--- MENU REPORT ---\n";
        cout << "1. Totale spese per categoria\n";
        cout << "2. Spese mensili vs budget\n";
        cout << "3. Elenco completo delle spese ordinate per data\n";
        cout << "4. Ritorna al menu principale\n";
        cout << "-------------------------\n";
        cout << "Inserisci la tua scelta: ";

        cin >> scelta;
        clearInputBuffer();

        switch (scelta) {
            case 1:
                reportTotalePerCategoria(conn);
                break;
            case 2:
                reportSpeseVsBudget(conn);
                break;
            case 3:
                reportElencoSpese(conn);
                break;
            case 4:
                cout << "Ritorno al menu principale...\n";
                break;
            default:
                cout << "Scelta non valida. Riprovare.\n";
        }

    } while (scelta != 4);

}


// creazione del Menu 
void menuPrincipale(MYSQL* conn) {
    int scelta;

    do {
        cout << "\n-------------------------\n";
        cout << " SISTEMA SPESE PERSONALI\n";
        cout << "-------------------------\n";
        cout << "1. Gestione Categorie\n";
        cout << "2. Inserisci Spesa\n";
        cout << "3. Definisci Budget Mensile\n";
        cout << "4. Visualizza Report\n";
        cout << "5. Esci\n";
        cout << "-------------------------\n";
        cout << "Inserisci la tua scelta: ";

        cin >> scelta;
        clearInputBuffer();

        switch (scelta) {
            case 1:
                gestioneCategorie(conn);
                break;
            case 2:
                inserisciSpesa(conn);
                break;
            case 3:
                definisciBudget(conn);
                break;
            case 4:
                visualizzaReport(conn);
                break;
            case 5:
                cout << "Uscita dal programma...\n";
                break;
            default:
                cout << "Scelta non valida. Riprovare.\n";
        }

    } while (scelta != 5);
}


// FUNZIONE MAIN

int main() {
    cout << "========================================\n";
    cout << "  Benvenuto nel sistema di gestione\n";
    cout << "    delle spese personali e Budget\n";
    cout << "========================================\n\n";

    // Connessione al database
    MYSQL* conn = connectDatabase();

    if (conn == NULL) {
        cerr << "Impossibile connettersi al database. Uscita.\n";
        return 1;
    }

    cout << "Connessione al database riuscita!\n";

    // Esecuzione menu principale
    menuPrincipale(conn);

    // Chiusura connessione
    mysql_close(conn);

    cout << "\nGrazie per aver utilizzato il sistema!\n";

    return 0;
}
