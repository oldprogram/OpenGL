// MainWnd.cpp: implementation of the CMainWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainWnd.h"
#include <sstream>
#include <fstream>
using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMainWnd::CMainWnd():
m_hWnd(NULL),
m_fAngle(0.0f),
m_hRC(NULL)
{

}

CMainWnd::~CMainWnd()
{
}

//
// GLCube()
// Renders a cube.
//
#define YU 1//800//2
void CMainWnd::GLCube()
{
	for(int i=0;i<m_pic.F.size();i++)
	{
		glBegin(GL_TRIANGLES);							// 绘制三角形
		if(m_pic.VT.size()!=0)glTexCoord2f(m_pic.VT[m_pic.F[i].T[0]].TU,m_pic.VT[m_pic.F[i].T[0]].TV);  //纹理	
		if(m_pic.VN.size()!=0)glNormal3f(m_pic.VN[m_pic.F[i].N[0]].NX,m_pic.VN[m_pic.F[i].N[0]].NY,m_pic.VN[m_pic.F[i].N[0]].NZ);//法向量
		glVertex3f(m_pic.V[m_pic.F[i].V[0]].X/YU,m_pic.V[m_pic.F[i].V[0]].Y/YU, m_pic.V[m_pic.F[i].V[0]].Z/YU);		// 上顶点

		if(m_pic.VT.size()!=0)glTexCoord2f(m_pic.VT[m_pic.F[i].T[1]].TU,m_pic.VT[m_pic.F[i].T[1]].TV);  //纹理
		if(m_pic.VN.size()!=0)glNormal3f(m_pic.VN[m_pic.F[i].N[1]].NX,m_pic.VN[m_pic.F[i].N[1]].NY,m_pic.VN[m_pic.F[i].N[1]].NZ);//法向量
		glVertex3f(m_pic.V[m_pic.F[i].V[1]].X/YU,m_pic.V[m_pic.F[i].V[1]].Y/YU, m_pic.V[m_pic.F[i].V[1]].Z/YU);		// 左下

		if(m_pic.VT.size()!=0)glTexCoord2f(m_pic.VT[m_pic.F[i].T[2]].TU,m_pic.VT[m_pic.F[i].T[2]].TV);  //纹理
		if(m_pic.VN.size()!=0)glNormal3f(m_pic.VN[m_pic.F[i].N[2]].NX,m_pic.VN[m_pic.F[i].N[2]].NY,m_pic.VN[m_pic.F[i].N[2]].NZ);//法向量
		glVertex3f(m_pic.V[m_pic.F[i].V[2]].X/YU,m_pic.V[m_pic.F[i].V[2]].Y/YU, m_pic.V[m_pic.F[i].V[2]].Z/YU);		// 右下
		glEnd();										// 三角形绘制结束	
	}		
}

//
// InitScene()
// Called when the OpenGL RC has been created. Sets the initial state for
// the OpenGL scene.
//
void CMainWnd::InitScene()
{
	ReadPIC();
	//glClear函数来自OPENGL,其中它是通过glClear使用红，绿，蓝以及AFA值来清除颜色缓冲区的，
	//并且都被归一化在（0，1）之间的值，其实就是清空当前的所有颜色。
	glClearColor(0.000f, 0.000f, 0.000f, 1.0f); //Background color
	
	// TODO: Replace the following sample code with your initialization code.
	
	// Activate lighting and a light source
	//用于启用各种功能。具体功能由参数决定。与glDisable相对应。glDisable用以关闭各项功能。
	glEnable(GL_LIGHT0);//启用0号灯到7号灯(光源)　　光源要求由函数glLight函数来完成
	glEnable(GL_LIGHTING);//启用灯源
	glEnable(GL_DEPTH_TEST);//启用深度测试。　　根据坐标的远近自动隐藏被遮住的图形（材料）
	 glEnable(GL_TEXTURE_2D);   // 启用二维纹理

	// Define material parameters
	static GLfloat glfMatAmbient[] = {0.000f, 0.450f, 1.000f, 1.0f};
	static GLfloat glfMatDiffuse[] = {0.000f, 0.000f, 0.580f, 1.0f};
	static GLfloat glfMatSpecular[]= {1.000f, 1.000f, 1.000f, 1.0f};
	static GLfloat glfMatEmission[]= {0.000f, 0.000f, 0.000f, 1.0f};
	static GLfloat fShininess = 128.000f;
	
	// Set material parameters
	//指定用于光照计算的当前材质属性。参数face的取值可以是GL_FRONT、GL_BACK或GL_FRONT_AND_BACK，指出材质属性将应用于物体的哪面。
	//void glMaterial{if}(GLenum face, GLenum pname, TYPE param);
	glMaterialfv(GL_FRONT, GL_AMBIENT,  glfMatAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE,  glfMatDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, glfMatSpecular);
	glMaterialfv(GL_FRONT, GL_EMISSION, glfMatEmission);
	glMaterialf(GL_FRONT, GL_SHININESS, fShininess);
}

