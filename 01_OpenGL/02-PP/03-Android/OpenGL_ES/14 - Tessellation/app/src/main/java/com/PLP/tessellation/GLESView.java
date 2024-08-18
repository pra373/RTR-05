package com.PLP.tessellation;

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

	private int mvpMatrixUniform;

	private float[] perspectiveProjectionMatrix = new float [16];

	// tissilation related variables

	private int numberOfSegmentsUniform = 0;
	private int numberOfStripsUniform = 0;
	private int lineColorUniform = 0;

	private int uiNumberOfLineSegments;

	private int singleTapCount = 0;

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
		singleTapCount += 1;

		if(singleTapCount < 30)
		{
			uiNumberOfLineSegments += 1;
		}

		if(singleTapCount >= 30 && uiNumberOfLineSegments > 1)
		{
			uiNumberOfLineSegments -= 1;
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
			"in vec2 aPosition;"+
			"void main(void)"+
			"{"+
			"gl_Position = vec4(aPosition,0.0,1.0);"+
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

		//tessellation Control  shader

		final String tesselationControlShaderSourceCode = String.format
		(
			"#version 320 es"+ 
			"\n"+
			"precision highp float;"+
			"layout(vertices=4) out;"+
			"uniform int uNumberOfSegments;"+
			"uniform int uNumberOfStrips;"+
			"void main(void)"+
			"{"+
				"gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;"+
				"gl_TessLevelOuter[0] = float(uNumberOfStrips);"+
				"gl_TessLevelOuter[1] = float(uNumberOfSegments);"+
			"}"
		);

		int tesselationControlShaderObject = GLES32.glCreateShader(GLES32.GL_TESS_CONTROL_SHADER);

		GLES32.glShaderSource(tesselationControlShaderObject,tesselationControlShaderSourceCode);

		GLES32.glCompileShader(tesselationControlShaderObject);

		shaderCompileStatus[0] = 0;

		infoLogLength[0] = 0;

		infoLog = null;

		GLES32.glGetShaderiv(tesselationControlShaderObject,GLES32.GL_COMPILE_STATUS,shaderCompileStatus,0);

		if(shaderCompileStatus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetShaderiv(tesselationControlShaderObject, GLES32.GL_INFO_LOG_LENGTH, infoLogLength,0);

			if(infoLogLength[0] > 0)
			{
				infoLog = GLES32.glGetShaderInfoLog(tesselationControlShaderObject);

				System.out.println("PLP: Tessellation Control Shader Compilation Error : " + infoLog);

				uninitialize();

				System.exit(0);
			}
		}

		//tessellation evaluation  shader

		final String tesselationEvaluationShaderSourceCode = String.format
		(
			"#version 320 es"+ 
			"\n"+
			"precision highp float;"+
			"layout(isolines) in;"+
			"uniform mat4 uMVPMatrix;"+
			"void main(void)"+
			"{"+
				"vec3 p0 = gl_in[0].gl_Position.xyz;"+
				"vec3 p1 = gl_in[1].gl_Position.xyz;"+
				"vec3 p2 = gl_in[2].gl_Position.xyz;"+
				"vec3 p3 = gl_in[3].gl_Position.xyz;"+
				"float t = gl_TessCoord.x;"+
				"vec3 p  = p0 * (1.0 - t) * (1.0 - t) * (1.0 - t) + p1 * 3.0 * (t) * (1.0 - t) * (1.0 - t) + p2 * 3.0 * (t) * (t) * (1.0 - t) + p3 * (t) * (t) * (t);"+
				"gl_Position = uMVPMatrix * vec4(p,1.0);"+	
			"}"
		);

		int tesselationEvaluationShaderObject = GLES32.glCreateShader(GLES32.GL_TESS_EVALUATION_SHADER);

		GLES32.glShaderSource(tesselationEvaluationShaderObject,tesselationEvaluationShaderSourceCode);

		GLES32.glCompileShader(tesselationEvaluationShaderObject);

		shaderCompileStatus[0] = 0;

		infoLogLength[0] = 0;

		infoLog = null;

		GLES32.glGetShaderiv(tesselationEvaluationShaderObject,GLES32.GL_COMPILE_STATUS,shaderCompileStatus,0);

		if(shaderCompileStatus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetShaderiv(tesselationEvaluationShaderObject, GLES32.GL_INFO_LOG_LENGTH, infoLogLength,0);

			if(infoLogLength[0] > 0)
			{
				infoLog = GLES32.glGetShaderInfoLog(tesselationEvaluationShaderObject);

				System.out.println("PLP: Tessellation evaluation Shader Compilation Error : " + infoLog);

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
			"uniform vec4 uLineColor;"+
			"out vec4 FragColor;"+ 
			"void main(void)"+ 
			"{\n"+ 
				"FragColor = uLineColor;"+                       //vec4 is working as a constructor here not as a datatype
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

		GLES32.glAttachShader(shaderProgramObject, tesselationControlShaderObject);

		GLES32.glAttachShader(shaderProgramObject, tesselationEvaluationShaderObject);

		GLES32.glAttachShader(shaderProgramObject, fragmentShaderObject);

		GLES32.glBindAttribLocation(shaderProgramObject,VertexAttributesEnum.AMC_ATTRIBUTE_POSITION,"aPosition");

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

		mvpMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uMVPMatrix");

		 numberOfSegmentsUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uNumberOfSegments");

		numberOfStripsUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uNumberOfStrips");

		 lineColorUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uLineColor");

		//geometry coordinates

		final float[] pointPosition = new float[]
		{
			 -1.0f,-1.0f,  //third quadrant
			 -0.5f,1.0f,   //second quadrant
			 0.5f,-1.0f,  //fourth quadrant
			 1.0f,1.0f    //first quadrant
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

		ByteBuffer byteBuffer = ByteBuffer.allocateDirect(pointPosition.length * 4);

		//step 2: set byte order

		byteBuffer.order(ByteOrder.nativeOrder());

		//step3 : convert created buffer as float array

		FloatBuffer positionBuffer = byteBuffer.asFloatBuffer();

		//step 4 : fill buffer with vertices array

		positionBuffer.put(pointPosition);

		//step 5 : rewind the buffer to 0th index

		positionBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, pointPosition.length * 4, positionBuffer, GLES32.GL_STATIC_DRAW);

		GLES32.glVertexAttribPointer(VertexAttributesEnum.AMC_ATTRIBUTE_POSITION, 2, GLES32.GL_FLOAT, false, 0, 0);

		GLES32.glEnableVertexAttribArray(VertexAttributesEnum.AMC_ATTRIBUTE_POSITION);

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

		
		//step 18 : unbind with VAO
		GLES32.glBindVertexArray(0);

		//depth setting

		GLES32.glClearDepthf(1.0f);

		GLES32.glEnable(GLES32.GL_DEPTH_TEST);

		GLES32.glDepthFunc(GLES32.GL_LEQUAL);


		uiNumberOfLineSegments = 1;     // for 1 line

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
		float[] modelViewProjectionMatrix = new float[16];

		Matrix.setIdentityM(modelViewMatrix,0);
		Matrix.setIdentityM(modelViewProjectionMatrix,0);

		Matrix.translateM(modelViewMatrix,0, 0.0f, 0.0f, -3.5f);
		Matrix.multiplyMM(modelViewProjectionMatrix, 0, perspectiveProjectionMatrix, 0, modelViewMatrix, 0);

		//push above mvp into vertex shader's mvp uniform

		GLES32.glUniformMatrix4fv(mvpMatrixUniform, 1, false, modelViewProjectionMatrix,0);

		GLES32.glUniform1i(numberOfSegmentsUniform, uiNumberOfLineSegments);

		GLES32.glUniform1i(numberOfStripsUniform, 1);

		float [] lineColor = {0.0f,1.0f,0.0f,1.0f};

		if(uiNumberOfLineSegments == 1)
		{
			GLES32.glUniform4fv(lineColorUniform, 1,lineColor,0);
		}

		if(uiNumberOfLineSegments == 30)
		{
			lineColor[0] = 1.0f;
			lineColor[1] = 0.0f;
			lineColor[2] = 0.0f;
			lineColor[3] = 1.0f;

			GLES32.glUniform4fv(lineColorUniform, 1,lineColor,0);
		}

		if(uiNumberOfLineSegments > 1 && uiNumberOfLineSegments < 30)
		{
			lineColor[0] = 1.0f;
			lineColor[1] = 1.0f;
			lineColor[2] = 0.0f;
			lineColor[3] = 1.0f;

			GLES32.glUniform4fv(lineColorUniform, 1,lineColor,0);
		}

	
		GLES32.glPatchParameteri(GLES32.GL_PATCH_VERTICES, 4);
	

		GLES32.glBindVertexArray(vao[0]);

		//step 3: Draw the geometry

		GLES32.glDrawArrays(GLES32.GL_PATCHES, 0, 4);

		//step 4: unBind with (VAO)

		GLES32.glBindVertexArray(0);

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

		if(vbo[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo, 0);
			vbo[0] = 0;
		}

		if(vao[0] > 0)
		{
			GLES32.glDeleteVertexArrays(1, vao, 0);
			vao[0] = 0;
		}
	}

};

