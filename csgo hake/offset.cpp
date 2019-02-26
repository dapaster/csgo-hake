#include "offset.h"

unsigned int offset::modules::panoramadll;
unsigned int offset::modules::enginedll;



unsigned int offset::dynamic::localPlayer;
unsigned int offset::dynamic::baseClientState;
unsigned int offset::dynamic::entityListBase;
unsigned int offset::dynamic::glowObjectManager;
float offset::dynamic::intervalBetweenTick;
std::deque<unsigned int> offset::dynamic::entityList;
std::deque<int> offset::dynamic::entityTeamMap;

void* offset::dynamic::localJumpAddr;
void* offset::dynamic::localStatusAddr;
