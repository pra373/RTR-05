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
    AMC_ATTRIBUTE_TEXCOORD: 1,
    AMC_ATTRIBUTE_NORMAL: 2,
    AMC_ATTRIBUTE_COLOR: 3
};

var shaderProgramObject = null;

var vao = null;
var vbo_PCNT = null;

var mvpMatrixUniform;

var perspectiveProjectionMatrix;

// for rotation
var angle_Cube = 0.0;

// texture variable
var marble_Texture;

var textureSamplerUniform;


// Uniforms

var modelMatrixUniform = 0;
var viewMatrixUniform = 0;
var projectionMatrixUniform = 0;

var lightDiffuseUniform = 0;
var lightAmbientUniform = 0;
var lightSpecularUniform = 0;
var lightPositionUniform = 0;

var materialDiffuseUniform = 0;
var materialAmbientUniform = 0;
var materialSpecularUniform = 0;
var materialShininessUniform = 0;


var keyPressUniform = 0;

var bLightingEnable = false;

var bAnimationEnable = false;


// Light Variables

var lightAmbient = new Float32Array([0.1, 0.1, 0.1]);
var lightDiffuse = new Float32Array([1.0, 1.0, 1.0]); 
var lightSpecular = new Float32Array([1.0, 1.0, 1.0]);
var lightPosition = new Float32Array([100.0, 100.0, 100.0, 1.0]);

var materialAmbient = new Float32Array([0.0, 0.0, 0.0]);
var materialDiffuse = new Float32Array([1.0, 1.0, 1.0]);
var materialSpecular = new Float32Array([1.0, 1.0, 1.0]);
var materialShininess = 50.0;



