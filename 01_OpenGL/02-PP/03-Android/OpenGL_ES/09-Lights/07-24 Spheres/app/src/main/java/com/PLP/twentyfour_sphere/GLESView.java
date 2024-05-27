package com.PLP.twentyfour_sphere;

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

	//private int[] vao = new int[1];

	//private int[] vbo = new int[1];

	//private int[] vbo_Normal = new int[1];

	//projection related uniforms

	//private int modelViewMatrixUniform;

		private int modelMatrixUniform;

		private int viewMatrixUniform;

	private int projectionMatrixUniform;

	//private int mvpMatrixUniform;

	private float[] perspectiveProjectionMatrix = new float [16];

	//light uniform variables

	private int lightDiffuseUniform = 0;

	private int lightPositionUniform = 0;

	private int lightAmbientUniform = 0;

	private int lightSpecularUniform = 0;

	//material uniform variables

	private int materialDiffuseUniform = 0;

	private int materialAmbientUniform = 0;

	private int materialSpecularUniform = 0;

	private int materialShininessUniform = 0;

	//double tap uniform

	private int doubleTapnUniform = 0;

	//lighting enable variable

	private boolean bLightingEnabled = false;

	//light properties

	private float[] lightPosition = new float[] {0.0f,0.0f,0.0f,1.0f};

	private float [] lightDiffuse = new float[] { 1.0f,1.0f,1.0f,1.0f };

	private float [] lightSpecular = new float[] { 1.0f,1.0f,1.0f,1.0f };

	private float [] lightAmbient = new float[] { 0.0f,0.0f,0.0f,1.0f };


	//material properties

	/*private float[] materialDiffuse = new float[] {0.5f,0.2f,0.5f,1.0f};

	private float[] materialAmbient = new float[] {0.0f,0.0f,0.0f,1.0f};

	private float[] materialSpecular = new float[] {0.7f,0.7f,0.7f,1.0f};

	private float materialShininess = 128.0f;*/

	//light rotation variables

	private float angleForXRotation = 0.0f;
	private float angleForYRotation = 0.0f;
	private float angleForZRotation = 0.0f;

	private float windowWidth;
	private float windowHeight;

	//sphere related variables

	private int[] vao_sphere = new int[1];
    private int[] vbo_sphere_position = new int[1];
    private int[] vbo_sphere_normal = new int[1];
    private int[] vbo_sphere_element = new int[1];

	private int numVertices = 0;

	private int numElements = 0;

	// other variables

	FloatBuffer lightAmbientBuffer;

	FloatBuffer lightDiffuseBuffer;

	FloatBuffer lightSpecularBuffer;

	FloatBuffer lightPositionBuffer;

	FloatBuffer materialAmbientBuffer;

	FloatBuffer materialDiffuseBuffer;

	FloatBuffer materialSpecularBuffer;

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
		singleTap++;

		if(singleTap > 3)
		{
			singleTap = 1;
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
			"uniform mat4 uProjectionMatrix;"+
			"uniform mat4 uModelMatrix;"+
			"uniform mat4 uViewMatrix;"+
			"uniform vec3 uLightAmbient;"+
			"uniform vec3 uLightDiffuse;"+
			"uniform vec3 uLightSpecular;"+
			"uniform vec4 uLightPosition;"+
			"uniform vec3 uMaterialAmbient;"+
			"uniform vec3 uMaterialDiffuse;"+
			"uniform vec3 uMaterialSpecular;"+
			"uniform float uMaterialShininess;"+
			"uniform highp int uDoubleTap;"+
			"out vec3 otransformedNormals;"+
			"out vec3 olightDirection;"+
			"out vec3 oviewerVector;"+
			"void main(void)"+
			"{"+
			"if(uDoubleTap == 1)"+
			"{"+
			    "vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;"+
				"otransformedNormals = (mat3(uViewMatrix * uModelMatrix) * aNormal);"+
				"olightDirection = (vec3 (uLightPosition - eyeCoordinates));"+
				"oviewerVector = (- eyeCoordinates.xyz);"+
			"}"+
			"else"+
			"{"+
			   "otransformedNormals = vec3(0.0, 0.0, 0.0);"+
			   "olightDirection = vec3(0.0, 0.0, 0.0);"+
			   "oviewerVector = vec3(0.0, 0.0, 0.0);"+
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
			"in vec3 olightDirection;"+
			"in vec3 oviewerVector;"+
			"uniform vec3 uLightAmbient;"+
			"uniform vec3 uLightDiffuse;"+
			"uniform vec3 uLightSpecular;"+
			"uniform vec3 uMaterialAmbient;"+
			"uniform vec3 uMaterialDiffuse;"+
			"uniform vec3 uMaterialSpecular;"+
			"uniform float uMaterialShininess;"+
			"uniform highp int uDoubleTap;"+
			"out vec4 FragColor;"+
			"void main(void)"+ 
			"{"+
			"vec3 PhongADSLight;"+
			"if(uDoubleTap == 1)"+
			"{"+
				"vec3 normalizedTransformedNormals = normalize(otransformedNormals);"+
				"vec3 normalizedLightDirection = normalize(olightDirection);"+
				"vec3 normalizedViwerVector = normalize(oviewerVector);"+
				"vec3 ambientLight = uLightAmbient * uMaterialAmbient;"+
				"vec3 diffuseLight = uLightDiffuse * uMaterialDiffuse * max(dot(normalizedLightDirection, normalizedTransformedNormals),0.0);"+
				"vec3 reflectionVector = reflect(-normalizedLightDirection, normalizedTransformedNormals);"+
				"vec3 specularLight = uLightSpecular * uMaterialSpecular * pow(max(dot(reflectionVector, normalizedViwerVector), 0.0),uMaterialShininess);"+
				"PhongADSLight = ambientLight + diffuseLight + specularLight;"+
			"}"+
			"else"+
			"{"+
			     "PhongADSLight = vec3(1.0, 1.0, 1.0);"+
			"}"+   
			"FragColor =  vec4 (PhongADSLight, 1.0);"+
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


		lightAmbientUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uLightAmbient");


		lightDiffuseUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uLightDiffuse");

		lightSpecularUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uLightSpecular");


		lightPositionUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uLightPosition");

		materialAmbientUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uMaterialAmbient");

		materialDiffuseUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uMaterialDiffuse");


		materialSpecularUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uMaterialSpecular");

		materialShininessUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uMaterialShininess");

		doubleTapnUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uDoubleTap");

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


		//light properties array conversion to float buffer

		//light ambient

		ByteBuffer byteBuffer_LA = ByteBuffer.allocateDirect(lightAmbient.length * 4);

		byteBuffer_LA.order(ByteOrder.nativeOrder());

		lightAmbientBuffer = byteBuffer_LA.asFloatBuffer();

		lightAmbientBuffer.put(lightAmbient);

		lightAmbientBuffer.position(0);

		//light diffuse

		ByteBuffer byteBuffer_LD = ByteBuffer.allocateDirect(lightDiffuse.length * 4);

		byteBuffer_LD.order(ByteOrder.nativeOrder());

		lightDiffuseBuffer = byteBuffer_LD.asFloatBuffer();

		lightDiffuseBuffer.put(lightDiffuse);

		lightDiffuseBuffer.position(0);

		//light Specular

		ByteBuffer byteBuffer_LS = ByteBuffer.allocateDirect(lightSpecular.length * 4);

		byteBuffer_LS.order(ByteOrder.nativeOrder());

		lightSpecularBuffer = byteBuffer_LS.asFloatBuffer();

		lightSpecularBuffer.put(lightSpecular);

		lightSpecularBuffer.position(0);

		/*//light position

		ByteBuffer byteBuffer_LP = ByteBuffer.allocateDirect(lightPosition.length * 4);

		byteBuffer_LP.order(ByteOrder.nativeOrder());

		lightPositionBuffer = byteBuffer_LP.asFloatBuffer();

		lightPositionBuffer.put(lightPosition);

		lightPositionBuffer.position(0);*/

		//material properties FloatBuffer conversion

		//material ambient

		/*ByteBuffer byteBuffer_MA = ByteBuffer.allocateDirect(materialAmbient.length * 4);

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

		materialSpecularBuffer.position(0);*/

		
		//step 18 : unbind with VAO
		GLES32.glBindVertexArray(0);
		//depth setting

		GLES32.glClearDepthf(1.0f);

		GLES32.glEnable(GLES32.GL_DEPTH_TEST);

		GLES32.glDepthFunc(GLES32.GL_LEQUAL);

		

		//clear color

		GLES32.glClearColor(0.6f,0.6f,0.6f,1.0f);

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

		windowHeight = height;
		windowWidth = width;

		GLES32.glViewport(0,0,width,height);

		Matrix.perspectiveM(perspectiveProjectionMatrix,0,45.0f,(float)width/(float)height,0.1f,100.0f);
	}

	private void display()
	{
		//code

		//variable declaration
	
		float horizontalDistance = windowWidth / 6;
		float verticalDistance = windowHeight / 7;

		float[] materialAmbient = new float[4];
		float[] materialDiffuse = new float[4];
		float[] materialSpecular = new float[4];

		float shininess;

		GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);

		//step 1: Use shader program object

		GLES32.glUseProgram(shaderProgramObject);

		//(emerald)

		materialAmbient[0] = 0.0215f;
		materialAmbient[1] = 0.1745f;
		materialAmbient[2] = 0.0215f;
		materialAmbient[3] = 1.0f;

	

		materialDiffuse[0] = 0.07568f;
		materialDiffuse[1] = 0.61424f;
		materialDiffuse[2] = 0.07568f;
		materialDiffuse[3] = 1.0f;

	

		materialSpecular[0] = 0.633f;
		materialSpecular[1] = 0.727811f;
		materialSpecular[2] = 0.633f;
		materialSpecular[3] = 1.0f;

	

       float materialShininess = 0.6f * 128.0f;

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

		//light position

		ByteBuffer byteBuffer_LP = ByteBuffer.allocateDirect(lightPosition.length * 4);

		byteBuffer_LP.order(ByteOrder.nativeOrder());

		lightPositionBuffer = byteBuffer_LP.asFloatBuffer();

		lightPositionBuffer.put(lightPosition);

		lightPositionBuffer.position(0);


		//transformations

		float[] modelViewMatrix = new float[16];

		float[] translationMatrix = new float[16];

		float[] modelMatrix = new float[16];

		float[] viewMatrix = new float[16];

		float[] scaleMatrix = new float[16];

		//set to identity

		Matrix.setIdentityM(modelViewMatrix,0);

		Matrix.setIdentityM(translationMatrix,0);

		Matrix.setIdentityM(modelMatrix,0);

		Matrix.setIdentityM(viewMatrix,0);

		Matrix.setIdentityM(scaleMatrix,0);
		
		

		//transformation

		Matrix.translateM(translationMatrix,0, 0.0f, 0.0f, -2.0f);

		//System.arraycopy(translationMatrix, 0, modelMatrix, 0, 16);

		Matrix.scaleM(scaleMatrix,0, 0.9f, 0.9f,0.9f);

		
		Matrix.multiplyMM(modelMatrix, 0, translationMatrix, 0,scaleMatrix,0);	

		

		//push above mvp into vertex shader's mvp uniform

		//GLES32.glUniformMatrix4fv(modelViewMatrixUniform, 1, false, modelViewMatrix,0);

		GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix,0);

		GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix,0);

		GLES32.glUniformMatrix4fv(projectionMatrixUniform, 1, false, perspectiveProjectionMatrix,0);

		//convert light and paterial properties arrays to float buffers

	

		GLES32.glBindVertexArray(vao_sphere[0]);

		if(bLightingEnabled == true)
		{
			GLES32.glUniform1i(doubleTapnUniform, 1);

			GLES32.glUniform3fv(lightAmbientUniform, 1, lightAmbientBuffer);
			GLES32.glUniform3fv(lightDiffuseUniform, 1, lightDiffuseBuffer);
			GLES32.glUniform3fv(lightSpecularUniform, 1, lightSpecularBuffer);
			GLES32.glUniform4fv(lightPositionUniform, 1, lightPositionBuffer);

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

		
		

		//bind element buffer
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);


		GLES32.glViewport(0, (int)(verticalDistance * 5), (int)(windowWidth / 4), (int)(windowHeight / 4));
		//step 3: Draw the geometry

		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);


		//first column second sphere

		GLES32.glViewport(0, (int)(verticalDistance * 4), (int)(windowWidth / 4), (int)(windowHeight / 4));


		materialAmbient[0] = 0.135f;
		materialAmbient[1] = 0.2225f;
		materialAmbient[2] = 0.1575f;
		materialAmbient[3] = 1.0f;

	

		materialDiffuse[0] = 0.54f;
		materialDiffuse[1] = 0.89f;
		materialDiffuse[2] = 0.63f;
		materialDiffuse[3] = 1.0f;



		materialSpecular[0] = 0.316228f;
		materialSpecular[1] = 0.316228f;
		materialSpecular[2] = 0.316228f;
		materialSpecular[3] = 1.0f;

	

		materialShininess = 0.1f * 128.0f;

		//first column second sphere

		byteBuffer_MA = ByteBuffer.allocateDirect(materialAmbient.length * 4);

		byteBuffer_MA.order(ByteOrder.nativeOrder());

		materialAmbientBuffer = byteBuffer_MA.asFloatBuffer();

		materialAmbientBuffer.put(materialAmbient);

		materialAmbientBuffer.position(0);

		//material diffuse

		byteBuffer_MD = ByteBuffer.allocateDirect(materialDiffuse.length * 4);

		byteBuffer_MD.order(ByteOrder.nativeOrder());

		materialDiffuseBuffer = byteBuffer_MD.asFloatBuffer();

		materialDiffuseBuffer.put(materialDiffuse);

		materialDiffuseBuffer.position(0);

		//material specular

		byteBuffer_MS = ByteBuffer.allocateDirect(materialSpecular.length * 4);

		byteBuffer_MS.order(ByteOrder.nativeOrder());

		materialSpecularBuffer = byteBuffer_MS.asFloatBuffer();

		materialSpecularBuffer.put(materialSpecular);

		materialSpecularBuffer.position(0);


		//set to identity

		Matrix.setIdentityM(modelViewMatrix,0);

		Matrix.setIdentityM(translationMatrix,0);

		Matrix.setIdentityM(modelMatrix,0);

		Matrix.setIdentityM(viewMatrix,0);

		
		

		//set to identity

		Matrix.setIdentityM(modelViewMatrix,0);

		Matrix.setIdentityM(translationMatrix,0);

		Matrix.setIdentityM(modelMatrix,0);

		Matrix.setIdentityM(viewMatrix,0);

		Matrix.setIdentityM(scaleMatrix,0);
		
		

		//transformation

		Matrix.translateM(translationMatrix,0, 0.0f, 00.0f, -2.0f);

		//System.arraycopy(translationMatrix, 0, modelMatrix, 0, 16);

		Matrix.scaleM(scaleMatrix,0, 0.9f, 0.9f,0.9f);

		
		Matrix.multiplyMM(modelMatrix, 0, translationMatrix, 0,scaleMatrix,0);	


	

	

		if(bLightingEnabled == true)
		{
			GLES32.glUniform1i(doubleTapnUniform, 1);

			GLES32.glUniform3fv(lightAmbientUniform, 1, lightAmbientBuffer);
			GLES32.glUniform3fv(lightDiffuseUniform, 1, lightDiffuseBuffer);
			GLES32.glUniform3fv(lightSpecularUniform, 1, lightSpecularBuffer);
			GLES32.glUniform4fv(lightPositionUniform, 1, lightPositionBuffer);

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

		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

		//first column third sphere

		GLES32.glViewport(0, (int)(verticalDistance * 3), (int)(windowWidth / 4), (int)(windowHeight / 4));


		materialAmbient[0] = 0.05375f;
		materialAmbient[1] = 0.05f;
		materialAmbient[2] = 0.06625f;
		materialAmbient[3] = 1.0f;

	

		materialDiffuse[0] = 0.18275f;
		materialDiffuse[1] = 0.17f;
		materialDiffuse[2] = 0.22525f;
		materialDiffuse[3] = 1.0f;

	

		materialSpecular[0] = 0.332741f;
		materialSpecular[1] = 0.328634f;
		materialSpecular[2] = 0.346435f;
		materialSpecular[3] = 1.0f;

	

		materialShininess = 0.3f * 128f;

		//first column third sphere

		byteBuffer_MA = ByteBuffer.allocateDirect(materialAmbient.length * 4);

		byteBuffer_MA.order(ByteOrder.nativeOrder());

		materialAmbientBuffer = byteBuffer_MA.asFloatBuffer();

		materialAmbientBuffer.put(materialAmbient);

		materialAmbientBuffer.position(0);

		//material diffuse

		byteBuffer_MD = ByteBuffer.allocateDirect(materialDiffuse.length * 4);

		byteBuffer_MD.order(ByteOrder.nativeOrder());

		materialDiffuseBuffer = byteBuffer_MD.asFloatBuffer();

		materialDiffuseBuffer.put(materialDiffuse);

		materialDiffuseBuffer.position(0);

		//material specular

		byteBuffer_MS = ByteBuffer.allocateDirect(materialSpecular.length * 4);

		byteBuffer_MS.order(ByteOrder.nativeOrder());

		materialSpecularBuffer = byteBuffer_MS.asFloatBuffer();

		materialSpecularBuffer.put(materialSpecular);

		materialSpecularBuffer.position(0);


		//set to identity

		Matrix.setIdentityM(modelViewMatrix,0);

		Matrix.setIdentityM(translationMatrix,0);

		Matrix.setIdentityM(modelMatrix,0);

		Matrix.setIdentityM(viewMatrix,0);

		Matrix.setIdentityM(scaleMatrix,0);
		
		

		//transformation

		Matrix.translateM(translationMatrix,0, 0.0f, 00.0f, -2.0f);

		//System.arraycopy(translationMatrix, 0, modelMatrix, 0, 16);

		Matrix.scaleM(scaleMatrix,0, 0.9f, 0.9f,0.9f);

		
		Matrix.multiplyMM(modelMatrix, 0, translationMatrix, 0,scaleMatrix,0);	


		 GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix,0);

		GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix,0);

		GLES32.glUniformMatrix4fv(projectionMatrixUniform, 1, false, perspectiveProjectionMatrix,0);

	

	

		if(bLightingEnabled == true)
		{
			GLES32.glUniform1i(doubleTapnUniform, 1);

			GLES32.glUniform3fv(lightAmbientUniform, 1, lightAmbientBuffer);
			GLES32.glUniform3fv(lightDiffuseUniform, 1, lightDiffuseBuffer);
			GLES32.glUniform3fv(lightSpecularUniform, 1, lightSpecularBuffer);
			GLES32.glUniform4fv(lightPositionUniform, 1, lightPositionBuffer);

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

		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

		//first column fourth Sphere

		GLES32.glViewport(0, (int)(verticalDistance * 2), (int)(windowWidth / 4), (int)(windowHeight / 4));


		materialAmbient[0] = 0.25f;
		materialAmbient[1] = 0.20725f;
		materialAmbient[2] = 0.20725f;
		materialAmbient[3] = 1.0f;

	

		materialDiffuse[0] = 1.0f;
		materialDiffuse[1] = 0.829f;
		materialDiffuse[2] = 0.829f;
		materialDiffuse[3] = 1.0f;

	

		materialSpecular[0] = 0.296648f;
		materialSpecular[1] = 0.296648f;
		materialSpecular[2] = 0.296648f;
		materialSpecular[3] = 1.0f;



	materialShininess = 0.088f * 128f;

		//first column third sphere

		byteBuffer_MA = ByteBuffer.allocateDirect(materialAmbient.length * 4);

		byteBuffer_MA.order(ByteOrder.nativeOrder());

		materialAmbientBuffer = byteBuffer_MA.asFloatBuffer();

		materialAmbientBuffer.put(materialAmbient);

		materialAmbientBuffer.position(0);

		//material diffuse

		byteBuffer_MD = ByteBuffer.allocateDirect(materialDiffuse.length * 4);

		byteBuffer_MD.order(ByteOrder.nativeOrder());

		materialDiffuseBuffer = byteBuffer_MD.asFloatBuffer();

		materialDiffuseBuffer.put(materialDiffuse);

		materialDiffuseBuffer.position(0);

		//material specular

		byteBuffer_MS = ByteBuffer.allocateDirect(materialSpecular.length * 4);

		byteBuffer_MS.order(ByteOrder.nativeOrder());

		materialSpecularBuffer = byteBuffer_MS.asFloatBuffer();

		materialSpecularBuffer.put(materialSpecular);

		materialSpecularBuffer.position(0);


		//set to identity

		Matrix.setIdentityM(modelViewMatrix,0);

		Matrix.setIdentityM(translationMatrix,0);

		Matrix.setIdentityM(modelMatrix,0);

		Matrix.setIdentityM(viewMatrix,0);

		Matrix.setIdentityM(scaleMatrix,0);
		
		

		//transformation

		Matrix.translateM(translationMatrix,0, 0.0f, 00.0f, -2.0f);

		//System.arraycopy(translationMatrix, 0, modelMatrix, 0, 16);

		Matrix.scaleM(scaleMatrix,0, 0.9f, 0.9f,0.9f);

		
		Matrix.multiplyMM(modelMatrix, 0, translationMatrix, 0,scaleMatrix,0);	



		 GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix,0);

		GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix,0);

		GLES32.glUniformMatrix4fv(projectionMatrixUniform, 1, false, perspectiveProjectionMatrix,0);

	

	

		if(bLightingEnabled == true)
		{
			GLES32.glUniform1i(doubleTapnUniform, 1);

			GLES32.glUniform3fv(lightAmbientUniform, 1, lightAmbientBuffer);
			GLES32.glUniform3fv(lightDiffuseUniform, 1, lightDiffuseBuffer);
			GLES32.glUniform3fv(lightSpecularUniform, 1, lightSpecularBuffer);
			GLES32.glUniform4fv(lightPositionUniform, 1, lightPositionBuffer);

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

		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

		//first column fifth sphere

		GLES32.glViewport(0, (int)(verticalDistance), (int)(windowWidth / 4), (int)(windowHeight / 4));


		materialAmbient[0] = 0.1745f;
		materialAmbient[1] = 0.01175f;
		materialAmbient[2] = 0.01175f;
		materialAmbient[3] = 1.0f;


		materialDiffuse[0] = 0.61424f;
		materialDiffuse[1] = 0.04136f;
		materialDiffuse[2] = 0.04136f;
		materialDiffuse[3] = 1.0f;


		materialSpecular[0] = 0.727811f;
		materialSpecular[1] = 0.626959f;
		materialSpecular[2] = 0.626959f;
		materialSpecular[3] = 1.0f;


		materialShininess = 0.6f * 128f;
		//first column third sphere

		byteBuffer_MA = ByteBuffer.allocateDirect(materialAmbient.length * 4);

		byteBuffer_MA.order(ByteOrder.nativeOrder());

		materialAmbientBuffer = byteBuffer_MA.asFloatBuffer();

		materialAmbientBuffer.put(materialAmbient);

		materialAmbientBuffer.position(0);

		//material diffuse

		byteBuffer_MD = ByteBuffer.allocateDirect(materialDiffuse.length * 4);

		byteBuffer_MD.order(ByteOrder.nativeOrder());

		materialDiffuseBuffer = byteBuffer_MD.asFloatBuffer();

		materialDiffuseBuffer.put(materialDiffuse);

		materialDiffuseBuffer.position(0);

		//material specular

		byteBuffer_MS = ByteBuffer.allocateDirect(materialSpecular.length * 4);

		byteBuffer_MS.order(ByteOrder.nativeOrder());

		materialSpecularBuffer = byteBuffer_MS.asFloatBuffer();

		materialSpecularBuffer.put(materialSpecular);

		materialSpecularBuffer.position(0);


		//set to identity

		Matrix.setIdentityM(modelViewMatrix,0);

		Matrix.setIdentityM(translationMatrix,0);

		Matrix.setIdentityM(modelMatrix,0);

		Matrix.setIdentityM(viewMatrix,0);

		Matrix.setIdentityM(scaleMatrix,0);
		
		

		//transformation

		Matrix.translateM(translationMatrix,0, 0.0f, 00.0f, -2.0f);

		//System.arraycopy(translationMatrix, 0, modelMatrix, 0, 16);

		Matrix.scaleM(scaleMatrix,0, 0.9f, 0.9f,0.9f);

		
		Matrix.multiplyMM(modelMatrix, 0, translationMatrix, 0,scaleMatrix,0);	


		 GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix,0);

		GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix,0);

		GLES32.glUniformMatrix4fv(projectionMatrixUniform, 1, false, perspectiveProjectionMatrix,0);

	

	

		if(bLightingEnabled == true)
		{
			GLES32.glUniform1i(doubleTapnUniform, 1);

			GLES32.glUniform3fv(lightAmbientUniform, 1, lightAmbientBuffer);
			GLES32.glUniform3fv(lightDiffuseUniform, 1, lightDiffuseBuffer);
			GLES32.glUniform3fv(lightSpecularUniform, 1, lightSpecularBuffer);
			GLES32.glUniform4fv(lightPositionUniform, 1, lightPositionBuffer);

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

		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

		//first column sixth sphere

		GLES32.glViewport(0, 0, (int)(windowWidth / 4), (int)(windowHeight / 4));


		materialAmbient[0] = 0.1f;
		materialAmbient[1] = 0.18725f;
		materialAmbient[2] = 0.1745f;
		materialAmbient[3] = 1.0f;


		materialDiffuse[0] = 0.396f;
		materialDiffuse[1] = 0.74151f;
		materialDiffuse[2] = 0.69102f;
		materialDiffuse[3] = 1.0f;


		materialSpecular[0] = 0.297254f;
		materialSpecular[1] = 0.30829f;
		materialSpecular[2] = 0.306678f;
		materialSpecular[3] = 1.0f;

		materialShininess = 0.1f * 128f;
		//first column third sphere

		byteBuffer_MA = ByteBuffer.allocateDirect(materialAmbient.length * 4);

		byteBuffer_MA.order(ByteOrder.nativeOrder());

		materialAmbientBuffer = byteBuffer_MA.asFloatBuffer();

		materialAmbientBuffer.put(materialAmbient);

		materialAmbientBuffer.position(0);

		//material diffuse

		byteBuffer_MD = ByteBuffer.allocateDirect(materialDiffuse.length * 4);

		byteBuffer_MD.order(ByteOrder.nativeOrder());

		materialDiffuseBuffer = byteBuffer_MD.asFloatBuffer();

		materialDiffuseBuffer.put(materialDiffuse);

		materialDiffuseBuffer.position(0);

		//material specular

		byteBuffer_MS = ByteBuffer.allocateDirect(materialSpecular.length * 4);

		byteBuffer_MS.order(ByteOrder.nativeOrder());

		materialSpecularBuffer = byteBuffer_MS.asFloatBuffer();

		materialSpecularBuffer.put(materialSpecular);

		materialSpecularBuffer.position(0);


		//set to identity

		Matrix.setIdentityM(modelViewMatrix,0);

		Matrix.setIdentityM(translationMatrix,0);

		Matrix.setIdentityM(modelMatrix,0);

		Matrix.setIdentityM(viewMatrix,0);

		Matrix.setIdentityM(scaleMatrix,0);
		
		

		//transformation

		Matrix.translateM(translationMatrix,0, 0.0f, 00.0f, -2.0f);

		//System.arraycopy(translationMatrix, 0, modelMatrix, 0, 16);

		Matrix.scaleM(scaleMatrix,0, 0.9f, 0.9f,0.9f);

		
		Matrix.multiplyMM(modelMatrix, 0, translationMatrix, 0,scaleMatrix,0);	


		 GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix,0);

		GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix,0);

		GLES32.glUniformMatrix4fv(projectionMatrixUniform, 1, false, perspectiveProjectionMatrix,0);

	

	

		if(bLightingEnabled == true)
		{
			GLES32.glUniform1i(doubleTapnUniform, 1);

			GLES32.glUniform3fv(lightAmbientUniform, 1, lightAmbientBuffer);
			GLES32.glUniform3fv(lightDiffuseUniform, 1, lightDiffuseBuffer);
			GLES32.glUniform3fv(lightSpecularUniform, 1, lightSpecularBuffer);
			GLES32.glUniform4fv(lightPositionUniform, 1, lightPositionBuffer);

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

		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

		// ************************** second column **********************************************

		//first sphere

		GLES32.glViewport((int)(horizontalDistance), (int)(verticalDistance * 5), (int)(windowWidth / 4), (int)(windowHeight / 4));

		materialAmbient[0] = 0.329412f;
		materialAmbient[1] = 0.223529f;
		materialAmbient[2] = 0.027451f;
		materialAmbient[3] = 1.0f;

		materialDiffuse[0] = 0.780392f;
		materialDiffuse[1] = 0.568627f;
		materialDiffuse[2] = 0.113725f;
		materialDiffuse[3] = 1.0f;


		materialSpecular[0] = 0.992157f;
		materialSpecular[1] = 0.941176f;
		materialSpecular[2] = 0.807843f;
		materialSpecular[3] = 1.0f;

		materialShininess = 0.21794872f * 128f;

		//first column third sphere

		byteBuffer_MA = ByteBuffer.allocateDirect(materialAmbient.length * 4);

		byteBuffer_MA.order(ByteOrder.nativeOrder());

		materialAmbientBuffer = byteBuffer_MA.asFloatBuffer();

		materialAmbientBuffer.put(materialAmbient);

		materialAmbientBuffer.position(0);

		//material diffuse

		byteBuffer_MD = ByteBuffer.allocateDirect(materialDiffuse.length * 4);

		byteBuffer_MD.order(ByteOrder.nativeOrder());

		materialDiffuseBuffer = byteBuffer_MD.asFloatBuffer();

		materialDiffuseBuffer.put(materialDiffuse);

		materialDiffuseBuffer.position(0);

		//material specular

		byteBuffer_MS = ByteBuffer.allocateDirect(materialSpecular.length * 4);

		byteBuffer_MS.order(ByteOrder.nativeOrder());

		materialSpecularBuffer = byteBuffer_MS.asFloatBuffer();

		materialSpecularBuffer.put(materialSpecular);

		materialSpecularBuffer.position(0);


		//set to identity

		Matrix.setIdentityM(modelViewMatrix,0);

		Matrix.setIdentityM(translationMatrix,0);

		Matrix.setIdentityM(modelMatrix,0);

		Matrix.setIdentityM(viewMatrix,0);

		Matrix.setIdentityM(scaleMatrix,0);
		
		

		//transformation

		Matrix.translateM(translationMatrix,0, 0.0f, 00.0f, -2.0f);

		//System.arraycopy(translationMatrix, 0, modelMatrix, 0, 16);

		Matrix.scaleM(scaleMatrix,0, 0.9f, 0.9f,0.9f);

		
		Matrix.multiplyMM(modelMatrix, 0, translationMatrix, 0,scaleMatrix,0);	


		 GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix,0);

		GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix,0);

		GLES32.glUniformMatrix4fv(projectionMatrixUniform, 1, false, perspectiveProjectionMatrix,0);

	

	

		if(bLightingEnabled == true)
		{
			GLES32.glUniform1i(doubleTapnUniform, 1);

			GLES32.glUniform3fv(lightAmbientUniform, 1, lightAmbientBuffer);
			GLES32.glUniform3fv(lightDiffuseUniform, 1, lightDiffuseBuffer);
			GLES32.glUniform3fv(lightSpecularUniform, 1, lightSpecularBuffer);
			GLES32.glUniform4fv(lightPositionUniform, 1, lightPositionBuffer);

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

		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

		//second Sphere

		

		GLES32.glViewport((int)(horizontalDistance), (int)(verticalDistance * 4), (int)(windowWidth / 4), (int)(windowHeight / 4));

		materialAmbient[0] = 0.2125f;
		materialAmbient[1] = 0.1275f;
		materialAmbient[2] = 0.054f;
		materialAmbient[3] = 1.0f;

		materialDiffuse[0] = 0.714f;
		materialDiffuse[1] = 0.568627f;
		materialDiffuse[2] = 0.113725f;
		materialDiffuse[3] = 1.0f;

		materialSpecular[0] = 0.992157f;
		materialSpecular[1] = 0.941176f;
		materialSpecular[2] = 0.807843f;
		materialSpecular[3] = 1.0f;


		materialShininess = 0.21794872f * 128f;

		//first column third sphere

		byteBuffer_MA = ByteBuffer.allocateDirect(materialAmbient.length * 4);

		byteBuffer_MA.order(ByteOrder.nativeOrder());

		materialAmbientBuffer = byteBuffer_MA.asFloatBuffer();

		materialAmbientBuffer.put(materialAmbient);

		materialAmbientBuffer.position(0);

		//material diffuse

		byteBuffer_MD = ByteBuffer.allocateDirect(materialDiffuse.length * 4);

		byteBuffer_MD.order(ByteOrder.nativeOrder());

		materialDiffuseBuffer = byteBuffer_MD.asFloatBuffer();

		materialDiffuseBuffer.put(materialDiffuse);

		materialDiffuseBuffer.position(0);

		//material specular

		byteBuffer_MS = ByteBuffer.allocateDirect(materialSpecular.length * 4);

		byteBuffer_MS.order(ByteOrder.nativeOrder());

		materialSpecularBuffer = byteBuffer_MS.asFloatBuffer();

		materialSpecularBuffer.put(materialSpecular);

		materialSpecularBuffer.position(0);


		//set to identity

		Matrix.setIdentityM(modelViewMatrix,0);

		Matrix.setIdentityM(translationMatrix,0);

		Matrix.setIdentityM(modelMatrix,0);

		Matrix.setIdentityM(viewMatrix,0);

		Matrix.setIdentityM(scaleMatrix,0);
		
		

		//transformation

		Matrix.translateM(translationMatrix,0, 0.0f, 00.0f, -2.0f);

		//System.arraycopy(translationMatrix, 0, modelMatrix, 0, 16);

		Matrix.scaleM(scaleMatrix,0, 0.9f, 0.9f,0.9f);

		
		Matrix.multiplyMM(modelMatrix, 0, translationMatrix, 0,scaleMatrix,0);	


		 GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix,0);

		GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix,0);

		GLES32.glUniformMatrix4fv(projectionMatrixUniform, 1, false, perspectiveProjectionMatrix,0);

	

	

		if(bLightingEnabled == true)
		{
			GLES32.glUniform1i(doubleTapnUniform, 1);

			GLES32.glUniform3fv(lightAmbientUniform, 1, lightAmbientBuffer);
			GLES32.glUniform3fv(lightDiffuseUniform, 1, lightDiffuseBuffer);
			GLES32.glUniform3fv(lightSpecularUniform, 1, lightSpecularBuffer);
			GLES32.glUniform4fv(lightPositionUniform, 1, lightPositionBuffer);

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

		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

		//third Sphere


		GLES32.glViewport((int)(horizontalDistance), (int)(verticalDistance * 3), (int)(windowWidth / 4), (int)(windowHeight / 4));

		materialAmbient[0] = 0.25f;
		materialAmbient[1] = 0.25f;
		materialAmbient[2] = 0.25f;
		materialAmbient[3] = 1.0f;

		materialDiffuse[0] = 0.4f;
		materialDiffuse[1] = 0.4f;
		materialDiffuse[2] = 0.4f;
		materialDiffuse[3] = 1.0f;


		materialSpecular[0] = 0.774597f;
		materialSpecular[1] = 0.774597f;
		materialSpecular[2] = 0.774597f;
		materialSpecular[3] = 1.0f;


		materialShininess = 0.6f * 128f;

		//first column third sphere

		byteBuffer_MA = ByteBuffer.allocateDirect(materialAmbient.length * 4);

		byteBuffer_MA.order(ByteOrder.nativeOrder());

		materialAmbientBuffer = byteBuffer_MA.asFloatBuffer();

		materialAmbientBuffer.put(materialAmbient);

		materialAmbientBuffer.position(0);

		//material diffuse

		byteBuffer_MD = ByteBuffer.allocateDirect(materialDiffuse.length * 4);

		byteBuffer_MD.order(ByteOrder.nativeOrder());

		materialDiffuseBuffer = byteBuffer_MD.asFloatBuffer();

		materialDiffuseBuffer.put(materialDiffuse);

		materialDiffuseBuffer.position(0);

		//material specular

		byteBuffer_MS = ByteBuffer.allocateDirect(materialSpecular.length * 4);

		byteBuffer_MS.order(ByteOrder.nativeOrder());

		materialSpecularBuffer = byteBuffer_MS.asFloatBuffer();

		materialSpecularBuffer.put(materialSpecular);

		materialSpecularBuffer.position(0);


		//set to identity

		Matrix.setIdentityM(modelViewMatrix,0);

		Matrix.setIdentityM(translationMatrix,0);

		Matrix.setIdentityM(modelMatrix,0);

		Matrix.setIdentityM(viewMatrix,0);

		Matrix.setIdentityM(scaleMatrix,0);
		
		

		//transformation

		Matrix.translateM(translationMatrix,0, 0.0f, 00.0f, -2.0f);

		//System.arraycopy(translationMatrix, 0, modelMatrix, 0, 16);

		Matrix.scaleM(scaleMatrix,0, 0.9f, 0.9f,0.9f);

		
		Matrix.multiplyMM(modelMatrix, 0, translationMatrix, 0,scaleMatrix,0);	


		 GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix,0);

		GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix,0);

		GLES32.glUniformMatrix4fv(projectionMatrixUniform, 1, false, perspectiveProjectionMatrix,0);

	

	

		if(bLightingEnabled == true)
		{
			GLES32.glUniform1i(doubleTapnUniform, 1);

			GLES32.glUniform3fv(lightAmbientUniform, 1, lightAmbientBuffer);
			GLES32.glUniform3fv(lightDiffuseUniform, 1, lightDiffuseBuffer);
			GLES32.glUniform3fv(lightSpecularUniform, 1, lightSpecularBuffer);
			GLES32.glUniform4fv(lightPositionUniform, 1, lightPositionBuffer);

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

		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

		//fourth sphere


		GLES32.glViewport((int)(horizontalDistance), (int)(verticalDistance * 2), (int)(windowWidth / 4), (int)(windowHeight / 4));

		materialAmbient[0] = 0.19125f;
		materialAmbient[1] = 0.0735f;
		materialAmbient[2] = 0.0225f;
		materialAmbient[3] = 1.0f;

		materialDiffuse[0] = 0.7038f;
		materialDiffuse[1] = 0.27048f;
		materialDiffuse[2] = 0.0828f;
		materialDiffuse[3] = 1.0f;


		materialSpecular[0] = 0.256777f;
		materialSpecular[1] = 0.137622f;
		materialSpecular[2] = 0.086014f;
		materialSpecular[3] = 1.0f;


	materialShininess = 0.1f * 128.0f;

		//first column third sphere

		byteBuffer_MA = ByteBuffer.allocateDirect(materialAmbient.length * 4);

		byteBuffer_MA.order(ByteOrder.nativeOrder());

		materialAmbientBuffer = byteBuffer_MA.asFloatBuffer();

		materialAmbientBuffer.put(materialAmbient);

		materialAmbientBuffer.position(0);

		//material diffuse

		byteBuffer_MD = ByteBuffer.allocateDirect(materialDiffuse.length * 4);

		byteBuffer_MD.order(ByteOrder.nativeOrder());

		materialDiffuseBuffer = byteBuffer_MD.asFloatBuffer();

		materialDiffuseBuffer.put(materialDiffuse);

		materialDiffuseBuffer.position(0);

		//material specular

		byteBuffer_MS = ByteBuffer.allocateDirect(materialSpecular.length * 4);

		byteBuffer_MS.order(ByteOrder.nativeOrder());

		materialSpecularBuffer = byteBuffer_MS.asFloatBuffer();

		materialSpecularBuffer.put(materialSpecular);

		materialSpecularBuffer.position(0);


		//set to identity

		Matrix.setIdentityM(modelViewMatrix,0);

		Matrix.setIdentityM(translationMatrix,0);

		Matrix.setIdentityM(modelMatrix,0);

		Matrix.setIdentityM(viewMatrix,0);

		Matrix.setIdentityM(scaleMatrix,0);
		
		

		//transformation

		Matrix.translateM(translationMatrix,0, 0.0f, 00.0f, -2.0f);

		//System.arraycopy(translationMatrix, 0, modelMatrix, 0, 16);

		Matrix.scaleM(scaleMatrix,0, 0.9f, 0.9f,0.9f);

		
		Matrix.multiplyMM(modelMatrix, 0, translationMatrix, 0,scaleMatrix,0);	


		 GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix,0);

		GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix,0);

		GLES32.glUniformMatrix4fv(projectionMatrixUniform, 1, false, perspectiveProjectionMatrix,0);

	

	

		if(bLightingEnabled == true)
		{
			GLES32.glUniform1i(doubleTapnUniform, 1);

			GLES32.glUniform3fv(lightAmbientUniform, 1, lightAmbientBuffer);
			GLES32.glUniform3fv(lightDiffuseUniform, 1, lightDiffuseBuffer);
			GLES32.glUniform3fv(lightSpecularUniform, 1, lightSpecularBuffer);
			GLES32.glUniform4fv(lightPositionUniform, 1, lightPositionBuffer);

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

		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

		//fifth sphere

		GLES32.glViewport((int)(horizontalDistance), (int)(verticalDistance), (int)(windowWidth / 4), (int)(windowHeight / 4));

		materialAmbient[0] = 0.24725f;
		materialAmbient[1] = 0.1995f;
		materialAmbient[2] = 0.0745f;
		materialAmbient[3] = 1.0f;

		materialDiffuse[0] = 0.75164f;
		materialDiffuse[1] = 0.60648f;
		materialDiffuse[2] = 0.22648f;
		materialDiffuse[3] = 1.0f;


		materialSpecular[0] = 0.628281f;
		materialSpecular[1] = 0.555802f;
		materialSpecular[2] = 0.366065f;
		materialSpecular[3] = 1.0f;


		materialShininess = 0.4f * 128.0f;


	

		//first column third sphere

		byteBuffer_MA = ByteBuffer.allocateDirect(materialAmbient.length * 4);

		byteBuffer_MA.order(ByteOrder.nativeOrder());

		materialAmbientBuffer = byteBuffer_MA.asFloatBuffer();

		materialAmbientBuffer.put(materialAmbient);

		materialAmbientBuffer.position(0);

		//material diffuse

		byteBuffer_MD = ByteBuffer.allocateDirect(materialDiffuse.length * 4);

		byteBuffer_MD.order(ByteOrder.nativeOrder());

		materialDiffuseBuffer = byteBuffer_MD.asFloatBuffer();

		materialDiffuseBuffer.put(materialDiffuse);

		materialDiffuseBuffer.position(0);

		//material specular

		byteBuffer_MS = ByteBuffer.allocateDirect(materialSpecular.length * 4);

		byteBuffer_MS.order(ByteOrder.nativeOrder());

		materialSpecularBuffer = byteBuffer_MS.asFloatBuffer();

		materialSpecularBuffer.put(materialSpecular);

		materialSpecularBuffer.position(0);


		//set to identity

		Matrix.setIdentityM(modelViewMatrix,0);

		Matrix.setIdentityM(translationMatrix,0);

		Matrix.setIdentityM(modelMatrix,0);

		Matrix.setIdentityM(viewMatrix,0);

		Matrix.setIdentityM(scaleMatrix,0);
		
		

		//transformation

		Matrix.translateM(translationMatrix,0, 0.0f, 00.0f, -2.0f);

		//System.arraycopy(translationMatrix, 0, modelMatrix, 0, 16);

		Matrix.scaleM(scaleMatrix,0, 0.9f, 0.9f,0.9f);

		
		Matrix.multiplyMM(modelMatrix, 0, translationMatrix, 0,scaleMatrix,0);	


		 GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix,0);

		GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix,0);

		GLES32.glUniformMatrix4fv(projectionMatrixUniform, 1, false, perspectiveProjectionMatrix,0);

	

	

		if(bLightingEnabled == true)
		{
			GLES32.glUniform1i(doubleTapnUniform, 1);

			GLES32.glUniform3fv(lightAmbientUniform, 1, lightAmbientBuffer);
			GLES32.glUniform3fv(lightDiffuseUniform, 1, lightDiffuseBuffer);
			GLES32.glUniform3fv(lightSpecularUniform, 1, lightSpecularBuffer);
			GLES32.glUniform4fv(lightPositionUniform, 1, lightPositionBuffer);

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

		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

		//sixth Sphere



		GLES32.glViewport((int)(horizontalDistance), 0, (int)(windowWidth / 4), (int)(windowHeight / 4));

		materialAmbient[0] = 0.19225f;
		materialAmbient[1] = 0.19225f;
		materialAmbient[2] = 0.19225f;
		materialAmbient[3] = 1.0f;

		materialDiffuse[0] = 0.50754f;
		materialDiffuse[1] = 0.50754f;
		materialDiffuse[2] = 0.50754f;
		materialDiffuse[3] = 1.0f;


		materialSpecular[0] = 0.508273f;
		materialSpecular[1] = 0.508273f;
		materialSpecular[2] = 0.508273f;
		materialSpecular[3] = 1.0f;


	materialShininess = 0.4f * 128.0f;



		materialShininess = 0.4f * 128.0f;


	

		//first column third sphere

		byteBuffer_MA = ByteBuffer.allocateDirect(materialAmbient.length * 4);

		byteBuffer_MA.order(ByteOrder.nativeOrder());

		materialAmbientBuffer = byteBuffer_MA.asFloatBuffer();

		materialAmbientBuffer.put(materialAmbient);

		materialAmbientBuffer.position(0);

		//material diffuse

		byteBuffer_MD = ByteBuffer.allocateDirect(materialDiffuse.length * 4);

		byteBuffer_MD.order(ByteOrder.nativeOrder());

		materialDiffuseBuffer = byteBuffer_MD.asFloatBuffer();

		materialDiffuseBuffer.put(materialDiffuse);

		materialDiffuseBuffer.position(0);

		//material specular

		byteBuffer_MS = ByteBuffer.allocateDirect(materialSpecular.length * 4);

		byteBuffer_MS.order(ByteOrder.nativeOrder());

		materialSpecularBuffer = byteBuffer_MS.asFloatBuffer();

		materialSpecularBuffer.put(materialSpecular);

		materialSpecularBuffer.position(0);


		//set to identity

		Matrix.setIdentityM(modelViewMatrix,0);

		Matrix.setIdentityM(translationMatrix,0);

		Matrix.setIdentityM(modelMatrix,0);

		Matrix.setIdentityM(viewMatrix,0);

		Matrix.setIdentityM(scaleMatrix,0);
		
		

		//transformation

		Matrix.translateM(translationMatrix,0, 0.0f, 00.0f, -2.0f);

		//System.arraycopy(translationMatrix, 0, modelMatrix, 0, 16);

		Matrix.scaleM(scaleMatrix,0, 0.9f, 0.9f,0.9f);

		
		Matrix.multiplyMM(modelMatrix, 0, translationMatrix, 0,scaleMatrix,0);	


		 GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix,0);

		GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix,0);

		GLES32.glUniformMatrix4fv(projectionMatrixUniform, 1, false, perspectiveProjectionMatrix,0);

	

	

		if(bLightingEnabled == true)
		{
			GLES32.glUniform1i(doubleTapnUniform, 1);

			GLES32.glUniform3fv(lightAmbientUniform, 1, lightAmbientBuffer);
			GLES32.glUniform3fv(lightDiffuseUniform, 1, lightDiffuseBuffer);
			GLES32.glUniform3fv(lightSpecularUniform, 1, lightSpecularBuffer);
			GLES32.glUniform4fv(lightPositionUniform, 1, lightPositionBuffer);

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

		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

		//******************************** third column ***********************************************

		//first Sphere


		
		GLES32.glViewport((int)(horizontalDistance * 2), (int)(verticalDistance * 5), (int)(windowWidth / 4), (int)(windowHeight / 4));

		materialAmbient[0] = 0.0f;
		materialAmbient[1] = 0.0f;
		materialAmbient[2] = 0.0f;
		materialAmbient[3] = 1.0f;

		materialDiffuse[0] = 0.01f;
		materialDiffuse[1] = 0.01f;
		materialDiffuse[2] = 0.01f;
		materialDiffuse[3] = 1.0f;


		materialSpecular[0] = 0.5f;
		materialSpecular[1] = 0.5f;
		materialSpecular[2] = 0.5f;
		materialSpecular[3] = 1.0f;


		materialShininess = 0.25f * 128.0f;


		//first column third sphere

		byteBuffer_MA = ByteBuffer.allocateDirect(materialAmbient.length * 4);

		byteBuffer_MA.order(ByteOrder.nativeOrder());

		materialAmbientBuffer = byteBuffer_MA.asFloatBuffer();

		materialAmbientBuffer.put(materialAmbient);

		materialAmbientBuffer.position(0);

		//material diffuse

		byteBuffer_MD = ByteBuffer.allocateDirect(materialDiffuse.length * 4);

		byteBuffer_MD.order(ByteOrder.nativeOrder());

		materialDiffuseBuffer = byteBuffer_MD.asFloatBuffer();

		materialDiffuseBuffer.put(materialDiffuse);

		materialDiffuseBuffer.position(0);

		//material specular

		byteBuffer_MS = ByteBuffer.allocateDirect(materialSpecular.length * 4);

		byteBuffer_MS.order(ByteOrder.nativeOrder());

		materialSpecularBuffer = byteBuffer_MS.asFloatBuffer();

		materialSpecularBuffer.put(materialSpecular);

		materialSpecularBuffer.position(0);


		//set to identity

		Matrix.setIdentityM(modelViewMatrix,0);

		Matrix.setIdentityM(translationMatrix,0);

		Matrix.setIdentityM(modelMatrix,0);

		Matrix.setIdentityM(viewMatrix,0);

		Matrix.setIdentityM(scaleMatrix,0);
		
		

		//transformation

		Matrix.translateM(translationMatrix,0, 0.0f, 00.0f, -2.0f);

		//System.arraycopy(translationMatrix, 0, modelMatrix, 0, 16);

		Matrix.scaleM(scaleMatrix,0, 0.9f, 0.9f,0.9f);

		
		Matrix.multiplyMM(modelMatrix, 0, translationMatrix, 0,scaleMatrix,0);	


		 GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix,0);

		GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix,0);

		GLES32.glUniformMatrix4fv(projectionMatrixUniform, 1, false, perspectiveProjectionMatrix,0);

	

	

		if(bLightingEnabled == true)
		{
			GLES32.glUniform1i(doubleTapnUniform, 1);

			GLES32.glUniform3fv(lightAmbientUniform, 1, lightAmbientBuffer);
			GLES32.glUniform3fv(lightDiffuseUniform, 1, lightDiffuseBuffer);
			GLES32.glUniform3fv(lightSpecularUniform, 1, lightSpecularBuffer);
			GLES32.glUniform4fv(lightPositionUniform, 1, lightPositionBuffer);

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

		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

		//second Sphere

		GLES32.glViewport((int)(horizontalDistance * 2), (int)(verticalDistance * 4), (int)(windowWidth / 4), (int)(windowHeight / 4));

		materialAmbient[0] = 0.0f;
		materialAmbient[1] = 0.1f;
		materialAmbient[2] = 0.06f;
		materialAmbient[3] = 1.0f;

		materialDiffuse[0] = 0.0f;
		materialDiffuse[1] = 0.50980392f;
		materialDiffuse[2] = 0.50980392f;
		materialDiffuse[3] = 1.0f;


		materialSpecular[0] = 0.50196078f;
		materialSpecular[1] = 0.50196078f;
		materialSpecular[2] = 0.50196078f;
		materialSpecular[3] = 1.0f;


	    materialShininess = 0.25f * 128.0f;


		

		byteBuffer_MA = ByteBuffer.allocateDirect(materialAmbient.length * 4);

		byteBuffer_MA.order(ByteOrder.nativeOrder());

		materialAmbientBuffer = byteBuffer_MA.asFloatBuffer();

		materialAmbientBuffer.put(materialAmbient);

		materialAmbientBuffer.position(0);

		//material diffuse

		byteBuffer_MD = ByteBuffer.allocateDirect(materialDiffuse.length * 4);

		byteBuffer_MD.order(ByteOrder.nativeOrder());

		materialDiffuseBuffer = byteBuffer_MD.asFloatBuffer();

		materialDiffuseBuffer.put(materialDiffuse);

		materialDiffuseBuffer.position(0);

		//material specular

		byteBuffer_MS = ByteBuffer.allocateDirect(materialSpecular.length * 4);

		byteBuffer_MS.order(ByteOrder.nativeOrder());

		materialSpecularBuffer = byteBuffer_MS.asFloatBuffer();

		materialSpecularBuffer.put(materialSpecular);

		materialSpecularBuffer.position(0);


		//set to identity

		Matrix.setIdentityM(modelViewMatrix,0);

		Matrix.setIdentityM(translationMatrix,0);

		Matrix.setIdentityM(modelMatrix,0);

		Matrix.setIdentityM(viewMatrix,0);

		Matrix.setIdentityM(scaleMatrix,0);
		
		

		//transformation

		Matrix.translateM(translationMatrix,0, 0.0f, 00.0f, -2.0f);

		//System.arraycopy(translationMatrix, 0, modelMatrix, 0, 16);

		Matrix.scaleM(scaleMatrix,0, 0.9f, 0.9f,0.9f);

		
		Matrix.multiplyMM(modelMatrix, 0, translationMatrix, 0,scaleMatrix,0);	


		 GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix,0);

		GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix,0);

		GLES32.glUniformMatrix4fv(projectionMatrixUniform, 1, false, perspectiveProjectionMatrix,0);

	

	

		if(bLightingEnabled == true)
		{
			GLES32.glUniform1i(doubleTapnUniform, 1);

			GLES32.glUniform3fv(lightAmbientUniform, 1, lightAmbientBuffer);
			GLES32.glUniform3fv(lightDiffuseUniform, 1, lightDiffuseBuffer);
			GLES32.glUniform3fv(lightSpecularUniform, 1, lightSpecularBuffer);
			GLES32.glUniform4fv(lightPositionUniform, 1, lightPositionBuffer);

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

		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

		//third Sphere

		GLES32.glViewport((int)(horizontalDistance * 2), (int)(verticalDistance * 3), (int)(windowWidth / 4), (int)(windowHeight / 4));

		materialAmbient[0] = 0.0f;
		materialAmbient[1] = 0.0f;
		materialAmbient[2] = 0.0f;
		materialAmbient[3] = 1.0f;

		materialDiffuse[0] = 0.1f;
		materialDiffuse[1] = 0.35f;
		materialDiffuse[2] = 0.1f;
		materialDiffuse[3] = 1.0f;


		materialSpecular[0] = 0.45f;
		materialSpecular[1] = 0.55f;
		materialSpecular[2] = 0.45f;
		materialSpecular[3] = 1.0f;

		materialShininess = 0.25f * 128.0f;


	


		//first column third sphere

		byteBuffer_MA = ByteBuffer.allocateDirect(materialAmbient.length * 4);

		byteBuffer_MA.order(ByteOrder.nativeOrder());

		materialAmbientBuffer = byteBuffer_MA.asFloatBuffer();

		materialAmbientBuffer.put(materialAmbient);

		materialAmbientBuffer.position(0);

		//material diffuse

		byteBuffer_MD = ByteBuffer.allocateDirect(materialDiffuse.length * 4);

		byteBuffer_MD.order(ByteOrder.nativeOrder());

		materialDiffuseBuffer = byteBuffer_MD.asFloatBuffer();

		materialDiffuseBuffer.put(materialDiffuse);

		materialDiffuseBuffer.position(0);

		//material specular

		byteBuffer_MS = ByteBuffer.allocateDirect(materialSpecular.length * 4);

		byteBuffer_MS.order(ByteOrder.nativeOrder());

		materialSpecularBuffer = byteBuffer_MS.asFloatBuffer();

		materialSpecularBuffer.put(materialSpecular);

		materialSpecularBuffer.position(0);


		//set to identity

		Matrix.setIdentityM(modelViewMatrix,0);

		Matrix.setIdentityM(translationMatrix,0);

		Matrix.setIdentityM(modelMatrix,0);

		Matrix.setIdentityM(viewMatrix,0);

		Matrix.setIdentityM(scaleMatrix,0);
		
		

		//transformation

		Matrix.translateM(translationMatrix,0, 0.0f, 00.0f, -2.0f);

		//System.arraycopy(translationMatrix, 0, modelMatrix, 0, 16);

		Matrix.scaleM(scaleMatrix,0, 0.9f, 0.9f,0.9f);

		
		Matrix.multiplyMM(modelMatrix, 0, translationMatrix, 0,scaleMatrix,0);	


		 GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix,0);

		GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix,0);

		GLES32.glUniformMatrix4fv(projectionMatrixUniform, 1, false, perspectiveProjectionMatrix,0);

	

	

		if(bLightingEnabled == true)
		{
			GLES32.glUniform1i(doubleTapnUniform, 1);

			GLES32.glUniform3fv(lightAmbientUniform, 1, lightAmbientBuffer);
			GLES32.glUniform3fv(lightDiffuseUniform, 1, lightDiffuseBuffer);
			GLES32.glUniform3fv(lightSpecularUniform, 1, lightSpecularBuffer);
			GLES32.glUniform4fv(lightPositionUniform, 1, lightPositionBuffer);

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

		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

		//fourth sphere

		
		GLES32.glViewport((int)(horizontalDistance * 2), (int)(verticalDistance * 2), (int)(windowWidth / 4), (int)(windowHeight / 4));

		materialAmbient[0] = 0.0f;
		materialAmbient[1] = 0.0f;
		materialAmbient[2] = 0.0f;
		materialAmbient[3] = 1.0f;

		materialDiffuse[0] = 0.5f;
		materialDiffuse[1] = 0.0f;
		materialDiffuse[2] = 0.0f;
		materialDiffuse[3] = 1.0f;


		materialSpecular[0] = 0.7f;
		materialSpecular[1] = 0.6f;
		materialSpecular[2] = 0.6f;
		materialSpecular[3] = 1.0f;

		materialShininess = 0.25f * 128.0f;



	


		//first column third sphere

		byteBuffer_MA = ByteBuffer.allocateDirect(materialAmbient.length * 4);

		byteBuffer_MA.order(ByteOrder.nativeOrder());

		materialAmbientBuffer = byteBuffer_MA.asFloatBuffer();

		materialAmbientBuffer.put(materialAmbient);

		materialAmbientBuffer.position(0);

		//material diffuse

		byteBuffer_MD = ByteBuffer.allocateDirect(materialDiffuse.length * 4);

		byteBuffer_MD.order(ByteOrder.nativeOrder());

		materialDiffuseBuffer = byteBuffer_MD.asFloatBuffer();

		materialDiffuseBuffer.put(materialDiffuse);

		materialDiffuseBuffer.position(0);

		//material specular

		byteBuffer_MS = ByteBuffer.allocateDirect(materialSpecular.length * 4);

		byteBuffer_MS.order(ByteOrder.nativeOrder());

		materialSpecularBuffer = byteBuffer_MS.asFloatBuffer();

		materialSpecularBuffer.put(materialSpecular);

		materialSpecularBuffer.position(0);


		//set to identity

		Matrix.setIdentityM(modelViewMatrix,0);

		Matrix.setIdentityM(translationMatrix,0);

		Matrix.setIdentityM(modelMatrix,0);

		Matrix.setIdentityM(viewMatrix,0);

		Matrix.setIdentityM(scaleMatrix,0);
		
		

		//transformation

		Matrix.translateM(translationMatrix,0, 0.0f, 00.0f, -2.0f);

		//System.arraycopy(translationMatrix, 0, modelMatrix, 0, 16);

		Matrix.scaleM(scaleMatrix,0, 0.9f, 0.9f,0.9f);

		
		Matrix.multiplyMM(modelMatrix, 0, translationMatrix, 0,scaleMatrix,0);	


		 GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix,0);

		GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix,0);

		GLES32.glUniformMatrix4fv(projectionMatrixUniform, 1, false, perspectiveProjectionMatrix,0);

	

	

		if(bLightingEnabled == true)
		{
			GLES32.glUniform1i(doubleTapnUniform, 1);

			GLES32.glUniform3fv(lightAmbientUniform, 1, lightAmbientBuffer);
			GLES32.glUniform3fv(lightDiffuseUniform, 1, lightDiffuseBuffer);
			GLES32.glUniform3fv(lightSpecularUniform, 1, lightSpecularBuffer);
			GLES32.glUniform4fv(lightPositionUniform, 1, lightPositionBuffer);

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

		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

		//fifth sphere

		
		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

		//fourth sphere

		
		GLES32.glViewport((int)(horizontalDistance * 2), (int)(verticalDistance), (int)(windowWidth / 4), (int)(windowHeight / 4));

		materialAmbient[0] = 0.0f;
		materialAmbient[1] = 0.0f;
		materialAmbient[2] = 0.0f;
		materialAmbient[3] = 1.0f;

		materialDiffuse[0] = 0.55f;
		materialDiffuse[1] = 0.55f;
		materialDiffuse[2] = 0.55f;
		materialDiffuse[3] = 1.0f;


		materialSpecular[0] = 0.70f;
		materialSpecular[1] = 0.70f;
		materialSpecular[2] = 0.70f;
		materialSpecular[3] = 1.0f;

		materialShininess = 0.25f * 128.0f;


		byteBuffer_MA = ByteBuffer.allocateDirect(materialAmbient.length * 4);

		byteBuffer_MA.order(ByteOrder.nativeOrder());

		materialAmbientBuffer = byteBuffer_MA.asFloatBuffer();

		materialAmbientBuffer.put(materialAmbient);

		materialAmbientBuffer.position(0);

		//material diffuse

		byteBuffer_MD = ByteBuffer.allocateDirect(materialDiffuse.length * 4);

		byteBuffer_MD.order(ByteOrder.nativeOrder());

		materialDiffuseBuffer = byteBuffer_MD.asFloatBuffer();

		materialDiffuseBuffer.put(materialDiffuse);

		materialDiffuseBuffer.position(0);

		//material specular

		byteBuffer_MS = ByteBuffer.allocateDirect(materialSpecular.length * 4);

		byteBuffer_MS.order(ByteOrder.nativeOrder());

		materialSpecularBuffer = byteBuffer_MS.asFloatBuffer();

		materialSpecularBuffer.put(materialSpecular);

		materialSpecularBuffer.position(0);


		//set to identity

		Matrix.setIdentityM(modelViewMatrix,0);

		Matrix.setIdentityM(translationMatrix,0);

		Matrix.setIdentityM(modelMatrix,0);

		Matrix.setIdentityM(viewMatrix,0);

		Matrix.setIdentityM(scaleMatrix,0);
		
		

		//transformation

		Matrix.translateM(translationMatrix,0, 0.0f, 00.0f, -2.0f);

		//System.arraycopy(translationMatrix, 0, modelMatrix, 0, 16);

		Matrix.scaleM(scaleMatrix,0, 0.9f, 0.9f,0.9f);

		
		Matrix.multiplyMM(modelMatrix, 0, translationMatrix, 0,scaleMatrix,0);	


		 GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix,0);

		GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix,0);

		GLES32.glUniformMatrix4fv(projectionMatrixUniform, 1, false, perspectiveProjectionMatrix,0);

	

	

		if(bLightingEnabled == true)
		{
			GLES32.glUniform1i(doubleTapnUniform, 1);

			GLES32.glUniform3fv(lightAmbientUniform, 1, lightAmbientBuffer);
			GLES32.glUniform3fv(lightDiffuseUniform, 1, lightDiffuseBuffer);
			GLES32.glUniform3fv(lightSpecularUniform, 1, lightSpecularBuffer);
			GLES32.glUniform4fv(lightPositionUniform, 1, lightPositionBuffer);

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

		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

		//sixth sphere

		
		GLES32.glViewport((int)(horizontalDistance * 2), 0, (int)(windowWidth / 4), (int)(windowHeight / 4));

		materialAmbient[0] = 0.0f;
		materialAmbient[1] = 0.0f;
		materialAmbient[2] = 0.0f;
		materialAmbient[3] = 1.0f;

		materialDiffuse[0] = 0.5f;
		materialDiffuse[1] = 0.5f;
		materialDiffuse[2] = 0.0f;
		materialDiffuse[3] = 1.0f;


		materialSpecular[0] = 0.60f;
		materialSpecular[1] = 0.60f;
		materialSpecular[2] = 0.50f;
		materialSpecular[3] = 1.0f;

		materialShininess = 0.25f * 128.0f;


		byteBuffer_MA = ByteBuffer.allocateDirect(materialAmbient.length * 4);

		byteBuffer_MA.order(ByteOrder.nativeOrder());

		materialAmbientBuffer = byteBuffer_MA.asFloatBuffer();

		materialAmbientBuffer.put(materialAmbient);

		materialAmbientBuffer.position(0);

		//material diffuse

		byteBuffer_MD = ByteBuffer.allocateDirect(materialDiffuse.length * 4);

		byteBuffer_MD.order(ByteOrder.nativeOrder());

		materialDiffuseBuffer = byteBuffer_MD.asFloatBuffer();

		materialDiffuseBuffer.put(materialDiffuse);

		materialDiffuseBuffer.position(0);

		//material specular

		byteBuffer_MS = ByteBuffer.allocateDirect(materialSpecular.length * 4);

		byteBuffer_MS.order(ByteOrder.nativeOrder());

		materialSpecularBuffer = byteBuffer_MS.asFloatBuffer();

		materialSpecularBuffer.put(materialSpecular);

		materialSpecularBuffer.position(0);


		//set to identity

		Matrix.setIdentityM(modelViewMatrix,0);

		Matrix.setIdentityM(translationMatrix,0);

		Matrix.setIdentityM(modelMatrix,0);

		Matrix.setIdentityM(viewMatrix,0);

		Matrix.setIdentityM(scaleMatrix,0);
		
		

		//transformation

		Matrix.translateM(translationMatrix,0, 0.0f, 00.0f, -2.0f);

		//System.arraycopy(translationMatrix, 0, modelMatrix, 0, 16);

		Matrix.scaleM(scaleMatrix,0, 0.9f, 0.9f,0.9f);

		
		Matrix.multiplyMM(modelMatrix, 0, translationMatrix, 0,scaleMatrix,0);	


		 GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix,0);

		GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix,0);

		GLES32.glUniformMatrix4fv(projectionMatrixUniform, 1, false, perspectiveProjectionMatrix,0);

	

	

		if(bLightingEnabled == true)
		{
			GLES32.glUniform1i(doubleTapnUniform, 1);

			GLES32.glUniform3fv(lightAmbientUniform, 1, lightAmbientBuffer);
			GLES32.glUniform3fv(lightDiffuseUniform, 1, lightDiffuseBuffer);
			GLES32.glUniform3fv(lightSpecularUniform, 1, lightSpecularBuffer);
			GLES32.glUniform4fv(lightPositionUniform, 1, lightPositionBuffer);

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

		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

		// **************************************** fourth column ******************************************


		//first sphere
		
		
		GLES32.glViewport((int)(horizontalDistance * 3), (int)(verticalDistance * 5), (int)(windowWidth / 4), (int)(windowHeight / 4));

		materialAmbient[0] = 0.02f;
		materialAmbient[1] = 0.02f;
		materialAmbient[2] = 0.02f;
		materialAmbient[3] = 1.0f;

		materialDiffuse[0] = 0.01f;
		materialDiffuse[1] = 0.01f;
		materialDiffuse[2] = 0.01f;
		materialDiffuse[3] = 1.0f;


		materialSpecular[0] = 0.4f;
		materialSpecular[1] = 0.4f;
		materialSpecular[2] = 0.4f;
		materialSpecular[3] = 1.0f;

		materialShininess = 0.078125f * 128.0f;


		byteBuffer_MA = ByteBuffer.allocateDirect(materialAmbient.length * 4);

		byteBuffer_MA.order(ByteOrder.nativeOrder());

		materialAmbientBuffer = byteBuffer_MA.asFloatBuffer();

		materialAmbientBuffer.put(materialAmbient);

		materialAmbientBuffer.position(0);

		//material diffuse

		byteBuffer_MD = ByteBuffer.allocateDirect(materialDiffuse.length * 4);

		byteBuffer_MD.order(ByteOrder.nativeOrder());

		materialDiffuseBuffer = byteBuffer_MD.asFloatBuffer();

		materialDiffuseBuffer.put(materialDiffuse);

		materialDiffuseBuffer.position(0);

		//material specular

		byteBuffer_MS = ByteBuffer.allocateDirect(materialSpecular.length * 4);

		byteBuffer_MS.order(ByteOrder.nativeOrder());

		materialSpecularBuffer = byteBuffer_MS.asFloatBuffer();

		materialSpecularBuffer.put(materialSpecular);

		materialSpecularBuffer.position(0);


		//set to identity

		Matrix.setIdentityM(modelViewMatrix,0);

		Matrix.setIdentityM(translationMatrix,0);

		Matrix.setIdentityM(modelMatrix,0);

		Matrix.setIdentityM(viewMatrix,0);

		Matrix.setIdentityM(scaleMatrix,0);
		
		

		//transformation

		Matrix.translateM(translationMatrix,0, 0.0f, 00.0f, -2.0f);

		//System.arraycopy(translationMatrix, 0, modelMatrix, 0, 16);

		Matrix.scaleM(scaleMatrix,0, 0.9f, 0.9f,0.9f);

		
		Matrix.multiplyMM(modelMatrix, 0, translationMatrix, 0,scaleMatrix,0);	


		 GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix,0);

		GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix,0);

		GLES32.glUniformMatrix4fv(projectionMatrixUniform, 1, false, perspectiveProjectionMatrix,0);

	

	

		if(bLightingEnabled == true)
		{
			GLES32.glUniform1i(doubleTapnUniform, 1);

			GLES32.glUniform3fv(lightAmbientUniform, 1, lightAmbientBuffer);
			GLES32.glUniform3fv(lightDiffuseUniform, 1, lightDiffuseBuffer);
			GLES32.glUniform3fv(lightSpecularUniform, 1, lightSpecularBuffer);
			GLES32.glUniform4fv(lightPositionUniform, 1, lightPositionBuffer);

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

		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

		//second Sphere

		
		GLES32.glViewport((int)(horizontalDistance * 3), (int)(verticalDistance * 4), (int)(windowWidth / 4), (int)(windowHeight / 4));

		materialAmbient[0] = 0.0f;
		materialAmbient[1] = 0.05f;
		materialAmbient[2] = 0.05f;
		materialAmbient[3] = 1.0f;

		materialDiffuse[0] = 0.4f;
		materialDiffuse[1] = 0.5f;
		materialDiffuse[2] = 0.5f;
		materialDiffuse[3] = 1.0f;


		materialSpecular[0] = 0.04f;
		materialSpecular[1] = 0.7f;
		materialSpecular[2] = 0.7f;
		materialSpecular[3] = 1.0f;

	materialShininess = 0.078125f * 128.0f;


		byteBuffer_MA = ByteBuffer.allocateDirect(materialAmbient.length * 4);

		byteBuffer_MA.order(ByteOrder.nativeOrder());

		materialAmbientBuffer = byteBuffer_MA.asFloatBuffer();

		materialAmbientBuffer.put(materialAmbient);

		materialAmbientBuffer.position(0);

		//material diffuse

		byteBuffer_MD = ByteBuffer.allocateDirect(materialDiffuse.length * 4);

		byteBuffer_MD.order(ByteOrder.nativeOrder());

		materialDiffuseBuffer = byteBuffer_MD.asFloatBuffer();

		materialDiffuseBuffer.put(materialDiffuse);

		materialDiffuseBuffer.position(0);

		//material specular

		byteBuffer_MS = ByteBuffer.allocateDirect(materialSpecular.length * 4);

		byteBuffer_MS.order(ByteOrder.nativeOrder());

		materialSpecularBuffer = byteBuffer_MS.asFloatBuffer();

		materialSpecularBuffer.put(materialSpecular);

		materialSpecularBuffer.position(0);


		//set to identity

		Matrix.setIdentityM(modelViewMatrix,0);

		Matrix.setIdentityM(translationMatrix,0);

		Matrix.setIdentityM(modelMatrix,0);

		Matrix.setIdentityM(viewMatrix,0);

		Matrix.setIdentityM(scaleMatrix,0);
		
		

		//transformation

		Matrix.translateM(translationMatrix,0, 0.0f, 00.0f, -2.0f);

		//System.arraycopy(translationMatrix, 0, modelMatrix, 0, 16);

		Matrix.scaleM(scaleMatrix,0, 0.9f, 0.9f,0.9f);

		
		Matrix.multiplyMM(modelMatrix, 0, translationMatrix, 0,scaleMatrix,0);	


		 GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix,0);

		GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix,0);

		GLES32.glUniformMatrix4fv(projectionMatrixUniform, 1, false, perspectiveProjectionMatrix,0);

	

	

		if(bLightingEnabled == true)
		{
			GLES32.glUniform1i(doubleTapnUniform, 1);

			GLES32.glUniform3fv(lightAmbientUniform, 1, lightAmbientBuffer);
			GLES32.glUniform3fv(lightDiffuseUniform, 1, lightDiffuseBuffer);
			GLES32.glUniform3fv(lightSpecularUniform, 1, lightSpecularBuffer);
			GLES32.glUniform4fv(lightPositionUniform, 1, lightPositionBuffer);

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

		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

		//third Sphere

		
		GLES32.glViewport((int)(horizontalDistance * 3), (int)(verticalDistance * 3), (int)(windowWidth / 4), (int)(windowHeight / 4));

		materialAmbient[0] = 0.0f;
		materialAmbient[1] = 0.05f;
		materialAmbient[2] = 0.0f;
		materialAmbient[3] = 1.0f;

		materialDiffuse[0] = 0.4f;
		materialDiffuse[1] = 0.5f;
		materialDiffuse[2] = 0.4f;
		materialDiffuse[3] = 1.0f;

		materialSpecular[0] = 0.04f;
		materialSpecular[1] = 0.7f;
		materialSpecular[2] = 0.04f;
		materialSpecular[3] = 1.0f;

		materialShininess = 0.078125f * 128.0f;


		byteBuffer_MA = ByteBuffer.allocateDirect(materialAmbient.length * 4);

		byteBuffer_MA.order(ByteOrder.nativeOrder());

		materialAmbientBuffer = byteBuffer_MA.asFloatBuffer();

		materialAmbientBuffer.put(materialAmbient);

		materialAmbientBuffer.position(0);

		//material diffuse

		byteBuffer_MD = ByteBuffer.allocateDirect(materialDiffuse.length * 4);

		byteBuffer_MD.order(ByteOrder.nativeOrder());

		materialDiffuseBuffer = byteBuffer_MD.asFloatBuffer();

		materialDiffuseBuffer.put(materialDiffuse);

		materialDiffuseBuffer.position(0);

		//material specular

		byteBuffer_MS = ByteBuffer.allocateDirect(materialSpecular.length * 4);

		byteBuffer_MS.order(ByteOrder.nativeOrder());

		materialSpecularBuffer = byteBuffer_MS.asFloatBuffer();

		materialSpecularBuffer.put(materialSpecular);

		materialSpecularBuffer.position(0);


		//set to identity

		Matrix.setIdentityM(modelViewMatrix,0);

		Matrix.setIdentityM(translationMatrix,0);

		Matrix.setIdentityM(modelMatrix,0);

		Matrix.setIdentityM(viewMatrix,0);

		Matrix.setIdentityM(scaleMatrix,0);
		
		

		//transformation

		Matrix.translateM(translationMatrix,0, 0.0f, 00.0f, -2.0f);

		//System.arraycopy(translationMatrix, 0, modelMatrix, 0, 16);

		Matrix.scaleM(scaleMatrix,0, 0.9f, 0.9f,0.9f);

		
		Matrix.multiplyMM(modelMatrix, 0, translationMatrix, 0,scaleMatrix,0);	


		 GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix,0);

		GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix,0);

		GLES32.glUniformMatrix4fv(projectionMatrixUniform, 1, false, perspectiveProjectionMatrix,0);

	

	

		if(bLightingEnabled == true)
		{
			GLES32.glUniform1i(doubleTapnUniform, 1);

			GLES32.glUniform3fv(lightAmbientUniform, 1, lightAmbientBuffer);
			GLES32.glUniform3fv(lightDiffuseUniform, 1, lightDiffuseBuffer);
			GLES32.glUniform3fv(lightSpecularUniform, 1, lightSpecularBuffer);
			GLES32.glUniform4fv(lightPositionUniform, 1, lightPositionBuffer);

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

		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

		//fourth Sphere

		GLES32.glViewport((int)(horizontalDistance * 3), (int)(verticalDistance * 2), (int)(windowWidth / 4), (int)(windowHeight / 4));

		materialAmbient[0] = 0.05f;
		materialAmbient[1] = 0.0f;
		materialAmbient[2] = 0.0f;
		materialAmbient[3] = 1.0f;

		materialDiffuse[0] = 0.5f;
		materialDiffuse[1] = 0.4f;
		materialDiffuse[2] = 0.4f;
		materialDiffuse[3] = 1.0f;

		materialSpecular[0] = 0.7f;
		materialSpecular[1] = 0.04f;
		materialSpecular[2] = 0.04f;
		materialSpecular[3] = 1.0f;

		materialShininess = 0.078125f * 128.0f;


		byteBuffer_MA = ByteBuffer.allocateDirect(materialAmbient.length * 4);

		byteBuffer_MA.order(ByteOrder.nativeOrder());

		materialAmbientBuffer = byteBuffer_MA.asFloatBuffer();

		materialAmbientBuffer.put(materialAmbient);

		materialAmbientBuffer.position(0);

		//material diffuse

		byteBuffer_MD = ByteBuffer.allocateDirect(materialDiffuse.length * 4);

		byteBuffer_MD.order(ByteOrder.nativeOrder());

		materialDiffuseBuffer = byteBuffer_MD.asFloatBuffer();

		materialDiffuseBuffer.put(materialDiffuse);

		materialDiffuseBuffer.position(0);

		//material specular

		byteBuffer_MS = ByteBuffer.allocateDirect(materialSpecular.length * 4);

		byteBuffer_MS.order(ByteOrder.nativeOrder());

		materialSpecularBuffer = byteBuffer_MS.asFloatBuffer();

		materialSpecularBuffer.put(materialSpecular);

		materialSpecularBuffer.position(0);


		//set to identity

		Matrix.setIdentityM(modelViewMatrix,0);

		Matrix.setIdentityM(translationMatrix,0);

		Matrix.setIdentityM(modelMatrix,0);

		Matrix.setIdentityM(viewMatrix,0);

		Matrix.setIdentityM(scaleMatrix,0);
		
		

		//transformation

		Matrix.translateM(translationMatrix,0, 0.0f, 00.0f, -2.0f);

		//System.arraycopy(translationMatrix, 0, modelMatrix, 0, 16);

		Matrix.scaleM(scaleMatrix,0, 0.9f, 0.9f,0.9f);

		
		Matrix.multiplyMM(modelMatrix, 0, translationMatrix, 0,scaleMatrix,0);	


		 GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix,0);

		GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix,0);

		GLES32.glUniformMatrix4fv(projectionMatrixUniform, 1, false, perspectiveProjectionMatrix,0);

	

	

		if(bLightingEnabled == true)
		{
			GLES32.glUniform1i(doubleTapnUniform, 1);

			GLES32.glUniform3fv(lightAmbientUniform, 1, lightAmbientBuffer);
			GLES32.glUniform3fv(lightDiffuseUniform, 1, lightDiffuseBuffer);
			GLES32.glUniform3fv(lightSpecularUniform, 1, lightSpecularBuffer);
			GLES32.glUniform4fv(lightPositionUniform, 1, lightPositionBuffer);

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

		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);


		//fifth sphere

		GLES32.glViewport((int)(horizontalDistance * 3), (int)(verticalDistance * 1), (int)(windowWidth / 4), (int)(windowHeight / 4));

		materialAmbient[0] = 0.05f;
		materialAmbient[1] = 0.05f;
		materialAmbient[2] = 0.05f;
		materialAmbient[3] = 1.0f;

		materialDiffuse[0] = 0.5f;
		materialDiffuse[1] = 0.5f;
		materialDiffuse[2] = 0.5f;
		materialDiffuse[3] = 1.0f;

		materialSpecular[0] = 0.7f;
		materialSpecular[1] = 0.7f;
		materialSpecular[2] = 0.7f;
		materialSpecular[3] = 1.0f;

		materialShininess = 0.078125f * 128.0f;

		byteBuffer_MA = ByteBuffer.allocateDirect(materialAmbient.length * 4);

		byteBuffer_MA.order(ByteOrder.nativeOrder());

		materialAmbientBuffer = byteBuffer_MA.asFloatBuffer();

		materialAmbientBuffer.put(materialAmbient);

		materialAmbientBuffer.position(0);

		//material diffuse

		byteBuffer_MD = ByteBuffer.allocateDirect(materialDiffuse.length * 4);

		byteBuffer_MD.order(ByteOrder.nativeOrder());

		materialDiffuseBuffer = byteBuffer_MD.asFloatBuffer();

		materialDiffuseBuffer.put(materialDiffuse);

		materialDiffuseBuffer.position(0);

		//material specular

		byteBuffer_MS = ByteBuffer.allocateDirect(materialSpecular.length * 4);

		byteBuffer_MS.order(ByteOrder.nativeOrder());

		materialSpecularBuffer = byteBuffer_MS.asFloatBuffer();

		materialSpecularBuffer.put(materialSpecular);

		materialSpecularBuffer.position(0);


		//set to identity

		Matrix.setIdentityM(modelViewMatrix,0);

		Matrix.setIdentityM(translationMatrix,0);

		Matrix.setIdentityM(modelMatrix,0);

		Matrix.setIdentityM(viewMatrix,0);

		Matrix.setIdentityM(scaleMatrix,0);
		
		

		//transformation

		Matrix.translateM(translationMatrix,0, 0.0f, 00.0f, -2.0f);

		//System.arraycopy(translationMatrix, 0, modelMatrix, 0, 16);

		Matrix.scaleM(scaleMatrix,0, 0.9f, 0.9f,0.9f);

		
		Matrix.multiplyMM(modelMatrix, 0, translationMatrix, 0,scaleMatrix,0);	


		 GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix,0);

		GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix,0);

		GLES32.glUniformMatrix4fv(projectionMatrixUniform, 1, false, perspectiveProjectionMatrix,0);

	

	

		if(bLightingEnabled == true)
		{
			GLES32.glUniform1i(doubleTapnUniform, 1);

			GLES32.glUniform3fv(lightAmbientUniform, 1, lightAmbientBuffer);
			GLES32.glUniform3fv(lightDiffuseUniform, 1, lightDiffuseBuffer);
			GLES32.glUniform3fv(lightSpecularUniform, 1, lightSpecularBuffer);
			GLES32.glUniform4fv(lightPositionUniform, 1, lightPositionBuffer);

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

		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

		//sixth sphere

		GLES32.glViewport((int)(horizontalDistance * 3), 0, (int)(windowWidth / 4), (int)(windowHeight / 4));

		materialAmbient[0] = 0.05f;
		materialAmbient[1] = 0.05f;
		materialAmbient[2] = 0.0f;
		materialAmbient[3] = 1.0f;

		materialDiffuse[0] = 0.5f;
		materialDiffuse[1] = 0.5f;
		materialDiffuse[2] = 0.4f;
		materialDiffuse[3] = 1.0f;

		materialSpecular[0] = 0.7f;
		materialSpecular[1] = 0.7f;
		materialSpecular[2] = 0.04f;
		materialSpecular[3] = 1.0f;


		materialShininess = 0.078125f * 128.0f;


		byteBuffer_MA = ByteBuffer.allocateDirect(materialAmbient.length * 4);

		byteBuffer_MA.order(ByteOrder.nativeOrder());

		materialAmbientBuffer = byteBuffer_MA.asFloatBuffer();

		materialAmbientBuffer.put(materialAmbient);

		materialAmbientBuffer.position(0);

		//material diffuse

		byteBuffer_MD = ByteBuffer.allocateDirect(materialDiffuse.length * 4);

		byteBuffer_MD.order(ByteOrder.nativeOrder());

		materialDiffuseBuffer = byteBuffer_MD.asFloatBuffer();

		materialDiffuseBuffer.put(materialDiffuse);

		materialDiffuseBuffer.position(0);

		//material specular

		byteBuffer_MS = ByteBuffer.allocateDirect(materialSpecular.length * 4);

		byteBuffer_MS.order(ByteOrder.nativeOrder());

		materialSpecularBuffer = byteBuffer_MS.asFloatBuffer();

		materialSpecularBuffer.put(materialSpecular);

		materialSpecularBuffer.position(0);


		//set to identity

		Matrix.setIdentityM(modelViewMatrix,0);

		Matrix.setIdentityM(translationMatrix,0);

		Matrix.setIdentityM(modelMatrix,0);

		Matrix.setIdentityM(viewMatrix,0);

		Matrix.setIdentityM(scaleMatrix,0);
		
		

		//transformation

		Matrix.translateM(translationMatrix,0, 0.0f, 00.0f, -2.0f);

		//System.arraycopy(translationMatrix, 0, modelMatrix, 0, 16);

		Matrix.scaleM(scaleMatrix,0, 0.9f, 0.9f,0.9f);

		
		Matrix.multiplyMM(modelMatrix, 0, translationMatrix, 0,scaleMatrix,0);	


		 GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix,0);

		GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix,0);

		GLES32.glUniformMatrix4fv(projectionMatrixUniform, 1, false, perspectiveProjectionMatrix,0);

	

	

		if(bLightingEnabled == true)
		{
			GLES32.glUniform1i(doubleTapnUniform, 1);

			GLES32.glUniform3fv(lightAmbientUniform, 1, lightAmbientBuffer);
			GLES32.glUniform3fv(lightDiffuseUniform, 1, lightDiffuseBuffer);
			GLES32.glUniform3fv(lightSpecularUniform, 1, lightSpecularBuffer);
			GLES32.glUniform4fv(lightPositionUniform, 1, lightPositionBuffer);

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

		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);


		//step 4: unBind with (VAO)

		GLES32.glBindVertexArray(0);
		

		

		//step 5: Unuse shader program object
		GLES32.glUseProgram(0);


		//Render

		requestRender();

	}

	private void update()
	{
		if (singleTap == 1)
		{
			if (angleForXRotation < (2 * Math.PI))   // rotation around X axis
			{
				lightPosition[0] = 0.0f;
				lightPosition[1] = 45.0f * (float)Math.sin(angleForXRotation);
				lightPosition[2] = 45.0f * (float)Math.cos(angleForXRotation);
				lightPosition[3] = 1.0f;

				angleForXRotation = angleForXRotation + 0.01f;
			}

			else
			{
				angleForXRotation = angleForXRotation - (float)(2 * Math.PI);
			}
		
		}

	if (singleTap == 2)   // rotation around Y axis
	{
		if (angleForYRotation < (2 * Math.PI))
		{
			lightPosition[0] = 45.0f * (float)Math.sin(angleForYRotation);
			lightPosition[1] = 0.0f;
			lightPosition[2] = 45.0f * (float)Math.cos(angleForYRotation);
			lightPosition[3] = 1.0f;

			angleForYRotation = angleForYRotation + 0.01f;
		}

		else
		{
			angleForYRotation = angleForYRotation - (float)(2 * Math.PI);
		}

	}

	if (singleTap == 3)   // rotation around Z axis
	{
		if (angleForZRotation < (2 * Math.PI))
		{
			lightPosition[0] = 45.0f * (float)Math.sin(angleForZRotation);
			lightPosition[1] = 45.0f * (float)Math.cos(angleForZRotation);
			lightPosition[2] = 0.0f;
			lightPosition[3] = 1.0f;

			angleForZRotation = angleForZRotation + 0.01f;
		}

		else
		{
			angleForZRotation = angleForZRotation - (float)(2 * Math.PI);
		}

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

