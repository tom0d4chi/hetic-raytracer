// Transform.hpp
// Helpers pour appliquer des transformations à des points, vecteurs, et normales.

/*
Fonctionnalités attendues :
- struct Transform { Matrix4 m; Matrix4 inv; } avec méthodes applyToPoint, applyToVector, applyToNormal
- Précalcul de l'inverse lors de la construction du Transform
- Notes sur l'utilisation correcte des normales (utiliser inv-transpose pour transformer les normales)
*/
