#include "iostream"
#include "definitions.h"
#include "shaders.h"

#ifndef COURSE_FUNCTIONS_H
#define COURSE_FUNCTIONS_H

/***************************************************
 * Team Activity for week #1.
 * When working on this activity be sure to 
 * comment out the following function calls in 
 * pipeline.cpp:main():
 *      1) processUserInputs(running);
 *      2) clearScreen(frame);
 *      3) Any draw calls that are being made there
 * 
 * When you finish this activity be sure to 
 * uncomment these functions again!!!
 **************************************************/
void GameOfLife(Buffer2D<PIXEL> & target)
{
        // 'Static's are initialized exactly once
        static bool isSetup = true;
        static bool holdDown = false;
        static int w = target.width();
        static int h = target.height();
        static int scaleFactor = 8;
        static int gridW = 64;
        static int gridH = 64; 
        static int grid[64][64];
        static int gridTmp[64][64];

        // Setup small grid, temporary grid from previous iteration
        for(int y = 0; y < gridH; y++)
        {
                for(int x = 0; x < gridW; x++)
                {
                        grid[y][x] = (target[y*scaleFactor][x*scaleFactor] == 0xffff0000) ? 1 : 0;
                        gridTmp[y][x] = grid[y][x];
                }
        }

        //Parse for inputs
        SDL_Event e;
        while(SDL_PollEvent(&e)) 
        {
                int mouseX;
                int mouseY;
                if(e.type == SDL_MOUSEBUTTONDOWN)
                {
                        holdDown = true;
                }
                if(e.type == SDL_MOUSEBUTTONUP)
                {
                        holdDown = false;
                }
                if(e.key.keysym.sym == 'g' && e.type == SDL_KEYDOWN) 
                {
                        isSetup = !isSetup;
                }
                if(holdDown && isSetup)
                {
                        // Clicking the mouse changes a pixel's color
                        SDL_GetMouseState(&mouseX, &mouseY);
                        mouseY = S_HEIGHT - mouseY;
                        int gridX = mouseX / scaleFactor;
                        int gridY = mouseY / scaleFactor;
                        if(grid[gridY][gridX] == 1)
                        {
                                // Dead
                                grid[gridY][gridX] = 0;
                        }
                        else
                        {
                                // Alive
                                grid[gridY][gridX] = 1;
                        }
                }
        }


        // Advance the simulation after pressing 'g'
        if(!isSetup)
        {
                // Your Code goes here
                for(int y = 0; y < gridH; y++)
                {
                        for(int x = 0; x < gridW; x++)
                        {       
                                int numNeighbors = 0;

                                // get number of neighbors
                                if (grid[y - 1][x] == 1) // directly below
                                        numNeighbors++;
                                if (grid[y + 1][x] == 1) // directly above
                                        numNeighbors++;
                                if (grid[y][x - 1] == 1) // directly left
                                        numNeighbors++;
                                if (grid[y][x + 1] == 1) // directly right
                                        numNeighbors++;
                                if (grid[y + 1][x - 1] == 1) // top left
                                        numNeighbors++;
                                if (grid[y + 1][x + 1] == 1) // top right
                                        numNeighbors++;
                                if (grid[y - 1][x - 1] == 1) // bottom left
                                        numNeighbors++;
                                if (grid[y - 1][x + 1] == 1) // bottom right
                                        numNeighbors++;

                                if (grid[y][x] == 1) // if alive
                                {
                                        if (numNeighbors < 2) // if 1 or 0 neighbors -> die
                                                gridTmp[y][x] = 0;
                                        else if (numNeighbors > 3) // if 4+ neighbors -> die
                                                gridTmp[y][x] = 0;
                                }
                                else // if dead
                                {
                                        if (numNeighbors == 3) // if 3 neighbors -> alive
                                                gridTmp[y][x] = 1;
                                }
                        }
                }

                for(int y = 0; y < gridH; y++)
                {
                        for(int x = 0; x < gridW; x++)
                        {
                                grid[y][x] = gridTmp[y][x];
                        }
                }

                // Wait a half-second between iterations
                SDL_Delay(500);
        }


        // Upscale/blit to screen
        for(int y = 0; y < h; y++)
        {
                for(int x = 0; x < w; x++)
                {
                        int yScal = y/scaleFactor;
                        int xScal = x/scaleFactor;
                        if(grid[yScal][xScal] == 0)
                        {
                                // Dead Color
                                target[y][x] = 0xff000000;
                        }
                        else
                        {
                                // Alive color
                                target[y][x] = 0xffff0000;
                        }
                }
        }
}

