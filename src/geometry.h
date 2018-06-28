struct Mesh {
  int vertices_len;
  int faces_len;
  float (*vertices)[3];
  unsigned int (*faces)[3];
};

struct Mesh regular_icosahedron();

void delete_mesh(struct Mesh mesh);
