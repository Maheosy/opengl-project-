/**
 * This project (GLSL_Project1) is created by Fahim Hasan Khan (fahimhasan.khan [at] ucalgary.ca)
 * based on the works of Andrew Robert Owens (arowens [at] ucalgary.ca)
 * who is a PhD student at Graphics Jungle, University of Calgary
 *
 * File:	main.cpp
 *
 * Summary:
 *
 * This is a (very) basic program to
 * 1) load shaders from external files, and make a shader program
 * 2) make Vertex Array Object and Vertex Buffer Object for the quad
 *
 **/

#include <iostream>
#include <cmath>
#include <chrono>
#include <limits>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "ShaderTools.h"
#include "Vec3f.h"
#include "Mat4f.h"
#include "OpenGLMatrixTools.h"
#include "Camera.h"

//==================== GLOBAL VARIABLES ====================//
/*	Put here for simplicity. Feel free to restructure into
*	appropriate classes or abstractions.
*/

// Drawing Program
GLuint basicProgramID;

// Data needed for Quad
GLuint vaoID,vaoID1,vaoID2,vaoID3,vaoID4,vaoID5,vaoID6,vaoID7,vaoID8,vaoID9,vaoID10;
GLuint vertBufferID,vertBufferID1,vertBufferID2,vertBufferID3,vertBufferID4,vertBufferID5,vertBufferID6,vertBufferID7,vertBufferID8,vertBufferID9,vertBufferID10;
Mat4f M;

// Data needed for Line
GLuint line_vaoID,line_vaoID1,line_vaoID2,line_vaoID3;
GLuint line_vertBufferID,line_vertBufferID1,line_vertBufferID2,line_vertBufferID3;
Mat4f line_M;

// Only one camera so only one veiw and perspective matrix are needed.
Mat4f V;
Mat4f P;

// Only one thing is rendered at a time, so only need one MVP
// When drawing different objects, update M and MVP = M * V * P
Mat4f MVP;

// Camera and veiwing Stuff
Camera camera;
int g_moveUpDown = 0;
int g_moveLeftRight = 0;
int g_moveBackForward = 0;
int g_rotateLeftRight = 0;
int g_rotateUpDown = 0;
int g_rotateRoll = 0;
float g_rotationSpeed = 0.015625;
float g_panningSpeed = 0.25;
bool g_cursorLocked;
float g_cursorX, g_cursorY;

bool g_play = false;

int WIN_WIDTH = 800, WIN_HEIGHT = 600;
int FB_WIDTH = 800, FB_HEIGHT = 600;
float WIN_FOV = 60;
float WIN_NEAR = 0.01;
float WIN_FAR = 1000;

//==================== FUNCTION DECLARATIONS ====================//
void displayFunc();
void resizeFunc();
void init();
void generateIDs();
void deleteIDs();
void setupVAO();
void loadQuadGeometryToGPU();
void loadQuadGeometryToGPU1();
void loadQuadGeometryToGPU2();
void loadQuadGeometryToGPU3();
void loadQuadGeometryToGPU4();// MOUNTAINs
void loadQuadGeometryToGPU5();
void loadQuadGeometryToGPU6();

void loadQuadGeometryToGPU7();
void loadQuadGeometryToGPU8();
void loadQuadGeometryToGPU9();
void loadQuadGeometryToGPU10();


//lines//
void loadLineGeometryToGPU();
void loadLineGeometryToGPU1();
void loadLineGeometryToGPU2();
void loadLineGeometryToGPU3();

void reloadProjectionMatrix();
void loadModelViewMatrix();
void setupModelViewProjectionTransform();

void windowSetSizeFunc();
void windowKeyFunc(GLFWwindow *window, int key, int scancode, int action,
                   int mods);
void windowMouseMotionFunc(GLFWwindow *window, double x, double y);
void windowSetSizeFunc(GLFWwindow *window, int width, int height);
void windowSetFramebufferSizeFunc(GLFWwindow *window, int width, int height);
void windowMouseButtonFunc(GLFWwindow *window, int button, int action,
                           int mods);
void windowMouseMotionFunc(GLFWwindow *window, double x, double y);
void windowKeyFunc(GLFWwindow *window, int key, int scancode, int action,
                   int mods);
void animateQuad(float t);
void moveCamera();
void reloadMVPUniform();
void reloadColorUniform(float r, float g, float b);
std::string GL_ERROR();
int main(int, char **);

//==================== FUNCTION DEFINITIONS ====================//

