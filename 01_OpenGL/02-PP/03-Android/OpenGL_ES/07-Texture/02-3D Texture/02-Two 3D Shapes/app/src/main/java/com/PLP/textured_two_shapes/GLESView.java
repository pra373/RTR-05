package com.PLP.textured_two_shapes;

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


import android.opengl.GLUtils;

public class GLESView extends GLSurfaceView implements GLSurfaceView.Renderer, OnDoubleTapListener, OnGestureListener
{
	private GestureDetector gestureDetector = null;

	//variable declarations

	private int shaderProgramObject;

	private int[] vao = new int[1];

	private int[] vbo = new int[1];

	private int[] vbo_Texture = new int[1];

	//VAO VBO for pyramid

	private int[] vao_Pyramid = new int[1];

	private int[] vbo_Pyramid_Position = new int[1];

	private int[] vbo_Pyramid_Texture = new int[1];

	private int mvpMatrixUniform;

	private final Context context;

	private float[] perspectiveProjectionMatrix = new float [16];

	//texture variable

	private int[] kundaliTexture = new int[1];

	private int[] stoneTexture = new int[1];


	private int textureSamplerUniform = 0;

	//rotation variable
	private float angleCube = 360.0f;

	
	private float anglePyramid = 360.0f;

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
			"in vec2 aTexCoord;"+
			"out vec2 oTexCoord;"+
			"uniform mat4 uMVPMatrix;"+          
			"void main(void)"+
			"{"+
			"gl_Position=uMVPMatrix*aPosition;"+
			"oTexCoord = aTexCoord;"+
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
			"in vec2 oTexCoord;"+
			"uniform highp sampler2D uTextureSampler;"+
			"out vec4 FragColor;"+
			"void main(void)"+ 
			"{"+
			"FragColor = texture(uTextureSampler, oTexCoord);"+                     
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

		GLES32.glBindAttribLocation(shaderProgramObject,VertexAttributesEnum.AMC_ATTRIBUTE_TEXTURE,"aTexCoord");

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

		textureSamplerUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uTextureSampler");

		if (textureSamplerUniform == -1)
		{
			System.out.println("PLP: Failed to get texture sampler uniform : " + infoLog);

			uninitialize();

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

		//geometry coordinates

		final float[] pyramidPosition = new float[]
		{
				 // front
			 0.0f,  1.0f,  0.0f, // front-top
			-1.0f, -1.0f,  1.0f, // front-left
			 1.0f, -1.0f,  1.0f, // front-right

			 // right
			 0.0f,  1.0f,  0.0f, // right-top
			 1.0f, -1.0f,  1.0f, // right-left
			 1.0f, -1.0f, -1.0f, // right-right

			 // back
			 0.0f,  1.0f,  0.0f, // back-top
			 1.0f, -1.0f, -1.0f, // back-left
			-1.0f, -1.0f, -1.0f, // back-right

			// left
			0.0f,  1.0f,  0.0f, // left-top
		   -1.0f, -1.0f, -1.0f, // left-left
		   -1.0f, -1.0f,  1.0f // left-right
		};

		final float[] pyramidTexCoord = new float[]
		{
			  // front
			 0.5f, 1.0f, // front-top
			 0.0f, 0.0f, // front-left
			 1.0f, 0.0f, // front-right

			 // right
			 0.5f, 1.0f, // right-top
			 1.0f, 0.0f, // right-left
			 0.0f, 0.0f, // right-right

			 // back
			 0.5f, 1.0f, // back-top
			 0.0f, 0.0f, // back-left
			 1.0f, 0.0f, // back-right

			 // left
			 0.5f, 1.0f, // left-top
			 1.0f, 0.0f, // left-left
			 0.0f, 0.0f // left-right
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

		GLES32.glGenBuffers(1, vbo_Texture,0);

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_Texture[0]);

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

		//vao and vbo for pyramid

		//vao

		GLES32.glGenVertexArrays(1,vao_Pyramid,0);

		//Bind with VAO

		GLES32.glBindVertexArray(vao_Pyramid[0]);

		//VBO for position

		GLES32.glGenBuffers(1, vbo_Pyramid_Position,0);
		
		// Bind with VBO of position

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_Pyramid_Position[0]);

		//prepare triangle vertices array for glBuffer data

		//step 1 : allocate enough size buffer for triangle vertices

		ByteBuffer byteBuffer_PP = ByteBuffer.allocateDirect(pyramidPosition.length * 4);

		//step 2: set byte order

		byteBuffer_PP.order(ByteOrder.nativeOrder());

		//step3 : convert created buffer as float array

		FloatBuffer positionBuffer_PP = byteBuffer_PP.asFloatBuffer();

		//step 4 : fill buffer with vertices array

		positionBuffer_PP.put(pyramidPosition);

		//step 5 : rewind the buffer to 0th index

