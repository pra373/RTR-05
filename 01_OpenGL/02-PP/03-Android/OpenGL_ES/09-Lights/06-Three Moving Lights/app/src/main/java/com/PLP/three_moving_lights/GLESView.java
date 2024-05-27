package com.PLP.three_moving_lights;

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

	private int shaderProgramObjectPerVertex;

	private int shaderProgramObjectPerFragment;

	//sphere related variables

	private int[] vao_sphere = new int[1];
    private int[] vbo_sphere_position = new int[1];
    private int[] vbo_sphere_normal = new int[1];
    private int[] vbo_sphere_element = new int[1];

	private int numVertices = 0;

	private int numElements = 0;

	

	//private int modelViewMatrixUniform;

	private int modelMatrixUniform;

	private int viewMatrixUniform;

	private int projectionMatrixUniform;

		//light uniform variables

	private int[] lightDiffuseUniform = new int[3];

	private int[] lightPositionUniform = new int[3];

	private int[] lightAmbientUniform = new int[3];

	private int[] lightSpecularUniform = new int[3];

	


	//material uniform variables

	private int materialDiffuseUniform = 0;

	private int materialAmbientUniform = 0;

	private int materialSpecularUniform = 0;

	private int materialShininessUniform = 0;

	//double tap 

	private int doubleTapnUniform = 0;

	//************************************ per fragment uniform variables *******************************

	private int modelMatrixUniform_PF;

	private int viewMatrixUniform_PF;

	private int projectionMatrixUniform_PF;

		//light uniform variables

	private int[] lightDiffuseUniform_PF = new int[3];

	private int[] lightPositionUniform_PF = new int[3];

	private int[] lightAmbientUniform_PF = new int[3];

	private int[] lightSpecularUniform_PF = new int[3];

	


	//material uniform variables

	private int materialDiffuseUniform_PF = 0;

	private int materialAmbientUniform_PF = 0;

	private int materialSpecularUniform_PF = 0;

	private int materialShininessUniform_PF = 0;

	//double tap 

	private int doubleTapnUniform_PF = 0;



	//private int mvpMatrixUniform;

	private float[] perspectiveProjectionMatrix = new float [16];


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

	private Light[] lights = new Light[3];

	private class Lightf
	{
		float [] ambient = new float[3];
		float [] diffuse = new float[3];
		float [] specular = new float[3];
		float [] position = new float[4];

	} 

	private Lightf[] lightsf = new Lightf[3];

	//sphere related variables

	

	// other variables

	FloatBuffer lightAmbientBuffer0;

	FloatBuffer lightAmbientBuffer1;

	FloatBuffer lightAmbientBuffer2;

	FloatBuffer lightDiffuseBuffer0;

	FloatBuffer lightDiffuseBuffer1;

	FloatBuffer lightDiffuseBuffer2;

	FloatBuffer lightSpecularBuffer0;

	
	FloatBuffer lightSpecularBuffer1;

	FloatBuffer lightSpecularBuffer2;



	FloatBuffer lightPositionBuffer0;

	FloatBuffer lightPositionBuffer1;

	FloatBuffer lightPositionBuffer2;

	FloatBuffer materialAmbientBuffer;

	FloatBuffer materialDiffuseBuffer;

	FloatBuffer materialSpecularBuffer;


	//per fragment lighting buffers

	FloatBuffer lightAmbientBuffer_PF0;

	FloatBuffer lightAmbientBuffer_PF1;

	FloatBuffer lightAmbientBuffer_PF2;

	FloatBuffer lightDiffuseBuffer_PF0;

	FloatBuffer lightDiffuseBuffer_PF1;

	FloatBuffer lightDiffuseBuffer_PF2;

	FloatBuffer lightSpecularBuffer_PF0;

	
	FloatBuffer lightSpecularBuffer_PF1;

	FloatBuffer lightSpecularBuffer_PF2;



	FloatBuffer lightPositionBuffer_PF0;

	FloatBuffer lightPositionBuffer_PF1;

	FloatBuffer lightPositionBuffer_PF2;


	//rotation variables

	private float vertexLight0Rotation = 0.0f;

	private float vertexLight1Rotation = 0.0f;

	private float vertexLight2Rotation = 0.0f;

	//rotation variables for per fragment light

	private float fragmentLight0Rotation = 0.0f;

	private float fragmentLight1Rotation = 0.0f;

	private float fragmentLight2Rotation = 0.0f;

	int select_SPO = 0;


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

		
		
		update();
		
		
		
		

		
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
		if(select_SPO == 0)
		{
			select_SPO = 1;
		}
		else
		{
			select_SPO = 0;
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

		final String vertexShaderSourceCodePerVertex = String.format
		(
			"#version 320 es"+
			"\n"+
			"in vec4 aPosition;"+
			"in vec3 aNormal;"+
			"uniform mat4 uModelMatrix;"+
			"uniform mat4 uViewMatrix;"+
			"uniform mat4 uProjectionMatrix;"+
			"uniform vec3 ulightAmbient[3];"+
			"uniform vec3 ulightDiffuse[3];"+
			"uniform vec3 ulightSpecular[3];"+
			"uniform vec4 ulightPosition[3];"+
			"uniform vec3 uMaterialAmbient;"+
			"uniform vec3 uMaterialDiffuse;"+
			"uniform vec3 uMaterialSpecular;"+
			"uniform float uMaterialShininess;"+
			"uniform highp int uDoubleTap;"+
			"out vec3 oPhongADSLight;"+
			"void main(void)"+
			"{"+
			"if(uDoubleTap == 1)"+
			"{"+
				"vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;"+
				"vec3 transformedNormals = normalize(mat3(uViewMatrix * uModelMatrix) * aNormal);"+
				"vec3 lightAmbient[3];"+
				"vec3 lightDirection[3];"+
				"vec3 lightDiffuse[3];"+
				"vec3 reflectionVector[3];"+
				"vec3 lightSpecular[3];"+
				"for(int i = 0; i < 3; i++)"+
				"{"+
					"lightAmbient[i] = ulightAmbient[i] * uMaterialAmbient;"+
					"lightDirection[i] = normalize(vec3 (ulightPosition[i] - eyeCoordinates));"+
					"lightDiffuse[i] = ulightDiffuse[i] * uMaterialDiffuse * max(dot(lightDirection[i], transformedNormals),0.0);"+
					"reflectionVector[i] = reflect(-lightDirection[i], transformedNormals);"+
					"vec3 viewerVector = normalize (- eyeCoordinates.xyz);"+
					"lightSpecular[i] = ulightSpecular[i] * uMaterialSpecular * pow(max(dot(reflectionVector[i], viewerVector), 0.0),uMaterialShininess);"+
					"oPhongADSLight = oPhongADSLight + lightAmbient[i] + lightDiffuse[i] + lightSpecular[i];"+
			   "}"+
			"}"+
			"else"+
			"{"+
				"oPhongADSLight = vec3( 0.0, 0.0, 0.0);"+
			"}"+
			"gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;"+
			"}"
		);

		int vertexShaderObjectPerVertex = GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);

		GLES32.glShaderSource(vertexShaderObjectPerVertex,vertexShaderSourceCodePerVertex);

		GLES32.glCompileShader(vertexShaderObjectPerVertex);

		int[] shaderCompileStatus = new int[1];

		int[] infoLogLength = new int[1];

		String infoLog = null;

		GLES32.glGetShaderiv(vertexShaderObjectPerVertex,GLES32.GL_COMPILE_STATUS,shaderCompileStatus,0);

		if(shaderCompileStatus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetShaderiv(vertexShaderObjectPerVertex, GLES32.GL_INFO_LOG_LENGTH, infoLogLength,0);

			if(infoLogLength[0] > 0)
			{
				infoLog = GLES32.glGetShaderInfoLog(vertexShaderObjectPerVertex);

				System.out.println("PLP: Vertex Shader per vertex Compilation Error : " + infoLog);

				uninitialize();

				System.exit(0);
			}
		}

		//fragment shader

		final String fragmentShaderSourceCodePerVertex = String.format
		(
			"#version 320 es"+ 
			"\n"+
			"precision highp float;"+
			"in vec3 oPhongADSLight;"+
			"uniform highp int uDoubleTap;"+
			"out vec4 FragColor;"+
			"void main(void)"+ 
			"{"+
			"if(uDoubleTap == 1)"+
			"{"+
				"FragColor = vec4(oPhongADSLight,1.0);"+
			"}"+
			"else"+
			"{"+
			    "FragColor = vec4(1.0, 1.0, 1.0, 1.0);"+
			"}"+                    
			"}"
		);

		int fragmentShaderObjectPerVertex = GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);

		GLES32.glShaderSource(fragmentShaderObjectPerVertex,fragmentShaderSourceCodePerVertex);

		GLES32.glCompileShader(fragmentShaderObjectPerVertex);

		shaderCompileStatus[0] = 0;

		infoLogLength[0] = 0;

		infoLog = null;

		GLES32.glGetShaderiv(fragmentShaderObjectPerVertex,GLES32.GL_COMPILE_STATUS,shaderCompileStatus,0);

		if(shaderCompileStatus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetShaderiv(fragmentShaderObjectPerVertex, GLES32.GL_INFO_LOG_LENGTH, infoLogLength,0);

			if(infoLogLength[0] > 0)
			{
				infoLog = GLES32.glGetShaderInfoLog(fragmentShaderObjectPerVertex);

				System.out.println("PLP: Fragment Shader Per vertex Compilation Error : " + infoLog);

				uninitialize();

				System.exit(0);
			}
		}

		//shader Program
		
		shaderProgramObjectPerVertex = GLES32.glCreateProgram();

		GLES32.glAttachShader(shaderProgramObjectPerVertex, vertexShaderObjectPerVertex);
		GLES32.glAttachShader(shaderProgramObjectPerVertex, fragmentShaderObjectPerVertex);

		GLES32.glBindAttribLocation(shaderProgramObjectPerVertex,VertexAttributesEnum.AMC_ATTRIBUTE_POSITION,"aPosition");

		GLES32.glBindAttribLocation(shaderProgramObjectPerVertex,VertexAttributesEnum.AMC_ATTRIBUTE_NORMALS,"aNormal");

		GLES32.glLinkProgram(shaderProgramObjectPerVertex);

		int[] programLinkStatus = new int[1];

		infoLogLength[0] = 0;

		infoLog = null;

		

		GLES32.glGetProgramiv(shaderProgramObjectPerVertex, GLES32.GL_LINK_STATUS, programLinkStatus,0);

		if(programLinkStatus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetProgramiv(shaderProgramObjectPerVertex, GLES32.GL_INFO_LOG_LENGTH, infoLogLength,0);

			if(infoLogLength[0] > 0)
			{
				//infoLog = GLES32.glGetProgramInfoLog(shaderProgramObject, infoLogLength, 0);

				infoLog = GLES32.glGetProgramInfoLog(shaderProgramObjectPerVertex);

				System.out.println("PLP:Shader Program object per vertex Compilation Error : " + infoLog);

				uninitialize();

				System.exit(0);
			}
		}

		//post linking uniform

		 modelMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex, "uModelMatrix");

	 viewMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex, "uViewMatrix");

	 projectionMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex, "uProjectionMatrix");

	 lightAmbientUniform[0] = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex, "ulightAmbient[0]");

	 lightDiffuseUniform[0] = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex, "ulightDiffuse[0]");

	 lightSpecularUniform[0] = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex, "ulightSpecular[0]");

	 lightPositionUniform[0] = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex, "ulightPosition[0]");

	 lightAmbientUniform[1] = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex, "ulightAmbient[1]");

	 lightDiffuseUniform[1] = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex, "ulightDiffuse[1]");

	 lightSpecularUniform[1] = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex, "ulightSpecular[1]");

	 lightPositionUniform[1] = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex, "ulightPosition[1]");

	 lightAmbientUniform[2] = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex, "ulightAmbient[2]");

	 lightDiffuseUniform[2] = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex, "ulightDiffuse[2]");

	 lightSpecularUniform[2] = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex, "ulightSpecular[2]");

	 lightPositionUniform[2] = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex, "ulightPosition[2]");


	 materialAmbientUniform = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex, "uMaterialAmbient");

	 materialDiffuseUniform = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex, "uMaterialDiffuse");


	 materialSpecularUniform = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex, "uMaterialSpecular");

	 materialShininessUniform = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex, "uMaterialShininess");

	doubleTapnUniform = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex, "uDoubleTap");

