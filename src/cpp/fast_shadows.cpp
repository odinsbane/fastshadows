/**
*   
*   This is intended to be a minimal implementation of stencil shadows 
*   for plugable use, and improving rendering of scientific 
*   applications.
*
*
**/

#include "Framework.h"
#include "Camera.h"
#include "InputAdapter.h"
#include "math.h"

void createScene(GLuint program);
void drawScene(GLuint program);
void graphicsLoop(GLuint program, GLFWwindow* window);

GLuint VAO[3];


int main(int argc, char** args){
    
    GLFWwindow* window = Framework::initializeWindow();
    GLuint program = Framework::loadProgram();
    createScene(program);
    Camera* cam = new Camera(program);
    InputAdapter* adapter = new InputAdapter(cam, window);
    float time = 0;
    float dt = 0.01;
    while( adapter->running ){
        
        drawScene(program);
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        
        /* Poll for and process events */
        glfwPollEvents();
        adapter->update();
        time += dt;
        cam->setLightPosition(5*sin(time), 5*cos(time), 10);
    }
    
}


float tetraColor[]{1,1,1,1};
float towerColor[]{1,0,0,1};
float groundColor[]{0,0,1,1};

void drawScene(GLuint program){
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(program);
    
    GLuint color = glGetUniformLocation(program, "inputColor");
    
    glBindVertexArray(VAO[0]);
    glUniform4fv(color, 1, tetraColor);
    glDrawArrays(GL_TRIANGLES, 0, 12);

    glBindVertexArray(VAO[1]);
    glUniform4fv(color, 1, groundColor);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(VAO[2]);
    glUniform4fv(color, 1, towerColor);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    glBindVertexArray(0);
    glUseProgram(0);
}
/**
 Assuming a,b, and c are vertices of a triangle.
 
 */
void calculateNormal(float* a, float* b, float* c, float* n){
    
    float ab[3];
    float ac[3];
    for(int i = 0;i<3;i++){
        ab[i]= b[i] - a[i];
        ac[i] = c[i] - a[i];
    }



    n[0] = ab[1]*ac[2] - ab[2]*ac[1];
    n[1] = ab[2]*ac[0] - ab[0]*ac[2];
    n[2] = ab[0]*ac[1] - ab[1]*ac[0];
    

    float l = sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
    n[0] = n[0]/l;
    n[1] = n[1]/l;
    n[2] = n[2]/l;

}

void createVertexBuffer(float* positions,int* indices, int triangles, float* output){
    int o = triangles*3*3;
    float* normals = &output[o];
    
    //over each triangle.
    float norm[3];
    
    for(int i = 0; i<triangles; i++){
        
        calculateNormal(
            &positions[3*indices[3*i]],
            &positions[3*indices[3*i+1]],
            &positions[3*indices[3*i+2]],
            norm);
        
        //over each vertex
        for(int j = 0; j<3; j++){
            int v = i*3 + j;
            int dex = indices[v];
            for( int k = 0; k<3; k++){
                int c = i*9+j*3 + k;
                output[ c ] = positions[3*dex + k];
                normals[ c ] = norm[k];
            }
        }
    }
    
}

void createScene(GLuint program){
    float d = sqrt(3);
    
    float tetrahedron[12]{
        0, 0, 1.5,
        0, d/2, 0,
        -1, -d/2, 0,
        1, -d/2, 0
    };
    
    int tetdex[12]{
        1, 2, 0,
        2, 3, 0,
        3, 1, 0,
        2, 1, 3
    };
    
    float* tetraVertices = new float[72];
    
    createVertexBuffer(tetrahedron, tetdex, 4, tetraVertices);

    float ground[12]{
        -4, -4, 0,
        -4, 4, 0,
        4, 4, 0,
        4, -4, 0
    };

    int gdex[6]{
        0, 2, 1,
        0, 3, 2
    };
    
    float* groundVertices = new float[2*2*3*3];
    createVertexBuffer(ground, gdex, 2, groundVertices);
    
    float tower[24]{
        1, 1, 0,
        1, 2, 0,
        2, 2, 0,
        2, 1, 0,
        1, 1, 3,
        1, 2, 3,
        2, 2, 3,
        2, 1, 3
    };
    
    
    int tdex[36]{
        0, 1, 2,  0, 2, 3,  //bottom
        4, 6, 5,  4, 7, 6,  //top
        0, 7, 4,  0, 3, 7,  //front
        3, 2, 7,  2, 6, 7,  //right
        1, 5, 6,  1, 6, 2,  //back
        0, 4, 1,  1, 4, 5   //left
    };
    float* towerVertices = new float[2*12*3*3];
    createVertexBuffer(tower, tdex, 12, towerVertices);
    
    glUseProgram(program);
    
    GLuint bufferObject[3];
    
    
    glGenBuffers(3, bufferObject);

    glBindBuffer(GL_ARRAY_BUFFER , bufferObject[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*72, tetraVertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, bufferObject[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*36,groundVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, bufferObject[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*216, towerVertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    long sizes[]{36*sizeof(float), 18*sizeof(float), 108*sizeof(float)};
    GLuint positionAttribute = glGetAttribLocation(program, "position");
    GLuint normalAttribute = glGetAttribLocation(program, "normal");
    printf("pos, norm: %d, %d \n", positionAttribute, normalAttribute);

    
    glGenVertexArrays(3, VAO);
    
    printf("generated\n");
    for(int i = 0; i<3; i++){
        glBindVertexArray(VAO[i]);
        
        glBindBuffer(GL_ARRAY_BUFFER, bufferObject[i]);
        
        glEnableVertexAttribArray(positionAttribute);
        glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(normalAttribute);
        glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, 0, (void*)sizes[i]);
    }
    glBindVertexArray(0);
    glUseProgram(0);
    
    
    delete[] tetraVertices;
    delete[] towerVertices;
    delete[] groundVertices;

}

