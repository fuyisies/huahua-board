#pragma once
#include <algorithm>
#include <cmath>
#include <vector>
#include <filesystem>
#include <string>
struct Box{float x=0,y=0,w=0,h=0;bool contains(float a,float b)const{return a>=x&&a<=x+w&&b>=y&&b<=y+h;}};
inline int swapTarget(const std::vector<Box>&boxes,Box viewport,float scroll,float x,float y,int source){if(!viewport.contains(x,y))return -1;for(int i=int(boxes.size())-1;i>=0;--i){auto b=boxes[i];b.y-=scroll;if(b.contains(x,y))return i==source?-1:i;}return -1;}
template<class T>bool swapItems(std::vector<T>&v,int a,int b){if(a<0||b<0||a>=int(v.size())||b>=int(v.size())||a==b)return false;std::swap(v[a],v[b]);return true;}
inline float approach(float v,float t,float dt){return t+(v-t)*std::exp(-16*dt);}
inline std::vector<Box> masonry(const std::vector<float>&ratios,float x,float width,int cols,float gap){cols=std::max(1,cols);float cw=(width-gap*(cols-1))/cols;std::vector<float>ys(cols,0);std::vector<Box>out;for(float r:ratios){auto c=std::min_element(ys.begin(),ys.end())-ys.begin();float h=cw/std::clamp(r,.12f,8.f);out.push_back({x+c*(cw+gap),ys[c],cw,h});ys[c]+=h+gap;}return out;}
template<class T>void reorder(std::vector<T>&v,int from,int before){if(from<0||from>=(int)v.size())return;before=std::clamp(before,0,(int)v.size());auto value=std::move(v[from]);v.erase(v.begin()+from);if(before>from)--before;v.insert(v.begin()+before,std::move(value));}
// CSS cubic-bezier(.22,1,.36,1): solve x(t), then evaluate y(t).
inline float webEase(float x){x=std::clamp(x,0.f,1.f);if(x==0||x==1)return x;float lo=0,hi=1,t=x;for(int i=0;i<16;i++){t=(lo+hi)/2;float u=1-t;float bx=3*u*u*t*.22f+3*u*t*t*.36f+t*t*t;if(bx<x)lo=t;else hi=t;}return 1-std::pow(1-t,3);}
inline int responsiveColumns(float width,float tile,float gap){return std::max(1,int(std::floor((width+gap)/(tile+gap))));}
struct Tween{float from=0,to=0,value=0;double start=0,duration=.35;void set(float next,double now,double seconds=.35){from=value;to=next;start=now;duration=seconds;}bool update(double now,bool motion){float p=motion?float((now-start)/duration):1;value=p>=1?to:from+(to-from)*webEase(p);return p<1&&std::abs(from-to)>.0001f;}};

inline float zoomPanOffset(float offset,float pointerFromCenter,float ratio){return pointerFromCenter*(1-ratio)+offset*ratio;}

inline std::filesystem::path uniqueImagePath(const std::filesystem::path&dir,const std::wstring&name,const std::wstring&ext){auto p=dir/(name+ext);int n=2;while(std::filesystem::exists(p))p=dir/(name+L" ("+std::to_wstring(n++)+L")"+ext);return p;}
