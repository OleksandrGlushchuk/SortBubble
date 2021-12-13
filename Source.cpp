#include "freeglut.h"
#include<vector>
#include<ctime>
#include<thread>
#include<chrono>
#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"
using namespace std;
int width, height;
const float small = 1e-6f, edge = 100.f;
unsigned int tex_numbers, tex_restart, tex_start;
void LoadTexture(const char* name, unsigned int& tex)
{
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	int t_width, t_height, channels;
	unsigned char* data = stbi_load(name, &t_width, &t_height, &channels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t_width, t_height, 0, channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
}

void SetTexturesCoords(float& tx1, float& ty1, float& tx2, float& ty2, int v)
{
	switch (v)
	{
	case 0:
		tx1 = 0.0f;  ty1 = 0.0f;
		tx2 = 0.16f; ty2 = 0.43f;
		break;
	case 1:
		tx1 = 0.2f;  ty1 = 0.0f;
		tx2 = 0.35f;  ty2 = 0.43f;
		break;
	case 2:
		tx1 = 0.38f;  ty1 = 0.0f;
		tx2 = 0.55f;  ty2 = 0.43f;
		break;
	case 3:
		tx1 = 0.6f;  ty1 = 0.0f;
		tx2 = 0.77f;  ty2 = 0.43f;
		break;
	case 4:
		tx1 = 0.8f;  ty1 = 0.0f;
		tx2 = 1.0f;  ty2 = 0.43f;
		break;
	case 5:
		tx1 = 0.0f;  ty1 = 0.57f;
		tx2 = 0.16f;  ty2 = 1.0f;
		break;
	case 6:
		tx1 = 0.2f;  ty1 = 0.57f;
		tx2 = 0.38f;  ty2 = 1.0f;
		break;
	case 7:
		tx1 = 0.42f;  ty1 = 0.57f;
		tx2 = 0.58f;  ty2 = 1.0f;
		break;
	case 8:
		tx1 = 0.62f;  ty1 = 0.57f;
		tx2 = 0.8f;  ty2 = 1.0f;
		break;
	case 9:
		tx1 = 0.83f;  ty1 = 0.57f;
		tx2 = 1.0f;  ty2 = 1.0f;
		break;
	}
}

class Spysok
{
	static float side;
	int number;
	float koef;
	struct Kube
	{
	private:
		void DrawKube()
		{
			glBegin(GL_QUADS);
			glVertex3f(x1, y1, z1);
			glVertex3f(x1, y2, z1);
			glVertex3f(x2, y2, z1);
			glVertex3f(x2, y1, z1);
			
			glVertex3f(x1, y1, z2);
			glVertex3f(x1, y2, z2);
			glVertex3f(x2, y2, z2);
			glVertex3f(x2, y1, z2);

			glVertex3f(x1, y1, z1);
			glVertex3f(x1, y2, z1);
			glVertex3f(x1, y2, z2);
			glVertex3f(x1, y1, z2);

			glVertex3f(x2, y1, z1);
			glVertex3f(x2, y2, z1);
			glVertex3f(x2, y2, z2);
			glVertex3f(x2, y1, z2);

			glVertex3f(x1, y2, z1);
			glVertex3f(x1, y2, z2);
			glVertex3f(x2, y2, z2);
			glVertex3f(x2, y2, z1);

			glVertex3f(x1, y1, z1);
			glVertex3f(x1, y1, z2);
			glVertex3f(x2, y1, z2);
			glVertex3f(x2, y1, z1);
			glEnd();
		}
	public:

		int value;
		float x1, y1, z1, x2,y2,z2;
		GLubyte color[3];
		void SetColor(GLubyte r, GLubyte g, GLubyte b)
		{
			color[0] = r;
			color[1] = g;
			color[2] = b;
		}
		Kube()
		{
			value = 0;
			x1 = 0.0f;
			x2 = side;
			y1 = side / 2.0f;
			y2 = -side / 2.0f;
			z1 = 0;
			z2 = -side/(edge*20);
			SetColor(30, 0, 180);
		}
		void Draw()
		{
			glBindTexture(GL_TEXTURE_2D, tex_numbers);
			float tx1, ty1, tx2, ty2;
			glPushMatrix();
			glColor3ubv(color);
			glTranslatef(0, 0, -2*small);
			glRectf(x1, y1, x2, y2);
			glTranslatef(0, 0, small);
			glColor3ub(30, 250, 180);
			glLineWidth(4);
			glBegin(GL_LINE_LOOP);
			glVertex2f(x1, y1);
			glVertex2f(x1, y2);
			glVertex2f(x2, y2);
			glVertex2f(x2, y1);
			glEnd();
			glBegin(GL_LINE_LOOP);
			glVertex3f(x1, y1, z1);
			glVertex3f(x1, y2, z1);
			glVertex3f(x1, y2, z2);
			glVertex3f(x1, y1, z2);
			glEnd();
			glBegin(GL_LINE_LOOP);
			glVertex3f(x1, y1, z1);
			glVertex3f(x2, y1, z1);
			glVertex3f(x2, y1, z2);
			glVertex3f(x1, y1, z2);
			glEnd();
			glBegin(GL_LINE_LOOP);
			glVertex3f(x1, y2, z1);
			glVertex3f(x2, y2, z1);
			glVertex3f(x2, y2, z2);
			glVertex3f(x1, y2, z2);
			glEnd();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(0, 0, -2*small);
			glColor3ubv(color);
			DrawKube();
			glColor3f(1, 1, 1);
			glTranslatef(0, 0, small);
			if (value / 100 == 0)
			{
				if (value / 10 == 0)
				{
					if (value < 0)
					{
						glRectf(x1 + 10, y1 - side * 0.45f, x1 + side / 2.0f - 10, y1 - side * 0.55f);
						glEnable(GL_ALPHA_TEST);
						glAlphaFunc(GL_GREATER, 0.9f);
						SetTexturesCoords(tx1, ty1, tx2, ty2, abs(value));
						glBegin(GL_QUADS);
						glTexCoord2f(tx1, ty1);		glVertex2f(x1 + side / 2.0f, y1);
						glTexCoord2f(tx1, ty2);		glVertex2f(x1 + side / 2.0f, y2);
						glTexCoord2f(tx2, ty2);		glVertex2f(x2, y2);
						glTexCoord2f(tx2, ty1);		glVertex2f(x2, y1);
						glEnd();
					}
					else
					{
						glEnable(GL_ALPHA_TEST);
						glAlphaFunc(GL_GREATER, 0.9f);
						SetTexturesCoords(tx1, ty1, tx2, ty2, value);
						glBegin(GL_QUADS);
						glTexCoord2f(tx1, ty1);		glVertex2f(x1, y1);
						glTexCoord2f(tx1, ty2);		glVertex2f(x1, y2);
						glTexCoord2f(tx2, ty2);		glVertex2f(x2, y2);
						glTexCoord2f(tx2, ty1);		glVertex2f(x2, y1);
						glEnd();
					}
				}
				else
				{
					if (value < 0)
					{
						glRectf(x1 + 7, y1 - side * 0.45f, x1 + side / 3.0f - 7, y1 - side * 0.55f);

						glEnable(GL_ALPHA_TEST);
						glAlphaFunc(GL_GREATER, 0.9f);
						int v1 = abs(value) / 10;
						SetTexturesCoords(tx1, ty1, tx2, ty2, v1);
						glBegin(GL_QUADS);
						glTexCoord2f(tx1, ty1);		glVertex2f(x1 + side / 3.0f, y1);
						glTexCoord2f(tx1, ty2);		glVertex2f(x1 + side / 3.0f, y2);
						glTexCoord2f(tx2, ty2);		glVertex2f(x1 + 2.0f * side / 3.0f, y2);
						glTexCoord2f(tx2, ty1);		glVertex2f(x1 + 2.0f * side / 3.0f, y1);
						glEnd();
						v1 = abs(value) - v1 * 10;
						SetTexturesCoords(tx1, ty1, tx2, ty2, v1);
						glBegin(GL_QUADS);
						glTexCoord2f(tx1, ty1);		glVertex2f(x1 + 2.0f * side / 3.0f, y1);
						glTexCoord2f(tx1, ty2);		glVertex2f(x1 + 2.0f * side / 3.0f, y2);
						glTexCoord2f(tx2, ty2);		glVertex2f(x2, y2);
						glTexCoord2f(tx2, ty1);		glVertex2f(x2, y1);
						glEnd();
					}
					else
					{
						glEnable(GL_ALPHA_TEST);
						glAlphaFunc(GL_GREATER, 0.9f);
						int v1 = value / 10;
						SetTexturesCoords(tx1, ty1, tx2, ty2, v1);
						glBegin(GL_QUADS);
						glTexCoord2f(tx1, ty1);		glVertex2f(x1, y1);
						glTexCoord2f(tx1, ty2);		glVertex2f(x1, y2);
						glTexCoord2f(tx2, ty2);		glVertex2f(x1 + side / 2.0f, y2);
						glTexCoord2f(tx2, ty1);		glVertex2f(x1 + side / 2.0f, y1);
						glEnd();
						v1 = value - v1 * 10;
						SetTexturesCoords(tx1, ty1, tx2, ty2, v1);
						glBegin(GL_QUADS);
						glTexCoord2f(tx1, ty1);		glVertex2f(x1 + side / 2.0f, y1);
						glTexCoord2f(tx1, ty2);		glVertex2f(x1 + side / 2.0f, y2);
						glTexCoord2f(tx2, ty2);		glVertex2f(x2, y2);
						glTexCoord2f(tx2, ty1);		glVertex2f(x2, y1);
						glEnd();
					}
				}
			}
			else
			{
				if (value < 0)
				{
					glRectf(x1 + 6, y1 - side * 0.45f, x1 + side / 4.0f - 6, y1 - side * 0.55f);

					glEnable(GL_ALPHA_TEST);
					glAlphaFunc(GL_GREATER, 0.9f);
					int v1 = abs(value) / 100;
					SetTexturesCoords(tx1, ty1, tx2, ty2, v1);
					glBegin(GL_QUADS);
					glTexCoord2f(tx1, ty1);		glVertex2f(x1 + side / 4.0f, y1);
					glTexCoord2f(tx1, ty2);		glVertex2f(x1 + side / 4.0f, y2);
					glTexCoord2f(tx2, ty2);		glVertex2f(x1 + side / 2.0f, y2);
					glTexCoord2f(tx2, ty1);		glVertex2f(x1 + side / 2.0f, y1);
					glEnd();
					v1 = abs(value) - v1 * 100;
					int v2 = v1 / 10;
					SetTexturesCoords(tx1, ty1, tx2, ty2, v2);
					glBegin(GL_QUADS);
					glTexCoord2f(tx1, ty1);		glVertex2f(x1 + side / 2.0f, y1);
					glTexCoord2f(tx1, ty2);		glVertex2f(x1 + side / 2.0f, y2);
					glTexCoord2f(tx2, ty2);		glVertex2f(x1 + 3.0f * side / 4.0f, y2);
					glTexCoord2f(tx2, ty1);		glVertex2f(x1 + 3.0f * side / 4.0f, y1);
					glEnd();
					v2 = v1 - v2 * 10;
					SetTexturesCoords(tx1, ty1, tx2, ty2, v2);
					glBegin(GL_QUADS);
					glTexCoord2f(tx1, ty1);		glVertex2f(x1 + 3.0f * side / 4.0f, y1);
					glTexCoord2f(tx1, ty2);		glVertex2f(x1 + 3.0f * side / 4.0f, y2);
					glTexCoord2f(tx2, ty2);		glVertex2f(x2, y2);
					glTexCoord2f(tx2, ty1);		glVertex2f(x2, y1);
					glEnd();
				}
				else
				{
					glEnable(GL_ALPHA_TEST);
					glAlphaFunc(GL_GREATER, 0.9f);
					int v1 = value / 100;
					SetTexturesCoords(tx1, ty1, tx2, ty2, v1);
					glBegin(GL_QUADS);
					glTexCoord2f(tx1, ty1);		glVertex2f(x1, y1);
					glTexCoord2f(tx1, ty2);		glVertex2f(x1, y2);
					glTexCoord2f(tx2, ty2);		glVertex2f(x1 + side / 3.0f, y2);
					glTexCoord2f(tx2, ty1);		glVertex2f(x1 + side / 3.0f, y1);
					glEnd();
					v1 = value - v1 * 100;
					int v2 = v1 / 10;
					SetTexturesCoords(tx1, ty1, tx2, ty2, v2);
					glBegin(GL_QUADS);
					glTexCoord2f(tx1, ty1);		glVertex2f(x1 + side / 3.0f, y1);
					glTexCoord2f(tx1, ty2);		glVertex2f(x1 + side / 3.0f, y2);
					glTexCoord2f(tx2, ty2);		glVertex2f(x1 + 2.0f * side / 3.0f, y2);
					glTexCoord2f(tx2, ty1);		glVertex2f(x1 + 2.0f * side / 3.0f, y1);
					glEnd();
					v2 = v1 - v2 * 10;
					SetTexturesCoords(tx1, ty1, tx2, ty2, v2);
					glBegin(GL_QUADS);
					glTexCoord2f(tx1, ty1);		glVertex2f(x1 + 2.0f * side / 3.0f, y1);
					glTexCoord2f(tx1, ty2);		glVertex2f(x1 + 2.0f * side / 3.0f, y2);
					glTexCoord2f(tx2, ty2);		glVertex2f(x2, y2);
					glTexCoord2f(tx2, ty1);		glVertex2f(x2, y1);
					glEnd();
				}
			}
			glPopMatrix();
			glDisable(GL_ALPHA_TEST);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	};

	std::vector<Kube> vec;
public:
	static int sort_speed;
	static bool need_to_restart;
	bool is_sorting = false;
	Spysok()
	{
		koef = side = 0;
		number = 0;
	}
	Spysok(int n)
	{
		number = n;
		side = float(width) / (number + 1);
		vec.resize(number);
		koef = 1.0f + 1.0f / (number - 1.0f);
		for (int i = 0; i < number; i++)
		{
			glPushMatrix();
			float k = koef * side * i;
			glTranslatef(k, 0, 0);
			vec[i].x1 += k;
			vec[i].x2 += k;
			int v = rand() % 2000 - 1000;
			vec[i].value = v == -1000 ? v + 1 : v;
			glPopMatrix();
		}
	}

	void swap(Kube& s1, Kube& s2)
	{
		float temp = s1.y1 + side;
		float step = 5;
		Kube s3 = s1;
		Kube s4 = s2;
		while (s1.y1 < temp)
		{
			if (need_to_restart) throw "Restart";
			s1.y1 += step;
			s1.y2 += step;
			s2.y1 -= step;
			s2.y2 -= step;
			this_thread::sleep_for(chrono::milliseconds(10/sort_speed));
		}
		temp = s1.x1 + side * koef;
		while (s1.x1 < temp)
		{
			if (need_to_restart) throw "Restart";
			s1.x1 += step;
			s1.x2 += step;
			s2.x1 -= step;
			s2.x2 -= step;
			this_thread::sleep_for(chrono::milliseconds(10/sort_speed));
		}
		temp = s1.y1 - side;
		while (s1.y1 > temp)
		{
			if (need_to_restart) throw "Restart";
			s1.y1 -= step;
			s1.y2 -= step;
			s2.y1 += step;
			s2.y2 += step;
			this_thread::sleep_for(chrono::milliseconds(10/sort_speed));
		}

		s2.x1 = s3.x1;
		s2.x2 = s3.x2;
		s2.y1 = s3.y1;
		s2.y2 = s3.y2;

		s1.x1 = s4.x1;
		s1.x2 = s4.x2;
		s1.y1 = s4.y1;
		s1.y2 = s4.y2;

		s3 = s1;
		s1 = s2;
		s2 = s3;
	}
	void sleep(float seconds)
	{
		float delay = seconds * CLOCKS_PER_SEC;
		clock_t start = clock();
		while (clock() - start < delay)
		{
			if (need_to_restart) throw "Restart";
		}
	}
	void SortBubble(int first, int last)
	{
		need_to_restart = false;
		try
		{
			sleep(1);
			int last1 = last;
			bool IND;
			for (int i = first; i < last; i++)
			{
				IND = false; //≥ндикатор, €кий показуЇ чи в≥дбулось сортуванн€
				for (int j = first; j < last1; j++)
				{
					vec[j].SetColor(150, 150, 150);
					vec[j + 1].SetColor(150, 150, 150);
					sleep(0.4 / sort_speed);
					if (vec[j].value > vec[j + 1].value)
					{
						vec[j].SetColor(0, 150, 0);
						vec[j + 1].SetColor(0, 150, 0);
						sleep(0.4 / sort_speed);
						swap(vec[j], vec[j + 1]);
						sleep(0.02 / sort_speed);
						vec[j].SetColor(30, 0, 180);
						vec[j + 1].SetColor(30, 0, 180);
						IND = true;
					}
					else
					{
						vec[j].SetColor(150, 0, 0);
						vec[j + 1].SetColor(150, 0, 0);
						sleep(0.4 / sort_speed);
						vec[j].SetColor(30, 0, 180);
						vec[j + 1].SetColor(30, 0, 180);
					}
				}
				if (IND == false) //якщо сортуванн€ не в≥дбулось то виходимо з циклу
				{
					for (int i = 0; i <= last1; i++)
						vec[i].SetColor(30, 30, 30);
					break;
				}
				sleep(0.4 / sort_speed);
				vec[last1].SetColor(30, 30, 30);
				last1--; //«меншенн€ нев≥дсортованоњ частини на 1
			}
			vec[0].SetColor(30, 30, 30);
		}
		catch (...)
		{
			return;
		}

	}
	void DrawAll()
	{
		for (auto& el : vec)
			el.Draw();
	}

};

class Button
{
	GLubyte color[3];
	float x1 = 0, y1 = 0, x2 = 0, y2 = 0, b_width = 0, b_height = 0;
	friend void MouseMove(int x, int y);
	friend void Init();
public:
	enum class RegimeButton { PLUS, MINUS, START, EXIT, RESTART };
	RegimeButton regime;
	bool is_under_mouse = false;
	bool clicked = false;
	Button()
	{
		SetColor(0, 0, 0);
		regime = RegimeButton::START;
	}
	Button(RegimeButton nregime, float nx1, float ny1, float nx2, float ny2) : x1(nx1), y1(ny1), x2(nx2), y2(ny2)
	{
		SetColor(0, 0, 0);
		regime = nregime;
		b_width = x2 - x1;
		b_height = y1 - y2;
	}
	void SetColor(GLubyte r, GLubyte g, GLubyte b)
	{
		color[0] = r;
		color[1] = g;
		color[2] = b;
	}
	void Draw()
	{
		glPushMatrix();

		glTranslatef(0, 0, -2 * small);
		glColor3ubv(color);
		glRectf(x1, y1, x2, y2);
		switch (regime)
		{
		case RegimeButton::START :
			glBindTexture(GL_TEXTURE_2D, tex_start);
			glColor3f(1, 1, 1);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.9f);
			glTranslatef(0, 0, small);
			glBegin(GL_QUADS);
			glTexCoord2f(0, 0);	glVertex2f(x1, y1);
			glTexCoord2f(0, 1);	glVertex2f(x1, y2);
			glTexCoord2f(1, 1);	glVertex2f(x2, y2);
			glTexCoord2f(1, 0);	glVertex2f(x2, y1);
			glEnd();
			glDisable(GL_ALPHA_TEST);
			glBindTexture(GL_TEXTURE_2D, 0);
			break;
			
		case RegimeButton::RESTART :
			glBindTexture(GL_TEXTURE_2D, tex_restart);
			glColor3f(1, 1, 1);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.9f);
			glTranslatef(0, 0, small);
			glBegin(GL_QUADS);
			glTexCoord2f(0, 0);	glVertex2f(x1, y1);
			glTexCoord2f(0, 1);	glVertex2f(x1, y2);
			glTexCoord2f(1, 1);	glVertex2f(x2, y2);
			glTexCoord2f(1, 0);	glVertex2f(x2, y1);
			glEnd();
			glDisable(GL_ALPHA_TEST);
			glBindTexture(GL_TEXTURE_2D, 0);
			break;

		case RegimeButton::EXIT :
			glLineWidth(5);
			glColor3ub(0, 0, 0);
			glTranslatef(0, 0, small);
			glBegin(GL_LINES);
			glVertex2f(x1, y1);
			glVertex2f(x2, y2);
			glVertex2f(x1, y2);
			glVertex2f(x2, y1);
			glEnd();
			break;

		case RegimeButton::MINUS :
			glColor3ub(0, 0, 200);
			glTranslatef(0, 0, small);
			glRectf(x1 + 10, y1 - b_height / 2.0f + 7, x2 - 10, y1 - b_height / 2.0f - 7);
			break;

		case RegimeButton::PLUS :
			glColor3ub(0, 0, 200);
			glTranslatef(0, 0, small);
			glRectf(x1 + 10, y1 - b_height / 2.0f + 7, x2 - 10, y1 - b_height / 2.0f - 7);
			glRectf(x1 + b_width / 2.0f - 10, y1 - 5, x1 + b_width / 2.0f + 10, y2 + 5);
			break;
		}
		glPopMatrix();
	}
};

