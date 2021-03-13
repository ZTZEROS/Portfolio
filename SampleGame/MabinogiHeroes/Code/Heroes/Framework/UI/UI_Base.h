#pragma once
class UI_Base
{
public:
	UI_Base();
	virtual ~UI_Base();

	virtual void Destroy();

	virtual void Update();
	virtual void Render();

	virtual void AddChild(UI_Base* child);
	virtual LRESULT InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual UI_Base* GetChildByID(int childID);

	inline const D3DXMATRIX& GetWorld() { return world; }
	inline int GetID() { return id; }
	inline D3DXVECTOR2& GetSize() { return size; }
	inline RECT getRect() { return rect; };
	inline bool GetIsShow() { return isShow; }


	void SetChildUI(int ID, UI_Base change);
	inline void SetPosition(float x, float y, float z) { position = D3DXVECTOR3(x, y, z); D3DXMatrixTranslation(&Translation, position.x, position.y, position.z); }
	inline void SetScale(float scale) { D3DXMatrixScaling(&Scale, scale, scale, scale); size *= scale; }
	inline void SetScale(float scaleX, float scaleY) { D3DXMatrixScaling(&Scale, scaleX, scaleY, 1.0f); size.x *= scaleX; size.y *= scaleY; }
	inline void SetSize(float width, float height) { size.x = width; size.y = height; }
	inline void SetID(int _ID) { id = _ID; }
	inline void SetShow(bool show = true) { isShow = show; }
	inline void SetchildPosition(float x, float y) { childPosition.x = x, childPosition.y= y; }
	void SetRect(); //�ڽ��� �����ǰ� ũ�⸦ �������� ��Ʈ�� ������ִ� �Լ�
	D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0); // ��ġ

protected:

	UI_Base* parent = nullptr;
	std::vector<UI_Base*> childArray;

	D3DXMATRIX world;
	D3DXMATRIX Scale;
	D3DXMATRIX Rotation;
	D3DXMATRIX Translation;

	D3DXVECTOR2 size = D3DXVECTOR2(0, 0);		 // ũ��
	RECT rect ;

	int id = -1;	//UI�� �����ϱ� ���� ����ũ ���̵�(-1�̸� ���õ��� ������)

	bool isShow = true;

	D3DXVECTOR2 childPosition;

	inline void SetParent(UI_Base* myParent) { parent = myParent; }
};