/***************************************************
 * Create a 3D View like in a CAD program
 * NOTE: Assumes that the resolution is an even 
 * value in both dimensions.
 **************************************************/
void CADView(Buffer2D<PIXEL> & target)
{
        // Each CAD Quadrant
        static int halfWid = target.width()/2;
        static int halfHgt = target.height()/2;
        static Buffer2D<PIXEL> topLeft(halfWid, halfHgt);
        static Buffer2D<PIXEL> topRight(halfWid, halfHgt);
        static Buffer2D<PIXEL> botLeft(halfWid, halfHgt);
        static Buffer2D<PIXEL> botRight(halfWid, halfHgt);


        // Your code goes here 
        // Feel free to copy from other test functions to get started!


        // Blit four panels to target
        int yStartSrc = 0;
        int xStartSrc = 0;
        int yLimitSrc = topLeft.height();
        int xLimitSrc = topLeft.width();
        for(int ySrc = yStartSrc; ySrc < yLimitSrc; ySrc++)
        {
                for(int xSrc = xStartSrc; xSrc < xLimitSrc; xSrc++)
                {
                        target[ySrc][xSrc]                 = botLeft[ySrc][xSrc];
                        target[ySrc][xSrc+halfWid]         = botRight[ySrc][xSrc];
                        target[ySrc+halfHgt][xSrc]         = topLeft[ySrc][xSrc];
                        target[ySrc+halfHgt][xSrc+halfWid] = topRight[ySrc][xSrc];
                }
        }
}

/***************************************************
 * Demonstrate pixel drawing for project 01.
 **************************************************/
void TestDrawPixel(Buffer2D<PIXEL> & target)
{
        Vertex vert = {10, 502, 1, 1};
        Attributes pointAttributes;
        PIXEL color = 0xffff0000;
        pointAttributes.color = color;      

        DrawPrimitive(POINT, target, &vert, &pointAttributes);
}

/***********************************************
 * Demonstrate Triangle Drawing for Project 02. 
 **********************************************/
void TestDrawTriangle(Buffer2D<PIXEL> & target)
{
        /**************************************************
        * 6 Flat color triangles below
        *************************************************/

        // top left triangle
        Vertex verts[3];
        Attributes attr[3];
        verts[0] = (Vertex){100, 362, 1, 1};
        verts[1] = (Vertex){150, 452, 1, 1};
        verts[2] = (Vertex){50, 452, 1, 1};
        PIXEL colors1[3] = {0xffff0000, 0xffff0000, 0xffff0000};
        
        for (int i = 0; i < 3; i++)
                attr[i].color = colors1[i];

        DrawPrimitive(TRIANGLE, target, verts, attr);

        // top middle triangle
        verts[0] = (Vertex){300, 402, 1, 1};
        verts[1] = (Vertex){250, 452, 1, 1};
        verts[2] = (Vertex){250, 362, 1, 1};
        PIXEL colors2[3] = {0xffff0000, 0xffff0000, 0xffff0000};
        
        for (int i = 0; i < 3; i++)
                attr[i].color = colors2[i];

        DrawPrimitive(TRIANGLE, target, verts, attr);

        // top right triangle
        verts[0] = (Vertex){450, 362, 1, 1};
        verts[1] = (Vertex){450, 452, 1, 1};
        verts[2] = (Vertex){350, 402, 1, 1};
        PIXEL colors3[3] = {0xff00ff00, 0xff00ff00, 0xff00ff00};

        for (int i = 0; i < 3; i++)
                attr[i].color = colors3[i];

        DrawPrimitive(TRIANGLE, target, verts, attr);
        
        // bottom left triangle
        verts[0] = (Vertex){110, 262, 1, 1};
        verts[1] = (Vertex){60, 162, 1, 1};
        verts[2] = (Vertex){150, 162, 1, 1};
        PIXEL colors4[3] = {0xff00ff00, 0xff00ff00, 0xff00ff00};

        for (int i = 0; i < 3; i++)
                attr[i].color = colors4[i];

        DrawPrimitive(TRIANGLE, target, verts, attr);

        // bottom middle
        verts[0] = (Vertex){210, 252, 1, 1};
        verts[1] = (Vertex){260, 172, 1, 1};
        verts[2] = (Vertex){310, 202, 1, 1};
        PIXEL colors5[3] = {0xff00ff00, 0xff00ff00, 0xff00ff00};

        for (int i = 0; i < 3; i++)
                attr[i].color = colors5[i];

        DrawPrimitive(TRIANGLE, target, verts, attr);
        
        // bottom right triangle
        verts[0] = (Vertex){370, 202, 1, 1};
        verts[1] = (Vertex){430, 162, 1, 1};
        verts[2] = (Vertex){470, 252, 1, 1};
        PIXEL colors6[3] = {0xff00ff00, 0xff00ff00, 0xff00ff00};

        for (int i = 0; i < 3; i++)
                attr[i].color = colors6[i];

        DrawPrimitive(TRIANGLE, target, verts, attr);
}


