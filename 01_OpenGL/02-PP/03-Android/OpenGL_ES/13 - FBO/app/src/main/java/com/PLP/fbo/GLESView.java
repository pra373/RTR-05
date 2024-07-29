package com.PLP.fbo;

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


//texture packages

import android.graphics.BitmapFactory;

import android.graphics.Bitmap;

//package related to sphere

import java.nio.ShortBuffer;

import android.opengl.GLUtils;



public class GLESView extends GLSurfaceView implements GLSurfaceView.Renderer, OnDoubleTapListener, OnGestureListener
{
	private GestureDetector gestureDetector = null;

	//variable declarations

	private int shaderProgramObject_Cube;

	private int[] vao_Cube = new int[1];

	private int[] vbo_Position_Cube = new int[1];

	private int[] vbo_Texture_Cube = new int[1];

	private int mvpMatrixUniform_Cube;

	private final Context context;

	private float[] perspectiveProjectionMatrix_Cube = new float [16];

	//texture variable

	

	private int textureSamplerUniform_Cube = 0;

	//rotation variable
	private float angleCube = 360.0f;

	//FBO WIDTH and FBO height

	private int FBO_WIDTH = 512;
	private int FBO_HEIGHT = 512;

	private int winWidth;
	private int winHeight;

	//sphere related variables


	//variable declarations

	private int shaderProgramObjectPerVertex_Sphere;

	private int shaderProgramObjectPerFragment_Sphere;

	//sphere related variables

	private int[] vao_sphere = new int[1];
    private int[] vbo_Position_Sphere = new int[1];
    private int[] vbo_Normal_Sphere = new int[1];
    private int[] vbo_Element_Sphere = new int[1];

	private int numVertices = 0;

	private int numElements = 0;

	

	//private int modelViewMatrixUniform;

	private int modelMatrixUniform_Sphere;

	private int viewMatrixUniform_Sphere;

	private int projectionMatrixUniform_Sphere;

		//light uniform variables

	private int[] lightDiffuseUniform_Sphere = new int[3];

	private int[] lightPositionUniform_Sphere = new int[3];

	private int[] lightAmbientUniform_Sphere = new int[3];

	private int[] lightSpecularUniform_Sphere = new int[3];

	


	//material uniform variables

	private int materialDiffuseUniform_Sphere = 0;

	private int materialAmbientUniform_Sphere = 0;

	private int materialSpecularUniform_Sphere = 0;

	private int materialShininessUniform_Sphere = 0;

	//double tap 

	private int doubleTapnUniform_Sphere = 0;

	//************************************ per fragment uniform variables *******************************

	private int modelMatrixUniform_PF_Sphere;

	private int viewMatrixUniform_PF_Sphere;

	private int projectionMatrixUniform_PF_Sphere;

		//light uniform variables

	private int[] lightDiffuseUniform_PF_Sphere = new int[3];

	private int[] lightPositionUniform_PF_Sphere = new int[3];

	private int[] lightAmbientUniform_PF_Sphere = new int[3];

	private int[] lightSpecularUniform_PF_Sphere = new int[3];

	


	//material uniform variables

	private int materialDiffuseUniform_PF_Sphere = 0;

	private int materialAmbientUniform_PF_Sphere = 0;

	private int materialSpecularUniform_PF_Sphere = 0;

	private int materialShininessUniform_PF_Sphere = 0;

	//double tap 

	private int doubleTapnUniform_PF_Sphere = 0;



	//private int mvpMatrixUniform;

	private float[] perspectiveProjectionMatrix_Sphere = new float [16];


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

	//fbo related global variables


	private int fbo = 0;
	private int rbo = 0;  //render buffer object
	private int texture_FBO = 0;

	private boolean bFBOResult = false;


	public GLESView(Context _context)
	{
		super(_context);

		context = _context;

		//OpenGL-ES related

		setEGLContextClientVersion(3);

		setRenderer(this);

		setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);



		//create and set GestureDetector object

		gestureDetector = new GestureDetector(_context,this,null,false);

