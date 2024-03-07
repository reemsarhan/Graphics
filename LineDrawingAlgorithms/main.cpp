#include <iostream>
#include <Windows.h>
#include <cmath>


void swap(int& x1,int& y1,int& x2,int& y2){
    x1 ^= x2;
    x2 ^= x1;
    x1 ^= x2;

    y1 ^= y2;
    y2 ^= y1;
    y1 ^= y2;
}


//1)Direct Algorithm

void LineDrawingAlgo1(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color){

    int dx=x2-x1,dy=y2-y1;
    double m=(double)dy/dx;  double mi=(double)dx/dy;
    if(abs(dy)<=abs(dx)){  //slope<=1

        if(dx==0){  //Draw point m=dy/0
            SetPixel(hdc,x1,y1,color);
            return;
        }
        if(x2<x1){ // dx<0
            swap(x1, y1, x2, y2);
        }

        for (int x = x1; x <=x2 ; ++x) {
            int y=round(m*(x-x1)+y1);
            SetPixel(hdc,x,y,color);
        }
    }
    else{

        if(y2<y1){ //dy<0
            swap(x1, y1, x2, y2);
        }
        for (int y = y1; y <=y2 ; ++y) {
            int x=round(mi*(y-y1)+x1);
            SetPixel(hdc,x,y,color);
        }
    }

}


//2)Simple Digital Differential Analyzer Method (DDA)
void LineDrawingAlgo2(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color){

    int dx=x2-x1,dy=y2-y1;
    SetPixel(hdc,x1,y1,color);
    double m=(double)dy/dx; double mi=(double)dx/dy; //slope and slope inverse
    if(abs(dy)<=abs(dx)){  //slope<=1

        int x=x1; double y=(double)y1;
        while (x<x2){
            x++;
            y+=m;
            SetPixel(hdc,x,round(y),color);
        }
    }
    else{

        double x=(double)x1; int y=y1;
        while (y<y2){
            y++;
            x+=mi;
            SetPixel(hdc, round(x),y,color);
        }
    }

}


/// Bresenham Algorithm, Midpoint Alg, Integer(DDA)
void drawLineBresenham(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color){

    if(abs(x2 - x1) >= abs(y2 - y1)){  //slope <= 1

        if(x2 < x1) swap(x1,y1,x2,y2);

        int deltaX = x2 - x1; // as Sign may be changed after the swap.
        int deltaY = y2 - y1;
        int absDeltaY = abs(deltaY);

        int dInitial = deltaX - 2 * absDeltaY;
        int change1 = 2 * (deltaX - absDeltaY);
        int change2 = -2 * absDeltaY;

        SetPixel(hdc, x1, y1, color);//draw first point

        int x = x1, y = y1;
        while(x < x2){
            if(dInitial <= 0){
                y += (deltaY < 0 ? -1:1);
                dInitial += change1;
            }
            else{
                dInitial += change2;
            }
            x++;
            SetPixel(hdc, x,y,color);
        }
    }
    else{  //slope > 1

        if(y2 < y1) swap(x1,y1,x2,y2);

        int deltaX = x2 - x1;
        int deltaY = y2 - y1;
        int absDeltaX = abs(deltaX);

        int dInitial = deltaY - 2 * absDeltaX;
        int change1 = 2 * (deltaY - absDeltaX);
        int change2 = -2 * absDeltaX;

        SetPixel(hdc, x1, y1, color);//draw fist point
        int x = x1, y = y1;

        while(y < y2){
            if(dInitial <= 0){
                x += (deltaX < 0 ? -1:1);
                dInitial += change1;
            }
            else{
                dInitial += change2;
            }
            y++;
            SetPixel(hdc, x,y,color);
        }
    }
}



LRESULT WINAPI WndProc(HWND hwnd, UINT m, WPARAM wp, LPARAM lp)
{

    static int startx,starty,endx,endy;
    HDC hdc;

    switch (m)
    {
        case WM_LBUTTONDOWN:
        startx= LOWORD(lp);
        starty= HIWORD(lp);
            break;
        case WM_LBUTTONUP:
            endx= LOWORD(lp);
            endy= HIWORD(lp);

            hdc= GetDC(hwnd);
           // PAINTSTRUCT ps;
//            HDC hdc = BeginPaint(hwnd, &ps);



//            int startX = 500;
//            int startY = 300;
//            int endX = 100;
//            int endY = 100;

//            COLORREF lineColor = RGB(0, 0, 0); // Blue color

          //  LineDrawingAlgo1(hdc, startx, starty, endx, endy, RGB(159,43,104));
          //  LineDrawingAlgo2(hdc, startx, starty, endx, endy, RGB(159,43,104));
           // drawLineBresenham(hdc, startx, starty, endx, endy, RGB(159, 43, 104)); // Draws a red line

            ReleaseDC(hwnd,hdc);
            break;
//            EndPaint(hwnd, &ps);
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



//
//
////3)Bresenham's Algorithm (Midpoint Alg) (Integer DDA)
//void LineDrawingAlgo3(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color){
//    int deltaX=x2-x1,deltaY=y2-y1;
//
//    int d_init = deltaX - 2 * deltaY;
//    int change1, change2 ;
//
//    if(deltaY<=deltaX){  //slope<=1
//        if(x2<x1){
//            swap(x1, x2, y1, y2);
//        }
//        //in case we SWAP
//        deltaX=x2-x1;
//        deltaY=y2-y1;
//        change1 = 2 * (deltaX - abs(deltaY)), change2 = -2 * abs(deltaY);
//        int x=x1,y=y1;
//        while (x<x2)
//        {
//            if (d_init <= 0) {
//                if(deltaY<0){
//                    y--;
//                }
//                else{
//                    y++;
//                }
//                d_init += change1;
//            }
//            else {
//                d_init += change2;
//            }
//            x++;
//            SetPixel(hdc, x, y, color);
//        }
//
//    }
//    else{  //slope >1
//        if(y1<y2){
//            swap(x1, x2, y1, y2);
//        }
//        deltaX=x2-x1;
//        deltaY=y2-y1;
//        change1 = 2 * (deltaY - abs(deltaX)), change2 = -2 * abs(deltaX);
//        int x=x1,y=y1;
//        while (y<y2)
//        {
//            if (d_init <= 0) {
//                if(deltaX<0){
//                    x--;
//                }
//                else{
//                    y++;
//                }
//                d_init += change1;
//            }
//            else {
//                d_init += change2;
//            }
//            y++;
//            SetPixel(hdc, x, y, color);
//        }
//    }