// ********************************* per fragment lighting shaders ********************************************************
	
	//vertex shader 

		final String vertexShaderSourceCodePerFragment = String.format
		(
			"#version 320 es"+
			"\n"+
			"in vec4 aPosition;"+
			"in vec3 aNormal;"+
			"uniform mat4 uModelMatrix;"+
			"uniform mat4 uViewMatrix;"+
			"uniform mat4 uProjectionMatrix;"+
			"uniform vec4 ulightPosition[3];"+
			"uniform highp int uDoubleTap;"+
			"out vec3 otransformedNormals;"+
			"out vec3 oviewerVector;"+
			"out vec3 olightDirection[3];"+
			"void main(void)"+
			"{"+
			"if(uDoubleTap == 1)"+
			"{"+
				"vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;"+
				"otransformedNormals = (mat3(uViewMatrix * uModelMatrix) * aNormal);"+	
				"for(int i = 0; i < 3; i++)"+
				"{"	+	
					"olightDirection[i] = (vec3(ulightPosition[i] - eyeCoordinates));"+	
					"oviewerVector = (-eyeCoordinates.xyz);"+
				"}"+
			"}"+
			"else"+
			"{"+
			"}"+
			"gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;"+
			"}"
		);

		int vertexShaderObjectPerFragment = GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);

		GLES32.glShaderSource(vertexShaderObjectPerFragment,vertexShaderSourceCodePerFragment);

		GLES32.glCompileShader(vertexShaderObjectPerFragment);

		shaderCompileStatus[0] = 0;

		infoLogLength[0] = 0;

		infoLog = null;

		GLES32.glGetShaderiv(vertexShaderObjectPerFragment,GLES32.GL_COMPILE_STATUS,shaderCompileStatus,0);

		if(shaderCompileStatus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetShaderiv(vertexShaderObjectPerFragment, GLES32.GL_INFO_LOG_LENGTH, infoLogLength,0);

			if(infoLogLength[0] > 0)
			{
				infoLog = GLES32.glGetShaderInfoLog(vertexShaderObjectPerFragment);

				System.out.println("PLP: Vertex Shader per fragment Compilation Error : " + infoLog);

				uninitialize();

				System.exit(0);
			}
		}

		//fragment shader

		final String fragmentShaderSourceCodePerFragment = String.format
		(
			"#version 320 es"+ 
			"\n"+
			"precision highp float;"+
			"in vec3 otransformedNormals;"+
			"in vec3 olightDirection[3];"+
			"in vec3 oviewerVector;"+
			"uniform mat4 uModelMatrix;"+
			"uniform mat4 uViewMatrix;"+
			"uniform mat4 uProjectionMatrix;"+
			"uniform vec3 ulightAmbient[3];"+
			"uniform vec3 ulightDiffuse[3];"+
			"uniform vec3 ulightSpecular[3];"+
			"uniform vec3 uMaterialAmbient;"+
			"uniform vec3 uMaterialDiffuse;"+
			"uniform vec3 uMaterialSpecular;"+
			"uniform float uMaterialShininess;"+
			"uniform highp int uDoubleTap;"+
			"out vec4 FragColor;"+
			"void main(void)"+
			"{"+
			"vec3 PhongADSLight = vec3(0.0,0.0,0.0);"+ // local variable
			"if(uDoubleTap == 1)"+
			"{"+
			"vec3 normalizedLightDirection[3];"+
			"vec3 normalizedTransformedNormals = normalize(otransformedNormals);"+
			"vec3 normalizedViwerVector = normalize(oviewerVector);"+
			"vec3 lightAmbient[3];"+
			"vec3 lightDiffuse[3];"+
			"vec3 reflectionVector[3];"+
			"vec3 lightSpecular[3];"+
			"for(int i = 0; i < 3; i++)"+
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

		int fragmentShaderObjectPerFragment = GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);

		GLES32.glShaderSource(fragmentShaderObjectPerFragment,fragmentShaderSourceCodePerFragment);

		GLES32.glCompileShader(fragmentShaderObjectPerFragment);

		shaderCompileStatus[0] = 0;

		infoLogLength[0] = 0;

		infoLog = null;

		GLES32.glGetShaderiv(fragmentShaderObjectPerFragment,GLES32.GL_COMPILE_STATUS,shaderCompileStatus,0);

		if(shaderCompileStatus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetShaderiv(fragmentShaderObjectPerFragment, GLES32.GL_INFO_LOG_LENGTH, infoLogLength,0);

			if(infoLogLength[0] > 0)
			{
				infoLog = GLES32.glGetShaderInfoLog(fragmentShaderObjectPerFragment);

				System.out.println("PLP: Fragment Shader per fragment Compilation Error : " + infoLog);

				uninitialize();

				System.exit(0);
			}
		}

		//shader Program
		
		shaderProgramObjectPerFragment = GLES32.glCreateProgram();

		GLES32.glAttachShader(shaderProgramObjectPerFragment, vertexShaderObjectPerFragment);
		GLES32.glAttachShader(shaderProgramObjectPerFragment, fragmentShaderObjectPerFragment);

		GLES32.glBindAttribLocation(shaderProgramObjectPerFragment,VertexAttributesEnum.AMC_ATTRIBUTE_POSITION,"aPosition");

		GLES32.glBindAttribLocation(shaderProgramObjectPerFragment,VertexAttributesEnum.AMC_ATTRIBUTE_NORMALS,"aNormal");

		GLES32.glLinkProgram(shaderProgramObjectPerFragment);

		programLinkStatus[0] = 0;

		infoLogLength[0] = 0;

		infoLog = null;

		

		GLES32.glGetProgramiv(shaderProgramObjectPerFragment, GLES32.GL_LINK_STATUS, programLinkStatus,0);

		if(programLinkStatus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetProgramiv(shaderProgramObjectPerFragment, GLES32.GL_INFO_LOG_LENGTH, infoLogLength,0);

			if(infoLogLength[0] > 0)
			{
				//infoLog = GLES32.glGetProgramInfoLog(shaderProgramObject, infoLogLength, 0);

				infoLog = GLES32.glGetProgramInfoLog(shaderProgramObjectPerFragment);

				System.out.println("PLP:Shader Program object per fragment Compilation Error : " + infoLog);

				uninitialize();

				System.exit(0);
			}
		}

			

		//post linking uniform

		 modelMatrixUniform_PF = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment, "uModelMatrix");

		 viewMatrixUniform_PF = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment, "uViewMatrix");

		 projectionMatrixUniform_PF = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment, "uProjectionMatrix");

		 lightAmbientUniform_PF[0] = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment, "ulightAmbient[0]");

		 lightDiffuseUniform_PF[0] = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment, "ulightDiffuse[0]");

		 lightSpecularUniform_PF[0] = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment, "ulightSpecular[0]");

		 lightPositionUniform_PF[0] = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment, "ulightPosition[0]");

		 lightAmbientUniform_PF[1] = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment, "ulightAmbient[1]");

		 lightDiffuseUniform_PF[1] = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment, "ulightDiffuse[1]");

		 lightSpecularUniform_PF[1] = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment, "ulightSpecular[1]");

		 lightPositionUniform_PF[1] = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment, "ulightPosition[1]");

		  lightAmbientUniform_PF[2] = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment, "ulightAmbient[2]");

		 lightDiffuseUniform_PF[2] = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment, "ulightDiffuse[2]");

		 lightSpecularUniform_PF[2] = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment, "ulightSpecular[2]");

		 lightPositionUniform_PF[2] = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment, "ulightPosition[2]");


		materialAmbientUniform_PF = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment, "uMaterialAmbient");

		materialDiffuseUniform_PF = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment, "uMaterialDiffuse");


		materialSpecularUniform_PF = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment, "uMaterialSpecular");

		materialShininessUniform_PF = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment, "uMaterialShininess");

		doubleTapnUniform_PF = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment, "uDoubleTap");
		
		
		//geometry coordinates

		Sphere sphere=new Sphere();
        float sphere_vertices[]=new float[1146];
        float sphere_normals[]=new float[1146];
        float sphere_textures[]=new float[764];
        short sphere_elements[]=new short[2280];

        sphere.getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);

        numVertices = sphere.getNumberOfSphereVertices();

        numElements = sphere.getNumberOfSphereElements();

		//vao

		GLES32.glGenVertexArrays(1,vao_sphere,0);

		//Bind with VAO

		GLES32.glBindVertexArray(vao_sphere[0]);

		//VBO for position

		GLES32.glGenBuffers(1, vbo_sphere_position,0);
		
		// Bind with VBO of position

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_sphere_position[0]);

		//prepare triangle vertices array for glBuffer data

		//step 1 : allocate enough size buffer for triangle vertices

		ByteBuffer byteBuffer = ByteBuffer.allocateDirect(sphere_vertices.length * 4);

		//step 2: set byte order

		byteBuffer.order(ByteOrder.nativeOrder());

		//step3 : convert created buffer as float array

		FloatBuffer verticesBuffer = byteBuffer.asFloatBuffer();

		//step 4 : fill buffer with vertices array

		verticesBuffer.put(sphere_vertices);

		//step 5 : rewind the buffer to 0th index

		verticesBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, sphere_vertices.length * 4, verticesBuffer, GLES32.GL_STATIC_DRAW);

		GLES32.glVertexAttribPointer(VertexAttributesEnum.AMC_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);

		GLES32.glEnableVertexAttribArray(VertexAttributesEnum.AMC_ATTRIBUTE_POSITION);

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);



		//buffer for normal

		

		GLES32.glGenBuffers(1, vbo_sphere_normal,0);
		
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_sphere_normal[0]);

	
		ByteBuffer byteBuffer_N = ByteBuffer.allocateDirect(sphere_normals.length * 4);

		
		byteBuffer_N.order(ByteOrder.nativeOrder());

		FloatBuffer normalBuffer = byteBuffer_N.asFloatBuffer();

		

		normalBuffer.put(sphere_normals);

	

		normalBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, sphere_normals.length * 4, normalBuffer, GLES32.GL_STATIC_DRAW);

		GLES32.glVertexAttribPointer(VertexAttributesEnum.AMC_ATTRIBUTE_NORMALS, 3, GLES32.GL_FLOAT, false, 0, 0);

		GLES32.glEnableVertexAttribArray(VertexAttributesEnum.AMC_ATTRIBUTE_NORMALS);

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);



		//vbo for elements

		GLES32.glGenBuffers(1, vbo_sphere_element,0);
		
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);

	
		ByteBuffer byteBuffer_E = ByteBuffer.allocateDirect(sphere_elements.length * 2);

		
		byteBuffer_E.order(ByteOrder.nativeOrder());

		ShortBuffer elementBuffer = byteBuffer_E.asShortBuffer();

		

		elementBuffer.put(sphere_elements);

	

		elementBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ELEMENT_ARRAY_BUFFER, sphere_elements.length * 2, elementBuffer, GLES32.GL_STATIC_DRAW);

		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, 0);
		
		//step 18 : unbind with VAO
		GLES32.glBindVertexArray(0);

		//material properties FloatBuffer conversion

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
		lights[0].specular[1] = 0.0f;
		lights[0].specular[2] = 0.0f;

		//diffuse

		lights[0].diffuse[0] = 1.0f;
		lights[0].diffuse[1] = 0.0f;
		lights[0].diffuse[2] = 0.0f;

		//position

		lights[0].position[0] = 1.0f;
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
		lights[1].specular[1] = 0.0f;
		lights[1].specular[2] = 1.0f;

		//diffuse

		lights[1].diffuse[0] = 0.0f;
		lights[1].diffuse[1] = 0.0f;
		lights[1].diffuse[2] = 1.0f;

		//position

		lights[1].position[0] = -1.0f;
		lights[1].position[1] = 0.0f;
		lights[1].position[2] = 0.0f;
		lights[1].position[3] = 1.0f;

		//light 3

		lights[2] = new Light();

		lights[2].ambient[0] = 0.0f;
		lights[2].ambient[1] = 0.0f;
		lights[2].ambient[2] = 0.0f;

		//Specular

		lights[2].specular[0] = 0.0f;
		lights[2].specular[1] = 1.0f;
		lights[2].specular[2] = 0.0f;

		//diffuse

		lights[2].diffuse[0] = 0.0f;
		lights[2].diffuse[1] = 1.0f;
		lights[2].diffuse[2] = 0.0f;

		//position

		lights[2].position[0] = 0.0f;
		lights[2].position[1] = 0.0f;
		lights[2].position[2] = 0.0f;
		lights[2].position[3] = 1.0f;

		//fragment lighting lights

		lightsf[0] = new Lightf();

		//ambient

		lightsf[0].ambient[0] = 0.0f;
		lightsf[0].ambient[1] = 0.0f;
		lightsf[0].ambient[2] = 0.0f;

		//Specular

		lightsf[0].specular[0] = 0.0f;
		lightsf[0].specular[1] = 1.0f;
		lightsf[0].specular[2] = 1.0f;

		//diffuse

		lightsf[0].diffuse[0] = 0.0f;
		lightsf[0].diffuse[1] = 1.0f;
		lightsf[0].diffuse[2] = 1.0f;

		//position

		lightsf[0].position[0] = 1.0f;
		lightsf[0].position[1] = 0.0f;
		lightsf[0].position[2] = 0.0f;
		lightsf[0].position[3] = 1.0f;

		//light 2

		lightsf[1] = new Lightf();

		lightsf[1].ambient[0] = 0.0f;
		lightsf[1].ambient[1] = 0.0f;
		lightsf[1].ambient[2] = 0.0f;

		//Specular

		lightsf[1].specular[0] = 1.0f;
		lightsf[1].specular[1] = 0.0f;
		lightsf[1].specular[2] = 0.0f;

		//diffuse

		lightsf[1].diffuse[0] = 1.0f;
		lightsf[1].diffuse[1] = 0.0f;
		lightsf[1].diffuse[2] = 0.0f;

		//position

		lightsf[1].position[0] = -1.0f;
		lightsf[1].position[1] = 0.0f;
		lightsf[1].position[2] = 0.0f;
		lightsf[1].position[3] = 1.0f;

		//light 3

		lightsf[2] = new Lightf();

		lightsf[2].ambient[0] = 0.0f;
		lightsf[2].ambient[1] = 0.0f;
		lightsf[2].ambient[2] = 0.0f;

		//Specular

		lightsf[2].specular[0] = 1.0f;
		lightsf[2].specular[1] = 1.0f;
		lightsf[2].specular[2] = 0.0f;

		//diffuse

		lightsf[2].diffuse[0] = 1.0f;
		lightsf[2].diffuse[1] = 1.0f;
		lightsf[2].diffuse[2] = 0.0f;

		//position

		lightsf[2].position[0] = 0.0f;
		lightsf[2].position[1] = 0.0f;
		lightsf[2].position[2] = 0.0f;
		lightsf[2].position[3] = 1.0f;



