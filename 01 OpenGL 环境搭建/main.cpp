#include "GLShaderManager.h"

#include "GLTools.h"

#include <glut/glut.h>

GLBatch triangleBatch;

GLShaderManager shaderManager;

GLfloat blockSize = 0.1f;

GLfloat vVerts[] = {
    -blockSize,-blockSize,0.0f,
    blockSize,-blockSize,0.0f,
    blockSize,blockSize,0.0f,
    -blockSize,blockSize,0.0f,
};

GLfloat xPot = 0.0f;
GLfloat yPot = 0.0f;


void ChangeSize(int w,int h)
{
    
    glViewport(0,0, w, h);
    
}

void SetupRC()

{
    glClearColor(1.0, 0.3, 0.6, 1.0f);
    shaderManager.InitializeStockShaders();
    triangleBatch.Begin(GL_TRIANGLE_FAN, 4);
    triangleBatch.CopyVertexData3f(vVerts);
    triangleBatch.End();
    

}

void RenderScene(void)

{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    //2.设置 填充颜色
    GLfloat vRed[] = {1.0,0.2,0.4,1.0f};
    M3DMatrix44f mFinalTransformMartix,mTransformMatrix,mRotationMatrix;
    
    m3dTranslationMatrix44(mTransformMatrix, xPot, yPot, 0.0f);
    
    

    static float angle = 0.0f;
    m3dRotationMatrix44(mRotationMatrix, m3dDegToRad(angle), 0.0f, 0.0f, 1.0f);
    angle += 5.0f;
    
    m3dMatrixMultiply44(mFinalTransformMartix, mTransformMatrix, mRotationMatrix);

    shaderManager.UseStockShader(GLT_SHADER_FLAT,mFinalTransformMartix,vRed);
    triangleBatch.Draw();
    glutSwapBuffers();
    
}

void specialKeys(int key, int x, int y) {
    GLfloat stepSize = 0.025f;
    
    if (key == GLUT_KEY_UP) {
        yPot += stepSize;
    }
    if (key == GLUT_KEY_DOWN) {
        yPot -= stepSize;
    }
    if (key == GLUT_KEY_LEFT) {
        xPot -= stepSize;
    }
    if (key == GLUT_KEY_RIGHT) {
        xPot += stepSize;
    }
    
    if (xPot < -1 +blockSize) {
        xPot = -1 + blockSize;
    }
    if (yPot > 1 - blockSize) {
        yPot = 1 - blockSize;
    }
    if (xPot > 1 - blockSize) {
        xPot = 1 - blockSize;
    }
    if (yPot < -1 + blockSize) {
        yPot = -1 + blockSize;
    }
    
    glutPostRedisplay();
    
}

int main(int argc,char* argv[])

{
    gltSetWorkingDirectory(argv[0]);
    
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH|GLUT_STENCIL);

    glutInitWindowSize(800,600);
    
    glutCreateWindow("Triangle");
    

    
    glutReshapeFunc(ChangeSize);

    glutDisplayFunc(RenderScene);
    

    glutSpecialFunc(specialKeys);
    
    GLenum err = glewInit();
    
    if(GLEW_OK != err) {
        
        fprintf(stderr,"glew error:%s\n",glewGetErrorString(err));
        
        return 1;
        
    }
    
    SetupRC();
    
    glutMainLoop();
    
    return 0;
    
}

