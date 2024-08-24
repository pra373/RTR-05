package com.PLP.robotic_arm;

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

	private int mvpMatrixUniform;

	private int colorUniform;

	private float[] perspectiveProjectionMatrix = new float [16];

	

	//sphere related variables

	private int[] vao_sphere = new int[1];
    private int[] vbo_sphere_position = new int[1];
    private int[] vbo_sphere_normal = new int[1];
    private int[] vbo_sphere_element = new int[1];

	private int numVertices = 0;

	private int numElements = 0;

	//Push Pop related variables

	private int MODEL_VIEW_MATRIX_STACK = 32;

	private float[][] matrixStack = new float[MODEL_VIEW_MATRIX_STACK][16];

	int matrixStackTop = -1;

	//rotation related variables

	//robotic arm variables

	float shoulder = 0.0f;
	float elbow = 0.0f;
	float wrist = 0.0f;


	//long press variable

	int singleTap = 0;

	//update variables

	int earthRevolution = 0;

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
			"uniform mat4 uMVPMatrix;"+          
			"void main(void)"+
			"{"+
			"gl_Position=uMVPMatrix*aPosition;"+
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
			"out vec4 FragColor;"+
			"uniform vec4 colorUniform;"+
			"void main(void)"+ 
			"{"+
			"FragColor = colorUniform;"+                     
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

		colorUniform = GLES32.glGetUniformLocation(shaderProgramObject, "colorUniform");

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

		//depth setting

		GLES32.glClearDepthf(1.0f);

		GLES32.glEnable(GLES32.GL_DEPTH_TEST);

		GLES32.glDepthFunc(GLES32.GL_LEQUAL);

		//initialize matrix stack

		initializeMatrixStack();

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

		//transformations for shoulder 

		float[] modelMatrix = new float[16];
		float[] viewMatrix = new float[16];
		float[] translationMatrix = new float[16];
		float[] modelViewProjectionMatrix = new float[16];
		float[] rotationMatrix = new float[16];
		float[] scaleMatrix = new float[16];

		Matrix.setIdentityM(modelMatrix,0);
		Matrix.setIdentityM(viewMatrix,0);
		Matrix.setIdentityM(translationMatrix,0);
		Matrix.setIdentityM(modelViewProjectionMatrix,0);
		Matrix.setIdentityM(rotationMatrix,0);
		Matrix.setIdentityM(scaleMatrix,0);
		

		Matrix.translateM(translationMatrix,0, 0.0f, 0.0f, -4.0f);

		System.arraycopy(translationMatrix,0,modelMatrix,0,16);	

		pushMatrix(modelMatrix);
		{

			Matrix.setRotateM(rotationMatrix,0, shoulder, 0.0f, 0.0f, 1.0f);
			Matrix.translateM(translationMatrix, 0, 1.0f, 0.0f, 0.0f);
			
			Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, rotationMatrix, 0);
			Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, translationMatrix, 0);

			//System.arraycopy(tempMatrix1,0,modelMatrix,0,16);

			pushMatrix(modelMatrix);
			{

				
				Matrix.scaleM(scaleMatrix,0,2.0f,0.5f,1.0f);

				Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, scaleMatrix, 0);

				Matrix.multiplyMM(modelViewProjectionMatrix, 0, perspectiveProjectionMatrix, 0, viewMatrix, 0);
				Matrix.multiplyMM(modelViewProjectionMatrix, 0, modelViewProjectionMatrix, 0, modelMatrix, 0);

				GLES32.glUniformMatrix4fv(mvpMatrixUniform, 1, false, modelViewProjectionMatrix,0);

				GLES32.glUniform4f(colorUniform, 0.8f, 0.6f, 0.4f, 1.0f);

				GLES32.glBindVertexArray(vao_sphere[0]);

				GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
				GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

				GLES32.glBindVertexArray(0);
			}
			Matrix.setIdentityM(modelViewProjectionMatrix, 0);

			modelMatrix = popMatrix();


			//transformation for forarm

			Matrix.setIdentityM(translationMatrix,0);
			

			Matrix.translateM(translationMatrix,0,1.0f,0.0f,0.0f);

			Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, translationMatrix, 0);

			Matrix.setIdentityM(rotationMatrix,0);

			Matrix.setRotateM(rotationMatrix,0,elbow,0.0f,0.0f,1.0f);

			Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, rotationMatrix, 0);

			Matrix.setIdentityM(translationMatrix,0);

			Matrix.translateM(translationMatrix,0, 1.0f,0.0f,0.0f);

		
			Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, translationMatrix, 0);


			//draw arm
		
			pushMatrix(modelMatrix);
			{

			
				Matrix.setIdentityM(scaleMatrix,0);

				Matrix.scaleM(scaleMatrix,0,2.0f,0.5f,1.0f);

				Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, scaleMatrix, 0);

				Matrix.setIdentityM(modelViewProjectionMatrix, 0);

				Matrix.multiplyMM(modelViewProjectionMatrix, 0, perspectiveProjectionMatrix, 0, viewMatrix, 0);
				Matrix.multiplyMM(modelViewProjectionMatrix, 0, modelViewProjectionMatrix, 0, modelMatrix, 0);

				GLES32.glUniformMatrix4fv(mvpMatrixUniform, 1, false, modelViewProjectionMatrix,0);

				GLES32.glUniform4f(colorUniform, 0.8f, 0.6f, 0.4f, 1.0f);

				GLES32.glBindVertexArray(vao_sphere[0]);

				GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
				GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

				GLES32.glBindVertexArray(0);

			}
			Matrix.setIdentityM(modelViewProjectionMatrix, 0);

			modelMatrix = popMatrix();


			// do transformation for wrist

			Matrix.setIdentityM(translationMatrix,0);

			Matrix.translateM(translationMatrix,0,1.0f,0.0f,0.0f);

			Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, translationMatrix, 0);

			Matrix.setIdentityM(rotationMatrix,0);

			Matrix.setRotateM(rotationMatrix,0,wrist,0.0f,0.0f,1.0f);

			Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, rotationMatrix, 0);

			Matrix.setIdentityM(translationMatrix,0);

			Matrix.translateM(translationMatrix,0, 0.25f, 0.0f, 0.0f);

			Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, translationMatrix, 0);

			//draw wrist
			
			pushMatrix(modelMatrix);
			{

				Matrix.setIdentityM(scaleMatrix,0);

				Matrix.scaleM(scaleMatrix,0,0.5f,0.5f,0.5f);

				Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, scaleMatrix, 0);

				Matrix.setIdentityM(modelViewProjectionMatrix,0);


				Matrix.multiplyMM(modelViewProjectionMatrix, 0, perspectiveProjectionMatrix, 0, viewMatrix, 0);

				Matrix.multiplyMM(modelViewProjectionMatrix, 0, modelViewProjectionMatrix, 0, modelMatrix, 0);

				GLES32.glUniformMatrix4fv(mvpMatrixUniform, 1, false, modelViewProjectionMatrix,0);

				GLES32.glBindVertexArray(vao_sphere[0]);

				GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
				GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

				GLES32.glBindVertexArray(0);
			}
			Matrix.setIdentityM(modelViewProjectionMatrix, 0);

			modelMatrix = popMatrix();


			// ******************* Draw Fingure ******************************

			// draw Thumb
			
			pushMatrix(modelMatrix);
			{

				Matrix.setIdentityM(rotationMatrix,0);

				Matrix.setRotateM(rotationMatrix,0,90.0f,0.0f,0.0f,1.0f);

				Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, rotationMatrix, 0);

				Matrix.setIdentityM(translationMatrix,0);

				Matrix.translateM(translationMatrix,0,0.45f,0.0f,0.0f);

				Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, translationMatrix, 0);

				Matrix.setIdentityM(scaleMatrix,0);

				
				Matrix.scaleM(scaleMatrix,0,0.35f,0.13f,0.25f);


				Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, scaleMatrix, 0);


				Matrix.setIdentityM(modelViewProjectionMatrix,0);


				Matrix.multiplyMM(modelViewProjectionMatrix, 0, perspectiveProjectionMatrix, 0, viewMatrix, 0);

				Matrix.multiplyMM(modelViewProjectionMatrix, 0, modelViewProjectionMatrix, 0, modelMatrix, 0);

				GLES32.glUniformMatrix4fv(mvpMatrixUniform, 1, false, modelViewProjectionMatrix,0);

				GLES32.glBindVertexArray(vao_sphere[0]);

				GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
				GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

				GLES32.glBindVertexArray(0);
			}
			Matrix.setIdentityM(modelViewProjectionMatrix, 0);

			modelMatrix = popMatrix();


			// draw Index Fingure
			
			pushMatrix(modelMatrix);
			{

				Matrix.setIdentityM(rotationMatrix,0);

				Matrix.setRotateM(rotationMatrix,0,180.0f,0.0f,0.0f,1.0f);

				Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, rotationMatrix, 0);

				Matrix.setIdentityM(translationMatrix,0);

				Matrix.translateM(translationMatrix,0,-0.5f,-0.2f,0.0f);

				Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, translationMatrix, 0);

				Matrix.setIdentityM(scaleMatrix,0);

				
				Matrix.scaleM(scaleMatrix,0,0.42f,0.13f,0.25f);


				Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, scaleMatrix, 0);


				Matrix.setIdentityM(modelViewProjectionMatrix,0);


				Matrix.multiplyMM(modelViewProjectionMatrix, 0, perspectiveProjectionMatrix, 0, viewMatrix, 0);

				Matrix.multiplyMM(modelViewProjectionMatrix, 0, modelViewProjectionMatrix, 0, modelMatrix, 0);

				GLES32.glUniformMatrix4fv(mvpMatrixUniform, 1, false, modelViewProjectionMatrix,0);

				GLES32.glBindVertexArray(vao_sphere[0]);

				GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
				GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

				GLES32.glBindVertexArray(0);
			}
			Matrix.setIdentityM(modelViewProjectionMatrix, 0);

			modelMatrix = popMatrix();


			// draw Middle Fingure
			
			pushMatrix(modelMatrix);
			{

				Matrix.setIdentityM(rotationMatrix,0);

				Matrix.setRotateM(rotationMatrix,0,180.0f,0.0f,0.0f,1.0f);

				Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, rotationMatrix, 0);

				Matrix.setIdentityM(translationMatrix,0);

				Matrix.translateM(translationMatrix,0,-0.55f,-0.01f,0.0f);

				Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, translationMatrix, 0);

				Matrix.setIdentityM(scaleMatrix,0);

				
				Matrix.scaleM(scaleMatrix,0,0.42f,0.13f,0.25f);


				Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, scaleMatrix, 0);


				Matrix.setIdentityM(modelViewProjectionMatrix,0);


				Matrix.multiplyMM(modelViewProjectionMatrix, 0, perspectiveProjectionMatrix, 0, viewMatrix, 0);

				Matrix.multiplyMM(modelViewProjectionMatrix, 0, modelViewProjectionMatrix, 0, modelMatrix, 0);

				GLES32.glUniformMatrix4fv(mvpMatrixUniform, 1, false, modelViewProjectionMatrix,0);

				GLES32.glBindVertexArray(vao_sphere[0]);

				GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
				GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

				GLES32.glBindVertexArray(0);
			}
			Matrix.setIdentityM(modelViewProjectionMatrix, 0);

			modelMatrix = popMatrix();


			// draw Ring Fingure
			
			pushMatrix(modelMatrix);
			{

				Matrix.setIdentityM(rotationMatrix,0);

				Matrix.setRotateM(rotationMatrix,0,180.0f,0.0f,0.0f,1.0f);

				Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, rotationMatrix, 0);

				Matrix.setIdentityM(translationMatrix,0);

				Matrix.translateM(translationMatrix,0,-0.55f,0.15f,0.0f);

				Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, translationMatrix, 0);

				Matrix.setIdentityM(scaleMatrix,0);

				
				Matrix.scaleM(scaleMatrix,0,0.42f,0.13f,0.25f);


				Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, scaleMatrix, 0);


				Matrix.setIdentityM(modelViewProjectionMatrix,0);


				Matrix.multiplyMM(modelViewProjectionMatrix, 0, perspectiveProjectionMatrix, 0, viewMatrix, 0);

				Matrix.multiplyMM(modelViewProjectionMatrix, 0, modelViewProjectionMatrix, 0, modelMatrix, 0);

				GLES32.glUniformMatrix4fv(mvpMatrixUniform, 1, false, modelViewProjectionMatrix,0);

				GLES32.glBindVertexArray(vao_sphere[0]);

				GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
				GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

				GLES32.glBindVertexArray(0);
			}
			Matrix.setIdentityM(modelViewProjectionMatrix, 0);

			modelMatrix = popMatrix();


			// draw Little Fingure
			
			pushMatrix(modelMatrix);
			{

				Matrix.setIdentityM(rotationMatrix,0);

				Matrix.setRotateM(rotationMatrix,0,180.0f,0.0f,0.0f,1.0f);

				Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, rotationMatrix, 0);

				Matrix.setIdentityM(translationMatrix,0);

				Matrix.translateM(translationMatrix,0,-0.44f,0.3f,0.0f);

				Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, translationMatrix, 0);

				Matrix.setIdentityM(scaleMatrix,0);

				
				Matrix.scaleM(scaleMatrix,0,0.42f,0.13f,0.25f);


				Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, scaleMatrix, 0);


				Matrix.setIdentityM(modelViewProjectionMatrix,0);


				Matrix.multiplyMM(modelViewProjectionMatrix, 0, perspectiveProjectionMatrix, 0, viewMatrix, 0);

				Matrix.multiplyMM(modelViewProjectionMatrix, 0, modelViewProjectionMatrix, 0, modelMatrix, 0);

				GLES32.glUniformMatrix4fv(mvpMatrixUniform, 1, false, modelViewProjectionMatrix,0);

				GLES32.glBindVertexArray(vao_sphere[0]);

				GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
				GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

				GLES32.glBindVertexArray(0);
			}
			Matrix.setIdentityM(modelViewProjectionMatrix, 0);

			modelMatrix = popMatrix();

		}
		modelMatrix = popMatrix();



			
			
		

		

	

		//step 5: Unuse shader program object
		GLES32.glUseProgram(0);


		//Render

		requestRender();

	}

	private void update()
	{
		//code

		if(singleTap == 1)
		{
			shoulder += 1.0f;

			if(shoulder > 360.0f)
			{
				shoulder -=360.0f;
			}
		}
		else if(singleTap == 2)
		{
			shoulder -= 1.0f;

			if(shoulder <= 0.0f)
			{
				shoulder = 360.0f;
			}
		}

		//for forearm

		if(singleTap == 3)
		{
			elbow += 1.0f;

			if(elbow > 360.0f)
			{
				elbow -=360.0f;
			}
		}
		else if(singleTap == 4)
		{
			elbow -= 1.0f;

			if(elbow <= 0.0f)
			{
				elbow = 360.0f;
			}
		}
		
		if(singleTap == 5)
		{
			wrist += 1.0f;

			if(wrist > 360.0f)
			{
				wrist -=360.0f;
			}
		}
		else if(singleTap == 6)
		{
			wrist -= 1.0f;

			if(wrist <= 0.0f)
			{
				wrist = 360.0f;
			}
		}
		
		/*else
		{
			singleTap = 0;
		}*/

		

		/*else if(singleTap == 7)
		{
			moon_day += 1.0f;

			if(moon_day > 360.0f)
			{
				moon_day -=360.0f;
			}
		}

		else if(singleTap == 8)
		{
			moon_day -= 1.0f;

			if(moon_day <= 0.0f)
			{
				moon_day = 360.0f;
			}
		}*/

	


		
		
    }

			
		

	

	private void initializeMatrixStack()
	{
		matrixStackTop = 0;

		for (int i = 0; i < MODEL_VIEW_MATRIX_STACK; i++)
		{
			Matrix.setIdentityM(matrixStack[i], 0);
		}

	}	

	private void pushMatrix(float[] matrix)
	{
		if(matrixStackTop >= MODEL_VIEW_MATRIX_STACK)
		{
			System.out.println("ERROR : Exceeded matrix stack limit\n");
			uninitialize();
		}

		matrixStack[matrixStackTop] = matrix.clone();

		System.out.println("PLP: Current Matrix Length : " + matrixStack[matrixStackTop]);

		matrixStackTop++;
	}

	private float[] popMatrix()
	{
		if (matrixStackTop < 0)
		{
			System.out.println("ERROR : Matrix Stack is empty\n");

			uninitialize();
		}

		Matrix.setIdentityM(matrixStack[matrixStackTop],0);

		matrixStackTop--;

		float[] matrix = matrixStack[matrixStackTop].clone();

		return matrix;
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

		if(vbo_sphere_element[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo_sphere_element, 0);
			vbo_sphere_element[0] = 0;
		}

		if(vbo_sphere_normal[0] > 0)
		{
			GLES32.glDeleteBuffers(1, vbo_sphere_normal, 0);
			vbo_sphere_normal[0] = 0;
		}



		if(vao_sphere[0] > 0)
		{
			GLES32.glDeleteVertexArrays(1, vao_sphere, 0);
			vao_sphere[0] = 0;
		}
	}

};

