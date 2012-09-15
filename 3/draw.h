float *depthBuffer=NULL;
float (*boundaryPoints)[4];
int y0min, y0max, y1min, y1max;
extern int MAX_H, MAX_W, WxH;
extern unsigned int *data;

void initDraw(){
  int i, j;
  if(!depthBuffer) depthBuffer=malloc(WxH*sizeof(float));
  for(i=0;i<WxH;i++)  depthBuffer[i]=2;
  j=127 + (127<<8) + (127<<16);
  for(i=0;i<WxH;++i)    data[i]=j;
}   

int testValid(Vertex a){
  if(a.x>=-1 && a.x<=1 && a.y>=-1 && a.y<=1 && a.z>=0 && a.z<=1)
    return 1;
  else return 0;
}

void addToBoundaryPoints(int x, int y, float z){
  if(x<0 || x>=MAX_W || y<0 || y>=MAX_H || z<0 || z>1) return;
  if(boundaryPoints[x][0]<0){
    boundaryPoints[x][0]=y; boundaryPoints[x][1]=z; boundaryPoints[x][2]=y; boundaryPoints[x][3]=z;
  } else {
    if(boundaryPoints[x][0]>y){
      boundaryPoints[x][0]=y; boundaryPoints[x][1]=z;
    } else if(boundaryPoints[x][2]<y){
      boundaryPoints[x][2]=y; boundaryPoints[x][3]=z;
    }
  }
}

int clip(Vertex *a, Vertex *b){
  int reg_a=0, reg_b=0, min=-1, ret=2;
  float x_check, y_check, z_check, param=0, max_param=0;
  if(a->x>1)  reg_a+=2;
  else if(a->x<-1)  reg_a+=1;
  if(a->y>1)  reg_a+=8;
  else if(a->y<-1)  reg_a+=4;
  if(a->z>1)  reg_a+=32;
  else if(a->z<0)  reg_a+=16;
  if(b->x>1)  reg_b+=2;
  else if(b->x<-1)  reg_b+=1;
  if(b->y>1)  reg_b+=8;
  else if(b->y<-1)  reg_b+=4;
  if(b->z>1)  reg_b+=32;
  else if(b->z<0)  reg_b+=16;
  if(reg_a==0 && reg_b==0)  return 1;
  if((a->x==b->x) && (a->y==b->y) && (a->z==b->z)){
    return 0;
  }
  if((reg_a&reg_b)!=0)  return 0;
  if(reg_a!=0){
    x_check=(reg_a&3)==0?0:((reg_a&3)*2-3); y_check=(reg_a&12)==0?0:((reg_a&12)/2-3); z_check=(reg_a&48)==0?-1:((reg_a&48)/16-1);
    //printf("\nProblem! %f %f %f,", x_check, y_check, z_check+1);
    if(x_check) { param=(x_check-a->x)/(b->x-a->x); if(max_param<param){ max_param=param; min=0; }}
    if(y_check) { param=(y_check-a->y)/(b->y-a->y); if(max_param<param){ max_param=param; min=1; }}
    if(z_check+1)       { param=(z_check-a->z)/(b->z-a->z); if(max_param<param){ max_param=param; min=2; }}
    if(min==0) {
      if(a->x<x_check){
        if(x_check==1){
          a->x=1+.000001;
        } else {
          ret+=3;
          a->x=-1;
        }
      } else if(a->x>x_check){
        if(x_check==-1){
          a->x=-1-.000001;
        } else {
          ret+=1;
          a->x=1;
        }
      }
    } else a->x=max_param*(b->x-a->x) + a->x;
    if(min==1) {
      if(a->y<y_check){
        if(y_check==1){
          a->y=1+.000001;
        } else {
          ret+=2;
          a->y=-1;
        }
      } else if(a->y>y_check){
        if(y_check==-1){
          a->y=-1-.000001;
        } else {
          ret+=0;
          a->y=1;
        }
      }
    } else a->y=max_param*(b->y-a->y) + a->y;
    if(min==2) {
      if(a->z<z_check){
        if(z_check==1){
          a->z=1+.000001;
        } else {
//          ret+=-250;
          a->z=0;
        }
      } else if(a->z>z_check){
        if(z_check==-1){
          a->z=-.000001;
        } else {
//          ret+=-250;
          a->z=1;
        }
      }
    } else a->z=max_param*(b->z-a->z) + a->z;
    if(!testValid(*a)){
      return 0;
    }
  } else ret+=4;
  param=0; max_param=0; min=-1;
  if(reg_b!=0){
    x_check=(reg_b&3)==0?0:(reg_b&3)*2-3; y_check=(reg_b&12)==0?0:(reg_b&12)/2-3; z_check=(reg_b&48)==0?-1:(reg_b&48)/16-1;
    //printf("\nProblem! %f %f %f", x_check, y_check, z_check+1);
    if(x_check) { param=(x_check-b->x)/(a->x-b->x); if(max_param<param){ max_param=param; min=0; }}
    if(y_check) { param=(y_check-b->y)/(a->y-b->y); if(max_param<param){ max_param=param; min=1; }}
    if(z_check+1)       { param=(z_check-b->z)/(a->z-b->z); if(max_param<param){ max_param=param; min=2; }}
    if(min==0) {
      if(b->x<x_check){
        if(x_check==1){
          b->x=1+.000001;
        } else {
          ret+=15;
          b->x=-1;
        }
      } else if(b->x>x_check){
        if(x_check==-1){
          b->x=-.000001;
        } else {
          ret+=5;
          b->x=1;
        }
      }
    } else b->x=max_param*(a->x-b->x) + b->x;
    if(min==1) {
      if(b->y<y_check){
        if(y_check==1){
          b->y=1+.000001;
        } else {
          ret+=10;
          b->y=-1;
        }
      } else if(b->y>y_check){
        if(y_check==-1){
          b->y=-1-.000001;
        } else {
          ret+=0;
          b->y=1;
        }
      }
    } else b->y=max_param*(a->y-b->y) + b->y;
    if(min==2) {
      if(b->z<z_check){
        if(z_check==1){
          b->z=1+.000001;
        } else {
//          ret+=-150;
          b->z=0;
        }
      } else if(b->z>z_check){
        if(z_check==-1){
          b->z=-1-.000001;
        } else {
//          ret+=-150;
          b->z=1;
        }
      }
    } else b->z=max_param*(a->z-b->z) + b->z;
    if(!testValid(*b)){
      return 0;
    }
  } else ret+=20;
  //printf("\n");
  return ret;
}

