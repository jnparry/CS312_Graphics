#define SDL_MAIN_HANDLED

#include "SDL2/SDL.h"
#include "gl.h"

// g++ gl.cpp -framework OpenGl -lglew -lSDL2
// g++ gl.cpp 0lopengl32 -lglew32 -lSDL2 - old one

int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window * win = SDL_CreateWindow("OpenGL Program", 200, 200, SCREEN_W, SCREEN_H, SDL_WINDOW_OPENGL);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetSwapInterval(1);
    SDL_GLContext cont = SDL_GL_CreateContext(win);

    glewInit();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilMask(0x0);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    bool success = true;
    string vertexShader;
    string billboardShader;
    string fragmentShader;

    int vertexHandle;
    int billboardHandle;
    int fragmentHandle;
    int programHandle;
    int imgHandle;

    success &= parseFile((char*)"vertex.vs", vertexShader);
    success &= parseFile((char*)"billboard.vs", billboardShader);
    success &= parseFile((char*)"fragment.fs", fragmentShader);
    success &= compileShader(vertexShader.c_str(), GL_VERTEX_SHADER, vertexHandle);
    success &= compileShader(billboardShader.c_str(), GL_VERTEX_SHADER, billboardHandle);
    success &= compileShader(fragmentShader.c_str(), GL_FRAGMENT_SHADER, fragmentHandle);
    success &= compileProgram(billboardHandle, fragmentHandle, programHandle);
    // success &= compileProgram(vertexHandle, fragmentHandle, programHandle);
    success &= loadTexture("checker.bmp", imgHandle);

    /***************************************************
     * OBJECT LOADER
     **************************************************/
    vector <material> materials;
    vector <material> materials2;
    vector <vertexData> vertexBuffer;
    vector <vertexData> vertexBuffer2;

    bool hasUV;
    bool hasUV2;
    bool hasNormal;
    bool hasNormal2;

    success &= getObjData("bunny.obj", materials, vertexBuffer, hasUV, hasNormal);
    success &= getObjData("bunny2.obj", materials2, vertexBuffer2, hasUV2, hasNormal2);
    
    // materials[0].map_Kd[0]  = 'r';
    // materials[0].map_Kd[1]  = 'g';
    // materials[0].map_Kd[2]  = 'w';
    // materials[0].map_Kd[3]  = 'h';
    // materials[0].map_Kd[4]  = 'i';
    // materials2[0].map_Kd[5]  = 'e';
    // materials2[0].map_Kd[6]  = 'r';
    // materials2[0].map_Kd[7]  = '.';
    // materials2[0].map_Kd[8]  = 'b';
    // materials2[0].map_Kd[9]  = 'm';
    // materials2[0].map_Kd[10] = 'p';

    // build out a single array of floats
    int stride = 3 + (2 * hasUV) + (3 * hasNormal);
    int vertexBufferNumBytes = stride * vertexBuffer.size() * sizeof(float);
    float * vertexBufferData = (float*)(malloc(vertexBufferNumBytes));

    // build out a single array of floats
    int stride2 = 3 + (2 * hasUV2) + (3 * hasNormal2);
    int vertexBufferNumBytes2 = stride2 * vertexBuffer2.size() * sizeof(float);
    float * vertexBufferData2 = (float*)(malloc(vertexBufferNumBytes2));
    
    int i  = 0;
    int i2 = 0;
    // join data into an interleaved buffer
    for (int vb = 0; vb < vertexBuffer.size(); vb++)
    {
        vertexBufferData[i++] = vertexBuffer[vb].vert[0];
        vertexBufferData[i++] = vertexBuffer[vb].vert[1];
        vertexBufferData[i++] = vertexBuffer[vb].vert[2];

        if (hasUV)
        {
            vertexBufferData[i++] = vertexBuffer[vb].uv[0];
            vertexBufferData[i++] = vertexBuffer[vb].uv[1];
        }

        if (hasNormal)
        {
            vertexBufferData[i++] = vertexBuffer[vb].normal[0];
            vertexBufferData[i++] = vertexBuffer[vb].normal[1];
            vertexBufferData[i++] = vertexBuffer[vb].normal[2];
        }
    }

    for (int vb2 = 0; vb2 < vertexBuffer2.size(); vb2++)
    {
        vertexBufferData2[i2++] = vertexBuffer2[vb2].vert[0];
        vertexBufferData2[i2++] = vertexBuffer2[vb2].vert[1];
        vertexBufferData2[i2++] = vertexBuffer2[vb2].vert[2];

        if (hasUV2)
        {
            vertexBufferData2[i2++] = vertexBuffer2[vb2].uv[0];
            vertexBufferData2[i2++] = vertexBuffer2[vb2].uv[1];
        }

        if (hasNormal2)
        {
            vertexBufferData2[i2++] = vertexBuffer2[vb2].normal[0];
            vertexBufferData2[i2++] = vertexBuffer2[vb2].normal[1];
            vertexBufferData2[i2++] = vertexBuffer2[vb2].normal[2];
        }
    }

    vector <int> textureIDs;
    for (int mat = 0; mat < materials.size(); mat++)
    {
        int tmp;
        material m = materials[mat]; // material m;
        success &= loadTexture(m.map_Kd, tmp);
        textureIDs.push_back(tmp);
    }

    vector <int> textureIDs2;
    for (int mat = 0; mat < materials2.size(); mat++)
    {
        int tmp;
        material m = materials2[mat];
        success &= loadTexture(m.map_Kd, tmp);
        textureIDs2.push_back(tmp);
    }

    validate(success, (char*)"Setup OpenGL Program");

    /***************************************************
     * VERTEX BUFFER OBJECT
     **************************************************/
    int VBO; // vertex buffer obejct
    glGenBuffers(1, (GLuint*)&VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexBufferNumBytes, vertexBufferData, GL_STATIC_DRAW);

    int VBO2; // vertex buffer obejct
    glGenBuffers(1, (GLuint*)&VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, vertexBufferNumBytes2, vertexBufferData2, GL_STATIC_DRAW);

    /***************************************************
     * ATTRIBUTE HANDLES
     **************************************************/
    int aPositionHandle = glGetAttribLocation(programHandle, "a_Position");
    int aUVHandle       = glGetAttribLocation(programHandle, "a_UV");
    int aNormHandle     = glGetAttribLocation(programHandle, "a_Normal");

    /***************************************************
     * UNIFORMS HANDLES
     **************************************************/
    int uModelHandle      = glGetUniformLocation(programHandle, "u_Model");
    int uViewHandle       = glGetUniformLocation(programHandle, "u_View");
    int uProjHandle       = glGetUniformLocation(programHandle, "u_Proj");

    int uTextureHandle     = glGetUniformLocation(programHandle, "u_Texture");
    int uThresholdHandle   = glGetUniformLocation(programHandle, "u_Threshold");
    int uLightPosHandle    = glGetUniformLocation(programHandle, "u_LightPos");
    int uLightColorHandle  = glGetUniformLocation(programHandle, "u_LightColor");
    int uLightNormalHandle = glGetUniformLocation(programHandle, "u_LightNormal");

    int uKaHandle  = glGetUniformLocation(programHandle, "u_Ka");
    int uKdHandle  = glGetUniformLocation(programHandle, "u_Kd");
    int uKsHandle  = glGetUniformLocation(programHandle, "u_Ks");
    int uNsHandle  = glGetUniformLocation(programHandle, "u_Ns");
    int uCamHandle = glGetUniformLocation(programHandle, "u_CamPos");

    int scaleHandle = glGetUniformLocation(programHandle, "scaleVal");
    int cylHandle   = glGetUniformLocation(programHandle, "cyl");

    // MVP matrix
    mat4 model;
    mat4 view;
    mat4 proj;

    float lightPos[]    = {0, 10, 0};
    float lightColor[]  = {1.0, 1.0, 1.0};
    float lightNormal[] = {0, -1, -1};

    // camera data
    myCam.camX = myCam.camY = myCam.camZ = myCam.pitch = myCam.yaw = myCam.roll = 0.0;

    int numDraw = vertexBuffer.size();
    int numDraw2 = vertexBuffer2.size();
    bool running = true;

    float scaleValue = 1.01;
    bool black = true;
    bool cylindrical = false; 
    bool grow = false;
    // Main Draw Loop
    while (running)
    {
        processUserInputs(running, cylindrical, grow);
        {
            // isolate the gl functions
            // clear buffers
            // glClearColor(0, 0, 0, 1.0);
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            glUseProgram(programHandle);

            /***************************************************
             * SET UP ATTRIBUTES
             **************************************************/
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glVertexAttribPointer(aPositionHandle, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);
            glEnableVertexAttribArray(aPositionHandle);

            glVertexAttribPointer(aUVHandle, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(0 + 3 * sizeof(float)));
            glEnableVertexAttribArray(aUVHandle);

            glVertexAttribPointer(aNormHandle, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(0 + 5 * sizeof(float)));
            glEnableVertexAttribArray(aNormHandle);

            /***************************************************
             * SET UP UNIFORMS
             **************************************************/
            float camPositions[] = {myCam.camX, myCam.camY, myCam.camZ};

            // update texture
            glActiveTexture(GL_TEXTURE0 + 0);
            glBindTexture(GL_TEXTURE_2D, textureIDs[0]);

            glUniform1i(uTextureHandle, 0);
            glUniform1i(uNsHandle, materials[0].Ns);
            glUniform1f(uThresholdHandle, threshold);

            // Settings up lighting
            glUniform3fv(uLightPosHandle, 1, lightPos);
            glUniform3fv(uLightColorHandle, 1, lightColor);
            glUniform3fv(uLightNormalHandle, 1, lightNormal);

            glUniform3fv(uKaHandle,  1, materials[0].Ka);
            glUniform3fv(uKdHandle,  1, materials[0].Kd);
            glUniform3fv(uKsHandle,  1, materials[0].Ks);
            glUniform3fv(uCamHandle, 1, camPositions);

            if (grow)
            {    
                if (!black)
                {
                    if (scaleValue < 1.15)
                        scaleValue += 0.001;
                }
                else
                {
                    if (scaleValue > 1.01)
                        scaleValue -= 0.001;
                }
            }

            glUniform1f(scaleHandle, scaleValue);
            glUniform1i(cylHandle, cylindrical);

            setupMVP(model, view, proj);
            glUniformMatrix4fv(uModelHandle, 1, false, &model[0][0]);
            glUniformMatrix4fv(uViewHandle,  1, false, &view[0][0]);
            glUniformMatrix4fv(uProjHandle,  1, false, &proj[0][0]);

            // glClear(GL_STENCIL_BUFFER_BIT);
            // glClearStencil(0x00);
            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glStencilMask(0xFF); // 0xFF writes to the buffer, sets 0s
            
            glDrawArrays(GL_TRIANGLES, 0, numDraw);

            /**************************************************************
             * 2nd Render Pass
             * 
             *************************************************************/
            glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
            glStencilMask(0x00); 
            glDisable(GL_DEPTH_TEST);

            glActiveTexture(GL_TEXTURE0 + 0);
            glBindTexture(GL_TEXTURE_2D, textureIDs2[0]);

            glUniform1i(uTextureHandle, 0);
            glUniform1i(uNsHandle, materials2[0].Ns);
            glUniform1f(uThresholdHandle, threshold);

            // // Settings up lighting
            glUniform3fv(uLightPosHandle, 1, lightPos);
            glUniform3fv(uLightColorHandle, 1, lightColor);
            glUniform3fv(uLightNormalHandle, 1, lightNormal);

            glUniform3fv(uKaHandle,  1, materials2[0].Ka);
            glUniform3fv(uKdHandle,  1, materials2[0].Kd);
            glUniform3fv(uKsHandle,  1, materials2[0].Ks);

            glUniform1f(scaleHandle, 40.0);
            // scaleValue

            // glUniform3fv(uCamHandle, 1, camPositions);

            setupMVPScale(model, view, proj, 1.1);
            glUniformMatrix4fv(uModelHandle, 1, false, &model[0][0]);
            glUniformMatrix4fv(uViewHandle,  1, false, &view[0][0]);
            glUniformMatrix4fv(uProjHandle,  1, false, &proj[0][0]);

            // glDrawArrays(GL_TRIANGLES, 0, numDraw2);
            glStencilMask(0xFF);
            glEnable(GL_DEPTH_TEST);

            glFlush();
            glFinish();
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            unsigned char data[4];
            int thex, they;
            SDL_GetMouseState(&thex, &they);
            glReadPixels(thex, they, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
            int pickedID = data[0] * 256 + data[1] * 256 + data[2] * 256;
            if (pickedID != 19200)
                black = false;
            else
                black = true;

            GLint viewport[4]; //var to hold the viewport info
            GLdouble modelview[16]; //var to hold the modelview info
            GLdouble projection[16]; //var to hold the projection matrix info
            GLfloat winX, winY, winZ; //variables to hold screen x,y,z coordinates
            GLdouble worldX, worldY, worldZ; //variables to hold woqrld x,y,z coordinates

            glGetDoublev(GL_MODELVIEW_MATRIX, modelview); //get the modelview info
            glGetDoublev(GL_PROJECTION_MATRIX, projection); //get the projection matrix info
            glGetIntegerv(GL_VIEWPORT, viewport); //get the viewport info

            winX = (float)thex;
            winY = (float)they;
            winZ = 0;
            gluUnProject( winX, winY, winZ, modelview, projection, viewport, &worldX, &worldY, &worldZ);
            std::cout << "(" << worldX << ", " << worldY << ", " << worldZ << ")\n";
        }

        SDL_GL_SwapWindow(win);
    }

    free(vertexBufferData);
    return 0;
}