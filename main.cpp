/*************************************************************************************/
//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacj¹ osi  
//  uk³adu wspó³rzêdnych dla rzutowania perspektywicznego
/*************************************************************************************/
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <math.h>
#include <iostream>

typedef float point3[3];
static GLfloat viewer[] = { 0.0, 0.0, 3.0 };

static GLfloat azymut = 0.0;   // Azymut
static GLfloat elewacja = 0.0;   // K¹t elewacji
float R = 3.0, pi = 3.14159, centerY = 0.0;	// Promieñ sfery, liczba pi,
											// oraz wspó³rzêdne Y punktu na który patrzy obserwator


static GLfloat pix2angle;     // przelicznik pikseli na stopnie
static GLfloat pix2angle1;     // przelicznik pikseli na stopnie


static GLint status = 0;       // stan klawiszy myszy 
							   // 0 - nie naciœniêto ¿adnego klawisza
							   // 1 - naciœniêty zostaæ lewy klawisz
static GLint status1 = 0;       // stan klawiszy myszy (prawy klawisz)


static int x_pos_old = 0;       // poprzednia pozycja kursora myszy

static int delta_x = 0;        // ró¿nica pomiêdzy pozycj¹ bie¿¹c¹
							   // i poprzedni¹ kursora myszy 

static int y_pos_old = 0;       // poprzednia pozycja kursora myszy

static int delta_y = 0;        // ró¿nica pomiêdzy pozycj¹ bie¿¹c¹
							   // i poprzedni¹ kursora myszy 

float width = 0.0, height = 0.0;
GLfloat ratio;


using namespace std;

//Zmienne potrzebne do tworzenia obiektu trójk¹ta Sierpiñskiego 3D
typedef float point3[3];
int iteracje = 0;

/*************************************************************************************/
// Funkcja rysuj¹ca piramidê

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
		// Obliczenie wspó³rzêdnych 5 nowych piramid 

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



// inicjalizacja po³o¿enia obserwatora
/*************************************************************************************/


/*************************************************************************************/
// Funkcja "bada" stan myszy i ustawia wartoœci odpowiednich zmiennych globalnych

void Mouse(int btn, int state, int x, int y)
{


	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;         // przypisanie aktualnie odczytanej pozycji kursora 
							   // jako pozycji poprzedniej

		y_pos_old = y;         // przypisanie aktualnie odczytanej pozycji kursora 
							   // jako pozycji poprzedniej
		status = 1;          // wciêniêty zosta³ lewy klawisz myszy
	}
	else

		status = 0;          // nie zosta³ wciêniêty ¿aden klawisz 

	if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{

		y_pos_old = y;         // przypisanie aktualnie odczytanej pozycji kursora 
							   // jako pozycji poprzedniej
		status1 = 1;          // wciêniêty zosta³ lewy klawisz myszy
	}
	else

		status1 = 0;          // nie zosta³ wciêniêty ¿aden klawisz 
}

/*************************************************************************************/
// Funkcja "monitoruje" po³o¿enie kursora myszy i ustawia wartoœci odpowiednich 
// zmiennych globalnych

void Motion(GLsizei x, GLsizei y)
{

	delta_x = x - x_pos_old;     // obliczenie ró¿nicy po³o¿enia kursora myszy

	x_pos_old = x;            // podstawienie bie¿¹cego po³o¿enia jako poprzednie

	delta_y = y - y_pos_old;     // obliczenie ró¿nicy po³o¿enia kursora myszy

	y_pos_old = y;            // podstawienie bie¿¹cego po³o¿enia jako poprzednie

	glutPostRedisplay();     // przerysowanie obrazu sceny
}

/*************************************************************************************/

// Funkcja rysuj¹ca osie uk³adu wspó?rz?dnych

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
// Funkcja okreœlaj¹ca co ma byæ rysowane (zawsze wywo³ywana, gdy trzeba 
// przerysowaæ scenê)