class Tablo
{
	float x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	float side = 0;
public:
	Tablo() { x1 = y1 = x2 = y2 = side = 0; };
	Tablo(float nx1, float ny1, float nx2, float ny2) : x1(nx1), y1(ny1), x2(nx2), y2(ny2)
	{
		side = x2 - x1;
	}
	void Draw(int n)
	{
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, tex_numbers);
		float tx1, ty1, tx2, ty2;
		glColor3f(1, 1, 1);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.9f);
		if (n / 10 == 0)
		{
			SetTexturesCoords(tx1, ty1, tx2, ty2, n);
			glBegin(GL_QUADS);
			glTexCoord2f(tx1, ty1);		glVertex2f(x1, y1);
			glTexCoord2f(tx1, ty2);		glVertex2f(x1, y2);
			glTexCoord2f(tx2, ty2);		glVertex2f(x2, y2);
			glTexCoord2f(tx2, ty1);		glVertex2f(x2, y1);
			glEnd();
		}
		else
		{
			int n1 = n / 10;
			SetTexturesCoords(tx1, ty1, tx2, ty2, n1);
			glBegin(GL_QUADS);
			glTexCoord2f(tx1, ty1);		glVertex2f(x1, y1);
			glTexCoord2f(tx1, ty2);		glVertex2f(x1, y2);
			glTexCoord2f(tx2, ty2);		glVertex2f(x1 + side / 2, y2);
			glTexCoord2f(tx2, ty1);		glVertex2f(x1 + side / 2, y1);
			glEnd();
			SetTexturesCoords(tx1, ty1, tx2, ty2, n - n1 * 10);
			glBegin(GL_QUADS);
			glTexCoord2f(tx1, ty1);		glVertex2f(x1 + side / 2, y1);
			glTexCoord2f(tx1, ty2);		glVertex2f(x1 + side / 2, y2);
			glTexCoord2f(tx2, ty2);		glVertex2f(x2, y2);
			glTexCoord2f(tx2, ty1);		glVertex2f(x2, y1);
			glEnd();
		}
		glDisable(GL_ALPHA_TEST);
		glBindTexture(GL_TEXTURE_2D, 0);
		glPopMatrix();
	}
};

