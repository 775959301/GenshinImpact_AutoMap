#include "ATM_GiState.h"

bool ATM_GiState::isRun()
{
	giHandle = FindWindowA(NULL, "ԭ��");/* ��ԭ�񴰿ڵĲ��� */
	if (giHandle != NULL)
	{
		isRunning = true;
	}
	else
	{
		isRunning = false;
	}
	return isRunning;
}

void ATM_GiState::getHandle()
{
	giHandle = FindWindowA(NULL, "ԭ��");
	if (giHandle != NULL)
	{
		GetWindowRect(giHandle, &giRect);
	}
}
