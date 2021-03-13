#include "stdafx.h"
#include "Panel_Shader.h"


Panel_Shader::Panel_Shader()
{
}


Panel_Shader::~Panel_Shader()
{
}

void Panel_Shader::Initialize(float xSize, float ySize, char* fileName)
{
	Panel_Texture::Initialize(xSize, ySize, fileName);
	shaderEffect = LoadEffect("./Data/Shader/shader.fx");
}

void Panel_Shader::Destroy()
{
	SAFE_RELEASE(shaderEffect);
	Panel_Texture::Destroy();
}

void Panel_Shader::Update()
{
	Panel_Texture::Update();
}

void Panel_Shader::Render()
{
	D3DXMATRIXA16 view, projection;
	GameManager::GetDevice()->GetTransform(D3DTS_VIEW, &view);
	GameManager::GetDevice()->GetTransform(D3DTS_PROJECTION, &projection);
	D3DXMATRIXA16 rotation;
	D3DXMatrixRotationX(&rotation, D3DXToRadian(45.0f));

	shaderEffect->SetMatrix("world333", &world);
	shaderEffect->SetMatrix("view", &view);
	shaderEffect->SetMatrix("projection", &projection);
	shaderEffect->SetMatrix("rotation", &rotation);

	GameManager::GetDevice()->SetTransform(D3DTS_WORLD, &world);
	GameManager::GetDevice()->SetTexture(0, texture);
	GameManager::GetDevice()->SetFVF(FVF_PositionTexture::FVF);

	shaderEffect->SetTexture("m_TextureDiffuse", texture);

	D3DXHANDLE technique = shaderEffect->GetTechniqueByName("Mosaik");
	shaderEffect->SetTechnique(technique);

	UINT numPasses = 0;
	shaderEffect->Begin(&numPasses, NULL);
	for (int i = 0; i < numPasses; ++i)
	{
		shaderEffect->BeginPass(i);

		Panel_Texture::Render();

		shaderEffect->EndPass();

	}
	shaderEffect->End();
		
}

LPD3DXEFFECT Panel_Shader::LoadEffect(const char* fileName)
{
	LPD3DXEFFECT effect = nullptr;

	// ���̴� �ε�
	LPD3DXBUFFER error = nullptr;	//���� ���� ( ���̴��� �������Ҷ� �߸� �� �����̳� ���������� �������ִ� ���� )
	DWORD shaderFlag = 0;			//���̴� �÷��� 0 

#ifdef _DEBUG
	shaderFlag = shaderFlag | D3DXSHADER_DEBUG;		//���̴��� ����׸��� �������ϰڴ� ( ����׸��� �ؾ� �߸��� ������ ������ ���� Error ���ۿ� ���������� ���� ) 
#endif
		
	//fx ���Ϸ� ���� ���̴� ��ü ����
	D3DXCreateEffectFromFileA(
		GameManager::GetDevice(),	// ����̽�
		fileName,					// �ҷ��� ���̴� �ڵ� �����̸�
		nullptr,					// ���̴��� �������Ҷ� �߰��� ����� #define ���� ( �ϴ� null )
		nullptr,					// ���̴��� �������Ҷ� #include ���ù��� ó���Ҷ� ����� �������̽� �÷��� ( �ϴ� null )
		shaderFlag,					// ���̴� ������ �÷���
		nullptr,					// ���̴� �Ű������� ������ �޸�Ǯ ( �ϴ� null )
		&effect,					// �ε��� ���̴� Effect ������
		&error						// ���̴��� �ε��ϰ� �������Ҷ� ������ ����� �ش� ���ۿ� �����޽����� ���� ( ���������� �ε��Ǹ� NULL �� �����ȴ� )
		);

	//���̴� ���Ϸε������簡 �ִٸ�..
	if (error != nullptr || effect == nullptr)
	{
		//������ ������ ���� ���ڿ��� Ȯ��
		int size = error->GetBufferSize();
		char* str = new char[size];

		//str�� ���ۿ��ִ� ������ �����Ѵ�.
		sprintf_s(str, size, (const char*)error->GetBufferPointer());

		OutputDebugStringA(str);
		//���������� ��������� �������� ����
		SAFE_RELEASE(error);
		if (str)
		{
			delete[] str;
		}

		return nullptr;
	}

	return effect;
}