		gestureDetector.setOnDoubleTapListener(this);

	}

	//implementation of 3 methods of GLSurfaceView.Renderer interface

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config)
	{
		//code

		initialize_Cube(gl);
		
	}

	@Override
	public void onSurfaceChanged(GL10 gl,int width,int height)
	{
		//code

		resize_Cube(width,height);
	}

	@Override
	public void onDrawFrame(GL10 gl)
	{
		//code

		display_Cube();

		update_Cube();
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

		uninitialize_Cube();

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

	//implementation of our display_Cube(), resize_Cube(), initialize_Cube().

	private void initialize_Cube(GL10 gl)
	{
		//code

		//print OpenGL ES info
		printGLInfo(gl);

		//vertex shader 

		final String vertexShaderSourceCode_Cube = String.format
		(
			"#version 320 es"+
			"\n"+
			"in vec4 aPosition;"+
			"in vec2 aTexCoord;"+
			"out vec2 oTexCoord;"+
			"uniform mat4 uMVPMatrix;"+          
			"void main(void)"+
			"{"+
			"gl_Position=uMVPMatrix*aPosition;"+
			"oTexCoord = aTexCoord;"+
			"}"
		);

		int vertexShaderObject_Cube = GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);

		GLES32.glShaderSource(vertexShaderObject_Cube,vertexShaderSourceCode_Cube);

		GLES32.glCompileShader(vertexShaderObject_Cube);

		int[] shaderCompileStatus = new int[1];

		int[] infoLogLength = new int[1];

		String infoLog = null;

		GLES32.glGetShaderiv(vertexShaderObject_Cube,GLES32.GL_COMPILE_STATUS,shaderCompileStatus,0);

		if(shaderCompileStatus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetShaderiv(vertexShaderObject_Cube, GLES32.GL_INFO_LOG_LENGTH, infoLogLength,0);

			if(infoLogLength[0] > 0)
			{
				infoLog = GLES32.glGetShaderInfoLog(vertexShaderObject_Cube);

				System.out.println("PLP: Vertex Shader Compilation Error : " + infoLog);

				uninitialize_Cube();

				System.exit(0);
			}
		}

		//fragment shader

		final String fragmentShaderSourceCode_Cube = String.format
		(
			"#version 320 es"+ 
			"\n"+
			"precision highp float;"+
			"in vec2 oTexCoord;"+
			"uniform highp sampler2D uTextureSampler;"+
			"out vec4 FragColor;"+
			"void main(void)"+ 
			"{"+
			"FragColor = texture(uTextureSampler, oTexCoord);"+                     
			"}"
		);

		int fragmentShaderObject_Cube = GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);

		GLES32.glShaderSource(fragmentShaderObject_Cube,fragmentShaderSourceCode_Cube);

		GLES32.glCompileShader(fragmentShaderObject_Cube);

		shaderCompileStatus[0] = 0;

		infoLogLength[0] = 0;

		infoLog = null;

		GLES32.glGetShaderiv(fragmentShaderObject_Cube,GLES32.GL_COMPILE_STATUS,shaderCompileStatus,0);

		if(shaderCompileStatus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetShaderiv(fragmentShaderObject_Cube, GLES32.GL_INFO_LOG_LENGTH, infoLogLength,0);

			if(infoLogLength[0] > 0)
			{
				infoLog = GLES32.glGetShaderInfoLog(fragmentShaderObject_Cube);

				System.out.println("PLP: Fragment Shader Compilation Error : " + infoLog);

				uninitialize_Cube();

				System.exit(0);
			}
		}

		//shader Program
		
		shaderProgramObject_Cube = GLES32.glCreateProgram();

		GLES32.glAttachShader(shaderProgramObject_Cube, vertexShaderObject_Cube);
		GLES32.glAttachShader(shaderProgramObject_Cube, fragmentShaderObject_Cube);

		GLES32.glBindAttribLocation(shaderProgramObject_Cube,VertexAttributesEnum.AMC_ATTRIBUTE_POSITION,"aPosition");

		GLES32.glBindAttribLocation(shaderProgramObject_Cube,VertexAttributesEnum.AMC_ATTRIBUTE_TEXTURE,"aTexCoord");

		GLES32.glLinkProgram(shaderProgramObject_Cube);

		int[] programLinkStatus = new int[1];

		infoLogLength[0] = 0;

		infoLog = null;

		

		GLES32.glGetProgramiv(shaderProgramObject_Cube, GLES32.GL_LINK_STATUS, programLinkStatus,0);

		if(programLinkStatus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetProgramiv(shaderProgramObject_Cube, GLES32.GL_INFO_LOG_LENGTH, infoLogLength,0);

			if(infoLogLength[0] > 0)
			{
				//infoLog = GLES32.glGetProgramInfoLog(shaderProgramObject_Cube, infoLogLength, 0);

				infoLog = GLES32.glGetProgramInfoLog(shaderProgramObject_Cube);

				System.out.println("PLP:Shader Program object Compilation Error : " + infoLog);

				uninitialize_Cube();

				System.exit(0);
			}
		}

		//post linking uniform

		mvpMatrixUniform_Cube = GLES32.glGetUniformLocation(shaderProgramObject_Cube, "uMVPMatrix");

		textureSamplerUniform_Cube = GLES32.glGetUniformLocation(shaderProgramObject_Cube, "uTextureSampler");

		if (textureSamplerUniform_Cube == -1)
		{
			System.out.println("PLP: Failed to get texture sampler uniform : " + infoLog);

			uninitialize_Cube();

			System.exit(0);
		}

		//geometry coordinates

		final float[] cubePosition = new float[]
		{
				// front
			  1.0f,  1.0f,  1.0f, // top-right of front
			 -1.0f,  1.0f,  1.0f, // top-left of front
			 -1.0f, -1.0f,  1.0f, // bottom-left of front
			  1.0f, -1.0f,  1.0f, // bottom-right of front

			  // right
			   1.0f,  1.0f, -1.0f, // top-right of right
			   1.0f,  1.0f,  1.0f, // top-left of right
			   1.0f, -1.0f,  1.0f, // bottom-left of right
			   1.0f, -1.0f, -1.0f, // bottom-right of right

			   // back
				1.0f,  1.0f, -1.0f, // top-right of back
			   -1.0f,  1.0f, -1.0f, // top-left of back
			   -1.0f, -1.0f, -1.0f, // bottom-left of back
				1.0f, -1.0f, -1.0f, // bottom-right of back

				// left
				-1.0f,  1.0f,  1.0f, // top-right of left
				-1.0f,  1.0f, -1.0f, // top-left of left
				-1.0f, -1.0f, -1.0f, // bottom-left of left
				-1.0f, -1.0f,  1.0f, // bottom-right of left

				// top
				 1.0f,  1.0f, -1.0f, // top-right of top
				-1.0f,  1.0f, -1.0f, // top-left of top
				-1.0f,  1.0f,  1.0f, // bottom-left of top
				 1.0f,  1.0f,  1.0f, // bottom-right of top

				 // bottom
				  1.0f, -1.0f,  1.0f, // top-right of bottom
				 -1.0f, -1.0f,  1.0f, // top-left of bottom
				 -1.0f, -1.0f, -1.0f, // bottom-left of bottom
				  1.0f, -1.0f, -1.0f, // bottom-right of bottom
			 
		};

		final float[] cubeTexCoord = new float[]
		{
			  // front
		 1.0f, 1.0f, // top-right of front
		 0.0f, 1.0f, // top-left of front
		 0.0f, 0.0f, // bottom-left of front
		 1.0f, 0.0f, // bottom-right of front

		 // right
		 1.0f, 1.0f, // top-right of right
		 0.0f, 1.0f, // top-left of right
		 0.0f, 0.0f, // bottom-left of right
		 1.0f, 0.0f, // bottom-right of right

		 // back
		 1.0f, 1.0f, // top-right of back
		 0.0f, 1.0f, // top-left of back
		 0.0f, 0.0f, // bottom-left of back
		 1.0f, 0.0f, // bottom-right of back

		 // left
		 1.0f, 1.0f, // top-right of left
		 0.0f, 1.0f, // top-left of left
		 0.0f, 0.0f, // bottom-left of left
		 1.0f, 0.0f, // bottom-right of left

		 // top
		 1.0f, 1.0f, // top-right of top
		 0.0f, 1.0f, // top-left of top
		 0.0f, 0.0f, // bottom-left of top
		 1.0f, 0.0f, // bottom-right of top

		 // bottom
		 1.0f, 1.0f, // top-right of bottom
		 0.0f, 1.0f, // top-left of bottom
		 0.0f, 0.0f, // bottom-left of bottom
		 1.0f, 0.0f, // bottom-right of bottom
		};

		//vao

		GLES32.glGenVertexArrays(1,vao_Cube,0);

		//Bind with VAO

		GLES32.glBindVertexArray(vao_Cube[0]);

		//VBO for position

		GLES32.glGenBuffers(1, vbo_Position_Cube,0);
		
		// Bind with VBO of position

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_Position_Cube[0]);

		//prepare triangle vertices array for glBuffer data

		//step 1 : allocate enough size buffer for triangle vertices

		ByteBuffer byteBuffer = ByteBuffer.allocateDirect(cubePosition.length * 4);

		//step 2: set byte order

		byteBuffer.order(ByteOrder.nativeOrder());

		//step3 : convert created buffer as float array

		FloatBuffer positionBuffer = byteBuffer.asFloatBuffer();

		//step 4 : fill buffer with vertices array

		positionBuffer.put(cubePosition);

		//step 5 : rewind the buffer to 0th index

		positionBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, cubePosition.length * 4, positionBuffer, GLES32.GL_STATIC_DRAW);

		GLES32.glVertexAttribPointer(VertexAttributesEnum.AMC_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);

		GLES32.glEnableVertexAttribArray(VertexAttributesEnum.AMC_ATTRIBUTE_POSITION);

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

		//VBO for texture

		GLES32.glGenBuffers(1, vbo_Texture_Cube,0);

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_Texture_Cube[0]);

		ByteBuffer byteBuffer_T = ByteBuffer.allocateDirect(cubeTexCoord.length * 4);

		//step 2: set byte order

		byteBuffer_T.order(ByteOrder.nativeOrder());

		//step3 : convert created buffer as float array

		FloatBuffer texCoordBuffer = byteBuffer_T.asFloatBuffer();

		//step 4 : fill buffer with vertices array

		texCoordBuffer.put(cubeTexCoord);

		//step 5 : rewind the buffer to 0th index

		texCoordBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, cubeTexCoord.length * 4, texCoordBuffer, GLES32.GL_STATIC_DRAW);

		GLES32.glVertexAttribPointer(VertexAttributesEnum.AMC_ATTRIBUTE_TEXTURE, 2, GLES32.GL_FLOAT, false, 0, 0);

		GLES32.glEnableVertexAttribArray(VertexAttributesEnum.AMC_ATTRIBUTE_TEXTURE);

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);


		
		//step 18 : unbind with VAO
		GLES32.glBindVertexArray(0);

		//depth setting

		GLES32.glClearDepthf(1.0f);

		GLES32.glEnable(GLES32.GL_DEPTH_TEST);

		GLES32.glDepthFunc(GLES32.GL_LEQUAL);

		//clear color

		GLES32.glClearColor(1.0f,1.0f,1.0f,1.0f);

		//load texture

		//stoneTexture[0] = loadGLTexture(R.raw.kundali);

		Matrix.setIdentityM(perspectiveProjectionMatrix_Cube,0);

		//fbo related code

		//FBO related code

		if (createFBO(FBO_WIDTH, FBO_HEIGHT) == true)
		{
			bFBOResult = initialize_Sphere(gl,FBO_WIDTH, FBO_HEIGHT);
		}
		else 
		{
			System.out.println("Failed to create framebuffer object");
		}


		

	}

	private void printGLInfo(GL10 gl)
	{
		//code

		System.out.println("PLP: OpenGL-ES Renderer : " + gl.glGetString(GL10.GL_RENDERER));

		System.out.println("PLP: OpenGL-ES version : " + gl.glGetString(GL10.GL_VERSION));

		System.out.println("PLP: OpenGL-ES Shading Launguage Version : " + gl.glGetString(GLES32.GL_SHADING_LANGUAGE_VERSION));
	}


	private boolean initialize_Sphere(GL10 gl,int textureWidth,int textureHeight)
	{
		//code

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

				uninitialize_Sphere();

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

				uninitialize_Sphere();

				System.exit(0);
			}
		}

		//shader Program
		
		shaderProgramObjectPerVertex_Sphere = GLES32.glCreateProgram();

		GLES32.glAttachShader(shaderProgramObjectPerVertex_Sphere, vertexShaderObjectPerVertex);
		GLES32.glAttachShader(shaderProgramObjectPerVertex_Sphere, fragmentShaderObjectPerVertex);

		GLES32.glBindAttribLocation(shaderProgramObjectPerVertex_Sphere,VertexAttributesEnum.AMC_ATTRIBUTE_POSITION,"aPosition");

		GLES32.glBindAttribLocation(shaderProgramObjectPerVertex_Sphere,VertexAttributesEnum.AMC_ATTRIBUTE_NORMALS,"aNormal");

		GLES32.glLinkProgram(shaderProgramObjectPerVertex_Sphere);

		int[] programLinkStatus = new int[1];

		infoLogLength[0] = 0;

		infoLog = null;

		

		GLES32.glGetProgramiv(shaderProgramObjectPerVertex_Sphere, GLES32.GL_LINK_STATUS, programLinkStatus,0);

		if(programLinkStatus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetProgramiv(shaderProgramObjectPerVertex_Sphere, GLES32.GL_INFO_LOG_LENGTH, infoLogLength,0);

			if(infoLogLength[0] > 0)
			{
				//infoLog = GLES32.glGetProgramInfoLog(shaderProgramObject, infoLogLength, 0);

				infoLog = GLES32.glGetProgramInfoLog(shaderProgramObjectPerVertex_Sphere);

				System.out.println("PLP:Shader Program object per vertex Compilation Error : " + infoLog);

				uninitialize_Sphere();

				System.exit(0);
			}
		}

		//post linking uniform

		 modelMatrixUniform_Sphere = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex_Sphere, "uModelMatrix");

	 viewMatrixUniform_Sphere = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex_Sphere, "uViewMatrix");

	 projectionMatrixUniform_Sphere = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex_Sphere, "uProjectionMatrix");

	 lightAmbientUniform_Sphere[0] = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex_Sphere, "ulightAmbient[0]");

	 lightDiffuseUniform_Sphere[0] = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex_Sphere, "ulightDiffuse[0]");

	 lightSpecularUniform_Sphere[0] = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex_Sphere, "ulightSpecular[0]");

	 lightPositionUniform_Sphere[0] = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex_Sphere, "ulightPosition[0]");

	 lightAmbientUniform_Sphere[1] = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex_Sphere, "ulightAmbient[1]");

	 lightDiffuseUniform_Sphere[1] = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex_Sphere, "ulightDiffuse[1]");

	 lightSpecularUniform_Sphere[1] = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex_Sphere, "ulightSpecular[1]");

	 lightPositionUniform_Sphere[1] = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex_Sphere, "ulightPosition[1]");

	 lightAmbientUniform_Sphere[2] = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex_Sphere, "ulightAmbient[2]");

	 lightDiffuseUniform_Sphere[2] = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex_Sphere, "ulightDiffuse[2]");

	 lightSpecularUniform_Sphere[2] = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex_Sphere, "ulightSpecular[2]");

	 lightPositionUniform_Sphere[2] = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex_Sphere, "ulightPosition[2]");


	 materialAmbientUniform_Sphere = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex_Sphere, "uMaterialAmbient");

	 materialDiffuseUniform_Sphere = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex_Sphere, "uMaterialDiffuse");


	 materialSpecularUniform_Sphere = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex_Sphere, "uMaterialSpecular");

	 materialShininessUniform_Sphere = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex_Sphere, "uMaterialShininess");

	doubleTapnUniform_Sphere = GLES32.glGetUniformLocation(shaderProgramObjectPerVertex_Sphere, "uDoubleTap");

