#include "Lab.h"
#include "Lights.h"
#include "Camera.h"


/********************** GLOBALS ******************************/
enum View_Type { FULL_SCREEN, VERTICAL_SPLIT, HORIZONTAL_SPLIT };
View_Type view = FULL_SCREEN;

double zTrans = -14.0f;
float rotationX = 0.0f;
float rotationY = 0.0f;

std::vector<LightSource*> lights;

// light sources
LightSource ambientLight(color(0.15, 0.15, 0.15, 1.0));
PositionalLight lightPos(dvec3(-3, 3, -3), dvec4(0.75, 0.75, 0.75, 1.0));
DirectionalLight lightDir(dvec3(1, 1, 1), dvec4(1.0, 1.0, 1.0, 1.0));
SpotLight lightSpot(dvec3(0.0, 10.0, 0.0), dvec3(0.0, -1.0, 0.0), glm::cos(glm::radians(15.0)), color(1.0, 1.0, 1.0, 1.0));

const GLint FRAMES_PER_SECOND = 60; // Desired maximum number of frames per second
const GLint FRAME_INTERVAL = 1000 / FRAMES_PER_SECOND; // Interval in milliseconds between frames

// Frame buffer that contains both the color and depth buffers
FrameBuffer frameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);

// Objects in the scene

Sphere redSphere(color(1.0, 0.0, 0.0, 0.5f), 1.0, 16, 16);
Pyramid purplePyramid(color(1.0, 0.0, 1.0, 0.5f), 1.0, 1.0);
Pyramid backBluePyramid(color(0.0, 0.0, 1.0, 0.5f), 1.0, 1.0);
Pyramid whitePyramid(color(1.0, 1.0, 1.0, 0.5f), 1.0, 1.0);

Box greenBox(color(0.0, 1.0, 0.0, 1.0), 1.0, 1.0, 1.0);
Box redBox(color(1.0, 0.0, 0.0, 1.0), 1.0, 1.0, 1.0);
Box yellowBox(color(1.0, 1.0, 0.0, 1.0), 1.0, 1.0, 1.0);
Box blueBox(color(0.0, 0.0, 1.0, 1.0), 1.0, 1.0, 1.0);

Box backBlueBox(color(0.0, 0.0, 1.0, 1.0), 1.0, 1.0, 1.0);
Box frontGreenBox(color(0.0, 1.0, 0.0, 1.0), 2.0, 2.0, 2.0);

// Reference plane
ReferencePlane referencePlane;

// Global to hold the rotation angle of objects in the scene
double angle = glm::radians(45.0);

Camera camera;

/********************** END GLOBALS ******************************/

void renderObjects()
{
    referencePlane.setPosition(dvec3(0.0, -3.0, 0.0));
    referencePlane.draw();

    // Set the position and orientation of the left pyramid and render it.
    purplePyramid.setPosition( dvec3( -3.0, 0.0, 0.0  )  );
    purplePyramid.setOrientation( angle, dvec3( 0.0, 0.0, 1.0  )  );
    purplePyramid.setScale(2.0);
    purplePyramid.draw(  );
    
    backBlueBox.setPosition(dvec3(3.5, -2.5, -3.5));
    backBlueBox.draw();

    backBluePyramid.setPosition(dvec3(3.5, -1.5, -3.5));
    backBluePyramid.draw();

    redSphere.setPosition(dvec3(3.0, 0.0, 0.0));
    redSphere.setOrientation(angle, dvec3(1.0, 0.0, 0.0));
    redSphere.draw();

    whitePyramid.setPosition(dvec3(10.0 * glm::sin(-angle), 3.0, 10 * glm::cos(-angle)));
    whitePyramid.setOrientation(0, dvec3(1.0, 1.0, 1.0));
    whitePyramid.draw();

    greenBox.setPosition(dvec3(-0.5, -2.5, -0.5));
    greenBox.draw();

    yellowBox.setPosition(dvec3(0.5, -2.5, -0.5));
    yellowBox.draw();

    redBox.setPosition(dvec3(0.0, -2.5, 0.5));
    redBox.draw();
    
    blueBox.setPosition(dvec3(0.0, -1.5, 0.0));
    blueBox.setOrientation(glm::radians(45.0), dvec3(0.0, 1.0, 0.0));
    blueBox.draw();

    frontGreenBox.setPosition(dvec3(-3.0, -2.0, 3.0));
    frontGreenBox.draw();

/*
    // white orbiting pyramid
    PerVertex::modelingTransformation = glm::rotate(-angle, dvec3(0.0, 1.0, 0.0)) * glm::translate(dvec3(10.0, 3.0, 0.0)) * glm::rotate(angle, dvec3(1.0, 0.0, 0.0));
    PerVertex::processTriangleVertices(whitePyramid.triangleVertices);
*/
} // end renderObjects