//************************************ per vertex lighting floatbuffer conversion **********************************
		//light ambient 0

		ByteBuffer byteBuffer_LA0 = ByteBuffer.allocateDirect(lights[0].ambient.length * 4);

		byteBuffer_LA0.order(ByteOrder.nativeOrder());

		lightAmbientBuffer0 = byteBuffer_LA0.asFloatBuffer();

		lightAmbientBuffer0.put(lights[0].ambient);

		lightAmbientBuffer0.position(0);

		//light ambient 2

		ByteBuffer byteBuffer_LA2 = ByteBuffer.allocateDirect(lights[2].ambient.length * 4);

		byteBuffer_LA2.order(ByteOrder.nativeOrder());

		lightAmbientBuffer2 = byteBuffer_LA2.asFloatBuffer();

		lightAmbientBuffer2.put(lights[2].ambient);

		lightAmbientBuffer2.position(0);

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

		//light diffuse 2

		ByteBuffer byteBuffer_LD2 = ByteBuffer.allocateDirect(lights[2].diffuse.length * 4);

		byteBuffer_LD2.order(ByteOrder.nativeOrder());

		lightDiffuseBuffer2 = byteBuffer_LD2.asFloatBuffer();

		lightDiffuseBuffer2.put(lights[2].diffuse);

		lightDiffuseBuffer2.position(0);

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

		//light Specular 2

		ByteBuffer byteBuffer_LS2 = ByteBuffer.allocateDirect(lights[2].specular.length * 4);

		byteBuffer_LS2.order(ByteOrder.nativeOrder());

		lightSpecularBuffer2 = byteBuffer_LS2.asFloatBuffer();

		lightSpecularBuffer2.put(lights[2].specular);

		lightSpecularBuffer2.position(0);


