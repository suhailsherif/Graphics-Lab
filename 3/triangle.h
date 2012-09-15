typedef struct _Triangle{
  Vertex v1, v2, v3;
  Vertex normal;
  int colour;
}Triangle;

Triangle triangle(Vertex v1, Vertex v2, Vertex v3, int colour){
  Triangle a;
  a.v1=v1; a.v2=v2; a.v3=v3; a.colour=colour;
  return a;
}

typedef struct _TriangleList{
  Triangle t;
  struct _TriangleList *next;
}TriangleList;

TriangleList *globalTriangles=NULL;
TriangleList *transformedTriangles=NULL;

void freeTriangleList(TriangleList *a){
  if(!a)	return;
  freeTriangleList(a->next);
  free(a);
}

void clearAllTriangles(){
  freeTriangleList(globalTriangles);
  globalTriangles=NULL;
  freeTriangleList(transformedTriangles);
  transformedTriangles=NULL;
}

Triangle globalTriangle(Triangle a){
  float light;
  Triangle t_a = triangle(globalVertex(a.v1), globalVertex(a.v2), globalVertex(a.v3), a.colour);
  t_a.normal = crossProduct(subVertex(t_a.v2, t_a.v1), subVertex(t_a.v3, t_a.v2));
  t_a.normal = normalizeVertex(t_a.normal);
  light=dotProduct(normalizeVertex(t_a.normal), normalizeVertex(vertex(10, 20, 15)));
  t_a.colour=multIntensity(a.colour, light);

  //printf("IColour Fcolour: %x %x\n", a.colour, t_a.colour);
  ////printf("Global: %f %f %f,  %f %f %f, %f %f %f\t", t_a.v1.x, t_a.v1.y, t_a.v1.z, t_a.v2.x, t_a.v2.y, t_a.v2.z, t_a.v3.x, t_a.v3.y, t_a.v3.z);
  ////printf("Normal: %f %f %f\n", t_a.normal.x, t_a.normal.y, t_a.normal.z);
  return t_a;
}

Triangle eyeTriangle(Triangle a){
  Triangle t_a = triangle(eyeVertex(a.v1), eyeVertex(a.v2), eyeVertex(a.v3), a.colour);
  t_a.normal = crossProduct(subVertex(t_a.v2, t_a.v1), subVertex(t_a.v3, t_a.v2));
  t_a.normal = normalizeVertex(t_a.normal);
  ////printf("Eye: %f %f %f,  %f %f %f, %f %f %f\t", t_a.v1.x, t_a.v1.y, t_a.v1.z, t_a.v2.x, t_a.v2.y, t_a.v2.z, t_a.v3.x, t_a.v3.y, t_a.v3.z);
  ////printf("Normal: %f %f %f\n", t_a.normal.x, t_a.normal.y, t_a.normal.z);
  return t_a;
}

void addTriangle(Triangle a){
  Triangle t_a = globalTriangle(a);
  TriangleList *temp=(TriangleList *)malloc(sizeof(TriangleList)), *curr;
  temp->t=t_a;
  temp->next=NULL;
  if(!globalTriangles)	globalTriangles=temp;
  else{
    curr=globalTriangles;
    while(curr->next){
      curr=curr->next;
    }
    curr->next=temp;
  }
}

void addTransformed(Triangle a){
  TriangleList *temp=(TriangleList *)malloc(sizeof(TriangleList)), *curr;
  temp->t=a;
  temp->next=NULL;
  if(!transformedTriangles)	transformedTriangles=temp;
  else{
    curr=transformedTriangles;
    while(curr->next){
      curr=curr->next;
    }
    curr->next=temp;
  }
}

void transformTriangles(){
  TriangleList *currTriangle;
  Triangle t;
  for(currTriangle=globalTriangles; currTriangle; currTriangle=currTriangle->next){
    t=eyeTriangle(currTriangle->t);
    if(t.normal.z<=0)	continue;
    addTransformed(t);
  }
}

void drawTriangle(Triangle);
void drawTriangles(){
 TriangleList *currTriangle;
 for(currTriangle=transformedTriangles; currTriangle; currTriangle=currTriangle->next){
   drawTriangle(currTriangle->t);
 }
}

