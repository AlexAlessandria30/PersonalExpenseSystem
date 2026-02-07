# PersonalExpenseSystem

# Sistema di gestione delle spese personali e del budget

Autore: Alex Alessandria

## Requisiti per l'esecuzione

### compilatore necessario
- **g++** (GCC) versione 7.0 o superiore

### Librerie standard utilizzate 
```cpp
#include <iostream>
#include <string>
#include <iomanip>
#include <mysql/mysql.h>
#include <cstdlib>
#include <limits>
```

### Software Aggiuntivo
- MySQL Server 5.7 o superiore (oppure MariaDB 10.2+)
- MySQL Connector C/C++ (libmysqlclient-dev su Ubuntu/Debian, mysql-client su macOS)

## Istruzioni per Eseguire il Programma

### 1. Configurazione Database

Creare il database eseguendo lo script SQL:
```bash
mysql -u root -p < sql/database.sql
```

### 2. Configurazione Credenziali

Modificare il file `src/main.cpp` alle righe 16-17 con le proprie credenziali MySQL:
```cpp
const char* DB_USER = "root";
const char* DB_PASSWORD = "";
```

### 3. Compilazione

Compilare il programma con il seguente comando:
```bash
g++ -o gestione_spese src/main.cpp -lmysqlclient -std=c++11
```

Su macOS, se il comando sopra non funziona:
```bash
g++ -o gestione_spese src/main.cpp $(mysql_config --cflags --libs) -std=c++11
```

### 4. Esecuzione

Eseguire il programma:
```bash
./gestione_spese
```# PersonalExpenseSystem
# PersonalExpenseSystem
