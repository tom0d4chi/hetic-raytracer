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

Notes :
- Le module `raymath` ne doit dépendre d'aucun autre module du projet.
- Favoriser des fonctions inline et éviter les allocations dans le hot-path.


# Contributing

This project follows the [Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/) specification for commit messages to ensure consistent and meaningful versioning.

A Git hook has been set up to prevent commits that don’t follow the convention. To use it, copy the `scripts/commit-msg` file to `.git/hooks`. After doing so, you should be met with an error message if your commit doesn't follow the convention.