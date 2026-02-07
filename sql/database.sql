-- Sistema di gestione delle spese personali e del budget 

-- creazione del DB
DROP DATABASE IF EXISTS gestione_spese;
CREATE DATABASE gestione_spese;
USE gestione_spese;

-- Tabella: categorie 

CREATE TABLE categoria (
    id INT AUTO_INCREMENT, 
    nome VARCHAR(100) NOT NULL,

    PRIMARY KEY (id),
    UNIQUE(nome),
    CHECK (nome <> "")
)ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- Tabella spese

CREATE TABLE spese(
    id INT AUTO_INCREMENT,
    data DATE NOT NULL,
    importo DECIMAL(10,2) NOT NULL,
    categoria_id INT NOT NULL,
    descrizione VARCHAR(255),

    PRIMARY KEY(id),
    FOREIGN KEY (categoria_id) REFERENCES categoria(id)
        ON DELETE RESTRICT
        ON UPDATE CASCADE,
    CHECK(importo >= 0)
)ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- Tabella budget

CREATE TABLE budget(
    id INT AUTO_INCREMENT,
    mese VARCHAR(10) NOT NULL,
    categoria_id INT NOT NULL,
    importo_budget DECIMAL(10,2) NOT NULL,

    PRIMARY KEY(id),
    FOREIGN KEY (categoria_id) REFERENCES categoria(id)
        ON DELETE RESTRICT
        ON UPDATE CASCADE,
        UNIQUE(mese, categoria_id),
        CHECK(importo_budget > 0),
        CHECK(mese REGEXP'^[0-9]{4}-[0-9]{2}$') -- imponiamo anno mese
)ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;



-- Inserimento valori
INSERT INTO categorie (nome) VALUES
    ('Alimentari'),
    ('Trasporti'),
    ('Intrattenimento'),
    ('Salute'),
    ('Abbigliamento');


INSERT INTO spese (data, importo, categoria_id, descrizione) VALUES
   
    ('2026-01-05', 45.80, 1, 'Spesa settimanale'),
    ('2026-01-10', 25.00, 1, 'Pranzo ristorante'),
    ('2026-01-15', 82.50, 1, 'Supermercato'),
    ('2026-01-20', 15.30, 1, 'Colazione bar'),
    ('2026-01-25', 98.40, 1, 'Spesa mensile'),

   
    ('2026-01-03', 35.00, 2, 'Abbonamento autobus'),
    ('2026-01-12', 50.00, 2, 'Benzina'),
    ('2026-01-22', 25.00, 2, 'Taxi'),

   
    ('2026-01-08', 15.00, 3, 'Cinema'),
    ('2026-01-18', 40.00, 3, 'Cena con amici'),
    ('2026-01-28', 12.99, 3, 'Abbonamento streaming'),

   
    ('2026-01-14', 28.50, 4, 'Farmacia'),
    ('2026-01-21', 80.00, 4, 'Visita medica'),

  
    ('2026-02-01', 120.00, 5, 'Scarpe nuove'),
    ('2026-02-03', 55.20, 1, 'Spesa settimanale'),
    ('2026-02-05', 30.00, 2, 'Benzina'),
    ('2026-02-08', 20.00, 3, 'Teatro'),
    ('2026-02-10', 45.00, 1, 'Ristorante');


INSERT INTO budget (mese, categoria_id, importo_budget) VALUES
   
    ('2026-01', 1, 300.00),  
    ('2026-01', 2, 150.00),  
    ('2026-01', 3, 100.00),  
    ('2026-01', 4, 150.00),  
    ('2026-01', 5, 200.00),  


    ('2026-02', 1, 350.00),  
    ('2026-02', 2, 120.00),  
    ('2026-02', 3, 80.00),   
    ('2026-02', 4, 100.00),  
    ('2026-02', 5, 150.00);

-- Query di verifica 

SELECT * FROM categoria;

-- Verifica spese inserite
SELECT s.id, s.data, c.nome AS categoria, s.importo, s.descrizione
FROM spese s
JOIN categorie c ON s.categoria_id = c.id
ORDER BY s.data DESC;

-- Verifica budget inseriti
SELECT b.mese, c.nome AS categoria, b.importo_budget
FROM budget b
JOIN categorie c ON b.categoria_id = c.id
ORDER BY b.mese, c.nome;