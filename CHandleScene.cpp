#include "CHandleScene.h"

const glm::vec3 p4 = glm::vec3(0.0f, -0.23f, 2.5f);
const glm::vec3 p3 = glm::vec3(0.0f, 2.0f, 2.5f);
const glm::vec3 p2 = glm::vec3(0.0f, 2.0f, 1.5f);
const glm::vec3 p1 = glm::vec3(0.0f, 0.7f, 0.7f);

double CHandleScene::rand0812() {
	double tmp = double(rand()) / (double(RAND_MAX) + 1.0);
	tmp = (tmp / 2.5) + 0.8;
	//std::cout << tmp << std::endl;
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
		if (piece->alive) {
			bornExplosionOnPizza(piece->angle, time, piece->p4);
		}
		piece->move = false;
	}
	piece->u = u;
}

void CHandleScene::handleExplosions(double time) {
	int id = 0;
	std::vector<SExplosion> toStay;
	for (auto & it : explosions ) {
		float live = (float)(time - it.startTime) * 2;
		int frame = (int) floor(live * 16);
		it.explosion.setTexFrame(frame);


		if (pizzaRotation) {
			it.explosion.constRotate();
		}
		if (live < 1.0) {
			toStay.push_back(it);
		}
	}

	explosions = toStay;
}

void CHandleScene::bornExplosion(float angle, double time) {
	SExplosion tmp;
	tmp.explosion = CObject(&(referenceExplosion));
	tmp.startTime = time;
	tmp.frame = 0;
	explosions.push_back(tmp);
	explosions.back().explosion.rotate(angle);
	explosions.back().explosion.modelRotate();
	explosions.back().explosion.setScale(0.3f);
}

void CHandleScene::bornExplosionOnPizza(float angle, double time, glm::vec3  position) {
	SExplosion tmp;
	tmp.explosion = CObject(&(referenceExplosion));
	tmp.startTime = time;
	tmp.frame = 0;
	explosions.push_back(tmp);
	position.y += 0.1f;
	explosions.back().explosion.changePosition(position);
	explosions.back().explosion.rotate(angle);
	explosions.back().explosion.setScale(0.5f);
}

void CHandleScene::bornAnanasPiece(double time)
{
	float rotationAngle;
	int index = aCounter % ANANASPIECES_MAX_COUNT;
	if (ananasPieces[index].light != -1 && aCounter >= ANANASPIECES_MAX_COUNT) {
		light.endLightPoint(ananasPieces[index].light);
	}
	ananasPieces[index].piece = CObject(&(referencePiece));
	ananasPieces[index].alive = true;
	ananasPieces[index].move = true;
	ananasPieces[index].startTime = time;
	ananasPieces[index].piece.rotate(rotationAngle = (float)(std::rand() % 360));
	ananasPieces[index].angle = rotationAngle;
	ananasPieces[index].p4 = p4;
	ananasPieces[index].p3 = p3;
	ananasPieces[index].p4.z *= (float)rand0812();
	ananasPieces[index].p3.z = ananasPieces[index].p4.z;
	ananasPieces[index].piece.setScale(0.1f);
	//std::cout << ananasPieces[index].p4.x << " " << ananasPieces[index].p4.y
	//	<< " " << ananasPieces[index].p4.z << std::endl;

	ananasPieces[index].light = light.addPointLight(ananasPieces[index].piece.getPosition());
	if (ananasPieces[index].light != -1) {
		light.udpate(ananasPieces[index].light, rotationAngle);
	}
	std::cout << "ID vracene " << ananasPieces[index].light << std::endl;
	bornExplosion(rotationAngle, time);
	//ENDTASK zalozit novy objekt vybuch a dat mu spravny smer pohledu 

	++aCounter;
}

void CHandleScene::killAnanasPiece(SAnanasPiece * piece)
{
	if (piece->light != -1) {
		light.endLightPoint(piece->light);
	}
	piece->light = -1;
	piece->alive = false;
}