int Spysok::sort_speed = 1;
float Spysok::side = 0.0;
bool Spysok::need_to_restart = false;
int n = 5;
Spysok spysok;
Button b_plus, b_minus, b_start, b_exit, b_restart;
Tablo tablo;

void Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (!b_start.clicked)
	{
		b_plus.Draw();
		b_minus.Draw();
		b_start.Draw();
		tablo.Draw(n);
	}
	if (b_start.clicked)
	{
		b_restart.Draw();
		spysok.DrawAll();
	}
	b_exit.Draw();
	glFlush();
}

void Idle()
{
	glutPostRedisplay();
	if (!spysok.is_sorting && b_start.clicked)
	{
		spysok.is_sorting = true;
		thread th([&]() {spysok.SortBubble(0, n - 1); });
		th.detach();
	}
}

void Init()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
	glFrustum(0, width, -height / 2.0, height / 2.0, 2, edge);
	glTranslatef(0, 0, -2);
	LoadTexture("numbers.png", tex_numbers);
	LoadTexture("restart.png", tex_restart);
	LoadTexture("start.png", tex_start);
	glEnable(GL_TEXTURE_2D);

	b_plus = Button(Button::RegimeButton::PLUS, width / 2.f - 180, -height / 2.f + 70, width / 2.f - 80, -height / 2.f + 20);
	b_minus = Button(Button::RegimeButton::MINUS, b_plus.x2 + 30, b_plus.y1, b_plus.x2 + 130, b_plus.y2);
	b_start = Button(Button::RegimeButton::START, b_minus.x2 + 30, b_plus.y1, b_minus.x2 + 130, b_plus.y2);
	b_exit = Button(Button::RegimeButton::EXIT, width - 30, height/2+1, width+1, height/2 - 30);
	b_restart = Button(Button::RegimeButton::RESTART, -1, height / 2 + 1, 30, height / 2 - 30);
	tablo = Tablo(width / 2.f - 150, 150, width / 2.f + 150, -150);
}