// ********************************* per fragment lighting shaders ********************************************************
	
	//vertex shader 

		final String vertexShaderSourceCodePerFragment_Sphere = String.format
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

		int vertexShaderObjectPerFragment_Sphere = GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);

		GLES32.glShaderSource(vertexShaderObjectPerFragment_Sphere,vertexShaderSourceCodePerFragment_Sphere);

		GLES32.glCompileShader(vertexShaderObjectPerFragment_Sphere);

		shaderCompileStatus[0] = 0;

		infoLogLength[0] = 0;

		infoLog = null;

		GLES32.glGetShaderiv(vertexShaderObjectPerFragment_Sphere,GLES32.GL_COMPILE_STATUS,shaderCompileStatus,0);

		if(shaderCompileStatus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetShaderiv(vertexShaderObjectPerFragment_Sphere, GLES32.GL_INFO_LOG_LENGTH, infoLogLength,0);

			if(infoLogLength[0] > 0)
			{
				infoLog = GLES32.glGetShaderInfoLog(vertexShaderObjectPerFragment_Sphere);

				System.out.println("PLP: Vertex Shader per fragment Compilation Error : " + infoLog);

				uninitialize_Sphere();

				System.exit(0);
			}
		}

		//fragment shader

		final String fragmentShaderSourceCodePerFragment_Sphere = String.format
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

		int fragmentShaderObjectPerFragment_Sphere = GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);

		GLES32.glShaderSource(fragmentShaderObjectPerFragment_Sphere,fragmentShaderSourceCodePerFragment_Sphere);

		GLES32.glCompileShader(fragmentShaderObjectPerFragment_Sphere);

		shaderCompileStatus[0] = 0;

		infoLogLength[0] = 0;

		infoLog = null;

		GLES32.glGetShaderiv(fragmentShaderObjectPerFragment_Sphere,GLES32.GL_COMPILE_STATUS,shaderCompileStatus,0);

		if(shaderCompileStatus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetShaderiv(fragmentShaderObjectPerFragment_Sphere, GLES32.GL_INFO_LOG_LENGTH, infoLogLength,0);

			if(infoLogLength[0] > 0)
			{
				infoLog = GLES32.glGetShaderInfoLog(fragmentShaderObjectPerFragment_Sphere);

				System.out.println("PLP: Fragment Shader per fragment Compilation Error : " + infoLog);

				uninitialize_Sphere();

				System.exit(0);
			}
		}

		//shader Program
		
		shaderProgramObjectPerFragment_Sphere = GLES32.glCreateProgram();

		GLES32.glAttachShader(shaderProgramObjectPerFragment_Sphere, vertexShaderObjectPerFragment_Sphere);
		GLES32.glAttachShader(shaderProgramObjectPerFragment_Sphere, fragmentShaderObjectPerFragment_Sphere);

		GLES32.glBindAttribLocation(shaderProgramObjectPerFragment_Sphere,VertexAttributesEnum.AMC_ATTRIBUTE_POSITION,"aPosition");

		GLES32.glBindAttribLocation(shaderProgramObjectPerFragment_Sphere,VertexAttributesEnum.AMC_ATTRIBUTE_NORMALS,"aNormal");

		GLES32.glLinkProgram(shaderProgramObjectPerFragment_Sphere);

		programLinkStatus[0] = 0;

		infoLogLength[0] = 0;

		infoLog = null;

		

		GLES32.glGetProgramiv(shaderProgramObjectPerFragment_Sphere, GLES32.GL_LINK_STATUS, programLinkStatus,0);

		if(programLinkStatus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetProgramiv(shaderProgramObjectPerFragment_Sphere, GLES32.GL_INFO_LOG_LENGTH, infoLogLength,0);

			if(infoLogLength[0] > 0)
			{
				//infoLog = GLES32.glGetProgramInfoLog(shaderProgramObject, infoLogLength, 0);

				infoLog = GLES32.glGetProgramInfoLog(shaderProgramObjectPerFragment_Sphere);

				System.out.println("PLP:Shader Program object per fragment Compilation Error : " + infoLog);

				uninitialize_Sphere();

				System.exit(0);
			}
		}

			

		//post linking uniform

		 modelMatrixUniform_PF_Sphere = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment_Sphere, "uModelMatrix");

		 viewMatrixUniform_PF_Sphere = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment_Sphere, "uViewMatrix");

		 projectionMatrixUniform_PF_Sphere = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment_Sphere, "uProjectionMatrix");

		 lightAmbientUniform_PF_Sphere[0] = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment_Sphere, "ulightAmbient[0]");

		 lightDiffuseUniform_PF_Sphere[0] = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment_Sphere, "ulightDiffuse[0]");

		 lightSpecularUniform_PF_Sphere[0] = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment_Sphere, "ulightSpecular[0]");

		 lightPositionUniform_PF_Sphere[0] = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment_Sphere, "ulightPosition[0]");

		 lightAmbientUniform_PF_Sphere[1] = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment_Sphere, "ulightAmbient[1]");

		 lightDiffuseUniform_PF_Sphere[1] = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment_Sphere, "ulightDiffuse[1]");

		 lightSpecularUniform_PF_Sphere[1] = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment_Sphere, "ulightSpecular[1]");

		 lightPositionUniform_PF_Sphere[1] = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment_Sphere, "ulightPosition[1]");

		  lightAmbientUniform_PF_Sphere[2] = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment_Sphere, "ulightAmbient[2]");

		 lightDiffuseUniform_PF_Sphere[2] = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment_Sphere, "ulightDiffuse[2]");

		 lightSpecularUniform_PF_Sphere[2] = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment_Sphere, "ulightSpecular[2]");

		 lightPositionUniform_PF_Sphere[2] = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment_Sphere, "ulightPosition[2]");


		materialAmbientUniform_PF_Sphere = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment_Sphere, "uMaterialAmbient");

		materialDiffuseUniform_PF_Sphere = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment_Sphere, "uMaterialDiffuse");


		materialSpecularUniform_PF_Sphere = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment_Sphere, "uMaterialSpecular");

		materialShininessUniform_PF_Sphere = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment_Sphere, "uMaterialShininess");

		doubleTapnUniform_PF_Sphere = GLES32.glGetUniformLocation(shaderProgramObjectPerFragment_Sphere, "uDoubleTap");
		
		
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

		GLES32.glGenBuffers(1, vbo_Position_Sphere,0);
		
		// Bind with VBO of position

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_Position_Sphere[0]);

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

		

		GLES32.glGenBuffers(1, vbo_Normal_Sphere,0);
		
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_Normal_Sphere[0]);

	
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

		GLES32.glGenBuffers(1, vbo_Element_Sphere,0);
		
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_Element_Sphere[0]);

	
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



		


		Matrix.setIdentityM(perspectiveProjectionMatrix_Sphere,0);

		return(true);

	}

	private boolean createFBO(int textureWidth, int textureHeight)
	{
		 // Check capacity of render buffer

		int[] maxRenderBufferSize = new int[1];

		GLES32.glGetIntegerv(GLES32.GL_MAX_RENDERBUFFER_SIZE, maxRenderBufferSize, 0);

		if (maxRenderBufferSize[0] < textureWidth || maxRenderBufferSize[0] < textureHeight)
		{
			System.out.println("Texture size overflow");
			return false;
		}

		// Create custom frame buffer

		int[] framebuffers = new int[1];

		GLES32.glGenFramebuffers(1, framebuffers, 0);

		fbo = framebuffers[0];

		GLES32.glBindFramebuffer(GLES32.GL_FRAMEBUFFER, fbo);

		 // Create texture for FBO in which we are going to render Scene 2 (Sphere)
		int[] textures = new int[1];
		GLES32.glGenTextures(1, textures, 0);

		texture_FBO = textures[0];

		GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, texture_FBO);

		GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_WRAP_S, GLES32.GL_CLAMP_TO_EDGE);
		GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_WRAP_T, GLES32.GL_CLAMP_TO_EDGE);
		GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_MAG_FILTER, GLES32.GL_LINEAR);
		GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_MIN_FILTER, GLES32.GL_LINEAR);

		GLES32.glTexImage2D(GLES32.GL_TEXTURE_2D, 0, GLES32.GL_RGB, textureWidth, textureHeight, 0, GLES32.GL_RGB, GLES32.GL_UNSIGNED_SHORT_5_6_5, null);

		 // Attach above texture to framebuffer at default color attachment 0

		GLES32.glFramebufferTexture2D(GLES32.GL_FRAMEBUFFER, GLES32.GL_COLOR_ATTACHMENT0, GLES32.GL_TEXTURE_2D, texture_FBO, 0);

		 // Create render buffer for depth

		int[] renderbuffers = new int[1];

		GLES32.glGenRenderbuffers(1, renderbuffers, 0);

		rbo = renderbuffers[0];

		GLES32.glBindRenderbuffer(GLES32.GL_RENDERBUFFER, rbo);

		// Set the storage of the render buffer for depth

		GLES32.glRenderbufferStorage(GLES32.GL_RENDERBUFFER, GLES32.GL_DEPTH_COMPONENT16, textureWidth, textureHeight);

		// Attach depth render buffer to FBO

		GLES32.glFramebufferRenderbuffer(GLES32.GL_FRAMEBUFFER, GLES32.GL_DEPTH_ATTACHMENT, GLES32.GL_RENDERBUFFER, rbo);

		// Check the framebuffer status

		if (GLES32.glCheckFramebufferStatus(GLES32.GL_FRAMEBUFFER) != GLES32.GL_FRAMEBUFFER_COMPLETE) 
		{
			System.out.println("Framebuffer creation status is not complete");
			return false;
		}

		// Unbind the framebuffer
		GLES32.glBindFramebuffer(GLES32.GL_FRAMEBUFFER, 0);

		resize_Sphere(winWidth, winHeight);
		
		return true;


	}


	
	private void resize_Cube(int width,int height)
	{
		//code

		if(height <= 0)
		{
			height = 1;
		}

		winWidth = width;
		winHeight = height;

		GLES32.glViewport(0,0,width,height);

		Matrix.perspectiveM(perspectiveProjectionMatrix_Cube,0,45.0f,(float)width/(float)height,0.1f,100.0f);
	}

	private void resize_Sphere(int width,int height)
	{
		//code

		if(height <= 0)
		{
			height = 1;
		}

		GLES32.glViewport(0,0,width,height);

		Matrix.perspectiveM(perspectiveProjectionMatrix_Sphere,0,45.0f,(float)width/(float)height,0.1f,100.0f);
	}

	private void display_Cube()
	{

		//local functions
	
		/*void display_Sphere(GLint, GLint);
		void update_Sphere(void);
		void resize_Cube(int width, int height);*/

		//render FBO scene

		if (bFBOResult == true)
		{
			display_Sphere(FBO_WIDTH, FBO_HEIGHT);
			update_Sphere();
		}

		//call resize cube again to companset the change done by displaySphere()

		resize_Cube(winWidth, winHeight);

		//reset the color to white to companset the change done by display sphere

		GLES32.glClearColor(1.0f, 1.0f, 1.0f, 1.0f);


		//code

		GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);

		//step 1: Use shader program object

		GLES32.glUseProgram(shaderProgramObject_Cube);

		//transformations

		float[] modelViewMatrix = new float[16];
		float[] modelViewProjectionMatrix = new float[16];
		float[] rotationMatrix = new float[16];

		Matrix.setIdentityM(modelViewMatrix,0);
		Matrix.setIdentityM(modelViewProjectionMatrix,0);
		Matrix.setIdentityM(rotationMatrix, 0); // Initialize rotation matrix

		Matrix.translateM(modelViewMatrix,0, 0.0f, 0.0f, -5.0f);

		Matrix.rotateM(rotationMatrix, 0, angleCube, 1.0f, 1.0f, 1.0f); // Rotate around Y axis

		// Combine rotation with modelViewMatrix
		Matrix.multiplyMM(modelViewMatrix, 0, modelViewMatrix, 0, rotationMatrix, 0);	

		Matrix.multiplyMM(modelViewProjectionMatrix, 0, perspectiveProjectionMatrix_Cube, 0, modelViewMatrix, 0);

		//push above mvp into vertex shader's mvp uniform

		GLES32.glUniformMatrix4fv(mvpMatrixUniform_Cube, 1, false, modelViewProjectionMatrix,0);

		//for texture

		GLES32.glActiveTexture(GLES32.GL_TEXTURE0);

		GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, texture_FBO);

		GLES32.glUniform1i(textureSamplerUniform_Cube, 0);


		GLES32.glBindVertexArray(vao_Cube[0]);

		//step 3: Draw the geometry

		//step 3: Draw the geometry

		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 0, 4);

		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 4, 4);

		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 8, 4);

		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 12, 4);

		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 16, 4);

		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 20, 4);



		//step 4: unBind with (VAO)

		GLES32.glBindVertexArray(0);

		//step 5: Unuse shader program object
		GLES32.glUseProgram(0);


		//Render

		requestRender();  

	}

	private void display_Sphere(int textureWidth,int textureHeight)
	{
		//code

		// Bind with FBO

		GLES32.glBindFramebuffer(GLES32.GL_FRAMEBUFFER, fbo);

		//call resize_Sphere()

		resize_Sphere(textureWidth, textureHeight);

		//set the clear color to back again to companset the change done by display_Cube()

		GLES32.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


		GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);

		//step 1: Use shader program object

		if(select_SPO == 1)
		{
			GLES32.glUseProgram(shaderProgramObjectPerFragment_Sphere);
		}

		else
		{
			GLES32.glUseProgram(shaderProgramObjectPerVertex_Sphere);
		}
		
		

		
		

		//GLES32.glUseProgram(shaderProgramObjectPerVertex_Sphere);

		

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
			GLES32.glUniformMatrix4fv(modelMatrixUniform_Sphere, 1, false, modelMatrix,0);

			GLES32.glUniformMatrix4fv(viewMatrixUniform_Sphere, 1, false, viewMatrix,0);

			GLES32.glUniformMatrix4fv(projectionMatrixUniform_Sphere, 1, false, perspectiveProjectionMatrix_Sphere,0);
		}

		else
		{
			GLES32.glUniformMatrix4fv(modelMatrixUniform_PF_Sphere, 1, false, modelMatrix,0);

			GLES32.glUniformMatrix4fv(viewMatrixUniform_PF_Sphere, 1, false, viewMatrix,0);

			GLES32.glUniformMatrix4fv(projectionMatrixUniform_PF_Sphere, 1, false, perspectiveProjectionMatrix_Sphere,0);
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
				GLES32.glUniform1i(doubleTapnUniform_Sphere, 1);

				GLES32.glUniform3fv(lightAmbientUniform_Sphere[0], 1, lightAmbientBuffer0);

				GLES32.glUniform3fv(lightDiffuseUniform_Sphere[0], 1, lightDiffuseBuffer0);

				GLES32.glUniform3fv(lightSpecularUniform_Sphere[0], 1,lightSpecularBuffer0);

				GLES32.glUniform4fv(lightPositionUniform_Sphere[0], 1, lightPositionBuffer0);

				GLES32.glUniform3fv(lightAmbientUniform_Sphere[1], 1, lightAmbientBuffer1);

				GLES32.glUniform3fv(lightDiffuseUniform_Sphere[1], 1, lightDiffuseBuffer1);

				GLES32.glUniform3fv(lightSpecularUniform_Sphere[1], 1, lightSpecularBuffer1);

				GLES32.glUniform4fv(lightPositionUniform_Sphere[1], 1, lightPositionBuffer1);


				GLES32.glUniform3fv(lightAmbientUniform_Sphere[2], 1, lightAmbientBuffer2);

				GLES32.glUniform3fv(lightDiffuseUniform_Sphere[2], 1, lightDiffuseBuffer2);

				GLES32.glUniform3fv(lightSpecularUniform_Sphere[2], 1, lightSpecularBuffer2);

				GLES32.glUniform4fv(lightPositionUniform_Sphere[2], 1, lightPositionBuffer2);


				GLES32.glUniform3fv(materialAmbientUniform_Sphere, 1, materialAmbientBuffer);

				GLES32.glUniform3fv(materialDiffuseUniform_Sphere, 1, materialDiffuseBuffer);

				GLES32.glUniform3fv(materialSpecularUniform_Sphere, 1, materialSpecularBuffer);

				GLES32.glUniform1f(materialShininessUniform_Sphere, materialShininess);
			}

			else
			{
				GLES32.glUniform1i(doubleTapnUniform_PF_Sphere, 1);

				GLES32.glUniform3fv(lightAmbientUniform_PF_Sphere[0], 1, lightAmbientBuffer_PF0);

				GLES32.glUniform3fv(lightDiffuseUniform_PF_Sphere[0], 1, lightDiffuseBuffer_PF0);

				GLES32.glUniform3fv(lightSpecularUniform_PF_Sphere[0], 1,lightSpecularBuffer_PF0);

				GLES32.glUniform4fv(lightPositionUniform_PF_Sphere[0], 1, lightPositionBuffer_PF0);

				GLES32.glUniform3fv(lightAmbientUniform_PF_Sphere[1], 1, lightAmbientBuffer_PF1);

				GLES32.glUniform3fv(lightDiffuseUniform_PF_Sphere[1], 1, lightDiffuseBuffer_PF1);

				GLES32.glUniform3fv(lightSpecularUniform_PF_Sphere[1], 1, lightSpecularBuffer_PF1);

				GLES32.glUniform4fv(lightPositionUniform_PF_Sphere[1], 1, lightPositionBuffer_PF1);


				GLES32.glUniform3fv(lightAmbientUniform_PF_Sphere[2], 1, lightAmbientBuffer_PF2);

				GLES32.glUniform3fv(lightDiffuseUniform_PF_Sphere[2], 1, lightDiffuseBuffer_PF2);

				GLES32.glUniform3fv(lightSpecularUniform_PF_Sphere[2], 1, lightSpecularBuffer_PF2);

				GLES32.glUniform4fv(lightPositionUniform_PF_Sphere[2], 1, lightPositionBuffer_PF2);


				GLES32.glUniform3fv(materialAmbientUniform_PF_Sphere, 1, materialAmbientBuffer);

				GLES32.glUniform3fv(materialDiffuseUniform_PF_Sphere, 1, materialDiffuseBuffer);

				GLES32.glUniform3fv(materialSpecularUniform_PF_Sphere, 1, materialSpecularBuffer);

				GLES32.glUniform1f(materialShininessUniform_PF_Sphere, materialShininess);
			}
			
				
		}

		else
		{
			GLES32.glUniform1i(doubleTapnUniform_Sphere, 0);
			GLES32.glUniform1i(doubleTapnUniform_PF_Sphere, 0);
		}

		//step 3: Draw the geometry

		
		//bind element buffer
		GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_Element_Sphere[0]);


		//step 3: Draw the geometry

		GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);


		//step 4: unBind with (VAO)

		GLES32.glBindVertexArray(0);
		

		
		

		


		//step 4: unBind with (VAO)

		GLES32.glBindVertexArray(0);
		

		

		//step 5: Unuse shader program object
		GLES32.glUseProgram(0);


		//Render

		//requestRender();

		//unbind with Framebuffer

		GLES32.glBindFramebuffer(GLES32.GL_FRAMEBUFFER, 0);

	}


	private void update_Cube()
	{
		angleCube = angleCube - 0.4f;

	    if(angleCube <= 0.0f)
	    {
			angleCube = 360.0f;
	    }
	 
	}

	private void update_Sphere()
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


	private void uninitialize_Cube()
	{
		//code

		if(shaderProgramObject_Cube > 0)
		{
			GLES32.glUseProgram(shaderProgramObject_Cube);
			int[] retVal = new int[1];

			//step 2 : get number of attached shaders
			GLES32.glGetProgramiv(shaderProgramObject_Cube, GLES32.GL_ATTACHED_SHADERS, retVal,0);

			if(retVal[0] > 0)
			{
				int numAttachedShaders = retVal[0];

				int[] shaderObjects = new int[numAttachedShaders];

				GLES32.glGetAttachedShaders(shaderProgramObject_Cube, numAttachedShaders, retVal,0, shaderObjects,0);

				for (int i = 0; i < numAttachedShaders ; i++)
				{
					GLES32.glDetachShader(shaderProgramObject_Cube, shaderObjects[i]);
					GLES32.glDeleteShader(shaderObjects[i]);
					shaderObjects[i] = 0;
				}

			}

			GLES32.glUseProgram(0);
			GLES32.glDeleteProgram(shaderProgramObject_Cube);
			shaderProgramObject_Cube = 0;

			


		}

		if (vbo_Texture_Cube[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo_Texture_Cube,0);
			vbo_Texture_Cube[0] = 0;
		}

		

		if(vbo_Position_Cube[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo_Position_Cube, 0);
			vbo_Position_Cube[0] = 0;
		}

		if(vao_Cube[0] > 0)
		{
			GLES32.glDeleteVertexArrays(1, vao_Cube, 0);
			vao_Cube[0] = 0;
		}
	}

	private void uninitialize_Sphere()
	{
		//code

		if(shaderProgramObjectPerVertex_Sphere > 0)
		{
			GLES32.glUseProgram(shaderProgramObjectPerVertex_Sphere);
			int[] retVal = new int[1];

			//step 2 : get number of attached shaders
			GLES32.glGetProgramiv(shaderProgramObjectPerVertex_Sphere, GLES32.GL_ATTACHED_SHADERS, retVal,0);

			if(retVal[0] > 0)
			{
				int numAttachedShaders = retVal[0];

				int[] shaderObjects = new int[numAttachedShaders];

				GLES32.glGetAttachedShaders(shaderProgramObjectPerVertex_Sphere, numAttachedShaders, retVal,0, shaderObjects,0);

				for (int i = 0; i < numAttachedShaders ; i++)
				{
					GLES32.glDetachShader(shaderProgramObjectPerVertex_Sphere, shaderObjects[i]);
					GLES32.glDeleteShader(shaderObjects[i]);
					shaderObjects[i] = 0;
				}

			}

			GLES32.glUseProgram(0);
			GLES32.glDeleteProgram(shaderProgramObjectPerVertex_Sphere);
			shaderProgramObjectPerVertex_Sphere = 0;

			


		}

		if(vbo_Position_Sphere[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo_Position_Sphere, 0);
			vbo_Position_Sphere[0] = 0;
		}

		if(vbo_Normal_Sphere[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo_Normal_Sphere, 0);
			vbo_Normal_Sphere[0] = 0;
		}

		if(vbo_Element_Sphere[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo_Element_Sphere, 0);
			vbo_Element_Sphere[0] = 0;
		}


		if(vao_sphere[0] > 0)
		{
			GLES32.glDeleteVertexArrays(1, vao_sphere, 0);
			vao_sphere[0] = 0;
		}
	}

	private int loadGLTexture(int imageResourceID)
	{
		//create BitmapFactory options class object

		BitmapFactory.Options options = new BitmapFactory.Options();

		//dont scale the image

		options.inScaled = false;

		//create the bitmap image from Image resource

		Bitmap bitmap = BitmapFactory.decodeResource(context.getResources(),imageResourceID,options);

		//create texture

		int[] texture = new int[1];

		GLES32.glGenTextures(1, texture,0);

		GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, texture[0]);

		GLES32.glPixelStorei(GLES32.GL_UNPACK_ALIGNMENT, 1);

		GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_MAG_FILTER, GLES32.GL_LINEAR);

		GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_MIN_FILTER,GLES32.GL_LINEAR_MIPMAP_LINEAR);

		GLUtils.texImage2D(GLES32.GL_TEXTURE_2D,0,bitmap,0);

		GLES32.glGenerateMipmap(GLES32.GL_TEXTURE_2D);

		GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, 0);

		return texture[0];


	}

};