void scanConvert(Vertex a, Vertex b){
  int tx1=((a.x+1)*(MAX_W-1))/2, tx2=((b.x+1)*(MAX_W-1))/2, ty1=((a.y+1)*(MAX_H-1))/2, ty2=((b.y+1)*(MAX_H-1))/2;
  int x1, x2, y1, y2;
  int dx, dy, err=0, incr;
  float m=(tx2==tx1)?100:(float)(ty2-ty1)/(float)(tx2-tx1), tz1=a.z, tz2=b.z, z1, z2, dz;
  //  if(!testValid(a) || !testValid(b)) return;
  if(tx1==tx2 && ty1 == ty2) return;
  if(m>1 || m<-1){
    if(ty1<ty2){ y1=ty1; y2=ty2; x1=tx1; x2=tx2; z1=tz1; z2=tz2; }
    else{ y1=ty2; y2=ty1; x1=tx2; x2=tx1; z1=tz2; z2=tz1; }
  } else {
    if(tx1<tx2){ x1=tx1; x2=tx2; y1=ty1; y2=ty2; z1=tz1; z2=tz2; }
    else{ x1=tx2; x2=tx1; y1=ty2; y2=ty1; z1=tz2; z2=tz1; }
  }
  dx=abs(x2-x1); dy=abs(y2-y1); err=0;
  ////printf("%d %d %f, %d %d %f, %d, %d\n", x1, y1, z1, x2, y2, z2, dx, dy);
  if(m<=1 && m>=-1){
    incr=m>0?1:-1; dz=(z2-z1)/dx;
    for(;x1<=x2;++x1){
      addToBoundaryPoints(x1, y1, z1);
      if(2*(err+dy)<dx)  err=err+dy;
      else { y1+=incr; err=err+dy-dx; }
      z1+=dz;
    }
  } else {
    incr=m>0?1:-1; dz=(z2-z1)/dy;
    for(;y1<=y2;++y1){
      addToBoundaryPoints(x1, y1, z1);
      if(2*(err+dx)<dy) err=err+dx;
      else { x1+=incr; err=err+dx-dy; }
      z1+=dz;
    }
  }

}

void fillTriangle(int colour){
  int i, j, no_pix=0;
  float z, dz;
  for(i=0;i<MAX_H;i++){
    if(boundaryPoints[i][0]>=0){
      dz=(boundaryPoints[i][3]-boundaryPoints[i][1])/(boundaryPoints[i][2]-boundaryPoints[i][0]);
      z=boundaryPoints[i][1];
      for(j=boundaryPoints[i][0]; j<=boundaryPoints[i][2]; ++j){
        if(depthBuffer[MAX_W*j+i]>z){
          depthBuffer[MAX_W*j+i]=z;
          data[MAX_W*j+i]=colour;
          ++no_pix;
        }
        z+=dz;
      }
    }
  }
  //printf("No_pix: %d\n", no_pix);
}

Vertex corner(int i){
  switch(i){
    case 0: return vertex(1.0, 1.0, 0.5);
    case 1: return vertex(1.0, -1.0, 0.5);
    case 2: return vertex(-1.0, -1.0, 0.5);
    case 3: return vertex(-1.0, 1.0, 0.5);
  }
}

