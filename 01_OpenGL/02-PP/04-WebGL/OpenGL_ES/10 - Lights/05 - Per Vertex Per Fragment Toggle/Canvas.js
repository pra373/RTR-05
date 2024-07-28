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

var shaderProgramObject_PerVertex = null;

var shaderProgramObject_PerFragment = null;

//transformation related uniform

var modelMatrixUniform_PerVertex = 0;
var viewMatrixUniform_PerVertex = 0;
var projectionMatrixUniform_PerVertex = 0;

//light related uniforms

var lightDiffuseUniform_PerVertex = 0;

var lightPositionUniform_PerVertex = 0;
var lightAmbientUniform_PerVertex = 0;
var lightSpecularUniform_PerVertex = 0;


var materialAmbientUniform_PerVertex = 0;
var materialDiffuseUniform_PerVertex = 0;
var materialSpecularUniform_PerVertex = 0;
var materialShininessUniform_PerVertex = 0;

var keyPressUniform_PerVertex = 0;

// Per fragment uniforms

//transformation related uniform

var modelMatrixUniform_PerFragment = 0;
var viewMatrixUniform_PerFragment = 0;
var projectionMatrixUniform_PerFragment = 0;

//light related uniforms

var lightDiffuseUniform_PerFragment = 0;

var lightPositionUniform_PerFragment = 0;
var lightAmbientUniform_PerFragment = 0;
var lightSpecularUniform_PerFragment = 0;


var materialAmbientUniform_PerFragment = 0;
var materialDiffuseUniform_PerFragment = 0;
var materialSpecularUniform_PerFragment = 0;
var materialShininessUniform_PerFragment = 0;

var keyPressUniform_PerFragment = 0;

var bLightingEnabled = false;

var select_SPO = 0;

//light source properties


var lightAmbient = new Float32Array([0.1, 0.1, 0.1]);

var lightDiffuse = new Float32Array([ 1.0, 1.0, 1.0]);
var lightSpecular = new Float32Array([ 1.0, 1.0, 1.0]);
var lightPosition = new Float32Array([ 100.0, 100.0, 100.0, 1.0]);

//material Properties

var materialAmbient = new Float32Array([ 0.0, 0.0, 0.0]);
var materialDiffuse = new Float32Array([ 0.5, 0.2, 0.7]);
var materialSpecular = new Float32Array([1.0, 1.0, 1.0]);

var materialShininess = 128.0;



//var mvpMatrixUniform;

var perspectiveProjectionMatrix;


var requestAnimationFrame =
    window.requestAnimationFrame ||
    window.webkitRequestAnimationFrame ||
    window.mozRequestAnimationFrame ||
    window.oRequestAnimationFrame ||
    window.msRequestAnimationFrame;

//sphere related variable