/***********************************************
 * Demonstrate Fragment Shader, linear VBO 
 * interpolation for Project 03. 
 **********************************************/
void TestDrawFragments(Buffer2D<PIXEL> & target)
{
        /**************************************************
        * 1. Interpolated color triangle
        *************************************************/
        Vertex colorTriangle[3];
        Attributes colorAttributes[3];
        colorTriangle[0] = (Vertex){250, 112, 1, 1};
        colorTriangle[1] = (Vertex){450, 452, 1, 1};
        colorTriangle[2] = (Vertex){50, 452, 1, 1};
        PIXEL colors[3] = {0xffff0000, 0xff00ff00, 0xff0000ff}; // Or {{1.0,0.0,0.0}, {0.0,1.0,0.0}, {0.0,0.0,1.0}}
        
        // changing colors to floats
        colorAttributes[0].attrValues[0].d = 1.0;
        colorAttributes[0].attrValues[1].d = 0.0;
        colorAttributes[0].attrValues[2].d = 0.0;
        colorAttributes[0].numValues = 3;

        colorAttributes[1].attrValues[1].d = 1.0;
        colorAttributes[1].attrValues[0].d = 0.0;
        colorAttributes[1].attrValues[2].d = 0.0;
        colorAttributes[1].numValues = 3;

        colorAttributes[2].attrValues[0].d = 0.0;
        colorAttributes[2].attrValues[1].d = 0.0;
        colorAttributes[2].attrValues[2].d = 1.0;
        colorAttributes[2].numValues = 3;

        FragmentShader myColorFragShader;
        myColorFragShader.FragShader = myBaryShader;
        // myColorFragShader.setShader(myBaryShader);
        // Your code for the color fragment shader goes here

        Attributes colorUniforms;
        // Your code for the uniform goes here, if any (don't pass NULL here)

        DrawPrimitive(TRIANGLE, target, colorTriangle, colorAttributes, &colorUniforms, &myColorFragShader);

        // take this out eventually

        /****************************************************
         * 2. Interpolated image triangle
        ****************************************************/
        Vertex imageTriangle[3];
        Attributes imageAttributes[3];
        imageTriangle[0] = (Vertex){425, 112, 1, 1};
        imageTriangle[1] = (Vertex){500, 252, 1, 1};
        imageTriangle[2] = (Vertex){350, 252, 1, 1};
        double coordinates[3][2] = { {1,0}, {1,1}, {0,1} };

        // texture coordinates
        imageAttributes[0].attrValues[0].d = 1;
        imageAttributes[0].attrValues[1].d = 0;
        imageAttributes[0].numValues = 2;

        imageAttributes[1].attrValues[0].d = 1;
        imageAttributes[1].attrValues[1].d = 1;
        imageAttributes[1].numValues = 2;

        imageAttributes[2].attrValues[0].d = 0;
        imageAttributes[2].attrValues[1].d = 1;
        imageAttributes[2].numValues = 2;

        static BufferImage myImage("checker.bmp"); // comment out deconstructor if there are issues
        // Provide an image in this directory that you would like to use (powers of 2 dimensions)

        Attributes imageUniforms;
        imageUniforms.pointerImg = &myImage; // the address of my images

        FragmentShader myImageFragShader;
        myImageFragShader.FragShader = imageFragShader;

        DrawPrimitive(TRIANGLE, target, imageTriangle, imageAttributes, &imageUniforms, &myImageFragShader);
}

