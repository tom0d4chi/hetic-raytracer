But du dossier `math`

Ce dossier contient des types et utilitaires géométriques purs et indépendants :
- Vec3 (vecteurs 3D), Ray, Matrix4, Transform helpers.
- Constantes numériques (PI, EPSILON) et alias de type (Real).

Conventions recommandées :
- Système de coordonnées right-handed.
- Unité arbitraire mais cohérente dans toute la scène.
- Choix de précision : typedef/alias `Real` configurable (float ou double).

Organisation :
- include/math/ : headers publics (API). Le projet est conçu pour pouvoir être header-only.
- doc/ : notes mathématiques, recommandations numériques.

Notes :
- Le module `math` ne doit dépendre d'aucun autre module du projet.
- Favoriser des fonctions inline et éviter les allocations dans le hot-path.
