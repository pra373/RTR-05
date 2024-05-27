package com.PLP.diffuselight_pyramid;

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

public class GLESView extends GLSurfaceView implements GLSurfaceView.Renderer, OnDoubleTapListener, OnGestureListener
{
	private GestureDetector gestureDetector = null;

	//variable declarations

	private int shaderProgramObject;

	private int[] vao = new int[1];

	private int[] vbo = new int[1];

	private int[] vbo_Normal = new int[1];

	//projection related uniforms

	private int modelViewMatrixUniform;

	private int projectionMatrixUniform;

	//private int mvpMatrixUniform;

	private float[] perspectiveProjectionMatrix = new float [16];

	private float angle_Pyramid = 0.0f;

	//light uniform variables

	private int ldUniform = 0;

	private int kdUniform = 0;

	private int lightPositionUniform = 0;

	private int doubleTapnUniform = 0;

	private boolean bLightingEnabled = false;

	private boolean bAnimationEnabled = false;

	private float[] lightPosition = new float[] {0.0f,0.0f,2.0f,1.0f};

	private float[] materialDiffuse = new float[] {1.0f,0.5f,0.5f,1.0f};

	private float [] lightDiffuse = new float[] { 0.0f,1.0f,1.0f,1.0f };

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
			"uniform mat4 umodelViewMatrix;"+
			"uniform mat4 uProjectionMatrix;"+
			"uniform vec3 uLd;"+
			"uniform vec3 uKd;"+
			"uniform vec4 uLightPosition;"+
			"uniform highp int uDoubleTap;"+
			"out vec3 oDiffuseLight;"+
			"void main(void)"+
			"{"+
			"if(uDoubleTap == 1)"+
			"{"+
			    "vec4 eyePosition = umodelViewMatrix * aPosition;"+
				"mat3 normalMatrix = mat3(transpose(inverse(umodelViewMatrix)));"+
				"vec3 n = normalize(normalMatrix * aNormal);"+
				"vec3 s = normalize(vec3(uLightPosition - eyePosition));"+
				"oDiffuseLight = uLd * uKd * dot(s,n);"+
			"}"+
			"else"+
			"{"+
			    "oDiffuseLight = vec3(0.0, 0.0, 0.0);"+
			"}"+
			" gl_Position = uProjectionMatrix * umodelViewMatrix * aPosition;"+
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
			"in vec3 oDiffuseLight;"+
			"uniform highp int uDoubleTap;"+
			"out vec4 FragColor;"+
			"void main(void)"+ 
			"{"+
			"if(uDoubleTap == 1)"+
			"{"+
				"FragColor = vec4(oDiffuseLight,1.0);"+
			"}"+
			"else"+
			"{"+
			    "FragColor = vec4(1.0, 1.0, 1.0, 1.0);"+
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

		GLES32.glBindAttribLocation(shaderProgramObject,VertexAttributesEnum.AMC_ATTRIBUTE_NORMAL,"aNormal");

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

		modelViewMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObject, "umodelViewMatrix");

		projectionMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uProjectionMatrix");

		ldUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uLd");

		kdUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uKd");

		lightPositionUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uLightPosition");

		doubleTapnUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uDoubleTap");

		//geometry coordinates

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

		//normals

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
		-0.894427f, 0.447214f,  0.000000f, // left-right
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

		GLES32.glVertexAttribPointer(VertexAttributesEnum.AMC_ATTRIBUTE_NORMAL, 3, GLES32.GL_FLOAT, false, 0, 0);

		GLES32.glEnableVertexAttribArray(VertexAttributesEnum.AMC_ATTRIBUTE_NORMAL);

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);


		
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

		GLES32.glUseProgram(shaderProgramObject);

		//transformations

		float[] modelViewMatrix = new float[16];
		
		float[] rotationMatrix = new float[16];

		float[] translationMatrix = new float[16];

		//set to identity

		Matrix.setIdentityM(modelViewMatrix,0);

		Matrix.setIdentityM(translationMatrix,0);
		
		Matrix.setIdentityM(rotationMatrix, 0); // Initialize rotation matrix

		//transformation

		Matrix.translateM(translationMatrix,0, 0.0f, 0.0f, -4.5f);

		Matrix.rotateM(rotationMatrix, 0, angle_Pyramid, 0.0f, 1.0f, 0.0f); // Rotate around Y axis

		// Combine rotation with modelViewMatrix
		Matrix.multiplyMM(modelViewMatrix, 0, translationMatrix, 0, rotationMatrix, 0);	

		

		//push above mvp into vertex shader's mvp uniform

		GLES32.glUniformMatrix4fv(modelViewMatrixUniform, 1, false, modelViewMatrix,0);

		GLES32.glUniformMatrix4fv(projectionMatrixUniform, 1, false, perspectiveProjectionMatrix,0);

		//convert light and paterial properties arrays to float buffers

		
		//light diffuse conversion
		ByteBuffer byteBuffer_LD = ByteBuffer.allocateDirect(lightDiffuse.length * 4);

		

		byteBuffer_LD.order(ByteOrder.nativeOrder());

		

		FloatBuffer lightDiffuseBuffer = byteBuffer_LD.asFloatBuffer();

		

		lightDiffuseBuffer.put(lightDiffuse);

		

		lightDiffuseBuffer.position(0);

		//material diffuse conversion

		ByteBuffer byteBuffer_MD = ByteBuffer.allocateDirect(materialDiffuse.length * 4);

		

		byteBuffer_MD.order(ByteOrder.nativeOrder());

		

		FloatBuffer materialDiffuseBuffer = byteBuffer_MD.asFloatBuffer();

		

		materialDiffuseBuffer.put(materialDiffuse);

		

		materialDiffuseBuffer.position(0);

		//light position conversion

		ByteBuffer byteBuffer_LP = ByteBuffer.allocateDirect(lightPosition.length * 4);

		

		byteBuffer_LP.order(ByteOrder.nativeOrder());

		

		FloatBuffer lightPositionBuffer = byteBuffer_LP.asFloatBuffer();

		

		lightPositionBuffer.put(lightPosition);

		

		lightPositionBuffer.position(0);



		GLES32.glBindVertexArray(vao[0]);

		if(bLightingEnabled == true)
		{
			GLES32.glUniform1i(doubleTapnUniform, 1);
			GLES32.glUniform3fv(ldUniform, 1, lightDiffuseBuffer);
			GLES32.glUniform3fv(kdUniform, 1, materialDiffuseBuffer);
			GLES32.glUniform4fv(lightPositionUniform, 1, lightPositionBuffer);
		}

		else
		{
			GLES32.glUniform1i(doubleTapnUniform, 0);
		}

		//step 3: Draw the geometry

		GLES32.glDrawArrays(GLES32.GL_TRIANGLES, 0, 12);

		//step 4: unBind with (VAO)

		GLES32.glBindVertexArray(0);

		//step 5: Unuse shader program object
		GLES32.glUseProgram(0);


		//Render

		requestRender();

	}

	private void update()
	{
		angle_Pyramid = angle_Pyramid + 0.5f;

		if(angle_Pyramid >= 360.0f)
		{
			angle_Pyramid = 0.0f;
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

