// JavaScript source code

//main function

var canvas = null;

var gl = null;

var bFullScreen = false;

var canvas_original_width;

var canvas_original_height;

//WebGL related variables

const vertexAttributeEnum =
{
    AMC_ATTRIBUTE_POSITION: 0
};

const vertexAttributeEnum2 =
{
    AMC_ATTRIBUTE_POSITION_SHAPES: 2,
    AMC_ATTRIBUTE_COLOR_SHAPES: 3

};

var shaderProgramObject = null;

var shaderProgramObject_Shapes = null;

var vao = null;
var vbo = null;
var mvpMatrixUniform;

var enableGraph = false;
var enableTriangle = false;
var enableSquare = false;
var enableCircle = false;

var perspectiveProjectionMatrix;

// vao and vbo for shapes shader

var vbo_Position_Triangle = 0;
var vbo_Color_Triangle = 0;

var vbo_Position_Square = 0;
var vbo_Color_Square = 0;

var vbo_Position_Circle = 0;
var vbo_Color_Circle = 0;

var vao_Triangle = 0;
var vao_Square = 0;
var vao_Circle = 0;

var mvpMatrixUniform_Shapes = 0;


var requestAnimationFrame =
    window.requestAnimationFrame ||
    window.webkitRequestAnimationFrame ||
    window.mozRequestAnimationFrame ||
    window.oRequestAnimationFrame ||
    window.msRequestAnimationFrame;

//circle positions dynamic arrays
                 


function main()
{
    //get canvas
    canvas = document.getElementById("PLP");

    if (!canvas)
    {
        console.log("Did not get canvas \n");

    }
    else
    {
        console.log("Got canvas successfully \n");
    }

    //set canvas width and height for future use

    canvas_original_width = canvas.width;

    canvas_original_height = canvas.height;

    //register for keyboard events

    window.addEventListener("keydown", keyDown, false);

    //register for muose events

    window.addEventListener("click", mouseDown, false);

    //register for resize events

    window.addEventListener("resize", resize, false);

    initialize();

    resize();

    display();

}

function keyDown(event)
{
    //code

    switch (event.keyCode)
    {
        case 81:
        case 113:
            uninitialize();
            window.close();   //not browser neutral
            break;

        case 70:
        case 102:
            toggleFullscreen();
            
            break;

        case 103:
        case 71:
            if (enableGraph == false)
            {

                enableGraph = true;
                
            }
            else
            {

                enableGraph = false;
                enableCircle = false;
                enableSquare = false;
                enableTriangle = false;
            }
            break;

        case 84:    // t and T
        case 116:
            if (enableTriangle == false)
            {

                enableTriangle = true;
                enableSquare = false;
                enableCircle = false;

            }
            else
            {

                enableTriangle = false;
            }
            break;

        case 83:    // t and T
        case 115:
            if (enableSquare == false)
            {

                enableSquare = true;
                enableTriangle = false;
                enableCircle = false;
            }
            else
            {

                enableSquare = false;
                //enableTriangle = false;
            }
            break;

        case 67:    // c and C
        case 99:
            if (enableCircle == false)
            {

                enableSquare = false;
                enableTriangle = false;
                enableCircle = true;
            }
            else
            {

                enableCircle = false;
                //enableTriangle = false;
            }
            break;

            
    }
}

function mouseDown(event)
{
    //alert("Mouse is clicked");
} 

function toggleFullscreen()
{
    var fullscreen_element =
        document.fullscreenElement ||
        document.webkitFullscreenElement ||
        document.mozFullScreenElement ||
        document.msFullscreenElement ||
        null; 

    //if not full Screen

    if (fullscreen_element == null)
    {
        if (canvas.requestFullscreen)
        {
            canvas.requestFullscreen();
        }

        else if (canvas.webkitRequestFullscreen)
        {
            canvas.webkitRequestFullscreen();
        }

        else if (canvas.mozRequestFullScreen)
        {
            canvas.mozRequestFullScreen();
        }

        else if (canvas.msRequestFullscreen)
        {
            canvas.msRequestFullscreen();
        }

        bFullScreen = true;
    }
    else    //if already full screen
    {
        if (document.exitFullscreen)
        {
            document.exitFullscreen();
        }

        else if (document.webkitExitFullscreen)
        {
            document.webkitExitFullscreen();
        }

        else if (document.mozCancelFullScreen)
        {
            document.mozCancelFullScreen();
        }

        else if (document.msExitFullscreen)
        {
            document.msExitFullscreen();
        }

        bFullScreen = false;
    }


}

