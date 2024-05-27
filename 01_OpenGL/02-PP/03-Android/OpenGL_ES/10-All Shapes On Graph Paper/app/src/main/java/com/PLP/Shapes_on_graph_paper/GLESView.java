package com.PLP.Shapes_on_graph_paper;

import android.content.Context;

//OpenGL-ES related packages

import android.opengl.GLSurfaceView;

import android.opengl.GLES32;  //OpenGL 3.2 version

//import javax.microedition.khronos.GL10;

import javax.microedition.khronos.opengles.GL10;

import javax.microedition.khronos.egl.EGLConfig;

//event related packages

import android.view.MotionEvent;

import android.view.GestureDetector;

import android.view.GestureDetector.OnDoubleTapListener;

import android.view.GestureDetector.OnGestureListener;

// IO packages

import java.nio.ByteBuffer;

import java.nio.ByteOrder;

import java.nio.FloatBuffer;

import android.opengl.Matrix;

import java.util.Vector;

import java.util.Arrays;

public class GLESView extends GLSurfaceView implements GLSurfaceView.Renderer, OnDoubleTapListener, OnGestureListener
{
	private GestureDetector gestureDetector = null;

	//variable declarations

	private int shaderProgramObject;

	private int shaderProgramObject_Shapes = 0;

	//VAO variables

	private int[] vao = new int[1];

	private int[] vbo_Position = new int[1];

	private int[] vao_Triangle = new int[1];
	private int[] vao_Square = new int[1];
	private int[] vao_Circle = new int[1];

	//vbo for shapes

	private int[] vbo_Position_Triangle = new int[1];
	private int[] vbo_Color_Triangle =  new int[1];

	private int[] vbo_Position_Square =  new int[1];
	private int[] vbo_Color_Square =  new int[1];

	private int[] vbo_Position_Circle =  new int[1];
	private int[] vbo_Color_Circle =  new int[1];
	

	private int[] vbo_Color = new int[1];

	private int mvpMatrixUniform;

	private int mvpMatrixUniform_Shapes;

	private float[] perspectiveProjectionMatrix = new float [16];

	//vectors to hold position coordinates of shapes 

	 Vector<Float> linePositions = new Vector<>();
     //Vector<Float> circlePositions = new Vector<>();
     //Vector<Float> circleColor = new Vector<>();

	 //boolean variables

	boolean enableGraph = false;
	boolean enableTriangle = false;
	boolean enableSquare = false;
	boolean enableCircle = true;

	//single tap variable

	private int singleTap = 0;

	public GLESView(Context context)
	{
		super(context);

		//OpenGL-ES related

		setEGLContextClientVersion(3);

		setRenderer(this);

		setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);



		//create and set GestureDetector object

		gestureDetector = new GestureDetector(context,this,null,false);

