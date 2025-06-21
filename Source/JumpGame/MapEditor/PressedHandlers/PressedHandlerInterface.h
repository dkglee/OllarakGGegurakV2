#pragma once

// UI, Gizmo, Actor, Background 클릭시 수행
#include "JumpGame/MapEditor/ClickHandlers/ClickResponse.h"
#include "GizmoPressedInfo.h"

class JUMPGAME_API IPressedHandler
{
public:
	IPressedHandler() {};
	virtual int32 GetPriority() const = 0;
	virtual bool HandlePressed(FClickResponse& PressedResponse, class AMapEditingPlayerController* PlayerController, const FGizmoPressedInfo& GizmoPressedInfo) = 0;
	virtual ~IPressedHandler() {};
};
