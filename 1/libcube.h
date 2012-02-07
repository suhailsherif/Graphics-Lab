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
