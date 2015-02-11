/*
 *  OpenGL 3 & 4 Shaders
 *
 *  Demonstrates the use of OpenGL 4 style shaders
 *  Mixes fixed and programmable pipelines
 *  Uses OpenGL to determine matrices
 *
 *  Hint:  If you cannot get this program to compile on your machine because
 *  OpenGL 4.3 is not supported, try changing all the 4.3 references to 3.3 in
 *  the shaders as well as when starting GLEW
 *
 *  Key bindings:
 *  m/M        Cycle through shaders
 *  p/P        Toggle between orthogonal & perspective projection
 *  s/S        Start/stop light
 *  -/+        Change light elevation
 *  a          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 */
#include "CSCIx239.h"
#include "glm/glm.hpp"
// #include "glm/gtc/matrix_projection.hpp"
// glm::translate, glm::rotate, glm::scale
#include "glm/gtc/matrix_transform.hpp"
 #include "glm/gtc/type_ptr.hpp"

int axes=1;       //  Display axes
int mode=0;       //  Shader mode
int move=1;       //  Move light
int proj=1;       //  Projection type
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
float fov=55;     //  Field of view (for perspective)
float asp=1;      //  Aspect ratio
float dim=3.0;    //  Size of world
int zh=90;        //  Light azimuth
float Ylight=2;   //  Light elevation
int crate;        //  Crate texture
int shader;       //  Shader programs
const char* text[] = {"Fixed Pipeline","Programmable Pipeline"};

// Tranformation matrix data
int transfBindingPoint = 1;
unsigned int transfBuffer;

//Light data
int lightBindingPoint = 2;
unsigned int lightBuffer;
const float lightData[] =
{
   //Position
   2, 0, 0, 1,
   //Global
   0.3, 0.3, 0.3, 1,
   //Ambient
   0.3, 0.3, 0.3, 1.0,
   // Diffuse
   1.0, 1.0, 1.0, 1.0,
   // Specular
   1.0, 1.0, 1.0, 1.0
};

//
//  Cube Vertexes
//
const int cube_size=36;
unsigned int cube_buffer;  // Vertex buffer object
const float cube_data[] =  // Vertex data
{
//  X  Y  Z  W   Nx Ny Nz    R G B   s t
   //  Front
   +1,+1,+1,+1,   0, 0,+1,   1,0,0,  1,1,
   -1,+1,+1,+1,   0, 0,+1,   1,0,0,  0,1,
   +1,-1,+1,+1,   0, 0,+1,   1,0,0,  1,0,
   -1,+1,+1,+1,   0, 0,+1,   1,0,0,  0,1,
   +1,-1,+1,+1,   0, 0,+1,   1,0,0,  1,0,
   -1,-1,+1,+1,   0, 0,+1,   1,0,0,  0,0,
   //  Back
   -1,-1,-1,+1,   0, 0,-1,   0,0,1,  1,0,
   +1,-1,-1,+1,   0, 0,-1,   0,0,1,  0,0,
   -1,+1,-1,+1,   0, 0,-1,   0,0,1,  1,1,
   +1,-1,-1,+1,   0, 0,-1,   0,0,1,  0,0,
   -1,+1,-1,+1,   0, 0,-1,   0,0,1,  1,1,
   +1,+1,-1,+1,   0, 0,-1,   0,0,1,  0,1,
   //  Right
   +1,+1,+1,+1,  +1, 0, 0,   1,1,0,  0,1,
   +1,-1,+1,+1,  +1, 0, 0,   1,1,0,  0,0,
   +1,+1,-1,+1,  +1, 0, 0,   1,1,0,  1,1,
   +1,-1,+1,+1,  +1, 0, 0,   1,1,0,  0,0,
   +1,+1,-1,+1,  +1, 0, 0,   1,1,0,  1,1,
   +1,-1,-1,+1,  +1, 0, 0,   1,1,0,  1,0,
   //  Left
   -1,+1,+1,+1,  -1, 0, 0,   0,1,0,  1,1,
   -1,+1,-1,+1,  -1, 0, 0,   0,1,0,  0,1,
   -1,-1,+1,+1,  -1, 0, 0,   0,1,0,  1,0,
   -1,+1,-1,+1,  -1, 0, 0,   0,1,0,  0,1,
   -1,-1,+1,+1,  -1, 0, 0,   0,1,0,  1,0,
   -1,-1,-1,+1,  -1, 0, 0,   0,1,0,  0,0,
   //  Top
   +1,+1,+1,+1,   0,+1, 0,   0,1,1,  1,0,
   +1,+1,-1,+1,   0,+1, 0,   0,1,1,  1,1,
   -1,+1,+1,+1,   0,+1, 0,   0,1,1,  0,0,
   +1,+1,-1,+1,   0,+1, 0,   0,1,1,  1,1,
   -1,+1,+1,+1,   0,+1, 0,   0,1,1,  0,0,
   -1,+1,-1,+1,   0,+1, 0,   0,1,1,  0,1,
   //  Bottom
   -1,-1,-1,+1,   0,-1, 0,   1,0,1,  0,0,
   +1,-1,-1,+1,   0,-1, 0,   1,0,1,  1,0,
   -1,-1,+1,+1,   0,-1, 0,   1,0,1,  0,1,
   +1,-1,-1,+1,   0,-1, 0,   1,0,1,  1,0,
   -1,-1,+1,+1,   0,-1, 0,   1,0,1,  0,1,
   +1,-1,+1,+1,   0,-1, 0,   1,0,1,  1,1,
   };

