#define DEBUG

//================================================LIBRARY INCLUDE================================================
#include<iostream>
#include<vector>
#include<pgr.h>

//================================================MY INCLUDE================================================
#include "CHandleScene.h"

//================================================CAMERA================================================
CCamera camera(glm::vec3(0.0f,1.0f,10.0f), 70.0f, (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 0.01f, 1000.0f, basicCameraSpeed);	//!< Instance kamery ve svete aplikace

//================================================SCENE================================================

CHandleScene scene(maximumNumberOfPointLights);

//================================================MOUSE================================================
/*!
Holds together informations about mouse using
*/
struct SMouse {
	float lastXPos;
	float lastYPos;
	bool inUsing;
};

SMouse mouseData; //!< informace o pouzivani myse

//================================================OTHER================================================
GLuint shaderProgram = 0;	//!< Drzi id programu s nahranymi shadery
double time = 0.5;	//!< cas od zacatku programu
double fps = 0;	//!< informace o poctu fps
int frame = 0;	//!< citadlo pro vypocet fps


//================================================PRINT ERROR================================================

/**
  Prints error in format
  \param[in] message Message for error print
*/
void  printError(const std::string & message) {
	std::cout << "ERROR: " << message << std::endl;
}

//================================================LOAD SHADERS================================================
/**
  Load shaders from constant strings vertexShader, fragmentShader (parameters of program).
*/
bool loadShaders() {
	GLuint shaders[] = {
		pgr::createShaderFromFile(GL_VERTEX_SHADER, vertexShader.c_str()),
		pgr::createShaderFromFile(GL_FRAGMENT_SHADER, fragmentShader.c_str()),
		0
	};
	if (shaders[0] == 0 || shaders[1] == 0) {
		printError("Unable to load shaders from File.");
		return false;
	}

	shaderProgram = pgr::createProgram(shaders);
	if (shaderProgram == 0) {
		printError("Unable to create program with shaders.");
		return false;
	}

	return true;
}

//================================================INIT================================================
/**
	main init of program
	initializes buffers, textures, etc.
*/
void init() {
	mouseData.inUsing = false;


	glClearColor(0.2f, 0.1f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

	if (!loadShaders()) {
		std::cout << "wasnt able to load shaders" << std::endl;
	}

	//INIT OBEJEKTU A SVETEL
	scene.init(shaderProgram);


	//UNIFORM
	camera.init(shaderProgram);

	CHECK_GL_ERROR();

}
//================================================DRAW================================================
/**
	main draw of program
	draws all on display
*/
void draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgram);
	


	camera.draw(time);

	//glActiveTexture(GL_TEXTURE0);
	scene.draw(shaderProgram,time);

	CHECK_GL_ERROR();
	glutSwapBuffers();
}


//================================================MOUSE CLICK================================================
/**
	Used for capture if is mouse clicked
*/
void mouseClick(int button, int state, int x, int y) {
	int id = 0;
	static int lastx;
	static int lasty;
	if (state == GLUT_DOWN) {
		lastx = x;
		lasty = y;
	}

	if (state == GLUT_UP) {
		if ( lastx -2 <  x && x < lastx + 2 && lasty - 2 < y && y < lasty + 2) {
			glReadPixels(x, WINDOW_HEIGHT - 1 - y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &id);
			//std::cout << "KLIKNUTO NA OBJEKT S ID" << id << std::endl;
			scene.objectEcho(id, time);
		}
	}
	
	mouseData.inUsing = false;
	
}

//================================================MOUSE MOVE================================================
/**
	recognize where is mouse, when mouse is clicked
*/
void mouseClickMove( int xPos, int yPos)
{

	if (!mouseData.inUsing) {
mouseData.lastXPos = (float)xPos;
mouseData.lastYPos = (float)yPos;
mouseData.inUsing = true;
	}

	float xoffset = xPos - mouseData.lastXPos;
	float yoffset = mouseData.lastYPos - yPos;

	mouseData.lastXPos = (float)xPos;
	mouseData.lastYPos = (float)yPos;

	float sensitivity = 0.5;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	camera.yaw += xoffset;
	camera.pitch += yoffset;

	if (camera.pitch > 89.0f)
		camera.pitch = 89.0f;
	if (camera.pitch < -89.0f)
		camera.pitch = -89.0f;

	if (camera.yaw > 360.0f)
		camera.yaw -= 360.0f;
	if (camera.yaw < 0.0f)
		camera.yaw += 360.0f;

	glm::vec3 front;
	front.x = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
	front.y = sin(glm::radians(camera.pitch));
	front.z = -cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
	camera.setFront(glm::normalize(front));
}

