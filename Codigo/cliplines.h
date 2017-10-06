#define outcode int

const int INSIDE = 0;
const int LEFT = 1;
const int RIGHT = 2;
const int BOTTOM = 4;
const int TOP = 8;

const int xmin = 5;
const int xmax = 19;

const int ymin = 6;
const int ymax = 17;


int outcode0, outcode1;

void dibujar_linea(int x, int y, int x1, int y1) {
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(x, y, 0);
	glVertex3f(x1, y1, 0);
	glEnd();
}

int compute(double x, double y) {
	int code;
	code = INSIDE;

	if (x < xmin)
		code |= LEFT;
	else if (x>xmax)
		code |= RIGHT;

	if (y < ymin)
		code |= BOTTOM;
	else if (y>ymax)
		code |= TOP;

	return code;
}

void  CohenSutherlandLineClipAndDraw(double x0, double y0, double x1, double y1) {
	outcode0 = compute(x0, y0);
	outcode1 = compute(x1, y1);

	bool accept = false;

	while (true) {
		if (!(outcode0 | outcode1)) {
			accept = true;
			break;
		}
		else if (outcode0 & outcode1)
			break;
		else {
			double x, y;
			int outcodeOut = outcode0 ? outcode0 : outcode1;

			if (outcodeOut  & TOP) {
				x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
				y = ymax;
			}
			else if (outcodeOut  & BOTTOM) {
				x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
				y = ymin;
			}
			else if (outcodeOut & RIGHT) {
				y = y0 + (y1 - y0)*(xmax - x0) / (x1 - x0);
				x = xmax;
			}
			else if (outcodeOut & LEFT) {
				y = y0 + (y1 - y0)*(xmin - x0) / (x1 - x0);
				x = xmin;
			}

			if (outcodeOut == outcode0) {
				x0 = x;
				y0 = y;
				outcode0 = compute(x0, y0);
			}
			else {
				x1 = x;
				y1 = y;
				outcode1 = compute(x1, y1);
			}
		}
	}
	//
	if (accept) {
		//dibujar_linea(x0, y0, x1, y1);
		glColor3f(0.0, 0.0, 1.0);
		glBegin(GL_LINES);
		glVertex2f(x0, y0);
		glVertex2f(x1, y1);
		glEnd();

	}
}