/************************************************
 * Demonstrate Perspective correct interpolation 
 * for Project 04. 
 ***********************************************/
void TestDrawPerspectiveCorrect(Buffer2D<PIXEL> & target)
{
        /**************************************************
        * 1. Image quad (2 TRIs) Code (texture interpolated)
        **************************************************/
        // Artificially projected, viewport transformed
        double divA = 6;
        double divB = 40;
        Vertex quad[] = {{(-1200 / divA) + (S_WIDTH/2), (-1500 / divA) + (S_HEIGHT/2), divA, 1.0/divA },
                         {(1200  / divA) + (S_WIDTH/2), (-1500 / divA) + (S_HEIGHT/2), divA, 1.0/divA },
                         {(1200  / divB) + (S_WIDTH/2), (1500  / divB) + (S_HEIGHT/2), divB, 1.0/divB },
                         {(-1200 / divB) + (S_WIDTH/2), (1500  / divB) + (S_HEIGHT/2), divB, 1.0/divB }};

        Vertex verticesImgA[3];
        Attributes imageAttributesA[3];
        verticesImgA[0] = quad[0];
        verticesImgA[1] = quad[1];
        verticesImgA[2] = quad[2];

        Vertex verticesImgB[3];        
        Attributes imageAttributesB[3];
        verticesImgB[0] = quad[2];
        verticesImgB[1] = quad[3];
        verticesImgB[2] = quad[0];

        double coordinates[4][2] = { {0/divA,0/divA}, {1/divA,0/divA}, {1/divB,1/divB}, {0/divB,1/divB} };
        
        // ImageAttributes A
        imageAttributesA[0].attrValues[0].d = coordinates[0][0]; // first point u
        imageAttributesA[0].attrValues[1].d = coordinates[0][1]; // first point v
        imageAttributesA[1].attrValues[0].d = coordinates[1][0]; // second point u
        imageAttributesA[1].attrValues[1].d = coordinates[1][1]; // second point v
        imageAttributesA[2].attrValues[0].d = coordinates[2][0]; // third point u
        imageAttributesA[2].attrValues[1].d = coordinates[2][1]; // third point v

        // ImageAttributes B
        imageAttributesB[0].attrValues[0].d = coordinates[2][0];
        imageAttributesB[0].attrValues[1].d = coordinates[2][1];
        imageAttributesB[1].attrValues[0].d = coordinates[3][0];
        imageAttributesB[1].attrValues[1].d = coordinates[3][1];
        imageAttributesB[2].attrValues[0].d = coordinates[0][0];
        imageAttributesB[2].attrValues[1].d = coordinates[0][1];

        // Set numValues to 2 (UV coordinates)
        imageAttributesA[0].numValues = 2;
        imageAttributesA[1].numValues = 2;
        imageAttributesA[2].numValues = 2;
        imageAttributesB[0].numValues = 2;
        imageAttributesB[1].numValues = 2;
        imageAttributesB[2].numValues = 2;

        BufferImage myImage("checker.bmp");
        // Ensure the checkboard image is in this directory

        Attributes imageUniforms;
        imageUniforms.pointerImg = &myImage;

        FragmentShader fragImg;
        fragImg.FragShader = imageFragShader;

        // Draw image triangle 
        DrawPrimitive(TRIANGLE, target, verticesImgA, imageAttributesA, &imageUniforms, &fragImg);
        DrawPrimitive(TRIANGLE, target, verticesImgB, imageAttributesB, &imageUniforms, &fragImg);
}

/************************************************
 * Demonstrate simple transformations for  
 * Project 05 in the vertex shader callback. 
 ***********************************************/
