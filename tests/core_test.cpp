#include "../src/core.hpp"
#include <cassert>
#include <random>
#include <iostream>
#include <fstream>
int main(){
 auto p=masonry({1,2,.5,1},10,620,3,10);assert(p.size()==4);assert(p[0].x==10&&p[1].x==220&&p[2].x==430);assert(p[3].x==220&&p[3].y==110);
 std::mt19937 g(123);std::uniform_real_distribution<float>d(.15f,6.f);for(int cols=1;cols<=8;cols++){std::vector<float>r(200);for(auto&x:r)x=d(g);auto q=masonry(r,0,1200,cols,14);for(size_t i=0;i<q.size();i++){assert(q[i].x>=0&&q[i].x+q[i].w<1200.01f);for(size_t j=0;j<i;j++){auto a=q[i],b=q[j];assert(a.x+a.w<=b.x+.01f||b.x+b.w<=a.x+.01f||a.y+a.h<=b.y+.01f||b.y+b.h<=a.y+.01f);}}}
 for(int from=0;from<10;from++)for(int before=0;before<=10;before++){std::vector<int>v{0,1,2,3,4,5,6,7,8,9};reorder(v,from,before);assert(v.size()==10);int pos=before-(before>from);assert(v[pos]==from);auto s=v;std::sort(s.begin(),s.end());for(int i=0;i<10;i++)assert(s[i]==i);}
 float x=0;for(int i=0;i<120;i++){float n=approach(x,100,1.f/60);assert(n>=x&&n<=100);x=n;}assert(x>99.99f);assert((Box{0,0,20,20}.contains(5,5)));
 assert(responsiveColumns(1000,240,16)==3);assert(responsiveColumns(1050,240,16)==4);assert(responsiveColumns(200,440,16)==1);
 assert(webEase(0)==0&&webEase(1)==1);float previous=0;for(int i=1;i<=1000;i++){float e=webEase(i/1000.f);assert(e>=previous&&e<=1);previous=e;}
 Tween t;t.set(2,0,.35);assert(t.update(.1,true));float intermediate=t.value;assert(intermediate>0&&intermediate<2);t.set(0,.1,.35);assert(t.value==intermediate);assert(t.update(.2,true));assert(t.value<intermediate);assert(!t.update(.5,true)&&t.value==0);t.set(2,.5);assert(!t.update(.5,false)&&t.value==2);
 auto narrow=masonry({1,2,.5,1},0,200,1,16);for(auto b:narrow)assert(b.x==0&&b.w==200);
 for(float offset:{-100.f,0.f,120.f})for(float pointer:{-350.f,0.f,200.f})for(float ratio:{.5f,1.f,2.f,8.f}){float next=zoomPanOffset(offset,pointer,ratio);assert(std::abs((pointer-next)/ratio-(pointer-offset))<.001f);float restored=zoomPanOffset(next,pointer,1/ratio);assert(std::abs(restored-offset)<.001f);}

 // Entire card targets, gaps, own card, outside, scroll with stationary pointer.
 std::vector<Box> cards{{20,20,100,180},{140,20,100,50},{140,90,100,110}};
 Box viewport{0,0,260,250};
 assert(swapTarget(cards,viewport,0,160,22,0)==1);
 assert(swapTarget(cards,viewport,0,160,68,0)==1);
 assert(swapTarget(cards,viewport,0,130,50,0)==-1);
 assert(swapTarget(cards,viewport,0,30,40,0)==-1);
 assert(swapTarget(cards,viewport,0,300,40,0)==-1);
 assert(swapTarget(cards,viewport,60,160,40,0)==2);
 for(int a=0;a<10;a++)for(int b=0;b<10;b++){
  std::vector<int> values{0,1,2,3,4,5,6,7,8,9};
  bool changed=swapItems(values,a,b);assert(changed==(a!=b));
  for(int i=0;i<10;i++)assert(values[i]==(i==a?b:i==b?a:i));
 }
 std::vector<int> values{0,1};assert(!swapItems(values,0,-1));assert(values[0]==0);

 auto testDir=std::filesystem::temp_directory_path()/"huahua-collision-test";
 std::filesystem::create_directories(testDir);
 auto first=uniqueImagePath(testDir,L"image",L".png");std::ofstream(first)<<"one";
 auto second=uniqueImagePath(testDir,L"image",L".png");assert(first!=second);std::ofstream(second)<<"two";
 auto third=uniqueImagePath(testDir,L"image",L".png");assert(third!=first&&third!=second);
 std::filesystem::remove(first);std::filesystem::remove(second);std::filesystem::remove(testDir);
 std::cout<<"PASS: waterfall layout, non-overlap, drag order, animation convergence, responsive sizing, CSS curve, interrupted animation\n";
}
