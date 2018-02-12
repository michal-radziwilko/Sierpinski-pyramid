/*************************************************************************************/
//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacj� osi  
//  uk�adu wsp�rz�dnych dla rzutowania perspektywicznego
/*************************************************************************************/
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <math.h>
#include <iostream>

typedef float point3[3];
static GLfloat viewer[] = { 0.0, 0.0, 3.0 };

static GLfloat azymut = 0.0;   // Azymut
static GLfloat elewacja = 0.0;   // K�t elewacji
float R = 3.0, pi = 3.14159, centerY = 0.0;	// Promie� sfery, liczba pi,
											// oraz wsp�rz�dne Y punktu na kt�ry patrzy obserwator


static GLfloat pix2angle;     // przelicznik pikseli na stopnie
static GLfloat pix2angle1;     // przelicznik pikseli na stopnie


static GLint status = 0;       // stan klawiszy myszy 
							   // 0 - nie naci�ni�to �adnego klawisza
							   // 1 - naci�ni�ty zosta� lewy klawisz
static GLint status1 = 0;       // stan klawiszy myszy (prawy klawisz)


static int x_pos_old = 0;       // poprzednia pozycja kursora myszy

static int delta_x = 0;        // r�nica pomi�dzy pozycj� bie��c�
							   // i poprzedni� kursora myszy 

static int y_pos_old = 0;       // poprzednia pozycja kursora myszy

static int delta_y = 0;        // r�nica pomi�dzy pozycj� bie��c�
							   // i poprzedni� kursora myszy 

float width = 0.0, height = 0.0;
GLfloat ratio;


using namespace std;

//Zmienne potrzebne do tworzenia obiektu tr�jk�ta Sierpi�skiego 3D
typedef float point3[3];
int iteracje = 0;

/*************************************************************************************/
// Funkcja rysuj�ca piramid�

void piramida(point3 ld, point3 pd, point3 g, int licznik)
{
	float bok = pd[0] - ld[0];
	float bok_maly = bok / 2;
	double wysokosc = ((bok * sqrt(3) / 2));

	point3 ld1 = { ld[0], ld[1], ld[2] + bok };
	point3 pd1 = { pd[0], pd[1], ld[2] + bok };


	if (licznik == 1)
	{

		glBegin(GL_TRIANGLES);

		glColor3f(1.0f, 0.0f, 0.0f);  // kolor rysowania - czerwony
		glVertex3fv(ld);
		glVertex3fv(pd);
		glVertex3fv(g);

		glColor3f(0.0f, 0.0f, 1.0f);  // kolor rysowania - niebieski
		glVertex3fv(ld1);
		glVertex3fv(ld);
		glVertex3fv(g);

		glColor3f(0.0f, 0.0f, 1.0f);  // kolor rysowania - niebieski
		glVertex3fv(pd);
		glVertex3fv(pd1);
		glVertex3fv(g);

		glColor3f(1.0f, 0.0f, 0.0f);  // kolor rysowania - czerwony
		glVertex3fv(pd1);
		glVertex3fv(ld1);
		glVertex3fv(g);



		glEnd();

		glBegin(GL_POLYGON);

		glColor3f(0.5f, 0.5f, 0.5f);  // kolor rysowania - szary
		glVertex3fv(pd1);
		glVertex3fv(ld1);
		glVertex3fv(ld);
		glVertex3fv(pd);

		glEnd();

	}

	else if (licznik > 1)
	{
		// Obliczenie wsp�rz�dnych 5 nowych piramid 

		point3 ld_new = { ld[0], ld[1], ld[2] };
		point3 pd_new = { pd[0] - bok_maly, pd[1], pd[2] };
		point3 g_new = { ld[0] + bok_maly / 2, ld[1] + wysokosc / 2, ld[2] + bok_maly / 2 };

		point3 ld_new1 = { ld[0] + bok_maly, ld[1], ld[2] };
		point3 pd_new1 = { pd[0], pd[1], pd[2] };
		point3 g_new1 = { pd[0] - bok_maly / 2, pd[1] + wysokosc / 2, pd[2] + bok_maly / 2 };

		point3 ld_new2 = { ld[0] + bok_maly / 2, ld[1] + wysokosc / 2, ld[2] + bok_maly / 2 };
		point3 pd_new2 = { pd[0] - bok_maly / 2, pd[1] + wysokosc / 2, pd[2] + bok_maly / 2 };
		point3 g_new2 = { g[0], g[1], g[2] };

		point3 ld_new3 = { ld[0] + bok_maly, ld[1], ld[2] + bok_maly };
		point3 pd_new3 = { pd[0], pd[1], pd[2] + bok_maly };
		point3 g_new3 = { pd[0] - bok_maly / 2, pd[1] + wysokosc / 2, pd[2] + bok - (bok_maly / 2) };

		point3 ld_new4 = { ld[0], ld[1], pd[2] + bok_maly };
		point3 pd_new4 = { ld[0] + bok_maly, ld[1], ld[2] + bok_maly };
		point3 g_new4 = { ld[0] + bok_maly / 2, ld[1] + wysokosc / 2, ld[2] + bok - (bok_maly / 2) };

		piramida(ld_new, pd_new, g_new, licznik - 1);
		piramida(ld_new1, pd_new1, g_new1, licznik - 1);
		piramida(ld_new2, pd_new2, g_new2, licznik - 1);
		piramida(ld_new3, pd_new3, g_new3, licznik - 1);
		piramida(ld_new4, pd_new4, g_new4, licznik - 1);

	}

}



