typedef struct _octree {
    struct _octree *children[8];
    int leaf;
} octree;

int cubeVertices[8][3] = { {-1, -1, -1}, {1, -1, -1}, {-1, 1, -1}, {1, 1, -1}, {-1, -1, 1}, {1, -1, 1}, {-1, 1, 1}, {1, 1, 1} };
int cubeFaces[6][4] = { {2, 3, 1, 0}, {1, 3, 7, 5 }, {5, 7, 6, 4}, {4, 6, 2, 0}, {0, 1, 5, 4}, {3, 2, 6, 7} };
int cubeColors[6][3] = { {1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {1, 1, 0}, {1, 0, 1}, {1, 1, 0} };

void drawCube(float centerx, float centery, float centerz, float side){
    int i, j;
    for(i=0;i<6;i++){
        glColor3f(cubeColors[i][0], cubeColors[i][1], cubeColors[i][2]);
        glBegin(GL_POLYGON);
        for(j=0;j<4;j++){
            glVertex3f(centerx+cubeVertices[cubeFaces[i][j]][0]*side/2, centery+cubeVertices[cubeFaces[i][j]][1]*side/2, centerz+cubeVertices[cubeFaces[i][j]][2]*side/2);
        }
        glEnd();
    }
}

void drawOctree(octree *a, float centerx, float centery, float centerz, float side){
    int i;
    if(a==NULL)	return;
    if(a->leaf==-1)	return;
    if(a->leaf==1){
        drawCube(centerx, centery, centerz, side);
    }
    else {
        for(i=0;i<8;i++){
            drawOctree(a->children[i], centerx+cubeVertices[i][0]*side/4, centery+cubeVertices[i][1]*side/4, centerz+cubeVertices[i][2]*side/4, side/2);
        }
    }
}

typedef struct _voxel {
    float x, y, z;
    struct _voxel *next;
} voxel;

typedef struct _voxels {
    voxel *begin, *end;
} voxels;

void drawVoxels(voxels *a, float offsetx, float offsety, float offsetz, float unitSize){
    voxel *current;
    if(a->begin==NULL)	return;
    current=a->begin;
    while(current){
        drawCube(offsetx+current->x+unitSize/2, offsety+current->y+unitSize/2, offsetz+current->z+unitSize/2, unitSize);
        current=current->next;
    }
}

void addVoxel(voxels *a, float x, float y, float z){
    voxel *newV = (voxel *)malloc(sizeof(voxel));
    newV->x=x;
    newV->y=y;
    newV->z=z;
    newV->next=NULL;
    if(a->begin==NULL){
        a->begin=newV;
        a->end=newV;
    } else {
        a->end->next=newV;
        a->end=newV;
    }
}




















void addChildren(octree *a, int o0, int o1, int o2, int o3, int o4, int o5, int o6, int o7){
    if(o0==0) a->children[0]=NULL;
    if(o0==1){
        a->children[0]=(octree *)malloc(sizeof(octree));
        a->children[0]->leaf=0;
    }
    if(o0==2){
        a->children[0]=(octree *)malloc(sizeof(octree));
        a->children[0]->leaf=1;
    }
    if(o1==0) a->children[1]=NULL;
    if(o1==1){
        a->children[1]=(octree *)malloc(sizeof(octree));
        a->children[1]->leaf=0;
    }
    if(o1==2){
        a->children[1]=(octree *)malloc(sizeof(octree));
        a->children[1]->leaf=1;
    }
    if(o2==0) a->children[2]=NULL;
    if(o2==1){
        a->children[2]=(octree *)malloc(sizeof(octree));
        a->children[2]->leaf=0;
    }
    if(o2==2){
        a->children[2]=(octree *)malloc(sizeof(octree));
        a->children[2]->leaf=1;
    }
    if(o3==0) a->children[3]=NULL;
    if(o3==1){
        a->children[3]=(octree *)malloc(sizeof(octree));
        a->children[3]->leaf=0;
    }
    if(o3==2){
        a->children[3]=(octree *)malloc(sizeof(octree));
        a->children[3]->leaf=1;
    }
    if(o4==0) a->children[4]=NULL;
    if(o4==1){
        a->children[4]=(octree *)malloc(sizeof(octree));
        a->children[4]->leaf=0;
    }
    if(o4==2){
        a->children[4]=(octree *)malloc(sizeof(octree));
        a->children[4]->leaf=1;
    }
    if(o5==0) a->children[5]=NULL;
    if(o5==1){
        a->children[5]=(octree *)malloc(sizeof(octree));
        a->children[5]->leaf=0;
    }
    if(o5==2){
        a->children[5]=(octree *)malloc(sizeof(octree));
        a->children[5]->leaf=1;
    }

    if(o6==0) a->children[6]=NULL;
    if(o6==1){
        a->children[6]=(octree *)malloc(sizeof(octree));
        a->children[6]->leaf=0;
    }
    if(o6==2){
        a->children[6]=(octree *)malloc(sizeof(octree));
        a->children[6]->leaf=1;
    }

    if(o7==0) a->children[7]=NULL;
    if(o7==1){
        a->children[7]=(octree *)malloc(sizeof(octree));
        a->children[7]->leaf=0;
    }
    if(o7==2){
        a->children[7]=(octree *)malloc(sizeof(octree));
        a->children[7]->leaf=1;
    }
}