function initialize()
{

    var upperX = 0.0;
    var lowerX = 0.0;
    var upperY = 1.0;
    var lowerY = -1.0;
    //code

    //get context from above canvas

    gl = canvas.getContext("webgl2");

    if (!gl)
    {
        console.log("Did not get WebGL2 context Successfully \n");

    }

    else
    {
        console.log("Got WebGL2 context successfully \n");
    }

    //set webgl2 context's view width and view height properties

    gl.viewportWidth = canvas.width;

    gl.viewportHeight = canvas.height;

    //vertex shader

    var vertexShaderSourceCode =
        (
            "#version 300 es" +
            "\n" +
            "in vec4 aPosition;\n"+
		    "out vec4 oColor;\n"+
            "uniform mat4 uMVPMatrix;\n"+          //u for uniform datatype
            "void main(void)\n"+
            "{\n" +
            "vec2 position = aPosition.xy;" +
            "gl_Position=uMVPMatrix * vec4(position,0.0,1.0);\n"+
            "if(gl_VertexID < 2)\n"+
            "{\n"+
            "oColor = vec4(0.0, 1.0, 0.0, 1.0);\n"+
            "}\n"+
            "else if(gl_VertexID >= 2 && gl_VertexID <= 3 )\n"+
            "{\n"+
            "oColor = vec4(1.0, 0.0, 0.0, 1.0);\n"+
            "}\n"+
            "else\n"+
            "{\n"+
            "oColor = vec4(0.0, 0.0, 1.0, 1.0);\n"+
            "}\n"+
            "}"
        );

    var vertexShaderObject = gl.createShader(gl.VERTEX_SHADER)
    gl.shaderSource(vertexShaderObject, vertexShaderSourceCode);

    gl.compileShader(vertexShaderObject);

    if (gl.getShaderParameter(vertexShaderObject, gl.COMPILE_STATUS) == false)
    {
        var error = gl.getShaderInfoLog(vertexShaderObject);

        if (error.length > 0)
        {
            var log = "vertex shader compilation error : " + error;
            alert(log);
            uninitialize();
        }
    }

    else
    {
        console.log("Vertex Shader Compiled Successfully !\n");

    }

    var fragmentShaderSourceCode =
        (
            "#version 300 es" +
            "\n" +
            "precision highp float;" +
            "in vec4 oColor;"+
		    "out vec4 FragColor;\n"+
            "void main(void)\n"+
            "{\n"+
            "FragColor = oColor;\n"+                       //vec4 is working as a constructor here not as a datatype
            "}"
        );

    var fragmentShaderObject = gl.createShader(gl.FRAGMENT_SHADER);

    gl.shaderSource(fragmentShaderObject, fragmentShaderSourceCode);

    gl.compileShader(fragmentShaderObject);

    if (gl.getShaderParameter(fragmentShaderObject, gl.COMPILE_STATUS) == false)
    {
        var error = gl.getShaderInfoLog(fragmentShaderObject);

        if (error.length > 0)
        {
            var log = "fragment shader compilation error : " + error;
            alert(log);
            uninitialize();
        }
    }

    else
    {
        console.log("fragment Shader Compiled Successfully !\n");

    }

    //shader program

    shaderProgramObject = gl.createProgram();

    gl.attachShader(shaderProgramObject, vertexShaderObject);
    gl.attachShader(shaderProgramObject, fragmentShaderObject);

    gl.bindAttribLocation(shaderProgramObject, vertexAttributeEnum.AMC_ATTRIBUTE_POSITION, "aPosition");

    gl.linkProgram(shaderProgramObject);

    if (gl.getProgramParameter(shaderProgramObject, gl.LINK_STATUS) == false) {
        var error = gl.getProgramInfoLog(shaderProgramObject);

        if (error.length > 0) {
            var log = "shader program linking error : " + error;
            alert(log);
            uninitialize();
        }
    }

    else
    {
        console.log("shader program linked Successfully !\n");
    }

    
    mvpMatrixUniform = gl.getUniformLocation(shaderProgramObject, "uMVPMatrix");


    // ************************************ shaders for shape handeling *******************************************

    //vertex shader

    var vertexShaderSourceCode_Shapes =
        (
            "#version 300 es" +
            "\n" +
            "in vec4 inputPosition;\n"+
		    "in vec4 inputColor;\n"+
            "out vec4 FragColor;\n"+          
            "uniform mat4 uMVPMatrixShapes;"+
            "void main(void)\n"+
            "{\n" +
            "vec2 position = inputPosition.xy;" +
            "gl_Position = uMVPMatrixShapes * vec4(position, 0.0, 1.0);\n"+
            "FragColor = inputColor;\n"+
            "}"
        );

    var vertexShaderObject_Shapes = gl.createShader(gl.VERTEX_SHADER)
    gl.shaderSource(vertexShaderObject_Shapes, vertexShaderSourceCode_Shapes);

    gl.compileShader(vertexShaderObject_Shapes);

    if (gl.getShaderParameter(vertexShaderObject_Shapes, gl.COMPILE_STATUS) == false) {
        var error = gl.getShaderInfoLog(vertexShaderObject_Shapes);

        if (error.length > 0) {
            var log = "vertex shader  shapes compilation error : " + error;
            alert(log);
            uninitialize();
        }
    }

    else {
        console.log("Vertex Shader  shapes Compiled Successfully !\n");

    }


    var fragmentShaderSourceCode_Shapes =
        (
            "#version 300 es" +
            "\n" +
            "precision highp float;" +
            "in vec4 FragColor;\n"+
		    "out vec4 outputColor;\n"+
            "void main(void)\n"+
            "{\n"+
            "outputColor = FragColor;\n"+
            "}"
        );

    var fragmentShaderObject_Shapes = gl.createShader(gl.FRAGMENT_SHADER);

    gl.shaderSource(fragmentShaderObject_Shapes, fragmentShaderSourceCode_Shapes);

    gl.compileShader(fragmentShaderObject_Shapes);

    if (gl.getShaderParameter(fragmentShaderObject_Shapes, gl.COMPILE_STATUS) == false) {
        var error = gl.getShaderInfoLog(fragmentShaderObject_Shapes);

        if (error.length > 0) {
            var log = "fragment shader shapes compilation error : " + error;
            alert(log);
            uninitialize();
        }
    }

    else
    {
        console.log("fragment Shader shapes Compiled Successfully !\n");

    }


    //shader program

    shaderProgramObject_Shapes = gl.createProgram();

    gl.attachShader(shaderProgramObject_Shapes, vertexShaderObject_Shapes);
    gl.attachShader(shaderProgramObject_Shapes, fragmentShaderObject_Shapes);

    gl.bindAttribLocation(shaderProgramObject_Shapes, vertexAttributeEnum2.AMC_ATTRIBUTE_POSITION_SHAPES, "inputPosition");
    gl.bindAttribLocation(shaderProgramObject_Shapes, vertexAttributeEnum2.AMC_ATTRIBUTE_COLOR_SHAPES, "inputColor");

    gl.linkProgram(shaderProgramObject_Shapes);

    if (gl.getProgramParameter(shaderProgramObject_Shapes, gl.LINK_STATUS) == false) {
        var error = gl.getProgramInfoLog(shaderProgramObject_Shapes);

        if (error.length > 0) {
            var log = "shader program shapes linking error : " + error;
            alert(log);
            uninitialize();
        }
    }

    else
    {
        console.log("shader program shapes linked Successfully !\n");
    }

    //get Shader uniform Locations

    mvpMatrixUniform_Shapes = gl.getUniformLocation(shaderProgramObject_Shapes, "uMVPMatrixShapes");

    //geometry position array declaration

   /* var linePosition = new Float32Array
    ([
        0.0, 1.0,
        0.0, -1.0,
        -1.0, 0.0,
        1.0, 0.0
    ])*/

    let linePositions = [];

    linePositions.push(0.0, 1.0, 0.0, -1.0, -1.0, 0.0, 1.0, 0.0);

    for (let i = 0; i <= 39; i++)   //loop for +x vertical lines
    {
        upperX = upperX + 0.025;
        lowerX = lowerX + 0.025;

        linePositions.push(upperX);

        linePositions.push(upperY);

        linePositions.push(lowerX);

        linePositions.push(lowerY);


    }

    upperX = 0.0;
    lowerX = 0.0;

    for (var i = 0; i <= 39; i++)   //loop for -X vertical lines
    {
        upperX = upperX - 0.025;
        lowerX = lowerX - 0.025;

        linePositions.push(upperX);

        linePositions.push(upperY);

        linePositions.push(lowerX);

        linePositions.push(lowerY);


    }

    var leftX = -1.0;
	 var rightX = 1.0;
	 
    var leftY = 0.0;
    var rightY = 0.0;

    for (var i = 0; i <= 39; i++)    //loop for +y vertical lines
    {
        leftY = leftY + 0.025;
        rightY = rightY + 0.025;

        linePositions.push(leftX);

        linePositions.push(leftY);

        linePositions.push(rightX);

        linePositions.push(rightY);


    }

    leftX = -1.0;
    rightX = 1.0;

    leftY = 0.0;
    rightY = 0.0;

    for (var i = 0; i <= 39; i++)    //loop for -y vertical lines
    {
        leftY = leftY - 0.025;
        rightY = rightY - 0.025;

        linePositions.push(leftX);

        linePositions.push(leftY);

        linePositions.push(rightX);

        linePositions.push(rightY);


    }


    var linePositionBuffer = new Float32Array(linePositions);


    //triangle positions

        var trianglePosition = new Float32Array
        ([
            0.0, 0.5,  //top vertex
            -0.5, -0.5, //left vertex

            -0.5, -0.5, //left vertex
            0.5, -0.5, //right vertex

            0.5, -0.5, //right vertex
            0.0, 0.5  //top vertex
        ]);

        var triangleColor = new Float32Array
        ([
            1.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            1.0, 1.0, 0.0
        ]);

    var squarePosition = new Float32Array
        ([
            0.5, 0.5, //right top
            -0.5, 0.5, //left top

            -0.5, 0.5, //left top
            -0.5, -0.5, //left bottom

            -0.5, -0.5, //left bottom
            0.5, -0.5,   //right bottom

            0.5, -0.5,   //right bottom
            0.5, 0.5     //right top
        ]);

    var squareColor = new Float32Array
        ([
            1.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            1.0, 1.0, 0.0
        ]);

    let circlePositions = [];
    let circleColor = [];

     var radious = 0.5;

    for (var angle = 0.0; angle <= 360.0; angle = angle + 0.01)
    {

		 //float radianangle = angle * (M_PI * 180.0f);
        var x = radious * Math.cos((angle * Math.PI) / 180.0) + (0.0);
        var y = radious * Math.sin((angle * Math.PI) / 180.0) + (0.0);

        circlePositions.push(x);
        circlePositions.push(y);


    }

    for (var i = 0; i < circlePositions.length; i++)
    {
        circleColor.push(1.0);
        circleColor.push(1.0);
        circleColor.push(0.0);
    }

    var circlePositionBuffer = new Float32Array(circlePositions);
    var circleColorBuffer = new Float32Array(circleColor);

    //vao

    vao = gl.createVertexArray();

    gl.bindVertexArray(vao);

    //vbo

    vbo = gl.createBuffer();

    gl.bindBuffer(gl.ARRAY_BUFFER, vbo);

    gl.bufferData(gl.ARRAY_BUFFER, linePositionBuffer, gl.STATIC_DRAW);

    gl.vertexAttribPointer(vertexAttributeEnum.AMC_ATTRIBUTE_POSITION, 2, gl.FLOAT, false, 0, 0);

    gl.enableVertexAttribArray(vertexAttributeEnum.AMC_ATTRIBUTE_POSITION);

    gl.bindBuffer(gl.ARRAY_BUFFER, null);  //unbind vbo

    //unbind vao

    gl.bindVertexArray(null);

    //vao triangle

    vao_Triangle = gl.createVertexArray();

    gl.bindVertexArray(vao_Triangle);

    //vbo position

    vbo_Position_Triangle = gl.createBuffer();

    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_Position_Triangle);

    gl.bufferData(gl.ARRAY_BUFFER, trianglePosition, gl.STATIC_DRAW);

    gl.vertexAttribPointer(vertexAttributeEnum2.AMC_ATTRIBUTE_POSITION_SHAPES, 2, gl.FLOAT, false, 0, 0);

    gl.enableVertexAttribArray(vertexAttributeEnum2.AMC_ATTRIBUTE_POSITION_SHAPES);

    gl.bindBuffer(gl.ARRAY_BUFFER, null);  //unbind vbo

    

    //vbo color

    vbo_Color_Triangle = gl.createBuffer();

    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_Color_Triangle);

    gl.bufferData(gl.ARRAY_BUFFER, triangleColor, gl.STATIC_DRAW);

    gl.vertexAttribPointer(vertexAttributeEnum2.AMC_ATTRIBUTE_COLOR_SHAPES, 3, gl.FLOAT, false, 0, 0);

    gl.enableVertexAttribArray(vertexAttributeEnum2.AMC_ATTRIBUTE_COLOR_SHAPES);

    gl.bindBuffer(gl.ARRAY_BUFFER, null);  //unbind vbo

    //unbind vao

    gl.bindVertexArray(null);

    //vao for square

    

    vao_Square = gl.createVertexArray();

    gl.bindVertexArray(vao_Square);

    //vbo position

    vbo_Position_Square = gl.createBuffer();

    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_Position_Square);

    gl.bufferData(gl.ARRAY_BUFFER, squarePosition, gl.STATIC_DRAW);

    gl.vertexAttribPointer(vertexAttributeEnum2.AMC_ATTRIBUTE_POSITION_SHAPES, 2, gl.FLOAT, false, 0, 0);

    gl.enableVertexAttribArray(vertexAttributeEnum2.AMC_ATTRIBUTE_POSITION_SHAPES);

    gl.bindBuffer(gl.ARRAY_BUFFER, null);  //unbind vbo



    //vbo color

    vbo_Color_Square = gl.createBuffer();

    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_Color_Square);

    gl.bufferData(gl.ARRAY_BUFFER, squareColor, gl.STATIC_DRAW);

    gl.vertexAttribPointer(vertexAttributeEnum2.AMC_ATTRIBUTE_COLOR_SHAPES, 3, gl.FLOAT, false, 0, 0);

    gl.enableVertexAttribArray(vertexAttributeEnum2.AMC_ATTRIBUTE_COLOR_SHAPES);

    gl.bindBuffer(gl.ARRAY_BUFFER, null);  //unbind vbo

    //unbind vao

    gl.bindVertexArray(null);

    //vao for circle



    vao_Circle = gl.createVertexArray();

    gl.bindVertexArray(vao_Circle);

    //vbo position

    vbo_Position_Circle = gl.createBuffer();

    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_Position_Circle);

    gl.bufferData(gl.ARRAY_BUFFER, circlePositionBuffer, gl.STATIC_DRAW);

    gl.vertexAttribPointer(vertexAttributeEnum2.AMC_ATTRIBUTE_POSITION_SHAPES, 2, gl.FLOAT, false, 0, 0);

    gl.enableVertexAttribArray(vertexAttributeEnum2.AMC_ATTRIBUTE_POSITION_SHAPES);

    gl.bindBuffer(gl.ARRAY_BUFFER, null);  //unbind vbo



    //vbo color

    vbo_Color_Circle = gl.createBuffer();

    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_Color_Circle);

    gl.bufferData(gl.ARRAY_BUFFER, circleColorBuffer, gl.STATIC_DRAW);

    gl.vertexAttribPointer(vertexAttributeEnum2.AMC_ATTRIBUTE_COLOR_SHAPES, 3, gl.FLOAT, false, 0, 0);

    gl.enableVertexAttribArray(vertexAttributeEnum2.AMC_ATTRIBUTE_COLOR_SHAPES);

    gl.bindBuffer(gl.ARRAY_BUFFER, null);  //unbind vbo

    //unbind vao

    gl.bindVertexArray(null);

        

    //depth initialization

    gl.clearDepth(1.0);

    gl.enable(gl.DEPTH_TEST);

    gl.depthFunc(gl.LEQUAL);

    //check for linewidth range

    const lineWidthRange = gl.getParameter(gl.ALIASED_LINE_WIDTH_RANGE);
    console.log('Supported line width range:', lineWidthRange);

    console.log('length of CirclePositionBuffer: ', circlePositionBuffer.length);


    

    // set clear color

    gl.clearColor(0.0, 0.0, 0.0, 1.0);

    //initialize projection matrix

    perspectiveProjectionMatrix = mat4.create();

}

