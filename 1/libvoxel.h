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
