/* Authors: Autumn Ara
 * Description: Declares geometric structures and functions.
 */

struct Mesh {
  int vertices_len;
  int faces_len;
  float (*vertices)[3];
  unsigned int (*faces)[3];
};

struct Mesh geodesic_icosahedron();
void delete_mesh(struct Mesh mesh);
