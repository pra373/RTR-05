// JavaScript source code

var canvas = null;

var gl = null;

var bFullScreen = false;

var canvas_orginal_width;

var canvas_orignal_height;

// WebGL related variables
const vertexAttributeEnum =
{
    AMC_ATTRIBUTE_POSITION: 0,
    AMC_ATTRIBUTE_NORMAL: 1,
    AMC_ATTRIBUTE_TEXTURE: 2
};

var shaderProgramObject = null;

var mvpMatrixUniform;

var perspectiveProjectionMatrix;

// Lights related

var modelViewMatrixUniform = 0;
var projectionMatrixUniform = 0;
var ldUniform = 0;
var kdUniform = 0;
var lightPositionUniform = 0;
var keyPressUniform = 0;


var bLightingEnable = false;

var bAnimationEnable = false;

// Light Variables

var lightDiffuse = new Float32Array([0.0, 1.0, 0.0]);       // white diffuse light
var lightPosition = new Float32Array([0.0, 0.0, 2.0, 1.0]);
var materialDiffuse = new Float32Array([1.0, 1.0, 1.0]);

var requestAnimationFrame =
    window.requestAnimationFrame ||    
    window.webkitRequestAnimationFrame ||  // for sufari
    window.mozRequestAnimationFrame ||  // for mozrella firefox
    window.oRequestAnimationFrame ||   // for opera
    window.msRequestAnimationFrame;   // for microsoft edge


var sphere = null;


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
        case 81: 
        case 113: 
            uninitialize();

            window.close();
            break;

        case 70 :  
        case 102:
            toggleFullscreen();
            break;

        case 76: 
        case 108: 
            if (bLightingEnable == false)
            {
                bLightingEnable = true;
            }
            else {
                bLightingEnable = false;
            }
            break;

        case 65: 
        case 97: 
            if (bAnimationEnable == false)
            {
                bAnimationEnable = true;
            }
            else {
                bAnimationEnable = false;
            }
            break;


    }
   

}

function mouseDown()
{
    
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
            "in vec3 aNormal;\n" +
            "uniform mat4 uMVPMatrix;" +
            "uniform mat4 uModelViewMatrix;\n" +
            "uniform mat4 uProjectionMatrix;\n" +
            "uniform vec3 uld;\n" +
            "uniform vec3 ukd;\n" +
            "uniform vec4 uLightPosition;\n" +
            "uniform highp int uKeyPress;\n" +
            "out vec3 oDiffuseLight;\n" +
            "in vec4 aColor;\n" +
            "out vec4 oColor;\n" +
            "void main(void)\n" +
            "{\n" +
            "if(uKeyPress == 1)" +
            "{\n" +
            "vec4 eyePosition = uModelViewMatrix * aPosition;\n" +
            "mat3 normalMatrix = mat3(transpose(inverse(uModelViewMatrix)));\n" +
            "vec3 n = normalize(normalMatrix * aNormal);\n" +
            "vec3 s = normalize(vec3(uLightPosition - eyePosition));\n" +
            "oDiffuseLight =  uld*ukd*dot(s,n);\n" +
            "}\n" +
            "else\n" +
            "{\n" +
            "oDiffuseLight = vec3( 0.0, 0.0, 0.0);\n" +
            "}\n" +
            "gl_Position = uProjectionMatrix * uModelViewMatrix * aPosition;\n" +
            "}"
        );

    var vertexShaderObject = gl.createShader(gl.VERTEX_SHADER);

    gl.shaderSource(vertexShaderObject, vertexShaderSourceCode);

   

    gl.compileShader(vertexShaderObject);

    if (gl.getShaderParameter(vertexShaderObject, gl.COMPILE_STATUS) == false)
    {
        var error = gl.getShaderInfoLog(vertexShaderObject);

        if (error.length > 0) {
            var log = "vertex Shader Compilation Error : " + error;

            alert(log); // we can  also use console.log()

            uninitialize();
        }
    }
    else
    {
        console.log("vertex Shader Complied Successfully\n");
    }


    // Fragment Shader

    var fragmentShaderSourceCode =
        (
            "#version 300 es" +
            "\n" +
            "precision highp float;" +
            "in vec4 oColor;\n" +
            "in vec3 oDiffuseLight;\n" +
            "uniform  highp int uKeyPress;\n" +
            "out vec4 FragColor;\n" +
            "void main(void)\n" +
            "{\n" +
            "if(uKeyPress == 1)\n" +
            "{\n" +
            "  FragColor = vec4 (oDiffuseLight, 1.0);\n" +
            "}\n" +
            "else\n" +
            "{\n" +
            "  FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n" +
            "}\n" +
            "}"
        );

    var fragmentShaderObject = gl.createShader(gl.FRAGMENT_SHADER);

    gl.shaderSource(fragmentShaderObject, fragmentShaderSourceCode);

    gl.compileShader(fragmentShaderObject);

    if (gl.getShaderParameter(fragmentShaderObject, gl.COMPILE_STATUS) == false)
    {
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
    gl.bindAttribLocation(shaderProgramObject, vertexAttributeEnum.AMC_ATTRIBUTE_POSITION, "aPosition");

    gl.bindAttribLocation(shaderProgramObject, vertexAttributeEnum.AMC_ATTRIBUTE_NORMAL, "aNormal");

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

    //mvpMatrixUniform = gl.getUniformLocation(shaderProgramObject, "uMVPMatrix");

    modelViewMatrixUniform = gl.getUniformLocation(shaderProgramObject, "uModelViewMatrix");


    projectionMatrixUniform = gl.getUniformLocation(shaderProgramObject, "uProjectionMatrix");


    ldUniform = gl.getUniformLocation(shaderProgramObject, "uld");

    kdUniform = gl.getUniformLocation(shaderProgramObject, "ukd");

    lightPositionUniform = gl.getUniformLocation(shaderProgramObject, "uLightPosition");

    keyPressUniform = gl.getUniformLocation(shaderProgramObject, "uKeyPress");

   // Triangle Position Array Declaration

   //Geometry Attribute declarations

    

   
    sphere = new Mesh();

    makeSphere(sphere, 1.0, 30, 30);
    

    

    
    // Depth initialization

    gl.clearDepth(1.0);

    gl.enable(gl.DEPTH_TEST);

    gl.depthFunc(gl.LEQUAL);




    // Set Clear Color
    
    gl.clearColor(0.0, 0.0, 0.0, 1.0); 

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

    var modelViewProjectionMatrix = mat4.create();

    mat4.translate(modelViewMatrix, modelViewMatrix, [0.0, 0.0, -3.0]); // similar behavar for

    mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);

    //push above mvp into vertex shader's mvp uniform

    //gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);


    gl.uniformMatrix4fv(modelViewMatrixUniform, false, modelViewMatrix);

    gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);

    if (bLightingEnable == true)
    {
        gl.uniform1i(keyPressUniform, 1);

        gl.uniform3fv(ldUniform, lightDiffuse);

        gl.uniform3fv(kdUniform, materialDiffuse);

        gl.uniform4fv(lightPositionUniform, lightPosition);

    } else
    {
        gl.uniform1i(keyPressUniform, 0);
    }






    //step 3: Draw the geometry

    sphere.draw();

    //step 4: unBind with (VAO)

    

    //step 5: Unuse shader program object
    gl.useProgram(null);

    // call update();
    
    
     // Update
     update();
    

    // Do the Double buffering
    requestAnimationFrame(display, canvas);
}

function update()
{
   
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

   
}

