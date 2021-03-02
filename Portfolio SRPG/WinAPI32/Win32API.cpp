#include "stdafx.h"

// hwnd ������ �ڵ� , h ���ҽ� �ڵ�

//���� ����
//�ν��Ͻ� -> ����ǰ� �ִ� ���α׷��� �޸� �����ּҸ� ������ �ִ�.
//���α׷��� �ϳ��� �ν��Ͻ��� ������ �����Ѵ�.
HINSTANCE	g_hInstance;
//������â�� �����ϴ� �ڵ�
//������â�� ������ ����ϴ� ��� �������� �ڵ��� �ʿ���
HWND		g_hwnd;
//������ â,���α׷� �̸�
LPCTSTR		g_AppName = _T("DirectX 11");
Vector2		g_MousePt;  //���콺 ��ǥ


//���������̽� �İ�ü
//�ּҰ� , I �������̽�
//new,delete�� �ƴ϶� create,release �� ���� ����

//������ ������ �ٸ��� ��� ���ٷ� ����

//I interface �����Լ��� ����ְ� ���ǵ� �Լ��� ���� ����
//DXGI DirectX Graphics Infrastructure �׷���,���÷���

//SwapChain ����� ����Ʈ���� �������ִ¾�(������۸�)
IDXGISwapChain*				g_SwapChain;

//I, d3d11, device
//�ڿ�����,������ �����ϴ� ������
ID3D11Device*				g_Device;

//������ ����
ID3D11DeviceContext*		g_DeviceContext;


//RenderTargetView -> �ڿ���
//��¹��� �ڿ��� �������ִ� �ڿ��� �̸�
//�ڿ���� �ڿ��� ���������ο� �������ִ� ����
ID3D11RenderTargetView*		g_RTV;


//MainGame*	Mg;
Scene*	    Mg;

float       g_MasterVolume      = 1.0f;
float       g_BackgroundVolume = 1.0f;

//�Լ����漱�� (�ؿ��� �Լ��� ����ϱ����� ������ ���´�)
//��ȯ��, �ݹ鼱�� �Լ��̸�(�Ű�����)
//�ݹ��Լ�:��� �̺�Ʈ�� �߻��Ǿ����� �Լ�ȣ���� �ȴ�.
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//					â�� ������ǥ  �ʺ�         ����
void setWindowSize(int x, int y, int width, int height);