const int shere_size = 42;
unsigned int sphereBuffer;
const float sphere_data[] =  // Vertex data
{
//  X  Y  Z  W   Nx Ny Nz 
   //  Front
   +1,+1,+1,+1,   +1,+1,+1,
   -1,+1,+1,+1,   -1,+1,+1,
   +0,+0,+sqrt(3),+1,   +0,+0,+1,

   +1,+1,+1,+1,   +1,+1,+1,
   +0,+0,+sqrt(3),+1,   +0,+0,+1,
   +1,-1,+1,+1,   +1,-1,+1,

   -1,+1,+1,+1,   -1,+1,+1,
   +0,+0,+sqrt(3),+1,   +0,+0,+1,
   -1,-1,+1,+1,   -1,-1,+1,

   +0,+0,+sqrt(3),+1,   +0,+0,+1,
   +1,-1,+1,+1,   +1,-1,+1,
   -1,-1,+1,+1,   -1,-1,+1,
   //  Back
   -1,-1,-1,+1,   -1,-1,-1,
   +1,-1,-1,+1,   +1,-1,-1,
   -1,+1,-1,+1,   -1,+1,-1,
   +1,-1,-1,+1,   +1,-1,-1,
   -1,+1,-1,+1,   -1,+1,-1,
   +1,+1,-1,+1,   +1,+1,-1,
   //  Right
   +1,+1,+1,+1,  +1,+1,+1,
   +1,-1,+1,+1,  +1,-1,+1,
   +1,+1,-1,+1,  +1,+1,-1,
   +1,-1,+1,+1,  +1,-1,+1,
   +1,+1,-1,+1,  +1,+1,-1,
   +1,-1,-1,+1,  +1,-1,-1,
   //  Left
   -1,+1,+1,+1,  -1,+1,+1,
   -1,+1,-1,+1,  -1,+1,-1,
   -1,-1,+1,+1,  -1,-1,+1,
   -1,+1,-1,+1,  -1,+1,-1,
   -1,-1,+1,+1,  -1,-1,+1,
   -1,-1,-1,+1,  -1,-1,-1,
   //  Top
   +1,+1,+1,+1,   +1,+1,+1,
   +1,+1,-1,+1,   +1,+1,-1,
   -1,+1,+1,+1,   -1,+1,+1,
   +1,+1,-1,+1,   +1,+1,-1,
   -1,+1,+1,+1,   -1,+1,+1,
   -1,+1,-1,+1,   -1,+1,-1,
   //  Bottom
   -1,-1,-1,+1,   -1,-1,-1,
   +1,-1,-1,+1,   +1,-1,-1,
   -1,-1,+1,+1,   -1,-1,+1,
   +1,-1,-1,+1,   +1,-1,-1,
   -1,-1,+1,+1,   -1,-1,+1,
   +1,-1,+1,+1,   +1,-1,+1,
   };

