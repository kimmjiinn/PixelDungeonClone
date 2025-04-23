#pragma once
#include "config.h"

class Camera
{
private:
	POINT centerP;
	int nowZoomScale;
	const float zoomScale[5] = { 1.0f, 1.5f, 2.0f, 2.5f, 3.0f };

	const int screenW[5] = {
		static_cast<int>(3240 / 1.0f),  // 3240
		static_cast<int>(3240 / 1.5f),  // 2160
		static_cast<int>(3240 / 2.0f),  // 1620
		static_cast<int>(3240 / 2.5f),  // 1296
		static_cast<int>(3240 / 3.0f)   // 1080
	};

	const int screenH[5] = {
		static_cast<int>(2160 / 1.0f),  // 2160
		static_cast<int>(2160 / 1.5f),  // 1440
		static_cast<int>(2160 / 2.0f),  // 1080
		static_cast<int>(2160 / 2.5f),  // 864
		static_cast<int>(2160 / 3.0f)   // 720
	};

	RECT screen;

	int offSetX;
	int offSetY;

	const float dragValue = 3.0f;


public:

	void Init(const FPOINT& playerPos);
	void Update();

	void ZoomIn();
	void ZoomOut();
	inline float GetZoomScale() { return zoomScale[nowZoomScale]; }
	inline float GetRevertedZoomScale() { return zoomScale[4 - nowZoomScale]; }

	void UpdateCenter(const FPOINT& playerPos);

	inline RECT GetScreen() { return screen; }

	inline int GetOffSetX() { return offSetX; }
	inline int GetOffSetY() { return offSetY; }

	int ConvertToRendererX(const int& worldNumX);
	int ConvertToRendererY(const int& worldNumY);
	int ConvertToWorldX(const int& redererNumX);
	int ConvertToWorldY(const int& redererNumY);

	Camera();
	~Camera();

};