int isInside(Vertex v){
  if(v.y<-1.5 || v.y>1.5 || v.x>0) return 0;
  return 1;
}

void innerclip(Vertex *v1, Vertex *v2){
  float param;
  //printf("From %f %f %f, %f %f %f => ", v1->x, v1->y, v1->z, v2->x, v2->y, v2->z);
  if(v1->y<v2->y){
    if(v1->y<-1.5){
      param=(-1.5-v2->y)/(v1->y-v2->y);
      v1->y=-1.5; v1->x = (v1->x-v2->x)*param + v2->x; v1->z = (v1->z-v2->z)*param + v2->z;
    } else {
      param=(1.5-v2->y)/(v1->y-v2->y);
      v2->y=1.5; v2->x = v2->x - (v2->x-v1->x)*param; v1->z = v1->z - (v2->z-v1->z)*param;
    }
  } else {
    if(v2->y<-1.5){
      param=(-1.5-v1->y)/(v2->y-v1->y);
      v2->y=-1.5; v2->x = (v2->x-v1->x)*param + v1->x; v2->z = (v2->z-v1->z)*param + v1->z;
    } else {
      param=(1.5-v1->y)/(v2->y-v1->y);
      v1->y=1.5; v1->x = v1->x - (v1->x-v2->x)*param; v2->z = v2->z - (v1->z-v2->z)*param;
    }
  }
  //printf("%f %f %f, %f %f %f.\n", v1->x, v1->y, v1->z, v2->x, v2->y, v2->z);
}

int innerclipTriangle(Triangle *t){
  Vertex v1, v2, v3, vt1, vt2;
  int i1, i2, i3;
  v1=t->v1; v2=t->v2; v3=t->v3;
  i1=isInside(v1); i2=isInside(v2); i3=isInside(v3);
  if(i1 && i2 && i3) return 1;
  if(!i1 && !i2 && !i3) return 0;
  //printf("%d %d %d\n", i1, i2, i3);
  if(i1+i2+i3>1){
    //printf("Do something\n");
  }
  if(i1){
    if(i2){
      vt1=v3; vt2=v3;
      innerclip(&v1, &vt1); innerclip(&v2, &vt2);
      t->v3=vt2;
      addTriangle(triangle(v1, vt2, vt1, restoreIntensity(t->colour)));
      return 2;
    } else if(!i3){
      vt1=v2; vt2=v3;
      innerclip(&v1, &vt1); innerclip(&v1, &vt2);
      t->v2=vt1; t->v3=vt2;
      return 2;
    }
  }
  if(i2){
    if(i3){
      vt1=v1; vt2=v1;
      innerclip(&v2, &vt1); innerclip(&v3, &vt2);
      t->v1=vt2;
      addTriangle(triangle(v2, vt2, vt1, restoreIntensity(t->colour)));
      return 2;
    } else if(!i1){
      vt1=v3; vt2=v1;
      innerclip(&v2, &vt1); innerclip(&v2, &vt2);
      t->v3=vt1; t->v1=vt2;
      return 2;
    }
  }
  if(i3){
    if(i1){
      vt1=v2; vt2=v2;
      innerclip(&v3, &vt1); innerclip(&v1, &vt2);
      t->v2=vt2;
      addTriangle(triangle(v3, vt2, vt1, restoreIntensity(t->colour)));
      return 2;
    } else if(!i2){
      vt1=v1; vt2=v2;
      innerclip(&v3, &vt1); innerclip(&v3, &vt2);
      t->v1=vt1; t->v2=vt2;
      return 2;
    }
  }
  return 2;
}

void innerclipTriangles(){
  int ret;
  TriangleList *T=globalTriangles, *prev=globalTriangles, *temp;
  while(T){
    ret=innerclipTriangle(&(T->t));
    if(!ret){
      if(T==globalTriangles){
        temp=globalTriangles; globalTriangles=globalTriangles->next; T=globalTriangles; free(temp);
      } else {
        prev->next=T->next; free(T); T=prev->next;
      }
    } else {
      prev=T;
      T=T->next;
    }
  }
}
