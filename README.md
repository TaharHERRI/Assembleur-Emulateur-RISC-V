# Assembleur et Émulateur RISC-V

Ce projet propose une implémentation en C d’un **assembleur** et d’un **émulateur** pour un sous-ensemble du jeu d’instructions **RISC-V**. Il permet :

- d'assembler des programmes écrits en assembleur RISC-V en fichiers binaires ou hexadécimaux,
- d’exécuter ces programmes dans un environnement émulé.

---

## Auteurs

- Herri Tahar  
- Le Berre Nathan

---

## Prérequis

Assurez-vous d’avoir les éléments suivants installés sur votre machine linux :

### Outils système
- `riscv64-elf-gcc` & `clang` & `clang-14` – Compilateurs C
- `llvm-14` & `llvm-14-tools` - Pour objcopy
- `make` – Outil d’automatisation de la compilation
- `python3` – Pour les tests
- `pytest` – pour exécuter `test.py`


---

## Installation et compilation

Clonez ou téléchargez le projet, puis compilez les deux composants principaux :

```bash
make
```

Cela génère deux exécutables à la racine du projet :
- `riscv-assembler` : assembleur RISC-V
- `riscv-emulator` : émulateur RISC-V

---

## Structure du projet

```
Assembleur-Emulateur-RISC-V/
├── assembler/           # Code source de l'assembleur
├── emulator/            # Code source de l'émulateur
├── tests/               # Dossier contenant les tests
├── test.py              # Script Python de test
├── Makefile             # Script de compilation
└── README.md            # Ce fichier
```

---

## Utilisation

### 1. Écrire un programme assembleur

Créez un fichier `exemple.s` contenant du code RISC-V, par exemple :

```asm
li x1, 5
li x2, 10
add x3, x1, x2
```

### 2. Assembler le programme

Utilisez l'assembleur pour traduire ce code :

```bash
./riscv-assembler exemple.s
```

Ce programme produit un ou plusieurs des fichiers suivants :
- `exemple.hex` : représentation hexadécimale pour l’émulateur
- `exemple.bin` : version binaire
- `exemple.state` : état initial du système


### 3. Exécuter le programme avec l’émulateur

Une fois assemblé, le fichier `.hex` peut être exécuté avec :

```bash
./riscv-emulator exemple.hex
```

L’émulateur charge le fichier, exécute les instructions et peut :
- afficher l’état des registres ou de la mémoire,
- générer un fichier `.state` avec l’état final du système.

---

## Tests

Le projet contient un script de test automatisé :

```bash
make test
```

Cela exécute `test.py` avec `pytest` pour tester l’assembleur et l’émulateur avec différents fichiers dans le dossier `tests/`.

---

## Nettoyage

Supprimer les fichiers générés :

```bash
make clean        # Nettoyage des fichiers compilés et temporaires de test
make cleanall     # Nettoyage complet (inclut les exécutables et les caches Python)
```

---

## Création d’une archive

Pour créer une archive `.tgz` du projet :

```bash
make tar
```

Cela crée une archive `../HerriTahar_LeBerreNathan.tgz` contenant tout le projet, après nettoyage.

---

## Remarques

- Le projet supporte un sous-ensemble des instructions RISC-V, adapté à un usage éducatif.

Voici la **liste des instructions réellement prises en charge** (extraites des commentaires à la fin de chaque ligne) :

### Instructions supportées
- `bge`
- `mv`
- `beq`
- `sd`
- `j`
- `li`
- `addi`
- `ld`
- `add`
- `sub`
- `blt`
- `bne`
- `jal`


---