void displayFunc() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Use our shader
  glUseProgram(basicProgramID);

  // ===== DRAW QUAD ====== //
  MVP = P * V * M;

  reloadMVPUniform();
  reloadColorUniform(0.5, 0, 0.5); // window 1
  glBindVertexArray(vaoID);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);

  reloadMVPUniform();
  reloadColorUniform(0.5, 0, 0.50); //window 2
  glBindVertexArray(vaoID1);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);

reloadMVPUniform();
  reloadColorUniform(0.0, 0.5, 0.0);        // GHRASS
  glBindVertexArray(vaoID2);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 7);

reloadMVPUniform();                         //mountain 1
  reloadColorUniform(.60, .4, .12);
  glBindVertexArray(vaoID3);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);

  reloadMVPUniform();                       //mountain 2
  reloadColorUniform(.60, .4, .12);
  glBindVertexArray(vaoID4);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);

  reloadMVPUniform();                        //mountain 3
  reloadColorUniform(.60, .4, .12);
  glBindVertexArray(vaoID5);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);

  reloadMVPUniform();                          //mountain 4
  reloadColorUniform(.60, .4, .12);
  glBindVertexArray(vaoID6);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);


  reloadMVPUniform();                          //window border
  reloadColorUniform(0.5, 0,0.5 );
  glBindVertexArray(vaoID7);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 10);

  reloadMVPUniform();                          // SUNN
  reloadColorUniform(1, 0, 0);
  glBindVertexArray(vaoID8);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 20);

  reloadMVPUniform();                          // SKY
  reloadColorUniform(0, 0.5, 1);
  glBindVertexArray(vaoID9);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);


  reloadMVPUniform();                          // WALL
  reloadColorUniform(0, 0.5, 0.5);
  glBindVertexArray(vaoID10);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 8);


  // ==== DRAW LINE ===== //
  MVP = P * V * line_M;

  reloadMVPUniform();

  reloadColorUniform(0, 0, 1);

  // Use VAO that holds buffer bindings
  // and attribute config of buffers
  glBindVertexArray(line_vaoID);
  glDrawArrays(GL_LINES, 0, 2);
  // Draw lines

  reloadMVPUniform();
  reloadColorUniform(0, 0, 1);
  glBindVertexArray(line_vaoID1);
  glDrawArrays(GL_LINES, 0, 2);

  reloadMVPUniform();
  reloadColorUniform(0, 0, 1);
  glBindVertexArray(line_vaoID2);
  glDrawArrays(GL_LINES, 0, 2);

  reloadMVPUniform();
  reloadColorUniform(0, 0, 1);
  glBindVertexArray(line_vaoID3);
  glDrawArrays(GL_LINES, 0, 2);

}

void animateQuad(float t) {
  M = RotateAboutYMatrix(100 * t);

  float s = (std::sin(t) + 1.f) / 2.f;
  float x = (1 - s) * (10) + s * (-10);

  M = TranslateMatrix(x, 0, 0) * M;

  setupModelViewProjectionTransform();
  reloadMVPUniform();
}

