#include "SPGM.h"
#include "SCharacter.h"
#include "SPC.h"
#include "SpartaGameState.h"

ASPGM::ASPGM()
{
	DefaultPawnClass = ASPGM::StaticClass();
	PlayerControllerClass = ASPC::StaticClass();
	GameStateClass = ASpartaGameState::StaticClass();

}