#include "CHandleScene.h"

const glm::vec3 p4 = glm::vec3(0.0f, -0.2f, 2.5f);
const glm::vec3 p3 = glm::vec3(0.0f, 2.0f, 2.5f);
const glm::vec3 p2 = glm::vec3(0.0f, 2.0f, 1.5f);
const glm::vec3 p1 = glm::vec3(0.0f, 0.7f, 0.7f);

double CHandleScene::rand0812() {
	double tmp = double(rand()) / (double(RAND_MAX) + 1.0);
	tmp = (tmp / 2.5) + 0.8;
	std::cout << tmp << std::endl;
	return tmp;
}


glm::vec3 CHandleScene::cubicBezier(float u, const glm::vec3 & cp1, const glm::vec3 & cp2, const glm::vec3 & cp3,const glm::vec3 & cp4) {
	glm::vec3 ret;
	float u3 = u * u* u;
	float u2 = u * u;
	float oneMinusU = (1 - u);
	float oneMinusU2 = (1 - u) * (1 - u);
	float oneMinusU3 = (1 - u) * (1 - u) * (1 - u);

	ret.x = cp4.x *u3 +		3 * cp3.x * u2 * oneMinusU +	3 * cp2.x * u * oneMinusU2 +	cp1.x * oneMinusU3;
	ret.y = cp4.y *u3 +		3 * cp3.y * u2 * oneMinusU +	3 * cp2.y * u * oneMinusU2 +	cp1.y * oneMinusU3;
	ret.z = cp4.z *u3 +		3 * cp3.z * u2 * oneMinusU +	3 * cp2.z * u * oneMinusU2 +	cp1.z * oneMinusU3;

	return ret;
}

void CHandleScene::setBezierAlfa(SAnanasPiece * piece, double time) {

	float u = (float)(time - piece->startTime) / 2;
	if (u >= 0.999f) {
		u = 0.999f;
		piece->move = false;
	}
	piece->u = u;
}

void CHandleScene::checkLife(SAnanasPiece * piece, double time) {
	if ((time - piece->startTime) > 20.0f) {
		piece->alive = false;
	}
}

void CHandleScene::handleLife(unsigned int shaderProgram, double time) {
	int born = std::rand() % 60;
	if(born == 56){
		ananasPieces[aCounter % ANANASPIECES_MAX_COUNT].piece = CObject(&(referencePiece));
		ananasPieces[aCounter % ANANASPIECES_MAX_COUNT].alive = true;
		ananasPieces[aCounter % ANANASPIECES_MAX_COUNT].move = true;
		ananasPieces[aCounter % ANANASPIECES_MAX_COUNT].startTime = time;
		ananasPieces[aCounter % ANANASPIECES_MAX_COUNT].piece.rotate((float)(std::rand() % 360));
		ananasPieces[aCounter % ANANASPIECES_MAX_COUNT].p4 = p4;
		ananasPieces[aCounter % ANANASPIECES_MAX_COUNT].p3 = p3;
		ananasPieces[aCounter % ANANASPIECES_MAX_COUNT].p4.z *= rand0812();
		ananasPieces[aCounter % ANANASPIECES_MAX_COUNT].p3.z = ananasPieces[aCounter % ANANASPIECES_MAX_COUNT].p4.z;
		std::cout << ananasPieces[aCounter % ANANASPIECES_MAX_COUNT].p4.x << " " << ananasPieces[aCounter % ANANASPIECES_MAX_COUNT].p4.y
			<< " " << ananasPieces[aCounter % ANANASPIECES_MAX_COUNT].p4.z  <<std::endl;
		++aCounter;
	}

	for (int i = 0; i < aCounter && i < ANANASPIECES_MAX_COUNT; ++i) {
		if (ananasPieces[i].move) {
			setBezierAlfa(&(ananasPieces[i]), time);
			glm::vec3 tmp = cubicBezier(ananasPieces[i].u, p1, p2,
				ananasPieces[i].p3, ananasPieces[i].p4);
			//glm::vec3 tmp = cubicBezier(ananasPieces[i].u, p1, p2, p3, p4);
			ananasPieces[i].piece.changePosition(tmp);
		}
		checkLife(&(ananasPieces[i]), time);
	}

}




CHandleScene::CHandleScene(unsigned int maxCountOfLights) :light(glm::vec3(3.0, 1.0, 0.0), maxCountOfLights)
{
	aCounter = 0;
	objects = std::vector<CObject>();
}


CHandleScene::~CHandleScene()
{
}

void CHandleScene::init(unsigned int shaders)
{
	light.init(shaders);
	for (auto & it : objects) {
		it.init(shaders);
	}

	referencePiece = CObject(CObject(CObject::EObjectType::ANANAS_PIECE, "ananasPiece.obj", "ananasPiece.png", glm::vec3(-3.0f, 1.0f, 0.0f)));
	referencePiece.init(shaders);
}

void CHandleScene::draw(unsigned int shaders, double time)
{
	handleLife(shaders,time);

	light.draw(time);
	for (auto & it : objects) {
		it.draw();
	}

	for (int i = 0; i < aCounter && i < ANANASPIECES_MAX_COUNT; ++i) {
		if (ananasPieces[i].alive) {
			ananasPieces[i].piece.draw();
		}
	}
}