void loadQuadGeometryToGPU() {
  // Just basic layout of floats, for a quad
  // 3 floats per vertex, 4 vertices
  std::vector<Vec3f> verts;
  verts.push_back(Vec3f(-15, 8, 0));          //WINDOW 1
  verts.push_back(Vec3f(-13,6, 0));
  verts.push_back(Vec3f(-15,-6, 0));
  verts.push_back(Vec3f(-13, -3.5, 0));
  verts.push_back(Vec3f(-13,6, 0));





  glBindBuffer(GL_ARRAY_BUFFER, vertBufferID);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(Vec3f) * 5, // byte size of Vec3f, 4 of them
               verts.data(),      // pointer (Vec3f*) to contents of verts
               GL_STATIC_DRAW);   // Usage pattern of GPU buffer
}
void loadQuadGeometryToGPU1() {
  std::vector<Vec3f> verts;                //WINDOW 2
  verts.push_back(Vec3f(14, 8, 0));
  verts.push_back(Vec3f(12, 6, 0));
  verts.push_back(Vec3f(14, -6, 0));
  verts.push_back(Vec3f(12, -3.5, 0));
  verts.push_back(Vec3f(12, 6, 0));

  glBindBuffer(GL_ARRAY_BUFFER, vertBufferID1);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(Vec3f) * 5,
               verts.data(),
               GL_STATIC_DRAW);
}
void loadQuadGeometryToGPU2() {
  std::vector<Vec3f> verts;
  verts.push_back(Vec3f(-15, -6, 0));
  verts.push_back(Vec3f(-13, -3.5, 0));
  verts.push_back(Vec3f(-5, -6, 0));
  verts.push_back(Vec3f(10, -3.5, 0));
  verts.push_back(Vec3f(11, -6, 0));
  verts.push_back(Vec3f(12, -3.5, 0));
  verts.push_back(Vec3f(14, -6, 0));
  glBindBuffer(GL_ARRAY_BUFFER, vertBufferID2);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(Vec3f) * 7,
               verts.data(),
               GL_STATIC_DRAW);
}
void loadQuadGeometryToGPU3() {              // MOUNTAIN 1
  std::vector<Vec3f> verts;
  verts.push_back(Vec3f(-13, -3.5, 0));
  verts.push_back(Vec3f(-9.5, 0, 0));
  verts.push_back(Vec3f(-6, -3.5, 0));


  glBindBuffer(GL_ARRAY_BUFFER, vertBufferID3);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(Vec3f) * 3,
               verts.data(),
               GL_STATIC_DRAW);
}
void loadQuadGeometryToGPU4() {       // MOUNTAIN 2
  std::vector<Vec3f> verts;
  verts.push_back(Vec3f(-6, -3.5, 0));
  verts.push_back(Vec3f(-2.8, 0, 0));
  verts.push_back(Vec3f(1, -3.5, 0));


  glBindBuffer(GL_ARRAY_BUFFER, vertBufferID4);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(Vec3f) * 3,
               verts.data(),
               GL_STATIC_DRAW);
}
void loadQuadGeometryToGPU5() {       // MOUNTAIN 3
  std::vector<Vec3f> verts;
  verts.push_back(Vec3f(1, -3.5, 0));
  verts.push_back(Vec3f(4.5, 0, 0));
  verts.push_back(Vec3f(8, -3.5, 0));


  glBindBuffer(GL_ARRAY_BUFFER, vertBufferID5);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(Vec3f) * 3,
               verts.data(),
               GL_STATIC_DRAW);
}
 void loadQuadGeometryToGPU6() {       // MOUNTAIN 4
  std::vector<Vec3f> verts;
  verts.push_back(Vec3f(8, -3.5, 0));
  verts.push_back(Vec3f(12, 0, 0));
  verts.push_back(Vec3f(12, -3.5, 0));


  glBindBuffer(GL_ARRAY_BUFFER, vertBufferID6);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(Vec3f) * 3,
               verts.data(),
               GL_STATIC_DRAW);

 }
 void loadQuadGeometryToGPU7() {       //Lower window border
  std::vector<Vec3f> verts;
  verts.push_back(Vec3f(-15, -6.5, 0));
  verts.push_back(Vec3f(-15,-6, 0));
  verts.push_back(Vec3f(-10.8,-6.5, 0));
  verts.push_back(Vec3f(-8,-6, 0));
  verts.push_back(Vec3f(-5,-6.5, 0));
  verts.push_back(Vec3f(-1,-6, 0));
  verts.push_back(Vec3f(6,-6.5, 0));
  verts.push_back(Vec3f(10,-6, 0));
  verts.push_back(Vec3f(14,-6.5, 0));
   verts.push_back(Vec3f(14,-6, 0));







  glBindBuffer(GL_ARRAY_BUFFER, vertBufferID7);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(Vec3f) * 10,
               verts.data(),
               GL_STATIC_DRAW);

 }

  void loadQuadGeometryToGPU8() {       //SUN
  std::vector<Vec3f> verts;
  verts.push_back(Vec3f(-6,-3.5, 0));
  verts.push_back(Vec3f(-6,-2.5, 0));
  verts.push_back(Vec3f(-5.5, -2.6, 0));
  verts.push_back(Vec3f(-6, -3.5, 0));

  verts.push_back(Vec3f(-6,-3.5, 0));
  verts.push_back(Vec3f(-6,-2.6, 0));
  verts.push_back(Vec3f(-5, -2.7, 0));
  verts.push_back(Vec3f(-6, -3.5, 0));

  verts.push_back(Vec3f(-6,-3.5, 0));
  verts.push_back(Vec3f(-6,-2.5, 0));
  verts.push_back(Vec3f(-6.5, -2.6, 0));
  verts.push_back(Vec3f(-6, -3.5, 0));

  verts.push_back(Vec3f(-6,-3.5, 0));
  verts.push_back(Vec3f(-6.5,-2.6, 0));
  verts.push_back(Vec3f(-6.3, -2.7, 0));
  verts.push_back(Vec3f(-6, -3.5, 0));

  verts.push_back(Vec3f(-6,-3.5, 0));
  verts.push_back(Vec3f(-6.3,-2.6, 0));
  verts.push_back(Vec3f(-7, -2.7, 0));
  verts.push_back(Vec3f(-6, -3.5, 0));






  glBindBuffer(GL_ARRAY_BUFFER, vertBufferID8);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(Vec3f) * 20,
               verts.data(),
               GL_STATIC_DRAW);

 }
  void loadQuadGeometryToGPU9() {       //SKY
  std::vector<Vec3f> verts;
  verts.push_back(Vec3f(-15,8, 0));
  verts.push_back(Vec3f(14,8, 0));
  verts.push_back(Vec3f(-13, -3.5, 0));

  verts.push_back(Vec3f(12,-3.5,0));
  verts.push_back(Vec3f(14,8, 0));




  glBindBuffer(GL_ARRAY_BUFFER, vertBufferID9);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(Vec3f) * 5,
               verts.data(),
               GL_STATIC_DRAW);

 }
  void loadQuadGeometryToGPU10() {       //WALL
  std::vector<Vec3f> verts;
  verts.push_back(Vec3f(-20,12, 0));
  verts.push_back(Vec3f(19,12, 0));
  verts.push_back(Vec3f(-20, -12, 0));

  verts.push_back(Vec3f(19,12,0));
  verts.push_back(Vec3f(19,-12, 0));




  glBindBuffer(GL_ARRAY_BUFFER, vertBufferID10);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(Vec3f) * 5,
               verts.data(),
               GL_STATIC_DRAW);

 }

