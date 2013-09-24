#include "Body.h"
#ifndef SCENE_H
#define	SCENE_H

using namespace std;

class Scene {
	int fileNumber;
	int NumOfBodies;
	Body body1;
	Body body2;
	const char *ContCond;
	bool inContact;
public:
	Scene();
	void Init(int NumOfBodies, const char * _ContCond, bool _inContact);
	int doNextStep(float maxTau, int methodType);
	~Scene();
};

#endif	/* SCENE_H */