// inicjalizacja po�o�enia obserwatora
/*************************************************************************************/


/*************************************************************************************/
// Funkcja "bada" stan myszy i ustawia warto�ci odpowiednich zmiennych globalnych

void Mouse(int btn, int state, int x, int y)
{


	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;         // przypisanie aktualnie odczytanej pozycji kursora 
							   // jako pozycji poprzedniej

		y_pos_old = y;         // przypisanie aktualnie odczytanej pozycji kursora 
							   // jako pozycji poprzedniej
		status = 1;          // wci�ni�ty zosta� lewy klawisz myszy
	}
	else

		status = 0;          // nie zosta� wci�ni�ty �aden klawisz 

	if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{

		y_pos_old = y;         // przypisanie aktualnie odczytanej pozycji kursora 
							   // jako pozycji poprzedniej
		status1 = 1;          // wci�ni�ty zosta� lewy klawisz myszy
	}
	else

		status1 = 0;          // nie zosta� wci�ni�ty �aden klawisz 
}

/*************************************************************************************/
// Funkcja "monitoruje" po�o�enie kursora myszy i ustawia warto�ci odpowiednich 
// zmiennych globalnych

void Motion(GLsizei x, GLsizei y)
{

	delta_x = x - x_pos_old;     // obliczenie r�nicy po�o�enia kursora myszy

	x_pos_old = x;            // podstawienie bie��cego po�o�enia jako poprzednie

	delta_y = y - y_pos_old;     // obliczenie r�nicy po�o�enia kursora myszy

	y_pos_old = y;            // podstawienie bie��cego po�o�enia jako poprzednie

	glutPostRedisplay();     // przerysowanie obrazu sceny
}

/*************************************************************************************/

// Funkcja rysuj�ca osie uk�adu wsp�?rz?dnych

void Axes(void)
{
	point3  x_min = { -5.0, 0.0, 0.0 };
	point3  x_max = { 5.0, 0.0, 0.0 };
	// pocz?tek i koniec obrazu osi x

	point3  y_min = { 0.0, -5.0, 0.0 };
	point3  y_max = { 0.0,  5.0, 0.0 };
	// pocz?tek i koniec obrazu osi y

	point3  z_min = { 0.0, 0.0, -5.0 };
	point3  z_max = { 0.0, 0.0,  5.0 };
	//  pocz?tek i koniec obrazu osi y
	glColor3f(1.0f, 0.0f, 0.0f);  // kolor rysowania osi - czerwony
	glBegin(GL_LINES); // rysowanie osi x
	glVertex3fv(x_min);
	glVertex3fv(x_max);
	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);  // kolor rysowania - zielony
	glBegin(GL_LINES);  // rysowanie osi y
	glVertex3fv(y_min);
	glVertex3fv(y_max);
	glEnd();

	glColor3f(0.0f, 0.0f, 1.0f);  // kolor rysowania - niebieski
	glBegin(GL_LINES); // rysowanie osi z
	glVertex3fv(z_min);
	glVertex3fv(z_max);
	glEnd();
}
/*************************************************************************************/
// Funkcja okre�laj�ca co ma by� rysowane (zawsze wywo�ywana, gdy trzeba 
// przerysowa� scen�)

