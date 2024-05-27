package com.PLP.pfl_two_static_lights;

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

//package related to sphere

import java.nio.ShortBuffer;

public class GLESView extends GLSurfaceView implements GLSurfaceView.Renderer, OnDoubleTapListener, OnGestureListener
{
	private GestureDetector gestureDetector = null;

	//variable declarations

	private int shaderProgramObject;

	private int[] vao = new int[1];

	private int[] vbo = new int[1];

	private int[] vbo_Normal = new int[1];

	//projection related uniforms

	//private int modelViewMatrixUniform;

	private int modelMatrixUniform;

	private int viewMatrixUniform;

	private int projectionMatrixUniform;

	//private int mvpMatrixUniform;

	private float[] perspectiveProjectionMatrix = new float [16];

	//light uniform variables

	private int[] lightDiffuseUniform = new int[2];

	private int[] lightPositionUniform = new int[2];

	private int[] lightAmbientUniform = new int[2];

	private int[] lightSpecularUniform = new int[2];

	//material uniform variables

	private int materialDiffuseUniform = 0;

	private int materialAmbientUniform = 0;

	private int materialSpecularUniform = 0;

	private int materialShininessUniform = 0;

	//double tap uniform

	private int doubleTapnUniform = 0;

	//lighting enable variable

	private boolean bLightingEnabled = false;
	private boolean bAnimationEnabled = false;

	//material properties

	private float[] materialDiffuse = new float[] {1.0f,1.0f,1.0f,1.0f};

	private float[] materialAmbient = new float[] {0.0f,0.0f,0.0f,1.0f};

	private float[] materialSpecular = new float[] {1.0f,1.0f,1.0f,1.0f};

	private float materialShininess = 128.0f;

	private class Light
	{
		float [] ambient = new float[3];
		float [] diffuse = new float[3];
		float [] specular = new float[3];
		float [] position = new float[4];

	} 

	private Light[] lights = new Light[2];

	private float anglePyramid = 0.0f;

	//sphere related variables

	private int numVertices = 0;

	private int numElements = 0;

	// other variables

	FloatBuffer lightAmbientBuffer0;

	FloatBuffer lightAmbientBuffer1;

	FloatBuffer lightDiffuseBuffer0;

	FloatBuffer lightDiffuseBuffer1;

	FloatBuffer lightSpecularBuffer0;

	
	FloatBuffer lightSpecularBuffer1;



	FloatBuffer lightPositionBuffer0;

	FloatBuffer lightPositionBuffer1;

	FloatBuffer materialAmbientBuffer;

	FloatBuffer materialDiffuseBuffer;

	FloatBuffer materialSpecularBuffer;


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

