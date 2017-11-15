# Cómo aprender OpenGL y sobrevivir en el intento

Intentaré mantener esta guía lo más personalizada posible, esto es, si algo ya lo sé bien, no lo voy a explicar. Sorry, tengo que aprobar.

Hasta donde he visto, puedo separar OpenGL en ciertos temas principales, que tal vez vayan aumentando, lo cual es probable:

1. Librerías
* OpenGL
* GLU
* GLUT
1. Pipeline
1. GLUT Workflow
1. OpenGL Workflow
 * Proyecciones
 * Primitivas
1. Glut y la maldición/bendición de los frameworks


## Librerias

Aunque en teoría solo hay una librería, OpenGL.h, en la práctica es tan pero tan bajo nivel que tenemos que meterle dos librerías más para hacer algo medianamente útil sin gastarnos una hora tipeando otras cosas.

### OpenGL

Ah, la librería estrella. No voy a repasar los conceptos de algebra líneal aquí, pero lo que he notado es que acá están métodos fundamentales que involucran a la matriz (su translate, rotate, push, pop, matrixmode, loadidentity, frustum, etc), las líneas o poligonos y el limpiar la pantalla, los colores, las luces, aparte de mil y un tipos de datos como GLint o cosas así. Las funciones siempre empiezan con "gl", así que no es muy difícil de identificarlas. Lo que sí, el bajo nivel de este API vuelve condenadamente dificil hacer cosas sin escribir siete mil líneas, y por eso esta...

### GLU

Realmente son pocas las cosas que veo con GLU, pero tal vez sea porque GLUT ya las incluye. En teoría GLU incluye funciones que sirven para reutilizar el código, o simplificarlo, como el gluPerspective que en realidad me parece más complicado que el glFrustum, pero whatever.

### GLUT

La verdadera librería estrella. En ningún momento mencioné en las anteriores cómo programar las ventanas y esas cosas. Eso es porque ni a OpenGl ni a GLU le interesa lidiar con eso. ¿Y cómo es que puedes ver lo que programas si no hay un ventana donde verla? ¿Y por qué lo hicieron así? Lo primero, esa chamba de la dejan a ti. Lo segundo, es porque supuestamente OpenGL es un estándar, lo que significa que tiene que funcionar en Windows, en Mac o en Linux, y ahí viene el problema, esos sistemas operativos tienen su propia forma de crear ventanas y eso, así que OpenGL no se mete problemas.

Aquí viene GLUT. GLUT se encarga de construir una librería donde puedas abrir ventanas, manejar interacciones con el teclado/mouse y en general crear un... ¿framework? La cosa es que crea un workflow, o una secuencia de cómo hacer las cosas, como para ordenar y estandarizar el código. Aparte usa lambas para ciertas cosas y la verdad es que los lambdas te arreglan la vida cuando los entiendes.

Por cierto, GLUT también tiene funciones para crear figuras básicas de frente, sin tener que usar los mil vertex, lo cual es cool. Cool, cool, cool. Estas formas, de las que he visto, pueden ser sólidas o en wireframe (que suena más feo de lo que parece, la verdad):

<pre>
	// Por cierto, no tengo idea de qué significan esos parámetros
	glutSolidSphere(1, slices, stacks);
	glutWireSphere(1, slices, stacks);
	
	glutSolidCone(1, 1, slices, stacks);
	glutWireCone(1, 1, slices, stacks);
	
	glutSolidTorus(0.2, 0.8, slices, stacks);
	glutWireTorus(0.2, 0.8, slices, stacks);

	glutSolidTeapot(0.8f);
	glutWireTeapot(0.8f);
	
	glutSwapBuffers(); // Importante
</pre>

¡Documentación en [2]!

<<All GLUT functions start with the glut prefix (for example, glutPostRedisplay marks the current window as needing to be redrawn).>>

## Pipeline

Uy, no, aquí no. Mucho GPU y el bajo nivel de OpenGl. Hasta donde veo, OpenGL en realidad solo se encarga de hacer operaciones matemáticas, y la vaina de mostrar los resultados en la pantalla ya corre por tu cuenta. Pero todos esos cálculos siguen una secuencia muy definida... que al final con los shaders parece que ya ni tanto. Pero no llevamos shaders aquí, wuju, así que estamos salvados. Por ahora. 

## GLUT Workflow

Cuando empiezas a notar el bajo nivel de OpenGL, también empiezas a notar que programar algo se vuelve una pesadilla. ¿Y cómo se vería el main? ¿Tengo que cranearla siempre para crear mis funciones? ¿Cómo hago el loop? ¿Y tengo que usar el _getch() como en progra uno? Dios, los switch, aparta de mí este cáliz...

O al menos así me pondría si supiese que solo puedo usar OpenGL.h, por suerte GLUT ya tiene una forma relativamente fija de trabajo, que al mismo tiempo es increíblemente flexible. Uno de los ejemplos más simples es este:

<pre>
int main() {
	//Step 1: Initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);

	//Step2: Create Windows
	glutCreateWindow("Holaa");
	initRendering();

	//Step3: Handlers functions
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(key);
	glutIdleFunc(idle);

	//Step4: Main Loop
	glutMainLoop();

	return EXIT_SUCCESS;
}
</pre>





## Glut y la maldición/bendición de los frameworks

Siempre he tenido curiosidad por saber cómo se crea una biblioteca como GLUT o el SpringFramework. O sea, es cómo si tuviesen ya todo hecho detrás, pero el cómo crean la arquitectura sin que todo se vuelva un desastre es algo que me da curiosidad. Eso sí, te obliga a trabajar siempre de una forma predeterminada, pero si eso te ayuda, bienvenido sea.

# Links

[1] https://www.opengl.org/resources/libraries/glut/
 
[2] https://www.opengl.org/resources/libraries/glut/glut-3.spec.pdf