//================================================ON TIMER / FPS================================================
/**
	Get the time elapsed from begining and get fps
*/
int lastTime = 0;
void onTimer(int) {
	glutPostRedisplay();
	glutTimerFunc(refreshTimeMs, onTimer, 0);

	int timeMs = glutGet(GLUT_ELAPSED_TIME);
	time = timeMs * 0.001f;

	++frame;
	if (timeMs - lastTime > 1000) {
		fps = frame * 1000.0 / (timeMs - lastTime);
		lastTime = timeMs;
		frame = 0;
	}
}

//================================================ON KEY================================================
/**
	bind functions for keys
*/
void onKey(unsigned char key, int, int) {
	switch (key) {
	case 27:
		glutLeaveMainLoop();
		break;
	case moveBackwardKey:
		camera.CameraMoveBackward();
		break;
	case moveLeftKey:
		camera.CameraMoveLeft();
		break;
	case moveRightKey:
		camera.CameraMoveRight();
		break;
	case moveForwardKey:
		camera.CameraMoveForward();
		break;
	case moveUpKey:
		camera.CameraMoveUp();
		break;
	case moveDownKey:
		camera.CameraMoveDown();
		break;
	case freeCameraKey:
		camera.changeViewType(CCamera::EViewState::FREE, time);
		break;
	case firstViewKey:
		camera.changeViewType(CCamera::EViewState::LOCK_ONE, time);
		break;
	case secondViewKey:
		camera.changeViewType(CCamera::EViewState::LOCK_TWO, time);
		break;
	case flashLightOnOff:
		scene.enableDisableFlashLight();
		break;
	case crazyModKey1:
		scene.howCrazy(60, 56);
		break;
	case crazyModKey2:
		scene.howCrazy(35, 32);
		break;
	case crazyModKey3:
		scene.howCrazy(10, 5);
		break;
	case crazyModKey4:
		scene.howCrazy(6, 5);
		break;
	}
}
//================================================ON SPECIAL KEY================================================
void onSpecialKey(int key, int, int){
	switch (key) {
	case crazyModKey1:
		scene.howCrazy(60, 56);
		break;
	case crazyModKey2:
		scene.howCrazy(35, 32);
		break;
	case crazyModKey3:
		scene.howCrazy(10, 5);
		break;
	case crazyModKey4:
		scene.howCrazy(6, 5);
		break;
	case moveBackwardKey:
		camera.CameraMoveBackward();
		break;
	case moveLeftKey:
		camera.CameraMoveLeft();
		break;
	case moveRightKey:
		camera.CameraMoveRight();
		break;
	case moveForwardKey:
		camera.CameraMoveForward();
		break;
	case moveUpKey:
		camera.CameraMoveUp();
		break;
	case moveDownKey:
		camera.CameraMoveDown();
		break;
	}
}

//================================================MAIN================================================
int main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitContextVersion(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow(WINDOW_TITLE.c_str());

	/// nastaveni funkci v glutu
	glutKeyboardFunc(onKey);
	glutSpecialFunc(onSpecialKey);
	glutMotionFunc(mouseClickMove);
	glutMouseFunc(mouseClick);
	glutTimerFunc(refreshTimeMs, onTimer, 0);
	glutDisplayFunc(draw);

	if (!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR))
		pgr::dieWithError("pgr init failed, required OpenGL not supported?");

	scene.addObjects();

	init();
	std::cout << "zacatek programu!" << std::endl;

	glutMainLoop();
	return 0;
}





// zeptat se na textury, co nejspis blbne
// zeptat se na podivnosti s rotacnimi maticemi
// ZEPTAT SE CO JE pgr::dieWithError a celkove jestli se da najit dokumentace k pgr
// zeptat se na doxygen a nepodporu pro visual studio http://www.doxygen.nl/manual/docblocks.html
// zeptat se na chybu s objloaderem
// zeptat se na onTimer