		if(bAnimationEnabled == true)
		{
			update();
		}
		
		
		

		
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
		if(bLightingEnabled == false)
		{
			bLightingEnabled = true;
		}
		else
		{
			bLightingEnabled = false;
		}

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
		if(bAnimationEnabled == false)
		{
			bAnimationEnabled = true;
		}
		else
		{
			bAnimationEnabled = false;
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
		//code

		//print OpenGL ES info
		printGLInfo(gl);

		//vertex shader 

		final String vertexShaderSourceCode = String.format
		(
			"#version 320 es"+
			"\n"+
			"in vec4 aPosition;"+
			"in vec3 aNormal;"+
			"uniform mat4 uModelMatrix;"+
			"uniform mat4 uViewMatrix;"+
			"uniform mat4 uProjectionMatrix;"+
			"uniform vec4 ulightPosition[2];"+
			"uniform highp int uDoubleTap;"+
			"out vec3 otransformedNormals;"+
			"out vec3 oviewerVector;"+
			"out vec3 olightDirection[2];"+
			"void main(void)"+
			"{"+
			"if(uDoubleTap == 1)"+
			"{"+
				"vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;"+
				"otransformedNormals = (mat3(uViewMatrix * uModelMatrix) * aNormal);"+	
				"for(int i = 0; i < 2; i++)"+
				"{"	+	
					"olightDirection[i] = (vec3(ulightPosition[i] - eyeCoordinates));"+	
					"oviewerVector = (-eyeCoordinates.xyz);"+
				"}"+
			"}"+
			"else"+
			"{"+
			//"oPhongADSLight = vec3( 0.0, 0.0, 0.0);\n
			   /* "otransformedNormals = vec3(0.0, 0.0, 0.0);\n"
				"for(int i = 0; i < 2; i++)\n"
				"{\n"
					"olightDirection[i] = vec3 (0.0, 0.0, 0.0);\n"
		       
				"}\n"
				 "oviewerVector = vec3(0.0, 0.0, 0.0);\n"*/
			"}"+
			"gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;"+
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

				System.out.println("PLP: Vertex Shader Compilation Error : " + infoLog);

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
			"in vec3 otransformedNormals;"+
			"in vec3 olightDirection[2];"+
			"in vec3 oviewerVector;"+
			"uniform mat4 uModelMatrix;"+
			"uniform mat4 uViewMatrix;"+
			"uniform mat4 uProjectionMatrix;"+
			"uniform vec3 ulightAmbient[2];"+
			"uniform vec3 ulightDiffuse[2];"+
			"uniform vec3 ulightSpecular[2];"+
			"uniform vec3 uMaterialAmbient;"+
			"uniform vec3 uMaterialDiffuse;"+
			"uniform vec3 uMaterialSpecular;"+
			"uniform float uMaterialShininess;"+
			"uniform highp int uDoubleTap;"+
			"out vec4 FragColor;"+
			"void main(void)"+
			"{"+
			"vec3 PhongADSLight;"+ // local variable
			"if(uDoubleTap == 1)"+
			"{"+
			"vec3 normalizedLightDirection[2];"+
			"vec3 normalizedTransformedNormals = normalize(otransformedNormals);"+
			"vec3 normalizedViwerVector = normalize(oviewerVector);"+
			"vec3 lightAmbient[2];"+
			"vec3 lightDiffuse[2];"+
			"vec3 reflectionVector[2];"+
			"vec3 lightSpecular[2];"+
			"for(int i = 0; i < 2; i++)"+
			"{"+
				"lightAmbient[i] = ulightAmbient[i] * uMaterialAmbient;"+
				"normalizedLightDirection[i] = normalize(olightDirection[i]);"+
				"lightDiffuse[i] = ulightDiffuse[i] * uMaterialDiffuse * max(dot(normalizedLightDirection[i], normalizedTransformedNormals),0.0);"+
				"reflectionVector[i] = reflect(-normalizedLightDirection[i], normalizedTransformedNormals);"+
			
				"lightSpecular[i] = ulightSpecular[i] * uMaterialSpecular * pow(max(dot(reflectionVector[i], normalizedViwerVector), 0.0),uMaterialShininess);"+
				"PhongADSLight = PhongADSLight + lightAmbient[i] + lightDiffuse[i] + lightSpecular[i];"+
			"}"+
			"FragColor = vec4(PhongADSLight, 1.0);"+
			"}"+
			"else"+
			"{"+
				"FragColor = vec4(1.0, 1.0, 1.0,1.0);"+
			"}"+
		   
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

				System.out.println("PLP: Fragment Shader Compilation Error : " + infoLog);

				uninitialize();

				System.exit(0);
			}
		}

		//shader Program
		
		shaderProgramObject = GLES32.glCreateProgram();

		GLES32.glAttachShader(shaderProgramObject, vertexShaderObject);
		GLES32.glAttachShader(shaderProgramObject, fragmentShaderObject);

		GLES32.glBindAttribLocation(shaderProgramObject,VertexAttributesEnum.AMC_ATTRIBUTE_POSITION,"aPosition");

