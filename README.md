# Raytracer sample

Refer to the course documentation for setting up this project [here](https://app.gitbook.com/o/b3dqd7QQRvmcMESGx996/s/TRzi8I72JI2Hspznwkxl/intro-to-c++/intro)

- Vec3 (vecteurs 3D), Ray, Matrix4, Transform helpers.
- Constantes numériques (PI, EPSILON) et alias de type (Real).

Conventions recommandées :
- Système de coordonnées right-handed.
- Unité arbitraire mais cohérente dans toute la scène.
- Choix de précision : typedef/alias `Real` configurable (float ou double).

Organisation :
- raymath/ : helpers vectoriels & géométriques (headers accessibles globalement).
- doc/ : notes mathématiques, recommandations numériques.

Notes :
- Le module `raymath` ne doit dépendre d'aucun autre module du projet.
- Favoriser des fonctions inline et éviter les allocations dans le hot-path.