void loadLineGeometryToGPU() {
  // Just basic layout of floats, for a quad
  // 3 floats per vertex, 4 vertices
  std::vector<Vec3f> verts;
  verts.push_back(Vec3f(-15, -6, 0));
  verts.push_back(Vec3f(-15, 8, 0));

  glBindBuffer(GL_ARRAY_BUFFER, line_vertBufferID);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(Vec3f) * 2, // byte size of Vec3f, 4 of them
               verts.data(),      // pointer (Vec3f*) to contents of verts
               GL_STATIC_DRAW);   // Usage pattern of GPU buffer
}
void loadLineGeometryToGPU1() {
  std::vector<Vec3f> verts;
  verts.push_back(Vec3f(-15, 8, 0));
  verts.push_back(Vec3f(14, 8, 0));

  glBindBuffer(GL_ARRAY_BUFFER, line_vertBufferID1);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(Vec3f) * 2,
               verts.data(),
               GL_STATIC_DRAW);
}
void loadLineGeometryToGPU2() {
  std::vector<Vec3f> verts;
  verts.push_back(Vec3f(14, 8, 0));
  verts.push_back(Vec3f(14, -6, 0));

  glBindBuffer(GL_ARRAY_BUFFER, line_vertBufferID2);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(Vec3f) * 2,
               verts.data(),
               GL_STATIC_DRAW);
}
void loadLineGeometryToGPU3() {
  std::vector<Vec3f> verts;
  verts.push_back(Vec3f(14, -6, 0));
  verts.push_back(Vec3f(-15, -6, 0));

  glBindBuffer(GL_ARRAY_BUFFER, line_vertBufferID3);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(Vec3f) * 2,
               verts.data(),
               GL_STATIC_DRAW);
}
void setupVAO() {
  glBindVertexArray(vaoID);

  glEnableVertexAttribArray(0); // match layout # in shader
  glBindBuffer(GL_ARRAY_BUFFER, vertBufferID);
  glVertexAttribPointer(0,        // attribute layout # above
                        3,        // # of components (ie XYZ )
                        GL_FLOAT, // type of components
                        GL_FALSE, // need to be normalized?
                        0,        // stride
                        (void *)0 // array buffer offset
                        );
glBindVertexArray(vaoID1);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertBufferID1);
  glVertexAttribPointer(0,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        0,
                        (void *)0
                        );
