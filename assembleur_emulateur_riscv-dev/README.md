# Projet RISC-V CS351

Auteurs : Herri Tahar et Nathan Le Berre

## Rendu 1

* Cochez (en remplaçant `[ ]` par `[x]`) si vous avez :
  - [x] Vérifié que `make test` se plaint sur le nombre d'instructions et pas
      sur l'existence des fichiers de sortie.
  - [x] Vu sur Chamilo que les soumissions se font avec `make tar`.


## Rendu 2

(Une dizaine de lignes devrait suffire à chaque fois)

* Comment avez-vous choisi de programmer l'analyse de texte (dont la lecture
des opérandes entières) ? Comment identifiez-vous les noms de registres, des noms "jolis", des entiers ?

L'analyse du texte se fait par un ensemble de fonction de lecture, considérant toutes la ligne actuellement traitée et l'index du dernier caractère
traité par ces fonctions.

Pour éxemple : La première fonction de ce type éxécutée est pour lire le mot d'instruction, cette fonction met ensuite a jour l'index de lecture pour qu'il pointe vers le premier caractere du premier argument.

La chaine de caractere représentant l'instruction est traitée comme un int (max 4 octets) puis passée dans un hash (un simple mask and puis un modulo) qui indexera un tableau
contenant des "templates" de notre structure d'instruction.

Cette template contient les opcode et func de l'instruction, ainsi qu'un tableau de type d'arguments qui sera utilisée pour rapidement traiter les arguments.

La lecture de registre en écriture mnemonic utilise de la même manière une fonction de hash, générée par gperf cette fois ci. Cette fonction mériterais d'être retravaillée.
Pour les registre en écriture directe (x..) et les valeurs immédiates, on utilise notre fonction de lecture d'entiers.

Cette fonction de lecture d'entiers ajoute les valeurs caractère par caractère en mutlipliant par 10 entre chaque. (avec une exception pour le potentiel premier caractère '-').

* Avez-vous vu des motifs récurrents émerger ? Avez-vous "factorisé" ces motifs
pour éviter de les répéter ? Si non, serait-ce difficile ?

Le traitement d'instruction a été écrit de manière générale factorisant toutes les étapes de lecture du mot d'instruction et lecture des arguments ensemble.

Nous avons donc fait un fichier assembler-write.c dans lequel chaques elements/motifs de chaque format d'instruction ont été traités.
Nous avons bien vu que certains motifs tels que (rs2, rs1, rd, funct3 et opcode) revenaient souvent et gardaient la même position dans l'instruction malgré le fait que ce soit dans des formats différents.
Cela nous a donc permis de gagner du temps tant à la lecture qu'à l'écriture.

En revanche, seules les valeurs immédiates étaient pénibles lors de l'écriture, car il fallait les traiter pour chaque format qui contenait des valeurs immédiates.
La détermination de ces formats d'instruction s'est faite dans la partie lecture (assembler-read.c).

Pour l'écriture comme cela à été évoqué précédemment, nous avons traité chacun des motifs existant individuellement dans des fonctions.
Ces fonctions consistaient à shifter à gauche ou à droite chacun des motifs afin qu'il soit au bon endroit dans des mots de 32 bits.
Ces motifs avant modification étaient récupérés grâce à la structure instr définie dans le fichier suivant (incude/def.h/).

Nous avons aussi choisi d'utiliser des masques, mais pourquoi ?
Même si en effet à première vue cela paraît inutile, le fait d'utiliser des masques nous a permis de nous assurer que chaque motif allait à chaque fois être au bon endroit et n'allait pas empiéter sur le territoire des autres.
De plus, ces masques nous permettaient de compter le nombre de bits de décalages. Ce qui était bien utile notamment pour la valeur immédiate du format J.

Comme la lecture s'est faite ligne par ligne grâce à la fonction getline, l'éctriture s'est également faite ligne par ligne. Nous avons donc créer une fonction write_line qui va chercher le format de l'instruction (toujours récupéré dans la structure def.h) puis qui écrit ce format grâce à un switch, et qui modifie tous les motifs en appelant les fonctions citées précédemment.
Afin d'assembler les motifs modifiés pour finalement assembler l'instruction, tout était prêt il suffisait de faire des OR bit à bit.

* Comment avez-vous procédé pour écrire les tests ? Étes-vous confiant·e·s que
toutes les instructions gérées et tous les types d'arguments sont couverts ?

Pour les tests, nous avons légèrement modifier dans un premier temps le fichier complex-parse.s pour tester l'assemblage de certaines instructions ayant les formats (I, J, B, S) avec des valeur immédiates négatives.

Ensuite une fois que tous les formats avaient été testés dans complex-parse.s et add-simple.s nous avons voulu faire deux autres types de tests.