		positionBuffer_PP.position(0);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, pyramidPosition.length * 4, positionBuffer_PP, GLES32.GL_STATIC_DRAW);

		GLES32.glVertexAttribPointer(VertexAttributesEnum.AMC_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);

		GLES32.glEnableVertexAttribArray(VertexAttributesEnum.AMC_ATTRIBUTE_POSITION);

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

		//VBO for texture

		GLES32.glGenBuffers(1, vbo_Pyramid_Texture,0);

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_Pyramid_Texture[0]);

		ByteBuffer byteBuffer_TP = ByteBuffer.allocateDirect(pyramidTexCoord.length * 4);

		//step 2: set byte order

		byteBuffer_TP.order(ByteOrder.nativeOrder());

		//step3 : convert created buffer as float array

		FloatBuffer texCoordBuffer_P = byteBuffer_TP.asFloatBuffer();

		//step 4 : fill buffer with vertices array

		texCoordBuffer_P.put(pyramidTexCoord);

		//step 5 : rewind the buffer to 0th index

		texCoordBuffer_P.position(0);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, pyramidTexCoord.length * 4, texCoordBuffer_P, GLES32.GL_STATIC_DRAW);

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

		GLES32.glClearColor(0.0f,0.0f,0.0f,1.0f);

		//load texture

		kundaliTexture[0] = loadGLTexture(R.raw.kundali);

		stoneTexture[0] = loadGLTexture(R.raw.stone);

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

		//transformations for cube

		float[] modelViewMatrix = new float[16];
		float[] modelViewProjectionMatrix = new float[16];
		float[] rotationMatrix = new float[16];

		Matrix.setIdentityM(modelViewMatrix,0);
		Matrix.setIdentityM(modelViewProjectionMatrix,0);
		Matrix.setIdentityM(rotationMatrix, 0); // Initialize rotation matrix

		Matrix.translateM(modelViewMatrix,0, 2.0f, 0.0f, -7.5f);

		Matrix.rotateM(rotationMatrix, 0, angleCube, 1.0f, 1.0f, 1.0f); // Rotate around Y axis

		// Combine rotation with modelViewMatrix
		Matrix.multiplyMM(modelViewMatrix, 0, modelViewMatrix, 0, rotationMatrix, 0);	

		Matrix.multiplyMM(modelViewProjectionMatrix, 0, perspectiveProjectionMatrix, 0, modelViewMatrix, 0);

		//push above mvp into vertex shader's mvp uniform

		GLES32.glUniformMatrix4fv(mvpMatrixUniform, 1, false, modelViewProjectionMatrix,0);

		//for texture

		GLES32.glActiveTexture(GLES32.GL_TEXTURE0);

		GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, kundaliTexture[0]);

		GLES32.glUniform1i(textureSamplerUniform, 0);


		GLES32.glBindVertexArray(vao[0]);

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


		//transformations for pyramid

		Matrix.setIdentityM(modelViewMatrix,0);
		Matrix.setIdentityM(modelViewProjectionMatrix,0);
		Matrix.setIdentityM(rotationMatrix, 0); // Initialize rotation matrix

		Matrix.translateM(modelViewMatrix,0, -1.8f, 0.0f, -6.0f);

		Matrix.rotateM(rotationMatrix, 0, anglePyramid, 0.0f, 1.0f, 0.0f); // Rotate around Y axis

		// Combine rotation with modelViewMatrix
		Matrix.multiplyMM(modelViewMatrix, 0, modelViewMatrix, 0, rotationMatrix, 0);	

		Matrix.multiplyMM(modelViewProjectionMatrix, 0, perspectiveProjectionMatrix, 0, modelViewMatrix, 0);

		//push above mvp into vertex shader's mvp uniform

		GLES32.glUniformMatrix4fv(mvpMatrixUniform, 1, false, modelViewProjectionMatrix,0);

		
		GLES32.glActiveTexture(GLES32.GL_TEXTURE0);

		GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, stoneTexture[0]);

		GLES32.glUniform1i(textureSamplerUniform, 0);

		GLES32.glBindVertexArray(vao_Pyramid[0]);

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
		angleCube = angleCube - 0.4f;

	    if(angleCube <= 0.0f)
	    {
			angleCube = 360.0f;
	    }

		anglePyramid = anglePyramid - 0.4f;

	    if(anglePyramid <= 0.0f)
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

		if (vbo_Texture[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo_Texture,0);
			vbo_Texture[0] = 0;
		}

		if(kundaliTexture[0] > 0)
		{
			GLES32.glDeleteTextures(1, kundaliTexture,0);
			kundaliTexture[0] = 0;
		}

		if(stoneTexture[0] > 0)
		{
			GLES32.glDeleteTextures(1, stoneTexture,0);
			stoneTexture[0] = 0;
		}

		if(vbo_Pyramid_Texture[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo_Pyramid_Texture, 0);
			vbo_Pyramid_Texture[0] = 0;
		}

		if(vbo_Pyramid_Position[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo_Pyramid_Position, 0);
			vbo_Pyramid_Position[0] = 0;
		}

		if(vbo[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo, 0);
			vbo[0] = 0;
		}

		if(vao_Pyramid[0] > 0)
		{
			GLES32.glDeleteVertexArrays(1, vao_Pyramid, 0);
			vao_Pyramid[0] = 0;
		}

		if(vao[0] > 0)
		{
			GLES32.glDeleteVertexArrays(1, vao, 0);
			vao[0] = 0;
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

