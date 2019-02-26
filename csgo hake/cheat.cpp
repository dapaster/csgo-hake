#include "cheat.h"

bool cheat::isOn = false;
bool cheat::once = true;

float recoilCompensationFactor = 2.f;
bool predictAim = false;

unsigned char playerStatus;
int localTeam;

void cheat::handleBhop()
{
	if (GetAsyncKeyState(VK_SPACE) < 0)
	{
		playerStatus = mem::read<unsigned char>(offset::dynamic::localStatusAddr);

		if (playerStatus == 1 || playerStatus == 7)
		{
			mem::write<unsigned char>(offset::dynamic::localJumpAddr, offset::flag::JUMP);
		}
	}
}

int id = 0;
float angles[2]; // {pitch, yaw}
unsigned char offsetAngleArr[sizeof(float) * 2];

struct
{
	float pitch = 0;
	float yaw = 0;
} offsetAngle;

void cheat::handleAimbot()
{
	if (!(GetAsyncKeyState(VK_XBUTTON2) < 0))
		return;


	unsigned int currplayer = offset::dynamic::entityList[id];

	while (mem::read<int>((void*)(currplayer + offset::player::health)) <= 0 || offset::dynamic::entityTeamMap[id] == localTeam)
	{

		if (!(GetAsyncKeyState(VK_XBUTTON2) < 0))
			return;

		id++;
		id %= offset::dynamic::entityList.size();
		currplayer = offset::dynamic::entityList[id];
	}


	unsigned char myplayerVecArr[vec3::size];
	mem::read((void*)(offset::dynamic::localPlayer + offset::player::vecOriginX), vec3::size, myplayerVecArr);
	vec3 myplayer = vec3::parse(myplayerVecArr);
	myplayer.z += mem::read<float>((void*)(offset::dynamic::localPlayer + offset::player::eyeZOffset));

	unsigned int cpBonePtr = mem::read<unsigned int>((void*)(currplayer + offset::BMOFFSET));

	unsigned char enemyplayerVecArr[offset::BM_HEAD_X * 3];
	mem::read((void*)(cpBonePtr + offset::BM_HEAD_X), offset::BM_HEAD_X * 3, enemyplayerVecArr);
	vec3 enemyplayer = vec3::parse(enemyplayerVecArr, offset::BM_COORDOFFSET);

	if (predictAim)
	{
		unsigned char enemyVelArr[vec3::size];
		mem::read((void*)(currplayer + offset::player::velocityX), vec3::size, enemyVelArr);
		vec3 enemyVel = vec3::parse(enemyVelArr);

		enemyplayer = enemyplayer + (enemyVel * offset::dynamic::intervalBetweenTick);
	}

	angle aimangle = vec3::calcAngle(enemyplayer - myplayer);


	mem::read((void*)(offset::dynamic::localPlayer + offset::LP_AIMPUNCHPITCH), sizeof(float) * 2, offsetAngleArr);
	for (int i = 0; i < 2; i++)
	{
		unsigned char a[4];
		for (int v = 0; v < 4; v++)
			a[v] = offsetAngleArr[i * 4 + v];
		
		if (i == 0)
			memcpy(&offsetAngle.pitch, a, 4);
		else
			memcpy(&offsetAngle.yaw, a, 4);

	}

	aimangle.pitch -= offsetAngle.pitch * recoilCompensationFactor;
	aimangle.yaw -= offsetAngle.yaw * recoilCompensationFactor;

	angles[0] = aimangle.pitch;
	angles[1] = aimangle.yaw;

	mem::write((void*)(offset::dynamic::baseClientState + offset::BCS_PITCH), sizeof(float) * 2, angles);
}

float coloure[] = { .8f, 0, .8f, .7f };
float colourf[] = { 0, .8f, 0, .7f };
bool flags[] = { true, false };

void cheat::handleWallhack()
{
	for (unsigned int i = 0; i < offset::dynamic::entityList.size(); i++)
	{
		int glowIndex = mem::read<int>((void*)(offset::dynamic::entityList[i] + offset::player::glowIndex));

		mem::write((void*)(offset::dynamic::glowObjectManager + glowIndex * 0x38 + offset::glowObject::r), sizeof(float) * 4, offset::dynamic::entityTeamMap[i] == localTeam ? colourf : coloure);

		mem::write((void*)(offset::dynamic::glowObjectManager + glowIndex * 0x38 + offset::glowObject::trueflag), sizeof(bool) * 2, flags);
	}
}

void cheat::getTeam()
{
	localTeam = mem::read<int>((void*)(offset::dynamic::localPlayer + offset::player::teamNum));
}