void drawTriangle(Triangle t){
  Vertex l1v1=t.v1, l1v2=t.v2, l2v1=t.v2, l2v2=t.v3, l3v1=t.v3, l3v2=t.v1;
  int i,j, cuta, cutb, cutc, cuta1=-1, cuta2=-1, cutb1=-1, cutb2=-1, cutc1=-1, cutc2=-1;
  if(!boundaryPoints) boundaryPoints = malloc(4*MAX_H*sizeof(float));
  y0min=-1; y0max=-1; y1min=-1; y1max=-1;
  for(i=0;i<MAX_H;++i){ boundaryPoints[i][0]=-1; boundaryPoints[i][2]=-1; }
  cuta=clip(&l1v1, &l1v2);
  //if(cuta>1 || cuta<0)
    //printf("Clipped (%d): %f %f %f, %f %f %f => %f %f %f, %f %f %f\n", cuta, t.v1.x, t.v1.y, t.v1.z, t.v2.x, t.v2.y, t.v2.z, l1v1.x, l1v1.y, l1v1.z, l1v2.x, l1v2.y, l1v2.z);
  if(cuta)
    scanConvert(l1v1, l1v2);
  if(cuta>1){
    cuta1=(cuta-2)%5; cuta2=(cuta-2)/5; cuta1=(cuta1==4)?-1:cuta1; cuta2=(cuta2==4)?-1:cuta2;
  }
  cutb=clip(&l2v1, &l2v2);
  //if(cutb>1)
    //printf("Clipped (%d): %f %f %f, %f %f %f => %f %f %f, %f %f %f\n", cutb, t.v2.x, t.v2.y, t.v2.z, t.v3.x, t.v3.y, t.v3.z, l2v1.x, l2v1.y, l2v1.z, l2v2.x, l2v2.y, l2v2.z);
  if(cutb)
    scanConvert(l2v1, l2v2);
  if(cutb>1){
    cutb1=(cutb-2)%5; cutb2=(cutb-2)/5; cutb1=(cutb1==4)?-1:cutb1; cutb2=(cutb2==4)?-1:cutb2;
  }
  cutc=clip(&l3v1, &l3v2);
  //if(cutc>1)
    //printf("Clipped (%d): %f %f %f, %f %f %f => %f %f %f, %f %f %f\n", cutc, t.v3.x, t.v3.y, t.v3.z, t.v1.x, t.v1.y, t.v1.z, l3v1.x, l3v1.y, l3v1.z, l3v2.x, l3v2.y, l3v2.z);
  if(cutc)
    scanConvert(l3v1, l3v2);
  if(cutc>1){
    cutc1=(cutc-2)%5; cutc2=(cutc-2)/5; cutc1=(cutc1==4)?-1:cutc1; cutc2=(cutc2==4)?-1:cutc2;
  }
  if(cuta2>=0){
    j=(cutb1>=0)?cutb1:(cutc1>=0)?cutc1:cuta1;
    for(i=cuta2;i!=j;i=(i+1)%4){
      scanConvert((i==cuta2)?l1v2:corner((i+3)%4), corner(i));
    }
    scanConvert((cutb1==i)?l2v1:(cutc1==i)?l3v1:l1v1,(j==cuta2)?l1v2:corner((i+3)%4));
  }
  if(cutb2>=0){
    j=(cutc1>=0)?cutc1:(cuta1>=0)?cuta1:cutb1;
    for(i=cutb2;i!=j;i=(i+1)%4){
      scanConvert((i==cutb2)?l2v2:corner((i+3)%4), corner(i));
    }
    scanConvert((cutc1==i)?l3v1:(cuta1==i)?l1v1:l2v1,(j==cutb2)?l2v2:corner((i+3)%4));
  }
  if(cutc2>=0){
    j=(cuta1>=0)?cuta1:(cutb1>=0)?cutb1:cutc1;
    for(i=cutc2;i!=j;i=(i+1)%4){
      scanConvert((i==cutc2)?l3v2:corner((i+3)%4), corner(i));
    }
    scanConvert((cuta1==i)?l1v1:(cutb1==i)?l2v1:l3v1,(j==cutc2)?l3v2:corner((i+3)%4));
  }
  /**/
        if(cuta<0 || cutb<0 || cutc<0){
      cuta=cuta<0?(cuta+200):0; cutb=cutb<0?(cutb+200):0; cutc=cutc<0?(cutc+200):0;
      if(cuta && cutb) { if(cuta<0) scanConvert(l1v2, l2v1); else scanConvert(l1v1, l2v2); }
      if(cutb && cutc) { if(cutb<0) scanConvert(l2v2, l3v1); else scanConvert(l2v1, l3v2); }
      if(cutc && cuta) { if(cutc<0) scanConvert(l3v2, l1v1); else scanConvert(l3v1, l1v2); }
      }
   /**/
  /*
     scanConvert(t.v1, t.v2);
     scanConvert(t.v2, t.v3);
     scanConvert(t.v3, t.v1);
   */  fillTriangle(t.colour);
}

void printBoundaryPointsTable(){
  int i;
  for(i=0;i<MAX_H;++i)  if(boundaryPoints[i][0]>=0);  //printf("%d: %f %f\n", i, boundaryPoints[i][0], boundaryPoints[i][2]);
}
