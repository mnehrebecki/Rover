#include "Rover.h"



Rover::Rover()
{
}


Rover::~Rover()
{
}

void Rover::draw()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	GLfloat centerRR[3] = { -50.0f,15.0f,40.0f };				//wsp œrodków kó³
	GLfloat centerFR[3] = { 50.0f,15.0f,40.0f };
	GLfloat centerRL[3] = { -50.0f,15.0f,-40.0f };
	GLfloat centerFL[3] = { 50.0f,15.0f, -40.0f };

	Wheel wheelRR(15, 15, centerRR);							//ko³a
	Wheel wheelFR(15, 15, centerFR);
	Wheel wheelRL(15, 15, centerRL);
	Wheel wheelFL(15, 15, centerFL);

	GLfloat beginningRAxis[3] = { -55.0f,35.0f, -20.0f };		//wsp pocz¹tku prostok¹tów
	Box rAxis(10, 5, 55, beginningRAxis);						//oœ tylna

	GLfloat beginningRLSuspension[3] = { -55.0f,10.0f, -25.0f };
	Box rlSuspension(10, 30, 5, beginningRLSuspension);			//³¹czenie kó³ ty³

	GLfloat beginningRRSuspension[3] = { -55.0f,10.0f, 35.0f };
	Box rrSuspension(10, 30, 5, beginningRRSuspension);

	GLfloat beginningFAxis[3] = { 45.0f,35.0f, -20.0f };
	Box fAxis(10, 5, 55, beginningFAxis);						//oœ przednia

	GLfloat beginningFLSuspension[3] = { 45.0f,10.0f, -25.0f };
	Box flSuspension(10, 30, 5, beginningFLSuspension);			//³¹czenie kó³ przód

	GLfloat beginningFRSuspension[3] = { 45.0f,10.0f, 35.0f };
	Box frSuspension(10, 30, 5, beginningFRSuspension);

	GLfloat beginningBody[3] = { -60.0f,40.0f, -20.0f };
	Box body(120, 10, 55, beginningBody);						//pod³oga

	GLfloat centerRLamp[3] = { 60.0f,45.0f, -15.0f };
	Lamp rLamp(5, 5, centerRLamp);									//œwiat³o prawe

	GLfloat centerCLamp[3] = { 60.0f,45.0f, 7.5f };
	Lamp cLamp(5, 5, centerCLamp);									//œwiat³o œrodkowe

	GLfloat centerLLamp[3] = { 60.0f,45.0f, 30.0f };
	Lamp lLamp(5, 5, centerLLamp);									//œwiat³o lewe

	GLfloat centerStick[3] = { -35.0f,50.0f, 7.5f };
	Roller stick(3, 55, centerStick);								//dr¹¿ek kamery

	GLfloat beginningRCameraHandle[3] = { -38.0f, 102.0f, 10.5f };
	Box rCameraHandle(6, 10, 2, beginningRCameraHandle);			//mocowanie kamery

	GLfloat beginningLCameraHandle[3] = { -38.0f, 102.0f, 2.5f };
	Box lCameraHandle(6, 10, 2, beginningLCameraHandle);

	GLfloat beginningCamera[3] = { -41.0f, 107.0f, 4.5f };
	Box camera(12, 6, 6, beginningCamera);							//kamera

	GLfloat centerLens[3] = { -29.0f,111.0f, 7.5f };
	Lamp lens(2, 2, centerLens);									//obiektyw kamery

	GLfloat beginningAntenna[3] = { -50.0f,50.0f, 25.0f };
	Roller antenna(0.35f, 80, beginningAntenna);

	GLfloat centerArmStick[3] = { 50.0f,50.0f, 7.5f };
	Roller armStick(3, 30, centerArmStick);								//dr¹¿ek ramienia

	GLfloat beginningRRArmHandler[3] = { 47.0f, 77.0f, 10.5f };
	Box rrArmHandler(6, 10, 2, beginningRRArmHandler);					//mocowanie ramienia

	GLfloat beginningRLArmHandler[3] = { 47.0f, 77.0f, 2.5f };
	Box rlArmHandler(6, 10, 2, beginningRLArmHandler);

	GLfloat centerArm[3] = { 47.0f, 85.0f, 7.5f };
	Lamp arm(3, 40, centerArm);

	GLfloat centerArm2[3] = { 91.0f, 85.0f, 7.5f };
	Lamp arm2(3, 40, centerArm2);

	GLfloat beginningFRArmHandler[3] = { 84.0f, 82.0f, 10.5f };
	Box frArmHandler(10, 6, 2, beginningFRArmHandler);						//mocowanie ramienia

	GLfloat beginningFLArmHandler[3] = { 84.0f, 82.0f, 2.5f };
	Box flArmHandler(10, 6, 2, beginningFLArmHandler);

	GLfloat beginningRFinger[3] = { 128.0f, 82.0f, 10.5f };
	Box rFinger(10, 6, 2, beginningRFinger);								//mocowanie ramienia

	GLfloat beginningLFinger[3] = { 128.0f, 82.0f, 2.5f };
	Box lFinger(10, 6, 2, beginningLFinger);

	wheelRR.draw();				
	wheelFR.draw();
	wheelRL.draw();
	wheelFL.draw();

	rAxis.draw();
	rlSuspension.draw();
	rrSuspension.draw();
	fAxis.draw();
	flSuspension.draw();
	frSuspension.draw();

	body.draw();

	rLamp.draw();
	cLamp.draw();
	lLamp.draw();

	stick.draw();
	lCameraHandle.draw();
	rCameraHandle.draw();
	camera.draw();
	lens.draw();
	
	antenna.draw();

	armStick.draw();

	rrArmHandler.draw();
	rlArmHandler.draw();
	arm.draw();
	arm2.draw();
	frArmHandler.draw();
	flArmHandler.draw();
	rFinger.draw();
	lFinger.draw();
}