//************************************** per fragment lighting float buffer conversion ******************************
	
	//light ambient 0

		ByteBuffer byteBuffer_LA_PF0 = ByteBuffer.allocateDirect(lightsf[0].ambient.length * 4);

		byteBuffer_LA_PF0.order(ByteOrder.nativeOrder());

		lightAmbientBuffer_PF0 = byteBuffer_LA_PF0.asFloatBuffer();

		lightAmbientBuffer_PF0.put(lightsf[0].ambient);

		lightAmbientBuffer_PF0.position(0);

		//light ambient 2

		ByteBuffer byteBuffer_LA_PF2 = ByteBuffer.allocateDirect(lightsf[2].ambient.length * 4);

		byteBuffer_LA_PF2.order(ByteOrder.nativeOrder());

		lightAmbientBuffer_PF2 = byteBuffer_LA_PF2.asFloatBuffer();

		lightAmbientBuffer_PF2.put(lightsf[2].ambient);

		lightAmbientBuffer_PF2.position(0);

		//light ambient 1

		ByteBuffer byteBuffer_LA_PF1 = ByteBuffer.allocateDirect(lightsf[1].ambient.length * 4);

		byteBuffer_LA_PF1.order(ByteOrder.nativeOrder());

		lightAmbientBuffer_PF1 = byteBuffer_LA_PF1.asFloatBuffer();

		lightAmbientBuffer_PF1.put(lightsf[1].ambient);

		lightAmbientBuffer_PF1.position(0);

		//light diffuse 0

		ByteBuffer byteBuffer_LD_PF0 = ByteBuffer.allocateDirect(lightsf[0].diffuse.length * 4);

		byteBuffer_LD_PF0.order(ByteOrder.nativeOrder());

		lightDiffuseBuffer_PF0 = byteBuffer_LD_PF0.asFloatBuffer();

		lightDiffuseBuffer_PF0.put(lightsf[0].diffuse);

		lightDiffuseBuffer_PF0.position(0);

		//light diffuse 1

		ByteBuffer byteBuffer_LD_PF1 = ByteBuffer.allocateDirect(lightsf[1].diffuse.length * 4);

		byteBuffer_LD_PF1.order(ByteOrder.nativeOrder());

		lightDiffuseBuffer_PF1 = byteBuffer_LD_PF1.asFloatBuffer();

		lightDiffuseBuffer_PF1.put(lightsf[1].diffuse);

		lightDiffuseBuffer_PF1.position(0);

		//light diffuse 2

		ByteBuffer byteBuffer_LD_PF2 = ByteBuffer.allocateDirect(lightsf[2].diffuse.length * 4);

		byteBuffer_LD_PF2.order(ByteOrder.nativeOrder());

		lightDiffuseBuffer_PF2 = byteBuffer_LD_PF2.asFloatBuffer();

		lightDiffuseBuffer_PF2.put(lightsf[2].diffuse);

		lightDiffuseBuffer_PF2.position(0);

		//light Specular 0

		ByteBuffer byteBuffer_LS_PF0 = ByteBuffer.allocateDirect(lightsf[0].specular.length * 4);

		byteBuffer_LS_PF0.order(ByteOrder.nativeOrder());

		lightSpecularBuffer_PF0 = byteBuffer_LS_PF0.asFloatBuffer();

		lightSpecularBuffer_PF0.put(lightsf[0].specular);

		lightSpecularBuffer_PF0.position(0);

		//light Specular 1

		ByteBuffer byteBuffer_LS_PF1 = ByteBuffer.allocateDirect(lightsf[1].specular.length * 4);

		byteBuffer_LS_PF1.order(ByteOrder.nativeOrder());

		lightSpecularBuffer_PF1 = byteBuffer_LS_PF1.asFloatBuffer();

		lightSpecularBuffer_PF1.put(lightsf[1].specular);

		lightSpecularBuffer_PF1.position(0);

		//light Specular 2

		ByteBuffer byteBuffer_LS_PF2 = ByteBuffer.allocateDirect(lightsf[2].specular.length * 4);

		byteBuffer_LS_PF2.order(ByteOrder.nativeOrder());

		lightSpecularBuffer_PF2 = byteBuffer_LS_PF2.asFloatBuffer();

		lightSpecularBuffer_PF2.put(lightsf[2].specular);

		lightSpecularBuffer_PF2.position(0);



		


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

		if(select_SPO == 1)
		{
			GLES32.glUseProgram(shaderProgramObjectPerFragment);
		}

		else
		{
			GLES32.glUseProgram(shaderProgramObjectPerVertex);
		}
		
		

		
		

		//GLES32.glUseProgram(shaderProgramObjectPerVertex);

		

		//transformations

		float[] modelViewMatrix = new float[16];

		float[] translationMatrix = new float[16];

		float[] modelMatrix = new float[16];

		float[] viewMatrix = new float[16];

		//set to identity

		Matrix.setIdentityM(modelViewMatrix,0);

		Matrix.setIdentityM(translationMatrix,0);

		Matrix.setIdentityM(modelMatrix,0);

		Matrix.setIdentityM(viewMatrix,0);

		
		

		//transformation

		Matrix.translateM(translationMatrix,0, 0.0f, 0.0f, -2.0f);

		System.arraycopy(translationMatrix, 0, modelMatrix, 0, 16);

		
		//Matrix.multiplyMM(modelViewMatrix, 0, translationMatrix, 0,modelViewMatrix,0);	

		

		//push above mvp into vertex shader's mvp uniform

		//GLES32.glUniformMatrix4fv(modelViewMatrixUniform, 1, false, modelViewMatrix,0);

		if(select_SPO == 0)
		{
			GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix,0);

			GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix,0);

			GLES32.glUniformMatrix4fv(projectionMatrixUniform, 1, false, perspectiveProjectionMatrix,0);
		}

		else
		{
			GLES32.glUniformMatrix4fv(modelMatrixUniform_PF, 1, false, modelMatrix,0);

			GLES32.glUniformMatrix4fv(viewMatrixUniform_PF, 1, false, viewMatrix,0);

			GLES32.glUniformMatrix4fv(projectionMatrixUniform_PF, 1, false, perspectiveProjectionMatrix,0);
		}

		

		//convert light and paterial properties arrays to float buffers

	// ************************************ per vertex lighting buffers *******************************

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

		//light position 1

		ByteBuffer byteBuffer_LP2 = ByteBuffer.allocateDirect(lights[2].position.length * 4);

		byteBuffer_LP2.order(ByteOrder.nativeOrder());

		lightPositionBuffer2 = byteBuffer_LP2.asFloatBuffer();

		lightPositionBuffer2.put(lights[2].position);

		lightPositionBuffer2.position(0);
		

    //********************************** per fragment lighting buffers *********************************

	//light position 0

		ByteBuffer byteBuffer_LP_PF0 = ByteBuffer.allocateDirect(lightsf[0].position.length * 4);

		byteBuffer_LP_PF0.order(ByteOrder.nativeOrder());

		lightPositionBuffer_PF0 = byteBuffer_LP_PF0.asFloatBuffer();

		lightPositionBuffer_PF0.put(lightsf[0].position);

		lightPositionBuffer_PF0.position(0);

		//light position 1

		ByteBuffer byteBuffer_LP_PF1 = ByteBuffer.allocateDirect(lightsf[1].position.length * 4);

		byteBuffer_LP_PF1.order(ByteOrder.nativeOrder());

		lightPositionBuffer_PF1 = byteBuffer_LP_PF1.asFloatBuffer();

		lightPositionBuffer_PF1.put(lightsf[1].position);

		lightPositionBuffer_PF1.position(0);

		//light position 1

		ByteBuffer byteBuffer_LP_PF2 = ByteBuffer.allocateDirect(lightsf[2].position.length * 4);

		byteBuffer_LP_PF2.order(ByteOrder.nativeOrder());

		lightPositionBuffer_PF2 = byteBuffer_LP_PF2.asFloatBuffer();

		lightPositionBuffer_PF2.put(lightsf[2].position);

		lightPositionBuffer_PF2.position(0);
		
	

		GLES32.glBindVertexArray(vao_sphere[0]);

		if(bLightingEnabled == true)
		{
			if(select_SPO == 0)
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


				GLES32.glUniform3fv(lightAmbientUniform[2], 1, lightAmbientBuffer2);

				GLES32.glUniform3fv(lightDiffuseUniform[2], 1, lightDiffuseBuffer2);

				GLES32.glUniform3fv(lightSpecularUniform[2], 1, lightSpecularBuffer2);

				GLES32.glUniform4fv(lightPositionUniform[2], 1, lightPositionBuffer2);


				GLES32.glUniform3fv(materialAmbientUniform, 1, materialAmbientBuffer);

				GLES32.glUniform3fv(materialDiffuseUniform, 1, materialDiffuseBuffer);

				GLES32.glUniform3fv(materialSpecularUniform, 1, materialSpecularBuffer);

				GLES32.glUniform1f(materialShininessUniform, materialShininess);
			}

			else
			{
				GLES32.glUniform1i(doubleTapnUniform_PF, 1);

				GLES32.glUniform3fv(lightAmbientUniform_PF[0], 1, lightAmbientBuffer_PF0);

				GLES32.glUniform3fv(lightDiffuseUniform_PF[0], 1, lightDiffuseBuffer_PF0);

				GLES32.glUniform3fv(lightSpecularUniform_PF[0], 1,lightSpecularBuffer_PF0);

				GLES32.glUniform4fv(lightPositionUniform_PF[0], 1, lightPositionBuffer_PF0);

				GLES32.glUniform3fv(lightAmbientUniform_PF[1], 1, lightAmbientBuffer_PF1);

				GLES32.glUniform3fv(lightDiffuseUniform_PF[1], 1, lightDiffuseBuffer_PF1);

				GLES32.glUniform3fv(lightSpecularUniform_PF[1], 1, lightSpecularBuffer_PF1);

				GLES32.glUniform4fv(lightPositionUniform_PF[1], 1, lightPositionBuffer_PF1);


				GLES32.glUniform3fv(lightAmbientUniform_PF[2], 1, lightAmbientBuffer_PF2);

				GLES32.glUniform3fv(lightDiffuseUniform_PF[2], 1, lightDiffuseBuffer_PF2);

				GLES32.glUniform3fv(lightSpecularUniform_PF[2], 1, lightSpecularBuffer_PF2);

				GLES32.glUniform4fv(lightPositionUniform_PF[2], 1, lightPositionBuffer_PF2);


				GLES32.glUniform3fv(materialAmbientUniform_PF, 1, materialAmbientBuffer);

				GLES32.glUniform3fv(materialDiffuseUniform_PF, 1, materialDiffuseBuffer);

				GLES32.glUniform3fv(materialSpecularUniform_PF, 1, materialSpecularBuffer);

				GLES32.glUniform1f(materialShininessUniform_PF, materialShininess);
			}
			
				
		}

		else
		{
			GLES32.glUniform1i(doubleTapnUniform, 0);
			GLES32.glUniform1i(doubleTapnUniform_PF, 0);
		}

		//step 3: Draw the geometry

		
		//bind element buffer
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);


		//step 3: Draw the geometry

		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);


		//step 4: unBind with (VAO)

		GLES32.glBindVertexArray(0);
		

		
		

		


		//step 4: unBind with (VAO)

		GLES32.glBindVertexArray(0);
		

		

		//step 5: Unuse shader program object
		GLES32.glUseProgram(0);


		//Render

		requestRender();

	}

	private void update()
	{
		if (bLightingEnabled == true)
		{
			if(select_SPO == 0)
			{
				if(vertexLight0Rotation < (2.0f * Math.PI))
				{
					//light 0    //x rotation
					lights[0].position[0] = 0.0f;
					lights[0].position[1] = 2.0f * (float)Math.sin(vertexLight0Rotation);
					lights[0].position[2] = (-2.0f) + (2.0f * (float)Math.cos(vertexLight0Rotation));
					lights[0].position[3] = 1.0f;

					//light 1   //y rotation

					lights[1].position[0] = 2.0f * (float)Math.sin(vertexLight1Rotation);
					lights[1].position[1] = 0.0f;
					lights[1].position[2] = (-2.0f) + (2.0f * (float)Math.cos(vertexLight1Rotation));
					lights[1].position[3] = 1.0f;

					//light 2   //Z rotation

					lights[2].position[0] = 2.0f * (float)Math.sin(vertexLight2Rotation);
					lights[2].position[1] = 2.0f * (float)Math.cos(vertexLight2Rotation);
					lights[2].position[2] = 0.0f;
					lights[2].position[3] = 1.0f;

					vertexLight0Rotation = vertexLight0Rotation + 0.008f;
					vertexLight1Rotation = vertexLight1Rotation + 0.008f;
					vertexLight2Rotation = vertexLight2Rotation + 0.008f;
				}

				else
				{
					vertexLight0Rotation = 0.0f;
					vertexLight1Rotation = 0.0f;
					vertexLight2Rotation = 0.0f;
				}
			}

			else
			{
				if(fragmentLight0Rotation < (2.0f * Math.PI))
				{
					//light 0    //x rotation
					lightsf[0].position[0] = 0.0f;
					lightsf[0].position[1] = 2.0f * (float)Math.sin(fragmentLight0Rotation);
					lightsf[0].position[2] = (-2.0f) + (2.0f * (float)Math.cos(fragmentLight0Rotation));
					lightsf[0].position[3] = 1.0f;

					//light 1   //y rotation

					lightsf[1].position[0] = 2.0f * (float)Math.sin(fragmentLight1Rotation);
					lightsf[1].position[1] = 0.0f;
					lightsf[1].position[2] = (-2.0f) + (2.0f * (float)Math.cos(fragmentLight1Rotation));
					lightsf[1].position[3] = 1.0f;

					//light 2   //Z rotation

					lightsf[2].position[0] = 2.0f * (float)Math.sin(fragmentLight2Rotation);
					lightsf[2].position[1] = 2.0f * (float)Math.cos(fragmentLight2Rotation);
					lightsf[2].position[2] = 0.0f;
					lightsf[2].position[3] = 1.0f;

					fragmentLight0Rotation = fragmentLight0Rotation + 0.008f;
					fragmentLight1Rotation = fragmentLight1Rotation + 0.008f;
					fragmentLight2Rotation = fragmentLight2Rotation + 0.008f;
				}

				else
				{
					fragmentLight0Rotation = 0.0f;
					fragmentLight1Rotation = 0.0f;
					fragmentLight2Rotation = 0.0f;
				}
			}

			
			
			
		}
	}

	private void uninitialize()
	{
		//code

		if(shaderProgramObjectPerVertex > 0)
		{
			GLES32.glUseProgram(shaderProgramObjectPerVertex);
			int[] retVal = new int[1];

			//step 2 : get number of attached shaders
			GLES32.glGetProgramiv(shaderProgramObjectPerVertex, GLES32.GL_ATTACHED_SHADERS, retVal,0);

			if(retVal[0] > 0)
			{
				int numAttachedShaders = retVal[0];

				int[] shaderObjects = new int[numAttachedShaders];

				GLES32.glGetAttachedShaders(shaderProgramObjectPerVertex, numAttachedShaders, retVal,0, shaderObjects,0);

				for (int i = 0; i < numAttachedShaders ; i++)
				{
					GLES32.glDetachShader(shaderProgramObjectPerVertex, shaderObjects[i]);
					GLES32.glDeleteShader(shaderObjects[i]);
					shaderObjects[i] = 0;
				}

			}

			GLES32.glUseProgram(0);
			GLES32.glDeleteProgram(shaderProgramObjectPerVertex);
			shaderProgramObjectPerVertex = 0;

			


		}

		if(vbo_sphere_position[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo_sphere_position, 0);
			vbo_sphere_position[0] = 0;
		}

		if(vbo_sphere_normal[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo_sphere_normal, 0);
			vbo_sphere_normal[0] = 0;
		}

		if(vbo_sphere_element[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo_sphere_element, 0);
			vbo_sphere_element[0] = 0;
		}


		if(vao_sphere[0] > 0)
		{
			GLES32.glDeleteVertexArrays(1, vao_sphere, 0);
			vao_sphere[0] = 0;
		}
	}

};

