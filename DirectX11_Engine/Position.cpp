#include "Position.h"


CPosition::CPosition(void)
{
	m_nfFrameTIme = 0.0f;
	m_nfRotationX = 0.0f;
	m_nfRotationY = 0.0f;
	m_nfLeftTurnSpeed = 0.0f;
	m_nfRightTurnSpeed = 0.0f;
	m_nfTopTurnSpeed = 0.0f;
	m_nfBottomTurnSpeed = 0.0f;
}

CPosition::CPosition(const CPosition& otehr)
{

}

CPosition::~CPosition(void)
{
}

void CPosition::SetFrameTIme(float time)
{
	m_nfFrameTIme = time;
}

void CPosition::GetRotation(float& x, float& y)
{
	x = m_nfRotationX;
	y = m_nfRotationY;
}

void CPosition::TurnLeft(bool keydown)
{
	// if the key is pressed increase the speed at which the camera turns left.
	// if not slow down the turn speed.
	if(keydown)
	{
		m_nfLeftTurnSpeed += m_nfFrameTIme * 0.01f;

		if(m_nfLeftTurnSpeed > (m_nfFrameTIme * 0.15f))
			m_nfLeftTurnSpeed = m_nfFrameTIme * 0.15f;
	}
	else
	{
		m_nfLeftTurnSpeed -= m_nfFrameTIme * 0.005f;

		if(m_nfLeftTurnSpeed < 0.0f)
			m_nfLeftTurnSpeed = 0.0f;
	}

	// Update the rotation using the truning speed.
	m_nfRotationY -= m_nfLeftTurnSpeed;
	if(m_nfRotationY < 0.0f)
		m_nfRotationY += 360.0f;
}

void CPosition::TurnRight(bool keydown)
{
	// If the key is pressed increase the speed at which the camera turns right.
	// If not slow down the turn speed.
	if(keydown)
	{
		m_nfRightTurnSpeed += m_nfFrameTIme * 0.01f;

		if(m_nfRightTurnSpeed > (m_nfFrameTIme * 0.15f))
			m_nfRightTurnSpeed = m_nfFrameTIme * 0.15f;
	}
	else
	{
		m_nfRightTurnSpeed -= m_nfFrameTIme * 0.005f;

		if(m_nfRightTurnSpeed < 0.0f)
			m_nfRightTurnSpeed = 0.0f;
	}

	// Update the rotation using the turning speed.
	m_nfRotationY += m_nfRightTurnSpeed;
	if(m_nfRotationY > 360.0f)
		m_nfRotationY -= 360.0f;
}

void CPosition::TurnTop(bool keydown)
{
	if(keydown)
	{
		m_nfTopTurnSpeed += m_nfFrameTIme * 0.01f;

		if(m_nfTopTurnSpeed > (m_nfFrameTIme * 0.15f))
			m_nfTopTurnSpeed = m_nfFrameTIme * 0.15f;
	}
	else
	{
		m_nfTopTurnSpeed -= m_nfFrameTIme * 0.005f;

		if(m_nfTopTurnSpeed < 0.0f)
			m_nfTopTurnSpeed = 0.0f;
	}

	// Update the rotation using the truning speed.
	m_nfRotationX += m_nfTopTurnSpeed;
	if(m_nfRotationX < 0.0f)
		m_nfRotationX += 360.0f;
}
void CPosition::TurnBottom(bool keydown)
{
	if(keydown)
	{
		m_nfBottomTurnSpeed += m_nfFrameTIme * 0.01f;

		if(m_nfBottomTurnSpeed > (m_nfFrameTIme * 0.15f))
			m_nfBottomTurnSpeed = m_nfFrameTIme * 0.15f;
	}
	else
	{
		m_nfBottomTurnSpeed -= m_nfFrameTIme * 0.005f;

		if(m_nfBottomTurnSpeed < 0.0f)
			m_nfBottomTurnSpeed = 0.0f;
	}

	// Update the rotation using the turning speed.
	m_nfRotationX -= m_nfBottomTurnSpeed;
	if(m_nfRotationX > 360.0f)
		m_nfRotationX -= 360.0f;
}