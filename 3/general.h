typedef struct _Vertex{
  float x, y, z;
}Vertex;

Vertex vertex(float x, float y, float z){
  Vertex a;
  a.x=x; a.y=y; a.z=z;
  return a;
}

Vertex subVertex(Vertex a, Vertex b){
  return vertex(a.x-b.x, a.y-b.y, a.z-b.z);
}

float dotProduct(Vertex a, Vertex b){
  return a.x*b.x+a.y*b.y+a.z*b.z;
}

Vertex crossProduct(Vertex a, Vertex b){
  Vertex c;
  c.x=a.y*b.z-a.z*b.y;
  c.y=a.z*b.x-a.x*b.z;
  c.z=a.x*b.y-a.y*b.x;
  return c;
}

float modVertex(Vertex a){
  return sqrt(pow(a.x,2)+pow(a.y,2)+pow(a.z,2));
}

Vertex normalizeVertex(Vertex a){
  float mod=modVertex(a);
  return vertex(a.x/mod, a.y/mod, a.z/mod);
}

int multIntensity(int colour, float intensity){
  int r, g, b;
  r=colour%128; g=(colour>>8)%128; b=(colour>>16)%128;
  //printf("%d %d %d, %f\t", r, g, b, intensity);
  r=r*intensity*0.5+0.5*r;
  g=g*intensity*0.5+0.5*g;
  b=b*intensity*0.5+0.5*b;
  return r+(g<<8)+(b<<16);
}

int restoreIntensity(int colour){
  return colour;
/*  int r, g, b;
  r=colour%128; g=(colour>>8)%128; b=(colour>>16)%128;
  r=r?127:0; g=g?127:0; b=b?127:0;
  //printf("%d %d %d\n", r, g, b);
  return r+(g<<8)+(b<<16);
*/
}
