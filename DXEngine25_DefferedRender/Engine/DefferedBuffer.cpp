#include "DefferedBuffer.h"



DefferedBuffer::DefferedBuffer()
{
	for (int ix = 0; ix < BUFFERCOUNT; ++ix)
	{
		renderTargetTextureArray[ix] = NULL;
		renderTargetViewArray[ix] = NULL;
		shaderResourceViewArray[ix] = NULL;
	}

}


DefferedBuffer::~DefferedBuffer()
{
}

bool DefferedBuffer::Initialize(ID3D11Device * device, int textureWidth, int textureHeight)
{
	this->textureWidth = textureWidth;
	this->textureHeight = textureHeight;

	//¼­¼úÀÚ
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	textureDesc.Width = textureWidth;
	textureDesc.Height = textureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;		//¾ÈÆ¼¿¤¸®¾î½Ì Á¶Àý°ª
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;		//ºñÆ® ÇÕ¿¬»ê
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	//·»´õ Å¸°Ù ÅØ½ºÃÄ
	HRESULT result;
	
	for (int ix = 0; ix < BUFFERCOUNT; ++ix)
	{
		result = device->CreateTexture2D(&textureDesc, NULL, &renderTargetTextureArray[ix]);

		//¿À·ù È®ÀÎ
		if (IsError(result, TEXT("RT ·»´õ Å¸°Ù ÅØ½ºÃÄ »ý¼º ½ÇÆÐ")))
		{
			return false;
		}
	}


	//¼­¼úÀÚ
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	rtvDesc.Format = textureDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	for (int ix = 0; ix < BUFFERCOUNT; ++ix)
	{
		//·»´õ Å¸°Ù ºä »ý¼º
		result = device->CreateRenderTargetView(renderTargetTextureArray[ix], &rtvDesc, &renderTargetViewArray[ix]);

		//¿À·ù È®ÀÎ
		if (IsError(result, TEXT("RT ·»´õ Å¸°Ù ºä »ý¼º ½ÇÆÐ")))
		{
			return false;
		}
	}

	//¼ÎÀÌ´õ ¸®¼Ò½º ºä
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;


	for (int ix = 0; ix < BUFFERCOUNT; ++ix)
	{
		//SRV »ý¼º
		result = device->CreateShaderResourceView(renderTargetTextureArray[ix], &srvDesc, &shaderResourceViewArray[ix]);

		//¿À·ù È®ÀÎ
		if (IsError(result, TEXT("RT ¼ÎÀÌ´õ ¸®¼Ò½º ºä »ý¼º ½ÇÆÐ")))
		{
			return false;
		}
	}

	return true;
}

void DefferedBuffer::SetRenderTargets(ID3D11DeviceContext * deviceContext, ID3D11DepthStencilView * depthStencilView)
{
	//·»´õ Å¸°Ù ¼³Á¤, ¹è¿­·Î ¼³Á¤
	deviceContext->OMSetRenderTargets(BUFFERCOUNT, renderTargetViewArray, depthStencilView);

}

void DefferedBuffer::ClearRenderTargets(ID3D11DeviceContext * deviceContext, ID3D11DepthStencilView * depthStencilView, float color[])
{
	//·»´õ Å¸°Ù ºä ºñ¿ì±â
	for (int ix = 0; ix < BUFFERCOUNT; ++ix)
	{
		deviceContext->ClearRenderTargetView(renderTargetViewArray[ix], color);
	}

	//µª½º ½ºÅÙ½Ç ºä Áö¿ì±â
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DefferedBuffer::Release()
{
	for (int ix = 0; ix < BUFFERCOUNT; ++ix)
	{
		Memory::SafeRelease(renderTargetTextureArray[ix]);
		Memory::SafeRelease(renderTargetViewArray[ix]);
		Memory::SafeRelease(shaderResourceViewArray[ix]);

	}

}