		GLES32.glBindAttribLocation(shaderProgramObject,VertexAttributesEnum.AMC_ATTRIBUTE_NORMALS,"aNormal");

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

				System.out.println("PLP:Shader Program object Compilation Error : " + infoLog);

				uninitialize();

				System.exit(0);
			}
		}

		//post linking uniform

		 modelMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uModelMatrix");

		viewMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uViewMatrix");

		projectionMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uProjectionMatrix");

		lightAmbientUniform[0] = GLES32.glGetUniformLocation(shaderProgramObject, "ulightAmbient[0]");

		lightDiffuseUniform[0] = GLES32.glGetUniformLocation(shaderProgramObject, "ulightDiffuse[0]");

		lightSpecularUniform[0] = GLES32.glGetUniformLocation(shaderProgramObject, "ulightSpecular[0]");

		lightPositionUniform[0] = GLES32.glGetUniformLocation(shaderProgramObject, "ulightPosition[0]");
	
		lightAmbientUniform[1] = GLES32.glGetUniformLocation(shaderProgramObject, "ulightAmbient[1]");

		lightDiffuseUniform[1] = GLES32.glGetUniformLocation(shaderProgramObject, "ulightDiffuse[1]");

		lightSpecularUniform[1] = GLES32.glGetUniformLocation(shaderProgramObject, "ulightSpecular[1]");

		lightPositionUniform[1] = GLES32.glGetUniformLocation(shaderProgramObject, "ulightPosition[1]");


		materialAmbientUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uMaterialAmbient");

		materialDiffuseUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uMaterialDiffuse");


		materialSpecularUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uMaterialSpecular");

		materialShininessUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uMaterialShininess");

		doubleTapnUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uDoubleTap");

		

       final float[] pyramidPosition = new float[]
	   {
			// front
			0.0f, 1.0f, 0.0f,
			-1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, 1.0f,

			// right
			0.0f, 1.0f, 0.0f,
			1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, -1.0f,

			// back
			0.0f, 1.0f, 0.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,

			// left
			0.0f, 1.0f, 0.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, 1.0f
	   };

	   final float[] pyramidNormals = new float[]
		{
			// front
		 0.000000f, 0.447214f,  0.894427f, // front-top
		 0.000000f, 0.447214f,  0.894427f, // front-left
		 0.000000f, 0.447214f,  0.894427f, // front-right

		 // right			    
		 0.894427f, 0.447214f,  0.000000f, // right-top
		 0.894427f, 0.447214f,  0.000000f, // right-left
		 0.894427f, 0.447214f,  0.000000f, // right-right

		 // back
		 0.000000f, 0.447214f, -0.894427f, // back-top
		 0.000000f, 0.447214f, -0.894427f, // back-left
		 0.000000f, 0.447214f, -0.894427f, // back-right

		 // left
		-0.894427f, 0.447214f,  0.000000f, // left-top
		-0.894427f, 0.447214f,  0.000000f, // left-left
		-0.894427f, 0.447214f,  0.000000f // left-right
		};

		//vao

		GLES32.glGenVertexArrays(1,vao,0);

		//Bind with VAO

		GLES32.glBindVertexArray(vao[0]);

		//VBO for position

		GLES32.glGenBuffers(1, vbo,0);
		
		// Bind with VBO of position

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo[0]);

		//prepare triangle vertices array for glBuffer data

		//step 1 : allocate enough size buffer for triangle vertices

		ByteBuffer byteBuffer = ByteBuffer.allocateDirect(pyramidPosition.length * 4);

		//step 2: set byte order

		byteBuffer.order(ByteOrder.nativeOrder());

		//step3 : convert created buffer as float array

		FloatBuffer positionBuffer = byteBuffer.asFloatBuffer();

		//step 4 : fill buffer with vertices array

		positionBuffer.put(pyramidPosition);

		//step 5 : rewind the buffer to 0th index

		positionBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, pyramidPosition.length * 4, positionBuffer, GLES32.GL_STATIC_DRAW);

		GLES32.glVertexAttribPointer(VertexAttributesEnum.AMC_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);

		GLES32.glEnableVertexAttribArray(VertexAttributesEnum.AMC_ATTRIBUTE_POSITION);

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

		//VBO for normal

		GLES32.glGenBuffers(1, vbo_Normal,0);
		
		// Bind with VBO of position

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_Normal[0]);

		//prepare triangle vertices array for glBuffer data

		//step 1 : allocate enough size buffer for triangle vertices

		ByteBuffer byteBuffer_N = ByteBuffer.allocateDirect(pyramidNormals.length * 4);

		//step 2: set byte order

		byteBuffer_N.order(ByteOrder.nativeOrder());

		//step3 : convert created buffer as float array

		FloatBuffer normalBuffer = byteBuffer_N.asFloatBuffer();

		//step 4 : fill buffer with vertices array

		normalBuffer.put(pyramidNormals);

		//step 5 : rewind the buffer to 0th index

		normalBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, pyramidNormals.length * 4, normalBuffer, GLES32.GL_STATIC_DRAW);

		GLES32.glVertexAttribPointer(VertexAttributesEnum.AMC_ATTRIBUTE_NORMALS, 3, GLES32.GL_FLOAT, false, 0, 0);

		GLES32.glEnableVertexAttribArray(VertexAttributesEnum.AMC_ATTRIBUTE_NORMALS);

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);



		//light properties array conversion to float buffer

		//material properties FloatBuffer conversion

		

		
		//step 18 : unbind with VAO
		GLES32.glBindVertexArray(0);

		//material ambient

		ByteBuffer byteBuffer_MA = ByteBuffer.allocateDirect(materialAmbient.length * 4);

		byteBuffer_MA.order(ByteOrder.nativeOrder());

		materialAmbientBuffer = byteBuffer_MA.asFloatBuffer();

		materialAmbientBuffer.put(materialAmbient);

		materialAmbientBuffer.position(0);

		//material diffuse

		ByteBuffer byteBuffer_MD = ByteBuffer.allocateDirect(materialDiffuse.length * 4);

		byteBuffer_MD.order(ByteOrder.nativeOrder());

		materialDiffuseBuffer = byteBuffer_MD.asFloatBuffer();

		materialDiffuseBuffer.put(materialDiffuse);

		materialDiffuseBuffer.position(0);

		//material specular

		ByteBuffer byteBuffer_MS = ByteBuffer.allocateDirect(materialSpecular.length * 4);

		byteBuffer_MS.order(ByteOrder.nativeOrder());

		materialSpecularBuffer = byteBuffer_MS.asFloatBuffer();

		materialSpecularBuffer.put(materialSpecular);

		materialSpecularBuffer.position(0);
		//depth setting

		GLES32.glClearDepthf(1.0f);

		GLES32.glEnable(GLES32.GL_DEPTH_TEST);

		GLES32.glDepthFunc(GLES32.GL_LEQUAL);

		

		//clear color

		GLES32.glClearColor(0.0f,0.0f,0.0f,1.0f);

		lights[0] = new Light();

		//ambient

		lights[0].ambient[0] = 0.0f;
		lights[0].ambient[1] = 0.0f;
		lights[0].ambient[2] = 0.0f;

		//Specular

		lights[0].specular[0] = 1.0f;
		lights[0].specular[1] = 1.0f;
		lights[0].specular[2] = 0.0f;

		//diffuse

		lights[0].diffuse[0] = 1.0f;
		lights[0].diffuse[1] = 1.0f;
		lights[0].diffuse[2] = 0.0f;

		//position

		lights[0].position[0] = -2.0f;
		lights[0].position[1] = 0.0f;
		lights[0].position[2] = 0.0f;
		lights[0].position[3] = 1.0f;

		//light 2

		lights[1] = new Light();

		lights[1].ambient[0] = 0.0f;
		lights[1].ambient[1] = 0.0f;
		lights[1].ambient[2] = 0.0f;

		//Specular

		lights[1].specular[0] = 0.0f;
		lights[1].specular[1] = 1.0f;
		lights[1].specular[2] = 1.0f;

		//diffuse

		lights[1].diffuse[0] = 0.0f;
		lights[1].diffuse[1] = 1.0f;
		lights[1].diffuse[2] = 1.0f;

		//position

		lights[1].position[0] = 2.0f;
		lights[1].position[1] = 0.0f;
		lights[1].position[2] = 0.0f;
		lights[1].position[3] = 1.0f;


		//light ambient 0

		ByteBuffer byteBuffer_LA0 = ByteBuffer.allocateDirect(lights[0].ambient.length * 4);

		byteBuffer_LA0.order(ByteOrder.nativeOrder());

		lightAmbientBuffer0 = byteBuffer_LA0.asFloatBuffer();

		lightAmbientBuffer0.put(lights[0].ambient);

		lightAmbientBuffer0.position(0);

		//light ambient 1

		ByteBuffer byteBuffer_LA1 = ByteBuffer.allocateDirect(lights[1].ambient.length * 4);

		byteBuffer_LA1.order(ByteOrder.nativeOrder());

		lightAmbientBuffer1 = byteBuffer_LA1.asFloatBuffer();

		lightAmbientBuffer1.put(lights[1].ambient);

		lightAmbientBuffer1.position(0);

		//light diffuse 0

		ByteBuffer byteBuffer_LD0 = ByteBuffer.allocateDirect(lights[0].diffuse.length * 4);

		byteBuffer_LD0.order(ByteOrder.nativeOrder());

		lightDiffuseBuffer0 = byteBuffer_LD0.asFloatBuffer();

		lightDiffuseBuffer0.put(lights[0].diffuse);

		lightDiffuseBuffer0.position(0);

		//light diffuse 1

		ByteBuffer byteBuffer_LD1 = ByteBuffer.allocateDirect(lights[1].diffuse.length * 4);

		byteBuffer_LD1.order(ByteOrder.nativeOrder());

		lightDiffuseBuffer1 = byteBuffer_LD1.asFloatBuffer();

		lightDiffuseBuffer1.put(lights[1].diffuse);

		lightDiffuseBuffer1.position(0);

		//light Specular 0

		ByteBuffer byteBuffer_LS0 = ByteBuffer.allocateDirect(lights[0].specular.length * 4);

		byteBuffer_LS0.order(ByteOrder.nativeOrder());

		lightSpecularBuffer0 = byteBuffer_LS0.asFloatBuffer();

		lightSpecularBuffer0.put(lights[0].specular);

		lightSpecularBuffer0.position(0);

		//light Specular 1

		ByteBuffer byteBuffer_LS1 = ByteBuffer.allocateDirect(lights[1].specular.length * 4);

		byteBuffer_LS1.order(ByteOrder.nativeOrder());

		lightSpecularBuffer1 = byteBuffer_LS1.asFloatBuffer();

		lightSpecularBuffer1.put(lights[1].specular);

		lightSpecularBuffer1.position(0);

		//light position 0

		ByteBuffer byteBuffer_LP0 = ByteBuffer.allocateDirect(lights[0].position.length * 4);

		byteBuffer_LP0.order(ByteOrder.nativeOrder());

		lightPositionBuffer0 = byteBuffer_LP0.asFloatBuffer();

		lightPositionBuffer0.put(lights[0].position);

		lightPositionBuffer0.position(0);

		//light position 1

		ByteBuffer byteBuffer_LP1 = ByteBuffer.allocateDirect(lights[1].position.length * 4);

		byteBuffer_LP1.order(ByteOrder.nativeOrder());

		lightPositionBuffer1 = byteBuffer_LP1.asFloatBuffer();

		lightPositionBuffer1.put(lights[1].position);

		lightPositionBuffer1.position(0);
		



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
		//code

		GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);

		//step 1: Use shader program object

		GLES32.glUseProgram(shaderProgramObject);

		//transformations

		float[] modelViewMatrix = new float[16];

		float[] translationMatrix = new float[16];

		float[] rotationMatrix = new float[16];

		float[] viewMatrix = new float[16];

		//set to identity

		Matrix.setIdentityM(modelViewMatrix,0);

		Matrix.setIdentityM(translationMatrix,0);

		Matrix.setIdentityM(rotationMatrix,0);

		Matrix.setIdentityM(viewMatrix,0);

		
		

		//transformation

		Matrix.translateM(translationMatrix,0, 0.0f, 0.0f, -4.5f);

		Matrix.setRotateM(rotationMatrix,0,anglePyramid, 0.0f, 1.0f, 0.0f);

		

		
		Matrix.multiplyMM(modelViewMatrix, 0, translationMatrix, 0,rotationMatrix,0);	

		

		//push above mvp into vertex shader's mvp uniform

		//GLES32.glUniformMatrix4fv(modelViewMatrixUniform, 1, false, modelViewMatrix,0);

		GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelViewMatrix,0);

		GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix,0);

		GLES32.glUniformMatrix4fv(projectionMatrixUniform, 1, false, perspectiveProjectionMatrix,0);

		//convert light and paterial properties arrays to float buffers

	

		GLES32.glBindVertexArray(vao[0]);

		if(bLightingEnabled == true)
		{
			GLES32.glUniform1i(doubleTapnUniform, 1);

			GLES32.glUniform3fv(lightAmbientUniform[0], 1, lightAmbientBuffer0);

			GLES32.glUniform3fv(lightDiffuseUniform[0], 1, lightDiffuseBuffer0);

			GLES32.glUniform3fv(lightSpecularUniform[0], 1,lightSpecularBuffer0);

			GLES32.glUniform4fv(lightPositionUniform[0], 1, lightPositionBuffer0);

			GLES32.glUniform3fv(lightAmbientUniform[1], 1, lightAmbientBuffer1);

			GLES32.glUniform3fv(lightDiffuseUniform[1], 1, lightDiffuseBuffer1);

			GLES32.glUniform3fv(lightSpecularUniform[1], 1, lightSpecularBuffer1);

			GLES32.glUniform4fv(lightPositionUniform[1], 1, lightPositionBuffer1);

			GLES32.glUniform3fv(materialAmbientUniform, 1, materialAmbientBuffer);

			GLES32.glUniform3fv(materialDiffuseUniform, 1, materialDiffuseBuffer);

			GLES32.glUniform3fv(materialSpecularUniform, 1, materialSpecularBuffer);

			GLES32.glUniform1f(materialShininessUniform, materialShininess);
		}

		else
		{
			GLES32.glUniform1i(doubleTapnUniform, 0);
		}

		//step 3: Draw the geometry

		
		

		
		GLES32.glDrawArrays(GLES32.GL_TRIANGLES,0,12);

		


		//step 4: unBind with (VAO)

		GLES32.glBindVertexArray(0);
		

		

		//step 5: Unuse shader program object
		GLES32.glUseProgram(0);


		//Render

		requestRender();

	}

	private void update()
	{
		anglePyramid = anglePyramid - 0.2f;

		if (anglePyramid <= 0.0f)
		{
			anglePyramid = 360.0f;
		}
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

		if(vbo[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo, 0);
			vbo[0] = 0;
		}

		if(vbo_Normal[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo_Normal, 0);
			vbo_Normal[0] = 0;
		}

		


		if(vao[0] > 0)
		{
			GLES32.glDeleteVertexArrays(1, vao, 0);
			vao[0] = 0;
		}
	}

};

