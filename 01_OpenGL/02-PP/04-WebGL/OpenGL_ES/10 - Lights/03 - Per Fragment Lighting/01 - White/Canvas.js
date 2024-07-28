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


var lightAmbient = new Float32Array([0.1, 0.1, 0.1]);

var lightDiffuse = new Float32Array([ 1.0, 1.0, 1.0]);
var lightSpecular = new Float32Array([ 1.0, 1.0, 1.0]);
var lightPosition = new Float32Array([ 100.0, 100.0, 100.0, 1.0]);

//material Properties

var materialAmbient = new Float32Array([ 0.0, 0.0, 0.0]);
var materialDiffuse = new Float32Array([ 0.8, 0.8, 0.8]);
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

        case 70:
        case 102:
            toggleFullscreen();
            
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


    //var modelViewMatrix = mat4.create();

    var translationMatrix = mat4.create();

    var modelMatrix = mat4.create();

    var viewMatrix = mat4.create();

    //var modelViewProjectionMatrix = mat4.create();

    mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -5.3]);

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
        gl.uniform1f(materialShininessUniform, materialShininess);
    }

    else
    {
        gl.uniform1i(keyPressUniform, 0);
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