//
// KillScene()
// Called when the OpenGL RC is about to be destroyed. 
//
void CMainWnd::KillScene()
{
	// TODO: Use KillScene to free resources.
}

//
// DrawScene()
// Called each time the OpenGL scene has to be drawn.
//
void CMainWnd::DrawScene()
{
	// TODO: Replace the following sample code with your code to draw the scene.
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear buffers  // 清除屏幕及深度缓存
	glLoadIdentity(); // Load identity matrix	// 重置模型观察矩阵
	
	// Add a light source
	GLfloat glfLight[] = {-4.0f, 4.0f, 4.0f, 0.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, glfLight);
	
	// Position and rotate the camera
	glTranslatef(0.0f, 0.0f, -5.0f);	// 移入屏幕 5.0
	glRotatef(30.0f, 1.0f, 0.0f, 0.0f);	//绕X轴旋转
	glRotatef(m_fAngle, 0.0f, 1.0f, 0.0f);
	
	// Draw a cube
	GLCube();
	
	glFlush();
}

//
// Tick()
// The applications "heartbeat" function. Called before another frame needs
// to be drawn. Use this function to calculate new positions for objects in 
// the scene, for instance. Set bRedrawScene = FALSE if the scene should not be 
// redrawn.
//
void CMainWnd::Tick(BOOL &bRedrawScene)
{
	m_fAngle+= 0.1; // Add some rotation to the cube
}

void CMainWnd::ReadPIC()
{
	ifstream ifs("Eight.obj");//cube bunny Eight
	string s;
	Mian *f;
	POINT3 *v;
	FaXiangLiang *vn;
	WenLi	*vt;
	while(getline(ifs,s))
	{
		if(s.length()<2)continue;
		if(s[0]=='v'){
			if(s[1]=='t'){//vt 0.581151 0.979929 纹理
				istringstream in(s);
				vt=new WenLi();
				string head;
				in>>head>>vt->TU>>vt->TV;
				m_pic.VT.push_back(*vt);
			}else if(s[1]=='n'){//vn 0.637005 -0.0421857 0.769705 法向量
				istringstream in(s);
				vn=new FaXiangLiang();
				string head;
				in>>head>>vn->NX>>vn->NY>>vn->NZ;
				m_pic.VN.push_back(*vn);
			}else{//v -53.0413 158.84 -135.806 点
				istringstream in(s);
				v=new POINT3();
				string head;
				in>>head>>v->X>>v->Y>>v->Z;
				m_pic.V.push_back(*v);
			}
		}
		else if(s[0]=='f'){//f 2443//2656 2442//2656 2444//2656 面
			for(int k=s.size()-1;k>=0;k--){
				if(s[k]=='/')s[k]=' ';
			}
			istringstream in(s);
			f=new Mian();
			string head;
			in>>head;
			int i=0;
			while(i<3)
			{
				if(m_pic.V.size()!=0)
				{
					in>>f->V[i];
					f->V[i]-=1;
				}
				if(m_pic.VT.size()!=0)
				{
					in>>f->T[i];
					f->T[i]-=1;
				}
				if(m_pic.VN.size()!=0)
				{
					in>>f->N[i];
					f->N[i]-=1;
				}
				i++;
			}
			m_pic.F.push_back(*f);
		}
	}
}