function resize()
{
    //code

    if (bFullScreen == true)
    {
        canvas.width = window.innerWidth;
        canvas.height = window.innerHeight;
    }

    else
    {

        canvas.width = canvas_original_width;
        canvas.height = canvas_original_height;
    }

    // set viewport

    gl.viewport(0, 0, canvas.width, canvas.height);

    //set perspective projection

    mat4.perspective(perspectiveProjectionMatrix, 45.0, parseFloat(canvas.width) / parseFloat(canvas.height), 0.1, 100.0);
    
}


function display()
{
    //code

    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    gl.useProgram(shaderProgramObject);

    //transformation

    var modelViewMatrix = mat4.create();

    var modelViewProjectionMatrix = mat4.create();

    mat4.translate(modelViewMatrix, modelViewMatrix, [0.0, 0.0, -2.0]);

    mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);

    gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);

   

    if (enableGraph == true)
    {

       
        gl.bindVertexArray(vao);
        
        gl.drawArrays(gl.LINES, 0, 324);
        gl.bindVertexArray(null);

        if (enableTriangle == true)
        {
            gl.useProgram(shaderProgramObject_Shapes);

            modelViewMatrix = mat4.create();

            modelViewProjectionMatrix = mat4.create();

            mat4.translate(modelViewMatrix, modelViewMatrix, [0.0, 0.0, -2.0]);

            mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);

            gl.uniformMatrix4fv(mvpMatrixUniform_Shapes, false, modelViewProjectionMatrix);

            gl.bindVertexArray(vao_Triangle);

            gl.drawArrays(gl.LINES, 0, 6);

            gl.bindVertexArray(null);

            gl.useProgram(null);

        }

        if (enableSquare == true)
        {
            gl.useProgram(shaderProgramObject_Shapes);

            modelViewMatrix = mat4.create();

            modelViewProjectionMatrix = mat4.create();

            mat4.translate(modelViewMatrix, modelViewMatrix, [0.0, 0.0, -2.0]);

            mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);

            gl.uniformMatrix4fv(mvpMatrixUniform_Shapes, false, modelViewProjectionMatrix);

            gl.bindVertexArray(vao_Square);

            gl.drawArrays(gl.LINES, 0, 8);

            gl.bindVertexArray(null);

            gl.useProgram(null);

        }

        if (enableCircle == true)
        {
            gl.useProgram(shaderProgramObject_Shapes);

            modelViewMatrix = mat4.create();

            modelViewProjectionMatrix = mat4.create();

            mat4.translate(modelViewMatrix, modelViewMatrix, [0.0, 0.0, -2.0]);

            mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);

            gl.uniformMatrix4fv(mvpMatrixUniform_Shapes, false, modelViewProjectionMatrix);

            gl.bindVertexArray(vao_Circle);

            gl.drawArrays(gl.LINES, 0, (72002 / 2));

            gl.bindVertexArray(null);

            gl.useProgram(null);

        }

    }

   

   

   

    gl.useProgram(null);

    update();

    //double buffering

    requestAnimationFrame(display, canvas);
    
}

function update()
{

    //code
}

function uninitialize()
{

    if (shaderProgramObject)  //this statement can be shaderProgramObject != null
    {
        gl.useProgram(shaderProgramObject);

        var shaderObjects = gl.getAttachedShaders(shaderProgramObject);

        if (shaderObjects && shaderObjects.length > 0)
        {
            for (let i = 0; i < shaderObjects.length; i++)
            {
                gl.detachShader(shaderProgramObject, shaderObjects[i]);
                gl.deleteShader(shaderObjects[i]);
                shaderObjects[i] = null;

            }


        }

        gl.useProgram(null);
        gl.deleteProgram(shaderProgramObject);
        shaderProgramObject = null;


    }

    if (vbo)
    {
        gl.deleteBuffer(vbo);
        vbo = null;
    }

    if (vao)
    {
        gl.deleteVertexArray(vao);
        vao = null;
    }



    

    
}





