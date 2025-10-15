## Exercice 4

L'algorithme récursif que nous avons utilisé pour la détermination du PGCD de deux nombre se base sur l'algorithmme d'Euclide.

Lorsque que l'on a deux entiers a et b avec a>b, alors PGCD(a,b) = PGCD(b,r) avec r le reste de la division euclidienne de a par b.

On peut alors voir un motif de récursion apparaître. On réduira la valeur des arguments au fur et à mesure des récursion en faisant des divisions euclidienne.

Le cas de base qui finira la récursion est le cas où le nombre le plus petit (b) est nul: dans ce cas, le pgcd est directement la deuxième valeur (a)

## Exercice 6

Le but est de déterminer la plus grande sous séquence d'un tableau d'entiers non ordonnés, ainsique 