#include "graphics.h"
#include "MyShape.h"
#include "myworld.h"
#include "algebra.h"
#include <Imagine/Graphics.h>
//#include <sstream>
using namespace Imagine;
namespace View{
int winWidth,winHeight;
double viewZoneLeft,viewZoneRight,viewZoneUp,viewZoneBottom;
double oneMeterLengthOnScreen;
double viewWidth,viewHeight;
double ratioWidth,ratioHeight;
Vector2 viewCenter;
}
void coordTranslate(double x,double y,int& x_onScreen,int& y_onScreen){
    using namespace View;
    x_onScreen=(x-viewZoneLeft)*ratioWidth;
    y_onScreen=(viewZoneUp-y)*ratioHeight;
}
void coordTranslateInverse(int x_onScreen, int y_onScreen, double& realX,double& realY)
{
    using namespace View;
    realX=x_onScreen/ratioWidth+viewZoneLeft;
    realY=viewZoneUp-y_onScreen/ratioHeight;
}
void drawButton(World::Button b){
    if(b.state){
        fillRect(b.x,b.y,b.w,b.h,RED);
    }else{
        fillRect(b.x,b.y,b.w,b.h,BLUE);
    }
    drawString(b.x,b.y,b.name,BLACK);

}

void myDisplay(bool DebugDrawing,Color col)
{
    using namespace World;

    setBackGround(WHITE);
    drawAxis();
    if(DebugDrawing){
        for(vector<MyShape*>::iterator i=ShapeList.begin();i<ShapeList.end();i++){
            MyShape* s=*i;
            switch (s->shapeType) {
            case POLYGON:
            {
                MyPolygon* sP=static_cast<MyPolygon*>(s);
                MyPolygon tempPoly(*sP);
                tempPoly.makeMove(dt);
                draw_each(&tempPoly,RED,false);
                break;
            }
            case CIRCLE:
                break;
            default:
                break;
            }


        }
    }
    for(vector<MyShape*>::iterator i=ShapeList.begin();i<ShapeList.end();i++){
        draw_each(*i,col,true);
    }


    drawButton(buttonDrag);
    drawButton(buttonPoly);
    drawButton(buttonCircle);


}
void draw_each(MyShape* s,Color col,bool drawID)
{
    switch (s->shapeType) {
    case POLYGON:
        myDrawPolygon(*static_cast<MyPolygon*>(s),col,drawID);
        break;
    case CIRCLE:
        myDrawCircle(*static_cast<MyCircle*>(s),col,drawID);
        break;
    default:
        break;
    }

}
void drawAxis()
{
    int i,j;
    int n=5;
    for(i=-n;i<=n;i++){
        myDrawLine(i,-n,i,n,Color(200,200,200));
    }
    for(j=-n;j<=n;j++){
        myDrawLine(-n,j,n,j,Color(200,200,200));
    }
    myDrawLine(0,-n,0,n,Color(200,200,200));
    myDrawLine(-n,0,n,0,Color(200,200,200));
}
void myDrawPoint(const Vector2& p, int radius, Color col) {

    int x,y;
    coordTranslate(p.x,p.y,x,y);
    fillCircle(x,y,radius,col);
}
void myDrawPolygon(MyPolygon &p, Color col,bool drawID){
    if(p.shapeType!=POLYGON){cout<<"drawpolygon need polygon"<<endl;exit(0);}
    vector<Vector2 >::iterator v;

    for (v=p.vertex.begin(); v<p.vertex.end()-1; ++v){
        myDrawLine((*v).x,(*v).y,(*(v+1)).x,(*(v+1)).y, col);

    }
    myDrawLine(p.vertex.back().x,p.vertex.back().y,p.vertex.front().x,p.vertex.front().y, col);

    if(drawID){
        std::stringstream ss;
        ss<<p.ID;
        int xx1,yy1;
        coordTranslate(p.center.x,p.center.y,xx1,yy1);
        drawString(xx1,yy1,(ss.str()),col);
    }
}

void myDrawCircle(MyCircle &p, Color col,bool drawID){
    if(p.shapeType!=CIRCLE){cout<<"drawCircle need circle"<<endl;exit(0);}
    int xx1,yy1,xx2,yy2,r;
    Vector2 pointOnCircle;
    Transform trans(p.angle,p.center);
    pointOnCircle=trans.Apply(Vector2(p.radius,0));
    coordTranslate(p.center.x,p.center.y,xx1,yy1);
    coordTranslate(pointOnCircle.x,pointOnCircle.y,xx2,yy2);
    r=p.radius*View::oneMeterLengthOnScreen;

    drawCircle(xx1,yy1,r,col);
    drawLine(xx1,yy1,xx2,yy2,col);
    if(drawID){
        std::stringstream ss;
        ss<<p.ID;
        int xx1,yy1;
        coordTranslate(p.center.x,p.center.y,xx1,yy1);
        drawString(xx1,yy1,(ss.str()),col);
    }
}
void myDrawLine(double x1,double y1,double x2,double y2,Color col,int penWidth){
    using namespace View;
    int xx1,xx2,yy1,yy2;
    coordTranslate(x1,y1,xx1,yy1);
    coordTranslate(x2,y2,xx2,yy2);
    drawLine(xx1,yy1,xx2,yy2,col,penWidth);
}


void initialView(int width, int height)
{
    using namespace View;
    winWidth=width;
    winHeight=height;
    viewCenter=Vector2(0.0,0);
    oneMeterLengthOnScreen=100.0;
    viewZoneLeft    =-winWidth/2/oneMeterLengthOnScreen+viewCenter.x;
    viewZoneRight   =winWidth/2/oneMeterLengthOnScreen+viewCenter.x;
    viewZoneUp      =winHeight/2/oneMeterLengthOnScreen+viewCenter.y;
    viewZoneBottom  =-winHeight/2/oneMeterLengthOnScreen+viewCenter.y;
    viewWidth=viewZoneRight-viewZoneLeft;
    viewHeight=viewZoneUp-viewZoneBottom;
    ratioWidth=winWidth/viewWidth;
    ratioHeight=winHeight/viewHeight;
}