glBindVertexArray(vaoID2);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertBufferID2);
  glVertexAttribPointer(0,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        0,
                        (void *)0
                        );
  glBindVertexArray(vaoID3);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertBufferID3);
  glVertexAttribPointer(0,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        0,
                        (void *)0
                        );
  glBindVertexArray(vaoID4);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertBufferID4);
  glVertexAttribPointer(0,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        0,
                        (void *)0
                        );
   glBindVertexArray(vaoID5);

  glEnableVertexAttribArray(0); // match layout # in shader
  glBindBuffer(GL_ARRAY_BUFFER, vertBufferID5);
  glVertexAttribPointer(0,        // attribute layout # above
                        3,        // # of components (ie XYZ )
                        GL_FLOAT, // type of components
                        GL_FALSE, // need to be normalized?
                        0,        // stride
                        (void *)0 // array buffer offset
                        );
 glBindVertexArray(vaoID6);

  glEnableVertexAttribArray(0); // match layout # in shader
  glBindBuffer(GL_ARRAY_BUFFER, vertBufferID6);
  glVertexAttribPointer(0,        // attribute layout # above
                        3,        // # of components (ie XYZ )
                        GL_FLOAT, // type of components
                        GL_FALSE, // need to be normalized?
                        0,        // stride
                        (void *)0 // array buffer offset
                        );
glBindVertexArray(vaoID8);

  glEnableVertexAttribArray(0); // match layout # in shader
  glBindBuffer(GL_ARRAY_BUFFER, vertBufferID8);
  glVertexAttribPointer(0,        // attribute layout # above
                        3,        // # of components (ie XYZ )
                        GL_FLOAT, // type of components
                        GL_FALSE, // need to be normalized?
                        0,        // stride
                        (void *)0 // array buffer offset
                        );

 glBindVertexArray(vaoID7);

  glEnableVertexAttribArray(0); // match layout # in shader
  glBindBuffer(GL_ARRAY_BUFFER, vertBufferID7);
  glVertexAttribPointer(0,        // attribute layout # above
                        3,        // # of components (ie XYZ )
                        GL_FLOAT, // type of components
                        GL_FALSE, // need to be normalized?
                        0,        // stride
                        (void *)0 // array buffer offset
                        );

 glBindVertexArray(vaoID9);

  glEnableVertexAttribArray(0); // match layout # in shader
  glBindBuffer(GL_ARRAY_BUFFER, vertBufferID9);
  glVertexAttribPointer(0,        // attribute layout # above
                        3,        // # of components (ie XYZ )
                        GL_FLOAT, // type of components
                        GL_FALSE, // need to be normalized?
                        0,        // stride
                        (void *)0 // array buffer offset
                        );
 glBindVertexArray(vaoID10);

  glEnableVertexAttribArray(0); // match layout # in shader
  glBindBuffer(GL_ARRAY_BUFFER, vertBufferID10);
  glVertexAttribPointer(0,        // attribute layout # above
                        3,        // # of components (ie XYZ )
                        GL_FLOAT, // type of components
                        GL_FALSE, // need to be normalized?
                        0,        // stride
                        (void *)0 // array buffer offset
                        );
  glBindVertexArray(line_vaoID);

  glEnableVertexAttribArray(0); // match layout # in shader
  glBindBuffer(GL_ARRAY_BUFFER, line_vertBufferID);
  glVertexAttribPointer(0,        // attribute layout # above
                        3,        // # of components (ie XYZ )
                        GL_FLOAT, // type of components
                        GL_FALSE, // need to be normalized?
                        0,        // stride
                        (void *)0 // array buffer offset
                        );

  glBindVertexArray(line_vaoID1);
  glEnableVertexAttribArray(0); // match layout # in shader
  glBindBuffer(GL_ARRAY_BUFFER, line_vertBufferID1);
  glVertexAttribPointer(0,        // attribute layout # above
                        3,        // # of components (ie XYZ )
                        GL_FLOAT, // type of components
                        GL_FALSE, // need to be normalized?
                        0,        // stride
                        (void *)0 // array buffer offset
                        );
  glBindVertexArray(line_vaoID2);
  glEnableVertexAttribArray(0); // match layout # in shader
  glBindBuffer(GL_ARRAY_BUFFER, line_vertBufferID2);
  glVertexAttribPointer(0,        // attribute layout # above
                        3,        // # of components (ie XYZ )
                        GL_FLOAT, // type of components
                        GL_FALSE, // need to be normalized?
                        0,        // stride
                        (void *)0 // array buffer offset
                        );
  glBindVertexArray(line_vaoID3);
  glEnableVertexAttribArray(0); // match layout # in shader
  glBindBuffer(GL_ARRAY_BUFFER, line_vertBufferID3);
  glVertexAttribPointer(0,        // attribute layout # above
                        3,        // # of components (ie XYZ )
                        GL_FLOAT, // type of components
                        GL_FALSE, // need to be normalized?
                        0,        // stride
                        (void *)0 // array buffer offset
                        );
  glBindVertexArray(0); // reset to default
}