void RenderScene(void)
{
	point3 pd = { 5.0, -((10 * sqrt(3) / 4)), -5.0 };
	point3 ld = { -5.0, -((10 * sqrt(3) / 4)), -5.0 };

	float bok = pd[0] - ld[0];
	float bok_maly = bok / 2;
	double wysokosc = ((bok * sqrt(3) / 2));

	// Zmienne potrzebne do obliczenia maxymaslego promienia sfery, po kt�rej porusza si� obsertwator
	// Maksymalny promie� tej sfery zmienia si� w zale�no�ci od po�o�enia obserwatora
	double nowa_wysokosc = wysokosc - (viewer[1] - ld[1]);
	double max_r = nowa_wysokosc / 2;

	point3 g = { 0.0, ld[1] + wysokosc, bok_maly-5.0 };

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz�cym

	glLoadIdentity();
	// Czyszczenie macierzy bie??cej

	viewer[0] = R*cos(azymut)*cos(elewacja);
	viewer[1] = R*sin(elewacja);
	viewer[2] = R*sin(azymut)*cos(elewacja);

	centerY = viewer[1];

	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, sin(centerY), 0.0, 0.0, cos(elewacja), 0.0);
	// Zdefiniowanie po�o�enia obserwatora
	Axes();
	// Narysowanie osi przy pomocy funkcji zdefiniowanej powy�ej


	if (status == 1)                     // je�li lewy klawisz myszy wci�ni�ty
	{
		azymut += delta_x*pix2angle / 40;    // modyfikacja k�ta obrotu o kat proporcjonalny do r�nicy po�o�e� kursora myszy

		elewacja += delta_y*pix2angle1 / 40;

	}

	if (status1 == 1)                     // je�li prawy klawisz myszy wci�ni�ty
	{
		if ( R + delta_y <= max_r && R + delta_y > 0.0 )
			R += delta_y;
	}

	if (R > max_r)
		R = max_r;


	piramida(ld, pd, g, iteracje);
	// Narysowanie piramid
	glFlush();
	// Przekazanie polece� rysuj�cych do wykonania
	glutSwapBuffers();

}
/*************************************************************************************/
// Funkcja ustalaj�ca stan renderowania

void keys(unsigned char key, int x, int y)
{
	// Zwi�kszenie iteracji piramidy
	if (key == 'i') iteracje++;

	// Zmniejszenie iteracji piramidy
	if (key == 'o') iteracje--;

	RenderScene(); // przerysowanie obrazu sceny
}

void MyInit(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Kolor czyszcz�cy (wype�nienia okna) ustawiono na czarny
}
/*************************************************************************************/

// Funkcja ma za zadanie utrzymanie sta�ych proporcji rysowanych 
// w przypadku zmiany rozmiar�w okna.
// Parametry vertical i horizontal (wysoko�� i szeroko�� okna) s� 
// przekazywane do funkcji za ka�dym razem gdy zmieni si� rozmiar okna.

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{

	width = horizontal;
	height = vertical;

	ratio = width / height;


	pix2angle = 360.0 / (float)horizontal;  // przeliczenie pikseli na stopnie
	pix2angle1 = 360.0 / (float)vertical;  // przeliczenie pikseli na stopnie


	glMatrixMode(GL_PROJECTION);
	// Prze��czenie macierzy bie��cej na macierz projekcji

	glLoadIdentity();
	// Czyszcznie macierzy bie��cej 

	gluPerspective(70, ratio, 1.0, 30.0);
	// Ustawienie parametr�w dla rzutu perspektywicznego


	glViewport(0, 0, width, height);
	// Ustawienie wielko�ci okna okna widoku (viewport) w zale�no�ci
	// relacji pomi�dzy wysoko�ci� i szeroko�ci� okna

	glMatrixMode(GL_MODELVIEW);
	// Prze��czenie macierzy bie��cej na macierz widoku modelu  

	glLoadIdentity();
	// Czyszczenie macierzy bie��cej 

}
/*************************************************************************************/
// G��wny punkt wej�cia programu. Program dzia�a w trybie konsoli

void main(void)
{

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(300, 300);

	glutCreateWindow("Tr�jk�t Sierpi�skiego 3D");

	glutDisplayFunc(RenderScene);
	// Okre�lenie, �e funkcja RenderScene b�dzie funkcj� zwrotn�
	// (callback function).  B�dzie ona wywo�ywana za ka�dym razem 
	// gdy zajdzie potrzeba przerysowania okna

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcj� zwrotn� odpowiedzialn�
	// za zmiany rozmiaru okna  

	glutKeyboardFunc(keys);

	glutMouseFunc(Mouse);
	// Ustala funkcj� zwrotn� odpowiedzialn� za badanie stanu myszy

	glutMotionFunc(Motion);
	// Ustala funkcj� zwrotn� odpowiedzialn� za badanie ruchu myszy


	MyInit();
	// Funkcja MyInit() (zdefiniowana powy�ej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przyst�pieniem do renderowania
	glEnable(GL_DEPTH_TEST);
	// W��czenie mechanizmu usuwania niewidocznych element�w sceny

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT
}
/*************************************************************************************/