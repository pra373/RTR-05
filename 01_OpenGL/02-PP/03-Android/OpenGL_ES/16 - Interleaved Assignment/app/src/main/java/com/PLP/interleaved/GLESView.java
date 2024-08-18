package com.PLP.interleaved;

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

// Packages for texture

import android.graphics.BitmapFactory;
import android.graphics.Bitmap;
import android.opengl.GLUtils;

public class GLESView extends GLSurfaceView implements GLSurfaceView.Renderer, OnDoubleTapListener, OnGestureListener
{
	private GestureDetector gestureDetector = null;

	//variable declarations

	private int shaderProgramObject;

	private int[] vao = new int[1];

	private int[] vbo_PCNT = new int[1];

	private float angle_Cube = 0.0f;

	// Unifroms

	private int modelMatrixUniform;

	private int viewMatrixUniform;

	private int projectionMatrixUniform;

	private float[] perspectiveProjectionMatrix = new float [16];

	// for texture

	private int[] marble_texture = new int[1];  // for texture.

	private int textureSamplerUniform = 0;

	
	//light properties

	private float[] lightPosition = new float[] {100.0f,100.0f,100.0f,1.0f};

	private float [] lightDiffuse = new float[] { 1.0f,1.0f,1.0f,1.0f };

	private float [] lightSpecular = new float[] { 1.0f,1.0f,1.0f,1.0f };

	private float [] lightAmbient = new float[] { 0.1f,0.1f,0.1f,1.0f };


	//material properties

	private float[] materialDiffuse = new float[] {1.0f,1.0f,1.0f,1.0f};

	private float[] materialAmbient = new float[] {0.0f,0.0f,0.0f,1.0f};

	private float[] materialSpecular = new float[] {1.0f,1.0f,1.0f,1.0f};

	private float materialShininess = 50.0f;

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


	// FloatBuffer variables

	FloatBuffer lightAmbientBuffer;

	FloatBuffer lightDiffuseBuffer;

	FloatBuffer lightSpecularBuffer;

	FloatBuffer lightPositionBuffer;

	FloatBuffer materialAmbientBuffer;

	FloatBuffer materialDiffuseBuffer;

	FloatBuffer materialSpecularBuffer;




