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

	// 셰이더 로딩
	LPD3DXBUFFER error = nullptr;	//에러 버퍼 ( 셰이더를 컴파일할때 잘못 된 문법이나 오류정보를 리턴해주는 버퍼 )
	DWORD shaderFlag = 0;			//셰이더 플레그 0 

#ifdef _DEBUG
	shaderFlag = shaderFlag | D3DXSHADER_DEBUG;		//셰이더를 디버그모드로 컴파일하겠다 ( 디버그모드로 해야 잘못된 컴파일 오류가 날때 Error 버퍼에 오류정보가 들어간다 ) 
#endif
		
	//fx 파일로 부터 셰이더 객체 생성
	D3DXCreateEffectFromFileA(
		GameManager::GetDevice(),	// 디바이스
		fileName,					// 불러올 셰이더 코드 파일이름
		nullptr,					// 셰이더를 컴파일할때 추가로 사용할 #define 정의 ( 일단 null )
		nullptr,					// 셰이더를 컴파일할때 #include 지시문을 처리할때 사용할 인터페이스 플레그 ( 일단 null )
		shaderFlag,					// 셰이더 컴파일 플레그
		nullptr,					// 셰이더 매개변수를 공유할 메모리풀 ( 일단 null )
		&effect,					// 로딩될 셰이더 Effect 포인터
		&error						// 셰이더를 로딩하고 컴파일할때 문제가 생기면 해당 버퍼에 에러메시지가 들어간다 ( 성공적으로 로딩되면 NULL 이 참조된다 )
		);

	//셰이더 파일로딩에문재가 있다면..
	if (error != nullptr || effect == nullptr)
	{
		//문제의 내용이 뭔지 문자열로 확인
		int size = error->GetBufferSize();
		char* str = new char[size];

		//str에 버퍼에있는 내용을 저장한다.
		sprintf_s(str, size, (const char*)error->GetBufferPointer());

		OutputDebugStringA(str);
		//오류내용을 출력했으니 오류버퍼 해제
		SAFE_RELEASE(error);
		if (str)
		{
			delete[] str;
		}

		return nullptr;
	}

	return effect;
}