//
//  Cube vertex buffer object
//
void InitCube()
{
   //  Copy data to vertex buffer object
   glGenBuffers(1,&cube_buffer);
   glBindBuffer(GL_ARRAY_BUFFER,cube_buffer);
   glBufferData(GL_ARRAY_BUFFER,sizeof(cube_data),cube_data,GL_STATIC_DRAW);

   //  Unbind this buffer
   glBindBuffer(GL_ARRAY_BUFFER,0);
}

void InitSpheere()
{
   //  Copy data to vertex buffer object
   glGenBuffers(1, &sphereBuffer);
   glBindBuffer(GL_ARRAY_BUFFER, sphereBuffer);
   glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_data), sphere_data, GL_DYNAMIC_DRAW);

   //  Unbind buffer
   glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void DrawSphere() 
{
   //  Select cube buffer
   glBindBuffer(GL_ARRAY_BUFFER,sphereBuffer);
   //   Attribute 0: vertex coordinate (vec4) at offset 0
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,7*sizeof(float),(void*)0);
   //   Attribute 2: vertex normal (vec3) at offset 4
   glEnableVertexAttribArray(1);
   glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void*)(4*sizeof(float)));      

   // Draw the cube
   glDrawArrays(GL_TRIANGLES,0,shere_size);

   //  Disable vertex arrays
   glDisableVertexAttribArray(0);
   glDisableVertexAttribArray(1);

   //  Unbind this buffer
   glBindBuffer(GL_ARRAY_BUFFER,0);
}

void UpdateTranformations();
//
//
//
void InitTranformations() 
{
   int transfBlockId = glGetUniformBlockIndex(shader, "Tranformations");
   if (transfBlockId >= 0)
   {
      // Set the biding point of the uniform
      glUniformBlockBinding(shader, transfBlockId, transfBindingPoint);

      // Generate buffer
      glGenBuffers(1, &transfBuffer);
      glBindBuffer(GL_UNIFORM_BUFFER, transfBuffer);

      // Bind buffer to biding point
      float data[48] = {0};
      glBufferData(GL_UNIFORM_BUFFER, sizeof(data), data, GL_DYNAMIC_DRAW);
      glBindBufferBase(GL_UNIFORM_BUFFER, transfBindingPoint, transfBuffer);

      // Initialize
      UpdateTranformations();
   }
}

