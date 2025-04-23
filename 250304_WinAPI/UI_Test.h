#pragma once
#include "config.h"

class D2DImage;
class UITestView;
class UI_TestScene
{
private:
public:
	virtual HRESULT Init() ;
	virtual void Release() ;
	virtual void Update() ;
	virtual void Render() ;

public:
	UITestView* testView;
};