void TestVertexShader(Buffer2D<PIXEL> & target)
{
        /**************************************************
        * 1. Interpolated color triangle
        *************************************************/
        Vertex colorTriangle[3];
        Attributes colorAttributes[3];
        colorTriangle[0] = (Vertex){ 350, 112, 1, 1};
        colorTriangle[1] = (Vertex){ 400, 200, 1, 1};
        colorTriangle[2] = (Vertex){ 300, 200, 1, 1};

        PIXEL colors[3] = {0xffff0000, 0xff00ff00, 0xff0000ff};
        // Your code for 'colorAttributes' goes here
        colorAttributes[0].attrValues[0].d = 1.0;
        colorAttributes[0].attrValues[1].d = 0.0;
        colorAttributes[0].attrValues[2].d = 0.0;
        colorAttributes[0].numValues = 3;

        colorAttributes[1].attrValues[0].d = 0.0;
        colorAttributes[1].attrValues[1].d = 1.0;
        colorAttributes[1].attrValues[2].d = 0.0;
        colorAttributes[1].numValues = 3;

        colorAttributes[2].attrValues[0].d = 0.0;
        colorAttributes[2].attrValues[1].d = 0.0;
        colorAttributes[2].attrValues[2].d = 1.0;
        colorAttributes[2].numValues = 3;

        FragmentShader myColorFragShader;
        myColorFragShader.FragShader = myBaryShader;

        Attributes colorUniforms;
        // Your code for the uniform goes here, if any (don't pass NULL here)
        
        VertexShader myColorVertexShader;
        myColorVertexShader.VertShader = vertShader;


        /******************************************************************
	 * TRANSLATE (move +100 in the X direction, +50 in the Y direction)
         *****************************************************************/
        // Your translating code that integrates with 'colorUniforms', used by 'myColorVertexShader' goes here
        Matrix translate;
        translate.addTranslate(100, 50, 1);
        colorUniforms.matrix = translate;

	DrawPrimitive(TRIANGLE, target, colorTriangle, colorAttributes, &colorUniforms, &myColorFragShader, &myColorVertexShader);

        /***********************************
         * SCALE (scale by a factor of 0.5)
         ***********************************/
        // Your scaling code that integrates with 'colorUniforms', used by 'myColorVertexShader' goes here
        Matrix scale;
        scale.addScale(0.5, 0.5, 0.5);
        colorUniforms.matrix = scale;

        DrawPrimitive(TRIANGLE, target, colorTriangle, colorAttributes, &colorUniforms, &myColorFragShader, &myColorVertexShader);

        /**********************************************
         * ROTATE 45 degrees in the X-Y plane around Z
         *********************************************/
        // Your rotation code that integrates with 'colorUniforms', used by 'myColorVertexShader' goes here
        Matrix rotate;
        rotate.addRotate(Z, 30);
        colorUniforms.matrix = rotate;

        DrawPrimitive(TRIANGLE, target, colorTriangle, colorAttributes, &colorUniforms, &myColorFragShader, &myColorVertexShader);

        /*************************************************
         * SCALE-TRANSLATE-ROTATE in left-to-right order
         * the previous transformations concatenated.
         ************************************************/
	// Your scale-translate-rotation code that integrates with 'colorUniforms', used by 'myColorVertexShader' goes here
        Matrix allMatrix = rotate * translate * scale;
        colorUniforms.matrix = allMatrix;

        DrawPrimitive(TRIANGLE, target, colorTriangle, colorAttributes, &colorUniforms, &myColorFragShader, &myColorVertexShader);	
}

/********************************************
 * Verify that the whole pipeline works. By
 * the end of week 07 you should be able to
 * run this code successfully.
 *******************************************/