// myPerspective
glm::dmat4 myPerspective(double fov, double aspect, double near, double far)
{
    glm::dmat4 mat;
    
    double yScale = 1.0 / glm::tan((MM_PI / 180.0) * fov / 2);
    double xScale = yScale / aspect;

    mat[0][0] = xScale;
    mat[1][1] = yScale;
    mat[2][2] = (far + near) / (near - far);
    mat[2][3] = -1.0;
    mat[3][2] = (2.0 * far * near) / (near - far);

    return mat;
} // end myPerspective // myOrthographic

glm::dmat4 myOrthographic(double left, double right, double bottom, double top, double near, double far)
{
    glm::dmat4 mat;

    mat[0][0] = 2.0 / (right - left);
    mat[3][0] = -(right + left) / (right - left);
    mat[1][1] = 2.0 / (top - bottom);
    mat[3][1] = -(top + bottom) / (top - bottom);
    mat[2][2] = -2.0 / (far - near);
    mat[3][2] = -(far + near) / (far - near);
    mat[3][3] = 1;

    return mat;
}

void twoViewsSplitVertically()
{
    // Render left side view
    double viewportWidth = (PerVertex::xViewportMax-PerVertex::xViewportMin)/2.0;
	double viewportHeight = (PerVertex::yViewportMax - PerVertex::yViewportMin);

	PerVertex::projectionTransformation = myPerspective(55.0, 
            ((double)viewportWidth) / ((double)viewportHeight), 0.1f, 100.0);

	// Set viewport transformation for left view
	PerVertex::viewportTransformation =
		glm::translate(dvec3(0.0, 0.0, 0.0)) *
        glm::scale(dvec3((double)(viewportWidth)/(PerVertex::xNdcMax-
                        PerVertex::xNdcMin),
                    (double)(viewportHeight)/(PerVertex::yNdcMax-PerVertex::yNdcMin),1.0)) *
        glm::translate(dvec3(-PerVertex::xNdcMin, -PerVertex::yNdcMin, 0.0));

	renderObjects();

    PerVertex::projectionTransformation = myOrthographic(-8.0, 8.0, -6.0, 6.0, 1.0, 100.0);

    // set viewport transformation for right view
	PerVertex::viewportTransformation =
		glm::translate(dvec3(4.0, 0.0, 0.0)) *
        glm::scale(dvec3((double)(viewportWidth)/(PerVertex::xNdcMax-
                        PerVertex::xNdcMin),
                    (double)(viewportHeight)/(PerVertex::yNdcMax-PerVertex::yNdcMin),1.0)) *
        glm::translate(dvec3(3, PerVertex::yNdcMax, 0.0));


    renderObjects();


}

