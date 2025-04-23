#include "MouseManager.h"
#include "CommonFunction.h"
// 전역 윈도우 핸들 필요 시 외부에서 정의
extern HWND g_hWnd;

template<typename T>
T Clamp(T val, T minVal, T maxVal) {
    return (val < minVal) ? minVal : ((val > maxVal) ? maxVal : val);
}

HRESULT MouseManager::Init()
{
    wheelDelta = 0;

    dragStartP = { 0, 0 };
    dragEndP = { 0, 0 };
    prevP = { 0, 0 };
    clickedP = { 0, 0 };

    dragElapsedTime = 0.0f;

    deltaX = 0;
    deltaY = 0;

    for (int i = 0; i < 3; ++i) {
        isDragging[i] = false;
        prevMouseDown[i] = false;
        currMouseDown[i] = false;
    }

    dragEndPUsed = true;
    clickedPUsed = true;

    return S_OK;
}

void MouseManager::Update()
{
    // 마우스 위치 추적
    GetCursorPos(&mousePos);
    ScreenToClient(g_hWnd, &mousePos);

    dragEndPUsed = true;
    clickedPUsed = true;

    // 레이어 갱신
    SetLayer();

    const int mouseVK[3] = { MOUSE_LEFT, MOUSE_RIGHT, MOUSE_MIDDLE };

    // 이전 상태 저장 & 현재 상태 갱신
 

    // 드래그 및 클릭 로직 처리
    for (int i = 0; i < 3; ++i)
    {

        prevMouseDown[i] = currMouseDown[i];
        currMouseDown[i] = (GetAsyncKeyState(mouseVK[i]) & 0x8000) != 0;


        if (IsOnceMouseDown(mouseVK[i])) {
            isDragging[i] = true;
            dragStartP = mousePos;
            prevP = mousePos;
                    }

        if (isDragging[i] && IsStayMouseDown(mouseVK[i])) {
            const int MAX_DELTA = 30; // 프레임당 이동 제한

            int rawDeltaX = mousePos.x - prevP.x;
            int rawDeltaY = mousePos.y - prevP.y;

            deltaX = Clamp(rawDeltaX, -MAX_DELTA, MAX_DELTA); //
            deltaY = Clamp(rawDeltaY, -MAX_DELTA, MAX_DELTA); //걸어놓은 제한 밖의 값이 들어올 시 제한 값으로 변환

            std::string msg = "[Camera] deltaX: " + std::to_string(deltaX) + ", deltaY: " + std::to_string(deltaY) + "\n";
            OutputDebugStringA(msg.c_str());

            prevP = mousePos;

            dragElapsedTime += TimerManager::GetInstance()->GetDeltaTime();

        }

        if (IsOnceMouseUp(mouseVK[i])) {
            dragEndP = mousePos;
            clickedP = mousePos;     
            isDragging[i] = false;

            float dragDistance = GetDistance(dragStartP, dragEndP);
            if (dragDistance <= 20.0f && dragElapsedTime <= 0.3f)
            {
                //클릭으로 간주
                clickedPUsed = false;
            }
            else //드래그로 간주
            {
                dragEndPUsed = false;
            }

            dragElapsedTime = 0.0f;
            deltaX = 0;
            deltaY = 0;
        }
    }

    //wheelDelta = 0;
}

bool MouseManager::IsOnceMouseDown(int button)
{
    int idx = (button == MOUSE_LEFT) ? 0 : (button == MOUSE_RIGHT ? 1 : 2);
    return currMouseDown[idx] && !prevMouseDown[idx];
}

bool MouseManager::IsOnceMouseUp(int button)
{
    int idx = (button == MOUSE_LEFT) ? 0 : (button == MOUSE_RIGHT ? 1 : 2);
    return !currMouseDown[idx] && prevMouseDown[idx];
}

bool MouseManager::IsStayMouseDown(int button)
{
    int idx = (button == MOUSE_LEFT) ? 0 : (button == MOUSE_RIGHT ? 1 : 2);
    return currMouseDown[idx];
}

bool MouseManager::IsOnceMouseDown(int button, Layer thisLayer)
{
    return (thisLayer == currLayer) && IsOnceMouseDown(button);
}

bool MouseManager::IsStayMouseDown(int button, Layer thisLayer)
{
    return (thisLayer == currLayer) && IsStayMouseDown(button);
}

bool MouseManager::IsOnceMouseUp(int button, Layer thisLayer)
{
    return (thisLayer == currLayer) && IsOnceMouseUp(button);
}

void MouseManager::SetLayer()
{
    // TODO: 필요에 따라 레이어 갱신 로직 작성
}

void MouseManager::InitPoints()
{
    dragStartP = { 0, 0 };
    dragEndP = { 0, 0 };
    prevP = { 0, 0 };
    clickedP = { 0, 0 };
}

void MouseManager::Release()
{
    ReleaseInstance();
}