//
//
//
void UpdateTranformations()
{
   // Bind buffer
   glBindBuffer(GL_UNIFORM_BUFFER, transfBuffer);

   // Update projection matrix
   glm::mat4 projectionMatrix = glm::perspective<float>(M_PI*fov/180.0, asp, dim/16.0, 16.0*dim);
   glBufferSubData(GL_UNIFORM_BUFFER, 0, 16*sizeof(float), glm::value_ptr(projectionMatrix));

   // Update modelView Matrix 
   float Ex = -2*dim*Sin(th)*Cos(ph);
   float Ey = +2*dim        *Sin(ph);
   float Ez = +2*dim*Cos(th)*Cos(ph);
   glm::mat4 viewMatrix = glm::lookAt(glm::vec3(Ex, Ey, Ez), glm::vec3(0), glm::vec3(0,Cos(ph),0));
   glBufferSubData(GL_UNIFORM_BUFFER, 16*sizeof(float), 16*sizeof(float), glm::value_ptr(viewMatrix));

   // Update projectionModelView matrix
   glBufferSubData(GL_UNIFORM_BUFFER, 32*sizeof(float), 16*sizeof(float), glm::value_ptr(projectionMatrix * viewMatrix));

   // Unbind buffer
   glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

//
//
//
void InitLights()

{
   int lightBlockId = glGetUniformBlockIndex(shader, "Light");
   if (lightBlockId >= 0)
   {
      // Set the binding point
      glUniformBlockBinding(shader, lightBlockId, lightBindingPoint);

      // Generate buffer
      glGenBuffers(1, &lightBuffer);
      glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);

      // Copy data to buffer
      glBufferData(GL_UNIFORM_BUFFER, sizeof(lightData), lightData, GL_DYNAMIC_DRAW);
      glBindBufferBase(GL_UNIFORM_BUFFER, lightBindingPoint, lightBuffer);

      // Unbind buffer
      glBindBuffer(GL_UNIFORM_BUFFER, 0);
   }
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);

   //  Draw light position as sphere
   //  This uses the fixed pipeline
   // glColor3f(1,1,1);
   // glPushMatrix();
   // glTranslated(Position[0],Position[1],Position[2]);
   // glutSolidSphere(0.03,10,10);
   // glPopMatrix();

   //  OpenGL 4 style shaders
   // Use our shader
   glUseProgram(shader);

   // Activate texture
   glActiveTexture(GL_TEXTURE0);
   glUniform1i(glGetUniformLocation(shader, "text"), 0 );
   glBindTexture(GL_TEXTURE_2D,crate);


   // Set current light position
   float Position[]  = {(float)(2*Cos(zh)),Ylight,(float)(2*Sin(zh)),1.0};
   glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
   glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Position), Position);
   glBindBuffer(GL_UNIFORM_BUFFER, 0);


   // //  Select cube buffer
   // glBindBuffer(GL_ARRAY_BUFFER,cube_buffer);
   // //   Attribute 0: vertex coordinate (vec4) at offset 0
   // glEnableVertexAttribArray(0);
   // glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,12*sizeof(float),(void*)0);
   // //   Attribute 2: vertex normal (vec3) at offset 4
   // glEnableVertexAttribArray(1);
   // glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, 12*sizeof(float), (void*)(4*sizeof(float)));      
   // //   Attribute 2:  vertex color (vec3) offset 7 floats
   // glEnableVertexAttribArray(2);
   // glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,12*sizeof(float),(void*)(7*sizeof(float)));
   // //   Attribute 3: vertex texture coordinate (vec2) offset 10
   // glEnableVertexAttribArray(3);
   // glVertexAttribPointer(3,2, GL_FLOAT, GL_FALSE, 12*sizeof(float), (void*)(10*sizeof(float)));

   // // Draw the cube
   // glDrawArrays(GL_TRIANGLES,0,cube_size);

   // //  Disable vertex arrays
   // glDisableVertexAttribArray(0);
   // glDisableVertexAttribArray(1);
   // glDisableVertexAttribArray(2);
   // glDisableVertexAttribArray(3);

   // //  Unbind this buffer
   // glBindBuffer(GL_ARRAY_BUFFER,0);

   DrawSphere();

   //  Display parameters
   glWindowPos2i(5,5);
   Print("FPS=%d Dim=%.1f Projection=%s Mode=%s",
     FramesPerSecond(),dim,proj?"Perpective":"Orthogonal",text[mode]);
   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   if (move) zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_DOWN)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP && dim>1)
      dim -= 0.1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   UpdateTranformations();
   // Project(proj?fov:0,asp,dim);
   // glm::mat4 projectionMatrix = glm::perspective<float>(M_PI*(55.0)/180.0, asp, dim/16,16*dim);
   // // Bind buffer
   // glBindBuffer(GL_UNIFORM_BUFFER, transfBuffer);
   // glBufferSubData(GL_UNIFORM_BUFFER, 16*sizeof(float), 16*sizeof(float), glm::value_ptr(projectionMatrix));
   // glBindBuffer(GL_UNIFORM_BUFFER, 0);

   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Toggle axes
   else if (ch == 'a' || ch == 'A')
      axes = 1-axes;
   //  Toggle projection type
   else if (ch == 'p' || ch == 'P')
      proj = 1-proj;
   //  Toggle light movement
   else if (ch == 's' || ch == 'S')
      move = 1-move;
   //  Cycle modes
   else if (ch == 'm' || ch == 'M')
      mode = 1-mode;
   //  Light elevation
   else if (ch == '+')
      Ylight += 0.1;
   else if (ch == '-')
      Ylight -= 0.1;
   //  Light position
   else if (ch == '[')
      zh--;
   else if (ch == ']')
      zh++;
   //  Reproject
   UpdateTranformations();
   // Project(proj?fov:0,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   UpdateTranformations();
   // Project(proj?fov:0,asp,dim);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("OpenGL 4");
#ifdef USEGLEW
   //  Initialize GLEW
   if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
   if (!GLEW_VERSION_4_3) Fatal("OpenGL 4.3 not supported\n");
#endif
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   //  Load crate
   crate = LoadTexBMP("pi.bmp");
   //  Create Shader Programs
   shader = CreateShaderProg("gl430.vert","gl430.frag");
   //  Initialize cube
   InitCube();
   InitSpheere();
   //  Initialize lights
   InitLights();
   //  Initialize tranfomations
   InitTranformations();
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