void RenderScene(void)
{
	point3 pd = { 5.0, -((10 * sqrt(3) / 4)), -5.0 };
	point3 ld = { -5.0, -((10 * sqrt(3) / 4)), -5.0 };

	float bok = pd[0] - ld[0];
	float bok_maly = bok / 2;
	double wysokosc = ((bok * sqrt(3) / 2));

	// Zmienne potrzebne do obliczenia maxymaslego promienia sfery, po której porusza siê obsertwator
	// Maksymalny promieñ tej sfery zmienia siê w zale¿noœci od po³o¿enia obserwatora
	double nowa_wysokosc = wysokosc - (viewer[1] - ld[1]);
	double max_r = nowa_wysokosc / 2;

	point3 g = { 0.0, ld[1] + wysokosc, bok_maly-5.0 };

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz¹cym

	glLoadIdentity();
	// Czyszczenie macierzy bie??cej

	viewer[0] = R*cos(azymut)*cos(elewacja);
	viewer[1] = R*sin(elewacja);
	viewer[2] = R*sin(azymut)*cos(elewacja);

	centerY = viewer[1];

	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, sin(centerY), 0.0, 0.0, cos(elewacja), 0.0);
	// Zdefiniowanie po³o¿enia obserwatora
	Axes();
	// Narysowanie osi przy pomocy funkcji zdefiniowanej powy¿ej


	if (status == 1)                     // jeœli lewy klawisz myszy wciêniêty
	{
		azymut += delta_x*pix2angle / 40;    // modyfikacja k¹ta obrotu o kat proporcjonalny do ró¿nicy po³o¿eñ kursora myszy

		elewacja += delta_y*pix2angle1 / 40;

	}

	if (status1 == 1)                     // jeœli prawy klawisz myszy wciêniêty
	{
		if ( R + delta_y <= max_r && R + delta_y > 0.0 )
			R += delta_y;
	}

	if (R > max_r)
		R = max_r;


	piramida(ld, pd, g, iteracje);
	// Narysowanie piramid
	glFlush();
	// Przekazanie poleceñ rysuj¹cych do wykonania
	glutSwapBuffers();

}
/*************************************************************************************/
// Funkcja ustalaj¹ca stan renderowania

void keys(unsigned char key, int x, int y)
{
	// Zwiêkszenie iteracji piramidy
	if (key == 'i') iteracje++;

	// Zmniejszenie iteracji piramidy
	if (key == 'o') iteracje--;

	RenderScene(); // przerysowanie obrazu sceny
}

void MyInit(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Kolor czyszcz¹cy (wype³nienia okna) ustawiono na czarny
}
/*************************************************************************************/

// Funkcja ma za zadanie utrzymanie sta³ych proporcji rysowanych 
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokoœæ i szerokoœæ okna) s¹ 
// przekazywane do funkcji za ka¿dym razem gdy zmieni siê rozmiar okna.

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{

	width = horizontal;
	height = vertical;

	ratio = width / height;


	pix2angle = 360.0 / (float)horizontal;  // przeliczenie pikseli na stopnie
	pix2angle1 = 360.0 / (float)vertical;  // przeliczenie pikseli na stopnie


	glMatrixMode(GL_PROJECTION);
	// Prze³¹czenie macierzy bie¿¹cej na macierz projekcji

	glLoadIdentity();
	// Czyszcznie macierzy bie¿¹cej 

	gluPerspective(70, ratio, 1.0, 30.0);
	// Ustawienie parametrów dla rzutu perspektywicznego


	glViewport(0, 0, width, height);
	// Ustawienie wielkoœci okna okna widoku (viewport) w zale¿noœci
	// relacji pomiêdzy wysokoœci¹ i szerokoœci¹ okna

	glMatrixMode(GL_MODELVIEW);
	// Prze³¹czenie macierzy bie¿¹cej na macierz widoku modelu  

	glLoadIdentity();
	// Czyszczenie macierzy bie¿¹cej 

}
/*************************************************************************************/
// G³ówny punkt wejœcia programu. Program dzia³a w trybie konsoli

void main(void)
{

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(300, 300);

	glutCreateWindow("Trójk¹t Sierpiñskiego 3D");

	glutDisplayFunc(RenderScene);
	// Okreœlenie, ¿e funkcja RenderScene bêdzie funkcj¹ zwrotn¹
	// (callback function).  Bêdzie ona wywo³ywana za ka¿dym razem 
	// gdy zajdzie potrzeba przerysowania okna

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcjê zwrotn¹ odpowiedzialn¹
	// za zmiany rozmiaru okna  

	glutKeyboardFunc(keys);

	glutMouseFunc(Mouse);
	// Ustala funkcjê zwrotn¹ odpowiedzialn¹ za badanie stanu myszy

	glutMotionFunc(Motion);
	// Ustala funkcjê zwrotn¹ odpowiedzialn¹ za badanie ruchu myszy


	MyInit();
	// Funkcja MyInit() (zdefiniowana powy¿ej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przyst¹pieniem do renderowania
	glEnable(GL_DEPTH_TEST);
	// W³¹czenie mechanizmu usuwania niewidocznych elementów sceny

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT
}
/*************************************************************************************/