	private final Context context;

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
			"in vec4 aColor;"+
			"in vec3 aNormal;\n"+
			"in vec2 aTexCoord;"+
			"out vec4 oColor;"+
			"out vec2 oTexCoord;"+
			"uniform mat4 uModelMatrix;\n"+
			"uniform mat4 uViewMatrix;\n"+
			"uniform mat4 uProjectionMatrix;\n" +
			"uniform vec3 ulightAmbient;\n"+
			"uniform vec3 ulightDiffuse;\n"+
			"uniform vec3 ulightSpecular;\n"+
			"uniform vec4 ulightPosition;\n"+
			"uniform vec3 uMaterialAmbient;\n"+
			"uniform vec3 uMaterialDiffuse;\n"+
			"uniform vec3 uMaterialSpecular;\n"+
			"uniform float uMaterialShininess;\n"+
			"uniform highp int uDoubleTap;\n"+
			"out vec3 otransformedNormals;\n"+
			"out vec3 olightDirection;\n"+
			"out vec3 oviewerVector;\n"+
			"void main(void)\n"+
			"{\n"+
			"if(uDoubleTap == 1)\n"+
			"{\n"+
			"vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;\n"+
			"otransformedNormals = (mat3(uViewMatrix * uModelMatrix) * aNormal);\n"+
			"olightDirection = (vec3 (ulightPosition - eyeCoordinates));\n"+
			"oviewerVector = (- eyeCoordinates.xyz);\n"+
			"}\n"+
			"else\n"+
			"{\n"+
			"otransformedNormals = vec3(0.0, 0.0, 0.0);\n"+
			"olightDirection = vec3(0.0, 0.0, 0.0);\n"+
			"oviewerVector = vec3(0.0, 0.0, 0.0);\n"+
			"}\n"+
			"oColor = aColor;\n"+
			"oTexCoord = aTexCoord;\n"+
			"gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;\n"+
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
			"in vec4 oColor;"+
			"in vec2 oTexCoord;"+
			"uniform highp sampler2D uTextureSampler;"+
			"in vec3 otransformedNormals;\n"+
			"in vec3 olightDirection;\n"+
			"in vec3 oviewerVector;\n"+
			"uniform vec3 ulightAmbient;\n"+
			"uniform vec3 ulightDiffuse;\n"+
			"uniform vec3 ulightSpecular;\n"+
			"uniform vec3 uMaterialAmbient;\n"+
			"uniform vec3 uMaterialDiffuse;\n"+
			"uniform vec3 uMaterialSpecular;\n"+
			"uniform float uMaterialShininess;\n"+
			"uniform highp int uDoubleTap;\n"+
			"out vec4 FragColor;"+
			"void main(void)\n"+
			"{\n"+
			"vec3 PhongADSLight;\n"+ // local variable
			"if(uDoubleTap == 1)\n"+
			"{\n"+
			"vec3 normalizedTransformedNormals = normalize(otransformedNormals);\n"+
			"vec3 normalizedLightDirection = normalize(olightDirection);\n"+
			"vec3 normalizedViwerVector = normalize(oviewerVector);\n"+
			"vec3 ambientLight = ulightAmbient * uMaterialAmbient;\n"+
			"vec3 diffuseLight = ulightDiffuse * uMaterialDiffuse * max(dot(normalizedLightDirection, normalizedTransformedNormals),0.0);\n"+
			"vec3 reflectionVector = reflect(-normalizedLightDirection, normalizedTransformedNormals);\n"+
			"vec3 specularLight = ulightSpecular * uMaterialSpecular * pow(max(dot(reflectionVector, normalizedViwerVector), 0.0),uMaterialShininess);\n"+
			"PhongADSLight = ambientLight + diffuseLight + specularLight;\n "+
			"}\n"+
			"else\n"+
			"{\n"+
			"PhongADSLight = vec3(1.0, 1.0, 1.0);\n"+
			"}\n"+
			"vec3 texColor = vec3(texture(uTextureSampler, oTexCoord));"+
			//"FragColor = vec4(texColor * vec3(oColor)*PhongADSLight, 1.0);"+
			"vec3 color = vec3(oColor) * PhongADSLight;"+
			"FragColor = vec4(texColor * color, 1.0);"+
			"}"
		);



			/*"void main(void)"+
			"{"+
			"vec4 texColor=texture(uTextureSampler, oTexCoord);"+
			"FragColor = vec4(texColor * oColor);"+
			"}"*/

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

		GLES32.glBindAttribLocation(shaderProgramObject,VertexAttributesEnum.AMC_ATTRIBUTE_COLOR,"aColor");

		GLES32.glBindAttribLocation(shaderProgramObject,VertexAttributesEnum.AMC_ATTRIBUTE_NORMAL,"aNormal");

		GLES32.glBindAttribLocation(shaderProgramObject,VertexAttributesEnum.AMC_ATTRIBUTE_TEXCOORD,"aTexCoord");


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


		lightAmbientUniform = GLES32.glGetUniformLocation(shaderProgramObject, "ulightAmbient");


		lightDiffuseUniform = GLES32.glGetUniformLocation(shaderProgramObject, "ulightDiffuse");

		lightSpecularUniform = GLES32.glGetUniformLocation(shaderProgramObject, "ulightSpecular");


		lightPositionUniform = GLES32.glGetUniformLocation(shaderProgramObject, "ulightPosition");

		materialAmbientUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uMaterialAmbient");

		materialDiffuseUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uMaterialDiffuse");


		materialSpecularUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uMaterialSpecular");

		materialShininessUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uMaterialShininess");

		doubleTapnUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uDoubleTap");




		//geometry coordinates

		/*final float[] cubePosition = new float[]
		{
									// top
									 1.0f, 1.0f, -1.0f,
									 -1.0f, 1.0f, -1.0f,
									 -1.0f, 1.0f, 1.0f,
									 1.0f, 1.0f, 1.0f,

									 // bottom
									 1.0f, -1.0f, -1.0f,
									-1.0f, -1.0f, -1.0f,
									-1.0f, -1.0f,  1.0f,
									 1.0f, -1.0f,  1.0f,

									 // front
									 1.0f, 1.0f, 1.0f,
									-1.0f, 1.0f, 1.0f,
									-1.0f, -1.0f, 1.0f,
									 1.0f, -1.0f, 1.0f,

									 // back
									 1.0f, 1.0f, -1.0f,
									-1.0f, 1.0f, -1.0f,
									-1.0f, -1.0f, -1.0f,
									 1.0f, -1.0f, -1.0f,

									 // right
									 1.0f, 1.0f, -1.0f,
									 1.0f, 1.0f, 1.0f,
									 1.0f, -1.0f, 1.0f,
									 1.0f, -1.0f, -1.0f,

									 // left
									 -1.0f, 1.0f, 1.0f,
									 -1.0f, 1.0f, -1.0f,
									 -1.0f, -1.0f, -1.0f,
									 -1.0f, -1.0f, 1.0f

		};


		final float[] cube_texcoord = new float[]
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
			1.0f, 0.0f // bottom-right of bottom
		};


		final float[] cubeColor = new float[]
		{
			 0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,

			1.0f, 0.5f, 0.0f,
			1.0f, 0.5f, 0.0f,
			1.0f, 0.5f, 0.0f,
			1.0f, 0.5f, 0.0f,

			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,

			1.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 0.0f,

			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,

			1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f
		};


		final float[] cubeNormal = new float[]
		{
			// Top
			0.0f, 1.0f, 0.0f,  // Normal for top face
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,

			// Bottom
			0.0f, -1.0f, 0.0f, // Normal for bottom face
			0.0f, -1.0f, 0.0f,
			0.0f, -1.0f, 0.0f,
			0.0f, -1.0f, 0.0f,

			// Front
			0.0f, 0.0f, 1.0f,  // Normal for front face
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,

			// Back
			0.0f, 0.0f, -1.0f, // Normal for back face
			0.0f, 0.0f, -1.0f,
			0.0f, 0.0f, -1.0f,
			0.0f, 0.0f, -1.0f,

			// Right
			1.0f, 0.0f, 0.0f,  // Normal for right face
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,

			// Left
			-1.0f, 0.0f, 0.0f, // Normal for left face
			-1.0f, 0.0f, 0.0f,
			-1.0f, 0.0f, 0.0f,
			-1.0f, 0.0f, 0.0f
		};*/


		final float[] cube_PCNT = new float[]
		{
			// front
			// position				// color			 // normals				// texcoords
			 1.0f,  1.0f,  1.0f,	1.0f, 0.0f, 0.0f,	 0.0f,  0.0f,  1.0f,	1.0f, 1.0f,
			-1.0f,  1.0f,  1.0f,	1.0f, 0.0f, 0.0f,	 0.0f,  0.0f,  1.0f,	0.0f, 1.0f,
			-1.0f, -1.0f,  1.0f,	1.0f, 0.0f, 0.0f,	 0.0f,  0.0f,  1.0f,	0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f,	1.0f, 0.0f, 0.0f,	 0.0f,  0.0f,  1.0f,	1.0f, 0.0f,
						 
			// right			 
			// position				// color			 // normals				// texcoords
			 1.0f,  1.0f, -1.0f,	0.0f, 0.0f, 1.0f,	 1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
			 1.0f,  1.0f,  1.0f,	0.0f, 0.0f, 1.0f,	 1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f,	0.0f, 0.0f, 1.0f,	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
			 1.0f, -1.0f, -1.0f,	0.0f, 0.0f, 1.0f,	 1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
						 
			// back				 
			// position				// color			 // normals				// texcoords
			 1.0f,  1.0f, -1.0f,	1.0f, 1.0f, 0.0f,	 0.0f,  0.0f, -1.0f,	1.0f, 1.0f,
			-1.0f,  1.0f, -1.0f,	1.0f, 1.0f, 0.0f,	 0.0f,  0.0f, -1.0f,	0.0f, 1.0f,
			-1.0f, -1.0f, -1.0f,	1.0f, 1.0f, 0.0f,	 0.0f,  0.0f, -1.0f,	0.0f, 0.0f,
			 1.0f, -1.0f, -1.0f,	1.0f, 1.0f, 0.0f,	 0.0f,  0.0f, -1.0f,	1.0f, 0.0f,
						 
			// left				 
			// position				// color			 // normals				// texcoords
			-1.0f,  1.0f,  1.0f,	1.0f, 0.0f, 1.0f,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
			-1.0f,  1.0f, -1.0f,	1.0f, 0.0f, 1.0f,	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
			-1.0f, -1.0f, -1.0f,	1.0f, 0.0f, 1.0f,	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
			-1.0f, -1.0f,  1.0f,	1.0f, 0.0f, 1.0f,	-1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
						 
			// top				 
			// position				// color			 // normals				// texcoords
			 1.0f,  1.0f, -1.0f,	0.0f, 1.0f, 0.0f,	 0.0f,  1.0f,  0.0f,	1.0f, 1.0f,
			-1.0f,  1.0f, -1.0f,	0.0f, 1.0f, 0.0f,	 0.0f,  1.0f,  0.0f,	0.0f, 1.0f,
			-1.0f,  1.0f,  1.0f,	0.0f, 1.0f, 0.0f,	 0.0f,  1.0f,  0.0f,	0.0f, 0.0f,
			 1.0f,  1.0f,  1.0f,	0.0f, 1.0f, 0.0f,	 0.0f,  1.0f,  0.0f,	1.0f, 0.0f,
						 
			// bottom			 
			// position				// color			 // normals				// texcoords
			 1.0f, -1.0f,  1.0f,	1.0f, 0.5f, 0.0f,	 0.0f, -1.0f,  0.0f,	1.0f, 1.0f,
			-1.0f, -1.0f,  1.0f,	1.0f, 0.5f, 0.0f,	 0.0f, -1.0f,  0.0f,	0.0f, 1.0f,
			-1.0f, -1.0f, -1.0f,	1.0f, 0.5f, 0.0f,	 0.0f, -1.0f,  0.0f,	0.0f, 0.0f,
			 1.0f, -1.0f, -1.0f,	1.0f, 0.5f, 0.0f,	 0.0f, -1.0f,  0.0f,	1.0f, 0.0f
		};


		//vao

		GLES32.glGenVertexArrays(1,vao,0);

		//Bind with VAO

		GLES32.glBindVertexArray(vao[0]);

		//VBO for position

		GLES32.glGenBuffers(1, vbo_PCNT,0);
		
		// Bind with VBO of position

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_PCNT[0]);

		//prepare triangle vertices array for glBuffer data

		//step 1 : allocate enough size buffer for triangle vertices

		ByteBuffer byteBuffer = ByteBuffer.allocateDirect(cube_PCNT.length * 4);

		//step 2: set byte order

		byteBuffer.order(ByteOrder.nativeOrder());

		//step3 : convert created buffer as float array

		FloatBuffer pcntBuffer = byteBuffer.asFloatBuffer();

		//step 4 : fill buffer with vertices array

		pcntBuffer.put(cube_PCNT);

		//step 5 : rewind the buffer to 0th index

		pcntBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, cube_PCNT.length * 4, pcntBuffer, GLES32.GL_STATIC_DRAW);


		// ************************************** Position **************************************

		GLES32.glVertexAttribPointer(VertexAttributesEnum.AMC_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 11*4, 0);

		GLES32.glEnableVertexAttribArray(VertexAttributesEnum.AMC_ATTRIBUTE_POSITION);


		// ************************************** Color **************************************

		GLES32.glVertexAttribPointer(VertexAttributesEnum.AMC_ATTRIBUTE_COLOR, 3, GLES32.GL_FLOAT, false, 11 * 4, 3 * 4);

		GLES32.glEnableVertexAttribArray(VertexAttributesEnum.AMC_ATTRIBUTE_COLOR);

		// ************************************** Normal **************************************

		GLES32.glVertexAttribPointer(VertexAttributesEnum.AMC_ATTRIBUTE_NORMAL, 3, GLES32.GL_FLOAT, false, 11 * 4, 6 * 4);

		GLES32.glEnableVertexAttribArray(VertexAttributesEnum.AMC_ATTRIBUTE_NORMAL);

		// ************************************** Texcoord **************************************

		GLES32.glVertexAttribPointer(VertexAttributesEnum.AMC_ATTRIBUTE_TEXCOORD, 2, GLES32.GL_FLOAT, false, 11 * 4, 9 * 4);

		GLES32.glEnableVertexAttribArray(VertexAttributesEnum.AMC_ATTRIBUTE_TEXCOORD);

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
		
		//step 18 : unbind with VAO
		GLES32.glBindVertexArray(0);


		
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

		

		//depth setting

		GLES32.glClearDepthf(1.0f);

		GLES32.glEnable(GLES32.GL_DEPTH_TEST);

		GLES32.glDepthFunc(GLES32.GL_LEQUAL);


		//clear color

		GLES32.glClearColor(0.0f,0.0f,0.0f,1.0f);

		// call loadGLTexture();

		marble_texture[0] = loadGLTexture(R.raw.marble);  // on this (smiley) place kundali, stone etc will come as per your texture code.
                                                        //Capital R is res directory.. tya atli subdirectory raw.. smiley is name of our image


		Matrix.setIdentityM(perspectiveProjectionMatrix,0);

		

	}

	private int loadGLTexture(int imageResourceID)
	{
		// Create bitmap factory options object

		BitmapFactory.Options options = new BitmapFactory.Options();

		// Don't Scale The Image
		options.inScaled = false;

		// Create the bitmap image from image resource
		Bitmap bitmap = BitmapFactory.decodeResource(context.getResources(), imageResourceID, options);

		// Create Texture
		int[] texture = new int[1];

		// Create OpenGL Texture Object
		GLES32.glGenTextures(1, texture, 0);

		// Bind Texture
		GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, texture[0]);


		//  Decide image pixel alignment and unpacking
		GLES32.glPixelStorei(GLES32.GL_UNPACK_ALIGNMENT, 1);  // 4 is good for alignment, 1 is good for performance..

		// Set Texture Parameters
		GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_MAG_FILTER, GLES32.GL_LINEAR);

		GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_MIN_FILTER, GLES32.GL_LINEAR_MIPMAP_LINEAR);

		//  Create Multiple MipMapImages
		GLUtils.texImage2D(GLES32.GL_TEXTURE_2D, 0, bitmap, 0); // last parameter zero is for boarder width..

		GLES32.glGenerateMipmap(GLES32.GL_TEXTURE_2D);

		// Unbind with the OpenGL texture Object
		GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, 0);

		return(texture[0]);

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

		float[] rotationMatrixY = new float[16];

		float[] rotationMatrixX = new float[16];

		float[] rotationMatrixZ = new float[16];

		float[] finalRotationMatrix = new float[16];

		float[] viewMatrix = new float[16];

		//set to identity

		Matrix.setIdentityM(modelViewMatrix,0);

		Matrix.setIdentityM(translationMatrix,0);

		Matrix.setIdentityM(rotationMatrixY,0);

		Matrix.setIdentityM(rotationMatrixX,0);

		Matrix.setIdentityM(rotationMatrixZ,0);

		Matrix.setIdentityM(viewMatrix,0);

		//transformation

		Matrix.translateM(translationMatrix,0, 0.0f, 0.0f, -5.0f);

		Matrix.setRotateM(rotationMatrixY,0,angle_Cube, 0.0f, 1.0f, 0.0f);

		Matrix.setRotateM(rotationMatrixX,0,angle_Cube, 1.0f, 0.0f, 0.0f);

		Matrix.setRotateM(rotationMatrixZ,0,angle_Cube, 0.0f, 0.0f, 1.0f);

		Matrix.multiplyMM(finalRotationMatrix, 0, rotationMatrixX, 0, rotationMatrixY, 0);

		Matrix.multiplyMM(finalRotationMatrix, 0, finalRotationMatrix, 0, rotationMatrixZ, 0);

		Matrix.multiplyMM(modelViewMatrix, 0, translationMatrix, 0,finalRotationMatrix,0);


		//push above mvp into vertex shader's mvp uniform

		//GLES32.glUniformMatrix4fv(mvpMatrixUniform, 1, false, modelViewProjectionMatrix,0);

		GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelViewMatrix,0);

		GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix,0);

		GLES32.glUniformMatrix4fv(projectionMatrixUniform, 1, false, perspectiveProjectionMatrix,0);
		

		// for texture
		GLES32.glActiveTexture(GLES32.GL_TEXTURE0);

		GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, marble_texture[0]);

		GLES32.glUniform1i(textureSamplerUniform, 0);

		GLES32.glBindVertexArray(vao[0]);


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

		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 0, 4);

		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 4, 4);

		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 8, 4);

		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 12, 4);

		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 16, 4);

		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 20, 4);

		//step 4: unBind with (VAO)

		GLES32.glBindVertexArray(0);

		// unbind texture
		GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, 0);

		//step 5: Unuse shader program object
		GLES32.glUseProgram(0);


		//Render

		requestRender();

	}

    private void update()
	{
		// code
		if(angle_Cube < 360.0f)
		{
			angle_Cube = angle_Cube + 0.3f;
		}
		else
		{
			angle_Cube = 0.0f;
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

		if(vbo_PCNT[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo_PCNT, 0);
			vbo_PCNT[0] = 0;
		}

		if(vao[0] > 0)
		{
			GLES32.glDeleteVertexArrays(1, vao, 0);
			vao[0] = 0;
		}
	}

};