void reloadProjectionMatrix() {
  // Perspective Only

  // field of view angle 60 degrees
  // window aspect ratio
  // near Z plane > 0
  // far Z plane

  P = PerspectiveProjection(WIN_FOV, // FOV
                            static_cast<float>(WIN_WIDTH) /
                                WIN_HEIGHT, // Aspect
                            WIN_NEAR,       // near plane
                            WIN_FAR);       // far plane depth
}

void loadModelViewMatrix() {
  M = IdentityMatrix();
  line_M = IdentityMatrix();
  // view doesn't change, but if it did you would use this
  V = camera.lookatMatrix();
}

void reloadViewMatrix() { V = camera.lookatMatrix(); }

void setupModelViewProjectionTransform() {
  MVP = P * V * M; // transforms vertices from right to left (odd huh?)
}

void reloadMVPUniform() {
  GLint id = glGetUniformLocation(basicProgramID, "MVP");

  glUseProgram(basicProgramID);
  glUniformMatrix4fv(id,        // ID
                     1,         // only 1 matrix
                     GL_TRUE,   // transpose matrix, Mat4f is row major
                     MVP.data() // pointer to data in Mat4f
                     );
}

void reloadColorUniform(float r, float g, float b) {
  GLint id = glGetUniformLocation(basicProgramID, "inputColor");

  glUseProgram(basicProgramID);
  glUniform3f(id, // ID in basic_vs.glsl
              r, g, b);
}

void generateIDs() {
  // shader ID from OpenGL
  std::string vsSource = loadShaderStringfromFile("./shaders/basic.vert");
  std::string fsSource = loadShaderStringfromFile("./shaders/basic.frag");
  basicProgramID = CreateShaderProgram(vsSource, fsSource);

  // VAO and buffer IDs given from OpenGL
  glGenVertexArrays(1, &vaoID);
  glGenBuffers(1, &vertBufferID);

  glGenVertexArrays(1, &vaoID1);
  glGenBuffers(1, &vertBufferID1);
  glGenVertexArrays(1, &vaoID2);
  glGenBuffers(1, &vertBufferID2);
  glGenVertexArrays(1, &vaoID3);
  glGenBuffers(1, &vertBufferID3);
  glGenVertexArrays(1, &vaoID4);
  glGenBuffers(1, &vertBufferID4);
  glGenVertexArrays(1, &vaoID5);
  glGenBuffers(1, &vertBufferID5);
  glGenVertexArrays(1, &vaoID6);
  glGenBuffers(1, &vertBufferID6);


  glGenVertexArrays(1, &vaoID7);
  glGenBuffers(1, &vertBufferID7);
  glGenVertexArrays(1, &vaoID8);
  glGenBuffers(1, &vertBufferID8);

  glGenVertexArrays(1, &vaoID9);  //SKY
  glGenBuffers(1, &vertBufferID9);
  glGenVertexArrays(1, &vaoID10);  //wall
  glGenBuffers(1, &vertBufferID10);


  glGenVertexArrays(1, &line_vaoID);
  glGenBuffers(1, &line_vertBufferID);
  glGenVertexArrays(1, &line_vaoID1);
  glGenBuffers(1, &line_vertBufferID1);
  glGenVertexArrays(1, &line_vaoID2);
  glGenBuffers(1, &line_vertBufferID2);
  glGenVertexArrays(1, &line_vaoID3);
  glGenBuffers(1, &line_vertBufferID3);
}

