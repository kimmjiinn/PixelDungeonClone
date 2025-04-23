#include "Camera.h"

void Camera::Init(const FPOINT& playerPos)
{
	nowZoomScale = 3;
	centerP = { (long)playerPos.x, (long)playerPos.y };

	screen = { centerP.x - screenW[nowZoomScale] / 2,
			   centerP.y - screenH[nowZoomScale] / 2,
			   centerP.x + screenW[nowZoomScale] / 2,
			   centerP.y + screenH[nowZoomScale] / 2 };

	offSetX = centerP.x - screenW[nowZoomScale] / 2;
	offSetY = centerP.y - screenH[nowZoomScale] / 2;
}

void Camera::Update()
{
	if (MouseManager::GetInstance()->GetIsDragging(MOUSE_LEFT))
	{
		long tempDeltaX = MouseManager::GetInstance()->GetDeltaX();
		long tempDeltaY = MouseManager::GetInstance()->GetDeltaY();

		float scale = zoomScale[nowZoomScale];
		centerP.x -= static_cast<float>(tempDeltaX) / scale * dragValue;
		centerP.y -= static_cast<float>(tempDeltaY) / scale * dragValue;

	}

	if (MouseManager::GetInstance()->GetWheelDelta() != 0) {
		short wd = MouseManager::GetInstance()->GetWheelDelta();

		if (wd > 0) {
			ZoomIn();
		}
		else if (wd < 0) {
			ZoomOut();
		}
		MouseManager::GetInstance()->SetWheelDelta(0.0f);
	}

	screen = { centerP.x - screenW[nowZoomScale] / 2,
			   centerP.y - screenH[nowZoomScale] / 2,
			   centerP.x + screenW[nowZoomScale] / 2,
			   centerP.y + screenH[nowZoomScale] / 2 };

	offSetX = centerP.x - screenW[nowZoomScale] / 2;
	offSetY = centerP.y - screenH[nowZoomScale] / 2;
}

void Camera::ZoomIn()
{
	nowZoomScale ++;
	if (nowZoomScale > 4 ) {
		nowZoomScale = 4;
	}
}

void Camera::ZoomOut()
{
	nowZoomScale --;
	if (nowZoomScale < 0) {
		nowZoomScale = 0;
	}
}

void Camera::UpdateCenter(const FPOINT& playerPos)
{
	centerP.x = (long)playerPos.x;
	centerP.y = (long)playerPos.y;

	screen = { centerP.x - screenW[nowZoomScale] / 2,
			   centerP.y - screenH[nowZoomScale] / 2,
			   centerP.x + screenW[nowZoomScale] / 2,
			   centerP.y + screenH[nowZoomScale] / 2 };

	offSetX = centerP.x - screenW[nowZoomScale] / 2;
	offSetY = centerP.y - screenH[nowZoomScale] / 2;

}


int Camera::ConvertToRendererX(const int& worldNumX)
{
	int rendererNum = (worldNumX - offSetX) * WINSIZE_X / screenW[nowZoomScale];
	return rendererNum;
}

int Camera::ConvertToRendererY(const int& worldNumY)
{
	int rendererNum = (worldNumY - offSetY) * WINSIZE_Y / screenH[nowZoomScale];
	return rendererNum;
}

int Camera::ConvertToWorldX(const int& redererNumX)
{
	int worldNum = screenW[nowZoomScale] * redererNumX / WINSIZE_X + offSetX;
	return worldNum;
}

int Camera::ConvertToWorldY(const int& redererNumY)
{
	int worldNum = screenH[nowZoomScale] * redererNumY / WINSIZE_Y + offSetY;
	return worldNum;
}

Camera::Camera()
{
}

Camera::~Camera()
{
}
