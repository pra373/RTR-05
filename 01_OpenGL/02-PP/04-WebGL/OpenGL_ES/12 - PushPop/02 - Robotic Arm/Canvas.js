// JavaScript source code

var canvas = null;

var gl = null;

var bFullScreen = false;

var canvas_orginal_width;

var canvas_orignal_height;


//matrix stack size

var MODEL_VIEW_MATRIX_STACK = 32;

// WebGL related variables
const vertexAttributeEnum =
{
    AMC_ATTRIBUTE_VERTEX: 0,
    AMC_ATTRIBUTE_NORMAL: 1,
    AMC_ATTRIBUTE_TEXTURE : 2
};



var sphere = null;

var shaderProgramObject = null;

var mvpMatrixUniform;

var colorUniform = 0;


var perspectiveProjectionMatrix;


let matrixStack = [];

var matrixStackTop = -1;


var shoulder = 0;
var elbow = 0;
var wrist = 0;


var requestAnimationFrame =
    window.requestAnimationFrame ||    
    window.webkitRequestAnimationFrame ||  // for sufari
    window.mozRequestAnimationFrame ||  // for mozrella firefox
    window.oRequestAnimationFrame ||   // for opera
    window.msRequestAnimationFrame;   // for microsoft edge


// Our main function
function main()
{
    // get canvas
    canvas = document.getElementById("PLP");

    if (!canvas)
    {
        console.log("Getting Canvas Failed!/n");
    }
    else
    {
        console.log("Getting Canvas Succeded!\n");
    }

 

    // Set canvas width and height for future use.

    canvas_orginal_width = canvas.width;

    canvas_orignal_height = canvas.height;
  

    // Step 4 -> Paint the canvas rectangle with above color

  //  context.fillRect(0, 0, canvas.width, canvas.height);


    // Step 9 -> register for keyboard events
    window.addEventListener("keydown", keyDown, false);

    // Step 10 -> register for mouse events
    window.addEventListener("click", mouseDown, false);

    window.addEventListener("resize", resize, false);

    // three method call

    initialize();

    resize();

    display();

}

function keyDown(event)
{
    // code
    switch (event.keyCode)
    {
        case 81: // Q
        case 113: //q
            uninitialize();

            window.close();
            break;

        case 70 :  // F
        case 102:
            toggleFullscreen();
            break;

    }

    switch (event.key)
    {

        case 'S':
            shoulder = (shoulder + 3) % 360;
            break;
        case 's':
            shoulder = (shoulder - 3) % 360;
            break;
        case 'E':
            elbow = (elbow + 3) % 360;
            break;
        case 'e':
            elbow = (elbow - 3) % 360;
            break;
        case 'W':
            wrist = (wrist + 3) % 360;
            break;
        case 'w':
            wrist = (wrist - 3) % 360;
            break;
    }

}

function mouseDown()
{
    ("Mouse is clicked");
}