// Draws two views of the scene. One on the top and one on the bottom.
void twoViewsSplitHorizontally()
{
	// Render left side view
    double viewportWidth = (PerVertex::xViewportMax-PerVertex::xViewportMin);
	double viewportHeight = (PerVertex::yViewportMax - PerVertex::yViewportMin)/2.0;

	PerVertex::projectionTransformation = myPerspective(55.0, 
            ((double)viewportWidth) / ((double)viewportHeight), 0.1f, 100.0);

	// Set viewport transformation for bottom view
	PerVertex::viewportTransformation =
		glm::translate(dvec3(0.0, 0.0, 0.0)) *
        glm::scale(dvec3((double)(viewportWidth)/(PerVertex::xNdcMax-
                        PerVertex::xNdcMin),
                    (double)(viewportHeight)/(PerVertex::yNdcMax-PerVertex::yNdcMin),1.0)) *
        glm::translate(dvec3(-PerVertex::xNdcMin, -PerVertex::yNdcMin, 0.0));

	renderObjects();

    PerVertex::projectionTransformation = myOrthographic(-23.0, 23.0, -4.0, 4.0, 1.0, 100.0);

    // set viewport transformation for top view
	PerVertex::viewportTransformation =
		glm::translate(dvec3(0.0, 4.0, 0.0)) *
        glm::scale(dvec3((double)(viewportWidth)/(PerVertex::xNdcMax-
                        PerVertex::xNdcMin),
                    (double)(viewportHeight)/(PerVertex::yNdcMax-PerVertex::yNdcMin),1.0)) *
        glm::translate(dvec3(PerVertex::xNdcMax, 3.0, 0.0));


    renderObjects();

} // end twoViewsSplitHorizontally


/**
* Acts as the display function for the window.
*/
static void RenderSceneCB()
{
	// time in milliseconds of last frame render
	static GLint lastRenderTime = 0;

	int currentTime = glutGet(GLUT_ELAPSED_TIME); // Get current time
	int elapsedTime = currentTime - lastRenderTime; // Calc time since last frame

	// Check if enough time has elapsed since the last render.
	if (elapsedTime >= FRAME_INTERVAL) {

		// Save time for this frame render
		lastRenderTime = currentTime;

		// Clear the color buffer
		frameBuffer.clearColorAndDepthBuffers();

		angle += glm::radians(1.0);

		// Determine the position of the viewpoint in world coordinates
		PerVertex::eyePositionInWorldCoords = glm::inverse(PerVertex::viewingTransformation)[3].xyz;

		// Draw the objects in the scene
		switch(view) {

		case VERTICAL_SPLIT:

			twoViewsSplitVertically();
			break;

		case HORIZONTAL_SPLIT:
			twoViewsSplitHorizontally();
			break;
	
		default:
			renderObjects();
        }

		// Display the color buffer
		frameBuffer.showColorBuffer();
	}

} // end RenderSceneCB


// Reset viewport limits for full window rendering each time the window is resized.
// This function is called when the program starts up and each time the window is 
// resized.
static void ResizeCB(int width, int height)
{
	// Size the color buffer to match the window size.
	frameBuffer.setFrameBufferSize(width, height); 

	// Set rendering window parameters for viewport transfomation
	PerVertex::xViewportMin = 0;
	PerVertex::yViewportMin = 0;
	PerVertex::xViewportMax = (double)width;
	PerVertex::yViewportMax = (double)height;

	// Create a perspective projection matrix. 
	PerVertex::projectionTransformation = myPerspective( 45.0, ( (double)PerVertex::xViewportMax - PerVertex::xViewportMin ) /
		( (double)PerVertex::yViewportMax - PerVertex::yViewportMin ), 1.0, 50.0 );
		
	// Set viewport transformation
	PerVertex::viewportTransformation =
		glm::translate(dvec3(PerVertex::xViewportMin, PerVertex::yViewportMin, 0.0)) *
		glm::scale(dvec3((double)(PerVertex::xViewportMax - PerVertex::xViewportMin) / 
			(PerVertex::xNdcMax - PerVertex::xNdcMin), 
			(double)(PerVertex::yViewportMax - PerVertex::yViewportMin) / 
			(PerVertex::yNdcMax - PerVertex::yNdcMin), 1.0)) *
		glm::translate(dvec3(-PerVertex::xNdcMin, -PerVertex::yNdcMin, 0.0));

	// Signal the operating system to re-render the window
	glutPostRedisplay();

} // end ResizeCB

void viewPortMenu(int value)
{
	switch (value) {

	case(0):

		// "Quit" selected on the menu
#warning uncomment
		//glutLeaveMainLoop();
		break;
	case(1):

		view = FULL_SCREEN;

		ResizeCB((int)PerVertex::xViewportMax,
			(int)PerVertex::yViewportMax);

		break;

	case(2):

		view = VERTICAL_SPLIT;
		std::cout << "vertical" << std::endl;
		break;

	case(3):

		view = HORIZONTAL_SPLIT;

		std::cout << "horizontal" << std::endl;

		break;

	default:
		std::cout << "Invalid view selection " << std::endl;
	}

	// Signal GLUT to call display callback
	glutPostRedisplay();

} // end viewPortMenu


