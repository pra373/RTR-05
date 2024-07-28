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

var shaderProgramObject_PerVertex_Sphere = null;

var shaderProgramObject_PerFragment_Sphere = null;

//sphere related variable

var sphere = null;



var perspectiveProjectionMatrix_Sphere;


var requestAnimationFrame =
    window.requestAnimationFrame ||
    window.webkitRequestAnimationFrame ||
    window.mozRequestAnimationFrame ||
    window.oRequestAnimationFrame ||
    window.msRequestAnimationFrame;

// Uniforms

var modelMatrixUniform_PerVertex_Sphere = 0;
var viewMatrixUniform_PerVertex_Sphere = 0;
var projectionMatrixUniform_PerVertex_Sphere = 0;

var lightDiffuseUniform_PerVertex_Sphere = new Array(3);
var lightAmbientUniform_PerVertex_Sphere = new Array(3);
var lightSpecularUniform_PerVertex_Sphere = new Array(3);
var lightPositionUniform_PerVertex_Sphere = new Array(3);

var materialDiffuseUniform_PerVertex_Sphere = 0;
var materialAmbientUniform_PerVertex_Sphere = 0;
var materialSpecularUniform_PerVertex_Sphere = 0;
var materialShininessUniform_PerVertex_Sphere = 0;

var keyPressUniform_PerVertex_Sphere = 0;

//per fragment lighting uniforms

var modelMatrixUniform_PerFragment_Sphere = 0;
var viewMatrixUniform_PerFragment_Sphere = 0;
var projectionMatrixUniform_PerFragment_Sphere = 0;

var lightDiffuseUniform_PerFragment_Sphere = new Array(3);
var lightAmbientUniform_PerFragment_Sphere = new Array(3);
var lightSpecularUniform_PerFragment_Sphere = new Array(3);
var lightPositionUniform_PerFragment_Sphere = new Array(3);

var materialDiffuseUniform_PerFragment_Sphere = 0;
var materialAmbientUniform_PerFragment_Sphere = 0;
var materialSpecularUniform_PerFragment_Sphere = 0;
var materialShininessUniform_PerFragment_Sphere = 0;

var keyPressUniform_PerFragment_Sphere = 0;



var bLightingEnable = false;

var bAnimationEnable = false;

var select_SPO = 0;

//material properties

var materialAmbient = new Float32Array([0.0, 0.0, 0.0]);
var materialDiffuse = new Float32Array([ 1.0, 1.0, 1.0]);
var materialSpecular = new Float32Array([ 1.0, 1.0, 1.0]);
var materialShininess = 128.0;


//lights properties

var light1Position = new Float32Array(4);
var light1Diffuse = new Float32Array(3);
var light1Specular = new Float32Array(3);
var light1Ambient = new Float32Array(3);

var light2Position = new Float32Array(4);
var light2Diffuse = new Float32Array(3);
var light2Specular = new Float32Array(3);
var light2Ambient = new Float32Array(3);

var light3Position = new Float32Array(4);
var light3Diffuse = new Float32Array(3);
var light3Specular = new Float32Array(3);
var light3Ambient = new Float32Array(3);

var light4Position = new Float32Array(4);
var light4Diffuse = new Float32Array(3);
var light4Specular = new Float32Array(3);
var light4Ambient = new Float32Array(3);

var light5Position = new Float32Array(4);
var light5Diffuse = new Float32Array(3);
var light5Specular = new Float32Array(3);
var light5Ambient = new Float32Array(3);

var light6Position = new Float32Array(4);
var light6Diffuse = new Float32Array(3);
var light6Specular = new Float32Array(3);
var light6Ambient = new Float32Array(3);

//light movement variable

var vLight0 = 0.0;
var vLight1 = 0.0;
var vLight2 = 0.0;

var fLight0 = 0.0;
var fLight1 = 0.0;
var fLight2 = 0.0;
                 

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

    window.addEventListener("resize", resize_Sphere, false);

    initialize_Sphere();

    resize_Sphere();

    display_Sphere();

}