void special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_DOWN:
		if (spysok.sort_speed > 1)
			spysok.sort_speed -= 1;
		break;
	case GLUT_KEY_UP:
		if(spysok.sort_speed<10)
		spysok.sort_speed += 1;
		break;
	}
}

void MouseMove(int x, int y)
{
	if (!b_start.clicked)
	{
		if (x > b_plus.x1 && x<b_plus.x2 && -y + height / 2.0>b_plus.y2 && -y + height / 2.0 < b_plus.y1)
		{
			b_plus.SetColor(200, 0, 0);
			b_plus.is_under_mouse = true;
		}
		else
		{
			b_plus.SetColor(210, 210, 12);
			b_plus.is_under_mouse = false;
		}
		if (x > b_minus.x1 && x<b_minus.x2 && -y + height / 2.0>b_minus.y2 && -y + height / 2.0 < b_minus.y1)
		{
			b_minus.SetColor(200, 0, 0);
			b_minus.is_under_mouse = true;
		}
		else
		{
			b_minus.SetColor(210, 210, 12);
			b_minus.is_under_mouse = false;
		}
		if (x > b_start.x1 && x<b_start.x2 && -y + height / 2.0>b_start.y2 && -y + height / 2.0 < b_start.y1)
		{
			b_start.SetColor(200, 0, 0);
			b_start.is_under_mouse = true;
		}
		else
		{
			b_start.SetColor(0, 180, 0);
			b_start.is_under_mouse = false;
		}
	}
	else
	{
		if (x > b_restart.x1 && x<b_restart.x2 && -y + height / 2.0>b_restart.y2 && -y + height / 2.0 < b_restart.y1)
		{
			b_restart.SetColor(200, 0, 0);
			b_restart.is_under_mouse = true;
		}
		else
		{
			b_restart.SetColor(0, 0, 200);
			b_restart.is_under_mouse = false;
		}
	}
	if (x > b_exit.x1 && x<b_exit.x2 && -y + height / 2.0>b_exit.y2 && -y + height / 2.0 < b_exit.y1)
	{
		b_exit.SetColor(200, 0, 0);
		b_exit.is_under_mouse = true;
	}
	else
	{
		b_exit.SetColor(210, 210, 12);
		b_exit.is_under_mouse = false;
	}
}