// Responds to 'f' and escape keys. 'f' key allows 
// toggling full screen viewing. Escape key ends the
// program. Allows lights to be individually turned on and off.
static void KeyboardCB(unsigned char key, int x, int y)
{
	switch (key) {

	case('f') : case('F') : // 'f' key to toggle full screen
#warning uncomment 
		//glutFullScreenToggle();
		break;
	case(27) : // Escape key
#warning uncomment
		//glutLeaveMainLoop();
		break;

    case('w') : case('W') :
        zTrans = zTrans + 1;

        break;

    case('s') : case('S') :
        zTrans = zTrans - 1;
        
        break;

	default:
		std::cout << key << " key pressed." << std::endl;
	}

	// Set the viewing transformation based on the values held in the global
	// varaiables zTrans, rotationX, and rotationY.
	glm::mat4 transView = glm::translate(glm::vec3(0.0f, 0.0f, zTrans));
	glm::mat4 rotateViewX = glm::rotate(glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotateViewY = glm::rotate(glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));

    PerVertex::viewingTransformation = transView * rotateViewX * rotateViewY;


	glutPostRedisplay();

} // end KeyboardCB

// Responds to presses of the arrow keys
static void SpecialKeysCB(int key, int x, int y)
{
	static const double rotateInc = glm::radians(20.0);

	switch (key) {

	case(GLUT_KEY_RIGHT) :
        rotationY = rotationY + 1;
		break;
	case(GLUT_KEY_LEFT) :
        rotationY = rotationY - 1;
        break;
	case(GLUT_KEY_UP) :
        rotationX = rotationX - 1;
        break;
	case(GLUT_KEY_DOWN) :
        rotationX = rotationX + 1;
        break;

	default:
		std::cout << key << " key pressed." << std::endl;
	}

    glm::mat4 transView = glm::translate(glm::vec3(0.0f, 0.0f, zTrans));
	glm::mat4 rotateViewX = glm::rotate(glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotateViewY = glm::rotate(glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));

    PerVertex::viewingTransformation = transView * rotateViewX * rotateViewY;

	glutPostRedisplay();

} // end SpecialKeysCB


// Register as the "idle" function to have the screen continously
// repainted. Due to software rendering, the frame rate will not
// be fast enough to support motion simulation
static void animate()
{
	glutPostRedisplay();

} // end animate


void mainMenu(int value)
{
	switch (value) {

	case(0):

		// "Quit" selected on the menu
#warning uncomment next line
		//glutLeaveMainLoop();
		break;

	default:
		std::cout << "Invalid main menu selection " << std::endl;
	}

	// Signal GLUT to call display callback
	glutPostRedisplay();

} // end mainMenu

void polygonRenderMenu( int value )
{
	switch( value ) {

		case( 0 ):

			PerVertex::polygonRenderMode = FILL;
			break;
		case( 1 ):

			PerVertex::polygonRenderMode = LINE;
			break;

		default:
			std::cout << "Invalid polygon render selection " << std::endl;
	}

	// Signal GLUT to call display callback
	glutPostRedisplay( );

} // end polygonRenderMenu

