# Half-Life Parpaing 1.51: Code Source


## Statut du projet

Personne n'a touché à rien depuis 2003 (ou 2005 pour le code pour GCC 2.95).

Donc aucune chance pour que ça compile tel quel de nos jours.

Peut-être qu'une copie du repo SVN de l'époque existe quelque part, je
ferai un autre repo le cas échéant si ça surgit de quelque part.

Si quelqu'un veut se lancer dans l'aventure de la mise à jour du code et
des libs, je zyeuterai les PR.

Le code source avait été originellement distribué sous GPL, insistant sur
le fait que cela ne concernait que notre delta par rapport au SDK.

Pour éviter toute confusion, ici tout est sujet à la licence du SDK :
dans les faits (dérivés permis, ...) ça ne change pas grand-chose.


## Auteurs originels du code d'Half-Life Parpaing

- Bob le Pointu
- douanier007 / TroyMcLure
- madfab
- BBk
- MiK

Si l'un d'entre eux se manifeste (ou un autre oublié) je l'ajouterai dans les collaborateurs.

Pour les autres, gardez à l'esprit que tout le monde était jeune et que
le standard de qualité était différent par rapport à aujourd'hui, donc
ne les jugez pas aujourd'hui d'après ce vieux code.


## Readme original ("Hep les codeurs, lisez donc ceci.txt")

Pour pouvoir compiler le code, vous aurez besoin :

- D'un compilateur compatible (MSVC++ 6 et ultérieur)
- Du SDK 2.2 d'Half-Life
- Des librairies et headers MySql et Fmod (incluses dans libhfiles.zip)

Stouw.

EDIT du 06/09/2005 concernant la compilation sous Linux :

Fabou, gloire à lui, a réussi à compiler la dll serveur sous Linux.

Il nous a même fourni les fichiers modifiés, qui sont placés dans le dossier dlls_linux.

Pour compiler vous-mêmes, vous aurez besoin de gcc-2.95.