function keyDown(event)
{
    //code

    switch (event.keyCode)
    {
        case 81:
        case 113:
            uninitialize_Sphere();
            window.close();   //not browser neutral

        case 69:              //letter E for full screen
        case 101:
            toggleFullscreen();
            
            break;

        case 86:              
        case 118:
            select_SPO = 0;

            break;

        case 70:
        case 102:
            select_SPO = 1;

            break;

        case 65:
        case 97:
            

            break;

        case 76:
        case 108:
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

function initialize_Sphere()
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

    var vertexShaderSourceCode_PerVertex_Sphere =
        (
            "#version 300 es" +
            "\n" +
            "in vec4 aPosition;\n"+
		    "in vec3 aNormal;\n"+
            "uniform mat4 uModelMatrix;\n" +
            "uniform mat4 uViewMatrix;\n" +
            "uniform mat4 uProjectionMatrix;\n" +
            "uniform vec3 ulightAmbient[3];\n" +
            "uniform vec3 ulightDiffuse[3];\n" +
            "uniform vec3 ulightSpecular[3];\n" +
            "uniform vec4 ulightPosition[3];\n" +
            "uniform vec3 uMaterialAmbient;\n" +
            "uniform vec3 uMaterialDiffuse;\n" +
            "uniform vec3 uMaterialSpecular;\n" +
            "uniform float uMaterialShininess;\n" +
            "uniform highp int uKeyPress;\n" +
            "out vec3 oPhongADSLight;\n" +
            "void main(void)\n" +
            "{\n" +
            "if(uKeyPress == 1)\n" +
            "{\n" +
            "vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;\n" +
            "vec3 transformedNormals = normalize(mat3(uViewMatrix * uModelMatrix) * aNormal);\n" +
            "vec3 lightAmbient[3];\n" +
            "vec3 lightDirection[3];\n" +
            "vec3 lightDiffuse[3];\n" +
            "vec3 reflectionVector[3];\n" +
            "vec3 lightSpecular[3];\n" +
            "for(int i = 0; i < 3; i++)\n" +
            "{\n" +
            "lightAmbient[i] = ulightAmbient[i] * uMaterialAmbient;\n" +
            "lightDirection[i] = normalize(vec3 (ulightPosition[i] - eyeCoordinates));\n" +
            "lightDiffuse[i] = ulightDiffuse[i] * uMaterialDiffuse * max(dot(lightDirection[i], transformedNormals),0.0);\n" +
            "reflectionVector[i] = reflect(-lightDirection[i], transformedNormals);\n" +
            "vec3 viewerVector = normalize (- eyeCoordinates.xyz);\n" +
            "lightSpecular[i] = ulightSpecular[i] * uMaterialSpecular * pow(max(dot(reflectionVector[i], viewerVector), 0.0),uMaterialShininess);\n" +
            "oPhongADSLight = oPhongADSLight + lightAmbient[i] + lightDiffuse[i] + lightSpecular[i];\n" +
            "}\n" +
            "}\n" +
            "else\n" +
            "{\n" +
            "oPhongADSLight = vec3( 0.0, 0.0, 0.0);\n" +
            "}\n" +
            "gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;\n" +
            "}" 
        );

    var vertexShaderObject_PerVertex_Sphere = gl.createShader(gl.VERTEX_SHADER)
    gl.shaderSource(vertexShaderObject_PerVertex_Sphere, vertexShaderSourceCode_PerVertex_Sphere);

    gl.compileShader(vertexShaderObject_PerVertex_Sphere);

    if (gl.getShaderParameter(vertexShaderObject_PerVertex_Sphere, gl.COMPILE_STATUS) == false) {
        var error = gl.getShaderInfoLog(vertexShaderObject_PerVertex_Sphere);

        if (error.length > 0)
        {
            var log = "vertex shader compilation error : " + error;
            alert(log);
            uninitialize_Sphere();
        }
    }

    else
    {
        console.log("Vertex Shader Compiled Successfully !\n");

    }

    var fragmentShaderSourceCode_PerVertex_Sphere =
        (
            "#version 300 es" +
            "\n" +
            "precision highp float;" +
            "in vec3 oPhongADSLight;\n"+
		    "uniform highp int uKeyPress;\n"+
            "out vec4 FragColor;\n"+
            "void main(void)\n"+
            "{\n"+
            "if(uKeyPress == 1)\n"+
            "{\n"+
            "  FragColor = vec4 (oPhongADSLight, 1.0);\n"+
            "}\n"+
            "else\n"+
            "{\n"+
            "  FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"+
            "}\n"+
            "}"
        );

    var fragmentShaderObject_PerVertex_Sphere = gl.createShader(gl.FRAGMENT_SHADER);

    gl.shaderSource(fragmentShaderObject_PerVertex_Sphere, fragmentShaderSourceCode_PerVertex_Sphere);

    gl.compileShader(fragmentShaderObject_PerVertex_Sphere);

    if (gl.getShaderParameter(fragmentShaderObject_PerVertex_Sphere, gl.COMPILE_STATUS) == false)
    {
        var error = gl.getShaderInfoLog(fragmentShaderObject_PerVertex_Sphere);

        if (error.length > 0)
        {
            var log = "fragment shader compilation error : " + error;
            alert(log);
            uninitialize_Sphere();
        }
    }

    else
    {
        console.log("fragment Shader Compiled Successfully !\n");

    }

    //shader program

    shaderProgramObject_PerVertex_Sphere = gl.createProgram();

    gl.attachShader(shaderProgramObject_PerVertex_Sphere, vertexShaderObject_PerVertex_Sphere);
    gl.attachShader(shaderProgramObject_PerVertex_Sphere, fragmentShaderObject_PerVertex_Sphere);

    gl.bindAttribLocation(shaderProgramObject_PerVertex_Sphere, vertexAttributeEnum.AMC_ATTRIBUTE_POSITION, "aPosition");

    gl.bindAttribLocation(shaderProgramObject_PerVertex_Sphere, vertexAttributeEnum.AMC_ATTRIBUTE_NORMAL, "aNormal");

    gl.linkProgram(shaderProgramObject_PerVertex_Sphere);

    if (gl.getProgramParameter(shaderProgramObject_PerVertex_Sphere, gl.LINK_STATUS) == false)
    {
        var error = gl.getProgramInfoLog(shaderProgramObject_PerVertex_Sphere);

        if (error.length > 0)
        {
            var log = "shader program linking error : " + error;
            alert(log);
            uninitialize_Sphere();
        }
    }

    else
    {
        console.log("shader program linked Successfully !\n");
    }

    
    //mvpMatrixUniform = gl.getUniformLocation(shaderProgramObject, "uMVPMatrix");

    modelMatrixUniform_PerVertex_Sphere = gl.getUniformLocation(shaderProgramObject_PerVertex_Sphere, "uModelMatrix");

    viewMatrixUniform_PerVertex_Sphere = gl.getUniformLocation(shaderProgramObject_PerVertex_Sphere, "uViewMatrix");

    projectionMatrixUniform_PerVertex_Sphere = gl.getUniformLocation(shaderProgramObject_PerVertex_Sphere, "uProjectionMatrix");

    lightAmbientUniform_PerVertex_Sphere[0] = gl.getUniformLocation(shaderProgramObject_PerVertex_Sphere, "ulightAmbient[0]");

    lightDiffuseUniform_PerVertex_Sphere[0] = gl.getUniformLocation(shaderProgramObject_PerVertex_Sphere, "ulightDiffuse[0]");

    lightSpecularUniform_PerVertex_Sphere[0] = gl.getUniformLocation(shaderProgramObject_PerVertex_Sphere, "ulightSpecular[0]");

    lightPositionUniform_PerVertex_Sphere[0] = gl.getUniformLocation(shaderProgramObject_PerVertex_Sphere, "ulightPosition[0]");

    lightAmbientUniform_PerVertex_Sphere[1] = gl.getUniformLocation(shaderProgramObject_PerVertex_Sphere, "ulightAmbient[1]");

    lightDiffuseUniform_PerVertex_Sphere[1] = gl.getUniformLocation(shaderProgramObject_PerVertex_Sphere, "ulightDiffuse[1]");

    lightSpecularUniform_PerVertex_Sphere[1] = gl.getUniformLocation(shaderProgramObject_PerVertex_Sphere, "ulightSpecular[1]");

    lightPositionUniform_PerVertex_Sphere[1] = gl.getUniformLocation(shaderProgramObject_PerVertex_Sphere, "ulightPosition[1]");
    //
    lightAmbientUniform_PerVertex_Sphere[2] = gl.getUniformLocation(shaderProgramObject_PerVertex_Sphere, "ulightAmbient[2]");

    lightDiffuseUniform_PerVertex_Sphere[2] = gl.getUniformLocation(shaderProgramObject_PerVertex_Sphere, "ulightDiffuse[2]");

    lightSpecularUniform_PerVertex_Sphere[2] = gl.getUniformLocation(shaderProgramObject_PerVertex_Sphere, "ulightSpecular[2]");

    lightPositionUniform_PerVertex_Sphere[2] = gl.getUniformLocation(shaderProgramObject_PerVertex_Sphere, "ulightPosition[2]");


    materialAmbientUniform_PerVertex_Sphere = gl.getUniformLocation(shaderProgramObject_PerVertex_Sphere, "uMaterialAmbient");

    materialDiffuseUniform_PerVertex_Sphere = gl.getUniformLocation(shaderProgramObject_PerVertex_Sphere, "uMaterialDiffuse");


    materialSpecularUniform_PerVertex_Sphere = gl.getUniformLocation(shaderProgramObject_PerVertex_Sphere, "uMaterialSpecular");

    materialShininessUniform_PerVertex_Sphere = gl.getUniformLocation(shaderProgramObject_PerVertex_Sphere, "uMaterialShininess");

    keyPressUniform_PerVertex_Sphere = gl.getUniformLocation(shaderProgramObject_PerVertex_Sphere, "uKeyPress");


    //***************************************** Fragment Lighting Starts here **********************************/

    //vertex shader per fragment

    var vertexShaderSourceCode_PerFragment_Sphere =
        (
            "#version 300 es" +
            "\n" +
            "in vec4 aPosition;\n" +
            "in vec3 aNormal;\n" +
            "uniform mat4 uModelMatrix;\n" +
            "uniform mat4 uViewMatrix;\n" +
            "uniform mat4 uProjectionMatrix;\n" +
            "uniform vec4 ulightPosition[3];\n" +
            "uniform highp int uKeyPress;\n" +
            "out vec3 otransformedNormals;\n" +
            "out vec3 oviewerVector;\n" +
            "out vec3 olightDirection[3];\n" +
            "void main(void)\n" +
            "{\n" +
            "if(uKeyPress == 1)\n" +
            "{\n" +
            "vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;\n" +
            "otransformedNormals = (mat3(uViewMatrix * uModelMatrix) * aNormal);\n" +
            "for(int i = 0; i < 3; i++)\n" +
            "{\n" +
            "olightDirection[i] = (vec3(ulightPosition[i] - eyeCoordinates));\n" +
            "oviewerVector = (-eyeCoordinates.xyz);\n" +
            "}\n" +
            "}\n" +
            "else\n" +
            "{\n" +
            "}\n" +
            "gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;\n" +
            "}"

        );

    var vertexShaderObject_PerFragment_Sphere = gl.createShader(gl.VERTEX_SHADER)
    gl.shaderSource(vertexShaderObject_PerFragment_Sphere, vertexShaderSourceCode_PerFragment_Sphere);

    gl.compileShader(vertexShaderObject_PerFragment_Sphere);

    if (gl.getShaderParameter(vertexShaderObject_PerFragment_Sphere, gl.COMPILE_STATUS) == false)
    {
        var error = gl.getShaderInfoLog(vertexShaderObject_PerFragment_Sphere);

        if (error.length > 0) {
            var log = "vertex shader per fragment compilation error : " + error;
            alert(log);
            uninitialize_Sphere();
        }
    }

    else
    {
        console.log("Vertex Shader Compiled Successfully !\n");

    }

    var fragmentShaderSourceCode_PerFragment_Sphere =
        (
            "#version 300 es" +
            "\n" +
            "precision highp float;" +
            "in vec3 otransformedNormals;\n" +
            "in vec3 olightDirection[3];\n" +
            "in vec3 oviewerVector;\n" +
            "uniform mat4 uModelMatrix;\n" +
            "uniform mat4 uViewMatrix;\n" +
            "uniform mat4 uProjectionMatrix;\n" +
            "uniform vec3 ulightAmbient[3];\n" +
            "uniform vec3 ulightDiffuse[3];\n" +
            "uniform vec3 ulightSpecular[3];\n" +
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
            "vec3 normalizedLightDirection[3];\n" +
            "vec3 normalizedTransformedNormals = normalize(otransformedNormals);\n" +
            "vec3 normalizedViwerVector = normalize(oviewerVector);\n" +
            "vec3 lightAmbient[3];\n" +
            "vec3 lightDiffuse[3];\n" +
            "vec3 reflectionVector[3];\n" +
            "vec3 lightSpecular[3];\n" +
            "for(int i = 0; i < 3; i++)\n" +
            "{\n" +
            "lightAmbient[i] = ulightAmbient[i] * uMaterialAmbient;\n" +
            "\n" +
            "normalizedLightDirection[i] = normalize(olightDirection[i]);\n" +
            "\n" +
            "lightDiffuse[i] = ulightDiffuse[i] * uMaterialDiffuse * max(dot(normalizedLightDirection[i], normalizedTransformedNormals),0.0);\n" +
            "\n" +
            "reflectionVector[i] = reflect(-normalizedLightDirection[i], normalizedTransformedNormals);\n" +
            "\n" +
            "lightSpecular[i] = ulightSpecular[i] * uMaterialSpecular * pow(max(dot(reflectionVector[i], normalizedViwerVector), 0.0),uMaterialShininess);\n" +
            "\n" +
            "PhongADSLight = PhongADSLight + lightAmbient[i] + lightDiffuse[i] + lightSpecular[i];\n" +
            "}\n" +
            "FragColor = vec4(PhongADSLight, 1.0);\n" +
            "}\n" +
            "else\n" +
            "{\n" +
            "FragColor = vec4(1.0, 1.0, 1.0,1.0);\n" +
            "}\n" +

            "}"
        );

    var fragmentShaderObject_PerFragment_Sphere = gl.createShader(gl.FRAGMENT_SHADER);

    gl.shaderSource(fragmentShaderObject_PerFragment_Sphere, fragmentShaderSourceCode_PerFragment_Sphere);

    gl.compileShader(fragmentShaderObject_PerFragment_Sphere);

    if (gl.getShaderParameter(fragmentShaderObject_PerFragment_Sphere, gl.COMPILE_STATUS) == false)
    {
        var error = gl.getShaderInfoLog(fragmentShaderObject_PerFragment_Sphere);

        if (error.length > 0)
        {
            var log = "fragment shader per fragment compilation error : " + error;
            alert(log);
            uninitialize_Sphere();
        }
    }

    else
    {
        console.log("fragment Shader Compiled Successfully !\n");

    }


    //shader program

    shaderProgramObject_PerFragment_Sphere = gl.createProgram();

    gl.attachShader(shaderProgramObject_PerFragment_Sphere, vertexShaderObject_PerFragment_Sphere);
    gl.attachShader(shaderProgramObject_PerFragment_Sphere, fragmentShaderObject_PerFragment_Sphere);

    gl.bindAttribLocation(shaderProgramObject_PerFragment_Sphere, vertexAttributeEnum.AMC_ATTRIBUTE_POSITION, "aPosition");

    gl.bindAttribLocation(shaderProgramObject_PerFragment_Sphere, vertexAttributeEnum.AMC_ATTRIBUTE_NORMAL, "aNormal");

    gl.linkProgram(shaderProgramObject_PerFragment_Sphere);

    if (gl.getProgramParameter(shaderProgramObject_PerFragment_Sphere, gl.LINK_STATUS) == false)
    {
        var error = gl.getProgramInfoLog(shaderProgramObject_PerFragment_Sphere);

        if (error.length > 0)
        {
            var log = "shader program per fragment linking error : " + error;
            alert(log);
            uninitialize_Sphere();
        }
    }

    else
    {
        console.log("shader program linked Successfully !\n");
    }

    modelMatrixUniform_PerFragment_Sphere = gl.getUniformLocation(shaderProgramObject_PerFragment_Sphere, "uModelMatrix");

    viewMatrixUniform_PerFragment_Sphere = gl.getUniformLocation(shaderProgramObject_PerFragment_Sphere, "uViewMatrix");

    projectionMatrixUniform_PerFragment_Sphere = gl.getUniformLocation(shaderProgramObject_PerFragment_Sphere, "uProjectionMatrix");

    lightAmbientUniform_PerFragment_Sphere[0] = gl.getUniformLocation(shaderProgramObject_PerFragment_Sphere, "ulightAmbient[0]");

    lightDiffuseUniform_PerFragment_Sphere[0] = gl.getUniformLocation(shaderProgramObject_PerFragment_Sphere, "ulightDiffuse[0]");

    lightSpecularUniform_PerFragment_Sphere[0] = gl.getUniformLocation(shaderProgramObject_PerFragment_Sphere, "ulightSpecular[0]");

    lightPositionUniform_PerFragment_Sphere[0] = gl.getUniformLocation(shaderProgramObject_PerFragment_Sphere, "ulightPosition[0]");

    lightAmbientUniform_PerFragment_Sphere[1] = gl.getUniformLocation(shaderProgramObject_PerFragment_Sphere, "ulightAmbient[1]");

    lightDiffuseUniform_PerFragment_Sphere[1] = gl.getUniformLocation(shaderProgramObject_PerFragment_Sphere, "ulightDiffuse[1]");

    lightSpecularUniform_PerFragment_Sphere[1] = gl.getUniformLocation(shaderProgramObject_PerFragment_Sphere, "ulightSpecular[1]");

    lightPositionUniform_PerFragment_Sphere[1] = gl.getUniformLocation(shaderProgramObject_PerFragment_Sphere, "ulightPosition[1]");

    //

    lightAmbientUniform_PerFragment_Sphere[2] = gl.getUniformLocation(shaderProgramObject_PerFragment_Sphere, "ulightAmbient[2]");

    lightDiffuseUniform_PerFragment_Sphere[2] = gl.getUniformLocation(shaderProgramObject_PerFragment_Sphere, "ulightDiffuse[2]");

    lightSpecularUniform_PerFragment_Sphere[2] = gl.getUniformLocation(shaderProgramObject_PerFragment_Sphere, "ulightSpecular[2]");

    lightPositionUniform_PerFragment_Sphere[2] = gl.getUniformLocation(shaderProgramObject_PerFragment_Sphere, "ulightPosition[2]");


    materialAmbientUniform_PerFragment_Sphere = gl.getUniformLocation(shaderProgramObject_PerFragment_Sphere, "uMaterialAmbient");

    materialDiffuseUniform_PerFragment_Sphere = gl.getUniformLocation(shaderProgramObject_PerFragment_Sphere, "uMaterialDiffuse");


    materialSpecularUniform_PerFragment_Sphere = gl.getUniformLocation(shaderProgramObject_PerFragment_Sphere, "uMaterialSpecular");

    materialShininessUniform_PerFragment_Sphere = gl.getUniformLocation(shaderProgramObject_PerFragment_Sphere, "uMaterialShininess");

    keyPressUniform_PerFragment_Sphere = gl.getUniformLocation(shaderProgramObject_PerFragment_Sphere, "uKeyPress");



    sphere = new Mesh();
    makeSphere(sphere, 2.0, 30, 30);

    //depth initialization

    gl.clearDepth(1.0);

    gl.enable(gl.DEPTH_TEST);

    gl.depthFunc(gl.LEQUAL);
    

    // set clear color

    gl.clearColor(0.0, 0.0, 0.0, 1.0);

    //initialize color Properties

    // ************************** Light 1 Properties ****************************

    //light 1 ambient 

    light1Ambient[0] = 0.0;
    light1Ambient[1] = 0.0;
    light1Ambient[2] = 0.0;

    //light 1 diffuse

    light1Diffuse[0] = 0.0;
    light1Diffuse[1] = 1.0;
    light1Diffuse[2] = 0.0;

    //light 1 specular

    light1Specular[0] = 0.0;
    light1Specular[1] = 1.0;
    light1Specular[2] = 0.0;

    //light 1 position

    light1Position[0] = 2.0;
    light1Position[1] = 0.0;
    light1Position[2] = 0.0;
    light1Position[3] = 1.0;

    // ************************** Light 2 Properties ****************************

    //light 2 ambient 

    light2Ambient[0] = 0.0;
    light2Ambient[1] = 0.0;
    light2Ambient[2] = 0.0;

    //light 2 diffuse

    light2Diffuse[0] = 0.0;
    light2Diffuse[1] = 0.0;
    light2Diffuse[2] = 1.0;

    //light 2 specular

    light2Specular[0] = 0.0;
    light2Specular[1] = 0.0;
    light2Specular[2] = 1.0;

    //light 2 position

    light2Position[0] = -2.0;
    light2Position[1] = 0.0;
    light2Position[2] = 0.0;
    light2Position[3] = 1.0;


    // ************************** Light 3 Properties ****************************

    //light 3 ambient 

    light3Ambient[0] = 0.0;
    light3Ambient[1] = 0.0;
    light3Ambient[2] = 0.0;

    //light 3 diffuse

    light3Diffuse[0] = 1.0;
    light3Diffuse[1] = 0.0;
    light3Diffuse[2] = 0.0;

    //light 3 specular

    light3Specular[0] = 1.0;
    light3Specular[1] = 0.0;
    light3Specular[2] = 0.0;

    //light 3 position

    light3Position[0] = 0.0;
    light3Position[1] = 0.0;
    light3Position[2] = 0.0;
    light3Position[3] = 1.0;

    // ************************** Light 4 Properties ****************************

    //light 4 ambient 

    light4Ambient[0] = 0.0;
    light4Ambient[1] = 0.0;
    light4Ambient[2] = 0.0;

    //light 4 diffuse

    light4Diffuse[0] = 1.0;
    light4Diffuse[1] = 1.0;
    light4Diffuse[2] = 0.0;

    //light 4 specular

    light4Specular[0] = 1.0;
    light4Specular[1] = 1.0;
    light4Specular[2] = 0.0;

    //light 4 position

    light4Position[0] = 0.0;
    light4Position[1] = 0.0;
    light4Position[2] = 0.0;
    light4Position[3] = 1.0;

    // ************************** Light 5 Properties ****************************

    //light 5 ambient 

    light5Ambient[0] = 0.0;
    light5Ambient[1] = 0.0;
    light5Ambient[2] = 0.0;

    //light 5 diffuse

    light5Diffuse[0] = 1.0;
    light5Diffuse[1] = 0.0;
    light5Diffuse[2] = 1.0;

    //light 5 specular

    light5Specular[0] = 1.0;
    light5Specular[1] = 0.0;
    light5Specular[2] = 1.0;

    //light 5 position

    light5Position[0] = -2.0;
    light5Position[1] = 0.0;
    light5Position[2] = 0.0;
    light5Position[3] = 1.0;

    // ************************** Light 6 Properties ****************************

    //light 6 ambient 

    light6Ambient[0] = 0.0;
    light6Ambient[1] = 0.0;
    light6Ambient[2] = 0.0;

    //light 5 diffuse

    light6Diffuse[0] = 0.0;
    light6Diffuse[1] = 0.0;
    light6Diffuse[2] = 1.0;

    //light 5 specular

    light6Specular[0] = 0.0;
    light6Specular[1] = 0.0;
    light6Specular[2] = 1.0;

    //light 5 position

    light6Position[0] = 2.0;
    light6Position[1] = 0.0;
    light6Position[2] = 0.0;
    light6Position[3] = 1.0;


    //initialize projection matrix

    perspectiveProjectionMatrix_Sphere = mat4.create();

}

function resize_Sphere()
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

    mat4.perspective(perspectiveProjectionMatrix_Sphere, 45.0, parseFloat(canvas.width) / parseFloat(canvas.height), 0.1, 100.0);
    
}

