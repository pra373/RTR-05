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
    AMC_ATTRIBUTE_POSITION: 0,
    AMC_ATTRIBUTE_NORMAL: 1,
    AMC_ATTRIBUTE_TEXTURE: 2
};

var shaderProgramObject = null;
var vao = null;
var vbo = null;

//transformation related uniform

var modelMatrixUniform = 0;
var viewMatrixUniform = 0;
var projectionMatrixUniform = 0;

//light related uniforms

var lightDiffuseUniform = 0;

var lightPositionUniform = 0;
var lightAmbientUniform = 0;
var lightSpecularUniform = 0;


var materialAmbientUniform = 0;
var materialDiffuseUniform = 0;
var materialSpecularUniform = 0;
var materialShininessUniform = 0;

var keyPressUniform = 0;

var bLightingEnabled = false;

//light source properties


var lightAmbient = new Float32Array([0.0, 0.0, 0.0]);

var lightDiffuse = new Float32Array([ 1.0, 1.0, 1.0]);
var lightSpecular = new Float32Array([ 1.0, 1.0, 1.0]);
var lightPosition = new Float32Array([ 0.0, 0.0, 0.0, 1.0]);


var perspectiveProjectionMatrix;


var requestAnimationFrame =
    window.requestAnimationFrame ||
    window.webkitRequestAnimationFrame ||
    window.mozRequestAnimationFrame ||
    window.oRequestAnimationFrame ||
    window.msRequestAnimationFrame;

//sphere related variable

var sphere = null;

//light rotation variables

var angleForXRotation = 0.0;
var angleForYRotation = 0.0;
var angleForZRotation = 0.0;

var WindowWidth;
var WindowHeight;