function toggleFullscreen()
{
    var fullscreen_element =
        document.fullscreenElement ||
        document.webkitfullscreenelement ||
        document.mozfullScreenelement ||
        document.msFullscreenElement ||
        null;

    // If not fullscreen
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
    else // if already fullscreen
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

function initialize() {
    // code

    gl = canvas.getContext("webgl2");

    if (!gl)  //  (gl == null) is same as (!gl)
    {
        console.log("Getting WebGL2 Context Failed!\n");
    }
    else {
        console.log("Getting WebGL2 Context Succeded!\n");
    }


    // Set WebGL2 context's view width and view height properties

    gl.viewportWidth = canvas.width;

    gl.viewportHeight = canvas.height;

    // Vertex Shader

    var vertexShaderSourceCode =
        (
            "#version 300 es" +
            "\n" +
            "in vec4 aPosition;" +
            "uniform mat4 uMVPMatrix;" +
            "void main(void)" +
            "{" +
            "gl_Position=uMVPMatrix*aPosition;" +
            "}"
        );

    var vertexShaderObject = gl.createShader(gl.VERTEX_SHADER);

    gl.shaderSource(vertexShaderObject, vertexShaderSourceCode);

   

    gl.compileShader(vertexShaderObject);

    if (gl.getShaderParameter(vertexShaderObject, gl.COMPILE_STATUS) == false) {
        var error = gl.getShaderInfoLog(vertexShaderObject);

        if (error.length > 0) {
            var log = "vertex Shader Compilation Error : " + error;

            alert(log); // we can  also use console.log()

            uninitialize();
        }
    }
    else {
        console.log("vertex Shader Complied Successfully\n");
    }


    // Fragment Shader

    var fragmentShaderSourceCode =
        (
            "#version 300 es" +
            "\n" +
            "precision highp float;" +
            "out vec4 FragColor;" +
            "void main(void)" +
            "{" +
            "FragColor=vec4(0.8,0.6,0.4,1.0);" +
            "}"
        );

    var fragmentShaderObject = gl.createShader(gl.FRAGMENT_SHADER);

    gl.shaderSource(fragmentShaderObject, fragmentShaderSourceCode);

    gl.compileShader(fragmentShaderObject);

    if (gl.getShaderParameter(fragmentShaderObject, gl.COMPILE_STATUS) == false) {
        var error = gl.getShaderInfoLog(fragmentShaderObject);

        if (error.length > 0) {
            var log = "Fragment Shader Compilation Error : " + error;

            alert(log); // we can  also use console.log()

            uninitialize();
        }
    }
    else {
        console.log("Fragment Shader Complied Successfully\n");
    }


    // Shader Program

    shaderProgramObject = gl.createProgram();

    gl.attachShader(shaderProgramObject, vertexShaderObject);

    gl.attachShader(shaderProgramObject, fragmentShaderObject);

    // binding
    gl.bindAttribLocation(shaderProgramObject, vertexAttributeEnum.AMC_ATTRIBUTE_VERTEX, "aPosition");

    // Linking
    gl.linkProgram(shaderProgramObject);

    if (gl.getProgramParameter(shaderProgramObject, gl.LINK_STATUS) == false)
    { 

         var error = gl.getProgramInfoLog(shaderProgramObject);

        if (error.length > 0)
        {
            var log = "Shader Program Linking Error : " + error;

             alert(log);

            uninitialize();
         }
    }
    else
    {
         console.log("Shader Program Linked Sucessfully\n");
    }

    mvpMatrixUniform = gl.getUniformLocation(shaderProgramObject, "uMVPMatrix");

   // Triangle Position Array Declaration

   //Geometry Attribute declarations

    sphere = new Mesh();

    makeSphere(sphere, 1.0, 30, 30);

   

    // Depth initialization

    gl.clearDepth(1.0);

    gl.enable(gl.DEPTH_TEST);

    gl.depthFunc(gl.LEQUAL);


    initializeMatrixStack();


    // Set Clear Color
    
    gl.clearColor(0.0, 0.0, 0.0, 1.0); //(our 1st webgl API -->> gl.clearColor)


    // initialize projection matrix

    perspectiveProjectionMatrix = mat4.create();
   
}

function resize()
{
    // code

     

    if (bFullScreen == true)
    {
        canvas.width = window.innerWidth;   // innerWidth is property
        canvas.height = window.innerHeight;
    }
    else
    {
        canvas.width = canvas_orginal_width;
        canvas.height = canvas_orignal_height;
    }

    // Set Viewport

    gl.viewport(0, 0, canvas.width, canvas.height);


    // set Perspective Projection

    mat4.perspective(perspectiveProjectionMatrix, 45.0, parseFloat(canvas.width) / parseFloat(canvas.height), 0.1, 100.0);


}

function display()
{
    // code
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    gl.useProgram(shaderProgramObject);


    //transformations

	var modelViewMatrix = mat4.create();

    var viewMatrix = mat4.create();

    var translationMatrix = mat4.create();

    var rotationMatrix = mat4.create();

    var scaleMatrix = mat4.create();

    var modelMatrix = mat4.create();

    var modelViewProjectionMatrix = mat4.create();


    mat4.translate(translationMatrix, translationMatrix, [-2.0, 0.0, -12.0]);
 

    mat4.copy(modelMatrix, translationMatrix);

    pushMatrix(modelMatrix);
    {

        //initialize rotation matrix

        mat4.rotate(rotationMatrix, rotationMatrix, shoulder * (Math.PI / 180.0), [0.0, 0.0, 1.0]);

        translationMatrix = mat4.create();

        mat4.translate(translationMatrix, translationMatrix, [2.0, 0.0, 0.0]);

       // modelMatrix = modelMatrix * rotationMatrix * translationMatrix;

        mat4.multiply(modelMatrix, modelMatrix, rotationMatrix);

        mat4.multiply(modelMatrix, modelMatrix, translationMatrix);


        pushMatrix(modelMatrix);
        {
            //modelMatrix = mat4.create();

            var scaleMatrix = mat4.create();

            mat4.scale(scaleMatrix, scaleMatrix, [2.0, 0.5, 1.0]);

            mat4.multiply(modelMatrix, modelMatrix, scaleMatrix);

            mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, viewMatrix);

            mat4.multiply(modelViewProjectionMatrix, modelViewProjectionMatrix, modelMatrix);

            gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);

            sphere.draw();


        }
        modelMatrix = mat4.create();
        modelMatrix = popMatrix();

        // do transformation for fore arm

        translationMatrix = mat4.create();

        mat4.translate(translationMatrix, translationMatrix, [2.3, 0.0, 0.0]);

        var rotationMatrix2 = mat4.create();



        mat4.rotate(rotationMatrix2, rotationMatrix2, elbow * (Math.PI / 180.0), [0.0, 0.0, 1.0]);

        //modelMatrix = modelMatrix * translationMatrix * rotationMatrix;

        mat4.multiply(modelMatrix, modelMatrix, translationMatrix);

        mat4.multiply(modelMatrix, modelMatrix, rotationMatrix2);

        translationMatrix = mat4.create();

        mat4.translate(translationMatrix, translationMatrix, [1.5, 0.0, 0.0]);

        // modelMatrix = modelMatrix * translationMatrix;

        mat4.multiply(modelMatrix, modelMatrix, translationMatrix);

        // Draw Arm

        pushMatrix(modelMatrix);
        {
            var scaleMatrix = mat4.create();

            mat4.scale(scaleMatrix, scaleMatrix, [1.8, 0.5, 1.0]);

            //modelMatrix = modelMatrix * scaleMatrix;

            mat4.multiply(modelMatrix, modelMatrix, scaleMatrix);

            // modelViewProjectionMatrix = perspectiveProjectionMatrix * viewMatrix * modelMatrix;

            modelViewProjectionMatrix = mat4.create();

            mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, viewMatrix);

            mat4.multiply(modelViewProjectionMatrix, modelViewProjectionMatrix, modelMatrix);

            gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);

            sphere.draw();

            
        }
        modelMatrix = mat4.create();
        modelMatrix = popMatrix();


        // do transformation for wrist

        translationMatrix = mat4.create();

        mat4.translate(translationMatrix, translationMatrix, [1.9, 0.0, 0.0]);

        var rotationMatrix3 = mat4.create();

        mat4.rotate(rotationMatrix3, rotationMatrix3, wrist * (Math.PI / 180.0), [0.0, 0.0, 1.0]);

        // modelMatrix = modelMatrix * translationMatrix * rotationMatrix;

        mat4.multiply(modelMatrix, modelMatrix, translationMatrix);

        mat4.multiply(modelMatrix, modelMatrix, rotationMatrix3);

       

        translationMatrix = mat4.create();

        mat4.translate(translationMatrix, translationMatrix, [0.5, 0.0, 0.0]);

        // modelMatrix = modelMatrix * translationMatrix;

        mat4.multiply(modelMatrix, modelMatrix, translationMatrix);

        // Draw Wrist

        pushMatrix(modelMatrix);
        {
            var scaleMatrix = mat4.create();

            mat4.scale(scaleMatrix, scaleMatrix, [0.6, 0.5, 1.0]);

            mat4.multiply(modelMatrix, modelMatrix, scaleMatrix);

            // modelViewProjectionMatrix = perspectiveProjectionMatrix * viewMatrix * modelMatrix;

            modelViewProjectionMatrix = mat4.create();

            mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, viewMatrix);

            mat4.multiply(modelViewProjectionMatrix, modelViewProjectionMatrix, modelMatrix);

            gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);

            sphere.draw();

        }
        modelMatrix = mat4.create();
        modelMatrix = popMatrix();

    }
    modelMatrix = mat4.create();
     modelMatrix = popMatrix();




    //step 5: Unuse shader program object
    gl.useProgram(null);

    // call update();

    // Do the Double buffering
    requestAnimationFrame(display, canvas);
}