function display_Sphere()
{
    //code

    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    if (select_SPO == 0)
    {
        gl.useProgram(shaderProgramObject_PerVertex_Sphere);
    }

    if (select_SPO == 1)
    {
        gl.useProgram(shaderProgramObject_PerFragment_Sphere);
    }

   

    //transformation


    var modelViewMatrix = mat4.create();

    var translationMatrix = mat4.create();

    var modelMatrix = mat4.create();

    var viewMatrix = mat4.create();

    //var modelViewProjectionMatrix = mat4.create();

    mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -5.3]);

    mat4.multiply(modelMatrix, modelMatrix, translationMatrix);

    // gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);


    if (select_SPO == 0)
    {
        gl.uniformMatrix4fv(modelMatrixUniform_PerVertex_Sphere, false, modelMatrix);

        gl.uniformMatrix4fv(viewMatrixUniform_PerVertex_Sphere, false, viewMatrix);

        gl.uniformMatrix4fv(projectionMatrixUniform_PerVertex_Sphere, false, perspectiveProjectionMatrix_Sphere);
    }

    if (select_SPO == 1)
    {
        gl.uniformMatrix4fv(modelMatrixUniform_PerFragment_Sphere, false, modelMatrix);

        gl.uniformMatrix4fv(viewMatrixUniform_PerFragment_Sphere, false, viewMatrix);

        gl.uniformMatrix4fv(projectionMatrixUniform_PerFragment_Sphere, false, perspectiveProjectionMatrix_Sphere);
    }
   


    //send uniforms

    if (select_SPO == 0)
    {
        if (bLightingEnable == true)
        {
            gl.uniform1i(keyPressUniform_PerVertex_Sphere, 1);

            gl.uniform3fv(lightAmbientUniform_PerVertex_Sphere[0], light1Ambient);

            gl.uniform3fv(lightDiffuseUniform_PerVertex_Sphere[0], light1Diffuse);

            gl.uniform3fv(lightSpecularUniform_PerVertex_Sphere[0], light1Specular);

            gl.uniform4fv(lightPositionUniform_PerVertex_Sphere[0], light1Position);

            gl.uniform3fv(lightAmbientUniform_PerVertex_Sphere[1], light2Ambient);

            gl.uniform3fv(lightDiffuseUniform_PerVertex_Sphere[1], light2Diffuse);

            gl.uniform3fv(lightSpecularUniform_PerVertex_Sphere[1], light2Specular);

            gl.uniform4fv(lightPositionUniform_PerVertex_Sphere[1], light2Position);
            //
            gl.uniform3fv(lightAmbientUniform_PerVertex_Sphere[2], light3Ambient);

            gl.uniform3fv(lightDiffuseUniform_PerVertex_Sphere[2], light3Diffuse);

            gl.uniform3fv(lightSpecularUniform_PerVertex_Sphere[2], light3Specular);

            gl.uniform4fv(lightPositionUniform_PerVertex_Sphere[2], light3Position);

            gl.uniform3fv(materialAmbientUniform_PerVertex_Sphere, materialAmbient);

            gl.uniform3fv(materialDiffuseUniform_PerVertex_Sphere, materialDiffuse);

            gl.uniform3fv(materialSpecularUniform_PerVertex_Sphere, materialSpecular);

            gl.uniform1f(materialShininessUniform_PerVertex_Sphere, materialShininess);

        }
        else
        {
            gl.uniform1i(keyPressUniform_PerVertex_Sphere, 0);
        }
    }

    if (select_SPO == 1)
    {
        if (bLightingEnable == true)
        {
            gl.uniform1i(keyPressUniform_PerFragment_Sphere, 1);

            gl.uniform3fv(lightAmbientUniform_PerFragment_Sphere[0], light4Ambient);

            gl.uniform3fv(lightDiffuseUniform_PerFragment_Sphere[0], light4Diffuse);

            gl.uniform3fv(lightSpecularUniform_PerFragment_Sphere[0], light4Specular);

            gl.uniform4fv(lightPositionUniform_PerFragment_Sphere[0], light4Position);

            gl.uniform3fv(lightAmbientUniform_PerFragment_Sphere[1], light5Ambient);

            gl.uniform3fv(lightDiffuseUniform_PerFragment_Sphere[1], light5Diffuse);

            gl.uniform3fv(lightSpecularUniform_PerFragment_Sphere[1], light5Specular);

            gl.uniform4fv(lightPositionUniform_PerFragment_Sphere[1], light5Position);

            gl.uniform3fv(lightAmbientUniform_PerFragment_Sphere[2], light6Ambient);

            gl.uniform3fv(lightDiffuseUniform_PerFragment_Sphere[2], light6Diffuse);

            gl.uniform3fv(lightSpecularUniform_PerFragment_Sphere[2], light6Specular);

            gl.uniform4fv(lightPositionUniform_PerFragment_Sphere[2], light6Position);

            gl.uniform3fv(materialAmbientUniform_PerFragment_Sphere, materialAmbient);

            gl.uniform3fv(materialDiffuseUniform_PerFragment_Sphere, materialDiffuse);

            gl.uniform3fv(materialSpecularUniform_PerFragment_Sphere, materialSpecular);

            gl.uniform1f(materialShininessUniform_PerFragment_Sphere, materialShininess);

        }
        else
        {
            gl.uniform1i(keyPressUniform_PerFragment_Sphere, 0);
        }
    }

   





    sphere.draw();

    gl.useProgram(null);

   
   
    update_Sphere();
    

   

    //double buffering

    requestAnimationFrame(display_Sphere, canvas);
    
}

