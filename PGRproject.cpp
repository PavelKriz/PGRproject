#define DEBUG

//================================================LIBRARY INCLUDE================================================
#include<iostream>
#include<vector>
#include<pgr.h>

//================================================MY INCLUDE================================================
#include "CHandleScene.h"

namespace krizpav6 {

	//================================================CAMERA================================================
	CCamera camera(glm::vec3(0.0f, 1.0f, 10.0f), 70.0f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01f, 1000.0f, basicCameraSpeed);	//!< Instance kamery ve svete aplikace

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

	SMouse mouseData; //!< inforamtions about mouse using

	//================================================OTHER================================================
	GLuint shaderProgram = 0;	//!< holds id of shaders
	double time = 0.5;	//!< time from beginning of program
	double fps = 0;	//!< information about fps
	int frame = 0;	//!< counter for calculating fps


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
	  \return true if shaders were loaded or false when they werent loaded
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
		scene.draw(time);

		CHECK_GL_ERROR();
		glutSwapBuffers();
	}


	//================================================MOUSE CLICK================================================
	/**
		Used for capture if is mouse clicked
		\param[in] button defines which mouse button was pressed
		\param[in] state clicked(down) or end of clicked(up)
		\param[in] x mouse coordinate
		\param[in] y mouse coordinate
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
			if (lastx - 2 < x && x < lastx + 2 && lasty - 2 < y && y < lasty + 2) {
				glReadPixels(x, WINDOW_HEIGHT - 1 - y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &id);
				scene.objectEcho(id, time);
			}
		}

		mouseData.inUsing = false;

	}

	//================================================MOUSE MOVE================================================
	/**
		recognize where is mouse, when mouse is clicked
		it serve to move view around the world
		\param[in] x mouse coordinate
		\param[in] y mouse coordinate
	*/
	void mouseClickMove(int xPos, int yPos)
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
		\param[in] key which key was pressed
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
	/**
		bind functions for special keys
		\param[in] key which key was pressed
	*/
	void onSpecialKey(int key, int, int) {
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
		case moveBackwardKey2:
			camera.CameraMoveBackward();
			break;
		case moveLeftKey2:
			camera.CameraMoveLeft();
			break;
		case moveRightKey2:
			camera.CameraMoveRight();
			break;
		case moveForwardKey2:
			camera.CameraMoveForward();
			break;
		}
	}

	//================================================MAIN================================================
	/**
		main2 is main function of program
	*/
	int main2(int argc, char** argv) {
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
}

int main(int argc, char** argv) { 
	krizpav6::main2(argc, argv);
}