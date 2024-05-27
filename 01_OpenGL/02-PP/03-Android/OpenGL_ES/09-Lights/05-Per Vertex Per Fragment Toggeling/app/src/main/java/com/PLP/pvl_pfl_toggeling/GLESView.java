package com.PLP.pvl_pfl_toggeling;

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

	private float[] lightPosition = new float[] {100.0f,100.0f,100.0f,1.0f};

	private float [] lightDiffuse = new float[] { 1.0f,1.0f,1.0f,1.0f };

	private float [] lightSpecular = new float[] { 1.0f,1.0f,1.0f,1.0f };

	private float [] lightAmbient = new float[] { 0.1f,0.1f,0.1f,1.0f };


	//material properties

	private float[] materialDiffuse = new float[] {0.5f,0.2f,0.7f,1.0f};

	private float[] materialAmbient = new float[] {0.0f,0.0f,0.0f,1.0f};

	private float[] materialSpecular = new float[] {0.7f,0.7f,0.7f,1.0f};

	private float materialShininess = 128.0f;

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

	//shader program object selector variable

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
			"out vec3 oPhongADSLight;"+
			"void main(void)"+
			"{"+
			"if(uDoubleTap == 1)"+
			"{"+
			    "vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;"+
				"vec3 transformedNormals = normalize(mat3(uViewMatrix * uModelMatrix) * aNormal);"+
				"vec3 lightDirection = normalize(vec3(uLightPosition - eyeCoordinates));"+  
				"vec3 reflectionVector = reflect(-lightDirection,transformedNormals);"+
				"vec3 viewerVector = normalize(-eyeCoordinates . xyz);"+
				"vec3 ambientLight = uLightAmbient * uMaterialAmbient;"+
				"vec3 diffuseLight = uLightDiffuse * uMaterialDiffuse * max(dot(lightDirection,transformedNormals), 0.0);"+
				"vec3 lightSpecular = uLightSpecular * uMaterialSpecular * pow(max(dot(reflectionVector,viewerVector), 0.0), uMaterialShininess);"+
				"oPhongADSLight = ambientLight + diffuseLight + lightSpecular;"+
			"}"+
			"else"+
			"{"+
			   "oPhongADSLight = vec3(0.0, 0.0, 0.0);"+
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

				System.out.println("PLP: Fragment Shader per vertex Compilation Error : " + infoLog);

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

		lightAmbientUniform = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex, "uLightAmbient");

		lightDiffuseUniform = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex, "uLightDiffuse");

		lightSpecularUniform = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex, "uLightSpecular");

		lightPositionUniform = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex, "uLightPosition");

		materialAmbientUniform = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex, "uMaterialAmbient");

		materialDiffuseUniform = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex, "uMaterialDiffuse");

		materialSpecularUniform = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex, "uMaterialSpecular");

		materialShininessUniform = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex, "uMaterialShininess");

		doubleTapnUniform = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex, "uDoubleTap");


// ***************************************** per fragment lighting shaders ****************************************** 

	   //vertex shader    
		
		final String vertexShaderSourceCodePerFragment = String.format
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

		//shader Program per fragment
		
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

		//post linking uniform per fragment

		modelMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment, "uModelMatrix");



		viewMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment, "uViewMatrix");

		projectionMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment, "uProjectionMatrix");


		lightAmbientUniform = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment, "uLightAmbient");


		lightDiffuseUniform = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment, "uLightDiffuse");

		lightSpecularUniform = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment, "uLightSpecular");


		lightPositionUniform = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment, "uLightPosition");

		materialAmbientUniform = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment, "uMaterialAmbient");

		materialDiffuseUniform = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment, "uMaterialDiffuse");


		materialSpecularUniform = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment, "uMaterialSpecular");

		materialShininessUniform = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment, "uMaterialShininess");

		doubleTapnUniform = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment, "uDoubleTap");



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

		//light position

		ByteBuffer byteBuffer_LP = ByteBuffer.allocateDirect(lightPosition.length * 4);

		byteBuffer_LP.order(ByteOrder.nativeOrder());

		lightPositionBuffer = byteBuffer_LP.asFloatBuffer();

		lightPositionBuffer.put(lightPosition);

		lightPositionBuffer.position(0);

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

		
		//step 18 : unbind with VAO
		GLES32.glBindVertexArray(0);
		//depth setting

		GLES32.glClearDepthf(1.0f);

		GLES32.glEnable(GLES32.GL_DEPTH_TEST);

		GLES32.glDepthFunc(GLES32.GL_LEQUAL);

		

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

		//code

		if(shaderProgramObjectPerFragment > 0)
		{
			GLES32.glUseProgram(shaderProgramObjectPerFragment);
			int[] retVal = new int[1];

			//step 2 : get number of attached shaders
			GLES32.glGetProgramiv(shaderProgramObjectPerFragment, GLES32.GL_ATTACHED_SHADERS, retVal,0);

			if(retVal[0] > 0)
			{
				int numAttachedShaders = retVal[0];

				int[] shaderObjects = new int[numAttachedShaders];

				GLES32.glGetAttachedShaders(shaderProgramObjectPerFragment, numAttachedShaders, retVal,0, shaderObjects,0);

				for (int i = 0; i < numAttachedShaders ; i++)
				{
					GLES32.glDetachShader(shaderProgramObjectPerFragment, shaderObjects[i]);
					GLES32.glDeleteShader(shaderObjects[i]);
					shaderObjects[i] = 0;
				}

			}

			GLES32.glUseProgram(0);
			GLES32.glDeleteProgram(shaderProgramObjectPerFragment);
			shaderProgramObjectPerFragment = 0;

			


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