var keyPressed = 0;

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

        case 70:
        case 102:
            toggleFullscreen();
            
            break;

        case 88:
        case 120:

            keyPressed = 1;
           

            break;

        case 89:
        case 121:

            keyPressed = 2;


            break;

        case 90:
        case 122:

            keyPressed = 3;


            break;

        case 76:
        case 108:
            if (bLightingEnabled == false)
            {
                bLightingEnabled = true;
            }
            else
            {
                bLightingEnabled = false;
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
            "in vec4 aPosition;\n" +
            "in vec3 aNormal;\n" +
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
            "gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;\n" +
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
            "in vec3 otransformedNormals;\n" +
            "in vec3 olightDirection;\n" +
            "in vec3 oviewerVector;\n" +
            "uniform vec3 ulightAmbient;\n" +
            "uniform vec3 ulightDiffuse;\n" +
            "uniform vec3 ulightSpecular;\n" +
            "uniform vec3 uMaterialAmbient;\n" +
            "uniform vec3 uMaterialDiffuse;\n" +
            "uniform vec3 uMaterialSpecular;\n" +
            "uniform float uMaterialShininess;\n" +
            "uniform highp int uKeyPress;\n" +
            "out vec4 FragColor;\n" +
            "void main(void)\n" +
            "{\n" +
            "vec3 PhongADSLight;\n" + // local variable
            "if(uKeyPress == 1)\n" +
            "{\n" +
            "vec3 normalizedTransformedNormals = normalize(otransformedNormals);\n" +
            "vec3 normalizedLightDirection = normalize(olightDirection);\n" +
            "vec3 normalizedViwerVector = normalize(oviewerVector);\n" +
            "vec3 ambientLight = ulightAmbient * uMaterialAmbient;\n" +
            "vec3 diffuseLight = ulightDiffuse * uMaterialDiffuse * max(dot(normalizedLightDirection, normalizedTransformedNormals),0.0);\n" +
            "vec3 reflectionVector = reflect(-normalizedLightDirection, normalizedTransformedNormals);\n" +
            "vec3 specularLight = ulightSpecular * uMaterialSpecular * pow(max(dot(reflectionVector, normalizedViwerVector), 0.0),uMaterialShininess);\n" +
            "PhongADSLight = ambientLight + diffuseLight + specularLight;\n " +
            "}\n" +
            "else\n" +
            "{\n" +
            "PhongADSLight = vec3(1.0, 1.0, 1.0);\n" +
            "}\n" +
            "FragColor =  vec4 (PhongADSLight, 1.0);\n" +
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

    gl.bindAttribLocation(shaderProgramObject, vertexAttributeEnum.AMC_ATTRIBUTE_NORMAL, "aNormal");

    gl.linkProgram(shaderProgramObject);

    if (gl.getProgramParameter(shaderProgramObject, gl.LINK_STATUS) == false)
    {
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

    
    //mvpMatrixUniform = gl.getUniformLocation(shaderProgramObject, "uMVPMatrix");

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


    //geometry position array declaration

    /*var pyramidPosition = new Float32Array
    ([
        0.0, 1.0, 0.0,
        -1.0, -1.0, 0.0,
        1.0,-1.0,0.0
    ])*/

    sphere = new Mesh();
    makeSphere(sphere, 2.0, 30, 30);


    /*//vao

    vao = gl.createVertexArray();

    gl.bindVertexArray(vao);

    //vbo

    vbo = gl.createBuffer();

    gl.bindBuffer(gl.ARRAY_BUFFER, vbo);

    gl.bufferData(gl.ARRAY_BUFFER, pyramidPosition, gl.STATIC_DRAW);

    gl.vertexAttribPointer(vertexAttributeEnum.AMC_ATTRIBUTE_POSITION, 3, gl.FLOAT, false, 0, 0);

    gl.enableVertexAttribArray(vertexAttributeEnum.AMC_ATTRIBUTE_POSITION);

    gl.bindBuffer(gl.ARRAY_BUFFER, null);  //unbind vbo

    //unbind vao

    gl.bindVertexArray(null);*/
        

    //depth initialization

    gl.clearDepth(1.0);

    gl.enable(gl.DEPTH_TEST);

    gl.depthFunc(gl.LEQUAL);
    

    // set clear color

    gl.clearColor(0.8, 0.8, 0.8, 1.0);

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

    WindowHeight = canvas.height;
    WindowWidth = canvas.width;

    // set viewport

    gl.viewport(0, 0, canvas.width, canvas.height);

    //set perspective projection

    mat4.perspective(perspectiveProjectionMatrix, 45.0, parseFloat(canvas.width) / parseFloat(canvas.height), 0.1, 100.0);
    
}

function display()
{

    //local variable declaration

    //variable declaration
	
	var horizontalDistance = WindowWidth / 6;
    var verticalDistance = WindowHeight / 7;

    var materialAmbient = new Float32Array(3);
    var materialDiffuse = new Float32Array(3);
    var materialSpecular = new Float32Array(3);

    var Shininess = 0.0;

    //code

    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    gl.useProgram(shaderProgramObject);

    //set material

    //First Column precious stones

    //(emerald)

    materialAmbient[0] = 0.0215;
    materialAmbient[1] = 0.1745;
    materialAmbient[2] = 0.0215;
    



    materialDiffuse[0] = 0.07568;
    materialDiffuse[1] = 0.61424;
    materialDiffuse[2] = 0.07568;
   



    materialSpecular[0] = 0.633;
    materialSpecular[1] = 0.727811;
    materialSpecular[2] = 0.633;
    

	

    Shininess = 0.6 * 128.0;


    //transformation


    var modelViewMatrix = mat4.create();

    var translationMatrix = mat4.create();

    var modelMatrix = mat4.create();

    var viewMatrix = mat4.create();

    //var modelViewProjectionMatrix = mat4.create();

    mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -8.0]);

    mat4.multiply(modelMatrix, modelMatrix, translationMatrix);


    //pass uniform to shader

    gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);

    gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);

    gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);

    //pass light uniforms

    if (bLightingEnabled == true)
    {
        gl.uniform1i(keyPressUniform, 1);
        gl.uniform3fv(lightAmbientUniform, lightAmbient);
        gl.uniform3fv(lightDiffuseUniform, lightDiffuse);
        gl.uniform3fv(lightSpecularUniform, lightSpecular);
        gl.uniform4fv(lightPositionUniform, lightPosition);

        gl.uniform3fv(materialAmbientUniform, materialAmbient);
        gl.uniform3fv(materialDiffuseUniform, materialDiffuse);
        gl.uniform3fv(materialSpecularUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, Shininess);
    }

    else
    {
        gl.uniform1i(keyPressUniform, 0);
    }

   /* gl.bindVertexArray(vao);

    gl.drawArrays(gl.TRIANGLES, 0, 3);

    gl.bindVertexArray(null);
*/

    gl.viewport(0, (verticalDistance * 5), (WindowWidth / 4), (WindowHeight / 4));

    sphere.draw();

    //first column second sphere

    materialAmbient[0] = 0.135;
    materialAmbient[1] = 0.2225;
    materialAmbient[2] = 0.1575;
    



    materialDiffuse[0] = 0.54;
    materialDiffuse[1] = 0.89;
    materialDiffuse[2] = 0.63;
   



    materialSpecular[0] = 0.316228;
    materialSpecular[1] = 0.316228;
    materialSpecular[2] = 0.316228;
   



    Shininess = 0.1 * 128;

    //transformation


    var modelViewMatrix = mat4.create();

    var translationMatrix = mat4.create();

    var modelMatrix = mat4.create();

    var viewMatrix = mat4.create();

    //var modelViewProjectionMatrix = mat4.create();

    mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -8.0]);

    mat4.multiply(modelMatrix, modelMatrix, translationMatrix);


    //pass uniform to shader

    gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);

    gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);

    gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);

    //pass light uniforms

    if (bLightingEnabled == true) {
        gl.uniform1i(keyPressUniform, 1);
        gl.uniform3fv(lightAmbientUniform, lightAmbient);
        gl.uniform3fv(lightDiffuseUniform, lightDiffuse);
        gl.uniform3fv(lightSpecularUniform, lightSpecular);
        gl.uniform4fv(lightPositionUniform, lightPosition);

        gl.uniform3fv(materialAmbientUniform, materialAmbient);
        gl.uniform3fv(materialDiffuseUniform, materialDiffuse);
        gl.uniform3fv(materialSpecularUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, Shininess);
    }

    else
    {
        gl.uniform1i(keyPressUniform, 0);
    }

    gl.viewport(0, (verticalDistance * 4), (WindowWidth / 4), (WindowHeight / 4));

    sphere.draw();

    //first column third sphere

    materialAmbient[0] = 0.05375;
    materialAmbient[1] = 0.05;
    materialAmbient[2] = 0.06625;
    

    materialDiffuse[0] = 0.18275;
    materialDiffuse[1] = 0.17;
    materialDiffuse[2] = 0.22525;
   

    materialSpecular[0] = 0.332741;
    materialSpecular[1] = 0.328634;
    materialSpecular[2] = 0.346435;
    

    Shininess = 0.3 * 128;

    //transformation


    var modelViewMatrix = mat4.create();

    var translationMatrix = mat4.create();

    var modelMatrix = mat4.create();

    var viewMatrix = mat4.create();

    //var modelViewProjectionMatrix = mat4.create();

    mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -8.0]);

    mat4.multiply(modelMatrix, modelMatrix, translationMatrix);


    //pass uniform to shader

    gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);

    gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);

    gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);

    //pass light uniforms

    if (bLightingEnabled == true) {
        gl.uniform1i(keyPressUniform, 1);
        gl.uniform3fv(lightAmbientUniform, lightAmbient);
        gl.uniform3fv(lightDiffuseUniform, lightDiffuse);
        gl.uniform3fv(lightSpecularUniform, lightSpecular);
        gl.uniform4fv(lightPositionUniform, lightPosition);

        gl.uniform3fv(materialAmbientUniform, materialAmbient);
        gl.uniform3fv(materialDiffuseUniform, materialDiffuse);
        gl.uniform3fv(materialSpecularUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, Shininess);
    }

    else
    {
        gl.uniform1i(keyPressUniform, 0);
    }

    gl.viewport(0, (verticalDistance * 3), (WindowWidth / 4), (WindowHeight / 4));

    sphere.draw();

    //first colomn fourth sphere

    materialAmbient[0] = 0.25;
    materialAmbient[1] = 0.20725;
    materialAmbient[2] = 0.20725;
   
    materialDiffuse[0] = 1.0;
    materialDiffuse[1] = 0.829;
    materialDiffuse[2] = 0.829;
   
    materialSpecular[0] = 0.296648;
    materialSpecular[1] = 0.296648;
    materialSpecular[2] = 0.296648;
   


    Shininess = 0.088 * 128;

    //transformation


    var modelViewMatrix = mat4.create();

    var translationMatrix = mat4.create();

    var modelMatrix = mat4.create();

    var viewMatrix = mat4.create();

    //var modelViewProjectionMatrix = mat4.create();

    mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -8.0]);

    mat4.multiply(modelMatrix, modelMatrix, translationMatrix);


    //pass uniform to shader

    gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);

    gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);

    gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);

    //pass light uniforms

    if (bLightingEnabled == true) {
        gl.uniform1i(keyPressUniform, 1);
        gl.uniform3fv(lightAmbientUniform, lightAmbient);
        gl.uniform3fv(lightDiffuseUniform, lightDiffuse);
        gl.uniform3fv(lightSpecularUniform, lightSpecular);
        gl.uniform4fv(lightPositionUniform, lightPosition);

        gl.uniform3fv(materialAmbientUniform, materialAmbient);
        gl.uniform3fv(materialDiffuseUniform, materialDiffuse);
        gl.uniform3fv(materialSpecularUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, Shininess);
    }

    else {
        gl.uniform1i(keyPressUniform, 0);
    }

    gl.viewport(0, (verticalDistance * 2), (WindowWidth / 4), (WindowHeight / 4));

    sphere.draw();

    //fifth column fifth sphere

    materialAmbient[0] = 0.1745;
    materialAmbient[1] = 0.01175;
    materialAmbient[2] = 0.01175;
   


    materialDiffuse[0] = 0.61424;
    materialDiffuse[1] = 0.04136;
    materialDiffuse[2] = 0.04136;
   


    materialSpecular[0] = 0.727811;
    materialSpecular[1] = 0.626959;
    materialSpecular[2] = 0.626959;
    


    Shininess = 0.6 * 128;

    //transformation


    var modelViewMatrix = mat4.create();

    var translationMatrix = mat4.create();

    var modelMatrix = mat4.create();

    var viewMatrix = mat4.create();

    //var modelViewProjectionMatrix = mat4.create();

    mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -8.0]);

    mat4.multiply(modelMatrix, modelMatrix, translationMatrix);


    //pass uniform to shader

    gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);

    gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);

    gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);

    //pass light uniforms

    if (bLightingEnabled == true) {
        gl.uniform1i(keyPressUniform, 1);
        gl.uniform3fv(lightAmbientUniform, lightAmbient);
        gl.uniform3fv(lightDiffuseUniform, lightDiffuse);
        gl.uniform3fv(lightSpecularUniform, lightSpecular);
        gl.uniform4fv(lightPositionUniform, lightPosition);

        gl.uniform3fv(materialAmbientUniform, materialAmbient);
        gl.uniform3fv(materialDiffuseUniform, materialDiffuse);
        gl.uniform3fv(materialSpecularUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, Shininess);
    }

    else {
        gl.uniform1i(keyPressUniform, 0);
    }

    gl.viewport(0, (verticalDistance), (WindowWidth / 4), (WindowHeight / 4));

    sphere.draw();

    //first column sixth sphere


    materialAmbient[0] = 0.1;
    materialAmbient[1] = 0.18725;
    materialAmbient[2] = 0.1745;
 


    materialDiffuse[0] = 0.396;
    materialDiffuse[1] = 0.74151;
    materialDiffuse[2] = 0.69102;
    


    materialSpecular[0] = 0.297254;
    materialSpecular[1] = 0.30829;
    materialSpecular[2] = 0.306678;
  

    Shininess = 0.1 * 128;

    //transformation


    var modelViewMatrix = mat4.create();

    var translationMatrix = mat4.create();

    var modelMatrix = mat4.create();

    var viewMatrix = mat4.create();

    //var modelViewProjectionMatrix = mat4.create();

    mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -8.0]);

    mat4.multiply(modelMatrix, modelMatrix, translationMatrix);


    //pass uniform to shader

    gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);

    gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);

    gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);

    //pass light uniforms

    if (bLightingEnabled == true) {
        gl.uniform1i(keyPressUniform, 1);
        gl.uniform3fv(lightAmbientUniform, lightAmbient);
        gl.uniform3fv(lightDiffuseUniform, lightDiffuse);
        gl.uniform3fv(lightSpecularUniform, lightSpecular);
        gl.uniform4fv(lightPositionUniform, lightPosition);

        gl.uniform3fv(materialAmbientUniform, materialAmbient);
        gl.uniform3fv(materialDiffuseUniform, materialDiffuse);
        gl.uniform3fv(materialSpecularUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, Shininess);
    }

    else {
        gl.uniform1i(keyPressUniform, 0);
    }

    gl.viewport(0, 0, (WindowWidth / 4), (WindowHeight / 4));

    sphere.draw();

    //second column first sphere

    materialAmbient[0] = 0.329412;
    materialAmbient[1] = 0.223529;
    materialAmbient[2] = 0.027451;
    

    materialDiffuse[0] = 0.780392;
    materialDiffuse[1] = 0.568627;
    materialDiffuse[2] = 0.113725;
   


    materialSpecular[0] = 0.992157;
    materialSpecular[1] = 0.941176;
    materialSpecular[2] = 0.807843;
    

    Shininess = 0.21794872 * 128;

    //transformation


    var modelViewMatrix = mat4.create();

    var translationMatrix = mat4.create();

    var modelMatrix = mat4.create();

    var viewMatrix = mat4.create();

    //var modelViewProjectionMatrix = mat4.create();

    mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -8.0]);

    mat4.multiply(modelMatrix, modelMatrix, translationMatrix);


    //pass uniform to shader

    gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);

    gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);

    gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);

    //pass light uniforms

    if (bLightingEnabled == true)
    {
        gl.uniform1i(keyPressUniform, 1);
        gl.uniform3fv(lightAmbientUniform, lightAmbient);
        gl.uniform3fv(lightDiffuseUniform, lightDiffuse);
        gl.uniform3fv(lightSpecularUniform, lightSpecular);
        gl.uniform4fv(lightPositionUniform, lightPosition);

        gl.uniform3fv(materialAmbientUniform, materialAmbient);
        gl.uniform3fv(materialDiffuseUniform, materialDiffuse);
        gl.uniform3fv(materialSpecularUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, Shininess);
    }

    else
    {
        gl.uniform1i(keyPressUniform, 0);
    }

    gl.viewport((horizontalDistance), (verticalDistance * 5), (WindowWidth / 4), (WindowHeight / 4));

    sphere.draw();

    //second column second sphere

    materialAmbient[0] = 0.2125;
    materialAmbient[1] = 0.1275;
    materialAmbient[2] = 0.054;
    

    materialDiffuse[0] = 0.714;
    materialDiffuse[1] = 0.568627;
    materialDiffuse[2] = 0.113725;
    

    materialSpecular[0] = 0.992157;
    materialSpecular[1] = 0.941176;
    materialSpecular[2] = 0.807843;
    


    Shininess = 0.21794872 * 128;

    //transformation


    var modelViewMatrix = mat4.create();

    var translationMatrix = mat4.create();

    var modelMatrix = mat4.create();

    var viewMatrix = mat4.create();

    //var modelViewProjectionMatrix = mat4.create();

    mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -8.0]);

    mat4.multiply(modelMatrix, modelMatrix, translationMatrix);


    //pass uniform to shader

    gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);

    gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);

    gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);

    //pass light uniforms

    if (bLightingEnabled == true) {
        gl.uniform1i(keyPressUniform, 1);
        gl.uniform3fv(lightAmbientUniform, lightAmbient);
        gl.uniform3fv(lightDiffuseUniform, lightDiffuse);
        gl.uniform3fv(lightSpecularUniform, lightSpecular);
        gl.uniform4fv(lightPositionUniform, lightPosition);

        gl.uniform3fv(materialAmbientUniform, materialAmbient);
        gl.uniform3fv(materialDiffuseUniform, materialDiffuse);
        gl.uniform3fv(materialSpecularUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, Shininess);
    }

    else
    {
        gl.uniform1i(keyPressUniform, 0);
    }

    gl.viewport((horizontalDistance), (verticalDistance * 4), (WindowWidth / 4), (WindowHeight / 4));

    sphere.draw();

    //second column third sphere


    materialAmbient[0] = 0.25;
    materialAmbient[1] = 0.25;
    materialAmbient[2] = 0.25;
    //materialAmbient[3] = 1.0f;

    materialDiffuse[0] = 0.4;
    materialDiffuse[1] = 0.4;
    materialDiffuse[2] = 0.4;
    //materialDiffuse[3] = 1.0f;


    materialSpecular[0] = 0.774597;
    materialSpecular[1] = 0.774597;
    materialSpecular[2] = 0.774597;
    //materialSpecular[3] = 1.0f;


    Shininess = 0.6 * 128; 

    //transformation


    var modelViewMatrix = mat4.create();

    var translationMatrix = mat4.create();

    var modelMatrix = mat4.create();

    var viewMatrix = mat4.create();

    //var modelViewProjectionMatrix = mat4.create();

    mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -8.0]);

    mat4.multiply(modelMatrix, modelMatrix, translationMatrix);


    //pass uniform to shader

    gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);

    gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);

    gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);

    //pass light uniforms

    if (bLightingEnabled == true)
    {
        gl.uniform1i(keyPressUniform, 1);
        gl.uniform3fv(lightAmbientUniform, lightAmbient);
        gl.uniform3fv(lightDiffuseUniform, lightDiffuse);
        gl.uniform3fv(lightSpecularUniform, lightSpecular);
        gl.uniform4fv(lightPositionUniform, lightPosition);

        gl.uniform3fv(materialAmbientUniform, materialAmbient);
        gl.uniform3fv(materialDiffuseUniform, materialDiffuse);
        gl.uniform3fv(materialSpecularUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, Shininess);
    }

    else
    {
        gl.uniform1i(keyPressUniform, 0);
    }

    gl.viewport((horizontalDistance), (verticalDistance * 3), (WindowWidth / 4), (WindowHeight / 4));

    sphere.draw();

    //second column fourth sphere

    materialAmbient[0] = 0.19125;
    materialAmbient[1] = 0.0735;
    materialAmbient[2] = 0.0225;
    //materialAmbient[3] = 1.0f;

    materialDiffuse[0] = 0.7038;
    materialDiffuse[1] = 0.27048;
    materialDiffuse[2] = 0.0828;
   // materialDiffuse[3] = 1.0f;


    materialSpecular[0] = 0.256777;
    materialSpecular[1] = 0.137622;
    materialSpecular[2] = 0.086014;
    //materialSpecular[3] = 1.0f;


    Shininess = 0.1 * 128;

    //transformation


    var modelViewMatrix = mat4.create();

    var translationMatrix = mat4.create();

    var modelMatrix = mat4.create();

    var viewMatrix = mat4.create();

    //var modelViewProjectionMatrix = mat4.create();

    mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -8.0]);

    mat4.multiply(modelMatrix, modelMatrix, translationMatrix);


    //pass uniform to shader

    gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);

    gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);

    gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);

    //pass light uniforms

    if (bLightingEnabled == true) {
        gl.uniform1i(keyPressUniform, 1);
        gl.uniform3fv(lightAmbientUniform, lightAmbient);
        gl.uniform3fv(lightDiffuseUniform, lightDiffuse);
        gl.uniform3fv(lightSpecularUniform, lightSpecular);
        gl.uniform4fv(lightPositionUniform, lightPosition);

        gl.uniform3fv(materialAmbientUniform, materialAmbient);
        gl.uniform3fv(materialDiffuseUniform, materialDiffuse);
        gl.uniform3fv(materialSpecularUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, Shininess);
    }

    else {
        gl.uniform1i(keyPressUniform, 0);
    }

    gl.viewport((horizontalDistance), (verticalDistance * 2), (WindowWidth / 4), (WindowHeight / 4));

    sphere.draw();

    //second column fifth sphere

    materialAmbient[0] = 0.24725;
    materialAmbient[1] = 0.1995;
    materialAmbient[2] = 0.0745;
   // materialAmbient[3] = 1.0f;

    materialDiffuse[0] = 0.75164;
    materialDiffuse[1] = 0.60648;
    materialDiffuse[2] = 0.22648;
    //materialDiffuse[3] = 1.0f;


    materialSpecular[0] = 0.628281;
    materialSpecular[1] = 0.555802;
    materialSpecular[2] = 0.366065;
    //materialSpecular[3] = 1.0f;


    Shininess = 0.4 * 128;

    //transformation


    var modelViewMatrix = mat4.create();

    var translationMatrix = mat4.create();

    var modelMatrix = mat4.create();

    var viewMatrix = mat4.create();

    //var modelViewProjectionMatrix = mat4.create();

    mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -8.0]);

    mat4.multiply(modelMatrix, modelMatrix, translationMatrix);


    //pass uniform to shader

    gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);

    gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);

    gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);

    //pass light uniforms

    if (bLightingEnabled == true) {
        gl.uniform1i(keyPressUniform, 1);
        gl.uniform3fv(lightAmbientUniform, lightAmbient);
        gl.uniform3fv(lightDiffuseUniform, lightDiffuse);
        gl.uniform3fv(lightSpecularUniform, lightSpecular);
        gl.uniform4fv(lightPositionUniform, lightPosition);

        gl.uniform3fv(materialAmbientUniform, materialAmbient);
        gl.uniform3fv(materialDiffuseUniform, materialDiffuse);
        gl.uniform3fv(materialSpecularUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, Shininess);
    }

    else {
        gl.uniform1i(keyPressUniform, 0);
    }

    gl.viewport((horizontalDistance), (verticalDistance), (WindowWidth / 4), (WindowHeight / 4));

    sphere.draw();

    //second column sixth sphere


    materialAmbient[0] = 0.19225;
    materialAmbient[1] = 0.19225;
    materialAmbient[2] = 0.19225;
   // materialAmbient[3] = 1.0f;

    materialDiffuse[0] = 0.50754;
    materialDiffuse[1] = 0.50754;
    materialDiffuse[2] = 0.50754;
   // materialDiffuse[3] = 1.0f;


    materialSpecular[0] = 0.508273;
    materialSpecular[1] = 0.508273;
    materialSpecular[2] = 0.508273;
   // materialSpecular[3] = 1.0f;


   Shininess = 0.4 * 128;


    Shininess = 0.4 * 128;

    //transformation


    var modelViewMatrix = mat4.create();

    var translationMatrix = mat4.create();

    var modelMatrix = mat4.create();

    var viewMatrix = mat4.create();

    //var modelViewProjectionMatrix = mat4.create();

    mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -8.0]);

    mat4.multiply(modelMatrix, modelMatrix, translationMatrix);


    //pass uniform to shader

    gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);

    gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);

    gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);

    //pass light uniforms

    if (bLightingEnabled == true) {
        gl.uniform1i(keyPressUniform, 1);
        gl.uniform3fv(lightAmbientUniform, lightAmbient);
        gl.uniform3fv(lightDiffuseUniform, lightDiffuse);
        gl.uniform3fv(lightSpecularUniform, lightSpecular);
        gl.uniform4fv(lightPositionUniform, lightPosition);

        gl.uniform3fv(materialAmbientUniform, materialAmbient);
        gl.uniform3fv(materialDiffuseUniform, materialDiffuse);
        gl.uniform3fv(materialSpecularUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, Shininess);
    }

    else {
        gl.uniform1i(keyPressUniform, 0);
    }

    gl.viewport((horizontalDistance), 0, (WindowWidth / 4), (WindowHeight / 4));

    sphere.draw();

    //third column first sphere

    materialAmbient[0] = 0.0;
    materialAmbient[1] = 0.0;
    materialAmbient[2] = 0.0;
    //materialAmbient[3] = 1.0f;

    materialDiffuse[0] = 0.01;
    materialDiffuse[1] = 0.01;
    materialDiffuse[2] = 0.01;
   // materialDiffuse[3] = 1.0f;


    materialSpecular[0] = 0.5;
    materialSpecular[1] = 0.5;
    materialSpecular[2] = 0.5;
    //materialSpecular[3] = 1.0f;


    Shininess = 0.25 * 128;

    //transformation


    var modelViewMatrix = mat4.create();

    var translationMatrix = mat4.create();

    var modelMatrix = mat4.create();

    var viewMatrix = mat4.create();

    //var modelViewProjectionMatrix = mat4.create();

    mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -8.0]);

    mat4.multiply(modelMatrix, modelMatrix, translationMatrix);


    //pass uniform to shader

    gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);

    gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);

    gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);

    //pass light uniforms

    if (bLightingEnabled == true) {
        gl.uniform1i(keyPressUniform, 1);
        gl.uniform3fv(lightAmbientUniform, lightAmbient);
        gl.uniform3fv(lightDiffuseUniform, lightDiffuse);
        gl.uniform3fv(lightSpecularUniform, lightSpecular);
        gl.uniform4fv(lightPositionUniform, lightPosition);

        gl.uniform3fv(materialAmbientUniform, materialAmbient);
        gl.uniform3fv(materialDiffuseUniform, materialDiffuse);
        gl.uniform3fv(materialSpecularUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, Shininess);
    }

    else {
        gl.uniform1i(keyPressUniform, 0);
    }

    gl.viewport((horizontalDistance * 2), (verticalDistance * 5), (WindowWidth / 4), (WindowHeight / 4));

    sphere.draw();

    //thirs column second sphere

    materialAmbient[0] = 0.0;
    materialAmbient[1] = 0.1;
    materialAmbient[2] = 0.06;
   // materialAmbient[3] = 1.0f;

    materialDiffuse[0] = 0.0;
    materialDiffuse[1] = 0.50980392;
    materialDiffuse[2] = 0.50980392;
    //materialDiffuse[3] = 1.0f;


    materialSpecular[0] = 0.50196078;
    materialSpecular[1] = 0.50196078;
    materialSpecular[2] = 0.50196078;
   // materialSpecular[3] = 1.0f;


    Shininess = 0.25 * 128;

    //transformation


    var modelViewMatrix = mat4.create();

    var translationMatrix = mat4.create();

    var modelMatrix = mat4.create();

    var viewMatrix = mat4.create();

    //var modelViewProjectionMatrix = mat4.create();

    mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -8.0]);

    mat4.multiply(modelMatrix, modelMatrix, translationMatrix);


    //pass uniform to shader

    gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);

    gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);

    gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);

    //pass light uniforms

    if (bLightingEnabled == true) {
        gl.uniform1i(keyPressUniform, 1);
        gl.uniform3fv(lightAmbientUniform, lightAmbient);
        gl.uniform3fv(lightDiffuseUniform, lightDiffuse);
        gl.uniform3fv(lightSpecularUniform, lightSpecular);
        gl.uniform4fv(lightPositionUniform, lightPosition);

        gl.uniform3fv(materialAmbientUniform, materialAmbient);
        gl.uniform3fv(materialDiffuseUniform, materialDiffuse);
        gl.uniform3fv(materialSpecularUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, Shininess);
    }

    else {
        gl.uniform1i(keyPressUniform, 0);
    }

    gl.viewport((horizontalDistance * 2), (verticalDistance * 4), (WindowWidth / 4), (WindowHeight / 4));

    sphere.draw();

    // third column third sphere

    materialAmbient[0] = 0.0;
    materialAmbient[1] = 0.0;
    materialAmbient[2] = 0.0;
    //materialAmbient[3] = 1.0f;

    materialDiffuse[0] = 0.1;
    materialDiffuse[1] = 0.35;
    materialDiffuse[2] = 0.1;
    //materialDiffuse[3] = 1.0f;


    materialSpecular[0] = 0.45;
    materialSpecular[1] = 0.55;
    materialSpecular[2] = 0.45;
    //materialSpecular[3] = 1.0f;

    Shininess = 0.25 * 128;

    //transformation


    var modelViewMatrix = mat4.create();

    var translationMatrix = mat4.create();

    var modelMatrix = mat4.create();

    var viewMatrix = mat4.create();

    //var modelViewProjectionMatrix = mat4.create();

    mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -8.0]);

    mat4.multiply(modelMatrix, modelMatrix, translationMatrix);


    //pass uniform to shader

    gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);

    gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);

    gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);

    //pass light uniforms

    if (bLightingEnabled == true) {
        gl.uniform1i(keyPressUniform, 1);
        gl.uniform3fv(lightAmbientUniform, lightAmbient);
        gl.uniform3fv(lightDiffuseUniform, lightDiffuse);
        gl.uniform3fv(lightSpecularUniform, lightSpecular);
        gl.uniform4fv(lightPositionUniform, lightPosition);

        gl.uniform3fv(materialAmbientUniform, materialAmbient);
        gl.uniform3fv(materialDiffuseUniform, materialDiffuse);
        gl.uniform3fv(materialSpecularUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, Shininess);
    }

    else {
        gl.uniform1i(keyPressUniform, 0);
    }

    gl.viewport((horizontalDistance * 2), (verticalDistance * 3), (WindowWidth / 4), (WindowHeight / 4));

    sphere.draw();

   // third column foruth sphere


    materialAmbient[0] = 0.0;
    materialAmbient[1] = 0.0;
    materialAmbient[2] = 0.0;
   // materialAmbient[3] = 1.0f;

    materialDiffuse[0] = 0.5;
    materialDiffuse[1] = 0.0;
    materialDiffuse[2] = 0.0;
    //materialDiffuse[3] = 1.0f;


    materialSpecular[0] = 0.7;
    materialSpecular[1] = 0.6;
    materialSpecular[2] = 0.6;
    //materialSpecular[3] = 1.0f;

    Shininess = 0.25 * 128;

    //transformation


    var modelViewMatrix = mat4.create();

    var translationMatrix = mat4.create();

    var modelMatrix = mat4.create();

    var viewMatrix = mat4.create();

    //var modelViewProjectionMatrix = mat4.create();

    mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -8.0]);

    mat4.multiply(modelMatrix, modelMatrix, translationMatrix);


    //pass uniform to shader

    gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);

    gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);

    gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);

    //pass light uniforms

    if (bLightingEnabled == true) {
        gl.uniform1i(keyPressUniform, 1);
        gl.uniform3fv(lightAmbientUniform, lightAmbient);
        gl.uniform3fv(lightDiffuseUniform, lightDiffuse);
        gl.uniform3fv(lightSpecularUniform, lightSpecular);
        gl.uniform4fv(lightPositionUniform, lightPosition);

        gl.uniform3fv(materialAmbientUniform, materialAmbient);
        gl.uniform3fv(materialDiffuseUniform, materialDiffuse);
        gl.uniform3fv(materialSpecularUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, Shininess);
    }

    else {
        gl.uniform1i(keyPressUniform, 0);
    }

    gl.viewport((horizontalDistance * 2), (verticalDistance * 2), (WindowWidth / 4), (WindowHeight / 4));

    sphere.draw();

    //third column fifth sphere

    materialAmbient[0] = 0.0;
    materialAmbient[1] = 0.0;
    materialAmbient[2] = 0.0;
    //materialAmbient[3] = 1.0f;

    materialDiffuse[0] = 0.55;
    materialDiffuse[1] = 0.55;
    materialDiffuse[2] = 0.55;
    //materialDiffuse[3] = 1.0f;


    materialSpecular[0] = 0.70;
    materialSpecular[1] = 0.70;
    materialSpecular[2] = 0.70;
    //materialSpecular[3] = 1.0f;

    Shininess = 0.25 * 128;

    //transformation


    var modelViewMatrix = mat4.create();

    var translationMatrix = mat4.create();

    var modelMatrix = mat4.create();

    var viewMatrix = mat4.create();

    //var modelViewProjectionMatrix = mat4.create();

    mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -8.0]);

    mat4.multiply(modelMatrix, modelMatrix, translationMatrix);


    //pass uniform to shader

    gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);

    gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);

    gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);

    //pass light uniforms

    if (bLightingEnabled == true) {
        gl.uniform1i(keyPressUniform, 1);
        gl.uniform3fv(lightAmbientUniform, lightAmbient);
        gl.uniform3fv(lightDiffuseUniform, lightDiffuse);
        gl.uniform3fv(lightSpecularUniform, lightSpecular);
        gl.uniform4fv(lightPositionUniform, lightPosition);

        gl.uniform3fv(materialAmbientUniform, materialAmbient);
        gl.uniform3fv(materialDiffuseUniform, materialDiffuse);
        gl.uniform3fv(materialSpecularUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, Shininess);
    }

    else {
        gl.uniform1i(keyPressUniform, 0);
    }

    gl.viewport((horizontalDistance * 2), (verticalDistance * 1), (WindowWidth / 4), (WindowHeight / 4));

    sphere.draw();

    //third column sixth sphere

    materialAmbient[0] = 0.0;
    materialAmbient[1] = 0.0;
    materialAmbient[2] = 0.0;
   // materialAmbient[3] = 1.0f;

    materialDiffuse[0] = 0.5;
    materialDiffuse[1] = 0.5;
    materialDiffuse[2] = 0.0;
    //materialDiffuse[3] = 1.0f;


    materialSpecular[0] = 0.60;
    materialSpecular[1] = 0.60;
    materialSpecular[2] = 0.50;
    //materialSpecular[3] = 1.0f;

    Shininess = 0.25 * 128;

    //transformation


    var modelViewMatrix = mat4.create();

    var translationMatrix = mat4.create();

    var modelMatrix = mat4.create();

    var viewMatrix = mat4.create();

    //var modelViewProjectionMatrix = mat4.create();

    mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -8.0]);

    mat4.multiply(modelMatrix, modelMatrix, translationMatrix);


    //pass uniform to shader

    gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);

    gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);

    gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);

    //pass light uniforms

    if (bLightingEnabled == true) {
        gl.uniform1i(keyPressUniform, 1);
        gl.uniform3fv(lightAmbientUniform, lightAmbient);
        gl.uniform3fv(lightDiffuseUniform, lightDiffuse);
        gl.uniform3fv(lightSpecularUniform, lightSpecular);
        gl.uniform4fv(lightPositionUniform, lightPosition);

        gl.uniform3fv(materialAmbientUniform, materialAmbient);
        gl.uniform3fv(materialDiffuseUniform, materialDiffuse);
        gl.uniform3fv(materialSpecularUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, Shininess);
    }

    else {
        gl.uniform1i(keyPressUniform, 0);
    }

    gl.viewport((horizontalDistance * 2), (verticalDistance * 0), (WindowWidth / 4), (WindowHeight / 4));

    sphere.draw();

    //fourth column first sphere


    materialAmbient[0] = 0.02;
    materialAmbient[1] = 0.02;
    materialAmbient[2] = 0.02;
   

    materialDiffuse[0] = 0.01;
    materialDiffuse[1] = 0.01;
    materialDiffuse[2] = 0.01;



    materialSpecular[0] = 0.4;
    materialSpecular[1] = 0.4;
    materialSpecular[2] = 0.4;
  

    Shininess = 0.078125 * 128;

    //transformation


    var modelViewMatrix = mat4.create();

    var translationMatrix = mat4.create();

    var modelMatrix = mat4.create();

    var viewMatrix = mat4.create();

    //var modelViewProjectionMatrix = mat4.create();

    mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -8.0]);

    mat4.multiply(modelMatrix, modelMatrix, translationMatrix);


    //pass uniform to shader

    gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);

    gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);

    gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);

    //pass light uniforms

    if (bLightingEnabled == true) {
        gl.uniform1i(keyPressUniform, 1);
        gl.uniform3fv(lightAmbientUniform, lightAmbient);
        gl.uniform3fv(lightDiffuseUniform, lightDiffuse);
        gl.uniform3fv(lightSpecularUniform, lightSpecular);
        gl.uniform4fv(lightPositionUniform, lightPosition);

        gl.uniform3fv(materialAmbientUniform, materialAmbient);
        gl.uniform3fv(materialDiffuseUniform, materialDiffuse);
        gl.uniform3fv(materialSpecularUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, Shininess);
    }

    else {
        gl.uniform1i(keyPressUniform, 0);
    }

    gl.viewport((horizontalDistance * 3), (verticalDistance * 5), (WindowWidth / 4), (WindowHeight / 4));

    sphere.draw();

    //fourth column second sphere


    materialAmbient[0] = 0.0;
    materialAmbient[1] = 0.05;
    materialAmbient[2] = 0.05;
    //materialAmbient[3] = 1.0f;

    materialDiffuse[0] = 0.4;
    materialDiffuse[1] = 0.5;
    materialDiffuse[2] = 0.5;
    //materialDiffuse[3] = 1.0f;


    materialSpecular[0] = 0.04;
    materialSpecular[1] = 0.7;
    materialSpecular[2] = 0.7;
   // materialSpecular[3] = 1.0f;

    Shininess = 0.078125 * 128;

    //transformation


    var modelViewMatrix = mat4.create();

    var translationMatrix = mat4.create();

    var modelMatrix = mat4.create();

    var viewMatrix = mat4.create();

    //var modelViewProjectionMatrix = mat4.create();

    mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -8.0]);

    mat4.multiply(modelMatrix, modelMatrix, translationMatrix);


    //pass uniform to shader

    gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);

    gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);

    gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);

    //pass light uniforms

    if (bLightingEnabled == true) {
        gl.uniform1i(keyPressUniform, 1);
        gl.uniform3fv(lightAmbientUniform, lightAmbient);
        gl.uniform3fv(lightDiffuseUniform, lightDiffuse);
        gl.uniform3fv(lightSpecularUniform, lightSpecular);
        gl.uniform4fv(lightPositionUniform, lightPosition);

        gl.uniform3fv(materialAmbientUniform, materialAmbient);
        gl.uniform3fv(materialDiffuseUniform, materialDiffuse);
        gl.uniform3fv(materialSpecularUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, Shininess);
    }

    else {
        gl.uniform1i(keyPressUniform, 0);
    }

    gl.viewport((horizontalDistance * 3), (verticalDistance * 4), (WindowWidth / 4), (WindowHeight / 4));

    sphere.draw();

    //fourth column third sphere



    materialAmbient[0] = 0.0;
    materialAmbient[1] = 0.05;
    materialAmbient[2] = 0.0;
   // materialAmbient[3] = 1.0f;

    materialDiffuse[0] = 0.4;
    materialDiffuse[1] = 0.5;
    materialDiffuse[2] = 0.4;
    //materialDiffuse[3] = 1.0f;

    materialSpecular[0] = 0.04;
    materialSpecular[1] = 0.7;
    materialSpecular[2] = 0.04;
   // materialSpecular[3] = 1.0f;

   Shininess = 0.078125 * 128;

    //transformation


    var modelViewMatrix = mat4.create();

    var translationMatrix = mat4.create();

    var modelMatrix = mat4.create();

    var viewMatrix = mat4.create();

    //var modelViewProjectionMatrix = mat4.create();

    mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -8.0]);

    mat4.multiply(modelMatrix, modelMatrix, translationMatrix);


    //pass uniform to shader

    gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);

    gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);

    gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);

    //pass light uniforms

    if (bLightingEnabled == true) {
        gl.uniform1i(keyPressUniform, 1);
        gl.uniform3fv(lightAmbientUniform, lightAmbient);
        gl.uniform3fv(lightDiffuseUniform, lightDiffuse);
        gl.uniform3fv(lightSpecularUniform, lightSpecular);
        gl.uniform4fv(lightPositionUniform, lightPosition);

        gl.uniform3fv(materialAmbientUniform, materialAmbient);
        gl.uniform3fv(materialDiffuseUniform, materialDiffuse);
        gl.uniform3fv(materialSpecularUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, Shininess);
    }

    else {
        gl.uniform1i(keyPressUniform, 0);
    }

    gl.viewport((horizontalDistance * 3), (verticalDistance * 3), (WindowWidth / 4), (WindowHeight / 4));

    sphere.draw();

    //fourth column fourth sphere


    materialAmbient[0] = 0.05;
    materialAmbient[1] = 0.0;
    materialAmbient[2] = 0.0;
   // materialAmbient[3] = 1.0f;

    materialDiffuse[0] = 0.5;
    materialDiffuse[1] = 0.4;
    materialDiffuse[2] = 0.4;
   // materialDiffuse[3] = 1.0f;

    materialSpecular[0] = 0.7;
    materialSpecular[1] = 0.04;
    materialSpecular[2] = 0.04;
    //materialSpecular[3] = 1.0f;

   Shininess = 0.078125 * 128;

    //transformation


    var modelViewMatrix = mat4.create();

    var translationMatrix = mat4.create();

    var modelMatrix = mat4.create();

    var viewMatrix = mat4.create();

    //var modelViewProjectionMatrix = mat4.create();

    mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -8.0]);

    mat4.multiply(modelMatrix, modelMatrix, translationMatrix);


    //pass uniform to shader

    gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);

    gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);

    gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);

    //pass light uniforms

    if (bLightingEnabled == true) {
        gl.uniform1i(keyPressUniform, 1);
        gl.uniform3fv(lightAmbientUniform, lightAmbient);
        gl.uniform3fv(lightDiffuseUniform, lightDiffuse);
        gl.uniform3fv(lightSpecularUniform, lightSpecular);
        gl.uniform4fv(lightPositionUniform, lightPosition);

        gl.uniform3fv(materialAmbientUniform, materialAmbient);
        gl.uniform3fv(materialDiffuseUniform, materialDiffuse);
        gl.uniform3fv(materialSpecularUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, Shininess);
    }

    else {
        gl.uniform1i(keyPressUniform, 0);
    }

    gl.viewport((horizontalDistance * 3), (verticalDistance * 2), (WindowWidth / 4), (WindowHeight / 4));

    sphere.draw();

    //fourth column fifth sphere


    materialAmbient[0] = 0.05;
    materialAmbient[1] = 0.05;
    materialAmbient[2] = 0.05;
   // materialAmbient[3] = 1.0f;

    materialDiffuse[0] = 0.5;
    materialDiffuse[1] = 0.5;
    materialDiffuse[2] = 0.5;
    //materialDiffuse[3] = 1.0f;

    materialSpecular[0] = 0.7;
    materialSpecular[1] = 0.7;
    materialSpecular[2] = 0.7;
   // materialSpecular[3] = 1.0f;

    Shininess = 0.078125 * 128;

    //transformation


    var modelViewMatrix = mat4.create();

    var translationMatrix = mat4.create();

    var modelMatrix = mat4.create();

    var viewMatrix = mat4.create();

    //var modelViewProjectionMatrix = mat4.create();

    mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -8.0]);

    mat4.multiply(modelMatrix, modelMatrix, translationMatrix);


    //pass uniform to shader

    gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);

    gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);

    gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);

    //pass light uniforms

    if (bLightingEnabled == true) {
        gl.uniform1i(keyPressUniform, 1);
        gl.uniform3fv(lightAmbientUniform, lightAmbient);
        gl.uniform3fv(lightDiffuseUniform, lightDiffuse);
        gl.uniform3fv(lightSpecularUniform, lightSpecular);
        gl.uniform4fv(lightPositionUniform, lightPosition);

        gl.uniform3fv(materialAmbientUniform, materialAmbient);
        gl.uniform3fv(materialDiffuseUniform, materialDiffuse);
        gl.uniform3fv(materialSpecularUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, Shininess);
    }

    else {
        gl.uniform1i(keyPressUniform, 0);
    }

    gl.viewport((horizontalDistance * 3), (verticalDistance * 1), (WindowWidth / 4), (WindowHeight / 4));

    sphere.draw();

    //fourth column sixth sphere


    materialAmbient[0] = 0.05;
    materialAmbient[1] = 0.05;
    materialAmbient[2] = 0.0;
    //materialAmbient[3] = 1.0f;

    materialDiffuse[0] = 0.5;
    materialDiffuse[1] = 0.5;
    materialDiffuse[2] = 0.4;
   // materialDiffuse[3] = 1.0f;

    materialSpecular[0] = 0.7;
    materialSpecular[1] = 0.7;
    materialSpecular[2] = 0.04;
    //materialSpecular[3] = 1.0f;


    Shininess = 0.078125 * 128;

    //transformation


    var modelViewMatrix = mat4.create();

    var translationMatrix = mat4.create();

    var modelMatrix = mat4.create();

    var viewMatrix = mat4.create();

    //var modelViewProjectionMatrix = mat4.create();

    mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -8.0]);

    mat4.multiply(modelMatrix, modelMatrix, translationMatrix);


    //pass uniform to shader

    gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);

    gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);

    gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);

    //pass light uniforms

    if (bLightingEnabled == true) {
        gl.uniform1i(keyPressUniform, 1);
        gl.uniform3fv(lightAmbientUniform, lightAmbient);
        gl.uniform3fv(lightDiffuseUniform, lightDiffuse);
        gl.uniform3fv(lightSpecularUniform, lightSpecular);
        gl.uniform4fv(lightPositionUniform, lightPosition);

        gl.uniform3fv(materialAmbientUniform, materialAmbient);
        gl.uniform3fv(materialDiffuseUniform, materialDiffuse);
        gl.uniform3fv(materialSpecularUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, Shininess);
    }

    else {
        gl.uniform1i(keyPressUniform, 0);
    }

    gl.viewport((horizontalDistance * 3), (verticalDistance * 0), (WindowWidth / 4), (WindowHeight / 4));

    sphere.draw();


    gl.useProgram(null);

    if (bLightingEnabled == true)
    {
        update();
    }

   

    //double buffering

    requestAnimationFrame(display, canvas);
    
}

