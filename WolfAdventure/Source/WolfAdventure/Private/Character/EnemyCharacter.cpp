// Made by Luis Plasencia Pulido


#include "Character/EnemyCharacter.h"
#include <WolfAdventure/WolfAdventure.h>

void AEnemyCharacter::HighLightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AEnemyCharacter::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
}