var requestAnimationFrame =
    window.requestAnimationFrame ||    
    window.webkitRequestAnimationFrame ||  
    window.mozRequestAnimationFrame ||  
    window.oRequestAnimationFrame ||   
    window.msRequestAnimationFrame;   


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
        case 102:  // f
            toggleFullscreen();
            break;

       /* case 76: // L
        case 108: // l
            if (bLightingEnable == false)
            {
                bLightingEnable = true;
            }
            else {
                bLightingEnable = false;
            }
            break;*/

        

    }


    switch (event.key)
    {

        case 'A':
        case 'a':
            if (bAnimationEnable == false) {
                bAnimationEnable = true;
            }
            else {
                bAnimationEnable = false;
            }
            break;

        case "L":
        case "l":
            if (bLightingEnable == false)
            {
                bLightingEnable = true;
            }
            else
            {
                bLightingEnable = false;
            }
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
            "in vec4 aPosition;\n" +
            "in vec3 aNormal;\n" +
            "in vec4 aColor;\n" +
            "in vec2 aTexCoord;\n" +// Added input for texture coordinates
            "out vec4 oColor;\n" +
            "uniform mat4 uModelMatrix;\n" +
            "uniform mat4 uViewMatrix;\n" +
            "uniform mat4 uProjectionMatrix;\n" +
            "uniform vec3 ulightAmbient;\n" +
            "uniform vec3 ulightDiffuse;\n" +
            "uniform vec3 ulightSpecular;\n" +
            "uniform vec4 ulightPosition;\n" +
            "uniform vec3 uMaterialAmbient;\n" +
            "uniform vec3 uMaterialDiffuse;\n" +
            "uniform vec3 uMaterialSpecular;\n" +
            "uniform float uMaterialShininess;\n" +
            "uniform highp int uKeyPress;\n" +
            "out vec3 otransformedNormals;\n" +
            "out vec3 olightDirection;\n" +
            "out vec3 oviewerVector;\n" +
            "out vec2 oTexCoord;\n" + // Output for texture coordinates
            "void main(void)\n" +
            "{\n" +
            "if(uKeyPress == 1)\n" +
            "{\n" +
            "vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;\n" +
            "otransformedNormals = (mat3(uViewMatrix * uModelMatrix) * aNormal);\n" +
            "olightDirection = (vec3 (ulightPosition - eyeCoordinates));\n" +
            "oviewerVector = (- eyeCoordinates.xyz);\n" +
            "}\n" +
            "else\n" +
            "{\n" +
            "otransformedNormals = vec3(0.0, 0.0, 0.0);\n" +
            "olightDirection = vec3(0.0, 0.0, 0.0);\n" +
            "oviewerVector = vec3(0.0, 0.0, 0.0);\n" +
            "}\n" +
            "    oColor = aColor;\n" +
            "oTexCoord = aTexCoord;\n" +// Pass texture coordinates to the fragment shader
            "gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;\n" +
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
            "in vec4 oColor;\n" +
            "in vec3 otransformedNormals;\n" +
            "in vec3 olightDirection;\n" +
            "in vec3 oviewerVector;\n" +
            "in vec2 oTexCoord;\n" + // Receive texture coordinates from vertex shader
            "uniform vec3 ulightAmbient;\n" +
            "uniform vec3 ulightDiffuse;\n" +
            "uniform vec3 ulightSpecular;\n" +
            "uniform vec3 uMaterialAmbient;\n" +
            "uniform vec3 uMaterialDiffuse;\n" +
            "uniform vec3 uMaterialSpecular;\n" +
            "uniform float uMaterialShininess;\n" +
            "uniform highp int uKeyPress;\n" +
            "uniform highp sampler2D uTextureSampler;\n" +  // Texture sampler uniform
            "out vec4 FragColor;\n" +
            "void main(void)\n" +
            "{\n" +
            "vec3 PhongADSLight;\n" +
            "if(uKeyPress == 1)\n" +
            "{\n" +
            "vec3 normalizedTransformedNormals = normalize(otransformedNormals);\n" +
            "vec3 normalizedLightDirection = normalize(olightDirection);\n" +
            "vec3 normalizedViwerVector = normalize(oviewerVector);\n" +
            "vec3 ambientLight = ulightAmbient * uMaterialAmbient;\n" +
            "vec3 diffuseLight = ulightDiffuse * uMaterialDiffuse * max(dot(normalizedLightDirection, normalizedTransformedNormals), 0.0);\n" +
            "vec3 reflectionVector = reflect(-normalizedLightDirection, normalizedTransformedNormals);\n" +
            "vec3 specularLight = ulightSpecular * uMaterialSpecular * pow(max(dot(reflectionVector, normalizedViwerVector), 0.0), uMaterialShininess);\n" +
            "PhongADSLight = ambientLight + diffuseLight + specularLight;\n" +
            "}\n" +
            "else\n" +
            "{\n" +
            "PhongADSLight = vec3(1.0, 1.0, 1.0);\n" +
            "}\n" +
            "vec3 tex = texture(uTextureSampler, oTexCoord).rgb;\n" +
            "FragColor = vec4(tex * vec3(oColor) * PhongADSLight, 1.0);\n" +
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
    gl.bindAttribLocation(shaderProgramObject, vertexAttributeEnum.AMC_ATTRIBUTE_POSITION, "aPosition");

    gl.bindAttribLocation(shaderProgramObject, vertexAttributeEnum.AMC_ATTRIBUTE_TEXCOORD, "aTexCoord");

    gl.bindAttribLocation(shaderProgramObject, vertexAttributeEnum.AMC_ATTRIBUTE_NORMAL, "aNormal");

    gl.bindAttribLocation(shaderProgramObject, vertexAttributeEnum.AMC_ATTRIBUTE_COLOR, "aColor");



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


     // Get shader uniform location **********************


   // mvpMatrixUniform = gl.getUniformLocation(shaderProgramObject, "uMVPMatrix");


    modelMatrixUniform = gl.getUniformLocation(shaderProgramObject, "uModelMatrix");



    viewMatrixUniform = gl.getUniformLocation(shaderProgramObject, "uViewMatrix");

    projectionMatrixUniform = gl.getUniformLocation(shaderProgramObject, "uProjectionMatrix");


    lightAmbientUniform = gl.getUniformLocation(shaderProgramObject, "ulightAmbient");


    lightDiffuseUniform = gl.getUniformLocation(shaderProgramObject, "ulightDiffuse");

    lightSpecularUniform = gl.getUniformLocation(shaderProgramObject, "ulightSpecular");


    lightPositionUniform = gl.getUniformLocation(shaderProgramObject, "ulightPosition");

    materialAmbientUniform = gl.getUniformLocation(shaderProgramObject, "uMaterialAmbient");

    materialDiffuseUniform = gl.getUniformLocation(shaderProgramObject, "uMaterialDiffuse");


    materialSpecularUniform = gl.getUniformLocation(shaderProgramObject, "uMaterialSpecular");

    materialShininessUniform = gl.getUniformLocation(shaderProgramObject, "uMaterialShininess");


    keyPressUniform = gl.getUniformLocation(shaderProgramObject, "uKeyPress");

    // For Texture

    textureSamplerUniform = gl.getUniformLocation(shaderProgramObject, "uTextureSampler");



   //Geometry Attribute declarations

    var cube_PCNT = new Float32Array([
        // front
        // position          // color           // normals          // texcoords
        1.0, 1.0,  1.0,      1.0, 0.0, 0.0,     0.0, 0.0, 1.0,      1.0, 1.0,
        -1.0, 1.0, 1.0,     1.0, 0.0, 0.0,      0.0, 0.0, 1.0,      0.0, 1.0,
        -1.0, -1.0, 1.0,     1.0, 0.0, 0.0,     0.0, 0.0, 1.0,      0.0, 0.0,
        1.0, -1.0, 1.0,     1.0, 0.0, 0.0,      0.0, 0.0, 1.0,      1.0, 0.0,

        // right
        // position          // color           // normals          // texcoords
        1.0, 1.0, -1.0,      0.0, 0.0, 1.0,     1.0, 0.0, 0.0,      1.0, 1.0,
        1.0, 1.0, 1.0,      0.0, 0.0, 1.0,      1.0, 0.0, 0.0,      0.0, 1.0,
        1.0, -1.0, 1.0,     0.0, 0.0, 1.0,      1.0, 0.0, 0.0,      0.0, 0.0,
        1.0, -1.0, -1.0,    0.0, 0.0, 1.0,      1.0, 0.0, 0.0,      1.0, 0.0,

        // back
        // position          // color           // normals          // texcoords
        1.0, 1.0, -1.0,      1.0, 1.0, 0.0,     0.0, 0.0, -1.0,     1.0, 1.0,
        -1.0, 1.0, -1.0,    1.0, 1.0, 0.0,      0.0, 0.0, -1.0,     0.0, 1.0,
        -1.0, -1.0, -1.0,   1.0, 1.0, 0.0,      0.0, 0.0, -1.0,     0.0, 0.0,
        1.0, -1.0, -1.0,    1.0, 1.0, 0.0,      0.0, 0.0, -1.0,     1.0, 0.0,

        // left
        // position          // color           // normals          // texcoords
        -1.0, 1.0, 1.0,     1.0, 0.0, 1.0,      -1.0, 0.0, 0.0,     1.0, 1.0,
        -1.0, 1.0, -1.0,    1.0, 0.0, 1.0,      -1.0, 0.0, 0.0,     0.0, 1.0,
        -1.0, -1.0, -1.0,   1.0, 0.0, 1.0,      -1.0, 0.0, 0.0,     0.0, 0.0,
        -1.0, -1.0, 1.0,    1.0, 0.0, 1.0,      -1.0, 0.0, 0.0,     1.0, 0.0,

        // top
        // position          // color           // normals          // texcoords
        1.0, 1.0, -1.0,     0.0, 1.0, 0.0,       0.0, 1.0, 0.0,     1.0, 1.0,
        -1.0, 1.0, -1.0,    0.0, 1.0, 0.0,      0.0, 1.0, 0.0,      0.0, 1.0,
        -1.0, 1.0, 1.0,     0.0, 1.0, 0.0,      0.0, 1.0, 0.0,      0.0, 0.0,
        1.0, 1.0, 1.0,      0.0, 1.0, 0.0,      0.0, 1.0, 0.0,      1.0, 0.0,

        // bottom
        // position          // color           // normals          // texcoords
        1.0, -1.0, 1.0,      1.0, 0.5, 0.0,     0.0, -1.0, 0.0,     1.0, 1.0,
        -1.0, -1.0, 1.0,      1.0, 0.5, 0.0,    0.0, -1.0, 0.0,     0.0, 1.0,
        -1.0, -1.0, -1.0,    1.0, 0.5, 0.0,     0.0, -1.0, 0.0,     0.0, 0.0,
        1.0, -1.0, -1.0,    1.0, 0.5, 0.0,      0.0, -1.0, 0.0,     1.0, 0.0

    ]);

    ////step 13: Create vertex array object
    //VAO

    // ********************************************** for cube ***********************************************

    // VAO

    vao = gl.createVertexArray();

    //  Bind with VAO
    gl.bindVertexArray(vao);


    //step 15: Create vertex buffer object (VBO) for position

    //VBO for position

  //  glGenBuffers(1, & vbo_PCNT);

    vbo_PCNT = gl.createBuffer();

    //step 16: Bind with VBO of position

    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_PCNT);

    gl.bufferData(gl.ARRAY_BUFFER, cube_PCNT, gl.STATIC_DRAW); // 24*11*sizeof(float) is nothing but size of cube_PCNT

    //*//****************** Position *****************************

    gl.vertexAttribPointer(vertexAttributeEnum.AMC_ATTRIBUTE_POSITION, 3, gl.FLOAT, false, 11 * 4, (0 * 4));

    gl.enableVertexAttribArray(vertexAttributeEnum.AMC_ATTRIBUTE_POSITION);


    //*//******************** Color ********************************

    gl.vertexAttribPointer(vertexAttributeEnum.AMC_ATTRIBUTE_COLOR, 3, gl.FLOAT, false, 11 * 4, (3 * 4));

    gl.enableVertexAttribArray(vertexAttributeEnum.AMC_ATTRIBUTE_COLOR);

    //******************* Normals ************************

    gl.vertexAttribPointer(vertexAttributeEnum.AMC_ATTRIBUTE_NORMAL, 3, gl.FLOAT, false, 11 * 4, (6 * 4));

    gl.enableVertexAttribArray(vertexAttributeEnum.AMC_ATTRIBUTE_NORMAL);

    //***************** Texcoord *****************************
    gl.vertexAttribPointer(vertexAttributeEnum.AMC_ATTRIBUTE_TEXCOORD, 2, gl.FLOAT, false, 11 * 4, (9 * 4));

    gl.enableVertexAttribArray(vertexAttributeEnum.AMC_ATTRIBUTE_TEXCOORD);

    // unbind of vbo

    gl.bindBuffer(gl.ARRAY_BUFFER, null);


    //step 18 : unbind with VAO
    gl.bindVertexArray(null);



 


    // Depth initialization

    gl.clearDepth(1.0);

    gl.enable(gl.DEPTH_TEST);

    gl.depthFunc(gl.LEQUAL);


    // Set Clear Color
    
    gl.clearColor(0.0, 0.0, 0.0, 1.0); 

    // call loadGLTexture
    loadGLTexture();

    // initialize projection matrix

    perspectiveProjectionMatrix = mat4.create();
   
}