function update()
{

    if (keyPressed == 1)
    {
        if (angleForXRotation < (2 * Math.PI))   // rotation around X axis
        {
            lightPosition[0] = 0.0;
            lightPosition[1] = 45.0 * Math.sin(angleForXRotation);
            lightPosition[2] = 45.0 * Math.cos(angleForXRotation);
            lightPosition[3] = 1.0;

            angleForXRotation = angleForXRotation + 0.01;
        }

        else
        {
            angleForXRotation = angleForXRotation - (2 * Math.PI);
        }

    }

    if (keyPressed == 2)   // rotation around Y axis
    {
        if (angleForYRotation < (2 * Math.PI))
        {
            lightPosition[0] = 45.0 * Math.sin(angleForYRotation);
            lightPosition[1] = 0.0;
            lightPosition[2] = 45.0 * Math.cos(angleForYRotation);
            lightPosition[3] = 1.0;

            angleForYRotation = angleForYRotation + 0.01;
        }

        else
        {
            angleForYRotation = angleForYRotation - (2 * Math.PI);
        }

    }

    if (keyPressed == 3)   // rotation around Z axis
    {
        if (angleForZRotation < (2 * Math.PI))
        {
            lightPosition[0] = 45.0 * Math.sin(angleForZRotation);
            lightPosition[1] = 45.0 * Math.cos(angleForZRotation);
            lightPosition[2] = 0.0;
            lightPosition[3] = 1.0;

            angleForZRotation = angleForZRotation + 0.01;
        }

        else
        {
            angleForZRotation = angleForZRotation - (2 * Math.PI);
        }

    }


   
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

    if (sphere)
    {
        sphere.deallocate();
        sphere = null;
    }




    

    
}