void MouseClick(int button, int state, int x, int y)
{
	if (!b_start.clicked)
	{
		if (b_plus.is_under_mouse && state == GLUT_DOWN)
		{
			if (n < 20) ++n;
		}
		else if (b_minus.is_under_mouse && state == GLUT_DOWN)
		{
			if (n > 2) --n;
		}
		else if (b_start.is_under_mouse && state == GLUT_DOWN)
		{
			spysok = Spysok(n);
			b_start.clicked = true;
		}
	}
	else
	{
		if (b_restart.is_under_mouse && state == GLUT_DOWN)
		{
			Spysok::need_to_restart = true;
			this_thread::sleep_for(20ms);
			Spysok::sort_speed = 1;
			b_start.clicked = false;
			
		}
	}
	if (b_exit.is_under_mouse && state == GLUT_DOWN)
	{
		glutExit();
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	srand(time(0));
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	glutInitWindowPosition(0, 0);
	width = glutGet(GLUT_SCREEN_WIDTH);
	height = glutGet(GLUT_SCREEN_HEIGHT);
	glutInitWindowSize(width, height);
	glutCreateWindow("Kursova");
	glutFullScreen();
	glutSpecialFunc(special);
	glutDisplayFunc(Draw);
	glutIdleFunc(Idle);
	glutPassiveMotionFunc(MouseMove);
	glutMouseFunc(MouseClick);
	Init();
	glutMainLoop();
	return 0;
}