void CHandleScene::checkLife(SAnanasPiece * piece, double time) {
	if ((time - piece->startTime) > 20.0f) {
		killAnanasPiece(piece);
	}
}

void CHandleScene::handleLife(unsigned int shaderProgram, double time) {
	glm::mat4 tmpRotation;
	if (pizzaRotation) {
		for (auto &it : objects) {
			if (it.getType() == CObject::EObjectType::PIZZA) {
				it.constRotate();
				tmpRotation = it.getRotationM();
			}
		}
	}

	int born = std::rand() % 60;
	if(born == 56){
		bornAnanasPiece(time);
	}

	for (int i = 0; i < aCounter && i < ANANASPIECES_MAX_COUNT; ++i) {
		if (ananasPieces[i].move) {
			setBezierAlfa(&(ananasPieces[i]), time);
			glm::vec3 tmp = cubicBezier(ananasPieces[i].u, p1, p2,
				ananasPieces[i].p3, ananasPieces[i].p4);
			//glm::vec3 tmp = cubicBezier(ananasPieces[i].u, p1, p2, p3, p4);
			ananasPieces[i].piece.changePosition(tmp);
			if (ananasPieces[i].light != -1) {
				light.udpate(ananasPieces[i].light, tmp);
			}
		}
		else {
			if (pizzaRotation) {
				ananasPieces[i].piece.constRotate();
				if (ananasPieces[i].light != -1) {
					light.udpate(ananasPieces[i].light, 0.5f);
				}
			}
		}
		checkLife(&(ananasPieces[i]), time);
	}

}




CHandleScene::CHandleScene(unsigned int maxCountOfLights) :light(glm::vec3(3.0, 1.0, 0.0), maxCountOfLights)
{
	aCounter = 0;
	pizzaRotation = true;
	objects = std::vector<CObject>();
	explosions = std::vector<SExplosion>();
}


CHandleScene::~CHandleScene()
{
}

void CHandleScene::objectEcho(int id, double time)
{
	if (id < 100) {
		if (id == CObject::EObjectType::ANANAS) {
			bornAnanasPiece(time);
		}
		else if (id == CObject::EObjectType::PIZZA) {
			if (pizzaRotation) {
				pizzaRotation = false;
			}
			else {
				pizzaRotation = true;
			}
		}
		else if (id == CObject::EObjectType::SKYBOX) {
			;
		}
	}
	else if (id >= 100) {
		id -= 100;
		killAnanasPiece(&(ananasPieces[id]));
	}
}

void CHandleScene::init(unsigned int shaders)
{
	light.init(shaders);
	for (auto & it : objects) {
		it.init(shaders);
	}

	referencePiece = CObject(CObject::EObjectType::ANANAS_PIECE, "ananasPiece.obj", "ananasPiece.png", glm::vec3(-3.0f, 1.0f, 0.0f));
	referencePiece.init(shaders);
	referenceExplosion = CObject(CObject::EObjectType::EXPLOSION, "explosion.obj", "explosion2.png",glm::vec3(0.0f, 0.75f, 0.75f));
	referenceExplosion.init(shaders);
}

void CHandleScene::draw(unsigned int shaders, double time)
{
	handleLife(shaders,time);
	handleExplosions(time);

	light.draw(time);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	int id = 0;
	
	for (auto & it : objects) {
		glStencilFunc(GL_ALWAYS, (int) it.getType() , 255);
		it.draw();
		++id;
	}
	

	id = 100;
	for (int i = 0; i < aCounter && i < ANANASPIECES_MAX_COUNT; ++i) {
		if (ananasPieces[i].alive) {
			glStencilFunc(GL_ALWAYS, id, 255);
			ananasPieces[i].piece.draw();
		}
		++id;
	}
	glDisable(GL_STENCIL_TEST);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (auto & it : explosions) {
		it.explosion.draw();
	}
	glDisable(GL_BLEND);
	CHECK_GL_ERROR();
}