void TestPipeline(Buffer2D<PIXEL> & target)
{
        // This is similar to TestDrawPerspectiveCorrect 
        // except that:
        //      1) perspective projection is expected from
        //         the programmer in the vertex shader.
        //      2) Clipping/normalization must be turned on.
        //      3) The ViewPort Transform must be applied.
        //      4) The Z-Buffer is incorporated into drawing.
        //      5) You may want to involve camera variables:
        //              i)   camYaw
        //              ii)  camPitch
        //              iii) camRoll, 
        //              iv)  camX
        //              v)   camY
        //              vi)  camZ
        //      To incorporate a view transform (add movement)
        
        static Buffer2D<double> zBuf(target.width(), target.height());
        // Will need to be cleared every frame, like the screen

        /**************************************************
        * 1. Image quad (2 TRIs) Code (texture interpolated)
        **************************************************/
        Vertex quad[] = { {-20,-20, 50, 1},
                          {20, -20, 50, 1},
                          {20, 20, 50, 1},
                          {-20,20, 50, 1}};

        Vertex     verticesImgA[3];
        Attributes imageAttributesA[3];
        verticesImgA[0] = quad[0];
        verticesImgA[1] = quad[1];
        verticesImgA[2] = quad[2];

        Vertex     verticesImgB[3];        
        Attributes imageAttributesB[3];
        verticesImgB[0] = quad[2];
        verticesImgB[1] = quad[3];
        verticesImgB[2] = quad[0];

        double coordinates[4][2] = { {0,0}, {1,0}, {1,1}, {0,1} };
        // Your texture coordinate code goes here for 'imageAttributesA, imageAttributesB'

        imageAttributesA[0].insertDbl(coordinates[0][0]);
        imageAttributesA[0].insertDbl(coordinates[0][1]);
        imageAttributesA[1].insertDbl(coordinates[1][0]);
        imageAttributesA[1].insertDbl(coordinates[1][1]);
        imageAttributesA[2].insertDbl(coordinates[2][0]);
        imageAttributesA[2].insertDbl(coordinates[2][1]);

        imageAttributesB[0].insertDbl(coordinates[2][0]);
        imageAttributesB[0].insertDbl(coordinates[2][1]);
        imageAttributesB[1].insertDbl(coordinates[3][0]);
        imageAttributesB[1].insertDbl(coordinates[3][1]);
        imageAttributesB[2].insertDbl(coordinates[0][0]);
        imageAttributesB[2].insertDbl(coordinates[0][1]);

        static BufferImage myImage("checker.bmp");
        Attributes  imageUniforms;
        Matrix      model;
        
        model.addTranslate(0, 0, 0);
        Matrix view = camera4x4(myCam.x, myCam.y, myCam.z, myCam.yaw, myCam.pitch, myCam.roll);
        Matrix proj = perspective4x4(60, 1.0, 1, 200); // FOV, aspect ratio, near, far

        // Uniforms -- add these to attributes as member variables
        // [0] -> Image reference
        // [1] -> Model transform
        // [2] -> View transform

        imageUniforms.insertPtr((void*)&myImage);
        imageUniforms.insertPtr((void*)&model);
        imageUniforms.insertPtr((void*)&view);
        imageUniforms.insertPtr((void*)&proj);

        FragmentShader fragImg;
        fragImg.FragShader = imageFragShader;

        VertexShader vertImg;
        vertImg.VertShader = SimpleVertexShader2; // copy of vert shader

        // Draw image triangle 
        DrawPrimitive(TRIANGLE, target, verticesImgA, imageAttributesA, &imageUniforms, &fragImg, &vertImg, &zBuf);
        DrawPrimitive(TRIANGLE, target, verticesImgB, imageAttributesB, &imageUniforms, &fragImg, &vertImg, &zBuf);

        // NOTE: To test the Z-Buffer additinonal draw calls/geometry need to be called into this scene
}

/********************************************
 * Test Visible Surface Determination
 *******************************************/