		gestureDetector.setOnDoubleTapListener(this);
	}

	//implementation of 3 methods of GLSurfaceView.Renderer interface

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config)
	{
		//code

		initialize(gl);
		
	}

	@Override
	public void onSurfaceChanged(GL10 gl,int width,int height)
	{
		//code

		resize(width,height);
	}

	@Override
	public void onDrawFrame(GL10 gl)
	{
		//code

		display();
	}


	//implementation of on touch event of view class

	@Override
	public boolean onTouchEvent(MotionEvent e)
	{
		if(!gestureDetector.onTouchEvent(e))
		{
			super.onTouchEvent(e);
		}

		return(true);
	}
	//implementation of 3 methods of OnDoubleTapListner interface
	@Override
	public boolean onDoubleTap(MotionEvent e)
	{
		

		return(true);
	}

	@Override
	public boolean onDoubleTapEvent(MotionEvent e)
	{
		return(true);
	}

	@Override
	public boolean onSingleTapConfirmed(MotionEvent e)
	{
		singleTap++;

		if(singleTap == 1)
		{
			enableGraph = true;
			enableCircle = false;
			enableSquare = false;
			enableTriangle = false;
		}
		else if (singleTap == 2)
		{
			enableGraph = true;
			enableTriangle = true;
			enableCircle = false;
			enableSquare = false;
		}
		else if (singleTap == 3)
		{
			enableGraph = true;
			enableTriangle = false;
			enableCircle = false;
			enableSquare = true;
		}
		else if (singleTap == 4)
		{
			enableGraph = true;
			enableTriangle = false;
			enableCircle = true;
			enableSquare = false;
		}
		else if(singleTap == 5)
		{
			enableGraph = true;
			enableTriangle = false;
			enableCircle = false;
			enableSquare = false;
		}
		else if(singleTap == 6)
		{
			enableGraph = false;
			enableTriangle = false;
			enableCircle = false;
			enableSquare = false;
		}
		else
		{

			singleTap = 0;
		}
		
		


		return(true);
	}



	//implementation of 6 methods of OnGestureListner interface

	@Override
	public boolean onDown(MotionEvent e)
	{
		
		return(true);
	}

	@Override
	public boolean onFling(MotionEvent e1,MotionEvent e2,float velocityX,float velocityY)
	{
		
		return(true);
	}

	@Override
	public void onLongPress(MotionEvent e)
	{
		
		
	}

	@Override
	public boolean onScroll(MotionEvent e1,MotionEvent e2,float distanceX,float distanceY)
	{
		//code

		uninitialize();

		System.exit(0);  //successfull exit status
		return(true);
		
	}

	@Override
	public void onShowPress(MotionEvent e)
	{

	}

	@Override
	public boolean onSingleTapUp(MotionEvent e)
	{
		return(true);
	}

	//implementation of our display(), resize(), initialize().

	private void initialize(GL10 gl)
	{
		

		//variable declaration
	
		float upperX = 0.0f;
		float lowerX = 0.0f;
		float upperY = 1.0f;
		float lowerY = -1.0f;

		//print OpenGL ES info
		printGLInfo(gl);

		//vertex shader 

		final String vertexShaderSourceCode = String.format
		(
			"#version 320 es"+            // (version of GLSL * 100) core means you are using core profile i.e PP
			"\n"+
			"in vec4 aPosition;"+
			"out vec4 oColor;"+
			"uniform mat4 uMVPMatrix;"+          //u for uniform datatype
			"void main(void)"+
			"{"+
			   "vec2 position = aPosition.xy;"+
			   "gl_Position=uMVPMatrix * vec4(position,0.0,1.0);"+
			   "if(gl_VertexID < 2)"+
			   "{"+
				   "oColor = vec4(0.0, 1.0, 0.0, 1.0);"+
			   "}\n"+
			   "else if(gl_VertexID >= 2 && gl_VertexID <= 3 )"+
			   "{"+
				   "oColor = vec4(1.0, 0.0, 0.0, 1.0);"+
			   "}"+
			   "else"+
			   "{"+
				   "oColor = vec4(0.0, 0.0, 1.0, 1.0);"+
			   "}"+
			"}"
		);

		int vertexShaderObject = GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);

		GLES32.glShaderSource(vertexShaderObject,vertexShaderSourceCode);

		GLES32.glCompileShader(vertexShaderObject);

		int[] shaderCompileStatus = new int[1];

		int[] infoLogLength = new int[1];

		String infoLog = null;

		GLES32.glGetShaderiv(vertexShaderObject,GLES32.GL_COMPILE_STATUS,shaderCompileStatus,0);

		if(shaderCompileStatus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetShaderiv(vertexShaderObject, GLES32.GL_INFO_LOG_LENGTH, infoLogLength,0);

			if(infoLogLength[0] > 0)
			{
				infoLog = GLES32.glGetShaderInfoLog(vertexShaderObject);

				System.out.println("PLP: Vertex Shader 1 Compilation Error : " + infoLog);

				uninitialize();

				System.exit(0);
			}
		}

		//fragment shader

		final String fragmentShaderSourceCode = String.format
		(
			"#version 320 es"+ 
			"\n"+
			"precision highp float;"+
			"in vec4 oColor;"+
			"out vec4 FragColor;\n"+ 
			"void main(void)\n"+ 
			"{\n"+ 
			"FragColor = oColor;\n"+                       //vec4 is working as a constructor here not as a datatype
			"}"
		);

		int fragmentShaderObject = GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);

		GLES32.glShaderSource(fragmentShaderObject,fragmentShaderSourceCode);

		GLES32.glCompileShader(fragmentShaderObject);

		shaderCompileStatus[0] = 0;

		infoLogLength[0] = 0;

		infoLog = null;

		GLES32.glGetShaderiv(fragmentShaderObject,GLES32.GL_COMPILE_STATUS,shaderCompileStatus,0);

		if(shaderCompileStatus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetShaderiv(fragmentShaderObject, GLES32.GL_INFO_LOG_LENGTH, infoLogLength,0);

			if(infoLogLength[0] > 0)
			{
				infoLog = GLES32.glGetShaderInfoLog(fragmentShaderObject);

				System.out.println("PLP: Fragment Shader 1 Compilation Error : " + infoLog);

				uninitialize();

				System.exit(0);
			}
		}

		//shader Program
		
		shaderProgramObject = GLES32.glCreateProgram();

		GLES32.glAttachShader(shaderProgramObject, vertexShaderObject);
		GLES32.glAttachShader(shaderProgramObject, fragmentShaderObject);

		GLES32.glBindAttribLocation(shaderProgramObject,VertexAttributesEnum.AMC_ATTRIBUTE_POSITION,"aPosition");

		//GLES32.glBindAttribLocation(shaderProgramObject,VertexAttributesEnum.AMC_ATTRIBUTE_COLOR,"aColor");


		GLES32.glLinkProgram(shaderProgramObject);

		int[] programLinkStatus = new int[1];

		infoLogLength[0] = 0;

		infoLog = null;

		

		GLES32.glGetProgramiv(shaderProgramObject, GLES32.GL_LINK_STATUS, programLinkStatus,0);

		if(programLinkStatus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetProgramiv(shaderProgramObject, GLES32.GL_INFO_LOG_LENGTH, infoLogLength,0);

			if(infoLogLength[0] > 0)
			{
				//infoLog = GLES32.glGetProgramInfoLog(shaderProgramObject, infoLogLength, 0);

				infoLog = GLES32.glGetProgramInfoLog(shaderProgramObject);

				System.out.println("PLP:Shader Program object 1 Compilation Error : " + infoLog);

				uninitialize();

				System.exit(0);
			}
		}

		//post linking uniform

		mvpMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uMVPMatrix");

		//geometry coordinates

		 linePositions = new Vector<>(Arrays.asList(0.0f, 1.0f, 0.0f,-1.0f,-1.0f,0.0f,1.0f,0.0f));

		/*final float[] triangleColor = new float[]
		{
			1.0f, 0.0f, 0.0f,
			0.0f,1.0f,0.0f,
			0.0f,0.0f,1.0f
		};*/

		for (int i = 0; i <= 39; i++)   //loop for +x vertical lines
		{
			 upperX = upperX + 0.025f;
			 lowerX = lowerX + 0.025f;

			 linePositions.add(upperX);

			 linePositions.add(upperY);

			 linePositions.add(lowerX);

			 linePositions.add(lowerY);

		 
		}

		upperX = 0.0f;
		lowerX = 0.0f;

	 for (int i = 0; i <= 39; i++)   //loop for -X vertical lines
	 {
		 upperX = upperX - 0.025f;
		 lowerX = lowerX - 0.025f;

		 linePositions.add(upperX);

		 linePositions.add(upperY);

		 linePositions.add(lowerX);

		 linePositions.add(lowerY);


	 }

		 float leftX = -1.0f;
		 float rightX = 1.0f;
	 
		 float leftY = 0.0f;
		 float rightY = 0.0f;

		 for (int i = 0; i <= 39; i++)    //loop for +y vertical lines
		 {
			 leftY = leftY + 0.025f;
			 rightY = rightY + 0.025f;

			 linePositions.add(leftX);

			 linePositions.add(leftY);

			 linePositions.add(rightX);

			 linePositions.add(rightY);


		 }

		  leftX = -1.0f;
		  rightX = 1.0f;

		  leftY = 0.0f;
		  rightY = 0.0f;

		 for (int i = 0; i <= 39; i++)    //loop for -y vertical lines
		 {
			 leftY = leftY - 0.025f;
			 rightY = rightY - 0.025f;

			 linePositions.add(leftX);

			 linePositions.add(leftY);

			 linePositions.add(rightX);

			 linePositions.add(rightY);


		 }

		//vao

		GLES32.glGenVertexArrays(1,vao,0);

		//Bind with VAO

		GLES32.glBindVertexArray(vao[0]);

		//VBO for position

		GLES32.glGenBuffers(1, vbo_Position,0);
		
		// Bind with VBO of position

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_Position[0]);

		//prepare triangle vertices array for glBuffer data

		//step 1 : allocate enough size buffer for triangle vertices

		ByteBuffer byteBuffer = ByteBuffer.allocateDirect(linePositions.size() * 4);

		//step 2: set byte order

		byteBuffer.order(ByteOrder.nativeOrder());

		//step3 : convert created buffer as float array

		FloatBuffer positionBuffer_Line = byteBuffer.asFloatBuffer();

		for (Float position : linePositions) {
            positionBuffer_Line.put(position);
        }

		 

		//step 4 : fill buffer with vertices array

		//positionBuffer_Line.put(linePositions);

		//step 5 : rewind the buffer to 0th index

		positionBuffer_Line.position(0);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, linePositions.size() * 4, positionBuffer_Line, GLES32.GL_STATIC_DRAW);

		GLES32.glVertexAttribPointer(VertexAttributesEnum.AMC_ATTRIBUTE_POSITION, 2, GLES32.GL_FLOAT, false, 0, 0);

		GLES32.glEnableVertexAttribArray(VertexAttributesEnum.AMC_ATTRIBUTE_POSITION);

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

	/*	//VBO for color

		GLES32.glGenBuffers(1, vbo_Color,0);
		
		// Bind with VBO of position

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_Color[0]);

		//prepare triangle vertices array for glBuffer data

		//step 1 : allocate enough size buffer for triangle vertices

		ByteBuffer byteBuffer_Color = ByteBuffer.allocateDirect(triangleColor.length * 4);

		//step 2: set byte order

		byteBuffer_Color.order(ByteOrder.nativeOrder());

		//step3 : convert created buffer as float array

		FloatBuffer colorBuffer = byteBuffer_Color.asFloatBuffer();

		//step 4 : fill buffer with vertices array

		colorBuffer.put(triangleColor);

		//step 5 : rewind the buffer to 0th index

		colorBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, triangleColor.length * 4, colorBuffer, GLES32.GL_STATIC_DRAW);

		GLES32.glVertexAttribPointer(VertexAttributesEnum.AMC_ATTRIBUTE_COLOR, 3, GLES32.GL_FLOAT, false, 0, 0);

		GLES32.glEnableVertexAttribArray(VertexAttributesEnum.AMC_ATTRIBUTE_COLOR);

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);*/


		
		//step 18 : unbind with VAO
		GLES32.glBindVertexArray(0);

		//************************************** shaders for shape handeling *************************************

		final String vertexShaderSourceCode_Shapes = String.format
		(
			"#version 320 es"+ 
			"\n"+
			"in vec4 inputPosition;\n"+
			"in vec4 inputColor;\n"+
			"out vec4 FragColor;\n"+
			"uniform mat4 uMVPMatrixShapes;"+
			"void main(void)\n"+
			"{\n"+
				"vec2 position = inputPosition.xy;"+
			   "gl_Position = uMVPMatrixShapes * vec4(position, 0.0, 1.0);\n"+
			   "FragColor = inputColor;\n"+
			"}"
		);

		int vertexShaderObject_Shapes = GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);

		GLES32.glShaderSource(vertexShaderObject_Shapes,vertexShaderSourceCode_Shapes);

		GLES32.glCompileShader(vertexShaderObject_Shapes);

		shaderCompileStatus[0] = 0;

		infoLogLength[0] = 0;

		infoLog = null;

		GLES32.glGetShaderiv(vertexShaderObject_Shapes,GLES32.GL_COMPILE_STATUS,shaderCompileStatus,0);

		if(shaderCompileStatus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetShaderiv(vertexShaderObject_Shapes, GLES32.GL_INFO_LOG_LENGTH, infoLogLength,0);

			if(infoLogLength[0] > 0)
			{
				infoLog = GLES32.glGetShaderInfoLog(vertexShaderObject_Shapes);

				System.out.println("PLP: Vertex Shader 2  Compilation Error : " + infoLog);

				uninitialize();

				System.exit(0);
			}
		}


		final String fragmentShaderSourceCode_Shapes = String.format
		(
			"#version 320 es"+ 
			"\n"+
			"precision highp float;"+
			"in vec4 FragColor;\n"+
			"out vec4 outputColor;\n"+
			"void main(void)\n"+
			"{\n"+
			"outputColor = FragColor;\n"+
			"}\n"
		);

		int fragmentShaderObject_Shapes = GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);

		GLES32.glShaderSource(fragmentShaderObject_Shapes,fragmentShaderSourceCode_Shapes);

		GLES32.glCompileShader(fragmentShaderObject_Shapes);

		shaderCompileStatus[0] = 0;

		infoLogLength[0] = 0;

		infoLog = null;

		GLES32.glGetShaderiv(fragmentShaderObject_Shapes,GLES32.GL_COMPILE_STATUS,shaderCompileStatus,0);

		if(shaderCompileStatus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetShaderiv(fragmentShaderObject_Shapes, GLES32.GL_INFO_LOG_LENGTH, infoLogLength,0);

			if(infoLogLength[0] > 0)
			{
				infoLog = GLES32.glGetShaderInfoLog(fragmentShaderObject_Shapes);

				System.out.println("PLP: Fragment Shader 2 Compilation Error : " + infoLog);

				uninitialize();

				System.exit(0);
			}
		}

		//shader Program
		
		shaderProgramObject_Shapes = GLES32.glCreateProgram();

		GLES32.glAttachShader(shaderProgramObject_Shapes, vertexShaderObject_Shapes);
		GLES32.glAttachShader(shaderProgramObject_Shapes, fragmentShaderObject_Shapes);

		GLES32.glBindAttribLocation(shaderProgramObject_Shapes, VertexAttributesEnum.AMC_ATTRIBUTE_SHAPES_POSITION, "inputPosition");
		GLES32.glBindAttribLocation(shaderProgramObject_Shapes, VertexAttributesEnum.AMC_ATTRIBUTE_SHAPES_COLOR, "inputColor");

		//GLES32.glBindAttribLocation(shaderProgramObject,VertexAttributesEnum.AMC_ATTRIBUTE_COLOR,"aColor");


		GLES32.glLinkProgram(shaderProgramObject_Shapes);

		programLinkStatus[0] = 0;

		infoLogLength[0] = 0;

		infoLog = null;

		

		GLES32.glGetProgramiv(shaderProgramObject_Shapes, GLES32.GL_LINK_STATUS, programLinkStatus,0);

		if(programLinkStatus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetProgramiv(shaderProgramObject_Shapes, GLES32.GL_INFO_LOG_LENGTH, infoLogLength,0);

			if(infoLogLength[0] > 0)
			{
				//infoLog = GLES32.glGetProgramInfoLog(shaderProgramObject, infoLogLength, 0);

				infoLog = GLES32.glGetProgramInfoLog(shaderProgramObject_Shapes);

				System.out.println("PLP:Shader Program object 2 Compilation Error : " + infoLog);

				uninitialize();

				System.exit(0);
			}
		}

		//post link uniform

		 mvpMatrixUniform_Shapes = GLES32.glGetUniformLocation(shaderProgramObject_Shapes, "uMVPMatrixShapes");

		 // triangle position

		 final float[] trianglePosition = new float[]
		 {
			 0.0f,0.5f,  //top vertex
			-0.5f,-0.5f, //left vertex

			-0.5f,-0.5f, //left vertex
			0.5f,-0.5f, //right vertex

			0.5f,-0.5f, //right vertex
			0.0f,0.5f,  //top vertex
		 };

		 final float[] triangleColor = new float[]
		 {
			 1.0f,1.0f,0.0f,
			 1.0f,1.0f,0.0f,
			 1.0f,1.0f,0.0f,
			 1.0f,1.0f,0.0f,
			 1.0f,1.0f,0.0f,
			 1.0f,1.0f,0.0f
		 };

		 final float[] squarePosition = new float[]
		 {
			  0.5f,0.5f, //right top
			 -0.5f,0.5f, //left top

			 -0.5f,0.5f, //left top
			 -0.5f,-0.5f, //left bottom

			  -0.5f,-0.5f, //left bottom
			 0.5f,-0.5f,   //right bottom

			  0.5f,-0.5f,   //right bottom
			  0.5f,0.5f     //right top
		 };

		 final float[] squareColor = new float[]
		 {
			 1.0f,1.0f,0.0f,
			 1.0f,1.0f,0.0f,
			 1.0f,1.0f,0.0f,
			 1.0f,1.0f,0.0f,
			 1.0f,1.0f,0.0f,
			 1.0f,1.0f,0.0f,
			 1.0f,1.0f,0.0f,
			 1.0f,1.0f,0.0f
		 };

		  //code to find circle positions

		 float radious = 0.5f;

		 /*for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.01f)
		 {

			 //float radianangle = angle * (M_PI * 180.0f);
			 float x = radious * (float)Math.cos((angle * Math.PI) / 180.0f) + (0.0f);
			 float y = radious * (float)Math.sin((angle * Math.PI) / 180.0f) + (0.0f);

			 circlePositions.add(x);
			 circlePositions.add(y);


		 }*/

		 //circlePositions.add()

		 //code for circle Color

		/* for (int i = 0; i < circlePositions.size(); i++)
		 {
			 circleColor.add(1.0f);
			 circleColor.add(1.0f);
			 circleColor.add(0.0f);
		 }*/

		 final float[] circlePositionArray = new float[72002];

		 final float[] circleColorArray = new float[108003];

		 int posIndex = 0;
		 int colorIndex = 0;

		 for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.01f)
		 {

			 //float radianangle = angle * (M_PI * 180.0f);
			 float x = radious * (float)Math.cos((angle * Math.PI) / 180.0f) + (0.0f);
			 float y = radious * (float)Math.sin((angle * Math.PI) / 180.0f) + (0.0f);

			 circlePositionArray[posIndex] = x;
			 circlePositionArray[posIndex + 1] = y;

			 posIndex += 2;

			 circleColorArray[colorIndex] = 1.0f;
			 circleColorArray[colorIndex + 1] = 1.0f;
			 circleColorArray[colorIndex + 2] = 0.0f;

			 colorIndex += 3;


		 }
		 
		 //vao for triangle

		

		GLES32.glGenVertexArrays(1,vao_Triangle,0);

		//Bind with VAO

		GLES32.glBindVertexArray(vao_Triangle[0]);

		//VBO for position

		GLES32.glGenBuffers(1, vbo_Position_Triangle,0);
		
		// Bind with VBO of position

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_Position_Triangle[0]);

		//prepare triangle vertices array for glBuffer data

		ByteBuffer byteBuffer_T = ByteBuffer.allocateDirect(trianglePosition.length * 4);

		//step 2: set byte order

		byteBuffer_T.order(ByteOrder.nativeOrder());

		//step3 : convert created buffer as float array

		FloatBuffer positionBuffer_T = byteBuffer_T.asFloatBuffer();

		//step 4 : fill buffer with vertices array

		positionBuffer_T.put(trianglePosition);

		//step 5 : rewind the buffer to 0th index

		positionBuffer_T.position(0);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, trianglePosition.length * 4, positionBuffer_T, GLES32.GL_STATIC_DRAW);

		GLES32.glVertexAttribPointer(VertexAttributesEnum.AMC_ATTRIBUTE_SHAPES_POSITION, 2, GLES32.GL_FLOAT, false, 0, 0);

		GLES32.glEnableVertexAttribArray(VertexAttributesEnum.AMC_ATTRIBUTE_SHAPES_POSITION);

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);


		//vbo for triangle color

		

		GLES32.glGenBuffers(1, vbo_Color_Triangle,0);
		
		

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_Color_Triangle[0]);

	

		ByteBuffer byteBuffer_TC = ByteBuffer.allocateDirect(triangleColor.length * 4);

		//step 2: set byte order

		byteBuffer_TC.order(ByteOrder.nativeOrder());

		//step3 : convert created buffer as float array

		FloatBuffer colorBuffer_TC = byteBuffer_TC.asFloatBuffer();

		//step 4 : fill buffer with vertices array

		colorBuffer_TC.put(triangleColor);

		//step 5 : rewind the buffer to 0th index

		colorBuffer_TC.position(0);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, triangleColor.length * 4, colorBuffer_TC, GLES32.GL_STATIC_DRAW);

		GLES32.glVertexAttribPointer(VertexAttributesEnum.AMC_ATTRIBUTE_SHAPES_COLOR, 3, GLES32.GL_FLOAT, false, 0, 0);

		GLES32.glEnableVertexAttribArray(VertexAttributesEnum.AMC_ATTRIBUTE_SHAPES_COLOR);

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);


		GLES32.glBindVertexArray(0);

		//vao for square
	
		GLES32.glGenVertexArrays(1,vao_Square,0);

		//Bind with VAO

		GLES32.glBindVertexArray(vao_Square[0]);

		//VBO for position

		GLES32.glGenBuffers(1, vbo_Position_Square,0);
		
		// Bind with VBO of position

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_Position_Square[0]);

		//prepare triangle vertices array for glBuffer data

		ByteBuffer byteBuffer_S = ByteBuffer.allocateDirect(squarePosition.length * 4);

		//step 2: set byte order

		byteBuffer_S.order(ByteOrder.nativeOrder());

		//step3 : convert created buffer as float array

		FloatBuffer positionBuffer_S = byteBuffer_S.asFloatBuffer();

		//step 4 : fill buffer with vertices array

		positionBuffer_S.put(squarePosition);

		//step 5 : rewind the buffer to 0th index

		positionBuffer_S.position(0);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, squarePosition.length * 4, positionBuffer_S, GLES32.GL_STATIC_DRAW);

		GLES32.glVertexAttribPointer(VertexAttributesEnum.AMC_ATTRIBUTE_SHAPES_POSITION, 2, GLES32.GL_FLOAT, false, 0, 0);

		GLES32.glEnableVertexAttribArray(VertexAttributesEnum.AMC_ATTRIBUTE_SHAPES_POSITION);

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);


		//vbo for square color

		

		GLES32.glGenBuffers(1, vbo_Color_Square,0);
		
		

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_Color_Square[0]);

	

		ByteBuffer byteBuffer_SC = ByteBuffer.allocateDirect(squareColor.length * 4);

		//step 2: set byte order

		byteBuffer_SC.order(ByteOrder.nativeOrder());

		//step3 : convert created buffer as float array

		FloatBuffer colorBuffer_SC = byteBuffer_SC.asFloatBuffer();

		//step 4 : fill buffer with vertices array

		colorBuffer_SC.put(squareColor);

		//step 5 : rewind the buffer to 0th index

		colorBuffer_SC.position(0);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, squareColor.length * 4, colorBuffer_SC, GLES32.GL_STATIC_DRAW);

		GLES32.glVertexAttribPointer(VertexAttributesEnum.AMC_ATTRIBUTE_SHAPES_COLOR, 3, GLES32.GL_FLOAT, false, 0, 0);

		GLES32.glEnableVertexAttribArray(VertexAttributesEnum.AMC_ATTRIBUTE_SHAPES_COLOR);

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);


		GLES32.glBindVertexArray(0);

		//VAO for circle

		
		GLES32.glGenVertexArrays(1,vao_Circle,0);

		//Bind with VAO

		GLES32.glBindVertexArray(vao_Circle[0]);

		//VBO for position

		GLES32.glGenBuffers(1, vbo_Position_Circle,0);
		
		// Bind with VBO of position

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_Position_Circle[0]);

		// Step 1: Allocate a direct ByteBuffer
        ByteBuffer byteBuffer_CP = ByteBuffer.allocateDirect(circlePositionArray.length * 4);

        // Step 2: Set byte order
        byteBuffer_CP.order(ByteOrder.nativeOrder());

        // Step 3: Convert the ByteBuffer to a FloatBuffer
        FloatBuffer positionBuffer_CP = byteBuffer_CP.asFloatBuffer();

        // Step 4: Fill the FloatBuffer with the Vector's elements
        
		
        positionBuffer_CP.put(circlePositionArray);
        

        // Step 5: Rewind the buffer to the 0th index
        positionBuffer_CP.position(0);


		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, circlePositionArray.length * 4, positionBuffer_CP, GLES32.GL_STATIC_DRAW);

		GLES32.glVertexAttribPointer(VertexAttributesEnum.AMC_ATTRIBUTE_SHAPES_POSITION, 2, GLES32.GL_FLOAT, false, 0, 0);

		GLES32.glEnableVertexAttribArray(VertexAttributesEnum.AMC_ATTRIBUTE_SHAPES_POSITION);

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);


		//vbo for circle color

		

		GLES32.glGenBuffers(1, vbo_Color_Circle,0);
		
		

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_Color_Circle[0]);

	

		// Step 1: Allocate a direct ByteBuffer
        ByteBuffer byteBuffer_CC = ByteBuffer.allocateDirect(circleColorArray.length * 4);

        // Step 2: Set byte order
        byteBuffer_CC.order(ByteOrder.nativeOrder());

        // Step 3: Convert the ByteBuffer to a FloatBuffer
        FloatBuffer colorBuffer_CC = byteBuffer_CC.asFloatBuffer();

        // Step 4: Fill the FloatBuffer with the Vector's elements
       
		
        colorBuffer_CC.put(circleColorArray);
        

        // Step 5: Rewind the buffer to the 0th index
        colorBuffer_CC.position(0);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, circleColorArray.length * 4, colorBuffer_CC, GLES32.GL_STATIC_DRAW);

		GLES32.glVertexAttribPointer(VertexAttributesEnum.AMC_ATTRIBUTE_SHAPES_COLOR, 3, GLES32.GL_FLOAT, false, 0, 0);

		GLES32.glEnableVertexAttribArray(VertexAttributesEnum.AMC_ATTRIBUTE_SHAPES_COLOR);

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);


		GLES32.glBindVertexArray(0);


		//depth setting

		GLES32.glClearDepthf(1.0f);

		GLES32.glEnable(GLES32.GL_DEPTH_TEST);

		GLES32.glDepthFunc(GLES32.GL_LEQUAL);

		//enable backface culling

		

		//clear color

		GLES32.glClearColor(0.0f,0.0f,0.0f,1.0f);

		Matrix.setIdentityM(perspectiveProjectionMatrix,0);

		

	}

	private void printGLInfo(GL10 gl)
	{
		//code

		System.out.println("PLP: OpenGL-ES Renderer : " + gl.glGetString(GL10.GL_RENDERER));

		System.out.println("PLP: OpenGL-ES version : " + gl.glGetString(GL10.GL_VERSION));

		System.out.println("PLP: OpenGL-ES Shading Launguage Version : " + gl.glGetString(GLES32.GL_SHADING_LANGUAGE_VERSION));
	}

	private void resize(int width,int height)
	{
		//code

		if(height <= 0)
		{
			height = 1;
		}

		GLES32.glViewport(0,0,width,height);

		Matrix.perspectiveM(perspectiveProjectionMatrix,0,45.0f,(float)width/(float)height,0.1f,100.0f);
	}

	private void display()
	{

		//variable declaration 

		int lineCounter = 0;
		//code

		GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);

		//step 1: Use shader program object

		GLES32.glUseProgram(shaderProgramObject);

		//transformations

		float[] modelViewMatrix = new float[16];
		float[] modelViewProjectionMatrix = new float[16];

		Matrix.setIdentityM(modelViewMatrix,0);
		Matrix.setIdentityM(modelViewProjectionMatrix,0);

		Matrix.translateM(modelViewMatrix,0, 0.0f, 0.0f, -2.5f);
		Matrix.multiplyMM(modelViewProjectionMatrix, 0, perspectiveProjectionMatrix, 0, modelViewMatrix, 0);

		//push above mvp into vertex shader's mvp uniform

		GLES32.glUniformMatrix4fv(mvpMatrixUniform, 1, false, modelViewProjectionMatrix,0);

		GLES32.glBindVertexArray(vao[0]);

		if (enableGraph == true)
		{
			GLES32.glLineWidth(3.5f);

			GLES32.glDrawArrays(GLES32.GL_LINES, 0, 4);

			GLES32.glLineWidth(1.0f);

			int offset = 0;
			int fun = 1;

			for (int i = 0; i < 80; i++)
			{
				if ((fun % 5) == 0)
				{
					GLES32.glLineWidth(3.0f);
				}
				else
				{
					GLES32.glLineWidth(1.0f);
				}
				GLES32.glDrawArrays(GLES32.GL_LINES, 4 + offset, 2);

				fun += 1;
				offset += 2;
			}

		//glDrawArrays(GL_LINES, 4, 80);  // right side vertical lines

			offset = 0;
			fun = 1;

			for (int i = 0; i < 80; i++)
			{
				if ((fun % 5) == 0)
				{
					GLES32.glLineWidth(3.0f);
				}
				else
				{
					GLES32.glLineWidth(1.0f);
				}

				GLES32.glDrawArrays(GLES32.GL_LINES, 84 + offset, 2);
				fun += 1;
				offset += 2;
			}

		//glDrawArrays(GL_LINES, 84, 80); // left side vertical lines

			offset = 0;
			fun = 1;

			for (int i = 0; i < 80; i++)
			{
				if ((fun % 5) == 0)
				{
					GLES32.glLineWidth(3.0f);
				}
				else
				{
					GLES32.glLineWidth(1.0f);
				}
				GLES32.glDrawArrays(GLES32.GL_LINES, 164 + offset, 2);

				fun += 1;
				offset += 2;
			}

		//glDrawArrays(GL_LINES, 164, 80); //upper horizonta lines

			offset = 0;
			fun = 1;

			for (int i = 0; i < 80; i++)
			{
				if ((fun % 5) == 0)
				{
					GLES32.glLineWidth(3.0f);
				}
				else
				{
					GLES32.glLineWidth(1.0f);
				}
				GLES32.glDrawArrays(GLES32.GL_LINES, 244 + offset, 2);

				fun += 1;
				offset += 2;
			}
		//glDrawArrays(GL_LINES, 244, 80); // lower horizontal lines

			GLES32.glBindVertexArray(0);

			if (enableTriangle == true)
			{
				GLES32.glUseProgram(shaderProgramObject_Shapes);

				float[] modelViewMatrix_Shapes = new float[16];
				float[] modelViewProjectionMatrix_Shapes = new float[16];

				Matrix.setIdentityM(modelViewMatrix_Shapes,0);
				Matrix.setIdentityM(modelViewProjectionMatrix_Shapes,0);

				Matrix.translateM(modelViewMatrix_Shapes,0, 0.0f, 0.0f, -2.5f);
				Matrix.multiplyMM(modelViewProjectionMatrix_Shapes, 0, perspectiveProjectionMatrix, 0, modelViewMatrix_Shapes, 0);

			//push above mvp into vertex shader's mvp uniform

				GLES32.glUniformMatrix4fv(mvpMatrixUniform_Shapes, 1, false, modelViewProjectionMatrix_Shapes,0);

				GLES32.glBindVertexArray(vao_Triangle[0]);

				GLES32.glDrawArrays(GLES32.GL_LINES, 0, 6);

				GLES32.glBindVertexArray(0);

				GLES32.glUseProgram(0);
			}

			if(enableSquare == true)
			{
				GLES32.glUseProgram(shaderProgramObject_Shapes);

				
				float[] modelViewMatrix_Shapes = new float[16];
				float[] modelViewProjectionMatrix_Shapes = new float[16];

				Matrix.setIdentityM(modelViewMatrix_Shapes,0);
				Matrix.setIdentityM(modelViewProjectionMatrix_Shapes,0);

				Matrix.translateM(modelViewMatrix_Shapes,0, 0.0f, 0.0f, -2.5f);
				Matrix.multiplyMM(modelViewProjectionMatrix_Shapes, 0, perspectiveProjectionMatrix, 0, modelViewMatrix_Shapes, 0);

			//push above mvp into vertex shader's mvp uniform

				GLES32.glUniformMatrix4fv(mvpMatrixUniform_Shapes, 1, false, modelViewProjectionMatrix_Shapes,0);

				GLES32.glBindVertexArray(vao_Square[0]);

				GLES32.glDrawArrays(GLES32.GL_LINES, 0, 8);

				GLES32.glBindVertexArray(0);

				GLES32.glUseProgram(0);
			}

			if(enableCircle == true)
			{
				GLES32.glUseProgram(shaderProgramObject_Shapes);

				float[] modelViewMatrix_Shapes = new float[16];
				float[] modelViewProjectionMatrix_Shapes = new float[16];

				Matrix.setIdentityM(modelViewMatrix_Shapes,0);
				Matrix.setIdentityM(modelViewProjectionMatrix_Shapes,0);

				Matrix.translateM(modelViewMatrix_Shapes,0, 0.0f, 0.0f, -2.5f);
				Matrix.multiplyMM(modelViewProjectionMatrix_Shapes, 0, perspectiveProjectionMatrix, 0, modelViewMatrix_Shapes, 0);

			//push above mvp into vertex shader's mvp uniform

				GLES32.glUniformMatrix4fv(mvpMatrixUniform_Shapes, 1, false, modelViewProjectionMatrix_Shapes,0);

				GLES32.glBindVertexArray(vao_Circle[0]);

				//GLES32.glPointSize(2.0f);

				GLES32.glDrawArrays(GLES32.GL_LINES, 0, 36001);

				//GLES32.glPointSize(1.0f);

				GLES32.glBindVertexArray(0);

				GLES32.glUseProgram(0);
			}
		
		}

		else
		{
			GLES32.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		}


		//GLES32.glBindVertexArray(0);

		//step 5: Unuse shader program object
		GLES32.glUseProgram(0);


		//Render

		requestRender();

	}

	private void uninitialize()
	{
		//code

		if(shaderProgramObject > 0)
		{
			GLES32.glUseProgram(shaderProgramObject);
			int[] retVal = new int[1];

			//step 2 : get number of attached shaders
			GLES32.glGetProgramiv(shaderProgramObject, GLES32.GL_ATTACHED_SHADERS, retVal,0);

			if(retVal[0] > 0)
			{
				int numAttachedShaders = retVal[0];

				int[] shaderObjects = new int[numAttachedShaders];

				GLES32.glGetAttachedShaders(shaderProgramObject, numAttachedShaders, retVal,0, shaderObjects,0);

				for (int i = 0; i < numAttachedShaders ; i++)
				{
					GLES32.glDetachShader(shaderProgramObject, shaderObjects[i]);
					GLES32.glDeleteShader(shaderObjects[i]);
					shaderObjects[i] = 0;
				}

			}

			GLES32.glUseProgram(0);
			GLES32.glDeleteProgram(shaderProgramObject);
			shaderProgramObject = 0;

			


		}

		//code

		if(shaderProgramObject_Shapes > 0)
		{
			GLES32.glUseProgram(shaderProgramObject_Shapes);
			int[] retVal = new int[1];

			//step 2 : get number of attached shaders
			GLES32.glGetProgramiv(shaderProgramObject_Shapes, GLES32.GL_ATTACHED_SHADERS, retVal,0);

			if(retVal[0] > 0)
			{
				int numAttachedShaders = retVal[0];

				int[] shaderObjects = new int[numAttachedShaders];

				GLES32.glGetAttachedShaders(shaderProgramObject_Shapes, numAttachedShaders, retVal,0, shaderObjects,0);

				for (int i = 0; i < numAttachedShaders ; i++)
				{
					GLES32.glDetachShader(shaderProgramObject_Shapes, shaderObjects[i]);
					GLES32.glDeleteShader(shaderObjects[i]);
					shaderObjects[i] = 0;
				}

			}

			GLES32.glUseProgram(0);
			GLES32.glDeleteProgram(shaderProgramObject_Shapes);
			shaderProgramObject = 0;

			


		}

		if(vbo_Color[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo_Color, 0);
			vbo_Color[0] = 0;
		}		

		if(vbo_Color_Square[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo_Color_Square, 0);
			vbo_Color_Square[0] = 0;
		}

		if(vbo_Color_Triangle[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo_Color_Triangle, 0);
			vbo_Color_Triangle[0] = 0;
		}


		if(vbo_Position[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo_Position, 0);
			vbo_Position[0] = 0;
		}

		if(vbo_Position_Triangle[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo_Position_Triangle, 0);
			vbo_Position_Triangle[0] = 0;
		}

		if(vbo_Position_Square[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo_Position_Square, 0);
			vbo_Position_Square[0] = 0;
		}

		if(vao[0] > 0)
		{
			GLES32.glDeleteVertexArrays(1, vao, 0);
			vao[0] = 0;
		}

		if(vao_Square[0] > 0)
		{
			GLES32.glDeleteVertexArrays(1, vao_Square, 0);
			vao_Square[0] = 0;
		}

		
		if(vao_Triangle[0] > 0)
		{
			GLES32.glDeleteVertexArrays(1, vao_Triangle, 0);
			vao_Triangle[0] = 0;
		}
	}

};