Nous voulions écrire un fichier qui allait pouvoir nous servir dans la partie émulation et qui contenait tous les types d'instruction. Nous avons donc écrits le fichier somme_val.s qui crée un tableau d'int et qui fait la somme de ses valeurs.
Même si nous n'avons pas pu tester si ce programme fonctionne (nous ne pouvons pas créer de main puisque nous n'avons pas de label, donc la compilation ne se fait pas),
Nous avons pu vérifier l'assemblage de chaque instruction grâce au simulateur riscV en ligne Venus.

Enfin, nous avons créé des tests qui sont censés échouer juste pour tester les fonctionnalités de gestion d'erreur de notre code. Ces tests nommés (invalidX.s.fail) contiennent des instructions dont la syntaxe n'est pas bonne ou du moins pas prise en charge par notre code. 
Comme ces tests ne sont pas des fichiers .s classiques ils ne sont pas pris en compte par le make test, il faut donc les lancer à la main.

* Quelle a été votre expérience avec l'utilisation et la compréhension de la
documentation fournie sur l'encodage des instructions RISC-V ?

La documentation était très claire et très lisible. Toutes les informations requises étaient aisément accessibles. Nous n'avons pas eu de problème ni à l'utilisation ni à la compréhension.
Nous n'avons pas particulièrement eu besoin de chercher des informations supplémentaires, tout ce qui a été fourni suffisait largement.

* Cochez (en remplaçant `[ ]` par `[x]`) si vous avez :
  - [x] Implémenté la traduction pour toutes les instructions de la documentation
  - [x] Pris en compte les cas particuliers comme les valeurs immédiates négatives et le bit manquant dans l'encodage de `jal`
  - [x] Écrit du code de gestion d'erreur pour que le programme ait une réaction propre si le code assembleur est invalide _(pas demandé par le sujet)_


## Rendu 3

Questions à remplir _avant_ de programmer l'émulateur (10 lignes sont conseillées à chaque fois pour bien y réfléchir) :

* Listez tous les éléments matériels auxquels vous pouvez penser dont l'émulateur doit reproduire le comportement, et déduisez-en une liste de toutes les tâches individuelles de l'émulateur.

Une mémoire (32 registres de 8 octets) qui va contenir les instructions (sur 4 octets), la pile, le pointeur de pile et le PC. Une fonction pour chaque instruction.
On émule le processeur qui se balade la mémoire pour lire puis ecrire dans les registres.

on devrait :
- Lire le PC de l'instruction courante (PC init à 0)
- Decodage de l'instruction (faire un peu l'inverse de l'assemblage)
- Executer l'instruction, modifier la valeur des registres de destination, du PC, du SP. (SP init à 16384)
- Faire ça en boucle (modifier le PC incrémenter ou pas si y a un saut dans le programme)
- Une instruction nulle stop le programme
- Comme la mémoire est innitialisé avec des 0, lorsque qu'on sort du bloc mémoire qui contient les instructions ça s'arrette.
- A la fin écrire le contenu des registres modifiés dans fichier .state 

* Quelle fonction de la bibliothèque standard pouvez-vous utiliser pour lire les valeurs listées dans le fichier `.hex` sans vous casser la tête ? (Indice : ces valeurs ont été écrites avec `fprintf()`.)

On pourait commancer avec fscanf().

* Décrivez comment vous allez répartir les tâches de l'émulateur en différents fichiers, ou ne pas les répartir et tout faire dans le même fichier. Expliquez les avantages de votre choix.

On devrait faire une partie décodage qui va faire l'inverse de l'assemblage. Un hash pour gerer les opcode et selectionner la bonne instruction.
Une partie execution qui va contenir une fonction pour chaque instruction et une fonction pour le cycle d'instruction du programme.

Questions à remplir _après_ avoir programmé l'émulateur :

* Aviez-vous réussi à listé toutes les tâches dans la première question ? Rétrospectivement, y a-t-il des tâches dont vous aviez sous-estimé ou sur-estimé la complexité ?

[COMPLÉTER ICI]

* Avez-vous compris le fonctionnement de chaque instruction à partir de la
documentation fournie ? Si non, quels sont les points obscurs ?

[COMPLÉTER ICI]

* Quels exemples de programmes avez-vous choisi pour tester le calcul ? Les
comparaisons et sauts ? La mémoire ?

[COMPLÉTER ICI]

* Reste-t-il des bugs que vous avez découverts et pas corrigés ?

[COMPLÉTER ICI]

* D'autres remarques sur votre programme ?

[COMPLÉTER ICI]

* Cochez (en remplaçant `[ ]` par `[x]`) si vous avez :**
  - [ ] Implémenté l'émulation de toutes les instructions gérées par le rendu 2.
  - [ ] Implémenté l'émulation de toutes les instructions.
  - [ ] Tous vos tests qui passent.
  - [ ] Vérifié que vous tests couvrent toutes les instructions émulées.
  - [ ] Testé les cas particuliers : valeurs négatives, overflows...
  - [ ] Testé les cas d'erreur : division par zéro, sauts invalides... _(pas demandé par le sujet)_
  - [ ] Un port fonctionnel de DOOM pour votre émulateur.

* Des retours sur le projet en général ?

[COMPLÉTER ICI]