function update()
{
    // code

}

function initializeMatrixStack() {
    matrixStackTop = 0;

    for (let i = 0; i < MODEL_VIEW_MATRIX_STACK; i++) {
        matrixStack.push(mat4.create());
    }
}

function pushMatrix(matrix)
{
    if (matrixStackTop >= MODEL_VIEW_MATRIX_STACK)
    {
        console.error("ERROR: Exceeded matrix stack limit");
        uninitialize();
    }

    matrixStack[matrixStackTop] = mat4.clone(matrix);

    matrixStackTop++;


}

function popMatrix()
{
    if (matrixStackTop < 0)
    {
        console.error("ERROR: Matrix Stack is empty");

        uninitialize();
    }

    matrixStack[matrixStackTop] = mat4.create();

    matrixStackTop--;

    var matrix = mat4.create();

    matrix = mat4.clone(matrixStack[matrixStackTop]);

    return matrix;
}


function uninitialize()
{
    // code

    if (shaderProgramObject)  // this statement can be if(shaderProgramObject != null)
    {
        gl.useProgram(shaderProgramObject);


        var shaderObjects = gl.getAttachedShaders(shaderProgramObject);

        if (shaderObjects && shaderObjects.length > 0) {
            for (let i = 0; i < shaderObjects.length; i++) {
                gl.detachShader(shaderProgramObject, shaderObjects[i]);

                gl.deleteShader(shaderObjects[i]);

                shaderObjects[i] = null;
            }



        }

        gl.useProgram(null);

        gl.deleteProgram(shaderProgramObject);

        shaderProgramObject = null;
    }


    if (sphere) {
        sphere.deallocate();
        sphere = null;
    }

}