function update_Sphere()
{

    if (bLightingEnable == true)
    {
        if (select_SPO == 0)
        {
            if (vLight0 < (2 * Math.PI))
            {
                //light 0    //x rotation
                light1Position[0] = 0.0;
                light1Position[1] = 2.0 * Math.sin(vLight0);
                light1Position[2] = (-2.0) + (2.0 * Math.cos(vLight0));
                light1Position[3] = 1.0;

                //fprintf(gpFile, "value of position[1] = %f", light[0].position[1]);

                //light 1   //y rotation

                light2Position[0] = 2.0 * Math.sin(vLight1);
                light2Position[1] = 0.0;
                light2Position[2] = (-2.0) + (2.0 * Math.cos(vLight1));
                light2Position[3] = 1.0;

                //light 2   //Z rotation

                light3Position[0] = 2.0 * Math.sin(vLight2);
                light3Position[1] = 2.0 * Math.cos(vLight2);
                light3Position[2] = 0.0;
                light3Position[3] = 1.0;

                vLight0 = vLight0 + 0.004;
                vLight1 = vLight1 + 0.004;
                vLight2 = vLight2 + 0.004;
            }

            else
            {
                vLight0 = 0.0;
                vLight1 = 0.0;
                vLight2 = 0.0;
            }
            
        }

        if (select_SPO == 1)
        {
            if (fLight0 < (2 * Math.PI))
            {
                //light 0    //x rotation
                light4Position[0] = 0.0;
                light4Position[1] = 2.0 * Math.sin(fLight0);
                light4Position[2] = (-2.0) + (2.0 * Math.cos(fLight0));
                light4Position[3] = 1.0;

                //fprintf(gpFile, "value of position[1] = %f", light[0].position[1]);

                //light 1   //y rotation

                light5Position[0] = 2.0 * Math.sin(fLight1);
                light5Position[1] = 0.0;
                light5Position[2] = (-2.0) + (2.0 * Math.cos(fLight1));
                light5Position[3] = 1.0;

                //light 2   //Z rotation

                light6Position[0] = 2.0 * Math.sin(fLight2);
                light6Position[1] = 2.0 * Math.cos(fLight2);
                light6Position[2] = 0.0;
                light6Position[3] = 1.0;

                fLight0 = fLight0 + 0.004;
                fLight1 = fLight1 + 0.004;
                fLight2 = fLight2 + 0.004;
            }

            else
            {
                fLight0 = 0.0;
                fLight1 = 0.0;
                fLight2 = 0.0;
            }

        }

    }

    //code
}

