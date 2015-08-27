/*
 * =====================================================================================
 *
 *       Filename:  L26.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2011-9-12 18:28:49
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  VR爱好者 (VRer), houyue9645@163.com
 *        Company:  虚拟现实工作室
 *
 * =====================================================================================
 */
    /* L26.c 
    OpenGL Framework using GLUT 3.7  
    Rob Fletcher  2002 
    Create a display list of a "unit cube" 
    Draw 9 cubes and rotate them all in x,y,z 
    Enable/disable depth test with d/D 
    Enable/Disable scissor test with s/S 
    Move scissor box with arror keypad 
    Change size of scissor box with F1 -> F4 
    */  
    #include <stdio.h>  
    #include <stdlib.h>     /* For "exit" prototype */  
    #include <GL/glut.h>    /* Header File For The GLUT Library */  
      
    /********** uncomment these to activate scale and rotation example */  
    #define ROT  
    #define SCALE  
    /* ASCII code for the escape key. */  
    #define ESCAPE 27  
    GLint window;           /* The number of our GLUT window */  
    GLint Xsize=400;  
    GLint Ysize=400;  
    GLfloat xangle=0.0,yangle=0.0,zangle=0.0;   
    #ifdef ROT  
    GLfloat xtheta=0.0,ytheta=0.0,ztheta=0.0;     
    #endif  
    /* Globals for scissor box location and size  */  
    GLint scissorx=0, scissory=0;            /* origin */  
    GLsizei scissorwidth=0, scissorheight=0; /* size */  
    GLint scissortest=0;  /* state toggle */  
    GLint chop[4];        /* return data */  
    /* Simple window transformation routine */  
    GLvoid Transform(GLfloat Width, GLfloat Height)  
    {  
        glViewport(0, 0, Width, Height);              /* Set the viewport */  
        glMatrixMode(GL_PROJECTION);                  /* Select the projection matrix */  
        glLoadIdentity();               /* Reset The Projection Matrix */  
        gluPerspective(45.0,Width/Height,0.1,100.0);  /* Calculate The Aspect Ratio Of The Window */  
        glMatrixMode(GL_MODELVIEW);                   /* Switch back to the modelview matrix */  
        /* (re)Set the scissor box to the current window */  
        scissorx = glutGet(GLUT_WINDOW_X)-8;       /* -8 for pixel border */  
        scissory = glutGet(GLUT_WINDOW_Y)-32;      /* -32 for top bar ! */  
        scissorwidth = glutGet(GLUT_WINDOW_WIDTH);  
        scissorheight = glutGet(GLUT_WINDOW_HEIGHT);  
        glScissor(scissorx, scissory, scissorwidth, scissorheight);  
        glGetIntegerv( GL_SCISSOR_BOX,chop);  
        printf("Scissor box x/y = %d %d    w/h = %d %d/n",chop[0],chop[1],chop[2],chop[3]);  
    }  
    GLuint CUBE = 1;  
    GLvoid MakeList()  
    {  
        glNewList( CUBE, GL_COMPILE);  
        glBegin(GL_QUADS);                /* start drawing the cube.*/  
        /* top of cube*/  
        glColor3f(0.0,1.0,0.0);            /* Set The Color To Blue*/  
        glVertex3f( 1.0, 1.0,-1.0);        /* Top Right Of The Quad (Top)*/  
        glVertex3f(-1.0, 1.0,-1.0);        /* Top Left Of The Quad (Top)*/  
        glVertex3f(-1.0, 1.0, 1.0);        /* Bottom Left Of The Quad (Top)*/  
        glVertex3f( 1.0, 1.0, 1.0);        /* Bottom Right Of The Quad (Top)*/  
        /* bottom of cube*/  
        glColor3f(1.0,0.5f,0.0);            /* Set The Color To Orange*/  
        glVertex3f( 1.0,-1.0, 1.0);        /* Top Right Of The Quad (Bottom)*/  
        glVertex3f(-1.0,-1.0, 1.0);        /* Top Left Of The Quad (Bottom)*/  
        glVertex3f(-1.0,-1.0,-1.0);        /* Bottom Left Of The Quad (Bottom)*/  
        glVertex3f( 1.0,-1.0,-1.0);        /* Bottom Right Of The Quad (Bottom)*/  
        /* front of cube*/  
        glColor3f(1.0,0.0,0.0);            /* Set The Color To Red*/  
        glVertex3f( 1.0, 1.0, 1.0);        /* Top Right Of The Quad (Front)*/  
        glVertex3f(-1.0, 1.0, 1.0);        /* Top Left Of The Quad (Front)*/  
        glVertex3f(-1.0,-1.0, 1.0);        /* Bottom Left Of The Quad (Front)*/  
        glVertex3f( 1.0,-1.0, 1.0);        /* Bottom Right Of The Quad (Front)*/  
        /* back of cube.*/  
        glColor3f(1.0,1.0,0.0);            /* Set The Color To Yellow*/  
        glVertex3f( 1.0,-1.0,-1.0);        /* Top Right Of The Quad (Back)*/  
        glVertex3f(-1.0,-1.0,-1.0);        /* Top Left Of The Quad (Back)*/  
        glVertex3f(-1.0, 1.0,-1.0);        /* Bottom Left Of The Quad (Back)*/  
        glVertex3f( 1.0, 1.0,-1.0);        /* Bottom Right Of The Quad (Back)*/  
        /* left of cube*/  
        glColor3f(0.0,0.0,1.0);            /* Blue*/  
        glVertex3f(-1.0, 1.0, 1.0);        /* Top Right Of The Quad (Left)*/  
        glVertex3f(-1.0, 1.0,-1.0);        /* Top Left Of The Quad (Left)*/  
        glVertex3f(-1.0,-1.0,-1.0);        /* Bottom Left Of The Quad (Left)*/  
        glVertex3f(-1.0,-1.0, 1.0);        /* Bottom Right Of The Quad (Left)*/  
        /* Right of cube */  
        glColor3f(1.0,0.0,1.0);            /* Set The Color To Violet*/  
        glVertex3f(1.0, 1.0, 1.0);        /* Top Right Of The Quad (Left)*/  
        glVertex3f(1.0, 1.0,-1.0);        /* Top Left Of The Quad (Left)*/  
        glVertex3f(1.0,-1.0,-1.0);        /* Bottom Left Of The Quad (Left)*/  
        glVertex3f(1.0,-1.0, 1.0);        /* Bottom Right Of The Quad (Left)*/  
        glEnd();                              /* Done Drawing The Cube*/  
        glEndList();  
    }  
      
    /* A general OpenGL initialization function.  Sets all of the initial parameters. */  
    GLvoid InitGL(GLfloat Width, GLfloat Height)      
    {  
        glClearColor(0.0, 0.0, 0.0, 0.0);       /* This Will Clear The Background Color To Black */  
        Transform( Width, Height );                   /* Perform the transformation */  
        MakeList();  
    }  
    /* The function called when our window is resized  */  
    GLvoid ReSizeGLScene(GLint Width, GLint Height)  
    {  
        if (Height==0)    Height=1;                   /* Sanity checks */  
        if (Width==0)      Width=1;  
        Transform( Width, Height );                   /* Perform the transformation */  
    }  
      
    /* The main drawing function 
    In here we put all the OpenGL and calls to routines which manipulate 
    the OpenGL state and environment. 
    This is the function which will be called when a "redisplay" is requested. 
    */  
    GLvoid DrawGLScene()  
    {  
        int i,j;  
        /** OpenGL spec says only the area INSIDE a scissor box will be cleared!!! */  
        if( glIsEnabled(GL_SCISSOR_TEST) )glDisable(GL_SCISSOR_TEST);  
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); /* Clear The Screen And The Depth Buffer */  
        if( !glIsEnabled(GL_SCISSOR_TEST) && scissortest==1 )glEnable(GL_SCISSOR_TEST);  
        for (j=0; j<3; j++ ) {  
            for (i = 0 ; i<3; i++) {  
                glLoadIdentity();  
                glTranslatef(1.5-i*1.5,1.5-j*1.5,-6.0);  
                /** both rotation examples */  
    #ifndef ROT  
                glRotatef(xangle,1.0,0.0,0.0);  
                glRotatef(yangle,0.0,1.0,0.0);  
                glRotatef(zangle,0.0,0.0,1.0);  
    #else  
                glRotatef(xtheta,1.0,0.0,0.0);  
                glRotatef(ytheta,0.0,1.0,0.0);  
                glRotatef(ztheta,0.0,0.0,1.0);  
    #endif  
                /* same or differnet scale! */  
    #ifndef SCALE  
                glScalef(0.5,0.5,0.5);  
    #else  
                glScalef(0.5 *(i+1)/2.5,0.5*(i+1)/2.5,0.5*(i+1)/2.5);  
    #endif  
                glCallList(CUBE); /* execute the display list */  
            }  
        }  
        /******* Draw the scissor window so we can see what gives ******/  
        glMatrixMode(GL_PROJECTION);  
        glPushMatrix();  
        glLoadIdentity();  
        glOrtho(0.0, glutGet(GLUT_WINDOW_WIDTH),   
            0.0, glutGet(GLUT_WINDOW_HEIGHT), -1.0, 1.0);  
        glMatrixMode(GL_MODELVIEW);  
        glPushMatrix();  
        glLoadIdentity();  
        glColor3f(1.0,1.0,1.0);  
        glBegin(GL_LINE_STRIP);  
        glVertex2f( scissorx+1 , scissory+1);  
        glVertex2f( scissorwidth+scissorx-1, scissory+1);  
        glVertex2f( scissorwidth+scissorx-1, scissorheight+scissory-1);  
        glVertex2f( scissorx+1, scissorheight+scissory-1);  
        glVertex2f( scissorx+1, scissory+1);       
        glEnd();  
        glPopMatrix();  
        glMatrixMode(GL_PROJECTION);  
        glPopMatrix();  
        glMatrixMode(GL_MODELVIEW);  
        glFlush();   
        glutSwapBuffers();  
    #ifdef ROT  
        /* Update the rotation angle */  
        xtheta += xangle; if( xtheta > 360.0 ) xtheta=0.0;  
        ytheta += yangle; if( ytheta > 360.0 ) ytheta=0.0;  
        ztheta += zangle; if( ztheta > 360.0 ) ztheta=0.0;  
    #endif  
    }  
      
    /*  The function called whenever a "normal" key is pressed. */  
    void NormalKey(GLubyte key, GLint x, GLint y)   
    {  
        switch ( key )    {   
         case ESCAPE :  
             printf("escape pressed. exit./n");  
             glutDestroyWindow(window); /* Kill our window */  
             exit(0);           /* Very dirty exit */                    
             break;             /* Do we need this??? */  
         case 'x':  
             xangle += 1.0;  
             break;  
         case 'X':  
             xangle -= 1.0;  
             break;  
         case 'y':  
             yangle += 1.0;  
             break;  
         case 'Y':  
             yangle -= 1.0;  
             break;  
         case 'z':  
             zangle += 1.0;  
             break;  
         case 'Z':  
             zangle -= 1.0;  
             break;  
         case 'd':  
             glEnable(GL_DEPTH_TEST);  
             break;  
         case 'D':  
             glDisable(GL_DEPTH_TEST);  
             break;  
         case 'r':  
         case 'R':  
             xangle = 0.0;  yangle = 0.0;  zangle = 0.0;  
    #ifdef ROT  
             xtheta = 0.0;  ytheta = 0.0;  ztheta = 0.0;  
    #endif  
             break;  
         case 's': scissortest = 1; glEnable(GL_SCISSOR_TEST);  
             break;  
         case 'S': scissortest = 0; glDisable(GL_SCISSOR_TEST);  
             break;  
         default:  
             break;  
        }  
    }  
      
    void SpecialKey(GLint key, GLint x, GLint y)   
    {  
        switch ( key )    {   
         case GLUT_KEY_F1: scissorwidth += 5;  glScissor(scissorx, scissory, scissorwidth, scissorheight);  
             break;  
         case GLUT_KEY_F2: scissorwidth -= 5;  glScissor(scissorx, scissory, scissorwidth, scissorheight);  
             break;  
         case GLUT_KEY_F3: scissorheight += 5;  glScissor(scissorx, scissory, scissorwidth, scissorheight);  
             break;  
         case GLUT_KEY_F4: scissorheight -= 5;  glScissor(scissorx, scissory, scissorwidth, scissorheight);  
             break;  
         case GLUT_KEY_UP: scissory += 5;  glScissor(scissorx, scissory, scissorwidth, scissorheight);  
             break;  
         case GLUT_KEY_DOWN: scissory -= 5;  glScissor(scissorx, scissory, scissorwidth, scissorheight);  
             break;  
         case GLUT_KEY_LEFT: scissorx -= 5;  glScissor(scissorx, scissory, scissorwidth, scissorheight);  
             break;  
         case GLUT_KEY_RIGHT: scissorx += 5;  glScissor(scissorx, scissory, scissorwidth, scissorheight);  
             break;  
        }  
    }  
    /*************************** Main ***************************************************************/  
    int main(int argc, char **argv)   
    {    
        /* Initialisation and window creation */  
        glutInit(&argc, argv);               /* Initialize GLUT state. */  
        glutInitDisplayMode(GLUT_RGBA |      /* RGB and Alpha */  
            GLUT_DOUBLE|     /* Double buffer */  
            GLUT_DEPTH);     /* Z buffer (depth) */  
        glutInitWindowSize(Xsize,Ysize);         /* set initial window size. */  
        glutInitWindowPosition(0,0);         /* upper left corner of the screen. */  
        glShadeModel(GL_SMOOTH);  
        window = glutCreateWindow("L26");     /* Open a window with a title. */   
        InitGL(Xsize,Ysize);                     /* Initialize our window. */  
        /* Now register the various callback functions */  
        glutDisplayFunc(DrawGLScene);        /* Function to do all our OpenGL drawing. */  
        glutIdleFunc(DrawGLScene);  
        glutReshapeFunc(ReSizeGLScene);  
        glutKeyboardFunc(NormalKey);         /*Normal key is pressed */  
        glutSpecialFunc(SpecialKey);         /*Special key is pressed */  
        /* Now drop into the event loop from which we never return */  
        glutMainLoop();                      /* Start Event Processing Engine. */    
        return 1;  
    }  

