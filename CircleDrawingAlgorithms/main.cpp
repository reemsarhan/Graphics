#include <iostream>
#include <Windows.h>
#include <cmath>


void Draw8Points(HDC hdc,int xc,int yc,int x,int y,COLORREF color){
    SetPixel(hdc,xc+x,yc+y,color);
    SetPixel(hdc,xc-x,yc+y,color);
    SetPixel(hdc,xc-x,yc-y,color);
    SetPixel(hdc,xc+x,yc-y,color);
    SetPixel(hdc,xc+y,yc+x,color);
    SetPixel(hdc,xc-y,yc+x,color);
    SetPixel(hdc,xc-y,yc-x,color);
    SetPixel(hdc,xc+y,yc-x,color);

}


//Naive algo using second octact
void DrawCicle1(HDC hdc, int xc, int yc, int R, COLORREF color){
    int x=0,y=R;
    int R2=R*R;
    Draw8Points(hdc,xc,yc,x,y,color);
    while(x<y){
        x++;
        y=round(sqrt((double)(R2-x*x)));
        Draw8Points(hdc,xc,yc,x,y,color);
    }
}

//Polar Algorithm
void DrawCicle2(HDC hdc, int xc, int yc, int R, COLORREF color) {
    int x=R,y=0;
    Draw8Points(hdc,xc,yc,x,y,color);
    double Theta=0,dTheta=1.0/R;

    while (x<y){
        Theta+=dTheta;
        x= round(R*cos(Theta));
        y=round(R*sin(Theta));
        Draw8Points(hdc,xc,yc,x,y,color);
    }
}




//Iterative Polar Algorithm
void DrawCicle3(HDC hdc, int xc, int yc, int R, COLORREF color) {
    double x=R,y=0;
    Draw8Points(hdc,xc,yc,x,y,color);

    double dTheta=1.0/R;
    double c=cos(dTheta) , s=sin(dTheta);

    while (x<y){
        double temp=x*c-y*s;
        y=x*s+y*c;
        x=temp;
        Draw8Points(hdc,xc,yc,x,y,color);
    }
}

//Based on first Order Differential

void DrawCicle4(HDC hdc, int xc, int yc, int R, COLORREF color) {
    double x=R,y=0;
    Draw8Points(hdc,xc,yc,x,y,color);
    int d=1-R;

    while (x<y){
        if(d>=0){
            d+=(2*(x-y)+5);
            y--;
        }
        else{
            d+=(2*x+3);
        }
        x++;
        Draw8Points(hdc,xc,yc,x,y,color);
    }
}


//Based on second Order Differential

void DrawCicle5(HDC hdc, int xc, int yc, int R, COLORREF color) {
    double x=R,y=0;
    Draw8Points(hdc,xc,yc,x,y,color);

    int d=1-R;
    int ch1=3,ch2=5-2*R;

    while (x<y){

        if(d<0){
            d+=ch1;
            ch2+=2;
        }
        else{
            d+=ch2;
            ch2+=4;
            y--;
        }
        ch1+=2;
        x++;
        Draw8Points(hdc,xc,yc,x,y,color);
    }
}




LRESULT WINAPI WndProc(HWND hwnd, UINT m, WPARAM wp, LPARAM lp)
{

    static int startx,starty,endx,endy,radius;
    HDC hdc;

    switch (m)
    {
        case WM_LBUTTONDOWN:
            startx = LOWORD(lp);
            starty = HIWORD(lp);
            break;
        case WM_LBUTTONUP:
            endx = LOWORD(lp);
            endy = HIWORD(lp);

            hdc = GetDC(hwnd);
            radius = static_cast<int>(sqrt((endx - startx) * (endx - startx) + (endy - starty) * (endy - starty)));
           DrawCicle1(hdc, startx, starty, radius, RGB(159, 43, 104));
           //DrawCicle2(hdc, startx, starty, radius, RGB(159, 43, 104));
          // DrawCicle3(hdc, startx, starty, radius, RGB(159, 43, 104));
           //DrawCicle4(hdc, startx, starty, radius, RGB(159, 43, 104));
        //  DrawCicle5(hdc, startx, starty, radius, RGB(159, 43, 104));


            ReleaseDC(hwnd, hdc);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        default:
            return DefWindowProc(hwnd, m, wp, lp);
    }
    return 0;
}








int APIENTRY WinMain(HINSTANCE hi, HINSTANCE pi, LPSTR c, int ns)
{
    WNDCLASS wc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hInstance = hi;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = "MyClass";
    wc.lpszMenuName = NULL;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&wc);
    HWND hwnd = CreateWindow("MyClass", "Hello World", WS_OVERLAPPEDWINDOW, 0, 0, 600, 400, NULL, NULL, hi, 0);
    ShowWindow(hwnd, ns);
    UpdateWindow(hwnd);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