//winmain ������ �������α׷� �����Լ�
int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	//������ Ŭ����
	//������ �����츦 �������ִ� ����ü
	WNDCLASS	wc;
	//��� ����� NULL (0) ���� �ʱ�ȭ
	ZeroMemory(&wc, sizeof(WNDCLASS));


	//	�������� ���� ���ڿ� ���� ��� ǥ������ �ʴ´�
	UNREFERENCED_PARAMETER(lpCmdLine);

	////��Ʈ ������ | or, & 
	wc.style = CS_HREDRAW | CS_VREDRAW;
	//lp�������� fn �Լ� WndProc ���������ν���
	wc.lpfnWndProc = static_cast<WNDPROC>(WndProc);
	//�ν��Ͻ� ����
	wc.hInstance = hInstance;
	//������(���α׷� ������)
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	//������â Ȱ��ȭ�ÿ� ���콺Ŀ�� ��� // IDC_ARROW ȭ��ǥ IDC_CROSS����
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	//���(������)								����
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	//�̸�
	wc.lpszClassName = g_AppName;
	wc.lpszMenuName = nullptr;

	//	�ü���� �������Ϳ� �� ���α׷��� ����Ѵ�
	if (!RegisterClass(&wc))
		return FALSE;
	//������ �ν��Ͻ��� ����
	g_hInstance = hInstance;

	//	���� ������ ����
	g_hwnd = CreateWindow(
		g_AppName,
		g_AppName,
		WS_OVERLAPPEDWINDOW,
        WINSTARTX,			//â�� ���� x��ǥ
        WINSTARTY,			//â�� ���� y��ǥ
        WINSIZEX,		//â�� �ʺ�
        WINSIZEY,		//â�� ����
		nullptr,
		nullptr,
		hInstance,	//��ϵ� �ν��Ͻ�
		nullptr
	);
    //�ڵ��� ����� ���� ������ ���ߴٸ� ���α׷� ����
    if (!g_hwnd)
        return FALSE;

    //Create Device and DeviceContext, SwapChain
    {
        DXGI_MODE_DESC bufferDesc;
        //desc ������ ����ü�� ����ȴ�. ���÷��� ��带 �����մϴ�.
        ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

        bufferDesc.Width = WINSIZEX;// ���ϴ� back buffer �ʺ�
        bufferDesc.Height = WINSIZEY;// ���ϴ� back buffer ����
        bufferDesc.RefreshRate.Numerator = FIXFRAME; //���� ��ħ �󵵸� �츣�� ������ ���� �ϴ� DXGI_RATIONAL ���� ����
        bufferDesc.RefreshRate.Denominator = 1; // �и�
        bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;// back buffer �ȼ� ����
        bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;// ���÷��� ��ĵ���� ���
        bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;//���÷��� ��ʸ��


        DXGI_SWAP_CHAIN_DESC desc;
        //IDXGISwapChain ��ü�� ���(Description) �� ���� ����ü.

        ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));

        desc.BufferDesc = bufferDesc;// �����ϰ��� �ϴ� back buffer�� �Ӽ����� �����ϴ� ����ü
        desc.SampleDesc.Count = 1;// Multisampling�� ���� ������ ǥ�� ������ ǰ�� ������ �����ϴ� ����ü
        desc.SampleDesc.Quality = 0;
        desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;// ������ �뵵�� �����ϴ� ����ü
        desc.BufferCount = 1; // Swap chain���� ����� back buffer�� ����.(���߹��۸� : 1��, ���߹��۸� : 2��)
        desc.OutputWindow = g_hwnd;// ������ ����� ǥ���� ������ â�� �ڵ�
        desc.Windowed = TRUE;// â ��带 ���ϸ� true, ��üȭ�� ��带 ���ϸ� false
        desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;// �߰����� �÷���

        UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

        //dx �������� �ش� ������ ������ ������������ �������� �������� Ž��
        D3D_FEATURE_LEVEL featureLevels[7] =
        {
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0,
            D3D_FEATURE_LEVEL_9_3,
            D3D_FEATURE_LEVEL_9_2,
            D3D_FEATURE_LEVEL_9_1
        };

        //S_OK; // 0
        //E_FAIL;//����
        //FAILED(E_FAIL); // true
        //SUCCEEDED(S_OK); // 1

        ////bool -> 0 �ƴϸ� ����  1

        //bool b = SUCCEEDED(S_OK) && "aaaa";
        //if(b)
        //{
        //    int a = 0;
        //}
       


        HRESULT hr = D3D11CreateDeviceAndSwapChain
        (
            NULL,//ǥ���� '���÷��� ����̽�(���� ī��)'�� 'IDXGIAdapter �������̽�'�� �����մϴ�.
                 //�̰��� DXGI�� �������̽��Դϴ�. NULL�� �����ϸ� ���ʿ� �߰��� ����̽��� ����մϴ�. 
                 //����ī�带 ������ �ʿ䰡 �������� �����ϰ�� �⺻������ NULL�� �����ϸ� �˴ϴ�
            D3D_DRIVER_TYPE_HARDWARE,//����̹� Ÿ�� D3D_DRIVER_TYPE_S0FTWARK,D3D_DRIVER_TYPE_REFERENCE ���� ����
            NULL, //����Ʈ���� ������ ���� �ϵ����Ÿ������ ����������� NULL
            creationFlags, //�߰����� ��ġ ���� �÷��׵��� �����Ѵ�NULL �� ���� D3D11_CREATE_DEVICE_DEBUG ���� ����
            featureLevels, //��ɼ��� ���� ���ϴ� �Ű�����,NULL �ְ���  D3D_FEATURE_LEVEL_11_0 ���� ����
            ARRAYSIZE(featureLevels), //��ɼ��� ������ D3D_FEATURE_LEVEL ���� �����̴�.�� ���� �����ߴٸ� �� �Ű������� 0���� �����ϸ� �ȴ�
            D3D11_SDK_VERSION, //�׻� D3D11_SDK_VERSI0N�� �����Ѵ�
            &desc,//����ü���� ���������� ������ ����ü�� �����͸� �Ѱ��ش�
            &g_SwapChain, //������ ����ü�� �������̽��� �����͸� ���� ����
            &g_Device,	//������ ����̽� �������̽��� �����͸� ���� ������ �����մϴ�.
            NULL, //������ ������ ��쿡�� pFeatureLevels���� �����ߴ� �迭�� ó������ �����ݴϴ�.������ ��쿡�� 0�� ��ȯ�˴ϴ�.
            &g_DeviceContext	//������ ����̽� ���ؽ�Ʈ �������̽��� �����͸� ���� ������ �����մϴ�.
        );
        //������ �ȉ����� ������ ���
        assert(SUCCEEDED(hr) && "����̽�����ü�� ���� �Ұ�");
        //         1 and "���ڿ�" -> 1
        //         1 and 1 -> 1

        //         0 and 1 -> 0
    }
    //Create BackBuffer
    {
        HRESULT hr;

        ID3D11Texture2D* BackBuffer;
        hr = g_SwapChain->GetBuffer(0, //������ ������ ��ȣ(�ε���)
                                    __uuidof(ID3D11Texture2D), //����۸� ���� �������̽��� Ÿ��
                                    (void**)&BackBuffer);//��ȯ�� �������̽��� ���� ������ ������
        assert(SUCCEEDED(hr));

        hr = g_Device->CreateRenderTargetView(BackBuffer,//����Ÿ�� �信�� �������ϴ� ���ҽ�
                                              NULL, //����Ÿ�� �並 �����ϴ� 'D3D11_RENDER_TARGET_VIEW_DESC'����ü�� ������.
                                                    //NULL�� �Ѱ��ָ�, ���ҽ��� ������������� ������ �״�� ����ϸ�, ��� ���ҽ��� �Ӹ� ���� 0�� �׼����ϴ� �並 �����Ѵ�.
                                              &g_RTV);//ID3D11RenderTargetView �������̽��� �޴� ������ ������. NULL�� �Ѱ��ְ� �Ǹ�, �ٸ� ���ڵ��� ��ȿ���� üũ�� �� �ִ�.

        assert(SUCCEEDED(hr));
        //����Ÿ�� �䰡 ������ ���Ŀ��� ���� ������� �����Ϳ� �������� �ʰ�, �� ����Ÿ�� �並 ����մϴ�. 
        //�׷��� ������ ����Ÿ�� ���� �������̽��� ��� �� ���Ŀ��� ������� �����͸� �����մϴ�.

        SAFE_RELEASE(BackBuffer);

        //��º��տ� �ش� ����Ÿ�ٺ� ����ŭ ����
        g_DeviceContext->OMSetRenderTargets(1,//����Ÿ���� �����
                                            &g_RTV, //������ ���迭���� ������
                                            NULL);//���ٽ� ���� ���� NULL�̸� ���̰� ���α�
    }


    //Create Viewport
    {
        D3D11_VIEWPORT viewport = { 0 };

        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.Width = WINSIZEX;
        viewport.Height = WINSIZEY;

        g_DeviceContext->RSSetViewports(1,//���ε� �� ����Ʈ ���Դϴ�.
                                        &viewport);//��ġ�� ���ε� �� D3D11_VIEWPORT ���� �� �迭�Դϴ�
    }
    //Disable DepthStencil
    {
        //�ش� �������ٽ� ��ɰ��úκ��� ���δ� �ɼ����� ����
        D3D11_DEPTH_STENCIL_DESC desc = { 0 };
        desc.DepthEnable = false;
        desc.StencilEnable = false;

        ID3D11DepthStencilState* depthStencilState;
        g_Device->CreateDepthStencilState(&desc, &depthStencilState);

        g_DeviceContext->OMSetDepthStencilState(depthStencilState, 0xFF);
        SAFE_RELEASE(depthStencilState);
    }
    //�����Ͷ����� �ɼ�
    {
        ID3D11RasterizerState* rasterizerState;
        D3D11_RASTERIZER_DESC desc;
        ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
        desc.FillMode = D3D11_FILL_SOLID; // D3D11_FILL_WIREFRAME�� �׵θ�������
        desc.CullMode = D3D11_CULL_NONE; //�ø��Ҷ� �ո�,�޸� �߷����� �Ұ���
        desc.DepthBias = 0;
        desc.FrontCounterClockwise = false; //cw ccw �� ���ϱ�
        desc.DepthBiasClamp = 0;
        desc.SlopeScaledDepthBias = 0;
        desc.DepthClipEnable = false; //����Ŭ���� ����
        desc.ScissorEnable = false; //�����׽�Ʈ ���� ����
        desc.MultisampleEnable = false; //��Ƽ ���ø� ��������
        desc.AntialiasedLineEnable = false; //���ξ�Ƽ�ٸ���� ����
        g_Device->CreateRasterizerState(&desc, &rasterizerState);
        g_DeviceContext->RSSetState(rasterizerState);
        SAFE_RELEASE(rasterizerState);
    }

    //��ĳ����
	Mg = new MainGame();
	Mg->init();

	
    //ȭ�� �۾� ������ ���� ���
    setWindowSize(WINSTARTX, WINSTARTY, WINSIZEX, WINSIZEY);


	//	������ ���α׷��� ���÷����� �ֻ����� �ø���
	ShowWindow(g_hwnd, nCmdShow);

	//	���� ������ ����
	UpdateWindow(g_hwnd);

	

	//�޼��� (*�߿�)
	MSG message;
	ZeroMemory(&message, sizeof(MSG));

	//**** ���Ⱑ �߿��� �κ�****
	//���ӿ����,�޼������� ��� �Ѵ�.
	//�ݺ���(����) -> �޼��� ����(������ ���µ����� ���α׷��� ����Ǵ���)

	while (true)
	{
		//PeekMessage ->���ӿ��� ���̴�  (�޼����� ���ö��� ����)
		//GetMessage -> �޼����� ��� ��ٸ���

		if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT)
				break;

			TranslateMessage(&message);
			DispatchMessage(&message);
		}
        else
        {
            //�ð�������� ���� ������Ʈ�� ������ ȣ��ǰ� ������ش�.
            TIMEMANAGER->SetFrame(FIXFRAME);
            Mg->update(); //���ŵǴ� ��
            Mg->render();//�׸����� �� 
        }
	}

    //���������̽��� ������ ������� ����
    SAFE_RELEASE(g_SwapChain);
    SAFE_RELEASE(g_Device);
    SAFE_RELEASE(g_DeviceContext);
    SAFE_RELEASE(g_RTV);

	//���⼭���ʹ� ���α׷� ���� �غ� �ؾ��Ѵ�.
	Mg->release();
    SAFE_DELETE(Mg);



	//������ ������ ����
	DestroyWindow(g_hwnd);
	//��ϵ� ���α׷� ����
	UnregisterClass(g_AppName, hInstance);
	//�������� �����Լ� ����
	return message.wParam;
}
//���漱���� �Լ� ����
// �� â�� �޼����� ó���ϴ� �Լ�
// param �� �Ķ������ ���Ӹ� word,long ����
// wParam ����,�ڵ鰪�� �޾ƿ��� lParam �����Ͱ��� �޾ƿ´�.

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//� �޼����� �߻��Ǿ��°��� ���� ó���� ���ǹ�
	switch (message)
	{
	//�ı��ϰų� ������� �޼����� ���°��
	case WM_DESTROY: case WM_CLOSE:
		//���α׷� ���� ȣ�� �Լ�
		PostQuitMessage(0);
		break;
        //���콺�� �����϶�
    case WM_MOUSEMOVE:
        //���콺 ��ǥ���� lParam �� ����� �ִ�.
        g_MousePt.x = (float)LOWORD(lParam) + MAINCAM->Pos.x;
        //WINDOW��ǥ�� DX ��ǥ�� �������� y ��ǥ �ٲ��ֱ�
        g_MousePt.y = (float)WINSIZEY - (float)HIWORD(lParam) + MAINCAM->Pos.y;
        break;
	case WM_KEYDOWN:
		//�߻��� Ű���� ��������� wParam
		if (wParam == VK_ESCAPE)
		{
			//���α׷� ���� ȣ�� �Լ�
			PostQuitMessage(0);
		}
	}
    //�굵 ���콺�Է��̶� Ű���� �Է°��� �ʿ�
    ImGui::WndProc((UINT*)hWnd, message, wParam, lParam);

	return (DefWindowProc(hWnd, message, wParam, lParam));
}

void setWindowSize(int x, int y, int width, int height)
{
    //ȭ��ũ�⿡ �´� ��Ʈ
    RECT rc;
    rc.left = 0;
    rc.top = 0;
    rc.right = width;
    rc.bottom = height;

    //���� ������ ũ�� ����
    AdjustWindowRect(&rc, WS_CAPTION | WS_SYSMENU, false);
    //�� RECT ������ ������ ������ ����
    SetWindowPos(g_hwnd, NULL, x, y, (rc.right - rc.left), (rc.bottom - rc.top),
                 SWP_NOZORDER | SWP_NOMOVE);
}