void deleteIDs() {
  glDeleteProgram(basicProgramID);

  glDeleteVertexArrays(1, &vaoID);
  glDeleteBuffers(1, &vertBufferID);
  glDeleteVertexArrays(1, &vaoID1);
  glDeleteBuffers(1, &vertBufferID1);
  glDeleteVertexArrays(1, &vaoID2);
  glDeleteBuffers(1, &vertBufferID2);
  glDeleteVertexArrays(1, &vaoID3);
  glDeleteBuffers(1, &vertBufferID3);
  glDeleteVertexArrays(1, &vaoID4);
  glDeleteBuffers(1, &vertBufferID4);
  glGenVertexArrays(1, &vaoID5);
  glGenBuffers(1, &vertBufferID5);
  glGenVertexArrays(1, &vaoID6);
  glGenBuffers(1, &vertBufferID6);

  glGenVertexArrays(1, &vaoID7); //SUNN
  glGenBuffers(1, &vertBufferID7);
  glGenVertexArrays(1, &vaoID8); //SUNN
  glGenBuffers(1, &vertBufferID8);


  glGenVertexArrays(1, &vaoID9);  //SKY
  glGenBuffers(1, &vertBufferID9);

  glGenVertexArrays(1, &vaoID10);
  glGenBuffers(1, &vertBufferID10);


  glDeleteVertexArrays(1, &line_vaoID);
  glDeleteBuffers(1, &line_vertBufferID);

  glDeleteVertexArrays(1, &line_vaoID1);
  glDeleteBuffers(1, &line_vertBufferID1);
  glDeleteVertexArrays(1, &line_vaoID2);
  glDeleteBuffers(1, &line_vertBufferID2);
  glDeleteVertexArrays(1, &line_vaoID3);
  glDeleteBuffers(1, &line_vertBufferID3);
}

void init() {
  glEnable(GL_DEPTH_TEST);
  glPointSize(50);

  camera = Camera(Vec3f{0, 0, 20}, Vec3f{0, 0, -1}, Vec3f{0, 1, 0});

  // SETUP SHADERS, BUFFERS, VAOs

  generateIDs();
  setupVAO();
  loadQuadGeometryToGPU();
  loadQuadGeometryToGPU1();
  loadQuadGeometryToGPU2();
  loadQuadGeometryToGPU3();
  loadQuadGeometryToGPU4();
  loadQuadGeometryToGPU5();
  loadQuadGeometryToGPU6();
  loadQuadGeometryToGPU7();
  loadQuadGeometryToGPU8();

  loadQuadGeometryToGPU9();
loadQuadGeometryToGPU10();


  loadLineGeometryToGPU();
  loadLineGeometryToGPU1();
  loadLineGeometryToGPU2();
  loadLineGeometryToGPU3();
  loadModelViewMatrix();
  reloadProjectionMatrix();
  setupModelViewProjectionTransform();
  reloadMVPUniform();
}

int main(int argc, char **argv) {
  GLFWwindow *window;

  if (!glfwInit()) {
    exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window =
      glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "CSE414 Project 1", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  glfwSetWindowSizeCallback(window, windowSetSizeFunc);
  glfwSetFramebufferSizeCallback(window, windowSetFramebufferSizeFunc);
  glfwSetKeyCallback(window, windowKeyFunc);
  glfwSetCursorPosCallback(window, windowMouseMotionFunc);
  glfwSetMouseButtonCallback(window, windowMouseButtonFunc);

  glfwGetFramebufferSize(window, &WIN_WIDTH, &WIN_HEIGHT);

  // Initialize glad
  if (!gladLoadGL()) {
    std::cerr << "Failed to initialise GLAD" << std::endl;
    return -1;
  }

  std::cout << "GL Version: :" << glGetString(GL_VERSION) << std::endl;
  std::cout << GL_ERROR() << std::endl;

  // Initialize all the geometry, and load it once to the GPU
  init(); // our own initialize stuff func

  float t = 0;
  float dt = 0.01;

  while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
         !glfwWindowShouldClose(window)) {

    if (g_play) {
      t += dt;
      animateQuad(t);
    }

    displayFunc();
    moveCamera();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // clean up after loop
  deleteIDs();

  return 0;
}

//==================== CALLBACK FUNCTIONS ====================//

void windowSetSizeFunc(GLFWwindow *window, int width, int height) {
  WIN_WIDTH = width;
  WIN_HEIGHT = height;

  reloadProjectionMatrix();
  setupModelViewProjectionTransform();
  reloadMVPUniform();
}

void windowSetFramebufferSizeFunc(GLFWwindow *window, int width, int height) {
  FB_WIDTH = width;
  FB_HEIGHT = height;

  glViewport(0, 0, FB_WIDTH, FB_HEIGHT);
}

void windowMouseButtonFunc(GLFWwindow *window, int button, int action,
                           int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT) {
    if (action == GLFW_PRESS) {
      g_cursorLocked = GL_TRUE;
    } else {
      g_cursorLocked = GL_FALSE;
    }
  }
}

