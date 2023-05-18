#include "UW_ExploreCursor.h"

#include "Components/Image.h"


void UUW_ExploreCursor::ChangeCursor(EExploreCursorType ExploreCursorType)
{
	if (ExploreCursorType == EExploreCursorType::Default)
	{
		CursorImage->SetBrushFromTexture(DefaultCursorImage, false);
	}
	else if (ExploreCursorType == EExploreCursorType::Enemy)
	{
		CursorImage->SetBrushFromTexture(EnemyCursorImage, false);
	}
	else if (ExploreCursorType == EExploreCursorType::Inventory)
	{
		CursorImage->SetBrushFromTexture(InventoryCursorImage, false);
	}
}
