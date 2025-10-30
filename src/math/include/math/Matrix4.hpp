// Matrix4.hpp
// Description : interface pour une matrice 4x4 utilisée pour transformations homogènes.

/*
API attendu :
- Matrix4 identity(), translation(vec), scale(sx,sy,sz), rotation(axis,angle)
- operator*(const Matrix4&, const Matrix4&)
- applyToPoint(const Matrix4&, const Vec3&) -> Vec3 (applique la transformation homogène)
- applyToVector(const Matrix4&, const Vec3&) -> Vec3 (ignore la composante translation)
- inverse(), transpose()

Notes:
- Stockage row-major ou column-major doit être documenté clairement.
- Fournir méthode rapide pour calculer l'inverse d'une matrice de transformation orthogonale + scale.
*/