void viewMenu( int value )
{
	switch( value ) {

		case( 1 ):
			PerVertex::viewingTransformation = glm::translate(glm::vec3( 0.0f, 0.0f, -12.0 ) );
			break;

		case( 2 ):
            PerVertex::viewingTransformation = glm::rotate(glm::translate(glm::vec3( 0.0f, 0.0f, -14.0 )) , glm::radians(45.0f), glm::vec3(1.0, 0.0, 0.0));
            break;

		case( 3 ):
            PerVertex::viewingTransformation = glm::rotate(glm::rotate(glm::translate(glm::vec3( 0.0f, 0.0f, -14.0 )) , glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0)), glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
			break;

		case( 4 ):
            PerVertex::viewingTransformation = glm::lookAt(dvec3(0.0, 0.0, 14.0), dvec3(0.0, 0.0, 0.0), dvec3(0.0, 1.0, 0.0)); 
			break;

		case( 5 ):
            PerVertex::viewingTransformation = glm::lookAt(dvec3(0.0, 7 * sqrt(2), 7 * sqrt(2)), dvec3(0.0, 0.0, 0.0), dvec3(0.0, 0.5, 0.0)); 
			break;

		case( 6 ):
            PerVertex::viewingTransformation = glm::lookAt(dvec3(0.0, 14.0, 0.0), dvec3(0.0, 0.0, 0.0), dvec3(1.0, 0.0, 0.0));
			break;

		default:
			std::cout << "Invalid view selection " << std::endl;
	}

	// Signal GLUT to call display callback
	glutPostRedisplay( );

} // end viewMenu


// To keep the console open on shutdown, start the project with Ctrl+F5 instead of just F5.
int main(int argc, char** argv)
{
	// freeGlut and Window initialization ***********************

	// initialize random seed
	srand((unsigned int)time(NULL));

	// Pass any applicable command line arguments to GLUT. These arguments
	// are platform dependent.
	glutInit(&argc, argv);

	// Set the initial display mode.
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);

	// Set the initial window size
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	// Create a window using a string and make it the current window.
	GLuint world_Window = glutCreateWindow("CSE 287 Project 2 - Dan Minik");

	// Indicate to GLUT that the flow of control should return to the program after
	// the window is closed and the GLUTmain loop is exited.
#warning uncomment next line
	//glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	// Callback for window redisplay
	glutDisplayFunc(RenderSceneCB);
	glutReshapeFunc(ResizeCB);
	glutKeyboardFunc(KeyboardCB);
	glutSpecialFunc(SpecialKeysCB);
	glutIdleFunc(animate);

    lights.push_back(&ambientLight);
    lights.push_back(&lightPos);
    lights.push_back(&lightDir);
    lights.push_back(&lightSpot);

	// Create polygon render submenu
	int polyMenuid = glutCreateMenu( polygonRenderMenu );
	// Specify menu items and integer identifiers
	glutAddMenuEntry( "Fill", 0 );
	glutAddMenuEntry( "Line", 1 );

	// Create view submenu
	int viewMenuid = glutCreateMenu( viewMenu );
	// Specify menu items and integer identifiers
	glutAddMenuEntry( "View 1", 1 );
	glutAddMenuEntry( "View 2", 2 );
	glutAddMenuEntry( "View 3", 3 );
	glutAddMenuEntry( "View 4", 4 );
	glutAddMenuEntry( "View 5", 5 );
	glutAddMenuEntry( "View 6", 6 );

	// Create viewport submenu
	int viewportMenuid = glutCreateMenu(viewPortMenu);
	// Specify menu items and integer identifiers
	glutAddMenuEntry("Full Screen", 1);
	glutAddMenuEntry("Vertical Split", 2);
	glutAddMenuEntry("Horizontal Split", 3);


	// Create main submenu
	int menu1id = glutCreateMenu( mainMenu );
	glutAddSubMenu( "Render", polyMenuid );
	glutAddSubMenu( "View", viewMenuid );
	glutAddSubMenu("Viewport", viewportMenuid);
	glutAddMenuEntry( "Quit", 0 );

#warning get lighting working
    // per fragment lighting false, the other one to true

	// Attach menu to right mouse button
	glutAttachMenu( GLUT_RIGHT_BUTTON );

	// Attach menu to right mouse button
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// Request that the window be made full screen
	//glutFullScreenToggle();

	color clearColor(0.0, 0.5, 1.0, 1.0);

	// Set red, green, blue, and alpha to which the color buffer is cleared.
	frameBuffer.setClearColor(clearColor);

	// ************* Object vertex coordinate initialization ***********************

	// Set the initial viewing tranformation for the scene
	PerVertex::viewingTransformation = glm::translate(dvec3(0.0, 0.0, -14.0));

	// Enter the GLUT main loop. Control will not return until the window is closed.
	glutMainLoop();

	return 0;

} // end main