void windowMouseMotionFunc(GLFWwindow *window, double x, double y) {
  if (g_cursorLocked) {
    float deltaX = (x - g_cursorX) * 0.01;
    float deltaY = (y - g_cursorY) * 0.01;
    camera.rotateAroundFocus(deltaX, deltaY);

    reloadViewMatrix();
    setupModelViewProjectionTransform();
    reloadMVPUniform();
  }

  g_cursorX = x;
  g_cursorY = y;
}

void windowKeyFunc(GLFWwindow *window, int key, int scancode, int action,
                   int mods) {
  bool set = action != GLFW_RELEASE && GLFW_REPEAT;
  switch (key) {
  case GLFW_KEY_ESCAPE:
    glfwSetWindowShouldClose(window, GL_TRUE);
    break;
  case GLFW_KEY_W:
    g_moveBackForward = set ? 1 : 0;
    break;
  case GLFW_KEY_S:
    g_moveBackForward = set ? -1 : 0;
    break;
  case GLFW_KEY_A:
    g_moveLeftRight = set ? 1 : 0;
    break;
  case GLFW_KEY_D:
    g_moveLeftRight = set ? -1 : 0;
    break;
  case GLFW_KEY_Q:
    g_moveUpDown = set ? -1 : 0;
    break;
  case GLFW_KEY_E:
    g_moveUpDown = set ? 1 : 0;
    break;
  case GLFW_KEY_UP:
    g_rotateUpDown = set ? -1 : 0;
    break;
  case GLFW_KEY_DOWN:
    g_rotateUpDown = set ? 1 : 0;
    break;
  case GLFW_KEY_LEFT:
    if (mods == GLFW_MOD_SHIFT)
      g_rotateRoll = set ? -1 : 0;
    else
      g_rotateLeftRight = set ? 1 : 0;
    break;
  case GLFW_KEY_RIGHT:
    if (mods == GLFW_MOD_SHIFT)
      g_rotateRoll = set ? 1 : 0;
    else
      g_rotateLeftRight = set ? -1 : 0;
    break;
  case GLFW_KEY_SPACE:
    g_play = set ? !g_play : g_play;
    break;
  case GLFW_KEY_LEFT_BRACKET:
    if (mods == GLFW_MOD_SHIFT) {
      g_rotationSpeed *= 0.5;
    } else {
      g_panningSpeed *= 0.5;
    }
    break;
  case GLFW_KEY_RIGHT_BRACKET:
    if (mods == GLFW_MOD_SHIFT) {
      g_rotationSpeed *= 1.5;
    } else {
      g_panningSpeed *= 1.5;
    }
    break;
  default:
    break;
  }
}

//==================== OPENGL HELPER FUNCTIONS ====================//

void moveCamera() {
  Vec3f dir;

  if (g_moveBackForward) {
    dir += Vec3f(0, 0, g_moveBackForward * g_panningSpeed);
  }
  if (g_moveLeftRight) {
    dir += Vec3f(g_moveLeftRight * g_panningSpeed, 0, 0);
  }
  if (g_moveUpDown) {
    dir += Vec3f(0, g_moveUpDown * g_panningSpeed, 0);
  }

  if (g_rotateUpDown) {
    camera.rotateUpDown(g_rotateUpDown * g_rotationSpeed);
  }
  if (g_rotateLeftRight) {
    camera.rotateLeftRight(g_rotateLeftRight * g_rotationSpeed);
  }
  if (g_rotateRoll) {
    camera.rotateRoll(g_rotateRoll * g_rotationSpeed);
  }

  if (g_moveUpDown || g_moveLeftRight || g_moveBackForward ||
      g_rotateLeftRight || g_rotateUpDown || g_rotateRoll) {
    camera.move(dir);
    reloadViewMatrix();
    setupModelViewProjectionTransform();
    reloadMVPUniform();
  }
}

std::string GL_ERROR() {
  GLenum code = glGetError();

  switch (code) {
  case GL_NO_ERROR:
    return "GL_NO_ERROR";
  case GL_INVALID_ENUM:
    return "GL_INVALID_ENUM";
  case GL_INVALID_VALUE:
    return "GL_INVALID_VALUE";
  case GL_INVALID_OPERATION:
    return "GL_INVALID_OPERATION";
  case GL_INVALID_FRAMEBUFFER_OPERATION:
    return "GL_INVALID_FRAMEBUFFER_OPERATION";
  case GL_OUT_OF_MEMORY:
    return "GL_OUT_OF_MEMORY";
  default:
    return "Non Valid Error Code";
  }
}
