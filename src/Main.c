// #include "C:/Wichtig/System/Static/Library/WindowEngine1.0.h"
// #include "C:/Wichtig/System/Static/Library/Victor2.h"
// #include "C:/Wichtig/System/Static/Library/Vdctor2.h"
// #include "C:/Wichtig/System/Static/Library/Complex.h"
// #include "C:/Wichtig/System/Static/Library/TransformedView.h"
// #include "C:\Wichtig\System\Static\Container\Vector.h"

#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/Victor2.h"
#include "/home/codeleaded/System/Static/Library/Vdctor2.h"
#include "/home/codeleaded/System/Static/Library/Complex2.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"
#include "/home/codeleaded/System/Static/Container/Vector.h"


TransformedViewD tv;
int nMode = 0;
int nIterations = 1;
float Curving = 0.5f;

void RenderPythagorasTree(Vec2 p1,Vec2 p2,float q,int Iteration,int MaxIteration,Pixel first,Pixel second){
	Vec2 delta = Vec2_Sub(p2,p1);
	if(Iteration<MaxIteration){
		Vec2 p3 = Vec2_Add(p1,Vec2_Add(Vec2_Mulf(delta,0.5f),Vec2_Perp(Vec2_Mulf(delta,0.5f))));
		Vec2 p4 = Vec2_Add(p1,Vec2_Add(Vec2_Mulf(delta,0.5f),Vec2_PerpA(Vec2_Mulf(delta,0.5f))));
		Vec2 dir = Vec2_Sub(p3,p2);
		Vec2 p5 = Vec2_Add(p2,Vec2_Add(Vec2_Mulf(dir,0.5f),Vec2_Mulf(Vec2_PerpA(dir),q)));

		float a = F32_Abs(delta.y / delta.x);

		Vec2 d1 = Vec2_Sub(p5,p3);
		Vec2 d2 = Vec2_Sub(p5,p2);

		Vec2 p6 = Vec2_Add(p2,Vec2_PerpA(d2));
		Vec2 p7 = Vec2_Add(p5,Vec2_Perp(d1));
		
		Pixel col = Pixel_Inter(first,second,(float)Iteration / (float)MaxIteration);
		//Poly_RenderX(WINDOW_STD_ARGS,(Vec2[]){ p1,p2,p3,p4 },4,col);
		Poly_RenderXWire(WINDOW_STD_ARGS,(Vec2[]){ p1,p2,p3,p4 },4,col,1.0f);

		//RenderTriangle(p3,p2,p5,col);
		RenderTriangleWire(p3,p2,p5,col,1.0f);
		RenderPythagorasTree(p5,p6,q,Iteration+1,MaxIteration,first,second);
		RenderPythagorasTree(p3,p7,q,Iteration+1,MaxIteration,first,second);
	}
}

void Setup(AlxWindow* w){
	tv = TransformedViewD_New((Vdc2){ 1.0f,1.0f });
	tv.Scale = (Vdc2){ 1000.0f,1000.0f };

	nIterations = 5;

	//nMode = 5;

	ResizeAlxFont(16,16);
}

void Update(AlxWindow* w){
	tv.ZoomSpeed = (float)w->ElapsedTime;
	TransformedViewD_HandlePanZoom(&tv,window.Strokes,(Vdc2){ GetMouse().x,GetMouse().y });
	
	Vic2 pix_tl = { 0,0 };
	Vic2 pix_br = { GetWidth(), GetHeight() };
	Vdc2 frac_tl = { -2, -1 };
	Vdc2 frac_br = { 1, 1 };
	
	frac_tl = TransformedViewD_ScreenWorldPos(&tv,(Vdc2){ pix_tl.x,pix_tl.y });
	frac_br = TransformedViewD_ScreenWorldPos(&tv,(Vdc2){ pix_br.x,pix_br.y });

	if (Stroke(ALX_KEY_UP).DOWN) 	nIterations += 1;
	if (Stroke(ALX_KEY_DOWN).DOWN) 	nIterations -= 1;
	if (nIterations < 1) 			nIterations = 1;

	if (Stroke(ALX_KEY_LEFT).DOWN) 	Curving *= 0.99f;
	if (Stroke(ALX_KEY_RIGHT).DOWN) Curving *= 1.01f;

	Timepoint tp1 = Time_Nano();

	Clear(BLACK);

	Vdc2 p1 = TransformedViewD_WorldScreenPos(&tv,(Vdc2){ 1.0f,1.0f });
	Vdc2 p2 = TransformedViewD_WorldScreenPos(&tv,(Vdc2){ 0.0f,0.0f });
	RenderPythagorasTree((Vec2){ p1.x,p1.y },(Vec2){ p2.x,p2.y },Curving,0,nIterations,BLUE,GREEN);
	
	Timepoint tp2 = Time_Nano();
	double elapsedTime = Time_Elapsed(tp1,tp2);

	String str = String_Format("| w->ElapsedTime Taken: %fs - Iterations: %d |",elapsedTime,nIterations);
	char* cstr = String_CStr(&str);
	RenderCStr(cstr,0,30,WHITE);
	free(cstr);
	String_Free(&str);
}

void Delete(AlxWindow* w){
    
}

int main(){
    if(Create("PythagorasTree",1200,1200,1,1,Setup,Update,Delete))
        Start();
    return 0;
}