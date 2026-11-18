#pragma once
#include <d3d11.h>
#include <dxgi1_3.h>
#include <d2d1_1.h>
struct GpuSurface {
 ID3D11Device*device=nullptr;ID3D11DeviceContext*context=nullptr;
 IDXGISwapChain2*swap=nullptr;ID2D1Device*d2d=nullptr;ID2D1Bitmap1*target=nullptr;HANDLE ready=nullptr;
 bool hardware=true;
 HRESULT create(HWND hwnd,ID2D1Factory1*factory,ID2D1DeviceContext**out,float dpi){
  HRESULT hr=D3D11CreateDevice(nullptr,D3D_DRIVER_TYPE_HARDWARE,nullptr,D3D11_CREATE_DEVICE_BGRA_SUPPORT,nullptr,0,D3D11_SDK_VERSION,&device,nullptr,&context);
  if(FAILED(hr)){hardware=false;hr=D3D11CreateDevice(nullptr,D3D_DRIVER_TYPE_WARP,nullptr,D3D11_CREATE_DEVICE_BGRA_SUPPORT,nullptr,0,D3D11_SDK_VERSION,&device,nullptr,&context);}if(FAILED(hr))return hr;
  IDXGIDevice*dx=nullptr;IDXGIAdapter*ad=nullptr;IDXGIFactory2*f=nullptr;IDXGISwapChain1*s=nullptr;
  hr=device->QueryInterface(IID_PPV_ARGS(&dx));if(SUCCEEDED(hr))hr=dx->GetAdapter(&ad);if(SUCCEEDED(hr))hr=ad->GetParent(IID_PPV_ARGS(&f));
  RECT r;GetClientRect(hwnd,&r);DXGI_SWAP_CHAIN_DESC1 desc{};desc.Width=std::max(1L,r.right);desc.Height=std::max(1L,r.bottom);desc.Format=DXGI_FORMAT_B8G8R8A8_UNORM;desc.SampleDesc.Count=1;desc.BufferUsage=DXGI_USAGE_RENDER_TARGET_OUTPUT;desc.BufferCount=2;desc.SwapEffect=DXGI_SWAP_EFFECT_FLIP_DISCARD;desc.AlphaMode=DXGI_ALPHA_MODE_IGNORE;desc.Flags=DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;
  if(SUCCEEDED(hr))hr=f->CreateSwapChainForHwnd(device,hwnd,&desc,nullptr,nullptr,&s);if(SUCCEEDED(hr))hr=s->QueryInterface(IID_PPV_ARGS(&swap));if(SUCCEEDED(hr)){swap->SetMaximumFrameLatency(1);ready=swap->GetFrameLatencyWaitableObject();f->MakeWindowAssociation(hwnd,DXGI_MWA_NO_ALT_ENTER);hr=factory->CreateDevice(dx,&d2d);}if(SUCCEEDED(hr))hr=d2d->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,out);
  release(s);release(f);release(ad);release(dx);if(SUCCEEDED(hr))hr=bind(*out,dpi);return hr;
 }
 HRESULT bind(ID2D1DeviceContext*rt,float dpi){IDXGISurface*s=nullptr;auto hr=swap->GetBuffer(0,IID_PPV_ARGS(&s));D2D1_BITMAP_PROPERTIES1 p{};p.pixelFormat={DXGI_FORMAT_B8G8R8A8_UNORM,D2D1_ALPHA_MODE_IGNORE};p.dpiX=p.dpiY=dpi;p.bitmapOptions=(D2D1_BITMAP_OPTIONS)(D2D1_BITMAP_OPTIONS_TARGET|D2D1_BITMAP_OPTIONS_CANNOT_DRAW);if(SUCCEEDED(hr))hr=rt->CreateBitmapFromDxgiSurface(s,&p,&target);release(s);if(SUCCEEDED(hr)){rt->SetTarget(target);rt->SetDpi(dpi,dpi);}return hr;}
 HRESULT resize(ID2D1DeviceContext*rt,UINT w,UINT h,float dpi){rt->SetTarget(nullptr);release(target);context->ClearState();auto hr=swap->ResizeBuffers(0,std::max(1u,w),std::max(1u,h),DXGI_FORMAT_UNKNOWN,DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT);return SUCCEEDED(hr)?bind(rt,dpi):hr;}
 void reset(){if(ready){CloseHandle(ready);ready=nullptr;}release(target);release(d2d);release(swap);release(context);release(device);}
};
