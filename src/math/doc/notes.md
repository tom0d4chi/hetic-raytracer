Notes mathématiques et recommandations

- Résolution de l'équation quadratique : utiliser une forme stable pour éviter les pertes de précision
  (par exemple calculer q = -0.5*(b + sign(b)*sqrt(discriminant)) puis t0 = q/a, t1 = c/q)

- EPSILON recommandé : dépend du choix Real (float ~1e-6..1e-4; double ~1e-12..1e-8).

- Normales : si une transformation non-uniforme est appliquée, transformer la normale par la matrice inverse-transposée.

- Conventions : right-handed coordinate system, directions unitaires pour `Ray::direction`.