function loadGLTexture()
{

    marble_Texture = gl.createTexture();

    marble_Texture.image = new Image();

    marble_Texture.image.src = "marble.png";

    marble_Texture.image.onload = function () {
        gl.bindTexture(gl.TEXTURE_2D, marble_Texture);

        gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);

        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);

        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);

        gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, marble_Texture.image);

        gl.generateMipmap(gl.TEXTURE_2D);

        gl.bindTexture(gl.TEXTURE_2D, null);

    }

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

    var rotationMatrixY = mat4.create(); // for Y rotation
    var rotationMatrixX = mat4.create(); // for X rotation
    var rotationMatrixZ = mat4.create(); // for Z rotation

    var finalRotationMatrix = mat4.create();

    mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -5.5]);

    mat4.rotate(rotationMatrixY, rotationMatrixY, angle_Cube, [0, 1, 0]);

    mat4.rotate(rotationMatrixX, rotationMatrixX, angle_Cube, [1, 0, 0]);

    mat4.rotate(rotationMatrixZ, rotationMatrixZ, angle_Cube, [0, 0, 1]);

    //finalRotationMatrix = rotationMatrixX * rotationMatrixY * rotationMatrixZ;

    mat4.multiply(finalRotationMatrix, rotationMatrixX, rotationMatrixY);

    mat4.multiply(finalRotationMatrix, finalRotationMatrix, rotationMatrixZ);

    mat4.multiply(modelViewMatrix, translationMatrix, finalRotationMatrix);

    //mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);

    //push above mvp into vertex shader's mvp uniform

    gl.uniformMatrix4fv(modelMatrixUniform, false, modelViewMatrix);

    gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);

    gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);


    if (bLightingEnable == true)
    {
        gl.uniform1i(keyPressUniform, 1);

        gl.uniform3fv(lightAmbientUniform, lightAmbient);

        gl.uniform3fv(lightDiffuseUniform, lightDiffuse);

        gl.uniform3fv(lightSpecularUniform, lightSpecular);

        gl.uniform4fv(lightPositionUniform, lightPosition);

        gl.uniform3fv(materialAmbientUniform, materialAmbient);

        gl.uniform3fv(materialDiffuseUniform, materialDiffuse);

        gl.uniform3fv(materialSpecularUniform, materialSpecular);

        gl.uniform1f(materialShininessUniform, materialShininess);

    }
    else {
        gl.uniform1i(keyPressUniform, 0);
    }


    // for texture
    gl.activeTexture(gl.TEXTURE0);

    gl.bindTexture(gl.TEXTURE_2D, marble_Texture);

    gl.uniform1i(textureSamplerUniform, 0);

    gl.bindVertexArray(vao);


    //step 3: Draw the geometry

    gl.drawArrays(gl.TRIANGLE_FAN, 0, 4);

    gl.drawArrays(gl.TRIANGLE_FAN, 4, 4);

    gl.drawArrays(gl.TRIANGLE_FAN, 8, 4);

    gl.drawArrays(gl.TRIANGLE_FAN, 12, 4);

    gl.drawArrays(gl.TRIANGLE_FAN, 16, 4);

    gl.drawArrays(gl.TRIANGLE_FAN, 20, 4);

    //step 4: unBind with (VAO)

    gl.bindVertexArray(null);

    // unbind texture
    gl.bindTexture(gl.TEXTURE_2D, null);


    //step 5: Unuse shader program object
    gl.useProgram(null);


      // call update();

    if (bAnimationEnable == true)
    {
        update();
    }
  

    // Do the Double buffering
    requestAnimationFrame(display, canvas);
}

function update()
{
    // code
    if (angle_Cube < 360.0)
    {
        angle_Cube = angle_Cube + 0.005;
    }
    else
    {
        angle_Cube = 0.0;
    }
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

   

    if (vbo_PCNT)
    {
        gl.deleteBuffer(vbo_PCNT);
        vbo_PCNT = null;
    }

    if (vao)
    {
        gl.deleteVetexArray(vao);
        vao = null;
    }
}

