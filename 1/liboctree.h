typedef struct _octree {
    struct _octree *children[8];
    int leaf;
} octree;

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

void addChildren(octree *a, int o0, int o1, int o2, int o3, int o4, int o5, int o6, int o7){
    int i, args[]={o0, o1, o2, o3, o4, o5, o6, o7};
    for(i=0;i<8;i++){
        if(args[i]==0) a->children[i]=NULL;
        if(args[i]==1){
            a->children[i]=(octree *)malloc(sizeof(octree));
            a->children[i]->leaf=0;
        }
        if(args[i]==2){
            a->children[i]=(octree *)malloc(sizeof(octree));
            a->children[i]->leaf=1;
        }
    }
}