void TestVSD(Buffer2D<PIXEL> & target)
{
        
        static Buffer2D<double> zBuf(target.width(), target.height());
        // Will need to be cleared every frame, like the screen

        /**************************************************
        * 1. Image quad (2 TRIs) Code (texture interpolated)
        **************************************************/
        Vertex wall1[] = { {  10,   0,  80, 1},
                           {  60,   0,  50, 1},
                           {  60,  40,  50, 1},
                           {  10,  40,  80, 1}};

        Vertex wall2[] = { {  85,   0,  35, 1},
                           { 135,   0,   5, 1},
                           { 135,  40,   5, 1},
                           {  85,  40,  35, 1}};

        Vertex wall3[] = { {  60,   0,  80, 1},
                           { 135,   0,  35, 1},
                           { 135,  40,  35, 1},
                           {  60,  40,  80, 1}};

        // WALL 1
        Vertex     wall1ImgA[3];
        Attributes wall1AttributesA[3];
        wall1ImgA[0] = wall1[0];
        wall1ImgA[1] = wall1[1];
        wall1ImgA[2] = wall1[2];

        Vertex     wall1ImgB[3];        
        Attributes wall1AttributesB[3];
        wall1ImgB[0] = wall1[2];
        wall1ImgB[1] = wall1[3];
        wall1ImgB[2] = wall1[0];

        // WALL 2
        Vertex     wall2ImgA[3];
        Attributes wall2AttributesA[3];
        wall2ImgA[0] = wall2[0];
        wall2ImgA[1] = wall2[1];
        wall2ImgA[2] = wall2[2];

        Vertex     wall2ImgB[3];        
        Attributes wall2AttributesB[3];
        wall2ImgB[0] = wall2[2];
        wall2ImgB[1] = wall2[3];
        wall2ImgB[2] = wall2[0];

        // WALL 3
        Vertex     wall3ImgA[3];
        Attributes wall3AttributesA[3];
        wall3ImgA[0] = wall3[0];
        wall3ImgA[1] = wall3[1];
        wall3ImgA[2] = wall3[2];

        Vertex     wall3ImgB[4];        
        Attributes wall3AttributesB[3];
        wall3ImgB[0] = wall3[2];
        wall3ImgB[1] = wall3[3];
        wall3ImgB[2] = wall3[0];

        // Quad testQuad1(wall1); // quad constructor with verts
        // Quad testQuad2(wall2);
        // testQuad1 = testQuad2; // quad assignment
        // Quad testQuad3(testQuad2); // quad constructor with quad
        // Node testNode1(wall1); // node constructor with verts
        // Node testNode2(testQuad1); // node constrictur with quad
        // Node testNode3(testNode1); // node constructor with a node
        // testNode3 = testNode2; // node assignment

        Node myWall1(wall1);
        Node myWall2(wall2);
        Node myWall3(wall3);

        // og
        Vertex tester1[] = { {  6,   0,  8, 1},
                           {  10,   0,  4, 1},
                           {  10,  40,  4, 1},
                           {  6,  40,  8, 1}};

        // previous toucher
        Vertex tester2[] = { {  9,   0,  7, 1},
                           { 10,   0,   11, 1},
                           { 10,  40,   11, 1},
                           {  9,  40,  7, 1}};

        // flat
        Vertex tester3[] = { {  2,   0,  6, 1},
                           { 6,   0,  6, 1},
                           { 6,  40,  6, 1},
                           {  2,  0,  6, 1}};

        Node * nodeA = new Node(tester1);
        Node * nodeB = new Node (tester2);
        Node * nodeC = new Node(tester3);

        std::vector <Node *> testerVector;
        testerVector.push_back(nodeB);
        testerVector.push_back(nodeA);
        testerVector.push_back(nodeC);

        BSPTree myTree(testerVector);

        double coordinates[4][2] = { {0,0}, {1,0}, {1,1}, {0,1} };
        // Your texture coordinate code goes here for 'imageAttributesA, imageAttributesB'

        // WALL 1
        wall1AttributesA[0].insertDbl(coordinates[0][0]);
        wall1AttributesA[0].insertDbl(coordinates[0][1]);
        wall1AttributesA[1].insertDbl(coordinates[1][0]);
        wall1AttributesA[1].insertDbl(coordinates[1][1]);
        wall1AttributesA[2].insertDbl(coordinates[2][0]);
        wall1AttributesA[2].insertDbl(coordinates[2][1]);

        wall1AttributesB[0].insertDbl(coordinates[2][0]);
        wall1AttributesB[0].insertDbl(coordinates[2][1]);
        wall1AttributesB[1].insertDbl(coordinates[3][0]);
        wall1AttributesB[1].insertDbl(coordinates[3][1]);
        wall1AttributesB[2].insertDbl(coordinates[0][0]);
        wall1AttributesB[2].insertDbl(coordinates[0][1]);

        // WALL 2
        wall2AttributesA[0].insertDbl(coordinates[0][0]);
        wall2AttributesA[0].insertDbl(coordinates[0][1]);
        wall2AttributesA[1].insertDbl(coordinates[1][0]);
        wall2AttributesA[1].insertDbl(coordinates[1][1]);
        wall2AttributesA[2].insertDbl(coordinates[2][0]);
        wall2AttributesA[2].insertDbl(coordinates[2][1]);

        wall2AttributesB[0].insertDbl(coordinates[2][0]);
        wall2AttributesB[0].insertDbl(coordinates[2][1]);
        wall2AttributesB[1].insertDbl(coordinates[3][0]);
        wall2AttributesB[1].insertDbl(coordinates[3][1]);
        wall2AttributesB[2].insertDbl(coordinates[0][0]);
        wall2AttributesB[2].insertDbl(coordinates[0][1]);

        // WALL 3
        wall3AttributesA[0].insertDbl(coordinates[0][0]);
        wall3AttributesA[0].insertDbl(coordinates[0][1]);
        wall3AttributesA[1].insertDbl(coordinates[1][0]);
        wall3AttributesA[1].insertDbl(coordinates[1][1]);
        wall3AttributesA[2].insertDbl(coordinates[2][0]);
        wall3AttributesA[2].insertDbl(coordinates[2][1]);

        wall3AttributesB[0].insertDbl(coordinates[2][0]);
        wall3AttributesB[0].insertDbl(coordinates[2][1]);
        wall3AttributesB[1].insertDbl(coordinates[3][0]);
        wall3AttributesB[1].insertDbl(coordinates[3][1]);
        wall3AttributesB[2].insertDbl(coordinates[0][0]);
        wall3AttributesB[2].insertDbl(coordinates[0][1]);

        static BufferImage myImage1("vsdphotos/howl1.bmp");
        static BufferImage myImage2("vsdphotos/howl2.bmp");
        static BufferImage myImage3("vsdphotos/howl3.bmp");


        Attributes  imageUniforms;
        Matrix      model;
        
        model.addTranslate(0, 0, 0);
        Matrix view = camera4x4(myCam.x, myCam.y, myCam.z, myCam.yaw, myCam.pitch, myCam.roll);
        Matrix proj = perspective4x4(60, 1.0, 1, 200); // FOV, aspect ratio, near, far

        // Uniforms -- add these to attributes as member variables
        // [0] -> Image reference
        // [1] -> Model transform
        // [2] -> View transform

        imageUniforms.insertPtr((void*)&myImage3);
        imageUniforms.insertPtr((void*)&model);
        imageUniforms.insertPtr((void*)&view);
        imageUniforms.insertPtr((void*)&proj);

        FragmentShader fragImg;
        fragImg.FragShader = imageFragShader;

        VertexShader vertImg;
        vertImg.VertShader = SimpleVertexShader2; // copy of vert shader

        // Draw image triangle 
        // BACK WALL - WALL 3
        DrawPrimitive(TRIANGLE, target, wall3ImgA, wall3AttributesA, &imageUniforms, &fragImg, &vertImg, &zBuf);
        DrawPrimitive(TRIANGLE, target, wall3ImgB, wall3AttributesB, &imageUniforms, &fragImg, &vertImg, &zBuf);

        // WALL 1
        imageUniforms[0].ptr = (void*)&myImage1;
        DrawPrimitive(TRIANGLE, target, wall1ImgA, wall1AttributesA, &imageUniforms, &fragImg, &vertImg, &zBuf);
        DrawPrimitive(TRIANGLE, target, wall1ImgB, wall1AttributesB, &imageUniforms, &fragImg, &vertImg, &zBuf);

        // WALL 2
        imageUniforms[0].ptr = (void*)&myImage2;
        DrawPrimitive(TRIANGLE, target, wall2ImgA, wall2AttributesA, &imageUniforms, &fragImg, &vertImg, &zBuf);
        DrawPrimitive(TRIANGLE, target, wall2ImgB, wall2AttributesB, &imageUniforms, &fragImg, &vertImg, &zBuf);

        // NOTE: To test the Z-Buffer additinonal draw calls/geometry need to be called into this scene
}

#endif