function uninitialize_Sphere()
{

    if (shaderProgramObject_PerVertex_Sphere)  //this statement can be shaderProgramObject != null
    {
        gl.useProgram(shaderProgramObject_PerVertex_Sphere);

        var shaderObjects = gl.getAttachedShaders(shaderProgramObject_PerVertex_Sphere);

        if (shaderObjects && shaderObjects.length > 0)
        {
            for (let i = 0; i < shaderObjects.length; i++)
            {
                gl.detachShader(shaderProgramObject_PerVertex_Sphere, shaderObjects[i]);
                gl.deleteShader(shaderObjects[i]);
                shaderObjects[i] = null;

            }


        }

        gl.useProgram(null);
        gl.deleteProgram(shaderProgramObject_PerVertex_Sphere);
        shaderProgramObject_PerVertex_Sphere = null;


    }

    if (shaderProgramObject_PerFragment_Sphere)  //this statement can be shaderProgramObject != null
    {
        gl.useProgram(shaderProgramObject_PerFragment_Sphere);

        var shaderObjects = gl.getAttachedShaders(shaderProgramObject_PerFragment_Sphere);

        if (shaderObjects && shaderObjects.length > 0) {
            for (let i = 0; i < shaderObjects.length; i++) {
                gl.detachShader(shaderProgramObject_PerFragment_Sphere, shaderObjects[i]);
                gl.deleteShader(shaderObjects[i]);
                shaderObjects[i] = null;

            }


        }

        gl.useProgram(null);
        gl.deleteProgram(shaderProgramObject_PerFragment_Sphere);
        shaderProgramObject_PerFragment_Sphere = null;


    }

    if (sphere)
    {
        sphere.deallocate();
        sphere = null;
    }



    

    
}