var sphere = null;

                 

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

        case 69:         // letter E and e for full screen (e for enlarge)
        case 101:

            toggleFullscreen();
            
            break;

        case 70:         // letter F and f
        case 102:

            select_SPO = 1;

            break;

        case 86:         // letter v and V
        case 118:

            select_SPO = 0;

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

    //************************************ Per vertex lighting starts here ****************************************************/


    //vertex shader

    var vertexShaderSourceCode_PerVertex =
        (
            "#version 300 es\n" +
            "precision highp float;\n" +
            "in vec4 aPosition;\n" +
            "in vec3 aNormal;\n" +
            "uniform mat4 uProjectionMatrix;\n" +
            "uniform mat4 uModelMatrix;\n" +
            "uniform mat4 uViewMatrix;\n" +
            "uniform vec3 uLightAmbient;\n" +
            "uniform vec3 uLightDiffuse;\n" +
            "uniform vec3 uLightSpecular;\n" +
            "uniform vec4 uLightPosition;\n" +
            "uniform vec3 uMaterialAmbient;\n" +
            "uniform vec3 uMaterialDiffuse;\n" +
            "uniform vec3 uMaterialSpecular;\n" +
            "uniform float uMaterialShininess;\n" +
            "uniform highp int uKeyPress;\n" +
            "out vec3 oPhongADSLight;\n" +
            "void main(void)\n" +
            "{\n" +
            "    if (uKeyPress == 1)\n" +
            "    {\n" +
            "        vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;\n" +
            "        vec3 transformedNormals = normalize(mat3(uViewMatrix * uModelMatrix) * aNormal);\n" +
            "        vec3 lightDirection = normalize(vec3(uLightPosition - eyeCoordinates));\n" +
            "        vec3 reflectionVector = reflect(-lightDirection, transformedNormals);\n" +
            "        vec3 viewerVector = normalize(-eyeCoordinates.xyz);\n" +
            "        vec3 ambientLight = uLightAmbient * uMaterialAmbient;\n" +
            "        vec3 diffuseLight = uLightDiffuse * uMaterialDiffuse * max(dot(lightDirection, transformedNormals), 0.0);\n" +
            "        vec3 lightSpecular = uLightSpecular * uMaterialSpecular * pow(max(dot(reflectionVector, viewerVector), 0.0), uMaterialShininess);\n" +
            "        oPhongADSLight = ambientLight + diffuseLight + lightSpecular;\n" +
            "    }\n" +
            "    else\n" +
            "    {\n" +
            "        oPhongADSLight = vec3(0.0, 0.0, 0.0);\n" +
            "    }\n" +
            "    gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;\n" +
            "}"
        );


    var vertexShaderObject_PerVertex = gl.createShader(gl.VERTEX_SHADER)
    gl.shaderSource(vertexShaderObject_PerVertex, vertexShaderSourceCode_PerVertex);

    gl.compileShader(vertexShaderObject_PerVertex);

    if (gl.getShaderParameter(vertexShaderObject_PerVertex, gl.COMPILE_STATUS) == false) {
        var error = gl.getShaderInfoLog(vertexShaderObject_PerVertex);

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

    var fragmentShaderSourceCode_PerVertex =
        (
            "#version 300 es\n" +
            "precision highp float;\n" +
            "in vec3 oPhongADSLight;\n" +
            "uniform highp int uKeyPress;\n" +
            "out vec4 FragColor;\n" +
            "void main(void)\n" +
            "{\n" +
            "    if (uKeyPress == 1)\n" +
            "    {\n" +
            "        FragColor = vec4(oPhongADSLight, 1.0);\n" +
            "    }\n" +
            "    else\n" +
            "    {\n" +
            "        FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n" +
            "    }\n" +
            "}"
        );

    var fragmentShaderObject_PerVertex = gl.createShader(gl.FRAGMENT_SHADER);

    gl.shaderSource(fragmentShaderObject_PerVertex, fragmentShaderSourceCode_PerVertex);

    gl.compileShader(fragmentShaderObject_PerVertex);

    if (gl.getShaderParameter(fragmentShaderObject_PerVertex, gl.COMPILE_STATUS) == false)
    {
        var error = gl.getShaderInfoLog(fragmentShaderObject_PerVertex);

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

    shaderProgramObject_PerVertex = gl.createProgram();

    gl.attachShader(shaderProgramObject_PerVertex, vertexShaderObject_PerVertex);
    gl.attachShader(shaderProgramObject_PerVertex, fragmentShaderObject_PerVertex);

    gl.bindAttribLocation(shaderProgramObject_PerVertex, vertexAttributeEnum.AMC_ATTRIBUTE_POSITION, "aPosition");

    gl.bindAttribLocation(shaderProgramObject_PerVertex, vertexAttributeEnum.AMC_ATTRIBUTE_NORMAL, "aNormal");

    gl.linkProgram(shaderProgramObject_PerVertex);

    if (gl.getProgramParameter(shaderProgramObject_PerVertex, gl.LINK_STATUS) == false)
    {
        var error = gl.getProgramInfoLog(shaderProgramObject_PerVertex);

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

    modelMatrixUniform_PerVertex = gl.getUniformLocation(shaderProgramObject_PerVertex, "uModelMatrix");

    viewMatrixUniform_PerVertex = gl.getUniformLocation(shaderProgramObject_PerVertex, "uViewMatrix");

    projectionMatrixUniform_PerVertex = gl.getUniformLocation(shaderProgramObject_PerVertex, "uProjectionMatrix");

    lightAmbientUniform_PerVertex = gl.getUniformLocation(shaderProgramObject_PerVertex, "uLightAmbient");

    lightDiffuseUniform_PerVertex = gl.getUniformLocation(shaderProgramObject_PerVertex, "uLightDiffuse");

    lightSpecularUniform_PerVertex = gl.getUniformLocation(shaderProgramObject_PerVertex, "uLightSpecular");

    lightPositionUniform_PerVertex = gl.getUniformLocation(shaderProgramObject_PerVertex, "uLightPosition");

    materialAmbientUniform_PerVertex = gl.getUniformLocation(shaderProgramObject_PerVertex, "uMaterialAmbient");

    materialDiffuseUniform_PerVertex = gl.getUniformLocation(shaderProgramObject_PerVertex, "uMaterialDiffuse");

    materialSpecularUniform_PerVertex = gl.getUniformLocation(shaderProgramObject_PerVertex, "uMaterialSpecular");

    materialShininessUniform_PerVertex = gl.getUniformLocation(shaderProgramObject_PerVertex, "uMaterialShininess");


    keyPressUniform_PerVertex = gl.getUniformLocation(shaderProgramObject_PerVertex, "uKeyPress");


    //************************************ Per fragment lighting starts here ****************************************************/

    //vertex shader

    var vertexShaderSourceCode_PerFragment =
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

    var vertexShaderObject_PerFragment = gl.createShader(gl.VERTEX_SHADER)
    gl.shaderSource(vertexShaderObject_PerFragment, vertexShaderSourceCode_PerFragment);

    gl.compileShader(vertexShaderObject_PerFragment);


    if (gl.getShaderParameter(vertexShaderObject_PerFragment, gl.COMPILE_STATUS) == false)
    {
        var error = gl.getShaderInfoLog(vertexShaderObject_PerFragment);

            if (error.length > 0) {
                var log = "vertex shader compilation error : " + error;
                alert(log);
                uninitialize();
            }
    }

    else
    {
        console.log("Vertex Shader Compiled Successfully !\n");

    }

    //fragment shader per fragment

    var fragmentShaderSourceCode_PerFragment =
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

    var fragmentShaderObject_PerFragment = gl.createShader(gl.FRAGMENT_SHADER);

    gl.shaderSource(fragmentShaderObject_PerFragment, fragmentShaderSourceCode_PerFragment);

    gl.compileShader(fragmentShaderObject_PerFragment);

    if (gl.getShaderParameter(fragmentShaderObject_PerFragment, gl.COMPILE_STATUS) == false) {
        var error = gl.getShaderInfoLog(fragmentShaderObject_PerFragment);

        if (error.length > 0) {
            var log = "fragment shader compilation error : " + error;
            alert(log);
            uninitialize();
        }
    }

    else {
        console.log("fragment Shader Compiled Successfully !\n");

    }

    //shader program per fragment

    shaderProgramObject_PerFragment = gl.createProgram();

    gl.attachShader(shaderProgramObject_PerFragment, vertexShaderObject_PerFragment);
    gl.attachShader(shaderProgramObject_PerFragment, fragmentShaderObject_PerFragment);

    gl.bindAttribLocation(shaderProgramObject_PerFragment, vertexAttributeEnum.AMC_ATTRIBUTE_POSITION, "aPosition");

    gl.bindAttribLocation(shaderProgramObject_PerFragment, vertexAttributeEnum.AMC_ATTRIBUTE_NORMAL, "aNormal");

    gl.linkProgram(shaderProgramObject_PerFragment);

    if (gl.getProgramParameter(shaderProgramObject_PerFragment, gl.LINK_STATUS) == false) {
        var error = gl.getProgramInfoLog(shaderProgramObject_PerFragment);

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


    modelMatrixUniform_PerFragment = gl.getUniformLocation(shaderProgramObject_PerFragment, "uModelMatrix");

    viewMatrixUniform_PerFragment = gl.getUniformLocation(shaderProgramObject_PerFragment, "uViewMatrix");

    projectionMatrixUniform_PerFragment = gl.getUniformLocation(shaderProgramObject_PerFragment, "uProjectionMatrix");

    lightAmbientUniform_PerFragment = gl.getUniformLocation(shaderProgramObject_PerFragment, "ulightAmbient");

    lightDiffuseUniform_PerFragment = gl.getUniformLocation(shaderProgramObject_PerFragment, "ulightDiffuse");

    lightSpecularUniform_PerFragment = gl.getUniformLocation(shaderProgramObject_PerFragment, "ulightSpecular");

    lightPositionUniform_PerFragment = gl.getUniformLocation(shaderProgramObject_PerFragment, "ulightPosition");

    materialAmbientUniform_PerFragment = gl.getUniformLocation(shaderProgramObject_PerFragment, "uMaterialAmbient");

    materialDiffuseUniform_PerFragment = gl.getUniformLocation(shaderProgramObject_PerFragment, "uMaterialDiffuse");

    materialSpecularUniform_PerFragment = gl.getUniformLocation(shaderProgramObject_PerFragment, "uMaterialSpecular");

    materialShininessUniform_PerFragment = gl.getUniformLocation(shaderProgramObject_PerFragment, "uMaterialShininess");


    keyPressUniform_PerFragment = gl.getUniformLocation(shaderProgramObject_PerFragment, "uKeyPress");

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


    if (select_SPO == 0)
    {
        gl.useProgram(shaderProgramObject_PerVertex);
    }

    if (select_SPO == 1)
    {
        gl.useProgram(shaderProgramObject_PerFragment);
    }
    


    //transformation


    var modelViewMatrix = mat4.create();

    var translationMatrix = mat4.create();

    var modelMatrix = mat4.create();

    var viewMatrix = mat4.create();

    //var modelViewProjectionMatrix = mat4.create();

    mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -5.3]);

    mat4.multiply(modelMatrix, modelMatrix, translationMatrix);


    //pass uniform to shader

    if (select_SPO == 0)
    {
        gl.uniformMatrix4fv(modelMatrixUniform_PerVertex, false, modelMatrix);

        gl.uniformMatrix4fv(viewMatrixUniform_PerVertex, false, viewMatrix);

        gl.uniformMatrix4fv(projectionMatrixUniform_PerVertex, false, perspectiveProjectionMatrix);
    }

    if (select_SPO == 1)
    {
        gl.uniformMatrix4fv(modelMatrixUniform_PerFragment, false, modelMatrix);

        gl.uniformMatrix4fv(viewMatrixUniform_PerFragment, false, viewMatrix);

        gl.uniformMatrix4fv(projectionMatrixUniform_PerFragment, false, perspectiveProjectionMatrix);
    }

   

    //pass light uniforms

    if (select_SPO == 0)
    {
        if (bLightingEnabled == true)
        {
            gl.uniform1i(keyPressUniform_PerVertex, 1);
            gl.uniform3fv(lightAmbientUniform_PerVertex, lightAmbient);
            gl.uniform3fv(lightDiffuseUniform_PerVertex, lightDiffuse);
            gl.uniform3fv(lightSpecularUniform_PerVertex, lightSpecular);
            gl.uniform4fv(lightPositionUniform_PerVertex, lightPosition);

            gl.uniform3fv(materialAmbientUniform_PerVertex, materialAmbient);
            gl.uniform3fv(materialDiffuseUniform_PerVertex, materialDiffuse);
            gl.uniform3fv(materialSpecularUniform_PerVertex, materialSpecular);
            gl.uniform1f(materialShininessUniform_PerVertex, materialShininess);
        }
        
         

        else
        {
            gl.uniform1i(keyPressUniform_PerVertex, 0);
            //gl.uniform1i(keyPressUniform_PerFragment, 0);
        }

        

        
        
    }

    
    if (select_SPO == 1)
    {
        if (bLightingEnabled == true)
        {
            gl.uniform1i(keyPressUniform_PerFragment, 1);
            gl.uniform3fv(lightAmbientUniform_PerFragment, lightAmbient);
            gl.uniform3fv(lightDiffuseUniform_PerFragment, lightDiffuse);
            gl.uniform3fv(lightSpecularUniform_PerFragment, lightSpecular);
            gl.uniform4fv(lightPositionUniform_PerFragment, lightPosition);

            gl.uniform3fv(materialAmbientUniform_PerFragment, materialAmbient);
            gl.uniform3fv(materialDiffuseUniform_PerFragment, materialDiffuse);
            gl.uniform3fv(materialSpecularUniform_PerFragment, materialSpecular);
            gl.uniform1f(materialShininessUniform_PerFragment, materialShininess);
        }

        else
        {
            gl.uniform1i(keyPressUniform_PerFragment, 0);
            //gl.uniform1i(keyPressUniform_PerFragment, 0);
        }
        
    }
   

    

   /* gl.bindVertexArray(vao);

    gl.drawArrays(gl.TRIANGLES, 0, 3);

    gl.bindVertexArray(null);
*/

    sphere.draw();

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

    if (shaderProgramObject_PerVertex)  //this statement can be shaderProgramObject != null
    {
        gl.useProgram(shaderProgramObject_PerVertex);

        var shaderObjects = gl.getAttachedShaders(shaderProgramObject_PerVertex);

        if (shaderObjects && shaderObjects.length > 0)
        {
            for (let i = 0; i < shaderObjects.length; i++)
            {
                gl.detachShader(shaderProgramObject_PerVertex, shaderObjects[i]);
                gl.deleteShader(shaderObjects[i]);
                shaderObjects[i] = null;

            }


        }

        gl.useProgram(null);
        gl.deleteProgram(shaderProgramObject_PerVertex);
        shaderProgramObject_PerVertex = null;


    }

    if (shaderProgramObject_PerFragment)  //this statement can be shaderProgramObject != null
    {
        gl.useProgram(shaderProgramObject_PerFragment);

        var shaderObjects = gl.getAttachedShaders(shaderProgramObject_PerFragment);

        if (shaderObjects && shaderObjects.length > 0) {
            for (let i = 0; i < shaderObjects.length; i++) {
                gl.detachShader(shaderProgramObject_PerFragment, shaderObjects[i]);
                gl.deleteShader(shaderObjects[i]);
                shaderObjects[i] = null;

            }


        }

        gl.useProgram(null);
        gl.deleteProgram(shaderProgramObject_PerFragment);
        shaderProgramObject_PerFragment = null;


    }


    if (sphere)
    {
        sphere.deallocate();
        sphere = null;
    }




    

    
}





